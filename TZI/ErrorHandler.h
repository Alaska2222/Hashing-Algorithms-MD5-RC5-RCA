#pragma once
#include<string>

class ErrorHandler
{
public:
	bool getYesNoInput(const std::string& message);
	bool handlePseudoRandomInput(std::string n_str);

private:
	bool isInteger(const std::string& str);

};

