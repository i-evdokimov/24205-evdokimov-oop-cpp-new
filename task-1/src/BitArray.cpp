#include "BitArray.h"

namespace bitarray {
    BitArray::BitArray(int num_bits, unsigned long value) {
        if (num_bits < 0) {
            throw std::invalid_argument("invalid num_bits");
        }

        int count_word = (num_bits + BITS_IN_WORD - 1) / BITS_IN_WORD;
        num_bits_used = num_bits;
        num_bits_free = (count_word * BITS_IN_WORD) - num_bits;
        array.resize(count_word, 0UL);

        if (num_bits == 0) {
            return;
        }

        // Просто сохраняем value, без reverse
        array[0] = value;

        if (count_word > 0) {
            size_t used_bits_in_last_word = num_bits % BITS_IN_WORD;
            if (used_bits_in_last_word != 0) {
                unsigned long mask = (1UL << used_bits_in_last_word) - 1;
                array[count_word - 1] &= mask;
            }
        }
    }

    void BitArray::swap(BitArray &b) {
        std::swap(array, b.array);
        std::swap(num_bits_free, b.num_bits_free);
        std::swap(num_bits_used, b.num_bits_used);
    }

    BitArray &BitArray::operator=(const BitArray &b) {
        if (this == &b) {
            return *this;
        }
        BitArray tmp(b);
        swap(tmp);
        return *this;
    }

    void BitArray::resize(int num_bits, bool value) {
        if (num_bits < 0) {
            throw std::invalid_argument("New size must be not negative");
        }

        if (num_bits == num_bits_used) {
            return;
        }

        int new_count_word = (num_bits + BITS_IN_WORD - 1) / BITS_IN_WORD;
        bool is_expansion = num_bits > num_bits_used;

        if (!is_expansion) {
            array.resize(new_count_word);

            if (num_bits > 0 && num_bits % BITS_IN_WORD != 0) {
                int used_bits_in_last_word = num_bits % BITS_IN_WORD;
                unsigned long mask = (1UL << used_bits_in_last_word) - 1;
                array.back() &= mask;
            }
        }
        else {
            unsigned long word_value = value ? ~0UL : 0UL;
            int old_count_word = array.size();
            array.resize(new_count_word, 0UL);

            if (value) {
                for (int i = old_count_word; i < new_count_word; ++i) {
                    array[i] = word_value;
                }
            }

            int bits_in_last_old_word = num_bits_used % BITS_IN_WORD;

            if (bits_in_last_old_word != 0) {
                unsigned long mask_for_new_bits = ~((1UL << bits_in_last_old_word) - 1);
                int word_to_modify = old_count_word - 1;

                if (value) {
                    array[word_to_modify] |= mask_for_new_bits;
                } else {
                    array[word_to_modify] &= ~mask_for_new_bits;
                }
            }

            if (num_bits % BITS_IN_WORD != 0) {
                int used_bits_in_last_word = num_bits % BITS_IN_WORD;
                unsigned long mask = (1UL << used_bits_in_last_word) - 1;
                array.back() &= mask;
            }
        }

        num_bits_used = num_bits;
        num_bits_free = (new_count_word * BITS_IN_WORD) - num_bits;
    }

    void BitArray::clear() {
        this->resize(0);
    }

    void BitArray::push_back(bool bit) {
        this->resize(num_bits_used + 1, bit);
    }

    BitArray &BitArray::operator&=(const BitArray &b) {
        if (this->num_bits_used != b.num_bits_used) {
            throw std::invalid_argument("BitArray sizes must be equal");
        }
        for (size_t i = 0; i < array.size(); ++i) {
            array[i] &= b.array[i];
        }
        return *this;
    }

    BitArray &BitArray::operator|=(const BitArray &b) {
        if (this->num_bits_used != b.num_bits_used) {
            throw std::invalid_argument("BitArray sizes must be equal");
        }
        for (size_t i = 0; i < array.size(); ++i) {
            array[i] |= b.array[i];
        }
        return *this;
    }

    BitArray &BitArray::operator^=(const BitArray &b) {
        if (this->num_bits_used != b.num_bits_used) {
            throw std::invalid_argument("BitArray sizes must be equal");
        }
        for (size_t i = 0; i < array.size(); ++i) {
            array[i] ^= b.array[i];
        }
        return *this;
    }

    BitArray &BitArray::operator>>=(int n) {
        if (n < 0) {
            throw std::invalid_argument("invalid shift");
        }
        if (n == 0 || num_bits_used == 0) {
            return *this;
        }

        if (n >= num_bits_used) {
            for (size_t i = 0; i < array.size(); ++i) {
                array[i] = 0UL;
            }
            return *this;
        }
        int shift_words = n / BITS_IN_WORD;
        int shift_bits = n % BITS_IN_WORD;
        int len = array.size();
        for (int i = len - 1; i >= shift_words; --i) {
            array[i] = array[i - shift_words];
        }
        for (int i = 0; i < shift_words; ++i) {
            array[i] = 0UL;
        }
        if (shift_bits > 0) {
            const int carry_shift = BITS_IN_WORD - shift_bits;

            for (int i = len - 1; i > shift_words; --i) {
                unsigned long carry = array[i - 1] >> carry_shift;
                array[i] = (array[i] << shift_bits) | carry;
            }

            if (shift_words < len) {
                array[shift_words] = array[shift_words] << shift_bits;
            }
        }
        int unused_bits = len * BITS_IN_WORD - num_bits_used;
        if (unused_bits > 0) {
            unsigned long mask = ~0UL >> unused_bits;
            array[len - 1] &= mask;
        }
        return *this;
    }

