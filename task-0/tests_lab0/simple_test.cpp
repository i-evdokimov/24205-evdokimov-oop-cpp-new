#include <iostream>
#include <fstream>
#include <cstdio>
#include <cassert>
#include "../src/FileHandler.h"
#include "../src/TextProcessor.h"
#include "../src/WordCounter.h"
#include "../src/CSVExporter.h"

using namespace std;

void runTest(const string& name, bool result, int& passed, int& total) {
    total++;
    if (result) {
        passed++;
        cout << "+ " << name << endl;
    } else {
        cout << "- " << name << endl;
    }
}

void testTextProcessor(int& passed, int& total) {
    // Test isWordChar
    runTest("isWordChar letters",
        TextProcessor::isWordChar('a') && TextProcessor::isWordChar('Z'), passed, total);

    runTest("isWordChar numbers", TextProcessor::isWordChar('5'), passed, total);

    runTest("isWordChar non-word",
        !TextProcessor::isWordChar(' ') && !TextProcessor::isWordChar(','), passed, total);

    // Test extractWords
    auto words = TextProcessor::extractWords("Hello world! Test 123.");
    bool extractTest = (words.size() == 4 && words[0] == "hello" && words[1] == "world");
    runTest("extractWords simple", extractTest, passed, total);

    // Test normalizeWord
    runTest("normalizeWord uppercase", TextProcessor::normalizeWord("HELLO") == "hello", passed, total);
    runTest("normalizeWord mixed case", TextProcessor::normalizeWord("HeLLo") == "hello", passed, total);
    runTest("normalizeWord with numbers", TextProcessor::normalizeWord("Test123") == "test123", passed, total);
}

void testWordCounter(int& passed, int& total) {
    // Test addWord
    WordCounter counter;
    counter.addWord("test");
    counter.addWord("test");
    counter.addWord("hello");

    bool addTest = (counter.getTotalWords() == 3);
    if (addTest) {
        const auto& wordMap = counter.getWordMap();
        addTest = (wordMap.find("test") != wordMap.end() &&
                  wordMap.at("test") == 2 &&
                  wordMap.find("hello") != wordMap.end() &&
                  wordMap.at("hello") == 1);
    }
    runTest("addWord counting", addTest, passed, total);

    // Test processText
    vector<string> lines = {"Hello world!", "Hello again"};
    WordCounter counter2;
    counter2.processText(lines);

    bool processTest = (counter2.getTotalWords() == 4);
    if (processTest) {
        const auto& wordMap = counter2.getWordMap();
        processTest = (wordMap.find("hello") != wordMap.end() &&
                      wordMap.at("hello") == 2 &&
                      wordMap.find("world") != wordMap.end() &&
                      wordMap.at("world") == 1 &&
                      wordMap.find("again") != wordMap.end() &&
                      wordMap.at("again") == 1);
    }
    runTest("processText", processTest, passed, total);

    // Test getSortedWords
    auto sorted = counter2.getSortedWords();
    bool sortTest = !sorted.empty();
    if (sortTest && sorted.size() >= 2) {
        sortTest = (sorted[0].second >= sorted[1].second);
    }
    runTest("getSortedWords", sortTest, passed, total);

    // Test empty word
    WordCounter counter3;
    counter3.addWord("");
    runTest("add empty word", counter3.getTotalWords() == 0 && counter3.getWordMap().empty(), passed, total);

}

void testFileHandler(int& passed, int& total) {
    string testFile = "test_file.txt";
    string testCSV = "test_output.csv";

    // Clean up old files
    remove(testFile.c_str());
    remove(testCSV.c_str());

    // Test write and read
    ofstream file(testFile);
    file << "Line 1\nLine 2\nLine 3";
    file.close();

    auto lines = FileHandler::readLines(testFile);
    runTest("readLines", lines.size() == 3, passed, total);

    if (lines.size() == 3) {
        bool contentTest = (lines[0] == "Line 1" &&
                           lines[1] == "Line 2" &&
                           lines[2] == "Line 3");
        runTest("readLines content", contentTest, passed, total);
    }

    // Test writeCSV
    vector<string> csvLines = {"Word;Count;Percentage (%)", "hello;5;50.000", "world;3;30.000"};
    bool writeResult = FileHandler::writeCSV(testCSV, csvLines);
    runTest("writeCSV", writeResult, passed, total);

    if (writeResult) {
        ifstream csvFile(testCSV, ios::binary);
        if (csvFile.is_open()) {
            unsigned char bom[3];
            csvFile.read(reinterpret_cast<char*>(bom), 3);
            bool bomTest = (bom[0] == 0xEF && bom[1] == 0xBB && bom[2] == 0xBF);
            runTest("CSV BOM", bomTest, passed, total);
            csvFile.close();
        }
    }

    // Clean up
    remove(testFile.c_str());
    remove(testCSV.c_str());
}

