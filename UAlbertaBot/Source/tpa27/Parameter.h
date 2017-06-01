#pragma once
#include <string>
#include <ostream>

using namespace std;

class Parameter
{
private:
	string param;
	bool var;

public:
	Parameter(int i, bool v);
	Parameter(string s, bool v);
	bool isVar();
	string getParam();

	friend ostream& operator<<(ostream& os, const Parameter& t) {
		os << t.param;
		return os;
	}

	bool Parameter::operator==(const Parameter &other) const {
		return (param == other.param);
	}

	bool Parameter::operator!=(const Parameter &other) const {
		return !(*this == other);
	}
};
