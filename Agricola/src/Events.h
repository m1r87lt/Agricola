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

class BuildRooms_andOr_BuildStables: public Simulator {
	//TODO BuildRooms_andOr_BuildStables declaration
	void state2();
public:
	virtual bool operator ()();
	virtual Fields shows() const;
	virtual std::string prints() const;
	BuildRooms_andOr_BuildStables(Player&);
};

} /* namespace agr */

#endif /* EVENTS_H_ */
