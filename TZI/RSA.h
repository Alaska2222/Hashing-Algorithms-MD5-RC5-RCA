#include <windows.h>
#include <wincrypt.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
#include <Windows.h>
#include "FileHandler.h"
#include "ErrorHandler.h"
#pragma comment(lib, "crypt32.lib")

using namespace std;

class RCA {
public:
    class User {
    private:
        HCRYPTPROV hCryptProv;
        HCRYPTKEY hPrivateKey;
    public:
        HCRYPTKEY hPublicKey;
        User() : hCryptProv(0), hPublicKey(0), hPrivateKey(0) {}
        ~User() {
            if (hPublicKey) CryptDestroyKey(hPublicKey);
            if (hPrivateKey) CryptDestroyKey(hPrivateKey);
            if (hCryptProv) CryptReleaseContext(hCryptProv, 0);
        }
        friend class RCA;
    };

    HANDLE console_color = GetStdHandle(STD_OUTPUT_HANDLE);
    bool InitializeCryptoAPI(User& user);
    bool GenerateRSAKeyPair(User& user);
    bool EncryptMessageToFile(const string& message, HCRYPTKEY hPublicKey, ErrorHandler err, FileHandler handler);
    bool DecryptMessageFromFile(const string& fileName, string& decryptedMessage, User& user);
    bool DisplayPublicKey(HCRYPTKEY hKey);
    int ChooseUser(char& c);
private:
    bool EncryptMessage(const string& message, vector<BYTE>& encryptedData, HCRYPTKEY hPublicKey);
    bool DecryptMessage(vector<BYTE>& encryptedData, string& decryptedMessage, HCRYPTKEY hPrivateKey);
    bool DecryptFile(const string& encryptedFilePath, const string& decryptedFilePath, HCRYPTKEY hPrivateKey);
};
