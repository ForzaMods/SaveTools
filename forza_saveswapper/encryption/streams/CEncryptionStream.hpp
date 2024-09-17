//
// Created by merika on 9/17/2024.
//

#ifndef CENCRYPTIONSTREAM_HPP
#define CENCRYPTIONSTREAM_HPP

#include "../TContext.hpp"
#include "../tfit/CMac.hpp"

#include <cstdint>

namespace forza_saveswapper {

    class CEncryptionStream {
    public:
        CEncryptionStream(std::ostream &os, uint32_t size, const std::array<uint8_t, 16> &iv, TContext &context);
        void WriteData(std::istream &is);

    protected:
        uint32_t size_;
        uint32_t padding_size_ = 0;
        uint32_t data_size_ = 0;

        TContext &context_;
        std::array<uint8_t, 16> iv_;
        std::ostream &os_;

        CMac mac_;
    };


} // forza_saveswapper

#endif //CENCRYPTIONSTREAM_HPP
