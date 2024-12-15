add_library(XMLTokenizer SHARED
        tokenizer.cpp
)

target_include_directories(XMLTokenizer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
