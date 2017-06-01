#ifndef RULEBASE_H 
#define RULEBASE_H

#include "Rule.h"

class RuleBase 
{
private:
	vector<Rule> rules;

public:
	void addRule(Rule r);
	vector<Rule> getRules();
	void print();
	static RuleBase parseRuleFile(string file);
};

#endif 