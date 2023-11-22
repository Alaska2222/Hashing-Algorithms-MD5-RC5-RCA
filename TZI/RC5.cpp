#include "RC5.h"
#include"MD5.h"
#include"PseudoRandGenerator.h"
#include"Constants_l1.h"
#include<iomanip>
#include <climits>
#include"Constants_l3.h"
#include<fstream>

RC5::RC5(const string& key_str) {
    this->key = generate_key(key_str);
    this->IV = generate_IV();
}

vector<uint8_t> RC5::generate_key(const string& key_str) {
    MD5Algorithm md5;
    string hash = md5.string_hashing(key_str); //128 bit
    return vector<uint8_t>(hash.begin(), hash.end());
}

vector<uint8_t> RC5::to_bytes(uint64_t value, size_t size) {
    vector<uint8_t> bytes(size, 0);
    for (size_t i = 0; i < size; ++i) {
        bytes[i] = (value >> (8 * i)) & 0xFF;
    }
    return bytes;
}

vector<uint8_t> RC5::generate_IV() {
    PseudoRandGenerator generator(a, c, m, x0);
    auto result = generator.GenerateRandSequence("1");
    int PseudoRandInt = result.first.front();
    vector<uint8_t> IV = to_bytes(PseudoRandInt, size_);
    return IV;
}

uint64_t RC5::ROTL(uint64_t val, uint64_t r_bits) {
    return ((val << (r_bits % w)) & (module - 1)) | ((val & (module - 1)) >> (w - (r_bits % w)));
}

uint64_t RC5::ROTR(uint64_t val, uint64_t r_bits) {
    return ((val & (module - 1)) >> (r_bits % w)) | ((val << (w - (r_bits % w))) & (module - 1));
}

uint32_t RC5::from_bytes(const vector<uint8_t>& bytes, bool little_endian) {
    uint32_t result = 0;
    if (little_endian) {
        for (int i = bytes.size() - 1; i >= 0; --i) {
            result = (result << 8) | bytes[i];
        }
    }
    else {
        for (uint8_t byte : bytes) {
            result = (result << 8) | byte;
        }
    }
    return result;
}

pair<vector<uint64_t>, vector<uint64_t>> RC5::expansion() {
    vector<uint64_t> L;
    for (int i = 0; i < b; i += w / 8) {
        L.push_back(from_bytes(vector<uint8_t>(key.begin() + i, key.begin() + i + w / 8)));
    }
    vector<uint64_t> S = { P };
    for (int i = 1; i < 2 * (r + 1); ++i) {
        S.push_back((S[i - 1] + Q) % module);
    }

    uint64_t A = 0, B = 0;
    uint64_t i = 0, j = 0;
    int len = max(3 * (int)key.size(), 3 * 2 * (r + 1));
    for (int cnt = 0; cnt < len; ++cnt) {
        A = S[i] = ROTL(S[i] + A + B, 3);
        B = L[j] = ROTL(L[j] + A + B, A + B);
        i = (i + 1) % (2 * (r + 1));
        j = (j + 1) % L.size();

    }

    return { L, S };
}

vector<uint8_t> RC5::encrypt_block(const vector<uint8_t>& data) {
    pair<vector<uint64_t>, vector<uint64_t>> tmp = expansion();
    vector<uint64_t> L = tmp.first;
    vector<uint64_t> S = tmp.second;
    uint32_t A, B;


    memcpy(&A, data.data(), sizeof(A));
    memcpy(&B, data.data() + sizeof(A), sizeof(B));

    A = (A + S[0]) % module;
    B = (B + S[1]) % module;

    for (int i = 1; i <= r; ++i) {
        A = (ROTL(A ^ B, B) + S[2 * i]) % module;
        B = (ROTL(B ^ A, A) + S[2 * i + 1]) % module;
    }

    vector<uint8_t> result;
    result.resize(2 * sizeof(uint32_t));
    memcpy(result.data(), &A, sizeof(A));
    memcpy(result.data() + sizeof(A), &B, sizeof(B));

    return result;
}

