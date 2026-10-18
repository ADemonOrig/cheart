#ifndef __ARRAY_HPP__
#define __ARRAY_HPP__ 1

#include <initializer_list>
#include <iostream>

namespace type
{
    using numbert = signed long long int;
    using unumbert = unsigned long long int;
    using numt = signed long long int;
    using unumt = unsigned long long int;
    using numbt = signed long long int;
    using unumbt = unsigned long long int;
    using numb = signed long long int;
    using unumb = unsigned long long int;
    using sizet = signed long long int;
    using usizet = unsigned long long int;
    using ssize = signed long long int;
    using usize = unsigned long long int;
    using indext = signed long long int;

    template<typename TYPE, usizet SIZE = 0>
    class array {
    private:
        TYPE _data[SIZE];

        static inline constexpr bool sortDefaultCmp(TYPE a, TYPE b) noexcept { return a < b; }
        static inline constexpr void sortSwapElems(TYPE &a, TYPE &b) noexcept { TYPE t = a; a = b; b = t; }

        template<typename Cmp>
        static inline constexpr void sortInsertion(TYPE *data, usizet lo, usizet hi, Cmp cmp) noexcept {
            for (usizet i = lo + 1; i < hi; i++) {
                TYPE key = data[i];
                usizet j = i;
                while (j > lo && cmp(key, data[j - 1])) {
                    data[j] = data[j - 1];
                    j--;
                }
                data[j] = key;
            }
        }

        template<typename Cmp>
        static inline constexpr usizet sortPartition(TYPE *data, usizet lo, usizet hi, Cmp cmp) noexcept {
            TYPE pivot = data[lo + (hi - lo) / 2];
            usizet i = lo, j = hi - 1;
            while (true) {
                while (cmp(data[i], pivot)) i++;
                while (cmp(pivot, data[j])) j--;
                if (i >= j) return j;
                sortSwapElems(data[i], data[j]);
                i++; j--;
            }
        }

        template<typename Cmp>
        static inline constexpr void sortQuick(TYPE *data, usizet lo, usizet hi, Cmp cmp) noexcept {
            while (hi - lo > 16) {
                usizet p = sortPartition(data, lo, hi, cmp);
                if (p - lo < hi - (p + 1)) {
                    sortQuick(data, lo, p + 1, cmp);
                    lo = p + 1;
                } else {
                    sortQuick(data, p + 1, hi, cmp);
                    hi = p + 1;
                }
            }
            sortInsertion(data, lo, hi, cmp);
        }

    public:
        static inline constexpr indext around(indext index = 0) noexcept { return ((index % SIZE) + SIZE) % SIZE; }

        inline constexpr explicit array(TYPE value = TYPE()) noexcept {
            fill(value);
        }

        inline constexpr array(const TYPE *data, usizet size) noexcept {
            copy(data, size);
        }

        template<usizet OSIZE>
        inline constexpr array(const array<TYPE, OSIZE> &arr) noexcept {
            copy(arr);
        }

        inline constexpr array(const std::initializer_list<TYPE> &list) noexcept {
            if (SIZE > list.size()) {
                for (indext i = 0; i < list.size(); i++) _data[i] = list.begin()[i];
                for (indext i = list.size(); i < SIZE; i++) _data[i] = TYPE();
            } else for (indext i = 0; i < SIZE; i++) _data[i] = list.begin()[i];
        }

        inline constexpr array<TYPE, SIZE> &copy(const TYPE *data = 0, usizet size = 0) noexcept {
            if (data) {
                if (SIZE > size) {
                    for (indext i = 0; i < size; i++) _data[i] = data[i];
                    for (indext i = size; i < SIZE; i++) _data[i] = TYPE();
                } else for (indext i = 0; i < SIZE; i++) _data[i] = data[i];
            } else for (indext i = 0; i < SIZE; i++) _data[i] = TYPE();
            return *this;
        }

        template<usizet OSIZE>
        inline constexpr array<TYPE, SIZE> &copy(const array<TYPE, OSIZE> &arr = array<TYPE, 0>()) noexcept {
            if (SIZE > OSIZE) {
                for (indext i = 0; i < OSIZE; i++) _data[i] = arr[i];
                for (indext i = OSIZE; i < SIZE; i++) _data[i] = TYPE();
            } else for (indext i = 0; i < SIZE; i++) _data[i] = arr[i];
            return *this;
        }

