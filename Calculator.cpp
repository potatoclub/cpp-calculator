#include "stdafx.h"
#include "Calculator.h"

#include "boost/range/algorithm/count.hpp"
#include <sstream>
#include <iomanip>
#include <iostream>



std::string Calculator::LastError() {
	return _LastError;
}

void Calculator::AddFunction(std::string Name, std::function<double(double)> Func) {
	_Funcs.emplace_back(Name, Func);
}





std::optional<double> Calculator::Calculate(std::string in) {
	try {

		_LastError = "";

		// Check if just a number
		if (!in.empty() && boost::count(in, '.') < 2 && in.find_first_not_of(".0123456789", size_t(in[0] == '-')) == std::string::npos)
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
				auto simplified = Calculate(in.substr(min + _Funcs[fidx].first.length() + 1, i - (min + _Funcs[fidx].first.length() + 1)));
				if (!simplified) return simplified;
				std::stringstream ss;
				ss << std::setprecision(std::numeric_limits<double>::max_digits10) << _Funcs[fidx].second(simplified.value());
				in.replace(min, i - min + 1, ss.str());
			}
			else break;
		}

		// Check again if just a number (checking twice helps efficiency)
		if (!in.empty() && boost::count(in, '.') < 2 && in.find_first_not_of(".0123456789", size_t(in[0] == '-')) == std::string::npos)
			return std::stod(in);


		// Scan for operators now
		/// Reuse min
		min = std::numeric_limits<size_t>::max();
		size_t minprec = std::numeric_limits<size_t>::max();
		size_t oidx = 0;
		while (true) {
			min = std::numeric_limits<size_t>::max();
			minprec = std::numeric_limits<size_t>::max();
			for (size_t i = 0; i < _Operators.size(); i++) {
				auto j = in.find_first_of(std::get<0>(_Operators[i]));
				if (j != std::string::npos && j < min && std::get<1>(_Operators[i]) < minprec) {
					min = j;
					minprec = std::get<1>(_Operators[i]);
					oidx = i;
				}
			}
			if (min != std::numeric_limits<size_t>::max()) {
				double left;
				double right;

				auto simplified = Calculate(in.substr(min + _Funcs[fidx].first.length() + 1, i - (min + _Funcs[fidx].first.length() + 1)));
				if (!simplified) return simplified;

				std::stringstream ss;
				ss << std::setprecision(std::numeric_limits<double>::max_digits10) << _Funcs[fidx].second(simplified.value());
				in.replace(min, i - min + 1, ss.str());
			}
			else break;
		}

		// Last check
		if (!in.empty() && boost::count(in, '.') < 2 && in.find_first_not_of(".0123456789", size_t(in[0] == '-')) == std::string::npos)
			return std::stod(in);

	}
	catch (std::exception e) {	// this type of exception is thrown by std::stod, if conversion is unsuccessful
		_LastError = "Failed to parse numerical string";
		return;
	}
	catch (std::string) {
		_LastError = "uhhh something";
	}

	_LastError = "meeeepsicle";
	return std::optional<double>();
}