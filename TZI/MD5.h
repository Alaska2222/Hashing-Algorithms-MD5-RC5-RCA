#ifndef MD5_H
#define MD5_H

#include <string>
#include <vector>
using namespace std;

class MD5Algorithm {
public:
    string string_hashing(const string& str);
    string hashFile(const string& filename);
    string getInput(const string& prompt);

private:
    vector<uint8_t> preparation(const string& message);
    string into_hash();
    void MD5(const string& message);
};

#endif