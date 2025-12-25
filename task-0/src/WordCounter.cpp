#include "WordCounter.h"
#include <algorithm>
#include <functional>
#include <cctype>

// Вспомогательные статические методы, скопированные из TextProcessor
namespace {
    bool isWordChar(char c) {
        return std::isalnum(static_cast<unsigned char>(c)) || (static_cast<unsigned char>(c) & 0x80);
    }

    std::string normalizeWord(const std::string& word) {
        std::string normalized = word;
        for (auto& ch : normalized) {
            if (static_cast<unsigned char>(ch) < 128) {
                ch = std::tolower(ch);
            }
        }
        return normalized;
    }
}

void WordCounter::processText(const std::vector<std::string>& lines) {
    for (const auto& line : lines) {
        std::string currentWord;

        for (size_t i = 0; i < line.size(); ++i) {
            unsigned char c = line[i];
            if (isWordChar(c)) {
                currentWord += c;
            } else if (!currentWord.empty()) {
                addWord(normalizeWord(currentWord));
                currentWord.clear();
            }
        }

        if (!currentWord.empty()) {
            addWord(normalizeWord(currentWord));
        }
    }
}

void WordCounter::addWord(const std::string& word) {
    if (!word.empty()) {
        wordCount[word]++;
        totalWords++;
    }
}

std::vector<std::pair<std::string, int>> WordCounter::getSortedWords() const {
    std::vector<std::pair<std::string, int>> wordsVec(wordCount.begin(), wordCount.end());

    std::sort(wordsVec.begin(), wordsVec.end(),
        [](const std::pair<std::string, int>& a, const std::pair<std::string, int>& b) {
            return a.second > b.second;
        });

    return wordsVec;
}