void testCSVExporter(int& passed, int& total) {
    string testCSV = "test_export.csv";
    remove(testCSV.c_str());

    // Test formatCSVLine
    string line = CSVExporter::formatCSVLine("hello", 5, 10);
    runTest("formatCSVLine basic", line == "hello;5;50.000", passed, total);

    line = CSVExporter::formatCSVLine("world", 1, 3);
    runTest("formatCSVLine fraction", line == "world;1;33.333", passed, total);

    line = CSVExporter::formatCSVLine("test", 0, 0);
    runTest("formatCSVLine zero division", line == "test;0;0.000", passed, total);

    // Test exportWordCount
    map<string, int> wordCount = {{"apple", 3}, {"banana", 2}, {"cherry", 1}};

    bool result = CSVExporter::exportWordCount(testCSV, wordCount, 6);
    runTest("exportWordCount", result, passed, total);

    if (result) {
        ifstream file(testCSV, ios::binary);
        if (file.is_open()) {
            file.seekg(3);

            vector<string> fileLines;
            string fileLine;
            while (getline(file, fileLine)) {
                fileLines.push_back(fileLine);
            }
            file.close();

            if (fileLines.size() >= 4) {
                runTest("CSV header", fileLines[0] == "Word;Count;Percentage (%)", passed, total);

                // Fix sorting test logic
                bool sortingTest = true;
                if (fileLines.size() >= 4) {
                    // Check that apple (count 3) comes before banana (count 2)
                    if (fileLines[1].find("apple") == string::npos) {
                        sortingTest = false;
                    }
                    if (fileLines[2].find("banana") == string::npos) {
                        sortingTest = false;
                    }
                    if (fileLines[3].find("cherry") == string::npos) {
                        sortingTest = false;
                    }
                }
                runTest("CSV sorting", sortingTest, passed, total);
            }
        }
    }

    // Test with empty map
    string testCSV2 = "test_export2.csv";
    remove(testCSV2.c_str());
    map<string, int> emptyCount;
    bool emptyResult = CSVExporter::exportWordCount(testCSV2, emptyCount, 0);
    runTest("export empty map", emptyResult, passed, total);

    if (emptyResult) {
        ifstream file(testCSV2);
        bool fileExists = file.is_open();
        file.close();
        runTest("empty CSV file exists", fileExists, passed, total);
    }

    remove(testCSV.c_str());
    remove(testCSV2.c_str());
}

void integrationTest(int& passed, int& total) {
    string inputFile = "test_input.txt";
    string outputFile = "test_output.csv";

    // Clean up old files
    remove(inputFile.c_str());
    remove(outputFile.c_str());

    // Create test input
    ofstream input(inputFile);
    input << "Hello world! This is a test.\n";
    input << "Hello again world! Testing is fun.";
    input.close();

    // Step 1: Read file
    auto lines = FileHandler::readLines(inputFile);
    bool readSuccess = !lines.empty();
    runTest("File reading", readSuccess, passed, total);

    if (!readSuccess) {
        remove(inputFile.c_str());
        return;
    }

    // Step 2: Process text
    WordCounter counter;
    counter.processText(lines);

    int totalWords = counter.getTotalWords();
    int uniqueWords = counter.getWordMap().size();

    bool processSuccess = (totalWords > 0 && uniqueWords > 0);
    runTest("Text processing", processSuccess, passed, total);

    // Step 3: Export to CSV
    bool exportResult = CSVExporter::exportWordCount(
        outputFile, counter.getSortedWords(), counter.getTotalWords());

    runTest("CSV export", exportResult, passed, total);

    if (exportResult) {
        ifstream csvFile(outputFile);
        bool fileExists = csvFile.is_open();
        csvFile.close();
        runTest("Output file created", fileExists, passed, total);
    }

    // Clean up
    remove(inputFile.c_str());
    remove(outputFile.c_str());
}

int main() {
    cout << "Starting tests..." << endl;
    cout << "==================" << endl;

    int passed = 0;
    int total = 0;

    try {
        testTextProcessor(passed, total);
        testWordCounter(passed, total);
        testFileHandler(passed, total);
        testCSVExporter(passed, total);
        integrationTest(passed, total);

    } catch (const exception& e) {
        cout << "\nException caught: " << e.what() << endl;
        return 1;
    } catch (...) {
        cout << "\nUnknown exception caught" << endl;
        return 1;
    }

    cout << "==================" << endl;
    cout << "Tests completed: " << passed << "/" << total << " passed" << endl;

    return (passed == total) ? 0 : 1;
}