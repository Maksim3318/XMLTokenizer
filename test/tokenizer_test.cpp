#include <gtest/gtest.h>
#include "tokenizer.h"

TEST(XMLTokenizerTest, Prolog) {
    std::string xml = R"(<?xml version="1.0" encoding="UTF-8"?>)";
    XMLTokenizer tokenizer(xml);
    auto tokens = tokenizer.tokenize();
    ASSERT_EQ(tokens.size(), 1);
    EXPECT_EQ(tokens[0].type, TokenType::PROLOG);
    EXPECT_EQ(tokens[0].content, xml);
}

TEST(XMLTokenizerTest, Doctype) {
    std::string xml = "<!DOCTYPE library SYSTEM \"Library.dtd\">";
    XMLTokenizer tokenizer(xml);
    auto tokens = tokenizer.tokenize();
    ASSERT_EQ(tokens.size(), 1);
    EXPECT_EQ(tokens[0].type, TokenType::DOCTYPE);
    EXPECT_EQ(tokens[0].content, xml);
}

TEST(XMLTokenizerTest, OpenTag) {
    std::string token_content = "library";
    std::string xml = "<" + token_content + ">";
    XMLTokenizer tokenizer(xml);
    auto tokens = tokenizer.tokenize();
    ASSERT_EQ(tokens.size(), 1);
    EXPECT_EQ(tokens[0].type, TokenType::OPEN_TAG);
    EXPECT_EQ(tokens[0].content, token_content);
}

TEST(XMLTokenizerTest, CloseTag) {
    std::string token_content = "library";
    std::string xml = "</" + token_content + ">";
    XMLTokenizer tokenizer(xml);
    auto tokens = tokenizer.tokenize();
    ASSERT_EQ(tokens.size(), 1);
    EXPECT_EQ(tokens[0].type, TokenType::CLOSE_TAG);
    EXPECT_EQ(tokens[0].content, token_content);
}

TEST(XMLTokenizerTest, SelfClosingTag) {
    std::string token_content = "book";
    std::string xml = "<" + token_content + "/>";
    XMLTokenizer tokenizer(xml);
    auto tokens = tokenizer.tokenize();
    ASSERT_EQ(tokens.size(), 1);
    EXPECT_EQ(tokens[0].type, TokenType::SELF_CLOSING_TAG);
    EXPECT_EQ(tokens[0].content, token_content);
}

TEST(XMLTokenizerTest, Text) {
    std::string xml = "Hello, World!";
    XMLTokenizer tokenizer(xml);
    auto tokens = tokenizer.tokenize();
    ASSERT_EQ(tokens.size(), 1);
    EXPECT_EQ(tokens[0].type, TokenType::TEXT);
    EXPECT_EQ(tokens[0].content, xml);
}

TEST(XMLTokenizerTest, Comment) {
    std::string xml = "<!-- This is a comment -->";
    XMLTokenizer tokenizer(xml);
    auto tokens = tokenizer.tokenize();
    ASSERT_EQ(tokens.size(), 1);
    EXPECT_EQ(tokens[0].type, TokenType::COMMENT);
    EXPECT_EQ(tokens[0].content, xml);
}

TEST(XMLTokenizerTest, CData) {
    std::string xml = "<![CDATA[Some unparsed <data> here]]>";
    XMLTokenizer tokenizer(xml);
    auto tokens = tokenizer.tokenize();
    ASSERT_EQ(tokens.size(), 1);
    EXPECT_EQ(tokens[0].type, TokenType::CDATA);
    EXPECT_EQ(tokens[0].content, xml);
}

TEST(XMLTokenizerTest, TokenizeEmpty) {
    XMLTokenizer tokenizer;
    auto tokens = tokenizer.tokenize();
    ASSERT_EQ(tokens.size(), 0);
}

TEST(XMLTokenizerTest, RepeatTokenize) {
    std::string xml = "Hello, World!";
    XMLTokenizer tokenizer(xml);
    auto tokens = tokenizer.tokenize();
    tokens.clear();
    tokens = tokenizer.tokenize();
    ASSERT_EQ(tokens.size(), 0);
}

TEST(XMLTokenizerTest, Reset) {
    std::string xml = "Hello, World!";
    XMLTokenizer tokenizer(xml);
    auto tokens = tokenizer.tokenize();
    tokens.clear();
    tokenizer.reset(xml);
    tokens = tokenizer.tokenize();
    ASSERT_EQ(tokens.size(), 1);
    EXPECT_EQ(tokens[0].type, TokenType::TEXT);
    EXPECT_EQ(tokens[0].content, xml);
}

TEST(XMLTokenizerTest, NestedTags) {
    std::string xml = "<library><book><title>XML Parsing</title></book></library>";
    XMLTokenizer tokenizer(xml);
    auto tokens = tokenizer.tokenize();
    ASSERT_EQ(tokens.size(), 7);
    EXPECT_EQ(tokens[0].type, TokenType::OPEN_TAG);
    EXPECT_EQ(tokens[1].type, TokenType::OPEN_TAG);
    EXPECT_EQ(tokens[2].type, TokenType::OPEN_TAG);
    EXPECT_EQ(tokens[3].type, TokenType::TEXT);
    EXPECT_EQ(tokens[4].type, TokenType::CLOSE_TAG);
    EXPECT_EQ(tokens[5].type, TokenType::CLOSE_TAG);
    EXPECT_EQ(tokens[6].type, TokenType::CLOSE_TAG);
}

