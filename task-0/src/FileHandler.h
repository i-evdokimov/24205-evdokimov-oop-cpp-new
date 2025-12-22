#pragma once
#include <string>
#include <vector>

class FileHandler {
public:
    static std::vector<std::string> readLines(const std::string& filename);
    static bool writeCSV(const std::string& filename, const std::vector<std::string>& lines);

private:
    static bool fileExists(const std::string& filename);
};