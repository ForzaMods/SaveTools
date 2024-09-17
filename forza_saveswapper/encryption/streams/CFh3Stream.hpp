//
// Created by merika on 9/17/2024.
//

#ifndef CFH3STREAM_HPP
#define CFH3STREAM_HPP

#include "../TContext.hpp"
#include "../keys/Fh4Keys.hpp"
#include "../keys/Fh5Keys.hpp"

#include <vector>

#include "CDecryptionStream.hpp"
#include "CEncryptionStream.hpp"

namespace forza_saveswapper {

    namespace iobinary {
        template<class It>
        uint32_t Read32LE(It it) {
            uint32_t result = 0;
            for (size_t i = 0; i < 4; i++) {
                result |= static_cast<uint8_t>(*(it++)) << (8 * i);
            }
            return result;
        }

        template<class It>
        uint32_t Read16LE(It it) {
            uint32_t result = 0;
            for (size_t i = 0; i < 2; i++) {
                result |= static_cast<uint8_t>(*(it++)) << (8 * i);
            }
            return result;
        }

        template<class It>
        void Write32LE(const uint32_t value, It it) {
            for (size_t i = 0; i < 4; i++) {
                *(it++) = value >> (8 * i);
            }
        }

        template<class It>
        void Write16LE(const uint32_t value, It it) {
            for (size_t i = 0; i < 2; i++) {
                *(it++) = value >> (8 * i);
            }
        }
    }

    class CFh3DecryptionStream : public CDecryptionStream {
    public:
        CFh3DecryptionStream(std::ifstream& input, uint32_t input_size);
    };

    class CFh3EncryptionStream : public CEncryptionStream {
    public:
        CFh3EncryptionStream(std::ostream &os, uint32_t size, const std::array<uint8_t, 16> &iv, TContext &context);
    };

    static std::vector<TContext> s_Contexts = {
        {
            EGameType::FH5_v1_619_349_0,
            EKeyType::Profile,
            0x200,
            std::span(Fh5Keys::fh5_v1_619_349_0_profile_encryption_keys).subspan<1, 17>(),
            std::span(Fh5Keys::fh5_v1_619_349_0_profile_decryption_keys).subspan<1, 17>(),
            std::span(Fh5Keys::fh5_v1_619_349_0_profile_mac_keys).subspan<1, 13>(),
            Fh4Keys::fh4_encryption_tables,
            Fh4Keys::fh4_decryption_tables,
            Fh4Keys::fh4_mac_tables
        },
        {
            EGameType::FH5,
            EKeyType::Profile,
            0x200,
            std::span(Fh5Keys::fh5_profile_encryption_keys).subspan<1, 17>(),
            std::span(Fh5Keys::fh5_profile_decryption_keys).subspan<1, 17>(),
            std::span(Fh5Keys::fh5_profile_mac_keys).subspan<1, 13>(),
            Fh4Keys::fh4_encryption_tables,
            Fh4Keys::fh4_decryption_tables,
            Fh4Keys::fh4_mac_tables
        }
    };


} // forza_saveswapper

#endif //CFH3STREAM_HPP
