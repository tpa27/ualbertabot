#ifndef TERM_H
#define TERM_H

#include <string>
#include <Vector>
#include "Parameter.h"

using namespace std;

class Term
{
private:
	string functor;
	vector<Parameter> parameters;
	bool negative;
public:
	Term(string f, bool negation=false);
	Term(string f, Parameter p, bool negation=false);
	Term(string f, Parameter p1, Parameter p2, bool negation=false);
	Term(string f, vector<Parameter> p, bool negation=false);

	string getFunctor();
	vector<Parameter> getParameters();
	bool isNegation();

	friend ostream& operator<<(ostream& os, const Term& t) {
		if (t.negative)
			os << "~";
		os << t.functor;
		if (!t.parameters.empty()) {
			os << "(";
			for (unsigned int i = 0; i < t.parameters.size()-1; i++) {
				Parameter s = t.parameters[i];
				os << s << ",";
			}
			Parameter s = t.parameters[t.parameters.size() - 1];
			os << s << ")";
		}
		return os;
	}
};

#endif 
