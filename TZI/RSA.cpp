#include "RSA.h"

bool RCA::InitializeCryptoAPI(User& user) {
	    if (!CryptAcquireContext(&user.hCryptProv, NULL, MS_ENHANCED_PROV, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT)) {
	        SetConsoleTextAttribute(console_color, 12);
	        cerr << "CryptAcquireContext failed: " << GetLastError() << endl;
	        SetConsoleTextAttribute(console_color, 15);
	        return false;
	    }
	    return true;
	}
bool RCA::GenerateRSAKeyPair(User& user) {
	    if (!CryptGenKey(user.hCryptProv, AT_KEYEXCHANGE, RSA1024BIT_KEY | CRYPT_EXPORTABLE, &user.hPrivateKey)) {
	        SetConsoleTextAttribute(console_color, 12);
	        cerr << "CryptGenKey failed for private key: " << GetLastError() << endl;
	        SetConsoleTextAttribute(console_color, 15);
	        return false;
	    }
	
	    DWORD publicKeyLen;
	    if (!CryptExportKey(user.hPrivateKey, 0, PUBLICKEYBLOB, 0, NULL, &publicKeyLen)) {
	        SetConsoleTextAttribute(console_color, 12);
	        cerr << "CryptExportKey failed (length): " << GetLastError() << endl;
	        SetConsoleTextAttribute(console_color, 15);
	        return false;
	    }
	
	    vector<BYTE> publicKeyBlob(publicKeyLen);
	    if (!CryptExportKey(user.hPrivateKey, 0, PUBLICKEYBLOB, 0, publicKeyBlob.data(), &publicKeyLen)) {
	        SetConsoleTextAttribute(console_color, 12);
	        cerr << "CryptExportKey failed: " << GetLastError() << endl;
	        SetConsoleTextAttribute(console_color, 15);
	        return false;
	    }
	
	    if (!CryptImportKey(user.hCryptProv, publicKeyBlob.data(), publicKeyLen, 0, 0, &user.hPublicKey)) {
	        SetConsoleTextAttribute(console_color, 12);
	        cerr << "CryptImportKey failed for public key: " << GetLastError() << endl;
	        SetConsoleTextAttribute(console_color, 15);
	        return false;
	    }
	
	    return true;
	}
bool RCA::EncryptMessage(const string& message, vector<BYTE>& encryptedData, HCRYPTKEY hPublicKey) {
	    DWORD messageLen = static_cast<DWORD>(message.size());
	    DWORD encryptedDataLen = 0;
	
	    if (!CryptEncrypt(hPublicKey, 0, TRUE, 0, NULL, &encryptedDataLen, 0)) {
	        SetConsoleTextAttribute(console_color, 12);
	        cerr << "CryptEncrypt failed when calculating buffer size: " << GetLastError() << endl;
	        SetConsoleTextAttribute(console_color, 15);
	        return false;
	    }
	
	    encryptedData.resize(encryptedDataLen);
	    copy(message.begin(), message.end(), encryptedData.begin());
	
	    encryptedDataLen = messageLen;
	
	    if (!CryptEncrypt(hPublicKey, 0, TRUE, 0, encryptedData.data(), &encryptedDataLen, encryptedData.size())) {
	        SetConsoleTextAttribute(console_color, 12);
	        cerr << "CryptEncrypt failed: " << GetLastError() << endl;
	        SetConsoleTextAttribute(console_color, 15);
	        return false;
	    }
	
	    encryptedData.resize(encryptedDataLen);
	
	    cout << "\nEncrypted Message:" << endl;
	    SetConsoleTextAttribute(console_color, 10);
	    for (BYTE byte : encryptedData) {
	        cout << setw(2) << setfill('0') << byte;
	    }
	    SetConsoleTextAttribute(console_color, 15);
	    cout << endl;
	    return true;
	}
bool RCA::DecryptMessage(vector<BYTE>& encryptedData, string& decryptedMessage, HCRYPTKEY hPrivateKey) {
	    DWORD decryptedDataLen = encryptedData.size();
	    if (!CryptDecrypt(hPrivateKey, 0, TRUE, 0, encryptedData.data(), &decryptedDataLen)) {
	        DWORD err = GetLastError();
	        SetConsoleTextAttribute(console_color, 12);
	        if (err == 87) {
	            cout << "\nCan not decrypt with your KEY. Must be an error" << endl;
	        }
	        else {
	            cerr << "CryptDecrypt failed: " << err << endl;
	        }
	        SetConsoleTextAttribute(console_color, 15);
	        return false;
	    }
	
	    decryptedMessage.assign(encryptedData.begin(), encryptedData.begin() + decryptedDataLen);
	    return true;
	}
