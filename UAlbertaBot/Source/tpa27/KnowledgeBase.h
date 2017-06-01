#ifndef KNOWLEDGEBASE_h
#define KNOWLEDGEBASE_H

#include <vector>
#include "Term.h"

class KnowledgeBase 
{
private:
	vector<Term> facts;

public:
	void addTerm(Term t);
	vector<Term> getTerms();
	void clear();
	void print();
};

#endif