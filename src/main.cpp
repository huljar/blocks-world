#include <blocks/parser.h>
#include <blocks/cnf.h>
#include <blocks/clause.h>
#include <blocks/literal.h>
#include <blocks/predicate.h>
#include <iostream>
#include <string>
#include <vector>
#include <set>

using namespace blocks;

const std::string tableStr = "table";

int main(int argc, char** argv) {
	// Check command line arguments
	if(argc != 4) {
		std::cerr << "Usage: " << argv[0] << " initial_state goal_state time_steps" << std::endl;
		exit(1);
	}

	// Parse and store initial and goal states and time steps
	Parser::world_state initial = Parser::parseState(argv[1]);
	Parser::world_state goal = Parser::parseState(argv[2]);
	int time = atoi(argv[3]);

	// Extract existing blocks
	Parser::block_set existingBlocks = Parser::extractBlocks(initial);
	existingBlocks.insert(tableStr); //important to add the table!

	// Create CNF formula
	CNF cnf;

	// Add domain constraints
	// The table is always clear
	for(int i = 0; i < time; ++i) {
		Literal clear(true, Predicate(Predicate::CLEAR, tableStr, "", "", i));
		Clause clear_clause;
		clear_clause << clear;
		cnf << clear_clause;
	}

	// A block except the table cannot be clear and support a block at the same time
	for(Parser::block_set_iterator it = existingBlocks.begin(); it != existingBlocks.end(); ++it) {
		for(Parser::block_set_iterator jt = existingBlocks.begin(); jt != existingBlocks.end(); ++jt) {
			Literal equal(true, Predicate(Predicate::EQUAL, *jt, tableStr, "", 0));
			for(int i = 0; i < time; ++i) {
				Literal clear(false, Predicate(Predicate::CLEAR, *jt, "", "", i));
				Literal on(false, Predicate(Predicate::ON, *it, *jt, "", i));
				Clause clear_support;
				clear_support << equal << clear << on;
				cnf << clear_support;
			}
		}
	}

	// A block cannot be on itself
	for(Parser::block_set_iterator it = existingBlocks.begin(); it != existingBlocks.end(); ++it) {
		for(int i = 0; i < time; ++i) {
			Literal on(false, Predicate(Predicate::ON, *it, *it, "", i));
			Clause itself;
			itself << on;
			cnf << itself;
		}
	}

	// The table cannot be on another block
	for(Parser::block_set_iterator it = existingBlocks.begin(); it != existingBlocks.end(); ++it) {
		for(int i = 0; i < time; ++i) {
			Literal on(false, Predicate(Predicate::ON, tableStr, *it, "", i));
			Clause table;
			table << on;
			cnf << table;
		}
	}

	// A block can only be on one block
	for(Parser::block_set_iterator it = existingBlocks.begin(); it != existingBlocks.end(); ++it) {
		for(Parser::block_set_iterator jt = existingBlocks.begin(); jt != existingBlocks.end(); ++jt) {
			for(Parser::block_set_iterator kt = existingBlocks.begin(); kt != existingBlocks.end(); ++kt) {
				Literal equal(true, Predicate(Predicate::EQUAL, *jt, *kt, "", 0));
				for(int i = 0; i < time; ++i) {
					Literal on1(false, Predicate(Predicate::ON, *it, *jt, "", i));
					Literal on2(false, Predicate(Predicate::ON, *it, *kt, "", i));
					Clause one;
					one << on1 << on2 << equal;
					cnf << one;
				}
			}
		}
	}

	// A block except the table can support only one block
	for(Parser::block_set_iterator it = existingBlocks.begin(); it != existingBlocks.end(); ++it) {
		for(Parser::block_set_iterator jt = existingBlocks.begin(); jt != existingBlocks.end(); ++jt) {
			Literal equal2(true, Predicate(Predicate::EQUAL, *it, *jt, "", 0));
			for(Parser::block_set_iterator kt = existingBlocks.begin(); kt != existingBlocks.end(); ++kt) {
				Literal equal1(true, Predicate(Predicate::EQUAL, *kt, tableStr, "", 0));
				for(int i = 0; i < time; ++i) {
					Literal on1(false, Predicate(Predicate::ON, *it, *kt, "", i));
					Literal on2(false, Predicate(Predicate::ON, *jt, *kt, "", i));
					Clause support;
					support << equal1 << on1 << on2 << equal2;
					cnf << support;
				}
			}
		}
	}

	// Add initial state
	for(Parser::world_state_iterator it = initial.begin(); it != initial.end(); ++it) {
		Literal onTable(true, Predicate(Predicate::ON, *it->begin(), tableStr, "", 0));
		Clause onTableClause;
		onTableClause << onTable;
		cnf << onTableClause;
		for(Parser::pile_iterator jt = it->begin() + 1; jt != it->end(); ++jt) {
			Literal
		}
	}

}
