#ifndef LAB_1_BITARRAY_H
#define LAB_1_BITARRAY_H

#include <iostream>
#include <cmath>
#include <vector>
#include <string>

namespace bitarray {
    class BitArray {
    public:
        explicit BitArray(int num_bits, unsigned long value = 0);

        void swap(BitArray &b);

        BitArray &operator=(const BitArray &b);

        void resize(int num_bits, bool value = false);

        void clear();

        void push_back(bool bit);

        BitArray &operator&=(const BitArray &b);

        BitArray &operator|=(const BitArray &b);

        BitArray &operator^=(const BitArray &b);

        BitArray &operator<<=(int n);

        BitArray &operator>>=(int n);

        BitArray operator<<(int n) const;

        BitArray operator>>(int n) const;

        BitArray &set(int n, bool val = true);

        BitArray &set();

        BitArray &reset(int n);

        BitArray &reset();

        bool any() const;

        bool none() const;

        BitArray operator~() const;

        int count() const;

        bool operator[](int i) const;

        int size() const;

        bool empty() const;

        std::string to_string() const;

    private:
        std::vector<unsigned long> array;
        int num_bits_used;
        int num_bits_free;
        static const int BITS_IN_WORD = sizeof(unsigned long) * 8;
    };

    bool operator==(const BitArray &a, const BitArray &b);

    bool operator!=(const BitArray &a, const BitArray &b);

    BitArray operator&(const BitArray &b1, const BitArray &b2);

    BitArray operator|(const BitArray &b1, const BitArray &b2);

    BitArray operator^(const BitArray &b1, const BitArray &b2);
} // bitarray
#endif //LAB_1_BITARRAY_H