bool RCA::EncryptMessageToFile(const string& message, HCRYPTKEY hPublicKey, ErrorHandler err, FileHandler handler) {
	    vector<BYTE> encryptedData;
	    if (!EncryptMessage(message, encryptedData, hPublicKey)) {
	        return false;
	    }
	
	    if (err.getYesNoInput("(1) Do you want to save the data to a file? (Y/N): ")) {
	        bool appendData = false;
	        string fullFilePath_ = handler.handleFileInputRC5(appendData);
	        if (!fullFilePath_.empty()) {
	            try {
	                ofstream outFile(fullFilePath_, ios::out | ios::binary);
	                if (!outFile) {
	                    SetConsoleTextAttribute(console_color, 12);
	                    cerr << "Failed to open file for writing: " << fullFilePath_ << endl;
	                    SetConsoleTextAttribute(console_color, 15);
	                    return false;
	                }
	
	                outFile.write(reinterpret_cast<const char*>(encryptedData.data()), encryptedData.size());
	                outFile.close();
	
	                cout << "\nMessage encrypted and saved to file: ";
	                SetConsoleTextAttribute(console_color, 10);
	                cout << fullFilePath_ << endl;
	                SetConsoleTextAttribute(console_color, 15);
	            }
	            catch (const exception& e) {
	                cout << "Error: " << e.what() << endl;
	            }
	        }
	    }
	
	    
	    return true;
	}
bool RCA::DecryptMessageFromFile(const string& fileName, string& decryptedMessage, User& user) {
	    ifstream inFile(fileName, ios::in | ios::binary);
	    if (!inFile) {
	        SetConsoleTextAttribute(console_color, 12);
	        cerr << "Failed to open file for reading: " << fileName << endl;
	        SetConsoleTextAttribute(console_color, 15);
	        return false;
	    }
	
	    vector<BYTE> encryptedData((istreambuf_iterator<char>(inFile)), (istreambuf_iterator<char>()));
	    inFile.close();
	
	    if (!DecryptMessage(encryptedData, decryptedMessage, user.hPrivateKey)) {
	        return false;
	    }
	    return true;
	}
bool RCA::DecryptFile(const string& encryptedFilePath, const string& decryptedFilePath, HCRYPTKEY hPrivateKey) {
	    ifstream inFile(encryptedFilePath, ios::in | ios::binary);
	    if (!inFile) {
	        cerr << "Failed to open encrypted file for reading: " << encryptedFilePath << endl;
	        return false;
	    }
	
	    vector<BYTE> encryptedData((istreambuf_iterator<char>(inFile)), (istreambuf_iterator<char>()));
	    inFile.close();
	
	    string decryptedMessage;
	    if (!DecryptMessage(encryptedData, decryptedMessage, hPrivateKey)) {
	        cerr << "Failed to decrypt file data." << endl;
	        return false;
	    }
	
	    ofstream outFile(decryptedFilePath, ios::out | ios::binary);
	    if (!outFile) {
	        cerr << "Failed to open file for writing: " << decryptedFilePath << endl;
	        return false;
	    }
	
	    outFile.write(decryptedMessage.c_str(), decryptedMessage.size());
	    outFile.close();
	
	    cout << "File decrypted and saved to: " << decryptedFilePath << endl;
	    return true;
	}
bool RCA::DisplayPublicKey(HCRYPTKEY hKey) {
	    DWORD publicKeyLen;
	    if (!CryptExportKey(hKey, 0, PUBLICKEYBLOB, 0, NULL, &publicKeyLen)) {
	        cerr << "CryptExportKey failed (length): " << GetLastError() << endl;
	        return false;
	    }
	
	    vector<BYTE> publicKeyBlob(publicKeyLen);
	    if (!CryptExportKey(hKey, 0, PUBLICKEYBLOB, 0, publicKeyBlob.data(), &publicKeyLen)) {
	        cerr << "CryptExportKey failed: " << GetLastError() << endl;
	        return false;
	    }
	
	    SetConsoleTextAttribute(console_color, 10);
	    for (BYTE b : publicKeyBlob) {
	        cout << hex << int(b);
	    }
	    SetConsoleTextAttribute(console_color, 15);
	    cout << endl;
	
	    return true;
	}
	
	
int RCA::ChooseUser( char& c) {
	    int userChoice;
	    if (c == 'e') {
	        cout << "\nChoose a user to encrypt data:\n(1) User 1\n(2) User 2\n: ";
	    }
	    else if (c == 'd') {
	        cout << "\nChoose a user to decrypt data:\n(1) User 1\n(2) User 2\n: ";
	    }
	    cin >> userChoice;
	    return userChoice;
	}