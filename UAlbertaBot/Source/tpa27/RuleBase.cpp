#include <iostream>
#include <fstream>
#include "RuleBase.h"

void RuleBase::addRule(Rule r) {
	rules.push_back(r);
}

vector<Rule> RuleBase::getRules() {
	return rules;
}

void RuleBase::print() {
	cout << "RuleBase: " << endl;
	for (auto &r : rules) {
		cout << r << endl;
	}
}

string trim(const string& str)
{
	size_t first = str.find_first_not_of(' ');
	if (string::npos == first)
	{
		return str;
	}
	size_t last = str.find_last_not_of(' ');
	return str.substr(first, (last - first + 1));
}

Term parseSpecialTerm(string term, bool isNegation) {
	vector<Parameter> params;
	string delimiter;
	if (term.find(">") != string::npos) 
		delimiter = ">";
	else if (term.find("<") != string::npos) 
		delimiter = "<";
	int loc = term.find(delimiter);
	string p1 = trim(term.substr(0, loc));
	string p2 = trim(term.substr(loc + delimiter.length(), term.length()));

	params.push_back(Parameter(p1, true));
	params.push_back(Parameter(p2, true));
		
	return Term(delimiter, params, isNegation);
}

Term parseTerm(string term) {
	string delimiter = ",";
	string param_start = "(";
	string param_end = ")";

	vector<Parameter> params;
	int args_start = term.find(param_start);
	string name = term.substr(0, args_start);
	bool isNegation = false;
	if (name[0] == '~') {
		isNegation = true;
		name = name.substr(1, name.length());
	}

	if (args_start == string::npos)  //no args
		return parseSpecialTerm(term, isNegation);

	int args_legnth = term.find(param_end) - args_start - delimiter.length();
	if (args_legnth == 0) {
		return Term(name, params, isNegation);
	}
	string args = term.substr(args_start + delimiter.length(), args_legnth); 
	string token;
	bool isVar;
	size_t last = 0; 
	size_t next = 0; 
	while ((next = args.find(delimiter, last)) != string::npos) {
		token = trim(args.substr(last, next - last));
		isVar = true;
		if (token.find("\"") == 0) {
			token = token.substr(1, token.length() - 2);
			isVar = false;
		}

		Parameter p(token, isVar);
		params.push_back(p);
		last = next + 1; 
	}
	token = trim(args.substr(last));
	isVar = true;
	if (token.find('"') == 0) {
		token = token.substr(1, token.length() - 2);
		isVar = false;
	}
	Parameter p(token, isVar);
	params.push_back(p);
	return Term(name, params, isNegation);
}

vector<Term> parseEffect(string effect) {
	vector<Term> e;
	Term t = parseTerm(effect);
	e.push_back(t);
	return e;
}

vector<Term> parsePattern(string effect) {
	vector<Term> p;
	string delimiter = ",";
	string param_starter = "(";
	string param_ender = ")";
	string token;
	size_t last = 0; 
	size_t next = 0; 
	while ((next = effect.find(delimiter, last)) != string::npos) { 
		token += trim(effect.substr(last, next - last));
		if (token.find(param_starter) != string::npos && 
			token.find(param_ender) == string::npos) {
			token += delimiter;
			last = next + delimiter.length();
			continue;
		}
		Term t = parseTerm(token);
		p.push_back(t);
		last = next + delimiter.length(); 
		token = "";
	}
	token += trim(effect.substr(last));
	Term t = parseTerm(token);
	p.push_back(t);
	return p;
}

Rule parseLine(string line) {
	string delimiter = ":-";
	string ending = ".";
	int pattern_start = line.find(delimiter);
	int pattern_length = line.find(ending) - pattern_start - delimiter.length();
	string effect = line.substr(0, pattern_start); 
	string pattern = line.substr(pattern_start + delimiter.length(), pattern_length); 

	EffectType t;
	if (pattern.find("do") == 0)
		t = action;
	else
		t = newterm;
	vector<Term> veffect = parseEffect(effect);
	vector<Term> vpattern = parsePattern(pattern);
	return Rule (vpattern, veffect, t);
}

RuleBase RuleBase::parseRuleFile(string file) {
	RuleBase rules;
	ifstream infile(file);

	string line;
	while (getline(infile, line))
	{
		if (line.empty())
			continue;
		if (line[0] == '#')
			continue;
		Rule r = parseLine(line);
		rules.addRule(r);
	}

	return rules;
}
