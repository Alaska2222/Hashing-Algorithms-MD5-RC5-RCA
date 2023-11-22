#include "lab4.h"
#include"FileHandler.h"
#include"ErrorHandler.h"
#include"RSA.h"

using namespace std;

void lab4::runLab4() {
    RCA rca;
    RCA::User user1, user2;
    HANDLE console_color = GetStdHandle(STD_OUTPUT_HANDLE);
    FileHandler handler;
    ErrorHandler err;
    rca.InitializeCryptoAPI(user1);
    rca.InitializeCryptoAPI(user2);

    rca.GenerateRSAKeyPair(user1);
    rca.GenerateRSAKeyPair(user2);
    char choice;
    cout << "-----------RCA ALGORITHM-------------" << endl << endl;
    while (true) {
        cout << "Choose an option:" << endl;
        cout << "(0) Show public keys" << endl;
        cout << "(1) Encrypt a string" << endl;
        cout << "(2) Decrypt a file" << endl;
        cout << "(3) Encrypt a file" << endl;
        cout << "(e) Exit RCA algorithm" << endl;
        cout << ": ";
        cin >> choice;

        if (choice == 'e') {
            system("cls");
            break;
        }
        else if (choice == '0') {
            cout << "User 1 Public Key: " << endl;
            rca.DisplayPublicKey(user1.hPublicKey);

            cout << "\nUser 2 Public Key: " << endl;
            rca.DisplayPublicKey(user2.hPublicKey);
        }
        else if (choice == '1') {
            string message;
            char c = 'e';
            RCA::User* selectedUser_enc = (rca.ChooseUser(c) == 1) ? &user1 : &user2;
            cout << "Enter a message to send: ";
            cin.ignore();
            getline(cin, message);
            rca.EncryptMessageToFile(message, selectedUser_enc->hPublicKey, err, handler);
        }
        else if (choice == '2') {
            string path, decryptedMessage;
            char c1 = 'd';
            RCA::User* selectedUser_dec = (rca.ChooseUser(c1) == 1) ? &user1 : &user2;

            cout << endl << "Enter a FULL path to file like this C:\\Users\\sindr\\Desktop\\f1.txt: ";
            cin >> path;
            string content = handler.readFile(path);
            if (rca.DecryptMessageFromFile(path, decryptedMessage, *selectedUser_dec)) {
                cout << "\nDecrypted Message: ";
                SetConsoleTextAttribute(console_color, 10);
                cout << decryptedMessage << endl;
                SetConsoleTextAttribute(console_color, 15);

                if (err.getYesNoInput("(1) Do you want to save data to a file? (Y/N): ")) {
                    bool appendData = false;
                    string fullFilePath = handler.handleFileInput(appendData);
                    if (!fullFilePath.empty()) {
                        try {
                            string data = "\Decrypted message: " + decryptedMessage;
                            handler.saveDataToFile(fullFilePath, data, appendData);
                        }
                        catch (const exception& e) {
                            cout << "Error: " << e.what() << endl;
                        }
                    }
                }
            }
            else {
                break;
            }
        }

        else if (choice == '3') {
            string message, path;
            ifstream nameFileout;
            cout << endl << "Enter a FULL path to file like this C:\\Users\\sindr\\Desktop\\f1.txt: ";
            cin >> path;
             nameFileout.open(path);
             std::getline(nameFileout, message);
             nameFileout.close();
            char c = 'e';
            RCA::User* selectedUser_enc = (rca.ChooseUser(c) == 1) ? &user1 : &user2;
            rca.EncryptMessageToFile(message, selectedUser_enc->hPublicKey, err, handler);
        }
        else {
            SetConsoleTextAttribute(console_color, 12);
            cout << "Invalid choice. Please enter '0' to show public keys, '1' to encrypt a string,\n'2' to decrypt a string, or 'e' to exit." << endl;
            SetConsoleTextAttribute(console_color, 15);
        }
    }
}
