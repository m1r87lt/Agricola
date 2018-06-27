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

long long unsigned Object::who() const {
	method<long long unsigned>("", nullptr, "who", track);

	return track;
}
Object* Object::where() const {
	method<Object*>("", nullptr, "where", position);

	return position;
}
time_t Object::when() const {
	method<time_t>("", nullptr, "when", creation);

	return creation;
}
std::map<std::string, std::string> Object::attributes() const {
	method<void>("", nullptr, "attributes", attributing);

	return attributing;
}
void Object::attribute(std::map<std::string, std::string> attributing,
		const Log* caller) {
	method<std::type_index>(nullptr, "attribute", typeid(void), "",
			"attributing",
			lister(attributing,
					[](std::pair<std::string, std::string> content) {
						return content.first + "=" + content.second;
					}, ",", true, true));
}
std::pair<time_t, std::map<std::string, std::pair<std::string, std::string>>>Object::what() {
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
	auto end = const_cast<container&>(containing).end();
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
		if (current.second->first.substr(0,
				current.second->first.find_last_of("_")) == name)
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
	Log log(
			method<std::type_index>(nullptr, "locate", typeid(result), "",
					"type", &type));
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
	Log log(
			Log::method<std::type_index>(nullptr, "", typeid(result), "",
					"type", &type));
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
	Log log(
			method<void>(caller, "insert_front", typeid(void), "", "name", name,
					"instance", instance.get()));

	insert(1, name, std::move(instance), &log);
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

			result = containing.emplace(result, naming(name), nullptr);
			result->second.swap(instance);
			result->second->position = this;
			modification = result->second->modification =
					std::chrono::system_clock::to_time_t(
							std::chrono::system_clock::now());
		} else
			message << "WARNING invalid argument: offset > 1 + size="
					<< containing.size() << ".";
	} else
		message << "ERROR invalid argument: instance cannot be undefined.";
	if (message.str().length()) {
		log.message(message.str());
		std::cerr << log.log() << " " << message.str() << std::endl;

		throw std::invalid_argument(message.str());
	}
}
void Location::insert_back(std::string name, std::unique_ptr<Object>&& instance,
		const Log* caller) {
	Log log(
			method<void>(caller, "insert_back", typeid(void), "", "name", name,
					"instance", instance.get()));

	insert(containing.size() + 1, name, std::move(instance), &log);
}
void Location::remove(size_t offset, const Log* caller) {
	Log log(
			method<std::type_index>(caller, "remove", typeid(void), "",
					"offset", offset));

	remove(locate(offset), &log);
}
void Location::remove(std::string name, const Log* caller) {
	Log log(
			method<std::type_index>(caller, "remove", typeid(void), "", "name",
					name));
	auto located = locate(name);

	for (auto found : located)
		remove(found.second, &log);
}
void Location::remove(std::type_index type, const Log* caller) {
	Log log(
			method<std::type_index>(caller, "remove", typeid(void), "", "type",
					&type));

	auto located = locate(type);

	for (auto found : located)
		remove(found.second, &log);
}
void Location::remove(const Object& instance, const Log* caller) {
	Log log(
			method<std::type_index>(caller, "remove", typeid(void), "",
					"instance", &instance));

	remove(locate(instance).second, &log);
}
std::unique_ptr<Object> Location::extract(size_t offset, const Log* caller) {
	std::unique_ptr<Object> result;
	Log log(
			method<std::type_index>(caller, "extract", typeid(result), "",
					"offset", offset));

	extract(locate(offset), &log).swap(result);
	log.returned(result.get());

	return std::move(result);
}
std::unique_ptr<Object> Location::extract(const Object& instance,
		const Log* caller) {
	std::unique_ptr<Object> result;
	Log log(
			method<std::type_index>(caller, "extract", typeid(result), "",
					"instance", &instance));

	extract(locate(instance).second, &log).swap(result);
	log.returned(result.get());

	return std::move(result);
}
size_t Location::size() const {
	method<size_t>("", nullptr, "size", containing.size());

	return containing.size();
}
size_t Location::which(const Object& instance) {
	size_t result = 0;
	Log log(
			function<std::type_index>(nullptr, "", typeid(result), "base",
					typeid(Location), "which", "instance", &instance));
	if (instance.position)
		result =
				dynamic_cast<Location*>(instance.position)->locate(instance).first;
	log.returned(result);

	return result;
}
std::string Location::who(const Object& instance) {
	std::string result;
	Log log(
			function<std::type_index>(nullptr, "", typeid(result), "base",
					typeid(Location), "who", "instance", &instance));

	if (instance.position)
		result =
				dynamic_cast<Location*>(instance.position)->locate(instance).second->first;
	log.returned(result);

	return result;
}
std::vector<const Object*> Location::path(const Object& instance) {
	std::vector<const Object*> result;
	Log log(
			function<std::type_index>(nullptr, "", typeid(result), "base",
					typeid(Location), "path", "instance", &instance));

	if (instance.position)
		result = path(*instance.position);
	result.push_back(&instance);
	log.returned(lister(result));

	return result;
}

Location::Location(Location* position,
		std::map<std::string, std::string> attributing, const Log* caller) :
		Object(position, attributing, caller) {
}
}
namespace game {
//Card
base::Object& Card::side(bool covered) const {
	Object* result = nullptr;
	Log log(
			method<std::type_index>(nullptr, "side", typeid(result), "",
					"covered", covered));

	if ((result = operator [](covered ? 0 : 1))) {
		log.returned(result);

		return *result;
	} else {
		std::string message = "ERROR the "
				+ std::string(covered ? "cover" : "face")
				+ " is not instanced.";

		log.message(message);
		std::cerr << log.log() << " " << message << std::endl;

		throw std::length_error(message);
	}
}
long long unsigned Card::who() const {
	Log log(method<std::type_index>(nullptr, "who", typeid(long long unsigned)))
}
	Location* where() const;
	time_t when() const;
	void attribute(std::map<std::string, std::string>, const Log*);
	std::map<std::string, std::string> attributes();
	std::pair<time_t,
			std::map<std::string, std::pair<std::string, std::string>>>what();
	bool operator ==(const Card&) const;
	bool operator !=(const Card&) const;
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
/*
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
