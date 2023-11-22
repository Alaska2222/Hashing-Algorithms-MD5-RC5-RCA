#include"lab3.h"
#include"FileHandler.h"
#include"ErrorHandler.h"
#include"RC5.h"
#include <iostream>
#include <vector>
#include<string>
#include<Windows.h>

using namespace std;

void lab3::runLab3() {
    HANDLE console_color = GetStdHandle(STD_OUTPUT_HANDLE);
    FileHandler handler;
    ErrorHandler err;
    string choice;
    cout << "-----------RC5 ALGORITHM-------------" << endl << endl;
    while (true) {
        cout << "Choose an option:" << endl;
        cout << "(0) Encrypt a string" << endl;
        cout << "(1) Decrypt a string file" << endl;
        cout << "(2) Encrypt a file" << endl;
        cout << "(3) Decrypt a non-string file" << endl;
        cout << "(e) Exit RC5 algorithm" << endl;
        cout << ": ";
        cin >> choice;
        if (choice[0] == 'e' && choice.size() == 1) {
            system("cls");
            break;
        }
        else if (choice[0] == '0' && choice.size() == 1) {
            string phrase, key_str;
            cout << endl << "Enter a string to encrypt: ";
            cin.ignore();
            getline(cin, phrase);
            cout << "Enter a secter key: ";
            getline(cin, key_str);
            RC5 rc(key_str);
            vector<uint8_t> mess(phrase.begin(), phrase.end());
            vector<uint8_t> encrypted = rc.encrypt(mess, rc.generate_IV());
            string res(encrypted.begin(), encrypted.end());
            cout << endl << "Encrypted data: ";
            SetConsoleTextAttribute(console_color, 10);
            cout << res << endl;
            SetConsoleTextAttribute(console_color, 15);
            if (err.getYesNoInput("(1) Do you want to save the data to a file? (Y/N): ")) {
                bool appendData = false;
                string fullFilePath_ = handler.handleFileInputRC5(appendData);
                if (!fullFilePath_.empty()) {
                    try {
                        string res(encrypted.begin(), encrypted.end());
                        handler.saveDataToFile(fullFilePath_, res, appendData);
                    }
                    catch (const exception& e) {
                        cout << "Error: " << e.what() << endl;
                    }
                }
            }

        }
        else if (choice[0] == '1' && choice.size() == 1) {
            string key_str1, path;
            cout << "Enter a secter key: ";
            cin >> key_str1;
            RC5 rc(key_str1);
            cout << endl << "Enter a FULL path to file like this C:\\Users\\sindr\\Desktop\\f1.txt: ";
            cin >> path;
            string content = handler.readFile(path);
            vector<uint8_t> vec(content.begin(), content.end() - 1);
            vector<uint8_t> decrypted = rc.decrypt(vec, rc.generate_IV());
            string res(decrypted.begin(), decrypted.end());
            cout << "Decrypted data: ";
            SetConsoleTextAttribute(console_color, 10);
            cout << res << endl;
            SetConsoleTextAttribute(console_color, 15);
        }

        else if (choice[0] == '2' && choice.size() == 1) {
            string key_str;
            RC5 rc5(key_str);
            cout << "Enter a secter key: ";
            cin.ignore();
            getline(cin, key_str);
            string path;
            cout << "Enter a FULL path to file like this C:\\Users\\sindr\\Desktop\\out.txt: ";
            cin >> path;
            if (!handler.fileExists(path)) {
                SetConsoleTextAttribute(console_color, 12);
                cout << "You have entered wrong path name!" << endl;
                SetConsoleTextAttribute(console_color, 15);
                break;
            }
            else {

                bool appendData = false;
                string fullFilePath_ ;
                cout << "Enter a FULL path to save file like this C:\\Users\\sindr\\Desktop\\out.txt: ";
                cin >> fullFilePath_;
                if (!fullFilePath_.empty()) {
                    try {
                        rc5.encryptFile(path, fullFilePath_, rc5.generate_IV());
                        cout << "File succsesfully encrpted at: ";
                        SetConsoleTextAttribute(console_color, 10);
                        cout << fullFilePath_ << endl;
                        SetConsoleTextAttribute(console_color, 15);
                    }
                    catch (const exception& e) {
                        cout << "Error: " << e.what() << endl;
                    }
                }
            }
        }
        else if (choice[0] == '3' && choice.size() == 1) {
            string key_str;
            RC5 rc5(key_str);
            cout << "Enter a secter key: ";
            cin.ignore();
            getline(cin, key_str);
            string path, path1;;
            cout << "Enter a FULL path to encrypded file like this C:\\Users\\sindr\\Desktop\\out.txt: ";
            cin >> path;
            if (!handler.fileExists(path)) {
                SetConsoleTextAttribute(console_color, 12);
                cout << "You have entered wrong path name!" << endl;
                SetConsoleTextAttribute(console_color, 15);
                break;
            }
            else {
                cout << "Enter a FULL path to save file like this C:\\Users\\sindr\\Desktop\\out.txt: ";
                cin >> path1;
                if (!path1.empty()) {
                    try {
                        rc5.decryptFile(path, path1, rc5.generate_IV());
                        cout << "File succsesfully decrypded at: ";
                        SetConsoleTextAttribute(console_color, 10);
                        cout << path1 << endl;
                        SetConsoleTextAttribute(console_color, 15);
                    }
                    catch (const exception& e) {
                        cout << "Error: " << e.what() << endl;
                    }
                }
            }
        }

        else {
            SetConsoleTextAttribute(console_color, 12);
            cout << "Invalid choice. Please enter '0' to encrypt a string, '1' to decrypt a file, or 'e' to exit." << endl;
            SetConsoleTextAttribute(console_color, 15);
        }
        cout << endl;
    }
}