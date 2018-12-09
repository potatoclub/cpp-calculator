#include "stdafx.h"
#include "Calculator.h"

#include "boost/range/algorithm/count.hpp"
#include <sstream>
#include <iomanip>
#include <iostream>
#include <cassert>



size_t Calculator::_ScopeAt(std::string s, size_t idx) {
	if (idx >= s.length()) return 0;
	long t = 0;
	for (size_t i = 0; i <= idx; i++)
		if (s[i] == '(') t++; else if (s[i] == ')') t--;
	return (s[idx] == ')') ? t + 1 : t;
}

std::string Calculator::LastError() {
	return _LastError;
}

void Calculator::AddFunction(std::string Name, std::function<double(double)> Func) {
	_Funcs.emplace_back(Name, Func);
}





std::optional<double> Calculator::Calculate(std::string in) {
	try {

		_LastError = "";
		_LastParse = in;

		std::cout << "Now Calculating: " << in << "\n";

		if (in == "") return std::optional<double>();
		if (in.front() == '(' && in.back() == ')') {
			bool flag = false;
			for (long i = 1, t = 0; i < in.size() - 2; i++) {
				if (in[i] == '(') t++; else if (in[i] == ')') t--;
				if (t < 0) { flag = true; break; }
			}
			if (!flag) {
				in = in.substr(1, in.size() - 2);
				std::cout << "Trimmed to: " << in << "\n";
			}
		}
		if (in == "") return std::optional<double>();

		// Check if just a number
		if (!in.empty() && boost::count(in, '.') < 2 && in.find_first_not_of(".0123456789", size_t(in[0] == '-')) == std::string::npos) {
			std::cout << "Found to be: " << std::stod(in) << "\n";
			return std::stod(in);
		}

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
				ss << std::setprecision(std::numeric_limits<double>::max_digits10) << std::fixed << _Funcs[fidx].second(simplified.value());
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
				if (j != std::string::npos && j < min && _ScopeAt(in, j) == 0 && std::get<1>(_Operators[i]) <= minprec) {
					min = j;
					minprec = std::get<1>(_Operators[i]);
					oidx = i;
				}
			}
			if (min != std::numeric_limits<size_t>::max()) {		// operator found
				long lc = min - 1, rc = min + 1;
				std::optional<double> left, right;
				if (in[lc] == ')') {
					for (long lt = 0; lc >= 0 && lt >= 0; lc--) {
						if (in[lc] == ')') lt++; else if (in[lc] == '(') lt--;
					}
					left = Calculate(in.substr(lc + 1, min - lc - 1));
				}
				else {
					auto k = in.substr(0, min - 1).find_last_not_of(".0123456789");
					left = Calculate(in.substr(k == std::string::npos ? 0 : k + 1, k == std::string::npos ? min : min - k - 1));
					lc = k == std::string::npos ? 0 : k + 1;
				}
				if (in[rc] == '(') {
					for (long rt = 0; rc < (long)in.size() && rt >= 0; rc++) {
						if (in[rc] == '(') rt++; else if (in[rc] == ')') rt--;
					}
					right = Calculate(in.substr(min + 1, rc - min - 1));
				}
				else {
					auto k = in.substr(min + 1).find_first_not_of(".0123456789", size_t(in[min + 1] == '-'));
					right = Calculate(in.substr(min + 1, k == std::string::npos ? in.size() - min - 1 : k));
					rc = k == std::string::npos ? in.size() - 1 : min + k;
				}

				if (!left || !right) throw "well flip";
				
				std::cout << in << "\n";
				char c = in[min];
				std::string s;
				s += c;
				std::cout << "Operands for \'" << s << "\' operation: ";
					std::cout << left.value() << " " << right.value() << "\n";

				std::stringstream ss;
				ss << std::setprecision(std::numeric_limits<double>::max_digits10) << std::fixed << 
					std::get<2>(_Operators[oidx])(left.value(), right.value());
				in.replace(std::max(lc, 0L), std::min(rc, long(in.size() - 1)) - std::max(lc, 0L) + 1, ss.str());
				std::cout << "Found to be: " << std::get<2>(_Operators[oidx])(left.value(), right.value()) << 
					"\nNew string: " << in << "\n";
			}
			else break;
		}

		// Last check
		if (!in.empty() && boost::count(in, '.') < 2 && in.find_first_not_of(".0123456789", size_t(in[0] == '-')) == std::string::npos)
			return std::stod(in);

	}
	catch (std::exception e) {	// this type of exception is thrown by std::stod, if conversion is unsuccessful
		_LastError = "Failed to parse numerical string \"" + _LastParse + "\"";
		return std::optional<double>();
	}
	catch (std::string msg) {
		_LastError = msg + " (while parsing \"" + _LastParse + "\"";
	}

	_LastError = "Unexpected character in \"" + _LastParse + "\"";
	return std::optional<double>();
}