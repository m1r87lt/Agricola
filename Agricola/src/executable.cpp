/*
 * executable.cpp
 *
 *  Created on: 22 gen 2018
 *      Author: MLaRosa
 */

#include "executable.h"

//Executable
std::map<std::string, Executable*> Executable::index;

const std::string& Executable::has_name() const {
	return as_method("", nullptr,
			base::Variable<const decltype(label)&>("has_name", "std", label));
}
int Executable::operator ()(bool attempting, const base::Log* caller) {
	auto log = as_method<int>(caller, "", "",
			base::Variable<decltype(attempting)&>("attempting", "",
					attempting));

	if (attempting)
		return log.returns(attempts(&log));
	else
		return log.returns(executes(&log));
}
std::vector<std::string> Executable::operator ()(std::string name) const {
	std::vector<std::string> result;
	auto log = as_method<decltype(result)>(nullptr, "", "");

	if (index.find(name) != index.end())
		result.push_back(name);

	return log.returns<decltype(result)&>(result,
			[result](const decltype(result)& r) {
				return "{" + r[0] + "}";
			});
}
int Executable::gives_null() {
	return as_function("", nullptr, base::Variable<int>("gives_null", "", 0),
			"", typeid(Executable));
}

Executable::Executable(std::string name, const base::Log* caller) :
		Log(caller, "", false, "") {
	label = name;
	active = nullptr;
}

