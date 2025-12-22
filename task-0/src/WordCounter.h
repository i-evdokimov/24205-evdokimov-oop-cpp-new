#pragma once
#include <string>
#include <map>
#include <vector>
#include <utility>

class WordCounter {
public:
    WordCounter() : totalWords(0) {}
    
    void processText(const std::vector<std::string>& lines);
    void addWord(const std::string& word);
    
    int getTotalWords() const { return totalWords; }
    const std::map<std::string, int>& getWordMap() const { return wordCount; }
    
    std::vector<std::pair<std::string, int>> getSortedWords() const;
    
private:
    std::map<std::string, int> wordCount;
    int totalWords;
};