vector<uint8_t> RC5::decrypt_block(const vector<uint8_t>& data) {
    uint64_t A = 0, B = 0;
    size_t w_8 = w / 8;

    pair<vector<uint64_t>, vector<uint64_t>> tmp = expansion();
    vector<uint64_t> L = tmp.first;
    vector<uint64_t> S = tmp.second;
    for (size_t i = 0; i < w_8; ++i) {
        A |= static_cast<uint64_t>(data[i]) << (8 * i);
        B |= static_cast<uint64_t>(data[i + w_8]) << (8 * i);
    }

    for (int i = r; i > 0; --i) {
        B = ROTR(B - S[2 * i + 1], A) ^ A;
        A = ROTR((A - S[2 * i]), B) ^ B;
    }

    B = (B - S[1]) % module;
    A = (A - S[0]) % module;

    vector<uint8_t> result(2 * w_8);
    for (size_t i = 0; i < w_8; ++i) {
        result[i] = static_cast<uint8_t>((A >> (8 * i)) & 0xFF);
        result[i + w_8] = static_cast<uint8_t>((B >> (8 * i)) & 0xFF);
    }

    return result;
}

vector<uint8_t> RC5::encrypt(const vector<uint8_t>& mess_, const vector<uint8_t>& IV_h) {
    vector<uint8_t> IV = IV_h;
    vector<uint8_t> IV_enc = encrypt_block(IV);
    vector<uint8_t> out;
    size_t index = 0;

    while (index < mess_.size()) {
        vector<uint8_t> block(mess_.begin() + index, mess_.begin() + min(index + bSize / 8, mess_.size()));
        block.resize(bSize / 8, 0);

        for (size_t i = 0; i < block.size(); ++i) {
            block[i] ^= IV[i];
        }

        vector<uint8_t> encrypted = encrypt_block(block);
        out.insert(out.end(), encrypted.begin(), encrypted.end());

        index += bSize / 8;
        IV = encrypted;
    }

    return out;
}

vector<uint8_t> RC5::decrypt(const vector<uint8_t>& mess, const vector<uint8_t>& IV_h) {
    size_t index = bSize / 8;
    vector<uint8_t> out;
    for (size_t i = mess.size(); i > 0; i -= index) {
        vector<uint8_t> block(mess.begin() + max(i, index) - index, mess.begin() + min(i, mess.size()));
        vector<uint8_t> decrypted_block = decrypt_block(block);

        vector<uint8_t> prev_block;
        if (i > index) {
            prev_block = vector<uint8_t>(mess.begin() + max(i, 2 * index) - 2 * index, mess.begin() + min(i - index, mess.size()));
        }
        else {
            prev_block = IV_h;
        }

        uint64_t mix = 0;
        for (size_t j = 0; j < decrypted_block.size(); ++j) {
            mix |= static_cast<uint64_t>(decrypted_block[j]) << (8 * j);
        }
        for (size_t j = 0; j < prev_block.size(); ++j) {
            mix ^= static_cast<uint64_t>(prev_block[j]) << (8 * j);
        }

        vector<uint8_t> decrypted_mix(bSize / 8);
        for (size_t j = 0; j < bSize / 8; ++j) {
            decrypted_mix[j] = static_cast<uint8_t>((mix >> (8 * j)) & 0xFF);
        }

        out.insert(out.begin(), decrypted_mix.begin(), decrypted_mix.end());
    }

    while (!out.empty() && out.back() == 0) {
        out.pop_back();
    }
    return out;
}

#include <fstream>

void RC5::encryptFile(const string& inputFilePath, const string& outputFilePath, const vector<uint8_t>& IV_h) {
    std::ifstream inputFile(inputFilePath, std::ios::binary);
    std::ofstream outputFile(outputFilePath, std::ios::binary);

    if (!inputFile.is_open() || !outputFile.is_open()) {
        throw std::runtime_error("Failed to open file(s).");
    }

    std::vector<uint8_t> buffer((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());
    std::vector<uint8_t> encryptedData = encrypt(buffer, IV_h);

    outputFile.write(reinterpret_cast<const char*>(encryptedData.data()), encryptedData.size());
}

void RC5::decryptFile(const std::string& inputFilePath, const std::string& outputFilePath, const vector<uint8_t>& IV_h) {
    std::ifstream inputFile(inputFilePath, std::ios::binary);
    std::ofstream outputFile(outputFilePath, std::ios::binary);

    if (!inputFile.is_open() || !outputFile.is_open()) {
        throw std::runtime_error("Failed to open file(s).");
    }

    std::vector<uint8_t> buffer((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());
    std::vector<uint8_t> decryptedData = decrypt(buffer, IV_h);

    outputFile.write(reinterpret_cast<const char*>(decryptedData.data()), decryptedData.size());
}