        inline constexpr array<TYPE, SIZE> clone() const noexcept {
            return array<TYPE, SIZE>(*this);
        }

        inline constexpr array<TYPE, SIZE> &swap(TYPE *data = 0, usizet size = 0) noexcept {
            if (data) {
                TYPE temp;
                if (SIZE > size) {
                    for (indext i = 0; i < size; i++) {
                        temp = _data[i];
                        _data[i] = data[i];
                        data[i] = temp;
                    }
                    for (indext i = size; i < SIZE; i++) _data[i] = TYPE();
                } else {
                    for (indext i = 0; i < SIZE; i++) {
                        temp = _data[i];
                        _data[i] = data[i];
                        data[i] = temp;
                    }
                    for (indext i = SIZE; i < size; i++) data[i] = TYPE();
                }
            } else for (indext i = 0; i < SIZE; i++) _data[i] = TYPE();
            return *this;
        }

        template<usizet OSIZE>
        inline constexpr array<TYPE, SIZE> &swap(array<TYPE, OSIZE> &arr = array<TYPE, 0>()) noexcept {
            TYPE temp;
            if (SIZE > OSIZE) {
                for (indext i = 0; i < OSIZE; i++) {
                    temp = _data[i];
                    _data[i] = arr[i];
                    arr[i] = temp;
                }
                for (indext i = OSIZE; i < SIZE; i++) _data[i] = TYPE();
            } else {
                for (indext i = 0; i < SIZE; i++) {
                    temp = _data[i];
                    _data[i] = arr[i];
                    arr[i] = temp;
                }
                for (indext i = SIZE; i < OSIZE; i++) arr[i] = TYPE();
            }
            return *this;
        }

        inline constexpr const TYPE *dataConst() const noexcept { return _data; }
        inline constexpr const TYPE *data() const noexcept { return _data; }
        inline constexpr TYPE *data() noexcept { return _data; }

        inline constexpr const TYPE *beginConst() const noexcept { return _data; }
        inline constexpr const TYPE *begin() const noexcept { return _data; }
        inline constexpr TYPE *begin() noexcept { return _data; }

        inline constexpr const TYPE *endConst() const noexcept { return _data + SIZE; }
        inline constexpr const TYPE *end() const noexcept { return _data + SIZE; }
        inline constexpr TYPE *end() noexcept { return _data + SIZE; }

        inline constexpr const TYPE *startConst() const noexcept { return _data; }
        inline constexpr const TYPE *start() const noexcept { return _data; }
        inline constexpr TYPE *start() noexcept { return _data; }

        inline constexpr const TYPE &firstConst() const noexcept { return _data[0]; }
        inline constexpr const TYPE &first() const noexcept { return _data[0]; }
        inline constexpr TYPE &first() noexcept { return _data[0]; }

        inline constexpr const TYPE &lastConst() const noexcept { return _data[SIZE - 1]; }
        inline constexpr const TYPE &last() const noexcept { return _data[SIZE - 1]; }
        inline constexpr TYPE &last() noexcept { return _data[SIZE - 1]; }

        inline constexpr const TYPE &elementConst(indext index = 0) const noexcept { return _data[around(index)]; }
        inline constexpr const TYPE &element(indext index = 0) const noexcept { return _data[around(index)]; }
        inline constexpr TYPE &element(indext index = 0) noexcept { return _data[around(index)]; }

        inline constexpr TYPE elementSafe(indext index = 0) const noexcept {
            if (index >= 0 && index < SIZE) return _data[index];
            return TYPE();
        }

        inline constexpr usizet size() const noexcept { return SIZE; }
        inline constexpr bool empty() const noexcept { return false; }

        inline constexpr bool isNull() const noexcept {
            for (indext i = 0; i < SIZE; i++) if (_data[i] != TYPE()) return false;
            return true;
        }

        inline constexpr array<TYPE, SIZE> &null() noexcept { return fill(TYPE()); }
        inline constexpr array<TYPE, SIZE> &clear() noexcept { return fill(TYPE()); }

        inline constexpr array<TYPE, SIZE> &fill(TYPE value = TYPE()) noexcept {
            for (indext i = 0; i < SIZE; i++) _data[i] = value;
            return *this;
        }

