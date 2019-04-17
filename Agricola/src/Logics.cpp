/*
 * Logics.cpp
 *
 *  Created on: 20 nov 2018
 *      Author: m1r
 */

#include "Logics.h"

namespace agr {

//Trigger
base::Object& Trigger::gives_parent() const {
	return *condition;
}

Trigger::Fields Trigger::shows() const {
	auto result = Object::shows();

	result.insert(VARIABLE(condition));

	return result;
}

Trigger::Trigger(Object& condition) {
	this->condition = &condition;
}
Trigger::~Trigger() {
	dynamic_cast<Conditional*>(condition)->trigger = nullptr;
}

//Conditional
Conditional::Conditional() {
	trigger = nullptr;
}
Conditional::~Conditional() {
//	Master::removes_trigger(trigger);
}

//Conditional::No
bool Conditional::No::operator ()() {
	return true;
}
Conditional::No::Fields Conditional::No::shows() const {
	return Trigger::shows();
}
std::string Conditional::No::prints() const {
	return NAME(agr::Trigger)+ "::" + NAME(No);
}
Conditional::No::No(Conditional& condition) :
		Trigger(condition) {
}

//Simulator
void Simulator::starts() {
	++state;
}
base::Object* Simulator::gives_operation() const {
	return operation;
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
std::set<base::Object*> Simulator::gives_branches() const {
	return branches;
}
void Simulator::gets_branch(base::Object* branch) {
	branches.emplace(branch);
}

Simulator::Fields Simulator::shows() const {
	auto result = Object::shows();

	result.insert(VARIABLE(state));
	result.insert(VARIABLE(operation));
	result.insert(VARIABLE(branches));

	return result;
}

Simulator::Simulator(Object& operation) {
	state = 1;
	this->operation = &operation;
}

//Operation
Player* Operation::is_performed_by() {
	return performer;
}
Operation::Fields Operation::shows() const {
	auto result = Object::shows();

	result.insert(VARIABLE(performer));

	return result;
}
/*
//Key
short unsigned Key::has_number() const {
	return number;
}
Key::Event Key::has_event() const {
	return event;
}
Key::Type Key::has_type() const {
	return type;
}/*
Key::Fields Key::shows() const {
	auto result = Object::shows();

	result.insert(VARIABLE(number));
	result.insert(
			std::make_pair(const_cast<const std::string>(TYPE(event)),
					event_label));
	result.insert(
			std::make_pair(const_cast<const std::string>(TYPE(type)),
					type_label));

	return result;
}
std::string Key::prints() const {
	return NAME(Key)+ "{ " + event_label + "; " + type_label + "; " + std::to_string(number) + " }";
}
Key::Key(Event event_value, std::string event_label, Type type_value,
		std::string type_label, short unsigned number) {
	event = event_value;
	this->event_label = event_label;
	type = type_value;
	this->type_label = type_label;
	this->number = number;
}
*/
}
/* namespace agr */
