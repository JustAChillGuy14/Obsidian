#include "frontend/lexer.hpp"
#include <unordered_map>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

static const std::unordered_map<std::string_view, lexer::TokenType> keywords = {
    {"return", lexer::TokenType::Return},
};


const char *lexer::tokentype_str(lexer::TokenType kind)
{
    switch (kind)
    {
    case TokenType::Add:
        return "`Add`";
    case TokenType::Sub:
        return "`Sub`";
    case TokenType::Mul:
        return "`Mul`";
    case TokenType::Div:
        return "`Div`";
    case TokenType::Mod:
        return "`Mod`";
    case TokenType::Integer:
        return "`Integer`";
    case TokenType::Identifier:
        return "`Identifier`";
    case TokenType::Return:
        return "`Return`";
    case TokenType::SemiColon:
        return "`SemiColon`";
    case TokenType::_EOF:
        return "`EOF`";
    default:
        std::cerr << "Exhaustive handling of TokenType in tokentype_str" << std::endl;
        exit(EXIT_FAILURE);
    }
}

lexer::Token lexer::token(TokenType kind, std::string_view value, uint32_t row, uint32_t col)
{
    return Token{
        kind,
        value,
        Position{
            row,
            col,
        },
    };
}
lexer::Lexer::Lexer() : idx(0) {}

lexer::Lexer::Lexer(std::string_view _src) : src(_src), idx(0) {}

bool lexer::Lexer::not_end(uint32_t offset)
{
    return idx + offset < src.length();
}

/*
Can error
*/
char lexer::Lexer::peek(uint32_t offset)
{
    return src[idx + offset];
}

/*
Can error
*/
char lexer::Lexer::consume()
{
    return src[idx++];
}

std::vector<lexer::Token> lexer::Lexer::tokenize(std::string_view file_name,bool &failure)
{
    failure = false;
    /*
    Ensure idx = 0
    */
    idx = 0;

    std::vector<Token> ret;

    uint32_t row, col;
    row = col = 0;

    while (not_end())
    {
        if (isalpha(peek()) || peek() == '_')
        {
            uint32_t start = idx;

            while (not_end() && (isalnum(peek()) || peek() == '_'))
            {
                consume();
                col++;
            }

            uint32_t end = idx;

            std::string_view ident(src.data() + start, end - start);
            if (auto it = keywords.find(ident); it != keywords.end())
            {
                ret.push_back(token((it->second), std::string_view{}, row, col));
            }
            else
            {
                ret.push_back(token(TokenType::Identifier, ident, row, col));
            }
        }
        else if (isdigit(peek()))
        {
            uint32_t start = idx;

            while (not_end() && isdigit(peek())) 
            {
                consume();
                col++;
            }

            uint32_t end = idx;
            ret.push_back(token(TokenType::Integer, std::string_view(src.data() + start, end - start), row, col));
        }
        else if (peek() == ';')
        {
            consume();
            ret.push_back(token(TokenType::SemiColon, std::string_view{}, row, col++)); // Had to do what i had to go i guess...
        }
        else if (peek() == '+')
        {
            consume();
            ret.push_back(token(TokenType::Add, std::string_view{}, row, col++)); // Had to do what i had to go i guess...
        }
        else if (peek() == '-')
        {
            consume();
            ret.push_back(token(TokenType::Sub, std::string_view{}, row, col++)); // Had to do what i had to go i guess...
        }
        else if (peek() == '*')
        {
            consume();
            ret.push_back(token(TokenType::Mul, std::string_view{}, row, col++)); // Had to do what i had to go i guess...
        }
        else if (peek() == '/')
        {
            consume();
            ret.push_back(token(TokenType::Div, std::string_view{}, row, col++)); // Had to do what i had to go i guess...
        }
        else if (peek() == '%')
        {
            consume();
            ret.push_back(token(TokenType::Mod, std::string_view{}, row, col++)); // Had to do what i had to go i guess...
        }
        else if (peek() == '\n')
        {
            row++;
            col = 0;
            consume();
        }
        else if (isspace(peek()))
        {
            col++;
            consume();
        }
        else
        {
            std::stringstream err;
            err << file_name << ":" << row + 1 << ":" << col + 1 << ": ERROR: Unrecognized character found in source: " << peek() << " ASCII value: " << (int)peek();
            failure = true;
            errors.push_back(err.str());
            consume();
            col++;
        }
    }

    ret.push_back(token(TokenType::_EOF, std::string_view{}, row, col)); // row,col are basically unused lol
    return ret;
}

std::vector<lexer::Token> lexer::Lexer::tokenize(std::string_view src, std::string_view file_name, bool &failure)
{
    this->src = src;
    return tokenize(file_name, failure);
}

std::vector<std::string> lexer::Lexer::get_errors()
{
    return errors;
}