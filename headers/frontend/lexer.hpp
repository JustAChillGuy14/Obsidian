#pragma once

#include <string>
#include <vector>

namespace lexer
{
    enum class TokenType
    {
        /*
        Operations
        */
        Add,
        Mul,
        Div,
        Mod,
        Sub,

        /*
        Literals/Terms
        */
        Integer,
        Identifier,

        /*
        Line/File Enders
        */
        SemiColon,
        _EOF,
    };

    const char *tokentype_str(TokenType kind);

    struct Position
    {
        uint32_t row;
        uint32_t col;
    };

    struct Token
    {
        TokenType kind;
        std::string_view value;
        Position pos;
    };

    Token token(TokenType kind, std::string_view value, uint32_t start, uint32_t end);

    class Lexer
    {
    private:
        std::string_view src;
        uint32_t idx;
        std::vector<std::string> errors;
        char peek(uint32_t offset = 0);
        bool not_end(uint32_t offset = 0);
        char consume();

    public:
        Lexer(std::string_view _src);
        Lexer();
        std::vector<Token> tokenize(std::string_view file_name, bool &failure);
        std::vector<Token> tokenize(std::string_view src, std::string_view file_name, bool &failure);
        std::vector<std::string> get_errors();
    };

} // namespace lexer