/*

MIT License

Copyright (c) 2019 Daniel Krupiński

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

#pragma once

#include <cstdint>

using hash32_t = std::uint32_t;

namespace fnv {
    constexpr hash32_t offsetBasis = 0x811c9dc5;
    constexpr hash32_t prime = 0x1000193;

    constexpr hash32_t hash(const char* str, const hash32_t value = offsetBasis) noexcept
    {
        return *str ? hash(str + 1, (value ^ *str) * static_cast<unsigned long long>(prime)) : value;
    }

    constexpr hash32_t hashRuntime(const char* str) noexcept
    {
        auto value = offsetBasis;

        while (*str) {
            value ^= *str++;
            value *= prime;
        }
        return value;
    }

    // FNV1a c++11 constexpr compile time hash functions, 32 and 64 bit
    // str should be a null terminated string literal, value should be left out 
    // e.g hash_32_fnv1a_const("example")
    // code license: public domain or equivalent
    // post: https://notes.underscorediscovery.com/constexpr-fnv1a/

    constexpr uint32_t val_32_const = 0x811c9dc5;
    constexpr uint32_t prime_32_const = 0x1000193;

    inline constexpr uint32_t hash_32_fnv_const(const char* const str, const uint32_t value = val_32_const) noexcept {
        return (str[0] == '\0') ? value : hash_32_fnv_const(&str[1], (value ^ uint32_t(str[0])) * prime_32_const);
    }
}
