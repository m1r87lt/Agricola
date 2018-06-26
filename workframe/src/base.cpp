/*
 * base.cpp
 *
 *  Created on: 15 dic 2017
 *      Author: m1r
 */

#include "base.h"
#include <stdexcept>
#include <chrono>///
#include <fstream>
#include <random>

namespace base {
bool run = true;

bool running() {
	return run;
}
void end() {
	run = false;
}

//Log
long long unsigned Log::tracker = 0;

std::string Log::tracking(const Log* caller) {
	if (caller)
		return caller->legacy + "." + std::to_string(caller->track);
	else
		return "";
}
std::string Log::messaging(std::string message) {
	if (message.length())
		message = " \"" + message + "\"";

	return message;
}
template<> std::string Log::type<std::type_info>(std::type_info&& object) {
	return object.name();
}
template<> std::string Log::type<const std::type_info&>(
		const std::type_info& object) {
	return object.name();
}
template<> std::string Log::type<std::type_index*>(std::type_index*&& object) {
	return object->name();
}
template<> std::string Log::type<const std::type_index*&>(
		const std::type_index*& object) {
	return object->name();
}
template<> std::type_index Log::returnType<std::type_index, std::type_index>(
		std::type_index&& returning) {
	return returning;
}
template<> std::type_index Log::returnType<std::type_index, std::type_index&>(
		std::type_index& returning) {
	return returning;
}
template<> std::string Log::returnValue<std::type_index, std::type_index>(
		std::type_index&& returning) {
	return " {";
}
template<> std::string Log::returnValue<std::type_index, std::type_index&>(
		std::type_index& returning) {
	return " {";
}
Log::list Log::arguments() {
	return list();
}
Log::list Log::parameters() {
	return list();
}
std::string Log::log() const {
	return legacy + "." + std::to_string(track) + ": " + logger;
}
void Log::message(std::string text) const {
	std::clog << legacy + "." + std::to_string(track) + "  " + messaging(text)
			<< std::endl;
}

Log::Log(std::type_index type) :
		returning(std::make_pair(std::make_pair(type, ""), "")) {
	track = ++tracker;
	open = false;
}
Log::~Log() {
	std::string log;

	if (open)
		log += legacy + "." + std::to_string(track) + "  }";
	if (returning.first.first != typeid(void))
		log += "=" + returning.second;
	if (log.length())
		std::clog << log << std::endl;
}
Log::Log(Log& copy) :
		returning(copy.returning) {
	legacy = copy.legacy;
	track = copy.track;
	ns = copy.ns;
	open = copy.open;
	logger = copy.logger;
	copy.open = false;
	copy.returning.first.first = typeid(void);
}
Log::Log(Log&& moved) :
		returning(moved.returning) {
	legacy = moved.legacy;
	track = moved.track;
	ns = moved.ns;
	open = moved.open;
	logger = moved.logger;
	moved.open = false;
	moved.returning.first.first = typeid(void);
}

//Object
std::set<Object*> Object::everything;

time_t Object::when() const {
	method<time_t>("", nullptr, "when", creation);

	return creation;
}
Object* Object::where() const {
	method<Object*>("", nullptr, "where", position);

	return position;
}
long long unsigned Object::who() const {
	method<long long unsigned>("", nullptr, "who", track);

	return track;
}
std::pair<time_t, std::map<std::string, std::pair<std::string, std::string>>> Object::what() {
	std::pair<time_t, std::map<std::string, std::pair<std::string, std::string>>> result;
	auto log = method<std::type_index>("", nullptr, "what", typeid(result));
	std::string logging = "{" + std::to_string(modification) + ";\n\t{";

	result.first = modification;
	for (auto change : changes) {
		result.second[change.first] = std::make_pair(change.second,
				attributing.at(change.first));
		logging += "\n\t\t" + change.first + ": " + change.second + "->"
				+ attributing.at(change.first) + ",";
	}
	if (logging.back() == ',') {
		logging.pop_back();
		logging += "\n\t";
	} else
		logging += " ";
	logging += "}\n}";
	log.returned(logging);

	return result;
}
bool Object::operator ==(const Object& than) const {
	auto result = attributing == than.attributing;

	binary<bool>(nullptr, result, "==", "base", &than, "");

	return result;
}
bool Object::operator !=(const Object& than) const {
	auto result = attributing != than.attributing;

	binary<bool>(nullptr, result, "!=", "base", &than, "");

	return result;
}
std::set<Object*>& Object::all() {
	function<std::type_index>("", nullptr, typeid(everything), "base",
			typeid(Object), "all").returned(
			lister(everything, ";", true, true));

	return everything;
}
std::set<Object*> Object::root() {
	std::set<Object*> result;
	auto log = function<std::type_index>("", nullptr, typeid(result), "base",
			typeid(Object), "root");

	for (auto object : everything)
		if (!object->position)
			result.insert(object);
	log.returned(lister(result, ";", true, true));

	return result;
}
Object::Object(Object* position, std::map<std::string, std::string> attributes,
		const Log* caller) :
		Log(caller, "", false, "base", "position", position, "attributes",
				lister(attributes,
						[](std::pair<const std::string, std::string> attribute) {
							return attribute.first + "=" + attribute.second;
						}, ",", true, true).c_str()) {
	modification = creation = std::chrono::system_clock::to_time_t(
			std::chrono::system_clock::now());
	this->position = position;
	attributing = attributes;
	everything.emplace(this);
}
Object::~Object() {
	std::ostringstream log;

	if (run)
		everything.erase(this);
	log << "base::" << typeid(Object).name() << "{" << this
			<< "} is being destroyed";
	message(log.str());
}

//Location
std::string Location::naming(std::string name) {
	Log log(
			method<std::type_index>(nullptr, "naming", typeid(std::string), "",
					"name", name));
	std::string candidate = name;

	if (candidate.empty())
		candidate = name = "content";
	for (auto suffix = 0; operator ()(candidate).size(); ++suffix)
		candidate = name + "_" + std::to_string(suffix);
	log.returned(candidate);

	return candidate;
}
Location::container::iterator Location::locate(size_t offset) const {
	auto result = const_cast<container&>(containing).begin();
	Log log(
			method<std::type_index>(nullptr, "locate", typeid(result), "",
					"offset", offset));
	auto end = containing.end();
	size_t index = 1;
	std::ostringstream message;

	if (offset)
		while (result != end && ++index < offset)
			++result;
	else
		result = end;
	if (result == end) {
		message << "WARNING invalid argument: offset > size="
				<< containing.size() << ".";
		log.message(message.str());
		std::cerr << log.log() << " " << message.str() << std::endl;
	} else {
		message << typeid(Location).name() << "::container->"
				<< which(*result->second) << "[" << index << "]";
		log.returned(message.str().c_str());
	}

	return result;
}
std::map<size_t, Location::container::iterator> Location::locate(
		std::string name) const {
	std::map<size_t, Location::container::iterator> result;
	Log log(
			method<std::type_index>(nullptr, "locate", typeid(result), "",
					"name", name));
	auto current = std::make_pair(1,
			const_cast<container&>(containing).begin());
	std::ostringstream message;

	for (auto end = containing.end(); current.second != end; ++current.first) {
		if (current.second->first == name)
			result.emplace(current);
		++current.second;
	}
	if (result.empty()) {
		message << "WARNING no Object exists here with that name.";
		log.message(message.str());
		std::cerr << log.log() << " " << message.str() << std::endl;
	} else
		log.returned(lister(result, [&message](decltype(current) found) {
			message.clear();
			message << "[" << found.first << "]" << found.second->second.get();

			return message.str();
		}));

	return result;
}
std::map<size_t, Location::container::iterator> Location::locate(
		std::type_index type) const {
	std::map<size_t, Location::container::iterator> result;
	auto typing = &type;
	Log log(
			method<std::type_index>(nullptr, "locate", typeid(result), "",
					"type", typing));
	auto current = std::make_pair(1,
			const_cast<container&>(containing).begin());
	std::ostringstream message;

	for (auto end = containing.end(); current.second != end; ++current.first) {
		if (type == typeid(*current.second->second))
			result.emplace(current);
		++current.second;
	}
	if (result.empty()) {
		message
				<< "WARNING invalid argument: the instance is not located here.";
		log.message(message.str());
		std::cerr << log.log() << " " << message.str() << std::endl;
	} else
		log.returned(lister(result, [&message](decltype(current) found) {
			message.clear();
			message << "[" << found.first << "]" << found.second->second.get();

			return message.str();
		}));

	return result;
}
std::pair<size_t, Location::container::iterator> Location::locate(
		const Object& instance) const {
	auto result = std::make_pair(1, const_cast<container&>(containing).begin());
	Log log(
			method<std::type_index>(nullptr, "locate", typeid(result), "",
					"instance", &instance));
	auto end = containing.end();
	std::ostringstream message;

	while (result.second != end && result.second->second.get() != &instance) {
		++result.first;
		++result.second;
	}
	if (result.second == end) {
		message
				<< "WARNING invalid argument: the instance is not located here.";
		log.message(message.str());
		std::cerr << log.log() << " " << message.str() << std::endl;
	} else {
		message << "[" << result.first << "]" << result.second->second.get();
		log.returned(message.str().c_str());
	}

	return result;
}
std::unique_ptr<Object> Location::extract(container::iterator iterator,
		const Log* caller) {
	std::unique_ptr<Object> result;
	Log log(
			method<std::type_index>(caller, "extract", typeid(result), "",
					"iterator",
					std::string(typeid(Location).name()) + "::container->"
							+ std::to_string(Object::who()) + "["
							+ std::to_string(which(*iterator->second)) + "]"));

	if (iterator == containing.end()) {
		std::string message =
				"ERROR invalid argument: iterator is out of range.";

		log.message(message);
		std::cerr << log.log() << " " << message << std::endl;

		throw std::invalid_argument(message);
	}
	iterator->second.swap(result);
	result->position = nullptr;
	containing.erase(iterator);
	modification = result->modification = std::chrono::system_clock::to_time_t(
			std::chrono::system_clock::now());

	return std::move(result);
}
void Location::remove(container::const_iterator iterator, const Log* caller) {
	Log log(
			method<std::type_index>(caller, "remove", typeid(void), "",
					"iterator",
					std::string(typeid(Location).name()) + "::container->"
							+ std::to_string(Object::who()) + "["
							+ std::to_string(which(*iterator->second))
							+ "]const"));

	if (iterator == containing.end()) {
		std::string message =
				"WARNING invalid argument: iterator is out of range.";

		log.message(message);
		std::cerr << log.log() << " " << message << std::endl;

		throw std::invalid_argument(message);
	} else {
		containing.erase(iterator);
		modification = std::chrono::system_clock::to_time_t(
				std::chrono::system_clock::now());
	}
}
Object* Location::operator [](size_t offset) const {
	Object* result = nullptr;
	Log log(
			binary<std::type_index>(nullptr, typeid(result), "[]", "", offset,
					""));
	auto iterator = locate(offset);

	if (iterator != containing.end())
		result = iterator->second.get();
	log.returned(result);

	return result;
}
std::map<size_t, Object*> Location::operator ()(std::string name) const {
	std::map<size_t, Object*> result;
	Log log(
			Log::method<std::type_index>(nullptr, "", typeid(result), "",
					"name", name));
	auto located = locate(name);

	for (auto found : located)
		result.emplace(found.first, found.second->second.get());
	log.returned(lister(result, [](std::pair<size_t, Object*> content) {
		std::ostringstream text;

		text << "[" << content.first << "]" << content.second;

		return text.str();
	}, ",", true, true));

	return result;
}
std::map<size_t, Object*> Location::operator ()(std::type_index type) const {
	std::map<size_t, Object*> result;
	auto typing = &type;
	Log log(
			Log::method<std::type_index>(nullptr, "", typeid(result), "",
					"type", typing));
	auto located = locate(type);

	for (auto found : located)
		result.emplace(found.first, found.second->second.get());
	log.returned(lister(result, [](std::pair<size_t, Object*> content) {
		std::ostringstream text;

		text << "[" << content.first << "]" << content.second;

		return text.str();
	}, ",", true, true));

	return result;
}
void Location::insert_front(std::string name,
		std::unique_ptr<Object>&& instance, const Log* caller) {
	insert(1, name, std::move(instance),
			&method<void>(caller, "insert_front", typeid(void), "", "name",
					name, "instance", instance.get()));
}
void Location::insert(size_t offset, std::string name,
		std::unique_ptr<Object>&& instance, const Log* caller) {
	Log log(
			method<void>(caller, "insert", typeid(void), "", "offset", offset,
					"name", name, "instance", instance.get()));
	std::ostringstream message;

	if (instance.get()) {
		if (offset > containing.size() + 1)
			message << "WARNING invalid argument: offset > 1 + size="
					<< containing.size() << ".";
		else if (offset) {
			auto result = locate(offset);

			result = containing.emplace(result, naming(name), nullptr,
					std::map<std::string, std::string>(), &log);
			result->second.swap(instance);
			result->second->position = this;
			modification = result->second->modification =
					std::chrono::system_clock::to_time_t(
							std::chrono::system_clock::now());
		} else
			message << "WARNING invalid argument: offset > 1 + size="
					<< containing.size() << ".";
	} else
		message = "ERROR invalid argument: instance cannot be undefined.";
	if (message.str().length()) {
		log.message(message.str());
		std::cerr << log.log() << " " << message.str() << std::endl;

		throw std::invalid_argument(message.str());
	}
}
void Location::insert_back(std::string name, std::unique_ptr<Object>&& instance,
		const Log* caller) {
	insert(++containing.size(), name, std::move(instance),
			&method<void>(caller, "insert_back", typeid(void), "", "name", name,
					"instance", instance.get()));
}
bool Location::remove(size_t offset, Log track) {
	container::iterator iterator;
	auto result = true;

	std::clog << track.tracker() << "bool base::Location{" << this
			<< "}.remove(size_t offset=" << offset << ") {" << std::endl;
	std::clog << track() << "}=" << (result = remove(locate(offset), track))
			<< std::endl;

	return result;
}
bool Location::remove(std::string name, Log track) {
	container::iterator iterator;
	auto result = false;

	std::clog << track.tracker() << "bool base::Location{" << this
			<< "}.remove(std::string name=\"" << name << "\") {" << std::endl;
	std::clog << track() << "}=" << (result = remove(locate(name), track))
			<< std::endl;

	return result;
}
bool Location::remove(const Object& instance, Log track) {
	auto result = false;

	std::clog << track.tracker() << "bool base::Location{" << this
			<< "}.remove(const base::Object* instance=" << &instance << ") {"
			<< std::endl;
	std::clog << track() << "}=" << (result = remove(locate(instance), track))
			<< std::endl;

	return result;
}/*
 std::unique_ptr<Object> Location::extract(size_t offset, Log track) {
 std::unique_ptr<Object> result;

 std::clog << track.tracker()
 << "std::unique_ptr<base::Object> base::Location{" << this
 << "}.extract(size_t offset=" << offset << ") {" << std::endl;
 extract(locate(offset), track).swap(result);
 std::clog << track() << "}=" << result.get() << std::endl;

 return std::move(result);
 }
 std::unique_ptr<Object> Location::extract(std::string name, Log track) {
 std::unique_ptr<Object> result;

 std::clog << track.tracker()
 << "std::unique_ptr<base::Object> base::Location{" << this
 << "}.extract(std::string name=\"" << name << "\") {" << std::endl;
 extract(locate(name), track).swap(result);
 std::clog << track() << "}=" << result.get() << std::endl;

 return std::move(result);
 }
 std::unique_ptr<Object> Location::extract(const Object& instance, Log track) {
 std::unique_ptr<Object> result;

 std::clog << track.tracker()
 << "std::unique_ptr<base::Object> base::Location{" << this
 << "}.extract(const base::Object& instance=" << &instance << ") {"
 << std::endl;
 extract(locate(instance), track).swap(result);
 std::clog << track() << "}=" << result.get() << std::endl;

 return std::move(result);
 }
 size_t Location::size() const {
 auto result = contained.size();

 std::clog << Log().tracker() << "size_t base::Location{" << this
 << "}.size()= " << result << std::endl;

 return result;
 }
 size_t Location::which(const Object& instance) const {
 auto iterator = const_cast<container&>(contained).begin();
 auto end = contained.end();
 size_t result = 0;
 std::ostringstream log;
 std::string message;
 Log track;

 log << track.tracker() << "size_t base::Location{" << this
 << "}.which(const base::Object& instance=" << &instance << ") {"
 << std::endl;
 std::clog << log.str() << " {" << std::endl;
 while (iterator != end && iterator->second.get() != &instance) {
 ++result;
 ++iterator;
 }
 if (iterator == end) {
 message = " WARNING invalid argument: instance is not located here.";
 std::cerr << log.str() << "=" << result << message << std::endl;
 }
 std::clog << track() << "}=" << result << message << std::endl;

 return result;
 }

 std::string Location::who(const Object& instance) {
 std::string result;
 container::iterator iterator;
 Location* place = nullptr;
 std::ostringstream log;
 Log track;

 log << track.tracker()
 << "std::string base::Location::who(const base::Object& instance="
 << &instance << ")";
 std::clog << log.str() << " {" << std::endl;
 if (instance.position) {
 place = dynamic_cast<Location*>(instance.position);
 if ((iterator = place->locate(instance)) == place->contained.end()) {
 std::string message =
 "= ERROR instance results located where correspondent object not exists.";

 std::clog << track() << "}" << message;
 std::cerr << log.str() << message;
 throw std::runtime_error(log.str() + message);
 } else
 result = iterator->first;
 }
 std::clog << track() << "}=\"" << result << "\"" << std::endl;

 return result;
 }
 std::list<const Object*> Location::path(const Object& instance) {
 std::list<const Object*> result;
 Log track;

 std::clog << track.tracker()
 << "std::vector< const base::Object* > base::Location::path("
 "const base::Object& instance=" << &instance << ") {"
 << std::endl;
 if (instance.position)
 result = path(*instance.position);
 result.push_back(&instance);
 std::clog << track() << "}=+" << &instance << std::endl;

 return result;
 }

 Location::Location(Location* location, Log track) :
 Object(location, track) {
 std::clog << track.tracker()
 << "base::Location::Location(base::Location* location=" << location
 << ")=" << this << std::endl;
 }

 std::map<std::string, std::string> datafile(std::string key,
 std::string filepath, base::Log track) {
 std::map<std::string, std::string> result;
 std::ifstream file(filepath);
 std::string line;
 std::vector<std::string> nested;
 std::string string;
 std::ostringstream log;
 std::string message;

 log << track.tracker() << "std::map< std::string, std::string > "
 "datafile(std::string key=\"" << key
 << "\", std::string filepath=\"" << filepath << "\")";
 std::clog << log.str() << " {" << std::endl;
 while (line != "[" + key + "]" && file.good())
 std::getline(file, line);
 while (!line.empty() && file.good()) {
 size_t equal = 0;

 std::getline(file, line);
 if (!line.empty()) {
 if (line == "}" && nested.size()) {
 nested.pop_back();
 string.clear();
 } else if (line.front() == '"' && !string.empty()) {
 line.erase(0, 1);
 if (line.back() == '"')
 line.pop_back();
 string += "\n" + line;
 } else if ((equal = line.find("=")) != line.npos) {
 std::string value = line.substr(equal + 1);

 string.clear();
 if (value == "{")
 nested.push_back(line.substr(0, equal));
 else if (value.front() == '"') {
 string = value.substr(1);
 if (string.back() == '"')
 string.pop_back();
 } else {
 std::string key;

 if (value == "#")
 value.clear();
 for (auto nest : nested)
 key += nest + "/";
 result[key + line.substr(0, equal)] = value;
 }
 } else {
 message = "= ERROR invalid line: \"" + line + "\"";
 std::clog << track() << "}" << message << std::endl;
 std::cerr << log.str() << message << std::endl;
 throw std::domain_error(log.str() + message);
 }
 }
 }
 file.close();
 std::clog << track() << "}=";
 for (auto element : result)
 message += ",\n" + element.first + "=" + element.second;
 if (message.length()) {
 message.erase(0, 1);
 message += "\n";
 } else
 message = " ";
 std::clog << "{" << message << "}" << std::endl;

 return result;
 }
 }

 namespace game {
 //Card
 base::Object* Card::enter(std::string content, size_t number,
 unsigned derived) const {
 return Location::enter(content, number, derived);
 }
 std::string Card::field(std::string variable, unsigned derived) const {
 if (variable == "covered")
 return covered ? "true" : "false";
 else
 return Location::field(variable, derived);
 }
 void Card::field(std::string variable, size_t position, std::string value,
 unsigned derived) {
 if (variable == "covered")
 covered = value == "true";
 else
 Location::field(variable, position, value, derived);
 modification = std::chrono::system_clock::to_time_t(
 std::chrono::system_clock::now());
 }

 base::Location* Card::operator ()(bool cover) const {
 Location* result = nullptr;
 base::Log track;
 std::ostringstream log;

 log << track.tracker() << "base::Location& game::Card{" << this << "}(bool "
 << cover << ")";
 std::clog << log.str() << " {" << std::endl;
 if ((result =
 cover ? dynamic_cast<Location*>(Location::operator [](0)) : dynamic_cast<Location*>(Location::operator [](
 1)))) {
 std::clog << track() << "}=" << result << std::endl;

 return result;
 } else {
 std::string message = "= ERROR a face is not instanced.";

 std::clog << track() << "}" << message << std::endl;
 std::cerr << log.str() << message << std::endl;
 throw std::length_error(log.str() + message);
 }
 }
 bool Card::facing() const {
 std::clog << base::Log().tracker() << "bool game::Card{" << this
 << "}.facing()=" << !covered << std::endl;

 return !covered;
 }
 base::Location& Card::operator ()() const {
 Location* result = nullptr;
 base::Log track;

 std::clog << track.tracker() << "base::Location& game::Card{" << this
 << "}() {" << std::endl;
 std::clog << track() << "}=" << (result = operator ()(covered))
 << std::endl;

 return *result;
 }
 bool Card::flip(base::Log track) {
 modification = std::chrono::system_clock::to_time_t(
 std::chrono::system_clock::now());
 std::clog << track.tracker() << "bool game::Card{" << this << "}.flip()="
 << (covered = !covered) << std::endl;

 return covered;
 }
 void Card::facing(bool face, base::Log track) {
 covered = !face;
 modification = std::chrono::system_clock::to_time_t(
 std::chrono::system_clock::now());
 std::clog << track.tracker() << "void game::Card{" << this
 << "}.facing(bool face=" << face << ")" << std::endl;
 }

 Card::Card(std::unique_ptr<Location>&& cover, std::unique_ptr<Location>&& face,
 Location* position, base::Log track) :
 Location(position, track) {
 std::clog << track.tracker()
 << "game::Card::Card(std::unique_ptr<base::Location>&& cover="
 << cover.get() << ", std::unique_ptr<base::Location>&& face="
 << face.get() << ", base::Location* position=" << position << ") {"
 << std::endl;
 covered = false;
 Location::insert(0, "cover", std::move(cover), track);
 Location::insert(1, "face", std::move(face), track);
 std::clog << track() << "}=" << this << std::endl;
 }

 //Deck
 base::Object* Deck::enter(std::string content, size_t number,
 unsigned derived) const {
 return Location::enter(content, number, derived);
 }
 std::string Deck::field(std::string variable, unsigned derived) const {
 if (variable == "name")
 return name;
 else
 return Location::field(variable, derived);
 }
 void Deck::field(std::string variable, size_t position, std::string value,
 unsigned derived) {
 if (variable == "name")
 name = value;
 else
 Location::field(variable, position, value, derived);
 modification = std::chrono::system_clock::to_time_t(
 std::chrono::system_clock::now());
 }
 std::string Deck::what() const {
 std::clog << base::Log().tracker() << "std::string game::Deck{" << this
 << "}.what()=" << typeid(*this).name() << std::endl;

 return typeid(*this).name();
 }

 const std::string& Deck::label() const {
 std::clog << base::Log().tracker() << "game::Deck{" << this
 << "}.label()=\"" << name << "\"" << std::endl;

 return name;
 }
 std::unique_ptr<Card> Deck::draw(base::Log track) {
 std::unique_ptr < Card > result;

 std::clog << track.tracker()
 << "std::unique_ptr<game::Card> game::Deck{" << this
 << "}.draw() {" << std::endl;
 result.reset(
 dynamic_cast<Card*>(Location::extract((size_t) 0, track).release()));
 std::clog << track() << "}=" << result.get() << std::endl;

 return std::move(result);
 }
 std::unique_ptr<Card> Deck::extract(base::Log track) {
 std::unique_ptr < Card > result;
 std::default_random_engine generator;
 size_t random = 0;

 std::clog << track.tracker()
 << "std::unique_ptr<game::Card> game::Deck{" << this
 << "}.extract() {" << std::endl;
 std::clog << track() << "random="
 << (random = std::uniform_int_distribution < size_t
 > (0, Location::size() - 1)(generator))
 << std::endl;
 result.reset(
 dynamic_cast<Card*>(Location::extract(random, track).release()));
 std::clog << track() << "}=" << result.get() << std::endl;

 return std::move(result);
 }
 std::unique_ptr<Card> Deck::get_bottom(base::Log track) {
 std::unique_ptr < Card > result;

 std::clog << track.tracker()
 << "std::unique_ptr<game::Card> game::Deck{" << this
 << "}.get_bottom() {" << std::endl;
 result.reset(
 dynamic_cast<Card*>(Location::extract(Location::size() - 1,
 track).release()));
 std::clog << track() << "}=" << result.get() << std::endl;

 return std::move(result);
 }
 void Deck::put_up(std::string name, std::unique_ptr<Card>&& card,
 base::Log track) {
 std::clog << track.tracker() << "void game::Deck{" << this
 << "}.put_up(std::string name=\"" << name
 << "\", std::unique_ptr< game::Card >&& card=" << card.get()
 << ") {" << std::endl;
 Location::insert(0, name,
 std::unique_ptr < Object > (card.release()), track);
 std::clog << track() << "}" << std::endl;
 }
 void Deck::insert(std::string name, std::unique_ptr<Card>&& card,
 base::Log track) {
 std::default_random_engine generator;
 size_t random = 0;

 std::clog << track.tracker() << "void game::Deck{" << this
 << "}.insert(std::string name=\"" << name
 << "\", std::unique_ptr< game::Card >&& card=" << card.get()
 << ") {" << std::endl;
 std::clog << track() << "random="
 << (random = std::uniform_int_distribution < size_t
 > (0, Location::size())(generator)) << std::endl;
 Location::insert(random, name,
 std::unique_ptr < Object > (card.release()), track);
 std::clog << track() << "}" << std::endl;
 }
 void Deck::put_down(std::string name, std::unique_ptr<Card>&& card,
 base::Log track) {
 std::clog << track.tracker() << "void game::Deck{" << this
 << "}.put_down(std::string name=\"" << name
 << "\", std::unique_ptr< game::Card >&& card=" << card.get()
 << ") {" << std::endl;
 Location::insert_back(name,
 std::unique_ptr < Object > (card.release()), track);
 std::clog << track() << "}" << std::endl;
 }
 void Deck::shuffle(base::Log track) {
 std::default_random_engine generator;
 size_t current = 0;
 size_t length = 0;
 std::uniform_int_distribution < size_t > distribution;

 std::clog << track.tracker() << "void game::Deck{" << this
 << "}.shuffle() {" << std::endl;
 length = Location::size();
 distribution.param(
 std::uniform_int_distribution < size_t
 > (current, length ? length - 1 : current).param());
 for (std::string card; length > 0; --length) {
 std::clog << track() << "current="
 << (current = distribution(generator)) << std::endl;
 card = who(*Location::operator [](current));
 Location::insert(0, card, Location::extract(current, track),
 track);
 }
 std::clog << track() << "}" << std::endl;
 }

 Deck* Deck::construct(std::string label, base::Log track) {
 Deck* result = nullptr;

 std::clog << track.tracker()
 << "game::Deck* game::Deck::construct(std::string label=\""
 << label << "\") {" << std::endl;
 result = new Deck(label, nullptr, track);
 std::clog << track() << "}=" << result << std::endl;

 return result;
 }

 Deck::Deck(std::string label, Location* position, base::Log track) :
 Location(position, track) {
 name = label;
 std::clog << track.tracker()
 << "game::Deck::Deck(std::string label=\"" << label
 << "\", base::Location* position=" << position << ")="
 << this << std::endl;
 }
 */
}
