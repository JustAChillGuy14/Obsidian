#include <iostream>
#include <cstring>
#include <fstream>
#include <sstream>

#include "frontend/lexer.hpp"

bool match_prefix(std::string_view arg, std::string_view full)
{
    if (arg.size() > full.size())
        return false; // can't be longer
    return arg == full.substr(0, arg.size());
}

std::string read_entire_file_string(std::string filename, bool &failed)
{
    std::ifstream file(filename); // Opens in text mode by default

    if (!file.is_open())
    {
        failed = true;
        return "";
    }

    // Use a stringstream to efficiently read the entire file buffer
    std::stringstream buffer;
    buffer << file.rdbuf();

    failed = false;
    // Convert the stringstream buffer to a standard string
    return buffer.str();
}

typedef uint8_t FlagType;

#define TOKEN_FLAG 1

void usage(const char *const name)
{
    std::cout << name << " [options] [output file] [source file]" << std::endl;
    std::cout << "options:" << std::endl;
    std::cout << "    -tokens dump tokens and stop immediately after tokenization." << std::endl;
    std::cout << "    --help print usage." << std::endl;
}

int main(int argc, const char *const *const argv) // the argv is just for the funsies
{
    if (argc < 2)
    {
        usage(argv[0]);
        exit(EXIT_FAILURE);
    }

    bool input_file_set = false;
    bool output_file_set = false;
    bool flagend = false;
    FlagType flags = 0;

    std::string input_file_name;
    std::string output_file_name;
    for (int i = 1; i < argc; i++)
    {
        const char *const arg = argv[i];
        if (!flagend && arg[0] == '-')
        {
            if (match_prefix(arg, "-tokens"))
                flags |= TOKEN_FLAG;
            else if (match_prefix(arg, "--help"))
            {
                usage(argv[0]);
                return 0;
            }
            else
            {
                std::cerr << "Unknown option `" << arg << "`\n";
                return 1;
            }
        }
        else
        {
            if (input_file_set)
            {
                std::cerr << "Too many files provided." << std::endl;
                exit(EXIT_FAILURE);
            }
            if (output_file_set)
            {
                input_file_set = true;
                input_file_name = arg;
            }
            else
            {
                output_file_set = true;
                output_file_name = arg;
            }
        }
    }

    if (!input_file_set)
    {
        std::cerr << "Provide at least 2 files output file and source file" << std::endl;
        exit(EXIT_FAILURE);
    }

    bool failure;
    
    std::string src = read_entire_file_string(input_file_name, failure);
    if (failure)
    {
        std::cerr << "Error opening file: " << input_file_name << std::endl;
        exit(EXIT_FAILURE);
    }

    lexer::Lexer l(src);
    std::vector<lexer::Token> tokens = l.tokenize(input_file_name, failure);

    if (failure)
    {
        std::vector<std::string> errors = l.get_errors();
        for (auto &&i : errors)
        {
            std::cerr << i << std::endl;
        }
        exit(EXIT_FAILURE);
    }

    if (flags & TOKEN_FLAG)
    {
        std::cout << "[\n";
        for (auto &&token : tokens)
        {
            std::cout << "{kind: " << lexer::tokentype_str(token.kind) << ", value: `" << token.value << "`}\n";
        }
        std::cout << "]" << std::endl; // No endl in loop,cuz that breaks the whole point
        return 0;
    }
}