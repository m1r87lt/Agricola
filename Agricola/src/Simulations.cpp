/*
 * Simulations.cpp
 *
 *  Created on: 3 gen 2019
 *      Author: m1r
 */

#include "Farmyards.h"
#include "Simulations.h"

namespace agr {

base::Primitive<bool> build_rooms_and_or_build_stables(Player& player,
		const base::Log* caller) {
	using Result = base::Primitive<bool>;
	auto log = base::Log::as_function(base::make_scopes(AGR, __func__), true,
			caller, typeid(base::Primitive<bool>), player);
	Result result(false, &log);



	return log.returns(result);
}

} /* namespace agr */
