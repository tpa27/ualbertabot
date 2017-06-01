#include "Parameter.h"

Parameter::Parameter(int i, bool v) {
	param = to_string(i);
	var = var;
}

Parameter::Parameter(string s, bool v) {
	param = s;
	var = v;
}

bool Parameter::isVar() {
	return var;
}

string Parameter::getParam() {
	return param;
}
