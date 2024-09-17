//
// Created by merika on 9/17/2024.
//

#ifndef TCONTEXT_HPP
#define TCONTEXT_HPP

#include "EKeyType.hpp"
#include "EGameType.hpp"

#include <cstdint>
#include <memory>
#include <span>

namespace forza_saveswapper {

    struct TContext {
        EGameType game_type;
        EKeyType key_type;
        uint32_t data_block_size;
        std::span<const std::array<std::array<uint8_t, 4>, 4>, 17> encryption_keys;
        std::span<const std::array<std::array<uint8_t, 4>, 4>, 17> decryption_keys;
        std::span<const std::array<std::array<uint8_t, 4>, 4>, 13> mac_keys;
        std::array<std::array<std::array<std::array<uint8_t, 4>, 256>, 16>, 17> &encryption_tables;
        std::array<std::array<std::array<std::array<uint8_t, 4>, 256>, 16>, 17> &decryption_tables;
        std::array<std::array<std::array<std::array<uint8_t, 4>, 256>, 16>, 13> &mac_tables;
    };

}

#endif //TCONTEXT_HPP
