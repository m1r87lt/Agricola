/*
 * executable.cpp
 *
 *  Created on: 22 gen 2018
 *      Author: MLaRosa
 */

#include "executable.h"

//Executable
Executable::operator const char*() const {
	return base::Log::unary(nullptr,
			base::variable(name.c_str(), "(const char*)"),
			base::Variable<Executable>(Executable), "");
}
int Executable::operator ()(std::string operation, bool attempting,
		const base::Log* caller) {
	auto log = base::Log::function<int>(caller, "", typeid(Executable),
			"{" + name + "}", "", base::variable(operation, "operation"),
			base::variable(attempting, "attempt"));

	if (attempt)
		return log.returning(attempt(operation, &log));
	else
		return log.returning(execute(operation, &log));
}
int Executable::null(Player* nobody, bool attempt, const base::Log* caller) {
	return base::Log::function<int>("", caller,
			base::variable(0, "{" + name + "}.null"), "", typeid(Executable));
}

Executable::Executable(std::string name, const base::Log* caller) :
		Log(caller, "", false, "") {
	active = nullptr;
	this->name = name;
}

