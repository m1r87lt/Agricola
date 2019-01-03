//============================================================================
// Name        : Agricola.cpp
// Author      : m1r
// Version     : 1.0
// Copyright   : 2018
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "Board.h"
using namespace agr;

void preparing_to_play() {
	auto log = base::Log::as_function(__func__, true, nullptr, typeid(void));

	construct_board(&log);
}

int main() {
	std::cout << "Agricola" << std::endl; // prints Agricola

	preparing_to_play();

	return 0;
}
