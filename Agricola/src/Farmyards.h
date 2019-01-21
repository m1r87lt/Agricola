/*
 * Farmyards.h
 *
 *  Created on: 3 gen 2019
 *      Author: m1r
 */

#ifndef FARMYARDS_H_
#define FARMYARDS_H_

#include "Components.h"
#define BUILD_ROOM_TYPE_RESOURCE

namespace agr {

base::Class<std::pair<std::type_index, short unsigned>> how_many_rooms_may_it_build(
		const Player&, const base::Log* = nullptr);

} /* namespace agr */

#endif /* FARMYARDS_H_ */
