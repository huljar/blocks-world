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

bool Predicate::operator==(const Predicate& other) const {
	if(type != other.getType()) return false;
	if(type == CLEAR && blockX == other.getBlockX() && time == other.getTime()) return true;
	if(type == EQUAL && blockX == other.getBlockX() && blockY == other.getBlockY()) return true;
	if(type == ON && blockX == other.getBlockX() && blockY == other.getBlockY() && time == other.getTime()) return true;
	if(type == MOVE && blockX == other.getBlockX() && blockY == other.getBlockY() && blockZ == other.getBlockZ() && time == other.getTime()) return true;
	return false;
}

Predicate::pred_type Predicate::getType() const {
	return type;
}

std::string Predicate::getBlockX() const {
	return blockX;
}

std::string Predicate::getBlockY() const {
	return blockY;
}

std::string Predicate::getBlockZ() const {
	return blockZ;
}

int Predicate::getTime() const {
	return time;
}
