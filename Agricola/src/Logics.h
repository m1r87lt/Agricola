/*
 * Logics.h
 *
 *  Created on: 20 nov 2018
 *      Author: m1r
 */

#ifndef LOGICS_H_
#define LOGICS_H_

#include "Player.h"

namespace agr {

struct Operation: public base::Log {
	using Unique_ptr = base::Class<std::unique_ptr<Operation>>;

	virtual base::Primitive<bool> operator ()() const = 0;

	virtual ~Operation() = default;
protected:
	base::Primitive<short unsigned> state;

	Operation(std::string, const Log* = nullptr);
	Operation(const Operation&);
	Operation& operator =(const Operation&);
};

} /* namespace agr */

namespace base {

using Quantity = Class<std::map<std::type_index, int>>;
template<typename First, typename Second> std::ostringstream unprint_std__map_std__type_info_second_(
		const std::map<First, Second>& container) {
	std::ostringstream result("{");

	for (auto content : container)
		result << "\n" << content.first.name() << ": " << content.second;
	result << (result.str() == "{" ? " " : "\n") << "}";

	return result;
}
template<typename Function> std::ostringstream print_std__function(
		const std::function<Function>& function) {
	std::ostringstream result;

	result << "std::function<" << typeid(Function).name() << ">";

	return result;
}
template<> class Class<std::unique_ptr<agr::Operation>> : public Unique_ptr {
	Class(Unique_ptr&&);
public:
	virtual const agr::Operation& operator *() const;
	virtual agr::Operation& operator *();
	static Class<std::unique_ptr<agr::Operation>> dynamicCast(Unique_ptr &&);
	template<typename Type, typename ... Arguments> static Class<
			std::unique_ptr<agr::Operation>> construct(const Log* caller =
			nullptr, Arguments&& ... arguments) {
		return dynamicCast(
				Unique_ptr::construct<Type>(caller,
						std::forward<Arguments&&>(arguments) ...));
	}

	virtual ~Class() = default;
	Class(const Class<std::unique_ptr<agr::Operation>>&) = delete;
	Class(Class<std::unique_ptr<agr::Operation>> &&);
	Class<std::unique_ptr<agr::Operation>>& operator =(
			const Class<std::unique_ptr<agr::Operation>>&) = delete;
};

} /* namespace base */

namespace agr {

class Condition: virtual public base::Log {
	base::Class<std::function<base::Primitive<bool>(const Log*)>> condition;
public:
	base::Primitive<bool> operator ()(const Log* = nullptr) const;
	virtual std::ostringstream prints() const = 0;
	static base::Primitive<bool> no(const Log* = nullptr);

	Condition(base::Class<std::function<base::Primitive<bool>(const Log*)>>,
			std::string, const Log* = nullptr);
	virtual ~Condition() = default;
	Condition(const Condition&);
	Condition& operator =(const Condition&);
};
template<typename Function> class Event: virtual public base::Log {
	base::Class<std::function<base::Primitive<bool>(Player&, const Log*)>> simulation;
public:
	base::Primitive<bool> simulates(const Player& player, const Log* caller =
			nullptr) const {
		auto log = as_method(__func__, caller, typeid(base::Primitive<bool>),
				player);

		return log.returns(simulation.is()(player, &log));
	}
	Operation::Unique_ptr operator ()(Player& player, const Log* caller =
			nullptr) {
		auto log = as_method("", caller, typeid(Operation::Unique_ptr), player);

		return log.returns(
				Operation::Unique_ptr::construct<Function>(&log, player,
						base::Primitive<const Log*>(&log, &log)));
	}
	virtual std::ostringstream prints() const = 0;

	Event(
			base::Class<
					std::function<base::Primitive<bool>(Player&, const Log*)>> event,
			std::string label, const Log* caller = nullptr) :
			NEW_LOG(caller, label, false), simulation(simulation) {
		as_constructor(AGR, __func__, this);
	}
	virtual ~Event() = default;
	Event(const Event& copy) :
			Object(copy), Log(&copy, copy.has_label(), false), simulation(
					copy.simulation) {
	}
	Event& operator =(const Event& copy) {
		simulation = copy.simulation;

		return as_binary<false>(__func__, copy, typeid(Event&)).returns(*this);
	}
};
template<typename Function> struct Action final: public base::Ensemble,
		public Condition,
		public Event<Function> {
	base::Primitive<Player*> performer;
	base::Quantity collection;
	friend Ensemble::Unique_ptr;
	Action(
			base::Class<std::function<base::Primitive<bool>(const Log*)>> condition,
			base::Class<
					std::function<base::Primitive<bool>(Player&, const Log*)>> event,
			base::Quantity collection, const Log* caller = nullptr,
			base::Fields attributes = nullptr) :
			ENSEMBLE(false, base::make_scopes(AGR, __func__), caller, attributes), Condition(
					condition, caller), Event<Function>(event, caller), performer(
					nullptr, caller), collection(collection) {
		as_constructor(AGR, __func__, caller, condition, event, collection,
				attributes);
	}
public:
	base::Primitive<Player*> is_performed_by(const Log* caller = nullptr) {
		return as_method(__func__, caller, typeid(base::Primitive<Player*>)).returns(
				performer);
	}
	virtual std::ostringstream prints() const {
		std::ostringstream result;

		result << has_label() << "{" << this << "}";

		return result;
	}
	base::Quantity does_collect(const Log* caller = nullptr) {
		return as_method<false>(__func__, caller, typeid(base::Quantity)).returns(
				collection);
	}
	Ensemble::Unique_ptr construct(
			base::Class<std::function<base::Primitive<bool>(const Log*)>> condition,
			base::Class<
					std::function<base::Primitive<bool>(Player&, const Log*)>> event,
			base::Quantity collection, const Log* caller = nullptr,
			base::Fields attributes = nullptr) {
		auto log = as_method(base::make_scopes(AGR, TYPEID(Action), __func__),
				true, caller, typeid(base::Quantity), condition, event,
				collection, attributes);

		return log.returns(
				Ensemble::Unique_ptr::construct<Action>(&log, condition, event,
						collection, base::Primitive<const Log*>(&log, &log),
						attributes));
	}
};

} /* namespace agr */

#endif /* LOGICS_H_ */
