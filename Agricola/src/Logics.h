/*
 * Logics.h
 *
 *  Created on: 20 nov 2018
 *      Author: m1r
 */

#ifndef LOGICS_H_
#define LOGICS_H_

#include "Player.h"
#define KEY(event, type, number) agr::Key(event, #event, type, #type, number)

namespace agr {

using Quantity = std::map<std::type_index, unsigned>;

class Trigger: public base::Object {
	Object* condition;
protected:
	Object& gives_parent() const;
	Trigger(Object&);
public:
	using Unique_ptr = std::unique_ptr<Trigger>;

	virtual bool operator ()() = 0;
	virtual Fields shows() const;
	virtual ~Trigger();
};
class Conditional: public virtual base::Object {
protected:
	Trigger::Unique_ptr* trigger;
public:
	struct No final: public Trigger {
		virtual bool operator ()();
		virtual Fields shows() const;
		virtual std::string prints() const;
		No(Conditional&);
	};
	friend Trigger;
	virtual Fields shows() const {
		auto result = Object::shows();

		result.insert(VARIABLE(trigger));

		return result;
	}
	virtual Trigger::Unique_ptr operator ()() = 0;
	Conditional();
	virtual ~Conditional();
};
template<typename Function> struct Condition: public Conditional {
	virtual Trigger::Unique_ptr operator ()() {
		return Trigger::Unique_ptr(new Function(*this));
	}
	virtual ~Condition() = default;
};

struct Simulator: public base::Object {
	using Unique_ptr = std::unique_ptr<Simulator>;

	void starts();
	virtual bool operator ()() = 0;
	virtual Fields shows() const;
	virtual ~Simulator() = default;
private:
	unsigned state;
	Object* operation;
	std::set<Object*> branches;
protected:
	Object* gives_operation() const;
	unsigned gives_state() const;
	void pass_to_state(unsigned);
	std::set<Object*> gives_branches() const;
	void gets_branch(Object*);
	Simulator(Object&);
};
class Operation: virtual public base::Object {
	friend Simulator;
protected:
	Player* performer;
public:
	Player* is_performed_by();
	virtual Fields shows() const;
	virtual Simulator::Unique_ptr operator ()() = 0;
	virtual ~Operation() = default;
};
template<typename Function> struct Operator: public Operation,
		virtual public base::Object {
	virtual Simulator::Unique_ptr operator ()() {
		return Simulator::Unique_ptr(new Function(*this));
	}
	virtual ~Operator() = default;
};

template<typename Trig, typename Operate> struct Event: public Condition<Trig>,
		public Operator<Operate> {
	virtual base::Object::Fields shows() const {
		auto result = Condition<Trig>::shows();
		auto o = Operator<Operate>::shows();

		result.insert(o.begin(), o.end());

		return result;
	}
	virtual ~Event() = default;
};
template<typename Trig, typename Operate> class Action final: public base::Ensemble,
		public Event<Trig, Operate> {
	Quantity collection;
	friend base::Ensemble;
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
/*
struct Key: public base::Object {
	enum class Event {
		action, spending, effect
	};
	enum class Type {
		buildRoom
	};

	short unsigned has_number() const;
	Event has_event() const;
	Type has_type() const;
	virtual Fields shows() const;
	virtual std::string prints() const;
	Key(Event, std::string, Type, std::string, short unsigned);
private:
	short unsigned number;
	Event event;
	Type type;
	std::string event_label;
	std::string type_label;
};
*/
struct Master: public base::Object {
	//TODO Master declaration
	class Branch: public base::Object {
		Simulator* parent;
		std::list<std::unique_ptr<Branch>> branches;
	public:
//		Simulator* gives_parent() const = 0;
//		virtual std::list<std::unique_ptr<Branch>> branches();
		Branch(const Simulator&);
		virtual ~Branch() = default;
	};

	static Quantity transcode(std::string);
private:
	static std::list<std::unique_ptr<Branch>> branches;
	static std::list<Trigger::Unique_ptr> triggers;
	friend Conditional;
	static void removes_trigger(Trigger::Unique_ptr*);
};

} /* namespace agr */

#endif /* LOGICS_H_ */
