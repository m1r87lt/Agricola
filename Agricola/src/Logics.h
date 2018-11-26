/*
 * Logics.h
 *
 *  Created on: 20 nov 2018
 *      Author: m1rma
 */

#ifndef LOGICS_H_
#define LOGICS_H_

#include "Player.h"

namespace a {

struct Condition: public base::Log {
	virtual base::Primitive<bool> operator ()(base::Primitive<short>,
			const Log* = nullptr) const = 0;
	virtual std::ostringstream prints() const = 0;

	Condition(base::Class<std::string>, const Log* = nullptr);
	virtual ~Condition();
	Condition(const Condition&);
	Condition& operator =(const Condition&);
	Condition(Condition&&);
	Condition& operator =(Condition&&);
};
class Quantity: public base::Log {
	unsigned quantity;
public:
	virtual std::ostringstream prints() const = 0;

	Quantity(base::Primitive<unsigned>, base::Class<std::string>, const Log* =
			nullptr);
	virtual ~Quantity();
	Quantity(const Quantity&);
	Quantity& operator =(const Quantity&);
	Quantity(Quantity&&);
	Quantity& operator =(Quantity&&);
};
struct Event: public base::Log {
	virtual void operator ()(base::Primitive<a::Player*>, const Log* =
			nullptr);
	virtual std::ostringstream prints() const = 0;

	Event(base::Class<std::string>, const Log* = nullptr);
	virtual ~Event();
	Event(const Event&);
	Event& operator =(const Event&);
	Event(Event&&);
	Event& operator =(Event&&);
};
class Action: public virtual Event,
		public virtual Condition,
		public base::Ensemble {
	virtual std::vector<Quantity*> does_collect(const Log* = nullptr) const;
	virtual std::ostringstream prints() const;

	Action(base::Class<std::string>, const Log* = nullptr);
	virtual ~Action();
	Action(const Action&);
	Action& operator =(const Action&);
	Action(Action&&);
	Action& operator =(Action&&);
};

} /* namespace a */

#endif /* LOGICS_H_ */
