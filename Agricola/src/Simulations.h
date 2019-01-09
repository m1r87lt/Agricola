/*
 * Simulations.h
 *
 *  Created on: 3 gen 2019
 *      Author: m1r
 */

#ifndef SIMULATIONS_H_
#define SIMULATIONS_H_

#include "Components.h"

namespace agr {

base::Primitive<bool> build_rooms_and_or_build_stables(Player&,
		const base::Log* = nullptr);

} /* namespace agr */

#endif /* SIMULATIONS_H_ */
