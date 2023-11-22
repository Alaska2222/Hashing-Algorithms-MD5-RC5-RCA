#include <iostream>
#include <cstdint>
#include <cmath>
#include <fstream>
#include "MD5.h"
#include "Constants_l2.h"

string MD5Algorithm::string_hashing(const  string& str) {
    MD5(str);
    return into_hash();
}

string MD5Algorithm::getInput(const string& prompt) {
    string input;
    cout << prompt;
    cin.ignore();
    getline(cin, input);
    return input;
}

string MD5Algorithm::hashFile(const  string& filename) {
    try {
        ifstream file(filename, ios::binary);

        if (!file) {
            throw  runtime_error("Unable to open the file for hashing.");
        }

        file.seekg(0, ios::end);
        size_t fileSize = file.tellg();
        file.seekg(0, ios::beg);

        if (fileSize > FILE_SIZE_LIMIT) {
            throw  runtime_error("File size exceeds the limit of 10 megabytes.");
        }

        string fileContents((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());

        return string_hashing(fileContents);
    }
    catch (const  exception& e) {
        cerr << "Error while hashing file: " << e.what() << endl;
        return "";
    }
}

vector<uint8_t> MD5Algorithm::preparation(const  string& message) {
    vector<  uint8_t> messageBytes(message.begin(), message.end());
    uint64_t messageLength = message.size() * 8;
    messageBytes.push_back(0x80);

    while (messageBytes.size() % 64 != 56) {
        messageBytes.push_back(0);
    }

    for (int i = 0; i < 8; ++i) {
        messageBytes.push_back(static_cast<uint8_t>((messageLength >> (i * 8)) & 0xFF));
    }

    return messageBytes;
}

string MD5Algorithm::into_hash() {
    string result;
    for (int i = 0; i < 4; ++i) {
        uint32_t value = ABCD[i];

        for (int j = 0; j < 4; ++j) {
            uint8_t byte = (value >> (j * 8)) & 0xFF;
            char hexChars[3];
            snprintf(hexChars, 3, "%02x", byte);
            result += hexChars;
        }
    }
    return result;
}

void MD5Algorithm::MD5(const  string& message) {
    vector<uint8_t> messageBytes = preparation(message);
    ABCD = { 0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476 };

    for (size_t offset = 0; offset < messageBytes.size(); offset += 64) {
        uint32_t a = ABCD[0];
        uint32_t b = ABCD[1];
        uint32_t c = ABCD[2];
        uint32_t d = ABCD[3];
        const vector<uint8_t>& chunk = vector<uint8_t>(
            messageBytes.begin() + offset, messageBytes.begin() + offset + 64
        );
        int n = 4 * 16;

        for (int i = 0; i < n; ++i) {
            uint32_t value;
            if (0 <= i && i < 16) {
                value = a + F(b, c, d) + T[i] + *reinterpret_cast<const   uint32_t*>(&chunk[4 * i]);
            }
            else if (16 <= i && i < 32) {
                value = a + G(b, c, d) + T[i] + *reinterpret_cast<const   uint32_t*>(&chunk[4 * P2(i)]);
            }
            else if (32 <= i && i < 48) {
                value = a + H(b, c, d) + T[i] + *reinterpret_cast<const   uint32_t*>(&chunk[4 * P3(i)]);
            }
            else {
                value = a + I(b, c, d) + T[i] + *reinterpret_cast<const   uint32_t*>(&chunk[4 * P4(i)]);
            }
            uint32_t tmp = b + LEFT_ROTATE(value, S[i]));
            a = d;
            d = c;
            c = b;
            b = tmp;
        }
        ABCD[0] += a;
        ABCD[1] += b;
        ABCD[2] += c;
        ABCD[3] += d;
    }
}

