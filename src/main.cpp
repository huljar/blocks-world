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
	for(int i = 1; i <= time; ++i) {
		Literal clear(true, Predicate(Predicate::CLEAR, tableStr, "", "", i));
		Clause clear_clause;
		clear_clause << clear;
		cnf << clear_clause;
	}

	// A block except the table cannot be clear and support a block at the same time
	for(Parser::block_set_iterator it = existingBlocks.begin(); it != existingBlocks.end(); ++it) {
		for(Parser::block_set_iterator jt = existingBlocks.begin(); jt != existingBlocks.end(); ++jt) {
			Literal equal(true, Predicate(Predicate::EQUAL, *jt, tableStr, "", 0));
			for(int i = 1; i <= time; ++i) {
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
		for(int i = 1; i <= time; ++i) {
			Literal on(false, Predicate(Predicate::ON, *it, *it, "", i));
			Clause itself;
			itself << on;
			cnf << itself;
		}
	}

	// The table cannot be on another block
	for(Parser::block_set_iterator it = existingBlocks.begin(); it != existingBlocks.end(); ++it) {
		for(int i = 1; i <= time; ++i) {
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
				for(int i = 1; i <= time; ++i) {
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
				for(int i = 1; i <= time; ++i) {
					Literal on1(false, Predicate(Predicate::ON, *it, *kt, "", i));
					Literal on2(false, Predicate(Predicate::ON, *jt, *kt, "", i));
					Clause support;
					support << equal1 << on1 << on2 << equal2;
					cnf << support;
				}
			}
		}
	}

	// Add states
	// Initial state
	for(Parser::world_state_iterator it = initial.begin(); it != initial.end(); ++it) {
		Literal onTable(true, Predicate(Predicate::ON, *it->begin(), tableStr, "", 1));
		Clause onTableClause;
		onTableClause << onTable;
		cnf << onTableClause;
		for(Parser::pile_iterator jt = it->begin() + 1; jt != it->end(); ++jt) {
			Literal onBlock(true, Predicate(Predicate::ON, *jt, *(jt - 1), "", 1));
			Clause onBlockClause;
			onBlockClause << onBlock;
			cnf << onBlockClause;
		}
		Literal topClear(true, Predicate(Predicate::CLEAR, *(it->end() - 1), "", "", 1));
		Clause topClearClause;
		topClearClause << topClear;
		cnf << topClearClause;
	}

	// Goal state
	for(Parser::world_state_iterator it = goal.begin(); it != goal.end(); ++it) {
		Literal onTable(true, Predicate(Predicate::ON, *it->begin(), tableStr, "", time));
		Clause onTableClause;
		onTableClause << onTable;
		cnf << onTableClause;
		for(Parser::pile_iterator jt = it->begin() + 1; jt != it->end(); ++jt) {
			Literal onBlock(true, Predicate(Predicate::ON, *jt, *(jt - 1), "", time));
			Clause onBlockClause;
			onBlockClause << onBlock;
			cnf << onBlockClause;
		}
		Literal topClear(true, Predicate(Predicate::CLEAR, *(it->end() - 1), "", "", time));
		Clause topClearClause;
		topClearClause << topClear;
		cnf << topClearClause;
	}	

	// Add action axioms
	// Move X from Y to Z
	for(Parser::block_set_iterator it = existingBlocks.begin(); it != existingBlocks.end(); ++it) {
		for(Parser::block_set_iterator jt = existingBlocks.begin(); jt != existingBlocks.end(); ++jt) {
			for(Parser::block_set_iterator kt = existingBlocks.begin(); kt != existingBlocks.end(); ++kt) {
				for(int i = 1; i < time; ++i) {
					Literal XonY(false, Predicate(Predicate::ON, *it, *jt, "", i));
					Literal Xclear(false, Predicate(Predicate::CLEAR, *it, "", "", i));
					Literal Zclear(false, Predicate(Predicate::CLEAR, *kt, "", "", i));
					Literal XY(true, Predicate(Predicate::EQUAL, *it, *jt, "", 0));
					Literal XZ(true, Predicate(Predicate::EQUAL, *it, *kt, "", 0));
					Literal YZ(true, Predicate(Predicate::EQUAL, *jt, *kt, "", 0));
					Literal Xtable(true, Predicate(Predicate::EQUAL, *it, tableStr, "", 0));
					Literal move(false, Predicate(Predicate::MOVE, *it, *jt, *kt, i));
					Literal newOn(true, Predicate(Predicate::ON, *it, *kt, "", i + 1));
					Literal newClear(true, Predicate(Predicate::CLEAR, *jt, "", "", i + 1));
					Clause c1, c2;
					c1 << XonY << Xclear << Zclear << XY << XZ << YZ << Xtable << move << newOn;
					c2 << XonY << Xclear << Zclear << XY << XZ << YZ << Xtable << move << newClear;
					cnf << c1 << c2;
				}
			}
		}
	}

	// Actions are only executed if their preconditions hold
	for(Parser::block_set_iterator it = existingBlocks.begin(); it != existingBlocks.end(); ++it) {
		for(Parser::block_set_iterator jt = existingBlocks.begin(); jt != existingBlocks.end(); ++jt) {
			for(Parser::block_set_iterator kt = existingBlocks.begin(); kt != existingBlocks.end(); ++kt) {
				for(int i = 1; i < time; ++i) {
					Literal move(false, Predicate(Predicate::MOVE, *it, *jt, *kt, i));
					Literal Xclear(true, Predicate(Predicate::CLEAR, *it, "", "", i));
					Literal Zclear(true, Predicate(Predicate::CLEAR, *kt, "", "", i));
					Literal XonY(true, Predicate(Predicate::ON, *it, *jt, "", i));
					Literal XY(false, Predicate(Predicate::EQUAL, *it, *jt, "", 0));
					Literal XZ(false, Predicate(Predicate::EQUAL, *it, *kt, "", 0));
					Literal YZ(false, Predicate(Predicate::EQUAL, *jt, *kt, "", 0));
					Literal Xtable(false, Predicate(Predicate::EQUAL, *it, tableStr, "", 0));
					Clause c1, c2, c3, c4, c5, c6, c7;
					c1 << move << Xclear;
					c2 << move << Zclear;
					c3 << move << XonY;
					c4 << move << XY;
					c5 << move << XZ;
					c6 << move << YZ;
					c7 << move << Xtable;
					cnf << c1 << c2 << c3 << c4 << c5 << c6 << c7;
				}
			}
		}
	}

	// Only one action occurs at a time
	for(Parser::block_set_iterator it = existingBlocks.begin(); it != existingBlocks.end(); ++it) {
		for(Parser::block_set_iterator jt = existingBlocks.begin(); jt != existingBlocks.end(); ++jt) {
			for(Parser::block_set_iterator kt = existingBlocks.begin(); kt != existingBlocks.end(); ++kt) {
				for(Parser::block_set_iterator lt = existingBlocks.begin(); lt != existingBlocks.end(); ++lt) {
					for(Parser::block_set_iterator mt = existingBlocks.begin(); mt != existingBlocks.end(); ++mt) {
						for(Parser::block_set_iterator nt = existingBlocks.begin(); nt != existingBlocks.end(); ++nt) {
							for(int i = 0; i < time; ++i) {
								Literal move1(false, Predicate(Predicate::MOVE, *it, *kt, *mt, i));
								Literal move2(false, Predicate(Predicate::MOVE, *jt, *lt, *nt, i));
								Literal XX(true, Predicate(Predicate::EQUAL, *it, *jt, "", 0));
								Literal YY(true, Predicate(Predicate::EQUAL, *kt, *lt, "", 0));
								Literal ZZ(true, Predicate(Predicate::EQUAL, *mt, *nt, "", 0));
								Clause c1, c2, c3;
								c1 << move1 << move2 << XX;
								c2 << move1 << move2 << YY;
								c3 << move1 << move2 << ZZ;
								cnf << c1 << c2 << c3;
							}
						}
					}
				}
			}
		}
	}

	// Some action occurs at every time
	for(int i = 0; i < time; ++i) {
		Clause action;
		for(Parser::block_set_iterator it = existingBlocks.begin(); it != existingBlocks.end(); ++it) {
			for(Parser::block_set_iterator jt = existingBlocks.begin(); jt != existingBlocks.end(); ++jt) {
				for(Parser::block_set_iterator kt = existingBlocks.begin(); kt != existingBlocks.end(); ++kt) {
					Literal move(true, Predicate(Predicate::MOVE, *it, *jt, *kt, i));
					action << move;
				}
			}
		}
		cnf << action;
	}

	// Add frame axioms
	// A clear block which is not covered as a result of a move action stays clear
	for(Parser::block_set_iterator it = existingBlocks.begin(); it != existingBlocks.end(); ++it) {
		for(Parser::block_set_iterator jt = existingBlocks.begin(); jt != existingBlocks.end(); ++jt) {
			for(Parser::block_set_iterator kt = existingBlocks.begin(); kt != existingBlocks.end(); ++kt) {
				for(Parser::block_set_iterator lt = existingBlocks.begin(); lt != existingBlocks.end(); ++lt) {
					for(int i = 0; i < time; ++i) {
						Literal clear(false, Predicate(Predicate::CLEAR, *jt, "", "", i));
						Literal move(false, Predicate(Predicate::MOVE, *it, *kt, *lt, i));
						Literal X2Y(true, Predicate(Predicate::EQUAL, *jt, *kt, "", 0));
						Literal X2Z(true, Predicate(Predicate::EQUAL, *jt, *lt, "", 0));
						Literal newClear(true, Predicate(Predicate::CLEAR, *jt, "", "", i + 1));
						Clause stayClear;
						stayClear << clear << move << X2Y << X2Z << newClear;
						cnf << stayClear;
					}
				}
			}
		}
	}

	// A block stays on top of another one if it is not moved
	for(Parser::block_set_iterator it = existingBlocks.begin(); it != existingBlocks.end(); ++it) {
		for(Parser::block_set_iterator jt = existingBlocks.begin(); jt != existingBlocks.end(); ++jt) {
			for(Parser::block_set_iterator kt = existingBlocks.begin(); kt != existingBlocks.end(); ++kt) {
				for(Parser::block_set_iterator lt = existingBlocks.begin(); lt != existingBlocks.end(); ++lt) {
					for(Parser::block_set_iterator mt = existingBlocks.begin(); mt != existingBlocks.end(); ++mt) {
						for(int i = 0; i < time; ++i) {
							Literal on(false, Predicate(Predicate::ON, *jt, *lt, "", i));
							Literal move(false, Predicate(Predicate::MOVE, *it, *kt, *mt, i));
							Literal X1X2(true, Predicate(Predicate::EQUAL, *it, *jt, "", 0));
							Literal newOn(true, Predicate(Predicate::ON, *jt, *lt, "", i + 1));
							Clause stay;
							stay << on << move << X1X2 << newOn;
							cnf << stay;
						}
					}
				}
			}
		}
	}
}
