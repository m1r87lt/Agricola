/*
 * Logics.cpp
 *
 *  Created on: 20 nov 2018
 *      Author: m1rma
 */

#include "Logics.h"

namespace base {

template<> std::function<
		std::ostringstream(const std::map<std::type_index, int>&)> Class<
		std::map<std::type_index, int>>::printer =
		unprint_std__map_std__type_info_second_<std::type_index, int>;

}

namespace agr {

//Condition
Condition::Condition(std::string label, const Log* caller) :
		NEW_LOG(caller, label, false) {
			as_constructor(AGR, __func__, this);
		}
		Condition::~Condition() {
			as_destructor(AGR, __func__);
		}
		Condition::Condition(const Condition& copy) :
		Object(copy),
Log(&copy, base::make_scopes(AGR, __func__), false) {
}
Condition& Condition::operator =(const Condition& copy) {
	return as_binary<false>(__func__, copy, typeid(Condition&)).returns(*this);
}

//Event
Event::Event(std::string label, const Log* caller) :
		NEW_LOG(caller, label, false) {
			as_constructor(AGR, __func__, this);
		}
		Event::~Event() {
			as_destructor(AGR, __func__);
		}
		Event::Event(const Event& copy) :
		Object(copy),
Log(&copy, base::make_scopes(AGR, __func__), false) {
}
Event& Event::operator =(const Event& copy) {
	return as_binary<false>(__func__, copy, typeid(Event&)).returns(*this);
}

//Action
base::Primitive<Player*> Action::is_performed_by(const Log* caller) {
	return as_method(__func__, caller, typeid(base::Primitive<Player*>)).returns(
			performer);
}

Action::Action(std::string label, const Log* caller, base::Fields attributes) :
		ENSEMBLE(false, label, caller, attributes), Condition(label, caller), Event(
				label, caller), performer(nullptr, this) {
	as_constructor(AGR, __func__, caller);
}
Action::Action(const Action& copy) :
		Object(copy), Log(&copy, base::make_scopes(AGR, __func__), false), Ensemble(
				base::make_scopes(AGR, __func__), &copy,
				copy.gives_attributes()), Condition(copy), Event(copy), performer(
				copy.performer) {
}
Action& Action::operator =(const Action& copy) {
	Condition::operator =(copy);
	Event::operator =(copy);
	performer = copy.performer;
	return as_binary(__func__, copy, typeid(Action&)).returns(*this);
}

} /* namespace agr */
