#ifndef CLAUSE_H
#define CLAUSE_H

#include <blocks/literal.h>
#include <vector>

namespace blocks {
	class Clause;
}

class blocks::Clause {

public:
	Clause();
	Clause& operator<<(const Literal& literal);
	const std::vector<Literal>& getLiterals() const;

private:
	std::vector<Literal> literals;

};

#endif
