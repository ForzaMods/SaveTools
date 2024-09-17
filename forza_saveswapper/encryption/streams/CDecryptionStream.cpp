//
// Created by merika on 9/17/2024.
//

#include "CDecryptionStream.hpp"
#include "../tfit/CDecryption.hpp"

#include <vector>
#include <fstream>
#include <istream>

namespace forza_saveswapper {

    CDecryptionStream::CDecryptionStream(std::ifstream &input)
        : iv_(),
          input_(input) {

    }

    void CDecryptionStream::ReadData(std::iostream &output) {
        std::array<uint8_t, 16> src{};
        std::array<uint8_t, 16> dst{};

        std::vector<uint8_t> dst_data_block(context_->data_block_size);

        CDecryption<17> cipher(iv_, context_->decryption_keys, context_->decryption_tables);
        for (size_t offset = 0; offset < data_size_; offset += context_->data_block_size) {
            auto dst_data_block_it = std::begin(dst_data_block);
            for (size_t i = 0; i < context_->data_block_size / 16; i++) {
                input_.read(reinterpret_cast<char *>(src.data()), 16);
                cipher.Update(src, dst);
                dst_data_block_it = std::ranges::copy(dst, dst_data_block_it).out;
            }

            input_.read(reinterpret_cast<char *>(src.data()), 16);
            cipher.Update(src, dst);

            if (offset + context_->data_block_size == data_size_ && padding_size_) {
                output.write(reinterpret_cast<char*>(dst_data_block.data()), context_->data_block_size - padding_size_);
            } else {
                output.write(reinterpret_cast<char*>(dst_data_block.data()), context_->data_block_size);
            }
        }
    }


} // forza_saveswapper