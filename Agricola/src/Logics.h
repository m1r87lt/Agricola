/*
 * Logics.h
 *
 *  Created on: 20 nov 2018
 *      Author: m1r
 */

#ifndef LOGICS_H_
#define LOGICS_H_

#include "Player.h"

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

} /* namespace base */

namespace agr {

class Condition: virtual public base::Log {
	base::Class<std::function<base::Primitive<bool>(const Log*)>> condition;
public:
	base::Primitive<bool> operator ()(const Log* = nullptr) const;
	virtual std::ostringstream prints() const = 0;

	Condition(base::Class<std::function<base::Primitive<bool>(const Log*)>>,
			const Log* = nullptr);
	virtual ~Condition();
	Condition(const Condition&);
	Condition& operator =(const Condition&);
};
class Event: virtual public base::Log {
	base::Class<std::function<base::Primitive<bool>(Player&, const Log*)>> event;
public:
	base::Primitive<bool> operator ()(agr::Player&, const Log* = nullptr);
	virtual std::ostringstream prints() const = 0;

	Event(
			base::Class<
					std::function<base::Primitive<bool>(Player&, const Log*)>>,
			const Log* = nullptr);
	virtual ~Event();
	Event(const Event&);
	Event& operator =(const Event&);
};
class Action: public base::Ensemble, public Condition, public Event {
	base::Primitive<Player*> performer;
	base::Quantity collection;
protected:
	Action(base::Class<std::function<base::Primitive<bool>(const Log*)>>,
			base::Class<
					std::function<base::Primitive<bool>(Player&, const Log*)>>,
			base::Quantity, const Log* = nullptr, base::Fields = nullptr);
	friend base::Unique_ptr;
public:
	base::Primitive<Player*> is_performed_by(const Log* = nullptr);
	virtual std::ostringstream prints() const;
	virtual base::Quantity does_collect(const Log* = nullptr);
	static base::Unique_ptr construct(
			base::Class<std::function<base::Primitive<bool>(const Log*)>>,
			base::Class<
					std::function<base::Primitive<bool>(Player&, const Log*)>>,
			base::Quantity, const Log* = nullptr, base::Fields = nullptr);

	virtual ~Action() = default;
	Action(const Action&);
	Action& operator =(const Action&);
};

} /* namespace agr */

#endif /* LOGICS_H_ */
