#include "RuleBasedSystem.h"
#include "Common.h"
#include <iostream>

//doTrainWorker(Z) :- workerNeeded(), resourcesAvailable(X), resourcesNeededFor("Worker", Y), X>Y, ownBase(Z), idle(Z).
/*
void perception(KnowledgeBase& kb) {
	int base = 01;
	int worker1 = 11;
	int worker2 = 12;
	int worker3 = 13;
	int resource = 21;
	kb.addTerm(Term("own", Parameter(base, false)));
	kb.addTerm(Term("type", Parameter(base, false), Parameter("Base", false)));
	kb.addTerm(Term("type", Parameter(resource, false), Parameter("Resource", false)));
	kb.addTerm(Term("own", Parameter(worker1, false)));
	kb.addTerm(Term("own", Parameter(worker2, false)));
	kb.addTerm(Term("own", Parameter(worker3, false)));
	kb.addTerm(Term("type", Parameter(worker1, false), Parameter("Worker", false)));
	kb.addTerm(Term("type", Parameter(worker2, false), Parameter("Worker", false)));
	kb.addTerm(Term("type", Parameter(worker3, false), Parameter("Worker", false)));
	kb.addTerm(Term("idle", Parameter(base, false)));
	kb.addTerm(Term("idle", Parameter(worker1, false)));
	kb.addTerm(Term("resourcesAvailable", Parameter(100, false)));
	kb.addTerm(Term("resourcesNeededFor", Parameter("Worker", false), Parameter(80, false)));
}
*/

void perception(KnowledgeBase& kb) {
	// Iterate through all the units that we own
	for (auto &u : BWAPI::Broodwar->self()->getUnits())
	{
		// Ignore the unit if it no longer exists
		// Make sure to include this block when handling any Unit pointer!
		if (!u->exists())
			continue;

		// Ignore the unit if it has one of the following status ailments
		if (u->isLockedDown() || u->isMaelstrommed() || u->isStasised())
			continue;

		// Ignore the unit if it is in one of the following states
		if (u->isLoaded() || !u->isPowered() || u->isStuck())
			continue;

		// Ignore the unit if it is incomplete or busy constructing
		if (!u->isCompleted() || u->isConstructing())
			continue;

		Parameter id(u->getID(), false);
		kb.addTerm(Term("own", id, false));
		if (u->getType().isWorker()) {
			Parameter type("Worker", false);
			kb.addTerm(Term("type", id, type));
		}
		
		if (u->getType().isResourceDepot()) { // A resource depot is a Command Center, Nexus, or Hatchery
			Parameter type("Base", false);
			kb.addTerm(Term("type", id, type));
		}
		
		if (u->isIdle()) {
			kb.addTerm(Term("idle", id, false));
		}
	}
}
vector <Rule> arbitrate(vector<Rule> rules) {
	vector<Rule> newRules;
	if (!rules.empty())
		newRules.push_back(rules[0]);
	//return newRules;
	return rules;
}

void execute(Rule r) {
	BWAPI::Broodwar << r << endl;
}

void addRule(Rule r, KnowledgeBase &kb) {
	if (r.getEffectType() == newterm) {
		vector<Term> terms = r.getEffect();
		for (Term &t : terms) {
			kb.addTerm(t);
		}
	}
	//cout << r << endl;
	BWAPI::Broodwar << r << endl;
}

bool isBound(vector<pair<Parameter, Parameter>> *bindings, Parameter p) {
	for (auto & binding : *bindings) {
		if (binding.first == p)
			return true;
	}
	return false;
}

bool isSpecialTerm(Term t) {
	return t.getFunctor() == "<" ||
		   t.getFunctor() == ">";
}

Parameter applyBindings(Parameter t, vector<pair<Parameter, Parameter>>* bindings) {
	for (unsigned int i = 0; i < bindings->size(); i++)
	{
		pair<Parameter, Parameter> b = bindings->at(i);
		if (t == b.first)
			return Parameter(b.second.getParam(), false);
	}
	return t;
}

Term applyBindings(Term t, vector<pair<Parameter, Parameter>>* bindings) {
	vector<Parameter> bound_params;
	for (auto & p : t.getParameters()) {
		Parameter bound_p = applyBindings(p, bindings);
		bound_params.push_back(bound_p);
	}
	return Term(t.getFunctor(), bound_params, t.isNegation());
}

vector<Term> applyBindings(vector<Term> pattern, vector<pair<Parameter, Parameter>>* bindings) {
	vector<Term> bound_pattern;
	for (auto & t : pattern) {
		Term bound_term = applyBindings(t, bindings);
		bound_pattern.push_back(bound_term);
	}
	return bound_pattern;
}

