// Calculator.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Calculator.h"
#include <iostream>
#include <iomanip>


int main()
{

	std::string s;
	std::cout << std::setprecision(8);

	while (true) {
		std::getline(std::cin, s);
		if (s == "end") break;

		std::cout << Calculator::Calculate(s) << "\n";
	}
    return 0;
}

