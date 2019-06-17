/*
 * Rules.cpp
 *
 *  Created on: 04 jun 2019
 *      Author: m1r
 */

#include "Rules.h"

namespace base {

template<> std::function<std::string(const std::vector<Object*>*)> Class<
		std::vector<Object*>>::printer =
		print_std__vector<Object*>;

} /* namespace base */

namespace agr {

//Room
Player* Room::player = nullptr;
std::vector<std::vector<base::Object*>> Room::costs;

//Room::BuildRooms_Stables
bool Room::BuildRooms_Stables::operator ()(std::string command) {
	size_t pos = 0;
	int quantity = std::stoi(command, &pos);

	if (quantity) {
		return true;
	} else
		throw throw_invalid_command(this, command);
}
std::set<std::string> Room::BuildRooms_Stables::provides() {
	std::set<std::string> result;

	result.emplace("# rooms");
	result.emplace("# stables");
	result.emplace("#rooms and # stables");

	return result;
}
std::string Room::BuildRooms_Stables::prints() const {

}

Room::BuildRooms_Stables();
	~Room::BuildRooms_Stables();

} /* namespace agr */
