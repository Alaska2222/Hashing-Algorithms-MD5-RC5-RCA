#include "ErrorHandler.h"
#include<iostream>
#include<regex>
#include<Windows.h>

bool ErrorHandler::getYesNoInput(const std::string& message) {
    HANDLE console_color;
    console_color = GetStdHandle(STD_OUTPUT_HANDLE);
    std::string input;
    while (true) {
        std::cout << message;
        std::cin >> input;
        if (input.length() == 1) {
            char choice = input[0];
            if (choice == 'Y' || choice == 'y') {
                return true;
            }
            else if (choice == 'N' || choice == 'n') {
                return false;
            }
        }
        SetConsoleTextAttribute(console_color, 12);
        std::cout << "* Invalid input. Please enter 'Y' or 'N' *" << std::endl;
        SetConsoleTextAttribute(console_color, 15);
    }
}

bool ErrorHandler::handlePseudoRandomInput(std::string n_str) {
    HANDLE console_color;
    console_color = GetStdHandle(STD_OUTPUT_HANDLE);
    if (!isInteger(n_str))
    {
        SetConsoleTextAttribute(console_color, 12);
        std::cout << "Number should be an integer, not a string.\n";
        SetConsoleTextAttribute(console_color, 15);
        return false;
    }
    int n = stoi(n_str);

    if (n < 0) {
        SetConsoleTextAttribute(console_color, 12);
        std::cout << "Number of randoms (n) cannot be negative.\n";
        SetConsoleTextAttribute(console_color, 15);
        return false;
    }
    if (n > 1000) {
        SetConsoleTextAttribute(console_color, 12);
        std::cout << "Number of randoms (n) cannot exceed 1000.\n";
        SetConsoleTextAttribute(console_color, 15);
        return false;
    }
    return true;
}

bool ErrorHandler::isInteger(const std::string& str) {
    static const std::regex intPattern("^[-+]?[0-9]+$");
    return std::regex_match(str, intPattern);
}
