#include "Encryptor.h"

Encryptor::Encryptor()
{
}

std::vector<char> Encryptor::getCrc32(const std::vector<char> &message, const int length) const
{
    boost::crc_32_type result;
    result.process_bytes(message.data(), length);
    std::vector<char> crc = {
        static_cast<char>((result.checksum() >> 24) & 0xff),
        static_cast<char>((result.checksum() >> 16) & 0xff),
        static_cast<char>((result.checksum() >> 8) & 0xff),
        static_cast<char>(result.checksum() & 0xff)};
    return crc;
}