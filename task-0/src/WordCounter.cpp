#include "WordCounter.h"
#include "TextProcessor.h"
#include <algorithm>
#include <functional>

void WordCounter::processText(const std::vector<std::string>& lines) {
    for (const auto& line : lines) {
        std::string currentWord;

        for (size_t i = 0; i < line.size(); ++i) {
            unsigned char c = line[i];
            if (TextProcessor::isWordChar(c)) {
                currentWord += c;
            } else if (!currentWord.empty()) {
                addWord(TextProcessor::normalizeWord(currentWord));
                currentWord.clear();
            }
        }

        if (!currentWord.empty()) {
            addWord(TextProcessor::normalizeWord(currentWord));
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