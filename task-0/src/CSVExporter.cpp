#include "CSVExporter.h"
#include "FileHandler.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <functional>

std::string CSVExporter::formatCSVLine(const std::string& word, int count, int totalWords) {
    std::stringstream ss;
    double percent = (totalWords > 0) ? (100.0 * count / totalWords) : 0.0;

    ss << word << ";" << count << ";" << std::fixed << std::setprecision(3) << percent;
    return ss.str();
}

bool CSVExporter::exportWordCount(const std::string& filename,
                                 const std::map<std::string, int>& wordCount,
                                 int totalWords) {
    std::vector<std::pair<std::string, int>> wordsVec(wordCount.begin(), wordCount.end());
    std::sort(wordsVec.begin(), wordsVec.end(),
        [](const auto& a, const auto& b) { return a.second > b.second; });

    return exportWordCount(filename, wordsVec, totalWords);
}

bool CSVExporter::exportWordCount(const std::string& filename,
                                 const std::vector<std::pair<std::string, int>>& sortedWords,
                                 int totalWords) {
    std::vector<std::string> lines;
    lines.push_back("Word;Count;Percentage (%)");

    for (const auto& [word, count] : sortedWords) {
        lines.push_back(formatCSVLine(word, count, totalWords));
    }

    return FileHandler::writeCSV(filename, lines);
}