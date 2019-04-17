/*
 * Events.h
 *
 *  Created on: 8 gen 2019
 *      Author: m1r
 */

#ifndef EVENTS_H_
#define EVENTS_H_

#include "Components.h"

namespace agr {

class Cost: virtual public base::Object {
	Player* personal_supply;
	std::list<std::unique_ptr<Quantity>> costs;

	void add(Quantity);
protected:
	virtual short unsigned evaluates() const = 0;
/*	template<typename ... Arguments> Cost(Player& farmyard, Arguments&& ... arguments) {
		farmyard =
	}*/
public:
	virtual ~Cost() = default;
};

class BuildRooms: public Simulator, protected Cost {
	std::vector<std::type_index> types;
	void state2();
public:
	virtual bool operator ()();
	virtual short unsigned evaluates() const;
	virtual Fields shows() const;
	template<typename> BuildRooms(Operation&, Player&, std::vector<std::type_index>);
};

class BuildRooms_andOr_BuildStables: public Simulator {
	//TODO BuildRooms_andOr_BuildStables declaration
	void state2();
public:
	virtual bool operator ()();
	virtual Fields shows() const;
	virtual std::string prints() const;
	BuildRooms_andOr_BuildStables(Operation&);
};

} /* namespace agr */

#endif /* EVENTS_H_ */
