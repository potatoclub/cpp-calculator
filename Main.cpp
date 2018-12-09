// Calculator.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Calculator.h"
#include <iostream>
#include <iomanip>


int main()
{

	std::string s;

	Calculator Calc;
	std::cout << std::setprecision(15);

	while (true) {
		std::getline(std::cin, s);
		if (s == "end") break;

		Calc.AddFunction("myfunc", [](double a) { return 2.0 * a; });

		auto n = Calc.Calculate(s);
		if (n)
			std::cout << "Final Result: " << n.value() << "\n";
		else std::cout << "Error: " << Calc.LastError() << "\n";
	}
    return 0;
}

