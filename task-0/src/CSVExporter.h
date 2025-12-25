#pragma once
#include <string>
#include <map>
#include <vector>

class CSVExporter {
public:
    static bool exportWordCount(const std::string& filename,
                               const std::map<std::string, int>& wordCount,
                               int totalWords);

    static bool exportWordCount(const std::string& filename,
                               const std::vector<std::pair<std::string, int>>& sortedWords,
                               int totalWords);

    // для внутреннего использования
    static std::string formatCSVLine(const std::string& word, int count, int totalWords);
};