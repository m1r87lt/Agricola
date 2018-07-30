/*
 * executable.cpp
 *
 *  Created on: 22 gen 2018
 *      Author: MLaRosa
 */

#include "executable.h"

//Executable
Executable::operator const char*() const {
	return base::Log::unary<const char*>(nullptr,
			base::variable(name.c_str(), "(const char*)"), typeid(), "");
}
int Executable::operator ()(std::string operation, bool attempting,
		const Log* caller) {
	auto log = method<int>(caller, "", "",
			base::variable(operation, "operation"),
			base::variable(attempting, "attempt"));

	if (attempt)
		return log.returning(attempt(operation, &log));
	else
		return log.returning(execute(operation, &log));
}
int Executable::null(Player* nobody, bool attempt, const base::Log* caller) {
	return method<int>("", caller, base::variable(0, "null"));
}

Executable::Executable(std::string name, const base::Log* caller):
		Log(caller, "", false, "") {
	active = nullptr;
	this->name = name;
}

