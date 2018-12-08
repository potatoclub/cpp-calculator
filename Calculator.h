#pragma once
#include <string>
#include <vector>
#include <functional>
#include <optional>


class Calculator {
private:

	std::string _LastError;

	std::vector <std::pair<std::string, std::function<double(double)>>> _Funcs = {					// name, function
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

	std::vector <std::tuple<char, size_t, std::function<double(double, double)>>> _Operators = {	// char, precendence, function
		std::make_tuple('+', 0, [](double a, double b) { return a + b; }),
	};
	/// precendence is inverse; lower values are calculated first

public:

	std::string LastError();

	void AddFunction(std::string Name, std::function<double(double)> Func);

	std::optional<double> Calculate(std::string in);

};