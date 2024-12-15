#include <iostream>

#include "tokenizer.h"

void print_tokens(const std::vector<Token> &tokens) {
    for (const auto &token: tokens) {
        std::cout << token << std::endl;
    }
    std::cout << std::endl;
}

int main() {
    XMLTokenizer tokenizer(R"(<?xml version="1.0" encoding="UTF-8"?>
    <data1>
    <item> <name>Item One</name> <description>This is the first item</description> </item>
    <item> <name>Item Two</name> <description>This is the second item</description> </item>
    </data1>)"
    );
    print_tokens(tokenizer.tokenize());

    tokenizer.initFromFile("../example_files/library.xml");
    print_tokens(tokenizer.tokenize());

    tokenizer.reset(R"(<?xml version="1.0" encoding="UTF-8"?>
    <data2>
    <entry><title>Entry One</title><content>This is the first entry</content></entry>
    <entry><title>Entry Two</title><content>This is the second entry</content></entry>
    </data2>)"
    );
    print_tokens(tokenizer.tokenize());

    tokenizer.initFromFile("../example_files/reminder.xml");
    print_tokens(tokenizer.tokenize());

    return 0;
}