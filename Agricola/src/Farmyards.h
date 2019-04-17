/*
 * Farmyards.h
 *
 *  Created on: 3 gen 2019
 *      Author: m1r
 */

#ifndef FARMYARDS_H_
#define FARMYARDS_H_

#include "Components.h"

namespace agr {

std::type_index give_room_type(const Farmyard&);
std::type_index give_next_room_cost(const Farmyard&);

} /* namespace agr */

#endif /* FARMYARDS_H_ */
