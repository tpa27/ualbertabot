#include <iostream>
#include "Term.h"

using namespace std;

Term::Term(string f, bool negation)
{
	functor = f;
	this->negative = negation;
}

Term::Term(string f, Parameter p, bool negation) : Term(f, negation)
{
	parameters.push_back(p);
}

Term::Term(string f, Parameter p1, Parameter p2, bool negation) : Term(f, p1, negation)
{
	parameters.push_back(p2);
}

Term::Term(string f, vector<Parameter> p, bool negation) : Term(f, negation)
{
	parameters = p;
}

string Term::getFunctor() {
	return functor;
}

vector<Parameter> Term::getParameters() {
	return parameters;
}

bool Term::isNegation() {
	return negative;
}
