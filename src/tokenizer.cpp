#include <stdexcept>
#include <iomanip>
#include <fstream>
#include "tokenizer.h"

XMLTokenizer::XMLTokenizer(const std::string &xml) : xml(xml), position(0) {}

std::vector<Token> XMLTokenizer::tokenize() {
    std::vector<Token> tokens;
    while (position < xml.size()) {
        char ch = nextChar();
        if (ch == '<') {
            if (peekChar() == '?') {
                tokens.push_back(extractProlog());
            } else if (peekChar() == '!') {
                if (xml.substr(position + 1, 2) == "--") {
                    tokens.push_back(extractComment());
                } else if (xml.substr(position + 1, 7) == "[CDATA[") {
                    tokens.push_back(extractCDATA());
                } else {
                    tokens.push_back(extractDoctype());
                }
            } else if (peekChar() == '/') {
                tokens.push_back(extractCloseTag());
            } else {
                tokens.push_back(extractOpenOrSelfClosingTag());
            }
        } else if (!isspace(ch)) {
            tokens.push_back(extractText(ch));
        }
    }
    return tokens;
}

char XMLTokenizer::nextChar() {
    if (position < xml.size()) {
        return xml[position++];
    }
    return '\0';
}

char XMLTokenizer::peekChar() {
    if (position < xml.size()) {
        return xml[position];
    }
    return '\0';
}

Token XMLTokenizer::extractProlog() {
    size_t endPos = xml.find("?>", position) + 2;
    std::string prolog = xml.substr(position - 1, endPos - position + 1);
    position = endPos;
    return {TokenType::PROLOG, prolog};
}

Token XMLTokenizer::extractComment() {
    size_t endPos = xml.find("-->", position) + 3;
    std::string comment = xml.substr(position - 1, endPos - position + 1);
    position = endPos;
    return {TokenType::COMMENT, comment};
}

Token XMLTokenizer::extractCDATA() {
    size_t endPos = xml.find("]]>", position) + 3;
    std::string cdata = xml.substr(position - 1, endPos - position + 1);
    position = endPos;
    return {TokenType::CDATA, cdata};
}

Token XMLTokenizer::extractDoctype() {
    size_t endPos = xml.find('>', position) + 1;
    std::string doctype = xml.substr(position - 1, endPos - position + 1);
    position = endPos;
    return {TokenType::DOCTYPE, doctype};
}

Token XMLTokenizer::extractCloseTag() {
    position++;  // Skip '/'
    std::string tagName = extractTagName();
    size_t endPos = xml.find('>', position) + 1;
    position = endPos;
    return {TokenType::CLOSE_TAG, tagName};
}

Token XMLTokenizer::extractOpenOrSelfClosingTag() {
    std::string tagName = extractTagName();
    while (peekChar() != '>' && peekChar() != '/') {
        nextChar();
    }

    if (peekChar() == '/') {
        position++;
        if (peekChar() == '>') {
            position++;
            return {TokenType::SELF_CLOSING_TAG, tagName};
        }
    }

    size_t endPos = xml.find('>', position) + 1;
    position = endPos;
    return {TokenType::OPEN_TAG, tagName};
}

std::string XMLTokenizer::extractTagName() {
    std::string tagName;
    while (peekChar() != ' ' && peekChar() != '>' && peekChar() != '/' && peekChar() != '\0') {
        tagName += nextChar();
    }
    return tagName;
}

Token XMLTokenizer::extractText(char ch) {
    std::string text(1, ch);
    while (peekChar() != '<' && peekChar() != '\0') {
        text += nextChar();
    }
    return {TokenType::TEXT, text};
}

void XMLTokenizer::initFromFile(const std::filesystem::path& path) {
    if (!std::filesystem::exists(path)) {
        throw std::runtime_error(path.string() + " doesn't exists");
    }
    if (!std::filesystem::is_regular_file(path)) {
        throw std::runtime_error(path.string() + " isn't a file");
    }
    if (path.extension() != ".xml") {
        throw std::runtime_error(path.string() + " isn't .xml file");
    }
    std::ifstream file(path.string());
    reset(std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()));
}

void XMLTokenizer::reset(const std::string &new_xml) {
    xml = new_xml;
    position = 0;
}

XMLTokenizer::XMLTokenizer() = default;

std::ostream &operator<<(std::ostream &os, const Token &token) {
    os << std::string_view("TokenType: ") << std::setw(17);

    switch (token.type) {
        case TokenType::PROLOG: {
            os << std::string_view("PROLOG");
            break;
        }
        case TokenType::DOCTYPE: {
            os << std::string_view("DOCTYPE");
            break;
        }
        case TokenType::OPEN_TAG: {
            os << std::string_view("OPEN_TAG");
            break;
        }
        case TokenType::CLOSE_TAG: {
            os << std::string_view("CLOSE_TAG");
            break;
        }
        case TokenType::SELF_CLOSING_TAG: {
            os << std::string_view("SELF_CLOSING_TAG");
            break;
        }
        case TokenType::TEXT: {
            os << std::string_view("TEXT");
            break;
        }
        case TokenType::COMMENT: {
            os << std::string_view("COMMENT");
            break;
        }
        case TokenType::CDATA: {
            os << std::string_view("CDATA");
            break;
        }
        default: {
            throw std::runtime_error("Invalid TokenType");
        }
    }

    return os << std::string_view(", data: ") << token.content;
}

bool operator==(const Token &lhs, const Token &rhs) {
    return lhs.type == rhs.type && lhs.content == rhs.content;
}
