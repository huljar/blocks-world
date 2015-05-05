#ifndef LITERAL_H
#define LITERAL_H

#include <blocks/predicate.h>

namespace blocks {
	class Literal;
}

class blocks::Literal {

public:
	Literal(bool value, const Predicate& predicate);
	bool getValue() const;
	const Predicate& getPredicate() const;

private:
	bool value;
	Predicate predicate;

};

#endif
