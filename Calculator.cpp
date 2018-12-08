#include "Calculator.h"

#include "C:/boost/algorithm/string.hpp"
#include <vector>
#include <functional>


static std::vector <std::pair<std::string, std::function<float(float)>>> _funcs = {
	std::make_pair("ln", [](float a) { return log(a); }),
};



static size_t ScopeEnd(std::string s, size_t start) {

}


double Calculate(std::string in) {

	for (auto &f : _funcs) {
		auto i = in.find(f.first + '(');
		if ()
	}

}