//
// Created by merika on 9/17/2024.
//

#ifndef CENCRYPTION_HPP
#define CENCRYPTION_HPP

#include "CTFIT.hpp"

namespace forza_saveswapper {

    template<size_t Rounds>
    class CEncryption : public CTFIT<Rounds>  {
    public:
        CEncryption(
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

#endif //CENCRYPTION_HPP
