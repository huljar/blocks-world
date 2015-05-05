#include <blocks/parser.h>
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
