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

void CNF::writeDimacsCnf(std::ostream& out) {
	if(!mapUpdated) createMapping();

	out << "p cnf " << countVariables() << countClauses() << std::endl;
	for(std::vector<Clause>::const_iterator it = clauses.cbegin(); it != clauses.cend(); ++it) {
		for(std::vector<Literal>::const_iterator jt = it->getLiterals().cbegin(); jt != it->getLiterals().cend(); ++jt) {
			out << (jt->getValue() ? "" : "-") << mapping[jt->getPredicate()] << " ";
		}
		out << "0" << std::endl;
	}
}

unsigned int CNF::countClauses() const {
	return clauses.size();
}

unsigned int CNF::countVariables() {
	if(!mapUpdated) createMapping();
	return mapping.size();
}
