#include "lab2.h"
#include"MD5.h"
#include"FileHandler.h"
#include"ErrorHandler.h"
#include<iostream>
#include<string>
#include <windows.h> 

using namespace std;

void lab2::runLab2() {

    FileHandler handler;
    MD5Algorithm md5;
    ErrorHandler err;
    HANDLE console_color = GetStdHandle(STD_OUTPUT_HANDLE);
    char choice;
    cout << "-----------MD5 ALGORITHM-------------" << endl << endl;
    while (true) {
        cout << "Choose an option:" << endl;
        cout << "(0) Hash a string" << endl;
        cout << "(1) Hash a file" << endl;
        cout << "(e) Exit MD5 algorithm" << endl;
        cout << ": ";
        cin >> choice;

        if (choice == 'e') {
            system("cls");
            break;
        }

        else if (choice == '0') {
            string input = md5.getInput("Enter string to hash:\n* Limitation 5 MB *\n: ");

            if (input.size() > 5000000) {
                SetConsoleTextAttribute(console_color, 12);
                cout << "* Input string exceeds the 5 megabytes limit. Aborting. *" << endl;
                SetConsoleTextAttribute(console_color, 15);
                continue;
            }

            string hash = md5.string_hashing(input);
            cout << "MD5 hash: ";

            SetConsoleTextAttribute(console_color, 10);
            cout << hash << endl;
            SetConsoleTextAttribute(console_color, 15);
            cout << "------------------------------------" << endl;

            if (err.getYesNoInput("(1) Do you want to save the string and hash to a file? (Y/N): ")) {
                bool appendData = false;
                string fullFilePath = handler.handleFileInput(appendData);
                if (!fullFilePath.empty()) {
                    try {
                        string data = "\nString: " + input + "\nMD5 hash: " + hash;
                        handler.saveDataToFile(fullFilePath, data, appendData);
                    }
                    catch (const exception& e) {
                        cout << "Error: " << e.what() << endl;
                    }
                }
            }
        }
        else if (choice == '1') {
            string fullFilePath = md5.getInput("Enter FULL file path\n* Example of file path C:\\Users\\sindr\\Desktop\\file.txt *\n* Limitation 10 MB *\n: ");
            if (!fullFilePath.empty()) {
                string hash = md5.hashFile(fullFilePath);
                if (hash.length() != 0) {
                    cout << "MD5 hash: ";
                    SetConsoleTextAttribute(console_color, 10);
                    cout << hash << endl;
                    SetConsoleTextAttribute(console_color, 15);
                    cout << "------------------------------------" << endl;

                    if (err.getYesNoInput("(1) Do you want to save the string and hash to a file? (Y/N): ")) {
                        bool appendData = false;
                        string fullFilePath_ = handler.handleFileInput(appendData);
                        if (!fullFilePath_.empty()) {
                            try {
                                string data = "\nFile: " + fullFilePath + "\nMD5 hash: " + hash;
                                handler.saveDataToFile(fullFilePath_, data, appendData);
                            }
                            catch (const exception& e) {
                                cout << "Error: " << e.what() << endl;
                            }
                        }
                    }
                }
            }
        }
        else {
            SetConsoleTextAttribute(console_color, 12);
            cout << "Invalid choice. Please enter '0' to hash a string, '1' to hash a file, or 'e' to exit." << endl;
            SetConsoleTextAttribute(console_color, 15);
        }
    }
}