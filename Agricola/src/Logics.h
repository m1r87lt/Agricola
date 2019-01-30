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

using Quantity = std::map<std::type_index, unsigned>;
struct Trigger: public base::Object {
	using Unique_ptr = std::unique_ptr<Trigger>;

	virtual bool operator ()() = 0;
	virtual Fields shows() const;
	virtual ~Trigger() = default;
protected:
	Trigger() = default;
};
template<typename Function> struct Condition: virtual public base::Object {
	Trigger::Unique_ptr operator ()(const Player& player) {
		return Trigger::Unique_ptr(new Function(player));
	}
	virtual ~Condition() = default;
protected:
	struct No final: public Trigger {
		virtual bool operator ()() {
			return true;
		}
		virtual Fields shows() const {
			return Trigger::shows();
		}
		virtual std::string prints() const {
			return NAME(agr::Trigger) + "::" + NAME(No);
		}
	};
	virtual Fields shows() const {
		return Object::shows();
	}
};

struct Simulator: public base::Object {
	using Unique_ptr = std::unique_ptr<Simulator>;

	virtual bool operator ()() = 0;
	virtual Fields shows() const;
	virtual ~Simulator() = default;
protected:
	unsigned state;

	Simulator();
};
template<typename Function> struct Operator: virtual public base::Object {
	Simulator::Unique_ptr operator ()(Player& player) {
		return Simulator::Unique_ptr(new Function(player));
	}
	virtual Fields shows() const {
		return Object::shows();
	}
	virtual ~Operator() = default;
};
template<typename Trig, typename Operate> class Action final: public base::Ensemble,
		public Condition<Trig>,
		public Operator<Operate> {
	Player* performer;
	Quantity collection;
public:
	Player* is_performed_by() {
		return performer;
	}
	Quantity does_collect() {
		return collection;
	}
	virtual Fields shows() const {
		auto result = Ensemble::shows();
		auto condition = Condition<Trig>::shows();
		auto op = Operator<Operate>::shows();

		result.insert(condition.begin(), condition.end());
		result.insert(op.begin(), op.end());
		result.insert(VARIABLE(performer));
		result.insert(VARIABLE(collection));

		return result;
	}
	virtual std::string prints() const {
		std::ostringstream result;

		result << NAME(agr::Action) << "{ " << this << " }";

		return result.str();
	}
	static Ensemble::Unique_ptr construct(Quantity collection = { }, Fields attributes = Fields()) {
		return Ensemble::Unique_ptr(new Action<Trig, Operate>(collection, attributes));
	}
};

} /* namespace agr */

#endif /* LOGICS_H_ */
