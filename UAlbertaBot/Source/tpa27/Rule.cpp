#include <iostream>
#include "Rule.h"

Rule::Rule(vector<Term> p, vector<Term> e, EffectType et) {
	pattern = p;
	effect = e;
	effectType = et;
}

vector<Term> Rule::getPattern() { 
	return pattern;
}

vector<Term> Rule::getEffect() { 
	return effect;
}

EffectType Rule::getEffectType() { 
	return effectType;
}