        inline constexpr bool has(TYPE value = TYPE()) const noexcept {
            for (indext i = 0; i < SIZE; i++) if (_data[i] == value) return true;
            return false;
        }

        inline constexpr indext findL(TYPE value = TYPE()) const noexcept {
            for (indext i = 0; i < SIZE; i++) if (_data[i] == value) return i;
            return -1;
        }

        inline constexpr indext findR(TYPE value = TYPE()) const noexcept {
            for (indext i = SIZE - 1; i >= 0; i--) if (_data[i] == value) return i;
            return -1;
        }

        inline constexpr indext find(TYPE value = TYPE()) const noexcept { return findL(value); }

        inline constexpr array<TYPE, SIZE> &reverse() noexcept {
            TYPE temp;
            for (indext r = SIZE - 1, w = 0; w < (SIZE / 2); (r--, w++)) {
                temp = _data[r];
                _data[r] = _data[w];
                _data[w] = temp;
            }
            return *this;
        }

        inline constexpr array<TYPE, SIZE> &sort() noexcept {
            sortQuick(_data, 0, SIZE, sortDefaultCmp);
            return *this;
        }

        template<typename Cmp>
        inline constexpr array<TYPE, SIZE> &sort(Cmp cmp) {
            sortQuick(_data, 0, SIZE, cmp);
            return *this;
        }

        inline constexpr array<TYPE, SIZE> &shiftR(sizet count = 1) noexcept {
            if (count < 0) return shiftL(-count);
            if (static_cast<usizet>(count) >= SIZE) return fill(TYPE());
            TYPE temp[SIZE];
            usizet shift = static_cast<usizet>(count);
            for (usizet i = 0; i < SIZE; i++) temp[i] = (i >= shift) ? _data[i - shift] : TYPE();
            for (usizet i = 0; i < SIZE; i++) _data[i] = temp[i];
            return *this;
        }

        inline constexpr array<TYPE, SIZE> &shiftL(sizet count = 1) noexcept {
            if (count < 0) return shiftR(-count);
            if (static_cast<usizet>(count) >= SIZE) return fill(TYPE());
            TYPE temp[SIZE];
            usizet shift = static_cast<usizet>(count);
            for (usizet i = 0; i < SIZE; i++) temp[i] = (i + shift < SIZE) ? _data[i + shift] : TYPE();
            for (usizet i = 0; i < SIZE; i++) _data[i] = temp[i];
            return *this;
        }

        inline constexpr array<TYPE, SIZE> &rotateR(sizet count = 1) noexcept {
            if (count < 0) return rotateL(-count);
            usizet shift = static_cast<usizet>(around(count));
            if (shift == 0) return *this;
            TYPE temp[SIZE];
            for (usizet i = 0; i < SIZE; i++) temp[i] = _data[(i + SIZE - shift) % SIZE];
            for (usizet i = 0; i < SIZE; i++) _data[i] = temp[i];
            return *this;
        }

        inline constexpr array<TYPE, SIZE> &rotateL(sizet count = 1) noexcept {
            if (count < 0) return rotateR(-count);
            usizet shift = static_cast<usizet>(around(count));
            if (shift == 0) return *this;
            TYPE temp[SIZE];
            for (usizet i = 0; i < SIZE; i++) temp[i] = _data[(i + shift) % SIZE];
            for (usizet i = 0; i < SIZE; i++) _data[i] = temp[i];
            return *this;
        }

        inline constexpr explicit operator const TYPE*() const noexcept { return _data; }
        inline constexpr explicit operator TYPE*() noexcept { return _data; }
        inline constexpr explicit operator const TYPE&() const noexcept { return _data[0]; }
        inline constexpr explicit operator TYPE&() noexcept { return _data[0]; }
        inline constexpr explicit operator bool() const noexcept { return !isNull(); }

        inline constexpr const TYPE& operator*() const noexcept { return _data[0]; }
        inline constexpr TYPE& operator*() noexcept { return _data[0]; }

        inline constexpr const TYPE& operator[](indext index) const noexcept { return _data[around(index)]; }
        inline constexpr TYPE& operator[](indext index) noexcept { return _data[around(index)]; }

        inline constexpr array<TYPE, SIZE>& operator()(TYPE value = TYPE()) noexcept {
            for (indext i = 0; i < SIZE; i++) _data[i] = value;
            return *this;
        }