//verify this special term is valid with current bindings
vector<pair<Parameter, Parameter>>* unification(Term t) {
	bool passing = false;
	int first = atoi(t.getParameters()[0].getParam().c_str());
	int second = atoi(t.getParameters()[1].getParam().c_str());
	if (t.getFunctor() == ">") {
		passing = first > second;
	}
	else if (t.getFunctor() == "<") {
		passing = first < second;
	}
	if (passing)
		return new vector<pair<Parameter, Parameter>>();
	return NULL;
}

//Single term unification
vector<pair<Parameter, Parameter>>* unification(Term t1, Term t2) {
	if (t1.getFunctor() != t2.getFunctor()) 
		return NULL;
	if (t1.getParameters().size() != t2.getParameters().size()) 
		return NULL;

	vector<pair<Parameter, Parameter>>* bindings = new vector<pair<Parameter, Parameter>>();
	for (unsigned int i = 0; i < t1.getParameters().size(); i++) {
		Parameter p1 = t1.getParameters()[i];
		Parameter p2 = t2.getParameters()[i];
		if (p1.isVar() && !isBound(bindings, p1))
			bindings->push_back(pair<Parameter, Parameter>(p1, p2));
		else if (p1 != p2)
			return NULL;
	}
	return bindings;
}

//Unifying a term T1 against the whole knowledge base
vector<pair<Parameter, Parameter>>* unification(Term t1, KnowledgeBase kb) {
	if (t1.isNegation()) {
		for (Term s : kb.getTerms()) {
			vector<pair<Parameter, Parameter>>* bindings = unification(t1, s);
			if (bindings != NULL)
				return NULL;
		}
		return new vector<pair<Parameter, Parameter>>();
	}
	else {
		for (Term s : kb.getTerms()) {
			vector<pair<Parameter, Parameter>>* bindings = unification(t1, s);
			if (bindings != NULL)
				return bindings;
		}
		return NULL;
	}
}

//Unifying a pattern against the whole knowledge base (binary "AND" every term of the pattern)
vector<pair<Parameter, Parameter>>* unification(vector<Term> pattern, KnowledgeBase kb) {
	vector<pair<Parameter, Parameter>> *bindings = new vector<pair<Parameter, Parameter>>();

	//if pattern is a single term, unify it with the kb
	if (pattern.size() == 1)
		return unification(pattern[0], kb);
	
	//if pattern is many terms, do logical AND from t1 to the rest of the terms
	for (Term s1 : kb.getTerms()) {
		Term t1 = pattern[0];
		vector<Term> t2(pattern.begin() + 1, pattern.end());
		if (isSpecialTerm(t1))
			bindings = unification(t1);
		else
			bindings = unification(t1, s1);
		if (bindings != NULL) {
			vector<Term> t2prime = applyBindings(t2, bindings);
			vector<pair<Parameter, Parameter>>* bindings2 = unification(t2prime, kb);
			if (bindings2 != NULL) {
				bindings->insert(bindings->end(), bindings2->begin(), bindings2->end());
				return bindings;
			}
		}
	}
	return NULL;
}

Rule instantiate(Rule varRule, vector<pair<Parameter, Parameter>>* bindings) {
	vector<Term> bound_effects = applyBindings(varRule.getEffect(), bindings);
	vector<Term> bound_pattern = applyBindings(varRule.getPattern(), bindings);
	return Rule(bound_pattern, bound_effects, varRule.getEffectType());
}

void ruleBasedSystemIteration(RuleBase rb, KnowledgeBase kb) {
	vector<Rule> firedRules;
	for (Rule &r : rb.getRules()) {
		vector<pair<Parameter, Parameter>> *bindings = unification(r.getPattern(), kb);
		if (bindings != NULL) {
			Rule instantiated_rule = instantiate(r, bindings);
			//if this is an newterm action, then do that immediately
			if (instantiated_rule.getEffectType() == newterm)
				addRule(instantiated_rule, kb);
			else
				firedRules.push_back(instantiated_rule);
		}
	}
	vector<Rule> rulesToExecute = arbitrate(firedRules);
	for (Rule &r : rulesToExecute) {
		execute(r);
	}
}

void RuleBasedSystem::readFile(string file) {
	rb = RuleBase::parseRuleFile(file);
}

RuleBasedSystem::RuleBasedSystem(string file) {
	rb = RuleBase::parseRuleFile(file);
}

void RuleBasedSystem::update() {
	perception(kb);
	ruleBasedSystemIteration(rb, kb);
	kb.clear();
}