#include <iostream>
#include "KnowledgeBase.h"

 void KnowledgeBase::addTerm(Term t) {
	facts.push_back(t);
}

vector<Term> KnowledgeBase::getTerms() {
	return facts;
 }
 
 void KnowledgeBase::clear() {
	facts.clear();
}
 
 void KnowledgeBase::print() {
	 for (auto &t: facts) {
		 std::cout << t << endl;
	 }
 }