        inline constexpr array<TYPE, SIZE>& operator++() noexcept {
            shiftR(1);
            return *this;
        }
        inline constexpr array<TYPE, SIZE> operator++(int) noexcept {
            array<TYPE, SIZE> arr(*this);
            shiftR(1);
            return arr;
        }
        inline constexpr array<TYPE, SIZE>& operator--() noexcept {
            shiftL(1);
            return *this;
        }
        inline constexpr array<TYPE, SIZE> operator--(int) noexcept {
            array<TYPE, SIZE> arr(*this);
            shiftL(1);
            return arr;
        }

        template<usizet OSIZE>
        inline constexpr array<TYPE, SIZE>& operator=(const array<TYPE, OSIZE> &arr) noexcept {
            if (SIZE > OSIZE) {
                for (indext i = 0; i < OSIZE; i++) _data[i] = arr[i];
                for (indext i = OSIZE; i < SIZE; i++) _data[i] = TYPE();
            }
            else for (indext i = 0; i < SIZE; i++) _data[i] = arr[i];
            return *this;
        }

        template<usizet OSIZE>
        inline constexpr array<TYPE, SIZE>& operator+=(const array<TYPE, OSIZE> &arr) noexcept {
            if (SIZE > OSIZE) { for (indext i = 0; i < OSIZE; i++) _data[i] += arr[i]; }
            else for (indext i = 0; i < SIZE; i++) _data[i] += arr[i];
            return *this;
        }
        template<usizet OSIZE>
        inline constexpr array<TYPE, SIZE>& operator-=(const array<TYPE, OSIZE> &arr) noexcept {
            if (SIZE > OSIZE) { for (indext i = 0; i < OSIZE; i++) _data[i] -= arr[i]; }
            else for (indext i = 0; i < SIZE; i++) _data[i] -= arr[i];
            return *this;
        }
        template<usizet OSIZE>
        inline constexpr array<TYPE, SIZE>& operator*=(const array<TYPE, OSIZE> &arr) noexcept {
            if (SIZE > OSIZE) { for (indext i = 0; i < OSIZE; i++) _data[i] *= arr[i]; }
            else for (indext i = 0; i < SIZE; i++) _data[i] *= arr[i];
            return *this;
        }
        template<usizet OSIZE>
        inline constexpr array<TYPE, SIZE>& operator/=(const array<TYPE, OSIZE> &arr) noexcept {
            if (SIZE > OSIZE) { for (indext i = 0; i < OSIZE; i++) _data[i] /= arr[i]; }
            else for (indext i = 0; i < SIZE; i++) _data[i] /= arr[i];
            return *this;
        }
        template<usizet OSIZE>
        inline constexpr array<TYPE, SIZE>& operator%=(const array<TYPE, OSIZE> &arr) noexcept {
            if (SIZE > OSIZE) { for (indext i = 0; i < OSIZE; i++) _data[i] %= arr[i]; }
            else for (indext i = 0; i < SIZE; i++) _data[i] %= arr[i];
            return *this;
        }
        template<usizet OSIZE>
        inline constexpr array<TYPE, SIZE>& operator|=(const array<TYPE, OSIZE> &arr) noexcept {
            if (SIZE > OSIZE) { for (indext i = 0; i < OSIZE; i++) _data[i] |= arr[i]; }
            else for (indext i = 0; i < SIZE; i++) _data[i] |= arr[i];
            return *this;
        }
        template<usizet OSIZE>
        inline constexpr array<TYPE, SIZE>& operator&=(const array<TYPE, OSIZE> &arr) noexcept {
            if (SIZE > OSIZE) { for (indext i = 0; i < OSIZE; i++) _data[i] &= arr[i]; }
            else for (indext i = 0; i < SIZE; i++) _data[i] &= arr[i];
            return *this;
        }
        template<usizet OSIZE>
        inline constexpr array<TYPE, SIZE>& operator^=(const array<TYPE, OSIZE> &arr) noexcept {
            if (SIZE > OSIZE) { for (indext i = 0; i < OSIZE; i++) _data[i] ^= arr[i]; }
            else for (indext i = 0; i < SIZE; i++) _data[i] ^= arr[i];
            return *this;
        }
        template<usizet OSIZE>
        inline constexpr array<TYPE, SIZE>& operator>>=(const array<TYPE, OSIZE> &arr) noexcept {
            if (SIZE > OSIZE) { for (indext i = 0; i < OSIZE; i++) _data[i] >>= arr[i]; }
            else for (indext i = 0; i < SIZE; i++) _data[i] >>= arr[i];
            return *this;
        }
        template<usizet OSIZE>
        inline constexpr array<TYPE, SIZE>& operator<<=(const array<TYPE, OSIZE> &arr) noexcept {
            if (SIZE > OSIZE) { for (indext i = 0; i < OSIZE; i++) _data[i] <<= arr[i]; }
            else for (indext i = 0; i < SIZE; i++) _data[i] <<= arr[i];
            return *this;
        }

