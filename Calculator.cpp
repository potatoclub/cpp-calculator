#include "stdafx.h"
#include "Calculator.h"

#include "boost/algorithm/string.hpp"
#include <vector>
#include <functional>
#include <sstream>
#include <iomanip>
#include <iostream>




namespace Calculator {

	/*
		If two are similar (one embedded in the other), the longer one must come first
			- Ex: "asin" must come before "sin", and "tanh" must come before "tan"
	*/
	static std::vector <std::pair<std::string, std::function<double(double)>>> _Funcs = {
		std::make_pair("ln", [](double a) { return log(a); }),
		std::make_pair("log2", [](double a) { return log2(a); }),
		std::make_pair("log", [](double a) { return log10(a); }),
		std::make_pair("asin", [](double a) { return asin(a); }),
		std::make_pair("acos", [](double a) { return acos(a); }),
		std::make_pair("atan", [](double a) { return atan(a); }),
		std::make_pair("sin", [](double a) { return sin(a); }),
		std::make_pair("cos", [](double a) { return cos(a); }),
		std::make_pair("tan", [](double a) { return tan(a); }),
		std::make_pair("exp", [](double a) { return exp(a); }),
		std::make_pair("sqrt", [](double a) { return sqrt(a); }),
	};





	double Calculator::Calculate(std::string in) {
		try {
			
			// Check if just a number
			if (!in.empty() && in.find_first_not_of(".0123456789", size_t(in[0] == '-')) == std::string::npos)
				return std::stod(in);

			// Search for any functions
			size_t min = std::string::npos, fidx = 0;
			while (true) {
				min = std::string::npos;
				for (size_t i = 0; i < _Funcs.size(); i++) {
					auto j = in.find(_Funcs[i].first + '(');
					if (j != std::string::npos && j < min) {
						min = j;
						fidx = i;
					}
				}
				if (min != std::string::npos) {
					long tracker = 0;
					size_t i = min + _Funcs[fidx].first.length() + 1;
					for (; i < in.length(); i++) {
						if (in[i] == '(') tracker++; else if (in[i] == ')') tracker--;
						if (tracker < 0) break;
					}
					double simplified = Calculate(in.substr(min + _Funcs[fidx].first.length() + 1, i - (min + _Funcs[fidx].first.length() + 1)));
					std::stringstream ss;
					ss << std::setprecision(std::numeric_limits<double>::max_digits10) << _Funcs[fidx].second(simplified);
					in.replace(min, i - min + 1, ss.str());
					std::cout << in << "\n";
				}
				else break;
			}

			// Check again if just a number (checking twice helps efficiency)
			if (!in.empty() && in.find_first_not_of(".0123456789", size_t(in[0] == '-')) == std::string::npos)
				return std::stod(in);

		}
		catch (std::exception e) {};

		return 0.0f;
	}




}