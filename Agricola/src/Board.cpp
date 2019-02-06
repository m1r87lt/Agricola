/*
 * Board.cpp
 *
 *  Created on: 2 gen 2019
 *      Author: m1r
 */

#include "Board.h"
#include "Conditions.h"
#include "Events.h"

namespace base {

template<> std::function<std::string(agr::Player* const *)> Class<agr::Player*>::printer =
		print_fundamental<agr::Player*>;
template<> std::function<std::string(std::unique_ptr<agr::Trigger>* const *)> Class<
		std::unique_ptr<agr::Trigger>*>::printer = print_fundamental<
		std::unique_ptr<agr::Trigger>*>;

} /* namespace base */

namespace agr {
Board construct_gameboard(base::Object::Fields attributes =
		base::Object::Fields()) {
	return Board(attributes);
}

Board gameBoard(construct_gameboard());

void construct_board() {
	for (size_t i = 1; i <= 6; ++i)
		gameBoard.generates<CardSpace>(NAME(CardSpace));
	gameBoard.generates<Action<Conditional::No, BuildRooms_andOr_BuildStables>>(
			NAME(BuildRooms_andOr_BuildStables), 7, Quantity());
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
