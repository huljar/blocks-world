#include <blocks/clause.h>

using namespace blocks;

Clause::Clause() {

}

Clause& Clause::operator<<(const Literal& literal) {
	literals.push_back(literal);
	return *this;
}
