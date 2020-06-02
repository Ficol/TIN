#ifndef ENCRYPTOR_H
#define ENCRYPTOR_H

#include <vector>

#include <boost/crc.hpp>

class Encryptor
{
public:
    Encryptor();
    std::vector<char> getCrc32(const std::vector<char>& message, const int length) const;

};

#endif // ENCRYPTOR_H
