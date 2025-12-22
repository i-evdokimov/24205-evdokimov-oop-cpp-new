#include "TextProcessor.h"
#include <cctype>
#include <algorithm>
#include <functional>

bool TextProcessor::isWordChar(char c) {
    return std::isalnum(static_cast<unsigned char>(c)) || (static_cast<unsigned char>(c) & 0x80);
}

std::string TextProcessor::normalizeWord(const std::string& word) {
    std::string normalized = word;
    for (auto& ch : normalized) {
        if ((unsigned char)ch < 128) {
            ch = std::tolower(ch);
        }
    }
    return normalized;
}

std::vector<std::string> TextProcessor::extractWords(const std::string& text) {
    std::vector<std::string> words;
    std::string currentWord;

    for (size_t i = 0; i < text.size(); ++i) {
        unsigned char c = text[i];
        if (isWordChar(c)) {
            currentWord += c;
        } else if (!currentWord.empty()) {
            words.push_back(normalizeWord(currentWord));
            currentWord.clear();
        }
    }

    if (!currentWord.empty()) {
        words.push_back(normalizeWord(currentWord));
    }

    return words;
}