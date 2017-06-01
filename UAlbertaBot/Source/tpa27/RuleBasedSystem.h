#ifndef RULEBASEDSYSTEM_H 
#define RULEBASEDSYSTEM_H

#include "RuleBase.h"
#include "KnowledgeBase.h"

class RuleBasedSystem
{
private:
	KnowledgeBase kb;
	RuleBase rb;

public:
	RuleBasedSystem(string file);
	void readFile(string file);
	void update();
};

#endif