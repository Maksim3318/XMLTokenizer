#pragma once

#include <string>
#include <vector>
#include <filesystem>

enum class TokenType : uint8_t {
    PROLOG,
    DOCTYPE,
    OPEN_TAG,
    CLOSE_TAG,
    SELF_CLOSING_TAG,
    TEXT,
    COMMENT,
    CDATA
};

struct Token {
    TokenType type;
    std::string content;
};

bool operator==(const Token &lhs, const Token &rhs);

std::ostream &operator<<(std::ostream &os, const Token &token);

class XMLTokenizer {
public:
    XMLTokenizer();

    XMLTokenizer(const XMLTokenizer &) = delete;

    explicit XMLTokenizer(const std::string &xml);

    void initFromFile(const std::filesystem::path &path);

    std::vector<Token> tokenize();

    void reset(const std::string &new_xml);

private:
    std::string xml;
    size_t position = 0;

    char nextChar();

    char peekChar();

    Token extractProlog();

    Token extractComment();

    Token extractCDATA();

    Token extractDoctype();

    Token extractCloseTag();

    Token extractOpenOrSelfClosingTag();

    std::string extractTagName();

    Token extractText(char ch);
};
