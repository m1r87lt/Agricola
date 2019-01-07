/*
 * Actions.h
 *
 *  Created on: 3 gen 2019
 *      Author: m1r
 */

#ifndef ACTIONS_H_
#define ACTIONS_H_

#include "Components.h"

namespace agr {

base::Primitive<bool> build_rooms(base::Primitive<bool>, Player&,
		const base::Log*);

} /* namespace agr */

#endif /* ACTIONS_H_ */