        inline constexpr array<TYPE, SIZE>& operator>>=(numbert count) noexcept {
            shiftR(count);
            return *this;
        }
        inline constexpr array<TYPE, SIZE>& operator<<=(numbert count) noexcept {
            shiftL(count);
            return *this;
        }

        template<usizet OSIZE>
        inline constexpr array<TYPE, SIZE> operator+(const array<TYPE, OSIZE> &arr) const noexcept { return (array<TYPE, SIZE>(*this) += arr); }
        template<usizet OSIZE>
        inline constexpr array<TYPE, SIZE> operator-(const array<TYPE, OSIZE> &arr) const noexcept { return (array<TYPE, SIZE>(*this) -= arr); }
        template<usizet OSIZE>
        inline constexpr array<TYPE, SIZE> operator*(const array<TYPE, OSIZE> &arr) const noexcept { return (array<TYPE, SIZE>(*this) *= arr); }
        template<usizet OSIZE>
        inline constexpr array<TYPE, SIZE> operator/(const array<TYPE, OSIZE> &arr) const noexcept { return (array<TYPE, SIZE>(*this) /= arr); }
        template<usizet OSIZE>
        inline constexpr array<TYPE, SIZE> operator%(const array<TYPE, OSIZE> &arr) const noexcept { return (array<TYPE, SIZE>(*this) %= arr); }
        template<usizet OSIZE>
        inline constexpr array<TYPE, SIZE> operator|(const array<TYPE, OSIZE> &arr) const noexcept { return (array<TYPE, SIZE>(*this) |= arr); }
        template<usizet OSIZE>
        inline constexpr array<TYPE, SIZE> operator&(const array<TYPE, OSIZE> &arr) const noexcept { return (array<TYPE, SIZE>(*this) &= arr); }
        template<usizet OSIZE>
        inline constexpr array<TYPE, SIZE> operator^(const array<TYPE, OSIZE> &arr) const noexcept { return (array<TYPE, SIZE>(*this) ^= arr); }
        template<usizet OSIZE>
        inline constexpr array<TYPE, SIZE> operator>>(const array<TYPE, OSIZE> &arr) const noexcept { return (array<TYPE, SIZE>(*this) >>= arr); }
        template<usizet OSIZE>
        inline constexpr array<TYPE, SIZE> operator<<(const array<TYPE, OSIZE> &arr) const noexcept { return (array<TYPE, SIZE>(*this) <<= arr); }

        inline constexpr array<TYPE, SIZE> operator>>(numbert count) const noexcept { return array<TYPE, SIZE>(*this).shiftR(count); }
        inline constexpr array<TYPE, SIZE> operator<<(numbert count) const noexcept { return array<TYPE, SIZE>(*this).shiftL(count); }
    };

