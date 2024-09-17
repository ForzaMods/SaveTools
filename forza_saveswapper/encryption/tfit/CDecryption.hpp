//
// Created by merika on 9/17/2024.
//

#ifndef CDECRYPTION_HPP
#define CDECRYPTION_HPP

#include "CTFIT.hpp"

#include <array>
#include <span>

namespace forza_saveswapper {

    template<size_t Rounds>
    class CDecryption final : public CTFIT<Rounds>  {
    public:
        CDecryption(
            std::array<uint8_t, 16> &previous,
            std::span<const std::array<std::array<uint8_t, 4>, 4>, Rounds> keys,
            std::array<std::array<std::array<std::array<uint8_t, 4>, 256>, 16>, Rounds> &tables
        );

        void Update(std::array<uint8_t, 16> &src, std::array<uint8_t, 16> &dst);

    private:
        void RoundB(
            std::array<uint8_t, 16> &data,
            const std::array<std::array<uint8_t, 4>, 4> &key,
            std::array<std::array<std::array<uint8_t, 4>, 256>, 16> &table
        ) override;
    };

} // forza_saveswapper

#endif //CDECRYPTION_HPP
