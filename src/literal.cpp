#include <blocks/literal.h>

using namespace blocks;

Literal::Literal(bool value, const Predicate& predicate) :
	value(value),
	predicate(predicate)
{

}

bool Literal::getValue() const {
	return value;
}

const Predicate& Literal::getPredicate() const {
	return predicate;
}
