/*
 * executable.cpp
 *
 *  Created on: 22 gen 2018
 *      Author: MLaRosa
 */

#include "executable.h"

//Executable
std::map<std::string, std::function<int(short unsigned, bool, base::Log)>> Executable::operations;

std::string Executable::field(std::string variable, unsigned derived) const {
	if (variable == "active")
		return std::to_string(active);
	else if (variable == "name")
		return name;
	else {
		std::ostringstream result;

		result << "std::string Executable{" << this
				<< "}.field(std::string variable=\"" << variable
				<< "\", [derived=" << derived
				<< "])= ERROR invalid argument: invalid variable.";
		std::cerr << result.str() << std::endl;
		throw std::invalid_argument(result.str());
	}
}
void Executable::field(std::string variable, size_t position, std::string value,
		unsigned derived) {
	if (variable == "active")
		active = std::stoul(value);
	else if (variable == "name")
		name = value;
	else if (variable == "operations")
		operations[value] = (int (*)(short unsigned, bool,
				base::Log)) position;
	else {
		std::ostringstream result;

		result << "void base::Executable{" << this
				<< "}.field(std::string variable=\"" << variable
				<< "\", size_t position=" << position
				<< ", std::string value=\"" << value << "\", derived="
				<< derived << ")= ERROR invalid argument: invalid variable.";
		std::cerr << result.str() << std::endl;
		throw std::invalid_argument(result.str());
	}
}

short unsigned Executable::used() const {
	std::clog << base::Log().tracker() << "short unsigned Executable{" << name
			<< "}.used()=" << active << std::endl;

	return active;
}
Executable::operator std::string() const {
	std::clog << base::Log().tracker() << "(std::string) Executable{" << name
			<< "}" << std::endl;

	return name;
}
int Executable::operator ()(short unsigned player, bool simulation,
		base::Log track) {
	int result = 0;
	std::ostringstream log;

	log << track.tracker() << "int Executable{" << name
			<< "}(short unsigned player=" << player << ", bool simulation"
			<< simulation << ")";
	std::clog << log.str() << " {" << std::endl;
	if (player > Player::quantity()) {
		std::ostringstream message;

		message << "= ERROR player > player number=" << Player::quantity()
				<< ".";
		std::clog << track() << "}" << message.str() << std::endl;
		std::cerr << log.str() << message.str() << std::endl;
		throw std::domain_error(log.str() + message.str());
	}
	if (!simulation)
		active = player;
	result = operations[name](player, simulation, track);
	std::clog << track() << "}=" << result << std::endl;

	return result;
}

int Executable::null(short unsigned player, bool simulation, base::Log track) {
	return -1;
}

Executable::Executable(std::string name, base::Log track) {
	active = 0;
	this->name = name;
	std::clog << track.tracker() << "Executable::Executable(std::string name="
			<< name << ")" << std::endl;
}

