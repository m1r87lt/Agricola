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
struct Conditional {
	virtual Trigger::Unique_ptr operator ()(const Player&) = 0;
	virtual ~Conditional() = default;
};
template<typename Function> struct Condition: public Conditional,
		virtual public base::Object {
	virtual Trigger::Unique_ptr operator ()(const Player& player) {
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
			return NAME(agr::Trigger)+ "::" + NAME(No);
		}
	};
	virtual Fields shows() const {
		return Object::shows();
	}
};

struct Simulator: public base::Object {
	using Unique_ptr = std::unique_ptr<Simulator>;

	void start(Unique_ptr*);
	virtual bool operator ()() = 0;
	virtual Fields shows() const;
	virtual ~Simulator() = default;
private:
	unsigned state;
	Unique_ptr* it;
	Player* performer;
protected:
	Player* gives_performer() const;
	unsigned gives_state() const;
	void pass_to_state(unsigned);
	Simulator(Player&);
};
class Operation {
	virtual Simulator::Unique_ptr operator ()(Player&) = 0;
	virtual ~Operation() = default;
};
template<typename Function> struct Operator: public Operation,
		virtual public base::Object {
	virtual Simulator::Unique_ptr operator ()(Player& player) {
		return Simulator::Unique_ptr(new Function(player));
	}
	virtual Fields shows() const {
		return Object::shows();
	}
	virtual ~Operator() = default;
};
template<typename Trig, typename Operate> class Event: public Condition<Trig>,
		public Operator<Operate> {
	Player* performer;
protected:
	Event() {
		performer = nullptr;
	}
public:
	Player* is_performed_by() {
		return performer;
	}
	virtual ~Event() = default;
};

template<typename Trig, typename Operate> class Action final: public base::Ensemble,
		public Event<Trig, Operate> {
	Quantity collection;

	Action(Quantity collection, Fields attributes = Fields()) :
			Ensemble(attributes), Event<Trig, Operate>() {
		this->collection = collection;
	}
public:
	Quantity does_collect() {
		return collection;
	}
	virtual Fields shows() const {
		auto result = Ensemble::shows();
		auto event = Event<Trig, Operate>::shows();

		result.insert(event.begin(), event.end());
		result.insert(VARIABLE(collection));

		return result;
	}
	virtual std::string prints() const {
		std::ostringstream result;

		result << "agr::Action{ " << this << " }";

		return result.str();
	}
	static Ensemble::Unique_ptr construct(Quantity collection = { },
			Fields attributes = Fields()) {
		return Ensemble::Unique_ptr(
				new Action<Trig, Operate>(collection, attributes));
	}
};

} /* namespace agr */

#endif /* LOGICS_H_ */
