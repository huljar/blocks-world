#include <blocks/predicate.h>

using namespace blocks;

Predicate::Predicate(pred_type type, const std::string& blockX, const std::string& blockY, const std::string& blockZ, int time) :
	type(type),
	blockX(blockX),
	blockY(blockY),
	blockZ(blockZ),
	time(time)
{

}

/*Predicate::Predicate(pred_type type, const char* blockX, const char* blockY, const char* blockZ, int time) :
	type(type),
	blockX(blockX),
	blockY(blockY),
	blockZ(blockZ),
	time(time)
{

}*/
