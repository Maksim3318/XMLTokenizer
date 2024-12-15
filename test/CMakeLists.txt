add_executable(XMLTokenizerTest
        tokenizer_test.cpp
)

add_subdirectory(gtest)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

target_link_libraries(XMLTokenizerTest PRIVATE XMLTokenizer gtest gtest_main)
target_include_directories(XMLTokenizerTest PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../src)
