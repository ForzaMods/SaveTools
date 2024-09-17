//
// Created by merika on 9/17/2024.
//

#ifndef CRUNTIMEHASH_HPP
#define CRUNTIMEHASH_HPP
#include "CHashBase.hpp"

namespace forza_saveswapper {

    class CRuntimeHash final : public CHashBase {
    public:
        template <typename T>
        __forceinline explicit CRuntimeHash(const T* data) noexcept {
            int strLen = 0;
            while(data[strLen]) {
                ++strLen;
            }

            m_Hash = ComputeHash(data, strLen);
        }

    private:
        template <typename T>
        static HashType ComputeHash(const T* str, const std::size_t strLen) {
            HashType hash = BASE;
            for (auto i = 0; i < strLen; i++) {
                hash ^= str[i] + (hash >> 2) + 32 * hash;
            }

            for (auto i = 0; i < strLen; i++) {
                hash ^= str[i] + (hash >> 2) + 32 * hash;
            }

            return hash;
        }
    };
} // forza_saveswapper

#endif //CRUNTIMEHASH_HPP
