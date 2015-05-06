#ifndef PREDICATE_HASHER_H
#define PREDICATE_HASHER_H

#include <blocks/predicate.h>

namespace blocks {
	struct PredicateHasher;
}

struct blocks::PredicateHasher {
	std::size_t operator()(const Predicate& k) const;
};

#endif
