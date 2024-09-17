//
// Created by merika on 9/17/2024.
//

#ifndef CCOMPILETIMEHASH_HPP
#define CCOMPILETIMEHASH_HPP

#include "CHashBase.hpp"

namespace forza_saveswapper {

    class CCompileTimeHash final : public CHashBase {
    public:
        template <typename T>
        __forceinline consteval explicit CCompileTimeHash(T* str) {
            int strLen = 0;
            while(str[strLen]) {
                ++strLen;
            }

            HashType hash = BASE;
            for (auto i = 0; i < strLen; i++) {
                hash ^= str[i] + (hash >> 2) + 32 * hash;
            }

            for (auto i = 0; i < strLen; i++) {
                hash ^= str[i] + (hash >> 2) + 32 * hash;
            }

            m_Hash = hash;
        }
    };

} // forza_saveswapper

#endif //CCOMPILETIMEHASH_HPP
