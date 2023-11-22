#include <iostream>
#include <vector>
#include<string>
#include<Windows.h>
#include"lab1.h"
#include"lab2.h"
#include"lab3.h"
#include"lab4.h"


using namespace std;

int main() {
    string choice;
    lab1 lb1;
    lab2 lb2;
    lab3 lb3;
    lab4 lb4;
    HANDLE console_color;
    console_color = GetStdHandle(STD_OUTPUT_HANDLE);

    while (true) {
        cout << "----------MAIN MENU-----------" << endl;
        cout << "(1) Run Pseudo Random Generator" << endl;
        cout << "(2) Run MD5 algorithm" << endl;
        cout << "(3) Run RC5 algorithm" << endl;
        cout << "(4) Run RSA algorithm" << endl;
        cout << "Enter 'exit' to close the program" << endl;
        cout << ": ";
        cin >> choice;

        if (choice == "1") {
            system("cls");
            lb1.runLab1();
        }
        else if (choice == "2") {
            system("cls");
            lb2.runLab2();

        }
        else if (choice == "3") {
            system("cls");
            lb3.runLab3();

        }
        else if (choice == "4") {
            system("cls");
            lb4.runLab4();

        }
        else if (choice == "exit" || choice == "Exit") {
            return 0;
        }
        else {
            SetConsoleTextAttribute(console_color, 12);
            cout << "Invalid choice: Enter only '1', '2', '3', '4' or 'exit' " << endl;
            SetConsoleTextAttribute(console_color, 15);
            continue;
        }


    }
    return 0;

}


