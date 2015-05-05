#include <blocks/cnf.h>

using namespace blocks;

CNF::CNF() {

}

CNF& CNF::operator<<(const Clause& clause) {
	clauses.push_back(clause);
	return *this;
}
