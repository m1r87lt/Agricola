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
				const std::function<Primitive<bool>(agr::Player&, const Log*)>&)> Class<
		std::function<Primitive<bool>(agr::Player&, const Log*)>>::printer =
		print_std__function<Primitive<bool>(agr::Player&, const Log*)>;

//Class<std::unique_ptr<agr::Operation>>
const agr::Operation& Class<std::unique_ptr<agr::Operation>>::operator *() const {
	return dynamic_cast<const agr::Operation&>(Unique_ptr::operator *());
}
agr::Operation& Class<std::unique_ptr<agr::Operation>>::operator *() {
	return dynamic_cast<agr::Operation&>(Unique_ptr::operator *());
}

Class<std::unique_ptr<agr::Operation>> Class<std::unique_ptr<agr::Operation>>::dynamicCast(
		Unique_ptr&& log) {
	return Class<std::unique_ptr<agr::Operation>>(std::move(log));
}

Class<std::unique_ptr<agr::Operation>>::Class(Unique_ptr && moving) :
		Unique_ptr(std::move(moving)) {
}
Class<std::unique_ptr<agr::Operation>>::Class(
		Class<std::unique_ptr<agr::Operation>> && moving) :
		Unique_ptr(std::move(moving)) {
}

} /* namespace base */

namespace agr {

//Operation
Operation::Operation(std::string label, const Log* caller) :
		NEW_LOG(caller, label, false), state(1, caller) {
	as_constructor(AGR, __func__, caller);
}
Operation::Operation(const Operation& copy) :
		Object(copy), Log(&copy, copy.has_label(), false), state(1, &copy) {
}
Operation& Operation::operator =(const Operation& copy) {
	return *this;
}

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
		std::string label, const Log* caller) :
		NEW_LOG(caller, label, false), condition(condition) {
	as_constructor(AGR, __func__, this);
}
Condition::Condition(const Condition& copy) :
		Object(copy), Log(&copy, copy.has_label(), false), condition(
				copy.condition) {
}
Condition& Condition::operator =(const Condition& copy) {
	condition = copy.condition;

	return as_binary<false>(__func__, copy, typeid(Condition&)).returns(*this);
}

} /* namespace agr */
