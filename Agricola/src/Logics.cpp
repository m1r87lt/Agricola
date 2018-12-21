/*
 * Logics.cpp
 *
 *  Created on: 20 nov 2018
 *      Author: m1rma
 */

#include "Logics.h"

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

} /* namespace agr */
