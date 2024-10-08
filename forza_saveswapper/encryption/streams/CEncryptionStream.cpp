//
// Created by merika on 9/17/2024.
//

#include "CEncryptionStream.hpp"

#include <algorithm>
#include <istream>

namespace forza_saveswapper {
    CEncryptionStream::CEncryptionStream(
        std::ostream &os,
        const uint32_t size,
        const std::array<uint8_t, 16> &iv, TContext &context
    ) : size_(size),
        context_(context),
        iv_(iv),
        os_(os),
        mac_(context.mac_keys, context.mac_tables) {

    }

    void CEncryptionStream::WriteData(std::istream &is) {
        std::array<uint8_t, 16> src{};
        std::array<uint8_t, 16> dst{};
        std::array<uint8_t, 16> mac{};

        std::vector<uint8_t> src_data_block(context_.data_block_size);

        CEncryption cipher(iv_, context_.encryption_keys, context_.encryption_tables);
        for (size_t offset = 0; offset < data_size_; offset += context_.data_block_size) {
            if (offset + context_.data_block_size == data_size_ && padding_size_) {
                is.read(reinterpret_cast<char *>(src_data_block.data()), src_data_block.size() - padding_size_);
                std::fill(std::end(src_data_block) - padding_size_, std::end(src_data_block), 0);
            } else {
                is.read(reinterpret_cast<char *>(src_data_block.data()), src_data_block.size());
            }

            auto src_data_block_it = std::begin(src_data_block);
            for (size_t i = 0; i < context_.data_block_size / 16; i++) {
                std::copy_n(src_data_block_it, 16, std::begin(src));
                cipher.Update(src, dst);
                std::advance(src_data_block_it, 16);
                os_.write(reinterpret_cast<char *>(dst.data()), 16);
            }

            mac_.Calculate(src_data_block, mac);
            cipher.Update(mac, dst);
            os_.write(reinterpret_cast<char *>(dst.data()), 16);
        }
    }

} // forza_saveswapper