#include "lab1.h"
#include"PseudoRandGenerator.h"
#include"Constants_l1.h"
#include"FileHandler.h"
#include"ErrorHandler.h"
#include<iostream>
#include <sstream>
#include<string>
#include <windows.h> 
using namespace std;

void lab1::runLab1() {
    
    PseudoRandGenerator generator(a, c, m, x0);
    FileHandler handler;
    ErrorHandler err;
    HANDLE console_color;
    console_color = GetStdHandle(STD_OUTPUT_HANDLE);
    cout << "-----------GENERATOR OF PSEUDO-RANDOM NUMBERS-------------" << endl << endl;

    while (true) {
        string n_str;
        
        cout << "Enter number of randoms to generate (N) " << endl;
        cout << "Limitations: \n\t* N should be a positive number\n\t* N should be less than 1000 " << endl;
        cout << "Enter 'e' to quit" << endl;
        cout << ": ";
        cin >> n_str;

        if (n_str == "e") {
            system("cls");
            break;
        }
        else if (!err.handlePseudoRandomInput(n_str)) {
            continue;
        }
        else {

            auto result = generator.GenerateRandSequence(n_str);
            cout << "\nPeriod: ";
            SetConsoleTextAttribute(console_color, 10);
            cout << result.second << endl;
            SetConsoleTextAttribute(console_color, 15);
            cout << "Sequence: ";
            SetConsoleTextAttribute(console_color, 10);
            for (int i = 0; i < result.first.size(); ++i) {
                cout << result.first[i];
                if ((i + 1) % 10 == 0) {
                    cout << endl;
                }
                else {
                    cout << " ";
                }
            }
            cout << endl;
            SetConsoleTextAttribute(console_color, 15);
            if (err.getYesNoInput("(1) Do you want to save data to a file? (Y/N): ")) {
                bool appendData = false;
                string fullFilePath = handler.handleFileInput(appendData);
                if (!fullFilePath.empty()) {
                    stringstream output;

                    output << "\nPeriod: " << result.second << endl;
                    output << "Sequence: ";

                    for (int i = 0; i < result.first.size(); ++i) {
                        output << result.first[i];
                        if ((i + 1) % 10 == 0) {
                            output << endl;
                        }
                        else {
                            output << " ";
                        }
                    }
                    string data = output.str();
                    handler.saveDataToFile(fullFilePath, data, appendData);
                }

            }

        }
        system("cls");
    }
}