/*
 * Board.cpp
 *
 *  Created on: 2 gen 2019
 *      Author: m1r
 */

#include "Board.h"

namespace agr {
Board gameBoard(TYPEID(Board));

void construct_board(const base::Log* caller) {
	auto log = base::Log::as_function(base::make_scopes(AGR, __func__), true,
			caller, typeid(void));
	base::Primitive<size_t> I(6, &log);
	base::Class<std::string> cardSpace(CARD_SPACE_NAME, &log);
	base::Primitive<std::string> ensemble(TYPEID(Ensemble), &log);
	base::Primitive<const base::Log*> it(&log, &log);

	for (base::Primitive<size_t> i(1, &log); i <= I; ++i)
		gameBoard.generates<base::Ensemble>(cardSpace, i, &log, ensemble, it);
	//gameBoard.generates<base::Ensemble>();
}

} /* namespace agr */
