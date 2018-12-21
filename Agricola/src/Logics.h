/*
 * Logics.h
 *
 *  Created on: 20 nov 2018
 *      Author: m1rma
 */

#ifndef LOGICS_H_
#define LOGICS_H_

#include "Player.h"

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
/*
 class Action: public Event,
 public Condition,
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
 */
} /* namespace agr */

#endif /* LOGICS_H_ */