TEST(XMLTokenizerTest, MixedContent) {
    std::string xml = "<book>Title: <title>XML Parsing</title></book>";
    XMLTokenizer tokenizer(xml);
    auto tokens = tokenizer.tokenize();
    ASSERT_EQ(tokens.size(), 6);
    EXPECT_EQ(tokens[0].type, TokenType::OPEN_TAG);
    EXPECT_EQ(tokens[1].type, TokenType::TEXT);
    EXPECT_EQ(tokens[2].type, TokenType::OPEN_TAG);
    EXPECT_EQ(tokens[3].type, TokenType::TEXT);
    EXPECT_EQ(tokens[4].type, TokenType::CLOSE_TAG);
    EXPECT_EQ(tokens[5].type, TokenType::CLOSE_TAG);
}

TEST(XMLTokenizerTest, SpecialCharacters) {
    std::string xml = "<book>XML &amp; Parsing</book>";
    XMLTokenizer tokenizer(xml);
    std::vector<Token> tokens = tokenizer.tokenize();
    ASSERT_EQ(tokens.size(), 3);
    EXPECT_EQ(tokens[0].type, TokenType::OPEN_TAG);
    EXPECT_EQ(tokens[1].type, TokenType::TEXT);
    EXPECT_EQ(tokens[2].type, TokenType::CLOSE_TAG);
}

TEST(XMLTokenizerTest, IncompleteXML) {
    std::string xml = "<book><title>XML Parsing</book>";
    XMLTokenizer tokenizer(xml);
    std::vector<Token> tokens = tokenizer.tokenize();
    ASSERT_EQ(tokens.size(), 4);
    EXPECT_EQ(tokens[0].type, TokenType::OPEN_TAG);
    EXPECT_EQ(tokens[1].type, TokenType::OPEN_TAG);
    EXPECT_EQ(tokens[2].type, TokenType::TEXT);
    EXPECT_EQ(tokens[3].type, TokenType::CLOSE_TAG);
}

TEST(XMLTokenizerTest, FileSameAsString) {
    XMLTokenizer tokenizer;
    tokenizer.initFromFile("../example_files/reminder.xml");
    auto file_tokens = tokenizer.tokenize();

    std::string xml = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
                      "<!DOCTYPE note SYSTEM \"Note.dtd\">\n"
                      "<note>\n"
                      "    <to>John</to>\n"
                      "    <from>Jane</from>\n"
                      "    <![CDATA[Some unparsed <data>]]>\n"
                      "    <heading>Reminder</heading>\n"
                      "    <body>Don't forget the meeting at 3 PM today!</body>\n"
                      "    <end/>\n"
                      "    <!-- Some comment not to sending -->\n"
                      "</note>";
    tokenizer.reset(xml);
    auto string_tokens = tokenizer.tokenize();
    EXPECT_EQ(file_tokens, string_tokens);
}

TEST(XMLTokenizerTest, FileExceptions) {
    XMLTokenizer tokenizer;

    EXPECT_THROW(tokenizer.initFromFile("file"), std::runtime_error);
    try {
        tokenizer.initFromFile("file");
    } catch (const std::runtime_error &e) {
        EXPECT_EQ(std::string_view(e.what()), "file doesn't exists");
    }

    EXPECT_THROW(tokenizer.initFromFile("gtest"), std::runtime_error);
    try {
        tokenizer.initFromFile("gtest");
    } catch (const std::runtime_error &e) {
        EXPECT_EQ(std::string_view(e.what()), "gtest isn't a file");
    }

    EXPECT_THROW(tokenizer.initFromFile("tokenizer_test.cpp"), std::runtime_error);
    try {
        tokenizer.initFromFile("tokenizer_test.cpp");
    } catch (const std::runtime_error &e) {
        EXPECT_EQ(std::string_view(e.what()), "tokenizer_test.cpp isn't .xml file");
    }
}

TEST(XMLTokenizerTest, CorrectMultipleTokens) {
    XMLTokenizer tokenizer;
    tokenizer.initFromFile("../example_files/library.xml");
    auto tokens = tokenizer.tokenize();
    std::unordered_map<TokenType, size_t> counter;
    for (const auto &token: tokens) {
        counter[token.type]++;
    }
    std::unordered_map<TokenType, size_t> expected = {
            {TokenType::PROLOG,           1},
            {TokenType::DOCTYPE,          1},
            {TokenType::OPEN_TAG,         61},
            {TokenType::CLOSE_TAG,        61},
            {TokenType::SELF_CLOSING_TAG, 10},
            {TokenType::TEXT,             60},
            {TokenType::COMMENT,          4},
            {TokenType::CDATA,            1}
    };
    EXPECT_EQ(counter, expected);
}