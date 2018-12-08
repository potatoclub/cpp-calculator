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

	Calculator Calc;


	while (true) {
		std::getline(std::cin, s);
		if (s == "end") break;

		auto n = Calc.Calculate(s);
		if (n)
			std::cout << n.value() << "\n";
		else std::cout << "Error: " << Calc.LastError() << "\n";
	}
    return 0;
}

