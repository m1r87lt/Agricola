/*
 * Logics.cpp
 *
 *  Created on: 20 nov 2018
 *      Author: m1r
 */

#include "Logics.h"

namespace base {

template<> std::function<
		std::ostringstream(const std::map<std::type_index, int>&)> Class<
		std::map<std::type_index, int>>::printer =
		unprint_std__map_std__type_info_second_<std::type_index, int>;
template<> std::function<
		std::ostringstream(const std::function<Primitive<bool>(const Log*)>&)> Class<
		std::function<Primitive<bool>(const Log*)>>::printer =
		print_std__function<Primitive<bool>(const Log*)>;
template<> std::function<
		std::ostringstream(
				const std::function<
						Primitive<bool>(Primitive<bool>, agr::Player&,
								const Log*)>&)> Class<
		std::function<Primitive<bool>(Primitive<bool>, agr::Player&, const Log*)>>::printer =
		print_std__function<
				Primitive<bool>(Primitive<bool>, agr::Player&, const Log*)>;

} /* namespace base */

namespace agr {

//Condition
base::Primitive<bool> Condition::operator ()(const Log* caller) const {
	auto log = as_method("", caller, typeid(base::Primitive<bool>));

	return log.returns(condition.is()(&log));
}
base::Primitive<bool> Condition::no(const Log* caller) {
	return base::function_primitive(true,
			base::make_scopes(AGR, TYPEID(Condition), __func__), caller);
}

Condition::Condition(
		base::Class<std::function<base::Primitive<bool>(const Log*)>> condition,
		const Log* caller) :
		NEW_LOG(caller, base::make_scopes(AGR, __func__), false), condition(
				condition) {
	as_constructor(AGR, __func__, this);
}
Condition::~Condition() {
	as_destructor(AGR, __func__);
}
Condition::Condition(const Condition& copy) :
		Object(copy), Log(&copy, base::make_scopes(AGR, __func__), false), condition(
				copy.condition) {
}
Condition& Condition::operator =(const Condition& copy) {
	condition = copy.condition;

	return as_binary<false>(__func__, copy, typeid(Condition&)).returns(*this);
}

//Event
base::Primitive<bool> Event::operator ()(base::Primitive<bool> simulation,
		Player& performer, const Log* caller) {
	auto log = as_method("", caller, typeid(base::Primitive<bool>), simulation,
			performer);

	return log.returns(event.is()(simulation, performer, &log));
}

Event::Event(
		base::Class<
				std::function<
						base::Primitive<bool>(base::Primitive<bool>, Player&,
								const Log*)>> event, const Log* caller) :
		NEW_LOG(caller, base::make_scopes(AGR, __func__), false), event(event) {
	as_constructor(AGR, __func__, this);
}
Event::~Event() {
	as_destructor(AGR, __func__);
}
Event::Event(const Event& copy) :
		Object(copy), Log(&copy, base::make_scopes(AGR, __func__), false), event(
				copy.event) {
}
Event& Event::operator =(const Event& copy) {
	event = copy.event;

	return as_binary<false>(__func__, copy, typeid(Event&)).returns(*this);
}

//Action
base::Primitive<Player*> Action::is_performed_by(const Log* caller) {
	return as_method(__func__, caller, typeid(base::Primitive<Player*>)).returns(
			performer);
}
std::ostringstream Action::prints() const {
	std::ostringstream result;

	result << has_label() << "{" << this << "}";

	return result;
}
base::Quantity Action::does_collect(const Log* caller) {
	return as_method<false>(__func__, caller, typeid(base::Quantity)).returns(
			collection);
}
base::Unique_ptr Action::construct(
		base::Class<std::function<base::Primitive<bool>(const Log*)>> condition,
		base::Class<
				std::function<
						base::Primitive<bool>(base::Primitive<bool>, Player&,
								const Log*)>> event, base::Quantity collection,
		const Log* caller, base::Fields attributes) {
	auto log = as_method(base::make_scopes(AGR, TYPEID(Action), __func__), true,
			caller, typeid(base::Quantity), condition, event, collection,
			attributes);

	return log.returns(
			base::Unique_ptr::construct<Action>(&log, condition, event,
					collection, base::Primitive<const Log*>(&log, &log),
					attributes));
}

Action::Action(
		base::Class<std::function<base::Primitive<bool>(const Log*)>> condition,
		base::Class<
				std::function<
						base::Primitive<bool>(base::Primitive<bool>, Player&,
								const Log*)>> event, base::Quantity collection,
		const Log* caller, base::Fields attributes) :
		ENSEMBLE(false, base::make_scopes(AGR, __func__), caller, attributes), Condition(
				condition, caller), Event(event, caller), performer(nullptr,
				caller), collection(collection) {
	as_constructor(AGR, __func__, caller, condition, event, collection,
			attributes);
}

} /* namespace agr */
