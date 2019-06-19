/*
 * Rules.h
 *
 *  Created on: 04 jun 2019
 *      Author: m1r
 */

#ifndef RULES_H_
#define RULES_H_

#include "src/Logics.h"
#include "Player.h"

namespace agr {

template<typename Type> class Quantity: public base::Object {
	int quantity;
public:
	using type = Type;

	int how_many_are() const {
		return quantity;
	}
	virtual Fields shows() const {
		auto result = Object::shows();

		result.insert(VARIABLE(quantity));

		return result;
	}
	virtual std::string prints() const {
		return "agr::Quantity{ " + std::to_string(quantity) + " "
				+ typeid(Type).name() + " }";
	}
};
template<typename Type> class Action final: public base::Ensemble {
	std::vector<Object*> collection;
	std::unique_ptr<game::Simulator::Choice> choice;

	Action(std::vector<Object*> collection = { }, Fields attributes = Fields()) :
			Ensemble(attributes), choice(new Type) {
		this->collection = collection;
	}
public:
	Type* gives_trigger() const {
		return choice.get();
	}
	std::vector<Object*> does_collect() {
		return collection;
	}
	virtual Fields shows() const {
		auto result = Ensemble::shows();

		result.insert(VARIABLE(collection));

		return result;
	}
	virtual std::string prints() const {
		std::ostringstream result;

		result << "agr::Action{ " << this << " }";

		return result.str();
	}
	static Unique_ptr construct(std::vector<Object*> collection = { },
			Fields attributes = Fields()) {
		return Unique_ptr(new Action<Type>(collection, attributes));
	}
};

class BuildRooms_Stables final : public game::Simulator::Choice {
	virtual bool operator ()(std::string);
	virtual std::set<std::string> provides();
	virtual std::string prints() const;
	virtual void reverts();
public:
	BuildRooms_Stables();
	~BuildRooms_Stables();
};
class Room {
	static std::map<Player*,
			std::pair<std::vector<std::vector<base::Object*>>,
					std::list<std::unique_ptr<game::Trigger>>>> costs;
	friend BuildRooms_Stables;
};

} /* namespace agr */

#endif /* RULES_H_ */
