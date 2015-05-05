#ifndef PREDICATE_H
#define PREDICATE_H

#include <string>

namespace blocks {
	class Predicate;
}

class blocks::Predicate {

public:
	typedef enum { ON, CLEAR, MOVE, EQUAL } pred_type;

	Predicate(pred_type type, const std::string& blockX, const std::string& blockY, const std::string& blockZ, int time);
	//Predicate(pred_type type, const char* blockX, const char* blockY, const char* blockZ, int time);

private:
	pred_type type;
	std::string blockX;
	std::string blockY;
	std::string blockZ;
	int time;

};

#endif
