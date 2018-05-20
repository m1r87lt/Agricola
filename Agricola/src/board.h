/*
 * board.h
 *
 *  Created on: 05 apr 2018
 *      Author: MLaRosa
 */

#ifndef BOARD_H_
#define BOARD_H_

#include "components.h"
#define B_ACTION_CARDS "Action cards"
#define B_MINORIMPROVEMENTS "Minor Improvements"
#define B_OCCUPATIONS "Occupations"
#define B_R1 "Round 1"
#define B_R2 "Round 2"
#define B_R3 "Round 3"
#define B_R4 "Round 4"
#define B_R5 "Round 5"
#define B_R6 "Round 6"
#define B_R7 "Round 7"
#define B_R8 "Round 8"
#define B_R9 "Round 9"
#define B_R10 "Round 10"
#define B_R11 "Round 11"
#define B_R12 "Round 12"
#define B_R13 "Round 13"
#define B_R14 "Round 14"
#define B_ROUND "Round"
#define B_ROUND_CARDS "Round cards"

Gameboard* game_board(size_t);
game::Deck* deck(std::string);
MajorImprovementBoard* major_improvement_board();

Card::Round* former_empty(unsigned round, unsigned span);

#endif /* BOARD_H_ */
