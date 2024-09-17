//
// Created by merika on 9/17/2024.
//

#ifndef CMAC_HPP
#define CMAC_HPP

#include <vector>

#include "CEncryption.hpp"

namespace forza_saveswapper {

    class CMac final : public CEncryption<13> {
    public:
        CMac(
            std::span<const std::array<std::array<uint8_t, 4>, 4>, 13> keys,
            std::array<std::array<std::array<std::array<uint8_t, 4>, 256>, 16>, 13> &tables
        );

        void Calculate(std::vector<uint8_t> &data, std::array<uint8_t, 16> &dst);

    private:
        static void CircularShift(const std::array<uint8_t, 16> &src, std::array<uint8_t, 16> &dst);
        std::array<uint8_t, 16> iv_placeholder_;
    };

} // forza_saveswapper

#endif //CMAC_HPP