    template<typename TYPE>
    class array<TYPE, 0> {
    public:
        static inline constexpr indext around(indext index = 0) noexcept { return 0; }
        inline constexpr explicit array(TYPE value = TYPE()) noexcept {}
        inline constexpr array(const TYPE *data, usizet size) noexcept {}
        template<usizet OSIZE>
        inline constexpr array(const array<TYPE, OSIZE> &arr) noexcept {}
        inline constexpr array(const std::initializer_list<TYPE> &list) noexcept {}
        inline constexpr array<TYPE, 0> &copy(const TYPE *data = 0, usizet size = 0) noexcept { return *this; }
        template<usizet OSIZE>
        inline constexpr array<TYPE, 0> &copy(const array<TYPE, OSIZE> &arr = array<TYPE, 0>()) noexcept { return *this; }
        inline constexpr array<TYPE, 0> clone() const noexcept { return array<TYPE, 0>(); }
        inline constexpr array<TYPE, 0> &swap(TYPE *data = 0, usizet size = 0) noexcept {
            if (data) for (indext i = 0; i < size; i++) data[i] = TYPE();
            return *this;
        }
        template<usizet OSIZE>
        inline constexpr array<TYPE, 0> &swap(array<TYPE, OSIZE> &arr = array<TYPE, 0>()) noexcept {
            for (indext i = 0; i < OSIZE; i++) arr[i] = TYPE();
            return *this;
        }
        inline constexpr const TYPE *dataConst() const noexcept { return 0; }
        inline constexpr const TYPE *data() const noexcept { return 0; }
        inline constexpr const TYPE *beginConst() const noexcept { return 0; }
        inline constexpr const TYPE *begin() const noexcept { return 0; }
        inline constexpr const TYPE *endConst() const noexcept { return 0; }
        inline constexpr const TYPE *end() const noexcept { return 0; }
        inline constexpr const TYPE *startConst() const noexcept { return 0; }
        inline constexpr const TYPE *start() const noexcept { return 0; }
        inline constexpr const TYPE firstConst() const noexcept { return TYPE(); }
        inline constexpr TYPE first() const noexcept { return TYPE(); }
        inline constexpr const TYPE lastConst() const noexcept { return TYPE(); }
        inline constexpr TYPE last() const noexcept { return TYPE(); }
        inline constexpr const TYPE elementConst(indext index = 0) const noexcept { return TYPE(); }
        inline constexpr TYPE element(indext index = 0) const noexcept { return TYPE(); }
        inline constexpr TYPE elementSafe(indext index = 0) const noexcept { return TYPE(); }
        inline constexpr usizet size() const noexcept { return 0; }
        inline constexpr bool empty() const noexcept { return true; }
        inline constexpr bool isNull() const noexcept { return true; }
        inline constexpr array<TYPE, 0> &null() noexcept { return *this; }
        inline constexpr array<TYPE, 0> &clear() noexcept { return *this; }
        inline constexpr array<TYPE, 0> &fill(TYPE value = TYPE()) noexcept { return *this; }
        inline constexpr bool has(TYPE value = TYPE()) const noexcept { return (value == TYPE()) ? true : false; }
        inline constexpr indext findL(TYPE value = TYPE()) const noexcept { return (value == TYPE()) ? 0 : -1; }
        inline constexpr indext findR(TYPE value = TYPE()) const noexcept { return (value == TYPE()) ? 0 : -1; }
        inline constexpr indext find(TYPE value = TYPE()) const noexcept { return findL(value); }
        inline constexpr array<TYPE, 0> &reverse() noexcept { return *this; }
        inline constexpr array<TYPE, 0> &sort() noexcept { return *this; }
        template<typename Cmp>
        inline constexpr array<TYPE, 0> &sort(Cmp cmp) noexcept { return *this; }
        inline constexpr array<TYPE, 0> &rotateR(sizet count = 1) noexcept { return *this; }
        inline constexpr array<TYPE, 0> &rotateL(sizet count = 1) noexcept { return *this; }
        inline constexpr array<TYPE, 0> &shiftR(sizet count = 1) noexcept { return *this; }
        inline constexpr array<TYPE, 0> &shiftL(sizet count = 1) noexcept { return *this; }
        inline constexpr explicit operator const TYPE*() const noexcept { return 0; }
        inline constexpr explicit operator TYPE*() noexcept { return 0; }
        inline constexpr explicit operator TYPE() const noexcept { return TYPE(); }
        inline constexpr explicit operator bool() const noexcept { return false; }
        inline constexpr TYPE operator*() const noexcept { return TYPE(); }
        inline constexpr TYPE operator[](indext index) const noexcept { return TYPE(); }
        inline constexpr array<TYPE, 0>& operator()(TYPE value = TYPE()) noexcept { return *this; }
        inline constexpr array<TYPE, 0>& operator++() noexcept { return *this; }
        inline constexpr array<TYPE, 0> operator++(int) noexcept { return array<TYPE, 0>(); }
        inline constexpr array<TYPE, 0>& operator--() noexcept { return *this; }
        inline constexpr array<TYPE, 0> operator--(int) noexcept { return array<TYPE, 0>(); }
        template<usizet OSIZE>
        inline constexpr array<TYPE, 0>& operator=(const array<TYPE, OSIZE> &arr) noexcept { return *this; }
        template<usizet OSIZE>
        inline constexpr array<TYPE, 0>& operator+=(const array<TYPE, OSIZE> &arr) noexcept { return *this; }
        template<usizet OSIZE>
        inline constexpr array<TYPE, 0>& operator-=(const array<TYPE, OSIZE> &arr) noexcept { return *this; }
        template<usizet OSIZE>
        inline constexpr array<TYPE, 0>& operator*=(const array<TYPE, OSIZE> &arr) noexcept { return *this; }
        template<usizet OSIZE>
        inline constexpr array<TYPE, 0>& operator/=(const array<TYPE, OSIZE> &arr) noexcept { return *this; }
        template<usizet OSIZE>
        inline constexpr array<TYPE, 0>& operator%=(const array<TYPE, OSIZE> &arr) noexcept { return *this; }
        template<usizet OSIZE>
        inline constexpr array<TYPE, 0>& operator|=(const array<TYPE, OSIZE> &arr) noexcept { return *this; }
        template<usizet OSIZE>
        inline constexpr array<TYPE, 0>& operator&=(const array<TYPE, OSIZE> &arr) noexcept { return *this; }
        template<usizet OSIZE>
        inline constexpr array<TYPE, 0>& operator^=(const array<TYPE, OSIZE> &arr) noexcept { return *this; }
        template<usizet OSIZE>
        inline constexpr array<TYPE, 0>& operator>>=(const array<TYPE, OSIZE> &arr) noexcept { return *this; }
        template<usizet OSIZE>
        inline constexpr array<TYPE, 0>& operator<<=(const array<TYPE, OSIZE> &arr) noexcept { return *this; }
        inline constexpr array<TYPE, 0>& operator>>=(numbert count) noexcept { return *this; }
        inline constexpr array<TYPE, 0>& operator<<=(numbert count) noexcept { return *this; }
        template<usizet OSIZE>
        inline constexpr array<TYPE, 0> operator+(const array<TYPE, OSIZE> &arr) const noexcept { return array<TYPE, 0>(); }
        template<usizet OSIZE>
        inline constexpr array<TYPE, 0> operator-(const array<TYPE, OSIZE> &arr) const noexcept { return array<TYPE, 0>(); }
        template<usizet OSIZE>
        inline constexpr array<TYPE, 0> operator*(const array<TYPE, OSIZE> &arr) const noexcept { return array<TYPE, 0>(); }
        template<usizet OSIZE>
        inline constexpr array<TYPE, 0> operator/(const array<TYPE, OSIZE> &arr) const noexcept { return array<TYPE, 0>(); }
        template<usizet OSIZE>
        inline constexpr array<TYPE, 0> operator%(const array<TYPE, OSIZE> &arr) const noexcept { return array<TYPE, 0>(); }
        template<usizet OSIZE>
        inline constexpr array<TYPE, 0> operator|(const array<TYPE, OSIZE> &arr) const noexcept { return array<TYPE, 0>(); }
        template<usizet OSIZE>
        inline constexpr array<TYPE, 0> operator&(const array<TYPE, OSIZE> &arr) const noexcept { return array<TYPE, 0>(); }
        template<usizet OSIZE>
        inline constexpr array<TYPE, 0> operator^(const array<TYPE, OSIZE> &arr) const noexcept { return array<TYPE, 0>(); }
        template<usizet OSIZE>
        inline constexpr array<TYPE, 0> operator>>(const array<TYPE, OSIZE> &arr) const noexcept { return array<TYPE, 0>(); }
        template<usizet OSIZE>
        inline constexpr array<TYPE, 0> operator<<(const array<TYPE, OSIZE> &arr) const noexcept { return array<TYPE, 0>(); }
        inline constexpr array<TYPE, 0> operator>>(numbert count) const noexcept { return array<TYPE, 0>(); }
        inline constexpr array<TYPE, 0> operator<<(numbert count) const noexcept { return array<TYPE, 0>(); }
    };

    template<typename TYPE, usizet SIZE>
    inline std::ostream& operator<<(std::ostream& os, const array<TYPE, SIZE> &arr) {
        if (SIZE) os << arr[0];
        for (indext i = 1; i < SIZE; i++) os << ", " << arr[i];
        return os;
    }

    template<typename TYPE>
    inline std::ostream& operator<<(std::ostream& os, const array<TYPE, 0> &arr) { return os; }
}

#endif
