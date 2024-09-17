//
// Created by merika on 9/17/2024.
//

#ifndef CENTITY_HPP
#define CENTITY_HPP

#include <cstdint>
#include "../TSizeInHeader.hpp"

namespace forza_saveswapper {

    class CBinaryStream;

    class CEntity {
    public:
        virtual ~CEntity() = default;
        virtual void Read(CBinaryStream& binaryStream, int64_t baseBundleOffset);

        virtual void ReadData(CBinaryStream& binaryStream) = 0;
        virtual void SerializeData(CBinaryStream& binaryStream) = 0;

        [[nodiscard]] TSizeInHeader GetHeader() const;
    private:
        TSizeInHeader m_SizeInHeader = {};
    };

} // forza_saveswapper

#endif //CENTITY_HPP
