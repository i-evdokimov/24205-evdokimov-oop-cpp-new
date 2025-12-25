#include <iostream>
#include "FileHandler.h"
#include "TextProcessor.h"
#include "WordCounter.h"
#include "CSVExporter.h"

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "usage: word_count.exe input.txt output.csv :)\n";
        return 1;
    }

    std::string inputFile = argv[1];
    std::string outputFile = argv[2];

    // 1. Чтение файла
    std::cout << "1) reading file...\n";
    auto lines = FileHandler::readLines(inputFile);
    if (lines.empty()) {
        std::cerr << "failed to read file or file is empty :(\n";
        return 1;
    }

    // 2. Обработка текста и подсчет слов
    std::cout << "2) processing text...\n";
    WordCounter wordCounter;
    wordCounter.processText(lines);

    // 3. Экспорт в CSV
    std::cout << "3) exporting to CSV...\n";
    auto sortedWords = wordCounter.getSortedWords();
    if (!CSVExporter::exportWordCount(outputFile, sortedWords, wordCounter.getTotalWords())) {
        std::cerr << "failed to export CSV :(\n";
        return 1;
    }

    std::cout << "done! CSV saved to " << outputFile << "\n";
    std::cout << "total words processed: " << wordCounter.getTotalWords() << "\n";
    std::cout << "unique words: " << sortedWords.size() << "\n";

    return 0;
}