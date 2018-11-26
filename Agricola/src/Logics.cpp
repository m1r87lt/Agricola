/*
 * Logics.cpp
 *
 *  Created on: 20 nov 2018
 *      Author: m1rma
 */

#include "Logics.h"

namespace a {

Condition::Condition(const Log* caller) :
		Object(caller, base::make_scopes(__func__, "a")), Log(caller,
				base::make_scopes(__func__, "a"), false) {
	as_constructor("a", __func__, this);
}
Condition::~Condition() {
	as_destructor("a", __func__);
}
Condition::Condition(const Condition& copy) :
		Object(copy), Log(copy) {
}
Condition& Condition::operator =(const Condition& assigned) {
	Object::operator =(assigned);

	return *this;
}
Condition::Condition(Condition&& moving) :
		Object(std::move(moving)), Log(std::move(moving)) {
}
Condition& Condition::operator =(Condition&& assigning) {
	Object::operator =(std::move(assigning));
	Log::operator =(std::move(assigning));

	return *this;
}
Quantity::Quantity(const Log* caller) :
		Object(caller, base::make_scopes(__func__, "a")), Log(caller,
				base::make_scopes(__func__, "a"), false) {
	as_constructor("a", __func__, this);
}
Quantity::~Quantity() {
	as_destructor("a", __func__);
}
Quantity::Quantity(const Quantity& copy) :
		Object(copy), Log(copy) {
}
Quantity& Quantity::operator =(const Quantity& assigned) {
	Object::operator =(assigned);

	return *this;
}
Quantity::Quantity(Quantity&& moving) :
		Object(std::move(moving)), Log(std::move(moving)) {
}
Quantity& Quantity::operator =(Quantity&& assigning) {
	Object::operator =(std::move(assigning));
	Log::operator =(std::move(assigning));

	return *this;
}
Event::Event(const Log* caller) :
		Object(caller, base::make_scopes(__func__, "a")), Log(caller,
				base::make_scopes(__func__, "a"), false) {
	as_constructor("a", __func__, this);
}
Event::~Event() {
	as_destructor("a", __func__);
}
Event::Event(const Event& copy) :
		Object(copy), Log(copy) {
}
Event& Event::operator =(const Event& assigned) {
	Object::operator =(assigned);

	return *this;
}
Event::Event(Event&& moving) :
		Object(std::move(moving)), Log(std::move(moving)) {
}
Event& Event::operator =(Event&& assigning) {
	Object::operator =(std::move(assigning));
	Log::operator =(std::move(assigning));

	return *this;
}

} /* namespace a */
