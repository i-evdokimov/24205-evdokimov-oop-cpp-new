#include <iostream>
#include <string>
#include "BitArray.h"

using namespace bitarray;

int passedTests = 0;
int totalTests = 0;

void runTest(const std::string& name, bool result) {
    totalTests++;
    if (result) {
        passedTests++;
        std::cout << "+ " << name << std::endl;
    } else {
        std::cout << "- " << name << std::endl;
    }
}

void testConstruction() {
    std::cout << "\n=== Testing Construction ===" << std::endl;

    BitArray a1(5, 0b10101);
    runTest("Constructor with value", a1.size() == 5 && a1[0] && !a1[1] && a1[2] && !a1[3] && a1[4]);

    BitArray a2(0);
    runTest("Empty bit array", a2.empty() && a2.size() == 0);

    BitArray a3(1, 1);
    runTest("Single bit true", a3.size() == 1 && a3[0] == true);

    BitArray a4(10);
    bool allZeros = true;
    for (int i = 0; i < 10; i++) {
        if (a4[i]) allZeros = false;
    }
    runTest("All zeros", allZeros && a4.count() == 0);
}

void testAccessAndModification() {
    std::cout << "\n=== Testing Access and Modification ===" << std::endl;

    BitArray a(5);
    a.set(2);
    runTest("Set bit at position 2", a[2] == true && a.count() == 1);

    BitArray b(5, 0b11111);
    b.reset(3);
    runTest("Reset bit at position 3", b[3] == false && b.count() == 4);

    BitArray c(7);
    c.set();
    bool allOnes = true;
    for (int i = 0; i < 7; i++) {
        if (!c[i]) allOnes = false;
    }
    runTest("Set all bits", allOnes && c.count() == 7);

    BitArray d(7, 0b1111111);
    d.reset();
    bool allZeros = true;
    for (int i = 0; i < 7; i++) {
        if (d[i]) allZeros = false;
    }
    runTest("Reset all bits", allZeros && d.count() == 0);

    BitArray e(3, 0b101);
    e.push_back(true);
    e.push_back(false);
    runTest("Push back operations", e.size() == 5 && e[3] == true && e[4] == false);
}

void testOperations() {
    std::cout << "\n=== Testing Bit Operations ===" << std::endl;

    BitArray a(4, 0b1010);
    BitArray b(4, 0b1100);

    BitArray c = a & b;
    runTest("AND operation (1010 & 1100 = 1000)", c.to_string() == "1000");

    BitArray d = a | b;
    runTest("OR operation (1010 | 1100 = 1110)", d.to_string() == "1110");

    BitArray e = a ^ b;
    runTest("XOR operation (1010 ^ 1100 = 0110)", e.to_string() == "0110");

    BitArray f(4, 0b1010);
    BitArray g = ~f;
    runTest("NOT operation (~1010 = 0101)", g.to_string() == "0101");
}

void testShiftOperations() {
    std::cout << "\n=== Testing Shift Operations ===" << std::endl;

    BitArray a(5, 0b10101);
    BitArray b = a << 1;
    runTest("Left shift by 1 (10101 << 1 = 01010)", b.to_string() == "01010");

    BitArray f(3, 0b111);
    f <<= 5;
    bool allZeros = true;
    for (int i = 0; i < 3; i++) {
        if (f[i]) allZeros = false;
    }
    runTest("Shift beyond size results in zeros", allZeros);
}

void testUtilityFunctions() {
    std::cout << "\n=== Testing Utility Functions ===" << std::endl;

    BitArray a(5, 0b00100);
    BitArray b(5);
    runTest("any() returns true for non-zero array", a.any() == true);
    runTest("any() returns false for zero array", b.any() == false);

    runTest("none() returns false for non-zero array", a.none() == false);
    runTest("none() returns true for zero array", b.none() == true);

    BitArray c(10, 0b1010101010);
    runTest("count() returns correct number of set bits", c.count() == 5);

    BitArray d(4, 0b1101);
    runTest("to_string() returns correct binary representation", d.to_string() == "1101");

    BitArray e(0);
    BitArray f(3);
    runTest("empty() returns true for zero-sized array", e.empty() == true);
    runTest("empty() returns false for non-zero-sized array", f.empty() == false);
}

