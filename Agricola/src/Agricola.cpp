//============================================================================
// Name        : Agricola.cpp
// Author      : m1r
// Version     : 1.0
// Copyright   : 2018
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
using namespace std;
#include "components.h"

void preparing_to_play() {
	auto log = base::Log::as_function(__func__, true, nullptr, typeid(void));


}

int main() {
	cout << "Agricola" << endl; // prints Agricola

	preparing_to_play();

	return 0;
}
