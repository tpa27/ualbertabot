#ifndef RULE_H
#define RULE_H

#include "Term.h"
#include "EffectType.h"
#include <vector>

class Rule 
{
private:
	std::vector<Term> pattern;
	std::vector<Term> effect;
	EffectType effectType;

public:
	Rule(vector<Term> p, vector<Term> e, EffectType et);
	std::vector<Term> getPattern();
	std::vector<Term> getEffect();
	EffectType getEffectType();

	friend ostream& operator<<(ostream& os, const Rule& t) {
		for (unsigned int i = 0; i < t.effect.size() - 1; i++) {
			os << t.effect[i] << ",";
		}
		os << t.effect[t.effect.size() - 1];
		os << " :- ";
		for (unsigned int i = 0; i < t.pattern.size() - 1; i++) {
			os << t.pattern[i] << ",";
		}
		os << t.pattern[t.pattern.size() - 1];
		return os;
	}
};

#endif 