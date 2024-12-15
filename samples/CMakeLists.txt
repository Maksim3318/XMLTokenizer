add_executable(XMLTokenizerSamples
        tokenizer_sample.cpp
)

target_link_libraries(XMLTokenizerSamples PRIVATE XMLTokenizer)

target_include_directories(XMLTokenizerSamples PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../src)
