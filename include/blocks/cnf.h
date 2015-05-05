#ifndef CNF_H
#define CNF_H

#include <blocks/clause.h>
#include <vector>

namespace blocks {
	class CNF;
}

class blocks::CNF {

public:
	CNF();
	CNF& operator<<(const Clause& clause);

private:
	std::vector<Clause> clauses;

};

#endif
