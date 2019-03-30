//============================================================================
// Name        : Agricola.cpp
// Author      : m1r
// Version     : 1.0
// Copyright   : 2018
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "Board.h"
#include <iostream>
using namespace agr;
/*
 void preparing_to_play() {
 construct_board();
 }
 */
int main() {
	std::cout << "Agricola" << std::endl; // prints Agricola

//	preparing_to_play();

	std::cout << "construct all Players...";
	Player::construct_all(
			std::vector<std::pair<std::string, Color>> { std::make_pair("Mark",
					COLOR(Color::Which::Blue)), std::make_pair("Lucy",
					COLOR(Color::Which::Yellow)) });
	std::cout << "\nhow many are they? " << Player::give_number()
			<< "\ntake the first [0]:\n\twho is it? ";
	Player& p = Player::give(1);
	auto who = p.who_is();
	std::cout << CLASS(who)() << "\n\twhich color has it? ";
	auto color = p.how_is();
	std::cout << CLASS(color)() << "\n\tprint it: " << p.prints() << "\n\tand show it: ";
	auto show = p.shows();
	std::cout << CLASS(show)() << "\ntake player \"Lucy\"... ";
	auto player = Player::give("Lucy");
	std::cout << player << "\n\twhich is it: " << player->which_is()
			<< "\n\tand which owns: ";
	auto owns = player->owns();
	std::cout << CLASS(owns)() << "\nfinally, get yellow player:\n\tyellow: "
			<< Player::give(COLOR(Color::Which::Yellow));

	return 0;
}
