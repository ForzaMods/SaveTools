//
// Created by merika on 9/17/2024.
//

#ifndef CDECRYPTIONSTREAM_HPP
#define CDECRYPTIONSTREAM_HPP

#include "../TContext.hpp"

#include <array>
#include <memory>

namespace forza_saveswapper {

    class CDecryptionStream {
    public:
        explicit CDecryptionStream(std::ifstream &input);

        void ReadData(std::iostream &output);
        explicit operator bool() const { return is_open_; }

    protected:
        bool is_open_ = false;
        uint32_t padding_size_ = 0;
        uint32_t data_size_ = 0;

        std::shared_ptr<TContext> context_;
        std::array<uint8_t, 16> iv_;
        std::ifstream &input_;
    };

} // forza_saveswapper

#endif //CDECRYPTIONSTREAM_HPP
