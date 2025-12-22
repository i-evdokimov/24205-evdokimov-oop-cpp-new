#pragma once
#include <string>
#include <vector>

class TextProcessor {
public:
    static std::vector<std::string> extractWords(const std::string& text);
    static std::string normalizeWord(const std::string& word);
    static bool isWordChar(char c);

private:
    TextProcessor() = default;
};