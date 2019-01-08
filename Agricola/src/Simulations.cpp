/*
 * Actions.cpp
 *
 *  Created on: 3 gen 2019
 *      Author: m1r
 */

#include "Actions.h"
#include "Farmyards.h"

namespace agr {

base::Primitive<bool> build_rooms(base::Primitive<bool> simulation,
		Player& player, const base::Log* caller) {
	auto log = base::Log::as_function(base::make_scopes(AGR, __func__), true,
			caller, typeid(base::Primitive<bool>));

	if (simulation)
		return log.returns(may_farmyard_build_rooms(player, &log));


	return log.returns(simulation);
}

} /* namespace agr */
