#include <blocks/cnf.h>

using namespace blocks;

CNF::CNF() :
	mapUpdated(false)
{

}

CNF& CNF::operator<<(const Clause& clause) {
	clauses.push_back(clause);
	mapUpdated = false;
	return *this;
}

const std::vector<Clause>& CNF::getClauses() const {
	return clauses;
}

void CNF::createMapping() {
	mapping.clear();
	unsigned int current = 1;

	for(std::vector<Clause>::const_iterator it = clauses.cbegin(); it != clauses.cend(); ++it) {
		for(std::vector<Literal>::const_iterator jt = it->getLiterals().cbegin(); jt != it->getLiterals().cend(); ++jt) {
			auto result = mapping.insert(std::make_pair(jt->getPredicate(), current));
			if(result.second) ++current;
		}
	}

	mapUpdated = true;
}

void CNF::writeDimacsCnf(FILE* out) {
	if(!mapUpdated) createMapping();

	fprintf(out, "p cnf %u %u\n", countVariables(), countClauses());
	for(std::vector<Clause>::const_iterator it = clauses.cbegin(); it != clauses.cend(); ++it) {
		for(std::vector<Literal>::const_iterator jt = it->getLiterals().cbegin(); jt != it->getLiterals().cend(); ++jt) {
			if(!jt->getValue()) fprintf(out, "-");
			fprintf(out, "%u ", mapping[jt->getPredicate()]);
		}
		fprintf(out, "0\n");
	}
}

void CNF::invertMapping() {
	for(std::unordered_map<Predicate, unsigned int, PredicateHasher>::const_iterator it = mapping.cbegin(); it != mapping.cend(); ++it) {
		inverted.insert(std::make_pair(it->second, &it->first));
	}
}

Predicate CNF::findPredicate(unsigned int number) const {
	return *inverted.at(number);
}

unsigned int CNF::countClauses() const {
	return clauses.size();
}

unsigned int CNF::countVariables() {
	if(!mapUpdated) createMapping();
	return mapping.size();
}
