/*
 * Events.cpp
 *
 *  Created on: 8 gen 2019
 *      Author: m1r
 */

#include "Events.h"

namespace agr {

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

BuildRooms_andOr_BuildStables::BuildRooms_andOr_BuildStables(Operation& operation) :
		Simulator(operation) {
}

} /* namespace agr */
