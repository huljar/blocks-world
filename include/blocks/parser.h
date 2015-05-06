#ifndef PARSER_H
#define PARSER_H

#include <blocks/predicate.h>
#include <blocks/cnf.h>
#include <string>
#include <vector>
#include <set>

namespace blocks {
	class Parser;
}

class blocks::Parser {

public:
	typedef std::vector<std::vector<std::string>> world_state;
	typedef std::vector<std::vector<std::string>>::iterator world_state_iterator;
	typedef std::vector<std::vector<std::string>>::const_iterator const_world_state_iterator;

	typedef std::vector<std::string> pile;
	typedef std::vector<std::string>::iterator pile_iterator;
	typedef std::vector<std::string>::const_iterator const_pile_iterator;

	typedef std::set<std::string> block_set;
	typedef std::set<std::string>::iterator block_set_iterator;
	typedef std::set<std::string>::const_iterator const_block_set_iterator;

	static world_state parseState(const std::string& state);
	static block_set extractBlocks(const world_state& world);

	static std::vector<Predicate> extractMoves(const std::string& result, const CNF& cnf, bool sort); 

};

#endif
