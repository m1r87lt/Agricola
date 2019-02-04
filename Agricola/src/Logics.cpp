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
void Simulator::start(Unique_ptr* it) {
	if (it->get() == this)
		++state;
	else {
		std::ostringstream result(
				"The first argument of Simulator::start must be the pointer to itself, instead of {");

		result << it->get() << " }.";

		throw base::Throw::not_allowed(result.str());
	}
}
unsigned Simulator::gives_state() const {
	return state;
}
void Simulator::pass_to_state(unsigned state) {
	if (state < 2)
		throw base::Throw::not_allowed(
				"An invalid Simulator-state is passed { "
						+ std::to_string(state) + " }.");
	this->state = state;
}
Player* Simulator::gives_performer() const {
	return performer;
}

Simulator::Fields Simulator::shows() const {
	auto result = Object::shows();

	result.insert(VARIABLE(state));

	return result;
}

Simulator::Simulator(Player& performer) {
	state = 1;
	it = nullptr;
	this->performer = &performer;
}

} /* namespace agr */
