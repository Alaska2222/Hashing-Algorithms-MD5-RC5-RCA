#ifndef RC5_H
#define RC5_H

#include <vector>
#include <string>

class RC5 {
public:
    RC5(const std::string& key_str);
    std::vector<uint8_t> encrypt(const std::vector<uint8_t>& mess, const std::vector<uint8_t>& IV_h);
    std::vector<uint8_t> decrypt(const std::vector<uint8_t>& mess, const std::vector<uint8_t>& IV_h);
    std::vector<uint8_t> generate_IV();
    void encryptFile(const std::string& inputFilePath, const std::string& outputFilePath, const  std::vector<uint8_t>& IV_h);
    void decryptFile(const std::string& inputFilePath, const std::string& outputFilePath, const  std::vector<uint8_t>& IV_h);
private:
    std::vector<uint8_t> key;
    std::vector<uint8_t> IV;
    std::vector<uint8_t> generate_key(const std::string& key_str);
    std::vector<uint8_t> to_bytes(uint64_t value, size_t size);
    uint64_t ROTL(uint64_t val, uint64_t r_bits);
    uint64_t ROTR(uint64_t val, uint64_t r_bits);
    uint32_t from_bytes(const std::vector<uint8_t>& bytes, bool little_endian = true);
    std::pair<std::vector<uint64_t>, std::vector<uint64_t>> expansion();
    std::vector<uint8_t> encrypt_block(const std::vector<uint8_t>& data);
    std::vector<uint8_t> decrypt_block(const std::vector<uint8_t>& data);
};

#endif 
