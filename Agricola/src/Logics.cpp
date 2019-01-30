/*
 * Logics.cpp
 *
 *  Created on: 20 nov 2018
 *      Author: m1r
 */

#include "Logics.h"

namespace agr {

//Trigger
Trigger::Fields Trigger::shows() const {
	return Object::shows();
}

//Simulator
Simulator::Fields Simulator::shows() const {
	auto result = Object::shows();

	result.insert(VARIABLE(state));

	return result;
}

Simulator::Simulator() {
	state = 1;
}

} /* namespace agr */