void testResizeAndClear() {
    std::cout << "\n=== Testing Resize and Clear ===" << std::endl;

    BitArray a(3, 0b101);
    a.resize(6, true);
    runTest("Resize larger with fill value true",
            a.size() == 6 && a[0] && !a[1] && a[2] && a[3] && a[4] && a[5]);

    BitArray b(5, 0b11111);
    b.resize(3);
    runTest("Resize smaller preserves bits", b.size() == 3 && b[0] && b[1] && b[2]);

    BitArray c(5, 0b10101);
    c.clear();
    runTest("Clear makes array empty", c.empty() && c.size() == 0);

    BitArray e(4, 0b1100);
    e.resize(4);
    runTest("Resize to same size doesn't change bits",
            e.size() == 4 && !e[0] && !e[1] && e[2] && e[3]);
}

void testComparisonAndAssignment() {
    std::cout << "\n=== Testing Comparison and Assignment ===" << std::endl;

    BitArray a(4, 0b1010);
    BitArray b(4, 0b1010);
    BitArray c(4, 0b0101);
    runTest("Equality operator for equal arrays", (a == b) == true);
    runTest("Equality operator for different arrays", (a == c) == false);

    runTest("Inequality operator for different arrays", (a != c) == true);
    runTest("Inequality operator for equal arrays", (a != b) == false);

    BitArray d(3, 0b101);
    BitArray e = d;
    runTest("Copy assignment copies size", d.size() == e.size());
    runTest("Copy assignment copies bits",
            d[0] == e[0] && d[1] == e[1] && d[2] == e[2]);

}

void testSwap() {
    std::cout << "\n=== Testing Swap ===" << std::endl;

    BitArray a(3, 0b101);
    BitArray b(5, 0b11011);

    int aSizeBefore = a.size();
    int bSizeBefore = b.size();
    std::string aStrBefore = a.to_string();
    std::string bStrBefore = b.to_string();

    a.swap(b);

    runTest("Swap exchanges sizes", a.size() == bSizeBefore && b.size() == aSizeBefore);
    runTest("Swap exchanges content", a.to_string() == bStrBefore && b.to_string() == aStrBefore);
}

void testEdgeCases() {
    std::cout << "\n=== Testing Edge Cases ===" << std::endl;

    BitArray a(1000);
    a.set(500);
    runTest("Large array (1000 bits) works correctly",
            a.size() == 1000 && a[500] == true && a.count() == 1);

    BitArray c(130);
    c.set(0);
    c.set(64);
    c.set(129);
    runTest("Multi-word storage works", c[0] && c[64] && c[129] && c.count() == 3);

    BitArray d(0);
    runTest("Operations on empty array don't crash",
            d.empty() && d.none() && !d.any() && d.count() == 0);
}

void runAllTests() {
    std::cout << "starting BitArray tests" << std::endl;
    std::cout << "=======================" << std::endl;

    passedTests = 0;
    totalTests = 0;

    testConstruction();
    testAccessAndModification();
    testOperations();
    testShiftOperations();
    testUtilityFunctions();
    testResizeAndClear();
    testComparisonAndAssignment();
    testSwap();
    testEdgeCases();

    std::cout << "\n=======================" << std::endl;
    std::cout << "tests passed: " << passedTests << "/" << totalTests << std::endl;

    if (passedTests == totalTests) {
        std::cout << "all tests passed successfully!" << std::endl;
    } else {
        std::cout << "some tests failed!" << std::endl;
    }
}

int main() {
    try {
        runAllTests();
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "\nexception caught: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "\nunknown exception caught" << std::endl;
        return 1;
    }
}