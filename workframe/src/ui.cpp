/*
 * ui.cpp
 *
 *  Created on: 04 mar 2018
 *      Author: MLaRosa
 */

#include "ui.h"

namespace base {/*
std::pair<bool, std::map<std::string, std::string>> dialog(std::string caption,
		std::map<std::string, std::pair<std::set<std::string>, std::string>> variables,
		Log track) {
	std::pair<bool, std::map<std::string, std::string>> result = { true, { } };
	std::ostringstream argument;
	auto end = variables.end();
	std::ostringstream log;
	std::string message = " user terminated.";

	for (auto variable : variables) {
		argument << ",\n\t " << variable.first << ": "
				<< Prompt::list(variable.second.first) << "; "
				<< variable.second.second;
	}
	argument.str(
			argument.str().empty() ?
					"{ }" : "{" + argument.str().substr(1) + "\n}");
	log << track.tracker()
			<< "std::pair< bool, std::map< std::string, std::string > > "
					"dialog(std::string caption=\"" << caption
			<< "\", std::map< std::string, std::pair< std::set< "
					"std::string >, std::string > > variables="
			<< argument.str() << ")";
	std::clog << log.str() << " {" << std::endl;
	std::cout << std::endl << "========================================"
			"========================================\n" << caption
			<< std::endl;
	for (auto variable = variables.begin(); variable != end && result.first;
			++variable) {
		std::cout << "________________________________________"
				"________________________________________\n"
				<< variable->second.second << " \"" << variable->first << "\" ";
		result.second[variable->first] = "";
		if (variable->second.first.size()) {
			std::cout << Prompt::list(variable->second.first) << "= ";
			for (std::getline(std::cin, result.second[variable->first]);
					variable->second.first.find(result.second[variable->first])
							== variable->second.first.end()
							&& result.second[variable->first] != "!"
							&& result.second[variable->first] != "#";
					std::getline(std::cin, result.second[variable->first])) {
				std::clog << track()
						<< "the user is requested to choose between "
								"a set of values, but it typed "
						<< result.second[variable->first] << std::endl;
				std::cout
						<< "Typed value is not in the possible set. Type again? ";
			}
			if (variable->second.first.find(result.second[variable->first])
					== variable->second.first.end()
					&& result.second[variable->first] == "!") {
				std::clog << track() << " }" << message << std::endl;
				throw std::runtime_error(log.str() + message);
			}
			result.first = variable->second.first.find(
					result.second[variable->first])
					!= variable->second.first.end();
		} else {
			std::cout << "= ";
			std::getline(std::cin, result.second[variable->first]);
			if (result.second[variable->first] == "!") {
				std::clog << track() << " }" << message << std::endl;
				throw std::runtime_error(log.str() + message);
			}
			result.first = result.second[variable->first] != "#";
		}
		std::clog << track() << "the user typed '"
				<< result.second[variable->first] << "'" << std::endl;
	}
	for (std::string command;
			command != "!" && command != "#" && command != ">";
			result.first = command == ">") {
		std::cout << "['!' quit, '#' cancel, '>' proceed";
		if (variables.begin() != end)
			std::cout << ", '^{variable}' modify";
		std::cout << "]? ";
		std::getline(std::cin, command);
		if (command == "!") {
			std::clog << track() << " }" << message << std::endl;
			throw std::runtime_error(log.str() + message);
		}
		if (command.substr(0, 1) == "^"
				&& variables.find(command.substr(1)) != end) {
			std::cout << result.second[command.substr(1)] << " -> ";
			if (variables[command.substr(1)].first.size()) {
				std::cout << Prompt::list(variables[command.substr(1)].first)
						<< "= ";
				for (std::getline(std::cin, result.second[command.substr(1)]);
						variables[command.substr(1)].first.find(
								result.second[command.substr(1)])
								== variables[command.substr(1)].first.end()
								&& result.second[command.substr(1)] != "!"
								&& result.second[command.substr(1)] != "#";
						std::getline(std::cin,
								result.second[command.substr(1)])) {
					std::clog << track()
							<< "the user is requested to choose between "
									"a set of values, but it typed "
							<< result.second[command.substr(1)] << std::endl;
					std::cout
							<< "Typed value is not in the possible set. Type again? ";
				}
				if (variables[command.substr(1)].first.find(
						result.second[command.substr(1)])
						== variables[command.substr(1)].first.end()
						&& result.second[command.substr(1)] == "!") {
					std::clog << track() << " }" << message << std::endl;
					throw std::runtime_error(log.str() + message);
				}
				result.first = variables[command.substr(1)].first.find(
						result.second[command.substr(1)])
						!= variables[command.substr(1)].first.end();
			} else {
				std::cout << "= ";
				std::getline(std::cin, result.second[command.substr(1)]);
				if (result.second[command.substr(1)] == "!") {
					std::clog << track() << " }" << message << std::endl;
					throw std::runtime_error(log.str() + message);
				}
				result.first = result.second[command.substr(1)] != "#";
			}
			std::clog << track() << "the user perform the modify '"
					<< command.substr(1) << "'->"
					<< result.second[command.substr(1)] << std::endl;
		}
	}
	std::cout << "========================================"
			"========================================" << std::endl;
	std::clog << track() << "}=" << result.first << std::endl;

	return result;
}

std::string display(const base::Object* object) {
	std::ostringstream result;

	result << "[" << object->what() << "]" << base::Location::who(*object)
			<< "{" << object << "}";

	return result.str();
}
std::string path(std::list<const base::Object*> sequence) {
	std::ostringstream result;

	if (sequence.size()) {
		result << "/" << display(sequence.front());
		sequence.pop_front();
		result << path(sequence);
	}

	return result.str();
}
std::string tree(const base::Object* object, size_t nesting) {
	std::ostringstream result;
	const base::Location* container = nullptr;
	size_t i = 0;

	for (size_t i = 0; i < nesting; ++i)
		result << "| ";
	result << "+-";
	if ((container = dynamic_cast<const base::Location*>(object))) {
		result << "# " << display(object) << "\n";
		for (auto size = container->size(); i < size; ++i)
			result << tree((*container)[i], nesting + 1);
	} else
		result << "- " << display(object) << "\n";

	return result.str();
}*/
} /* namespace base */
