//
// Created by merika on 9/17/2024.
//

#ifndef CHASH_HPP
#define CHASH_HPP

#include <cstdint>

namespace forza_saveswapper {

    using HashType = uint32_t;
    constexpr HashType BASE = 0x1505;

    class CHashBase {
    public:
        virtual ~CHashBase() = default;

        constexpr bool operator==(const CHashBase& other) const noexcept {
            return m_Hash == other.m_Hash;
        }

        constexpr bool operator!=(const CHashBase& other) const noexcept {
            return m_Hash != other.m_Hash;
        }

        [[nodiscard]] virtual HashType value() const {
            return m_Hash;
        }

    protected:
        HashType m_Hash = 0;
    };


} // forza_saveswapper

#endif //CHASH_HPP
