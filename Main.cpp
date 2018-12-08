// Calculator.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Calculator.h"
#include <iostream>


int main()
{

	std::string s;
	std::getline(std::cin, s);

	std::cout << Calculator::Calculate(s);

    return 0;
}

