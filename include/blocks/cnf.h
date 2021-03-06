#ifndef CNF_H
#define CNF_H

#include <blocks/clause.h>
#include <blocks/predicate.h>
#include <blocks/predicatehasher.h>
#include <vector>
#include <unordered_map>
#include <ostream>

namespace blocks {
	class CNF;
}

class blocks::CNF {

public:
	CNF();
	CNF& operator<<(const Clause& clause);
	const std::vector<Clause>& getClauses() const;

	void createMapping();
	void writeDimacsCnf(FILE* out);

	void invertMapping();
	Predicate findPredicate(unsigned int number) const;

	unsigned int countClauses() const;
	unsigned int countVariables();

private:
	std::vector<Clause> clauses;
	std::unordered_map<Predicate, unsigned int, PredicateHasher> mapping;
	std::unordered_map<unsigned int, const Predicate*> inverted;
	bool mapUpdated;

};

#endif
