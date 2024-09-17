#ifndef CBINARYSTREAM_HPP
#define CBINARYSTREAM_HPP

#include <iostream>
#include <memory>

namespace forza_saveswapper {

    class CBinaryStream {
    public:
        explicit CBinaryStream(const std::shared_ptr<std::iostream>& stream)
            : m_Stream(stream) {}

        template <typename T>
        T Read() {
            T value;
            m_Stream->read(reinterpret_cast<char*>(&value), sizeof(T));
            return value;
        }

        template <typename T>
        void Write(const T& value) {
            m_Stream->write(reinterpret_cast<const char*>(&value), sizeof(T));
        }

        void SetPosition(const std::streampos pos) const {
            m_Stream->seekg(pos);
            m_Stream->seekp(pos);
        }

        [[nodiscard]] std::streampos GetPosition() const {
            return m_Stream->tellg();
        }

        void ClearStreamFlags() const {
            m_Stream->clear();
        }

    private:
        std::shared_ptr<std::iostream> m_Stream;
    };

}  // namespace forza_saveswapper

#endif  // CBINARYSTREAM_HPP
