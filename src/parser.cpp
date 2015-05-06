#include <blocks/parser.h>
#include <algorithm>
#include <cstdlib>
#include <sstream>
#include <string>
#include <vector>
#include <set>

using namespace blocks;

Parser::world_state Parser::parseState(const std::string& state) {
	world_state ret;

	// Split the input string into the different block piles (separated by spaces)
	std::stringstream input(state);
	std::vector<std::string> piles;
	std::string pile;
	while(std::getline(input, pile, ' ')) {
		piles.push_back(pile);
	}

	// Split the piles into the individual blocks(separated by commas)
	for(pile_iterator it = piles.begin(); it != piles.end(); ++it) {
		std::stringstream pinput(*it);
		std::vector<std::string> blocks;
		std::string block;
		while(std::getline(pinput, block, ',')) {
			blocks.push_back(block);
		}
		ret.push_back(blocks);
	}

	return ret;
}

Parser::block_set Parser::extractBlocks(const world_state& world) {
	block_set ret;

	for(const_world_state_iterator it = world.cbegin(); it != world.cend(); ++it) {
		for(const_pile_iterator jt = it->cbegin(); jt != it->cend(); ++jt) {
			ret.insert(*jt);
		}
	}

	return ret;
}

std::vector<Predicate> Parser::extractMoves(const std::string& result, const CNF& cnf, bool sort) {
	std::vector<Predicate> ret;

	// Split the result string at spaces
	std::stringstream resultstream(result);
	std::string assignment;

	// First read the "SAT" line and ignore it
	std::getline(resultstream, assignment);

	// Now iterate over the assignments
	while(std::getline(resultstream, assignment, ' ')) {
		int value = std::stoi(assignment);
		if(value < 0) continue;
		if(value == 0) break;

		Predicate pred = cnf.findPredicate((unsigned int)value);
		if(pred.getType() == Predicate::MOVE) {
			ret.push_back(pred);
		}
	}

	if(sort) {
		std::sort(ret.begin(), ret.end(), [](const Predicate& a, const Predicate& b) {
			return a.getTime() < b.getTime();
		});
	}

	return ret;
}
