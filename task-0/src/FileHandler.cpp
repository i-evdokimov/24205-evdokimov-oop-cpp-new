#include "FileHandler.h"
#include <fstream>
#include <iostream>
#include <cstdio>

std::vector<std::string> FileHandler::readLines(const std::string& filename) {
    std::vector<std::string> lines;
    std::ifstream inputFile(filename);

    if (!inputFile.is_open()) {
        std::cerr << "Cannot open file: " << filename << "\n";
        return lines;
    }

    std::string line;
    while (std::getline(inputFile, line)) {
        lines.push_back(line);
    }

    inputFile.close();
    return lines;
}

bool FileHandler::writeCSV(const std::string& filename, const std::vector<std::string>& lines) {
    std::ofstream outputFile(filename, std::ios::binary);
    if (!outputFile.is_open()) {
        std::cerr << "Cannot write file: " << filename << "\n";
        return false;
    }

    // BOM для UTF-8
    unsigned char bom[] = {0xEF, 0xBB, 0xBF};
    outputFile.write(reinterpret_cast<char*>(bom), 3);

    for (const auto& line : lines) {
        outputFile << line << "\n";
    }

    outputFile.close();
    return true;
}