    BitArray BitArray::operator<<(int n) const {
        BitArray result = *this;
        result <<= n;
        return result;
    }

    BitArray &BitArray::operator<<=(int n) {
        if (n < 0) {
            throw std::invalid_argument("invalid shift");
        }
        if (n == 0 || num_bits_used == 0) {
            return *this;
        }
        int shift_words = n / BITS_IN_WORD;
        int shift_bits = n % BITS_IN_WORD;
        int len = array.size();
        if (n >= num_bits_used) {
            for (size_t i = 0; i < len; ++i) {
                array[i] = 0UL;
            }
            return *this;
        }
        for (int i = 0; i < len - shift_words; ++i) {
            array[i] = array[i + shift_words];
        }
        for (int i = len - shift_words; i < len; ++i) {
            array[i] = 0UL;
        }
        if (shift_bits > 0) {
            const int carry_shift = BITS_IN_WORD - shift_bits;
            for (int i = 0; i < len - shift_words; ++i) {
                unsigned long current_word = array[i] >> shift_bits;
                if (i < len - shift_words - 1) {
                    unsigned long carry = array[i + 1] << carry_shift;
                    current_word |= carry;
                }

                array[i] = current_word;
            }
        }
        int current_num_words = (num_bits_used + BITS_IN_WORD - 1) / BITS_IN_WORD;
        int unused_bits = current_num_words * BITS_IN_WORD - num_bits_used;
        if (unused_bits > 0 && current_num_words > 0) {
            unsigned long mask = ~0UL >> unused_bits;
            array[current_num_words - 1] &= mask;
        }
        return *this;
    }

    BitArray BitArray::operator>>(int n) const {
        BitArray result = *this;
        result >>= n;
        return result;
    }

    BitArray &BitArray::set(int n, bool val) {
        if (n < 0 || n >= num_bits_used) {
            throw std::out_of_range("Bit index out of range");
        }
        int word_idx = n / BITS_IN_WORD;
        int bit_pos = n % BITS_IN_WORD;
        unsigned long mask = 1UL << bit_pos;
        if (val) {
            array[word_idx] |= mask;
        } else {
            array[word_idx] &= (~mask);
        }
        return *this;
    }

    BitArray &BitArray::set() {
        if (num_bits_used == 0) {
            return *this;
        }
        int len = array.size();
        for (int i = 0; i < len - 1; ++i) {
            array[i] = ~0UL;
        }
        if (len > 0) {
            int unused_bits = len * BITS_IN_WORD - num_bits_used;
            unsigned long mask = ~0UL;
            if (unused_bits > 0) {
                mask = ~0UL >> unused_bits;
            }
            array[len - 1] |= mask;
        }
        return *this;
    }

    BitArray &BitArray::reset(int n) {
        return set(n, false);
    }

    BitArray &BitArray::reset() {
        if (num_bits_used == 0) {
            return *this;
        }
        for (size_t i = 0; i < array.size(); i++) {
            array[i] = 0;
        }
        return *this;
    }

    bool BitArray::any() const {
        int count_words = (this->num_bits_used + BITS_IN_WORD - 1) / BITS_IN_WORD;
        for (int i = 0; i < count_words; i++) {
            if (array[i] != 0) {
                return true;
            }
        }
        return false;
    }

    bool BitArray::none() const {
        return !any();
    }

    BitArray BitArray::operator~() const {
        BitArray result = *this;
        int count_words = (num_bits_used + BITS_IN_WORD - 1) / BITS_IN_WORD;
        for (int i = 0; i < count_words; i++) {
            result.array[i] = ~result.array[i];
        }
        if (result.num_bits_used > 0) {
            unsigned long mask = ~0UL >> num_bits_free;
            result.array[count_words - 1] &= mask;
        }
        return result;
    }

    int BitArray::count() const {
        if (num_bits_used == 0) {
            return 0;
        }
        int count = 0;
        const int count_words = array.size();
        for (int i = 0; i < count_words; ++i) {
            count += __builtin_popcountll(array[i]);
        }
        return count;
    }

    bool BitArray::operator[](int i) const {
        if (i < 0 || i >= num_bits_used) {
            throw std::out_of_range("Bit index out of range");
        }
        int word_idx = i / BITS_IN_WORD;
        int bit_pos = i % BITS_IN_WORD;
        return (array[word_idx] & (1UL << bit_pos));
    }

    int BitArray::size() const {
        return num_bits_used;
    }

    bool BitArray::empty() const {
        return num_bits_used == 0;
    }

    std::string BitArray::to_string() const {
        if (num_bits_used == 0) {
            return "";
        }
        std::string result;

        // Собираем строку от старшего бита к младшему
        for (int i = num_bits_used - 1; i >= 0; --i) {
            if (this->operator[](i)) {
                result.push_back('1');
            } else {
                result.push_back('0');
            }
        }

        return result;
    }

    bool operator==(const BitArray &a, const BitArray &b) {
        if (a.size() != b.size()) {
            return false;
        }
        bool result = 1;
        for (int i = 0; i < a.size(); i++) {
            result = result && (a[i] == b[i]);
        }
        return result;
    }

    bool operator!=(const BitArray &a, const BitArray &b) {
        return !(a == b);
    }

    BitArray operator&(const BitArray &a, const BitArray &b) {
        BitArray result = a;
        result &= b;
        return result;
    }

    BitArray operator|(const BitArray &a, const BitArray &b) {
        BitArray result = a;
        result |= b;
        return result;
    }

    BitArray operator^(const BitArray &a, const BitArray &b) {
        BitArray result = a;
        result ^= b;
        return result;
    }
} // bitarray