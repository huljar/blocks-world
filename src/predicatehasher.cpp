#include <blocks/predicatehasher.h>
#include <functional>
#include <string>

using namespace blocks;

std::size_t PredicateHasher::operator()(const Predicate& k) const {
	return (((std::hash<int>()(k.getType())
			^ std::hash<std::string>()(k.getBlockX()) >> 1)
			^ (std::hash<std::string>()(k.getBlockY()) << 1) >> 1)
			^ (std::hash<std::string>()(k.getBlockZ()) >> 1) << 1)
			^ (std::hash<int>()(k.getTime()) << 1);
}
