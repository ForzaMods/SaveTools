//
// Created by merika on 9/17/2024.
//

#include "CMac.hpp"

#include <functional>
#include <algorithm>
#include <ranges>

namespace forza_saveswapper {

    CMac::CMac(
        std::span<const std::array<std::array<uint8_t, 4>, 4>, 13> keys,
        std::array<std::array<std::array<std::array<uint8_t, 4>, 256>, 16>, 13> &tables
    ) : CEncryption(iv_placeholder_, keys, tables) {

    }

    void CMac::Calculate(std::vector<uint8_t> &data, std::array<uint8_t, 16> &dst) {
      size_t full_block_count = data.size() / 16;
      size_t partial_block_size = data.size() % 16;
      size_t block_count = full_block_count + (partial_block_size != 0);
      std::array<uint8_t, 16> last_block_scramble{};
      std::array<uint8_t, 16> temp{};
      std::array<uint8_t, 16> temp2{};
      std::array<uint8_t, 16> partial_block_scramble{};
      std::array<uint8_t, 16> iv{}; // not iv

      std::vector<std::span<uint8_t, 16>> blocks;
      blocks.reserve(full_block_count);
      auto data_it = std::begin(data);
      for (size_t i = 0; i < full_block_count; i++) {
        blocks.push_back(std::span<uint8_t, 16>(data_it, 16));
        std::advance(data_it, 16);
      }

      ProcessBlock(std::span(iv), temp);
      CircularShift(temp, last_block_scramble);
      for (size_t i = 0; i < block_count - 1; i++) { // blocks | take(blocks.size() - 2)
        std::transform(std::begin(blocks[i]), std::end(blocks[i]), std::begin(temp2), std::begin(temp), std::bit_xor<uint8_t>()); // https://en.wikipedia.org/wiki/ISO/IEC_9797-1#Iteration
        ProcessBlock(temp, temp2);
      }
      if (partial_block_size) {
        std::span<uint8_t> partial_block = std::span(data_it, partial_block_size);
        std::array<uint8_t, 15> padding = {0x80}; // https://en.wikipedia.org/wiki/ISO/IEC_9797-1#Padding_method_2
        std::array<std::span<uint8_t>, 2> padded_block = {partial_block, padding};
        std::transform(std::begin(temp2), std::end(temp2), std::begin(padded_block | std::views::join), std::begin(temp), std::bit_xor<uint8_t>()); // Update(padded_block, temp)
        CircularShift(last_block_scramble, partial_block_scramble); // TODO: inplace last_block_scramble shift
        std::transform(std::begin(temp), std::end(temp), std::begin(partial_block_scramble), std::begin(temp), std::bit_xor<uint8_t>()); // Update(last_block_scramble, dst); common
      } else {
        std::transform(std::begin(temp2), std::end(temp2), std::begin(blocks.back()), std::begin(temp), std::bit_xor<uint8_t>()); // Update(blocks[n], temp)
        std::transform(std::begin(temp), std::end(temp), std::begin(last_block_scramble), std::begin(temp), std::bit_xor<uint8_t>()); // Update(last_block_scramble, dst)
      }

        ProcessBlock(temp, dst);
    }

    void CMac::CircularShift(const std::array<uint8_t, 16> &src, std::array<uint8_t, 16> &dst) {
        uint8_t a;
        if (src[0] >> 7) // if high bit exist
            a = 135;
        else
            a = 0;
        dst[15] = a ^ (src[15] << 1);
        for (int i = 14; i >= 0; i--) { // left bit shift of whole array
            dst[i] = (src[i + 1] >> 7) | (src[i] << 1);
        }
    }
} // forza_saveswapper