/*
 * Board.cpp
 *
 *  Created on: 2 gen 2019
 *      Author: m1r
 */

#include "Board.h"
#include "Simulations.h"
#include "Events.h"

namespace agr {
Board construct_gameboard(const base::Log* caller = nullptr) {
	auto log = base::Log::as_function(base::make_scopes(AGR, __func__), true,
			caller, typeid(Board));

	return Board(base::make_scopes(AGR, TYPEID(Board)), &log, nullptr);
}

Board gameBoard(construct_gameboard());

void construct_board(const base::Log* caller) {
	auto log = base::Log::as_function(base::make_scopes(AGR, __func__), true,
			caller, typeid(void));
	base::Primitive<size_t> I(6, &log);
	base::Class<std::string> cardSpace(CARD_SPACE_NAME, &log);
	base::Primitive<std::string> ensemble(TYPEID(Ensemble), &log);
	base::Primitive<const base::Log*> it(&log, &log);

	for (base::Primitive<size_t> i(1, &log); i <= I; ++i)
		gameBoard.generates<base::Ensemble>(cardSpace, i, &log, ensemble, it);
	gameBoard.generates<Action<BuildRooms_andOr_BuildStables>>(
			base::Class<std::string>(TYPEID(BuildRooms_andOr_BuildStables),
					&log), base::Primitive<size_t>(1, &log), &log,
			Condition::Function(Condition::no, &log),
			Simulator(build_rooms_and_or_build_stables, &log),
			base::Quantity(std::map<std::type_index, int>(), &log),
			base::Primitive<const base::Log*>(&log, &log));
}

} /* namespace agr */
