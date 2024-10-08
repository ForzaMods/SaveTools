//
// Created by merika on 9/17/2024.
//

#include "CDecryption.hpp"

#include <algorithm>
#include <functional>
#include <cstring>

namespace forza_saveswapper {

    template<size_t Rounds>
    CDecryption<Rounds>::CDecryption(
        std::array<uint8_t, 16> &previous,
        std::span<const std::array<std::array<uint8_t, 4>, 4>, Rounds> keys,
        std::array<std::array<std::array<std::array<uint8_t, 4>, 256>, 16>, Rounds> &tables
    ) : CTFIT<Rounds>(previous, keys, tables) {

    }

    template<size_t Rounds>
    void CDecryption<Rounds>::RoundB(
        std::array<uint8_t, 16> &data,
        const std::array<std::array<uint8_t, 4>, 4> &key,
        std::array<std::array<std::array<uint8_t, 4>, 256>, 16> &table
    ) {
        std::array<uint32_t, 4> temp = {
            *reinterpret_cast<uint32_t *>(table[0][data[0]].data())
              ^ *reinterpret_cast<uint32_t *>(table[7][data[7]].data())
              ^ *reinterpret_cast<uint32_t *>(table[10][data[10]].data())
              ^ *reinterpret_cast<uint32_t *>(table[13][data[13]].data())
              ^ *reinterpret_cast<const uint32_t *>(key[0].data()),
            *reinterpret_cast<uint32_t *>(table[1][data[1]].data())
              ^ *reinterpret_cast<uint32_t *>(table[4][data[4]].data())
              ^ *reinterpret_cast<uint32_t *>(table[11][data[11]].data())
              ^ *reinterpret_cast<uint32_t *>(table[14][data[14]].data())
              ^ *reinterpret_cast<const uint32_t *>(key[1].data()),
            *reinterpret_cast<uint32_t *>(table[2][data[2]].data())
              ^ *reinterpret_cast<uint32_t *>(table[5][data[5]].data())
              ^ *reinterpret_cast<uint32_t *>(table[8][data[8]].data())
              ^ *reinterpret_cast<uint32_t *>(table[15][data[15]].data())
              ^ *reinterpret_cast<const uint32_t *>(key[2].data()),
            *reinterpret_cast<uint32_t *>(table[3][data[3]].data())
              ^ *reinterpret_cast<uint32_t *>(table[6][data[6]].data())
              ^ *reinterpret_cast<uint32_t *>(table[9][data[9]].data())
              ^ *reinterpret_cast<uint32_t *>(table[12][data[12]].data())
              ^ *reinterpret_cast<const uint32_t *>(key[3].data())
          };
        memcpy(data.data(), temp.data(), 16);
    }

    template<size_t Rounds>
    void CDecryption<Rounds>::Update(std::array<uint8_t, 16> &src, std::array<uint8_t, 16> &dst) {
        this->ProcessBlock(src, dst);
        std::transform(std::begin(dst), std::end(dst), std::begin(this->previous_), std::begin(dst), std::bit_xor<uint8_t>());
        this->previous_ = src;
    }

    template class CDecryption<17>;
    template class CDecryption<13>;


} // forza_saveswapper