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

void preparing_to_play() {
	construct_board();
}

int main() {
	std::cout << "Agricola" << std::endl; // prints Agricola

	preparing_to_play();

	return 0;
}
