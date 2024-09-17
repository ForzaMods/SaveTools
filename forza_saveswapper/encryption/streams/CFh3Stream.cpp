//
// Created by merika on 9/17/2024.
//

#include "CFh3Stream.hpp"

#include <fstream>

namespace forza_saveswapper {

    CFh3DecryptionStream::CFh3DecryptionStream(std::ifstream &input, const uint32_t input_size) : CDecryptionStream(input) {
        if (input_size < 36) {
            return;
        }

        std::array<uint8_t, 16> header_mac{};
        std::array<uint8_t, 16> dst{};
        uint32_t data_block_size = 0;
        uint32_t data_size = 0;

        input.read(reinterpret_cast<char *>(iv_.data()), 16);
        padding_size_ = iobinary::Read32LE(std::istreambuf_iterator<char>(input));
        input.read(reinterpret_cast<char *>(header_mac.data()), 16);

        std::vector<uint8_t> header;
        std::fill_n(std::back_inserter(header), 4, 0);
        header.insert(std::end(header), std::begin(iv_), std::end(iv_));
        iobinary::Write32LE(padding_size_, std::back_inserter(header));

        for (auto &context : s_Contexts) {
            constexpr uint32_t block_size = 0x10;
            data_block_size = context.data_block_size;
            data_size = (input_size - (2 * block_size + 4)) / (data_block_size + block_size) * data_block_size;
            iobinary::Write32LE(data_size, std::begin(header));

            CMac mac(context.mac_keys, context.mac_tables);
            mac.Calculate(header, dst);
            if (header_mac == dst) {
                context_ = std::shared_ptr<TContext>(&context, [](void *) {});
                data_size_ = data_size;
                is_open_ = true;
                break;
            }
        }

        if (!is_open_) {
            input.seekg(-36, std::ios_base::cur);
        }
    }

    CFh3EncryptionStream::CFh3EncryptionStream(std::ostream &os, const uint32_t size, const std::array<uint8_t, 16> &iv, TContext &context)
      : CEncryptionStream(os, size, iv, context) {
      padding_size_ = context.data_block_size - size_ % context.data_block_size;
      data_size_ = size_ + padding_size_;

      std::vector<uint8_t> header;
      iobinary::Write32LE(data_size_, std::back_inserter(header));
      header.insert(std::end(header), std::begin(iv_), std::end(iv_));
      iobinary::Write32LE(padding_size_, std::back_inserter(header));
      os.write(reinterpret_cast<char *>(header.data()) + 4, 20);

      std::array<uint8_t, 16> header_mac{};
      mac_.Calculate(header, header_mac);
      os.write(reinterpret_cast<char *>(header_mac.data()), 16);
    }

} // forza_saveswapper