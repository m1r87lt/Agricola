/*
 * Board.cpp
 *
 *  Created on: 2 gen 2019
 *      Author: m1r
 */

#include "Board.h"

#include "Conditions.h"
#include "Events.h"

namespace agr {
Board construct_gameboard(base::Object::Fields attributes =
		base::Object::Fields()) {
	return Board(attributes);
}

Board gameBoard(construct_gameboard());

void construct_board() {
	for (size_t i = 1; i <= 6; ++i)
		gameBoard.generates<CardSpace>(NAME(CardSpace));
/*	gameBoard.gets(NAME(agr::BuildRooms_andOr_BuildStables),
			Action<Condition::No, BuildRooms_andOr_BuildStables>::construct(),
			7);*/
}

//CardSpace
CardSpace::Fields CardSpace::shows() const {
	return Ensemble::shows();
}
std::string CardSpace::prints() const {
	return NAME(agr::CardSpace)+"{ " + std::get<2>(localize(*this));
}

}
/* namespace agr */
