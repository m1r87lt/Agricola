/*
 * board.cpp
 *
 *  Created on: 05 apr 2018
 *      Author: MLaRosa
 */

#include "board.h"
/*
base::Log logging() {
	return base::Log();
}

Gameboard gameboard0(logging());
Gameboard gameboard1(logging());
Gameboard gameboard2(logging());
auto actionCards = game::Deck::construct(B_ACTION_CARDS, base::Log());
auto roundCards = game::Deck::construct(B_ROUND_CARDS, base::Log());
auto occupations = game::Deck::construct(B_OCCUPATIONS, base::Log());
auto minorImprovements = game::Deck::construct(B_MINORIMPROVEMENTS,
		base::Log());
MajorImprovementBoard majorImprovements(logging());

Gameboard* game_board(size_t number) {
	std::ostringstream log;

	log << base::Log().tracker() << "Gameboard* game_board(size_t number="
			<< number << ")=";
	std::clog << log.str();
	if (number == 0) {
		std::clog << &gameboard0 << std::endl;

		return &gameboard0;
	} else if (number == 1) {
		std::clog << &gameboard1 << std::endl;

		return &gameboard1;
	} else if (number == 2) {
		std::clog << &gameboard2 << std::endl;

		return &gameboard2;
	} else {
		std::clog << 0 << std::endl;
		std::cerr << log.str()
				<< " WARNING invalid argument: game board are between 0 and 2.";

		return nullptr;
	}
}
game::Deck* deck(std::string name) {
	std::ostringstream log;

	log << base::Log().tracker() << "game::Deck* deck(std::string name=" << name
			<< ")=";
	std::clog << log.str();
	if (name == B_ACTION_CARDS) {
		std::clog << actionCards << std::endl;

		return actionCards;
	} else if (name == B_ROUND_CARDS) {
		std::clog << roundCards << std::endl;

		return roundCards;
	} else if (name == B_OCCUPATIONS) {
		std::clog << occupations << std::endl;

		return occupations;
	} else if (name == B_MINORIMPROVEMENTS) {
		std::clog << minorImprovements << std::endl;

		return minorImprovements;
	} else {
		std::clog << 0 << std::endl;
		std::cerr << log.str()
				<< " WARNING invalid argument: a deck with this name does not exist.";

		return nullptr;
	}
}
MajorImprovementBoard* major_improvement_board() {
	std::clog << base::Log().tracker()
			<< "MajorImprovementBoard* major_improvement_board()="
			<< &majorImprovements << std::endl;

	return &majorImprovements;
}

Card::Round* former_empty(unsigned round, unsigned span) {
	Card::Round* result = nullptr;
	base::Log track;

	std::clog << track.tracker() << "Cover* round_space(unsigned round="
			<< round << ", unsigned span=" << span << ") {" << std::endl;
	result = dynamic_cast<Card::Round*>(gameboard0(
			std::string(B_ROUND) + " " + std::to_string(round)));
	std::clog << track() << "}=" << result << std::endl;

	return result;
}
*/
