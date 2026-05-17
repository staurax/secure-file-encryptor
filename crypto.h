#ifndef CRYPTO_H
#define CRYPTO_H

#include <cstdint>
#include <cstddef>
#include <string>
#include <vector>

extern const std::string MAGIC_HEADER;
extern const int KEY_STRETCH;

uint32_t passwordChecksum(const std::string& password);
std::vector<unsigned char> stretchKey(const std::string& password, std::size_t length);
std::vector<unsigned char> xorCrypt(const std::vector<unsigned char>& data,
                                    const std::string& password);
bool encryptFile(const std::string& inputPath,
                 const std::string& outputPath,
                 const std::string& password);
bool decryptFile(const std::string& inputPath,
                 const std::string& outputPath,
                 const std::string& password);

#endif
