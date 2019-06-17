/*
 * Events.cpp
 *
 *  Created on: 8 gen 2019
 *      Author: m1r
 */

#include "Events.h"
#include "Farmyards.h"

namespace agr {

//Cost
/*Farmyard* Cost::gives_farmyard() const {
	return farmyard;
}

Cost::Cost(Farmyard& farmyard, Quantity def) {
	this->farmyard = &farmyard;
	cost = def;
}

//BuildRooms
void BuildRooms::state2() {
	//TODO BuildRooms::state2
	/*Evaluation of room's type and cost.*//*
	if (types.empty()) {
		auto farmyard = gives_farmyard();

		if (farmyard)
			types.emplace_back(give_room_type(*farmyard));
	}
	if ()
}

bool BuildRooms::operator ()() {
	//TODO BuildRooms ()
}
BuildRooms::Fields BuildRooms::shows() const {
	//TODO BuildRooms_andOr_BuildStables::shows
}

BuildRooms::BuildRooms(Operation& operation, Farmyard&, Quantity,
		std::vector<std::type_index>) :
		Simulator(operation) {
	//TODO BuildRooms::BuildRooms
}

//BuildRooms_andOr_BuildStables
void BuildRooms_andOr_BuildStables::state2() {
	//TODO BuildRooms_andOr_BuildStables::state2
}

bool BuildRooms_andOr_BuildStables::operator ()() {
	//TODO BuildRooms_andOr_BuildStables ()
}
BuildRooms_andOr_BuildStables::Fields BuildRooms_andOr_BuildStables::shows() const {
	//TODO BuildRooms_andOr_BuildStables::shows
}
std::string BuildRooms_andOr_BuildStables::prints() const {
	std::ostringstream result(NAME(agr::BuildRooms_andOr_BuildStables));

	result << "{ " << this << " }";

	return result.str();
}

BuildRooms_andOr_BuildStables::BuildRooms_andOr_BuildStables(
		Operation& operation) :
		Simulator(operation) {
	//TODO BuildRooms_andOr_BuildStables::BuildRooms_andOr_BuildStables
}
*/
} /* namespace agr */
