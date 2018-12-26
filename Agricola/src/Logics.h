/*
 * Logics.h
 *
 *  Created on: 20 nov 2018
 *      Author: m1rma
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

} /* namespace base */

namespace agr {

struct Condition: virtual public base::Log {
	virtual base::Primitive<bool> operator ()(const Log* = nullptr) const = 0;
	virtual std::ostringstream prints() const = 0;

	Condition(std::string, const Log* = nullptr);
	virtual ~Condition();
	Condition(const Condition&);
	Condition& operator =(const Condition&);
};
struct Event: virtual public base::Log {
	virtual void operator ()(base::Primitive<agr::Player*>,
			const Log* = nullptr) = 0;
	virtual std::ostringstream prints() const = 0;

	Event(std::string, const Log* = nullptr);
	virtual ~Event();
	Event(const Event&);
	Event& operator =(const Event&);
};
class Action: public base::Ensemble, public Condition, public Event {
	base::Primitive<Player*> performer;
protected:
	Action(std::string, const Log* = nullptr, base::Fields = nullptr);
	Action(const Action&);
	Action& operator =(const Action&);
public:
	base::Primitive<Player*> is_performed_by(const Log* = nullptr);
	virtual std::ostringstream prints() const = 0;
	virtual base::Quantity does_collect(const Log* = nullptr) = 0;

	virtual ~Action() = default;
};

} /* namespace agr */

#endif /* LOGICS_H_ */
