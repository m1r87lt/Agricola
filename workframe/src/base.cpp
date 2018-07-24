/*
 * base.cpp
 *
 *  Created on: 15 dic 2017
 *      Author: m1r
 */

#include "base.h"
#include <chrono>
#include <type_traits>
#include <stdexcept>
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

std::string Log::tracking() const {
	return legacy + std::to_string(track);
}
std::string Log::log() const {
	return tracking() + ": " + logging;
}
std::string Log::arguments() {
	return "";
}
std::string Log::tracking(const Log* caller) {
	if (caller)
		return caller->legacy + "." + std::to_string(caller->track) + ".";
	else
		return ".";
}
std::string Log::messaging(std::string message) {
	if (message.length())
		message = " \"" + message + "\"";

	return message;
}
void Log::message(std::string text) const {
	std::clog << tracking() + " " + messaging(text) << std::endl;
}
Variable<Log&> Log::variable() const {
	return Variable<Log&>("", ns, const_cast<Log&>(*this), transcoder);
}
void Log::error(std::string message) const {
	std::cerr << log() << messaging(message) << std::endl;
}
std::string Log::transcoder(Log& derived) {
	return std::to_string(derived.track);
}
std::string Log::parameters() {
	return "";
}
Variable<Log&> Log::variable(std::string name) const {
	return Variable<Log&>(name, ns, const_cast<Log&>(*this), transcoder);
}

Log::Log(const Log* caller, std::string ns, bool open) {
	legacy = tracking(caller);
	track = ++tracker;
	this->open = open;
	this->ns = ns;
}
Log::Log(const Log* caller, std::string ns, bool open, std::string message) {
	legacy = tracking(caller);
	track = ++tracker;
	this->ns = ns;
	logging = variable()() + "::" + typeid(*this).name() + "()";
	std::clog << log() << messaging(message)
			<< (((this->open = open)) ? " {" : "") << std::endl;
}
Log::~Log() {
	if (std::is_same<typename std::decay<decltype(*this)>::type, Log>::value
			&& open)
		std::clog << tracking() << "  }" << std::endl;
	else if (!open)
		std::clog << "~" << variable()() << std::endl;
}
Log::Log(Log& copy) {
	legacy = copy.legacy;
	track = copy.track;
	open = copy.open;
	ns = copy.ns;
	logging = copy.logging;
	copy.open = false;
}
Log::Log(Log&& moved) {
	legacy = moved.legacy;
	track = moved.track;
	open = moved.open;
	ns = moved.ns;
	logging = moved.logging;
	moved.open = false;
}

//Object
std::set<Object*> Object::everything;

long long unsigned Object::who() const {
	return method("", nullptr, base::variable(track, "who"));
}
Object* Object::where() const {
	return method("", nullptr, base::variable(position, "where"));
}
time_t Object::when() const {
	return method("", nullptr, base::variable(creation, "when"));
}
std::map<std::string, std::string> Object::attributes() const {
	return method("", nullptr,
			Variable<const decltype(attributing)&>(lister, "attributes",
					attributing));
}
void Object::attribute(std::map<std::string, std::string> attributing,
		const Log* caller) {
	auto log = method<void>(caller, "attribute", "",
			Variable<decltype(attributing)&>(lister, "attributing",
					attributing));
	for (auto field : attributing) {
		auto substituting = this->attributing.find(field.first);

		if (substituting == this->attributing.end())
			this->attributing.insert(field);
		else {
			changes[substituting->first] = substituting->second;
			substituting->second = field.second;
		}
	}
	modification = std::chrono::system_clock::to_time_t(
			std::chrono::system_clock::now());
}
Object::modifications Object::what() {
	modifications result;
	auto log = method<decltype(result)>(nullptr, "what", "");

	result.first = modification;
	for (auto change : changes)
		result.second[change.first] = std::make_pair(change.second,
				attributing.at(change.first));

	return log.returning<decltype(result)&>(result, changing);
}
bool Object::operator ==(const Object& than) const {
	return binary(nullptr,
			base::variable(attributing == than.attributing, "=="),
			than.variable(), "");
}
bool Object::operator !=(const Object& than) const {
	return binary(nullptr,
			base::variable(attributing != than.attributing, "=="),
			than.variable(), "");
}
std::set<Object*>& Object::all() {
	return function("", nullptr,
			Variable<decltype(everything)&>(objects, "all", everything), "base",
			typeid(Object));
}
std::set<Object*> Object::root() {
	std::set<Object*> result;
	auto log = function<decltype(result)>(nullptr, "base", typeid(Object),
			"root", "");
	std::set<Object*>& e = everything;

	for (auto object : e)
		if (!object->position)
			result.insert(object);
	return log.returning<decltype(result)&>(result, objects);
}
std::string Object::lister(const std::map<std::string, std::string>& mapping) {
	std::string result = "{";

	for (auto mapped : mapping)
		result += "\n\t" + mapped.first + "=" + mapped.second + ",";
	if (result.back() == ',')
		result.back() = '\n';
	else
		result += " ";

	return result + "}";
}
std::string Object::changing(modifications& mods) {
	std::string result = "{ ";

	result += ctime(&mods.first);
	result += ":";
	for (auto mod : mods.second)
		result += "\n\t" + mod.first + "=" + mod.second.first + "->"
				+ mod.second.second + ",";
	if (result.back() == ',')
		result.pop_back();

	return result + "\n}";
}
std::string Object::objects(std::set<Object*>& set) {
	std::stringstream result("{");

	for (auto object : set)
		result << " " << object << ";";
	result.seekg(0, std::ios::end);
	result.unget();
	if (result.peek() == ';')
		result.get();

	return result.str() + " }";
}

Object::Object(Object* position, std::map<std::string, std::string> attributes,
		const Log* caller, std::string ns, bool open, std::string message) :
		Log(caller, ns, open, message, base::variable(position, "position"),
				Variable<decltype(attributes)&>(lister, "attributes",
						attributes)) {
	modification = creation = std::chrono::system_clock::to_time_t(
			std::chrono::system_clock::now());
	this->position = position;
	attributing = attributes;
	everything.emplace(this);
}

//Location
std::string Location::naming(std::string name) {
	std::string candidate = name;
	auto log = method<decltype(candidate)>(nullptr, "naming", "",
			base::variable(name, "name"));

	if (candidate.empty())
		candidate = name = "content";
	for (auto suffix = 0; operator ()(candidate).size(); ++suffix)
		candidate = name + "_" + std::to_string(suffix);

	return log.returning(candidate);
}
Location::container::iterator Location::locate(size_t offset) const {
	auto result = const_cast<container&>(containing).begin();
	auto log = method<decltype(result)>(nullptr, "locate", "",
			base::variable(offset, "offset"));
	auto end = const_cast<container&>(containing).end();
	size_t index = 1;

	if (offset)
		while (result != end && ++index < offset)
			++result;
	else
		result = end;
	if (result == end) {
		std::ostringstream message;

		message << "WARNING invalid argument: offset > size="
				<< containing.size() << ".";
		log.error(message.str());
	}

	return log.returning<decltype(result)&>(result, iterate);
}
std::map<size_t, Location::container::iterator> Location::locate(
		std::string name) const {
	std::map<size_t, Location::container::iterator> result;
	Log log(
			method<decltype(result)>(nullptr, "locate", "",
					base::variable(name, "name")));
	auto current = std::make_pair(1,
			const_cast<container&>(containing).begin());

	for (auto end = containing.end(); current.second != end; ++current.first) {
		if (current.second->first.substr(0,
				current.second->first.find_last_of("_")) == name)
			result.emplace(current);
		++current.second;
	}
	if (result.empty()) {
		std::ostringstream message;

		message << "WARNING no Object exists here with that name.";
		log.message(message.str());
		log.error(message.str());
	}

	return log.returning<decltype(result)&>(result, mapper);
}
std::map<size_t, Location::container::iterator> Location::locate(
		std::type_index type) const {
	std::map<size_t, Location::container::iterator> result;
	auto log = method<decltype(result)>(nullptr, "locate", "",
			Variable<decltype(type)&>(typer, "type", type));
	auto current = std::make_pair(1,
			const_cast<container&>(containing).begin());

	for (auto end = containing.end(); current.second != end; ++current.first) {
		if (type == typeid(*current.second->second))
			result.emplace(current);
		++current.second;
	}
	if (result.empty()) {
		std::ostringstream message;

		message
				<< "WARNING invalid argument: the instance is not located here.";
		log.message(message.str());
		log.error(message.str());
	}

	return log.returning<decltype(result)&>(result, mapper);
}
std::pair<size_t, Location::container::iterator> Location::locate(
		const Object& instance) const {
	auto result = std::make_pair(1, const_cast<container&>(containing).begin());
	auto log = method<decltype(result)>(nullptr, "locate", "",
			instance.variable("instance"));
	auto end = containing.end();

	while (result.second != end && result.second->second.get() != &instance) {
		++result.first;
		++result.second;
	}
	if (result.second == end) {
		std::ostringstream message;

		message
				<< "WARNING invalid argument: the instance is not located here.";
		log.message(message.str());
		log.error(message.str());
	}

	return log.returning<std::pair<size_t, Location::container::iterator>>(
			result, locater);
}
std::unique_ptr<Object> Location::extract(container::iterator iterator,
		const Log* caller) {
	std::unique_ptr<Object> result;
	auto log = method<decltype(result)>(caller, "extract", "",
			Variable<container::iterator&>(iterate, "iterator", iterator));

	if (iterator == containing.end()) {
		std::string message =
				"ERROR invalid argument: iterator is out of range.";

		log.message(message);
		log.error(message);

		throw std::invalid_argument(message);
	}
	iterator->second.swap(result);
	result->position = nullptr;
	containing.erase(iterator);
	modification = result->modification = std::chrono::system_clock::to_time_t(
			std::chrono::system_clock::now());

	return unique_ptr(std::move(result));
}
void Location::remove(container::const_iterator iterator, const Log* caller) {
	auto log = method<void>(caller, "remove", "",
			Variable<container::const_iterator&>(
					[](container::const_iterator& iterator) {
						std::ostringstream result;

						result << iterator->second.get();

						return result.str();
					}, "iterator", iterator));

	if (iterator == containing.end()) {
		std::string message =
				"WARNING invalid argument: iterator is out of range.";

		log.message(message);
		log.error(message);

		throw std::invalid_argument(message);
	} else {
		containing.erase(iterator);
		modification = std::chrono::system_clock::to_time_t(
				std::chrono::system_clock::now());
	}
}
std::string Location::iterate(container::iterator& iterator) {
	std::ostringstream result;

	result << iterator->second.get();

	return result.str();
}
std::string Location::mapper(std::map<size_t, container::iterator>& map) {
	std::stringstream result("{");

	for (auto m : map)
		result << "\n\t[" << m.first << "]" << m.second->second.get() << ";";
	result.seekg(0, std::ios::end);
	result.unget();
	if (result.peek() == ';') {
		result.get();
		result.put('\n');
	} else
		result.put(' ');

	return result.str() + "}";
}
std::string Location::locater(std::pair<size_t, container::iterator>& pair) {
	std::ostringstream result("[");

	result << pair.first << "]" << pair.second->second.get();

	return result.str();
}
Object* Location::operator [](size_t offset) const {
	Object* result = nullptr;
	auto log = binary<decltype(result)>(nullptr, "[]",
			base::variable(offset, "offset"), "");
	auto iterator = locate(offset);

	if (iterator != containing.end())
		result = iterator->second.get();

	return log.returning(result);
}
std::map<size_t, Object*> Location::operator ()(std::string name) const {
	std::map<size_t, Object*> result;
	auto log = method<decltype(result)>(nullptr, "", "",
			base::variable(name, "name"));
	auto located = locate(name);

	for (auto found : located)
		result.emplace(found.first, found.second->second.get());

	return log.returning<decltype(result)&>(result, map_loc);
}
std::map<size_t, Object*> Location::operator ()(std::type_index type) const {
	std::map<size_t, Object*> result;
	auto log = method<decltype(result)>(nullptr, "", "",
			Variable<decltype(type)&>(typer, "type", type));
	auto located = locate(type);

	for (auto found : located)
		result.emplace(found.first, found.second->second.get());

	return log.returning<decltype(result)&>(result, map_loc);
}
void Location::insert_front(std::string name,
		std::unique_ptr<Object>&& instance, const Log* caller) {
	auto log = method<void>(caller, "insert_front", "",
			base::variable(name, "name"),
			base::variable(instance.get(), "instance"));

	insert(1, name, std::move(instance), &log);
}
void Location::insert(size_t offset, std::string name,
		std::unique_ptr<Object>&& instance, const Log* caller) {
	auto log = method<void>(caller, "insert", "",
			base::variable(offset, "offset"), base::variable(name, "name"),
			base::variable(instance.get(), "instance"));
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
		log.error(message.str());

		throw std::invalid_argument(message.str());
	}
}
void Location::insert_back(std::string name, std::unique_ptr<Object>&& instance,
		const Log* caller) {
	auto log = method<void>(caller, "insert_back", "",
			base::variable(name, "name"),
			base::variable(instance.get(), "instance"));

	insert(containing.size() + 1, name, std::move(instance), &log);
}
void Location::remove(size_t offset, const Log* caller) {
	auto log = method<void>(caller, "remove", "",
			base::variable(offset, "offset"));

	remove(locate(offset), &log);
}
void Location::remove(std::string name, const Log* caller) {
	auto log = method<void>(caller, "remove", "", base::variable(name, "name"));
	auto located = locate(name);

	for (auto found : located)
		remove(found.second, &log);
}
void Location::remove(std::type_index type, const Log* caller) {
	auto log = method<void>(caller, "remove", "",
			Variable<decltype(type)&>(typer, "type", type));

	auto located = locate(type);

	for (auto found : located)
		remove(found.second, &log);
}
void Location::remove(const Object& instance, const Log* caller) {
	auto log = method<void>(caller, "remove", "",
			instance.variable("instance"));

	remove(locate(instance).second, &log);
}
std::unique_ptr<Object> Location::extract(size_t offset, const Log* caller) {
	std::unique_ptr<Object> result;
	auto log = method<decltype(result)>(caller, "extract", "",
			base::variable(offset, "offset"));

	extract(locate(offset), &log).swap(result);

	return log.unique_ptr(std::move(result));
}
std::unique_ptr<Object> Location::extract(const Object& instance,
		const Log* caller) {
	std::unique_ptr<Object> result;
	auto log = method<decltype(result)>(caller, "extract", "",
			instance.variable("instance"));

	extract(locate(instance).second, &log).swap(result);
	return log.unique_ptr(std::move(result));
}
void Location::take(size_t source, Location& location, size_t destination,
		const Log* caller) {
	auto log = method<void>(caller, "take", "",
			base::variable(source, "source"), location.variable("location"),
			base::variable(destination, "destination"));
	auto moved = locate(source);

	location.insert(destination, moved->first, Location::extract(moved, &log),
			&log);
}
void Location::take(const Object& instance, Location& location, size_t position,
		const Log* caller) {
	auto log = method<void>(caller, "take", "", instance.variable("instance"),
			location.variable("location"),
			base::variable(position, "position"));
	auto moved = locate(instance);

	location.insert(position, moved.second->first,
			Location::extract(moved.second, &log), &log);
}
size_t Location::size() const {
	return method("", nullptr, base::variable(containing.size(), "size"));
}
std::string Location::map_loc(std::map<size_t, Object*>& map) {
	std::stringstream result("{");

	for (auto m : map)
		result << "\n\t[" << m.first << "]" << m.second << ";";
	result.seekg(0, std::ios::end);
	result.unget();
	if (result.peek() == ';') {
		result.get();
		result.put('\n');
	} else
		result.put(' ');

	return result.str() + "}";
}
size_t Location::which(const Object& instance) {
	size_t result = 0;
	auto log = function<decltype(result)>(nullptr, "base", typeid(Object),
			"which", "", instance.variable("instance"));
	if (instance.position)
		result =
				dynamic_cast<Location*>(instance.position)->locate(instance).first;

	return log.returning(std::move(result));
}
std::string Location::who(const Object& instance) {
	std::string result;
	auto log = function<decltype(result)>(nullptr, "base", typeid(Location),
			"who", "", instance.variable("instance"));

	if (instance.position)
		result =
				dynamic_cast<Location*>(instance.position)->locate(instance).second->first;

	return log.returning(std::move(result));
}
std::vector<Object*> Location::path(const Object& instance) {
	std::vector<Object*> result;
	auto log = function<decltype(result)>(nullptr, "base", typeid(Location),
			"path", "", instance.variable("instance"));

	if (instance.position)
		result = path(*instance.position);
	result.push_back(&const_cast<Object&>(instance));

	return log.returning<decltype(result)&>(result,
			[](std::vector<Object*>& value) {
				std::stringstream text("{");

				for (auto pointer : value)
				text << "\t\n" << pointer << ";";
				text.seekg(0, std::ios::end);
				text.unget();
				if (text.peek() == ';') {
					text.get();
					text.put('\n');
				} else
				text.put(' ');

				return text.str() + "}";
			});
}

Location::Location(Location* position,
		std::map<std::string, std::string> attributing, const Log* caller,
		std::string ns, bool open, std::string message) :
		Object(position, attributing, caller, ns, open, message) {
}

std::string typer(std::type_index& type) {
	return type.name();
}
}
namespace game {
//Card
base::Object& Card::side(bool covered) const {
	Object* result = nullptr;
	auto log = method<decltype(result)>(nullptr, "side", "",
			base::variable(covered, "covered"));

	if ((result = container[covered ? 0 : 1]))
		return *log.returning(result);
	else {
		std::string message = "ERROR the "
				+ std::string(covered ? "cover" : "face")
				+ " is not instanced.";

		log.message(message);
		log.error(message);

		throw std::length_error(message);
	}
}
std::map<std::string, std::string> Card::evaluate(
		std::map<std::string, std::string> attributing) {
	std::map<std::string, std::string> result;
	auto log = method<decltype(result)>(nullptr, "evaluate", "",
			base::Variable<std::map<std::string, std::string>&>(lister,
					"attributing", attributing));
	auto c = attributing.find("covered");
	auto f = attributing.find("faced");
	auto C = attributing.find("cover");
	auto F = attributing.find("face");
	auto command = 0;

	if (c != attributing.end())
		++command;
	if (f != attributing.end())
		command += 2;
	if (C != attributing.end())
		command += 4;
	if (F != attributing.end())
		command += 8;
	if ((command == 1 || command == 3) && c->second == "?")
		result["covered"] = covered ? "true" : "false";
	if ((command == 2 || command == 3) && c->second == "?")
		result["faced"] = covered ? "false" : "true";
	if (command == 4 || command == 8) {
		result["error"] = "false";
		if (c->second == "true")
			covered = command == 4;
		else if (c->second == "false")
			covered = command == 8;
		else
			result["error"] = "true";
	}
	if (result.empty())
		result["error"] = "true";

	return log.returning<std::map<std::string, std::string>&>(result, lister);
}
base::Object& Card::operator ()() const {
	auto log = method<Object&>(nullptr, "", "");

	return *log.returning(&side(covered));
}
bool Card::facing() const {
	return method("", nullptr, base::variable(!covered, "facing"));
}
void Card::facing(const Log* caller) {
	auto log = method<void>(caller, "facing", "");

	covered = false;
	modification = std::chrono::system_clock::to_time_t(
			std::chrono::system_clock::now());
}
bool Card::covering() const {
	return method("", nullptr, base::variable(covered, "covering"));
}
void Card::covering(const Log* caller) {
	auto log = method<void>(caller, "covering", "");

	covered = true;
	modification = std::chrono::system_clock::to_time_t(
			std::chrono::system_clock::now());
}
void Card::flip(const Log* caller) {
	auto log = method<void>(caller, "flip", "");

	covered = !covered;
	modification = std::chrono::system_clock::to_time_t(
			std::chrono::system_clock::now());
}
std::unique_ptr<Card> Card::construct(std::unique_ptr<Object>&& cover,
		std::unique_ptr<Object>&& face, bool covered, base::Location* position,
		std::map<std::string, std::string> attributing, const Log* caller) {
	std::unique_ptr<Card> result;
	auto log = function<decltype(result)>(caller, "game", typeid(Card),
			"construct", "", base::variable(cover.get(), "cover"),
			base::variable(face.get(), "face"),
			base::variable(covered, "covered"),
			base::variable(position, "position"),
			base::Variable<std::map<std::string, std::string>&>(lister,
					"attributing", attributing));

	result.reset(
			new Card(std::move(cover), std::move(face), covered, position,
					attributing, &log));

	return log.unique_ptr(std::move(result));
}

Card::Card(std::unique_ptr<Object>&& cover, std::unique_ptr<Object>&& face,
		bool covered, base::Location* position,
		std::map<std::string, std::string> attributing, const Log* caller) :
		base::Object(position, attributing, caller, "game", true, ""), container(
				this, caller) {
	this->covered = covered;
	container.insert_front("cover", std::move(cover), this);
	container.insert_back("face", std::move(face), this);
}
//Card::That
std::map<std::string, std::string> Card::That::evaluate(
		std::map<std::string, std::string> attributing) {
	auto log = method<std::map<std::string, std::string>>(nullptr, "evaluate",
			"",
			base::Variable<std::map<std::string, std::string>&>(lister,
					"attributing", attributing));

	return log.returning<std::map<std::string, std::string>&>(attributing,
			base::Location::lister);
}

Card::That::That(Object* owner, const Log* caller) :
		base::Location(dynamic_cast<Location*>(owner),
				std::map<std::string, std::string>(), caller, "game", false, "") {
}

//Deck
size_t Deck::size() const {
	auto log = method<size_t>(nullptr, "size", "");

	return log.returning(container.size());
}
std::unique_ptr<Card> Deck::draw(const Log* caller) {
	std::unique_ptr<Card> result;
	auto log = method<decltype(result)>(caller, "draw", "");

	result.reset(dynamic_cast<Card*>(container.extract(1, &log).release()));

	return log.unique_ptr(std::move(result));
}
std::unique_ptr<Card> Deck::extract(const Log* caller) {
	std::unique_ptr<Card> result;
	auto log = method<decltype(result)>(caller, "extract", "");
	auto size = container.size();
	std::default_random_engine generator;

	result.reset(
			dynamic_cast<Card*>(container.extract(
					std::uniform_int_distribution<size_t>(size ? 1 : 0, size)(
							generator), &log).release()));

	return log.unique_ptr(std::move(result));
}
std::unique_ptr<Card> Deck::get_bottom(const Log* caller) {
	std::unique_ptr<Card> result;
	auto log = method<decltype(result)>(caller, "draw", "");

	result.reset(
			dynamic_cast<Card*>(container.extract(container.size(), &log).release()));

	return log.unique_ptr(std::move(result));
}
void Deck::put_up(std::string name, std::unique_ptr<Card>&& card,
		const Log* caller) {
	auto log = method<void>(caller, "put_up", "", base::variable(name, "name"),
			base::variable(card.get(), "card"));

	container.insert_front(name,
			std::unique_ptr<Object>(card.release()),
			&log);
}
void Deck::insert(std::string name, std::unique_ptr<Card>&& card,
		const Log* caller) {
	auto log = method<void>(caller, "insert", "", base::variable(name, "name"),
			base::variable(card.get(), "card"));
	std::default_random_engine generator;

	container.insert(
			std::uniform_int_distribution<size_t>(1, container.size() + 1)(
					generator), name, std::unique_ptr<Object>(card.release()), &log);
}
void Deck::put_down(std::string name, std::unique_ptr<Card>&& card,
		const Log* caller) {
	auto log = method<void>(caller, "put_down", "",
			base::variable(name, "name"), base::variable(card.get(), "card"));

	container.insert_back(name, std::unique_ptr<Object>(card.release()), &log);
}
void Deck::shuffle(const Log* caller) {
	auto log = method<void>(caller, "shuffle", "");
	auto length = container.size();
	decltype(length) current = 1;
	std::uniform_int_distribution<size_t> distribution(length ? current : 0,
			length);

	for (std::default_random_engine generator; length > 0; --length) {
		log.message(
				"current=" + std::to_string(current = distribution(generator)));
		container.take(current, container, 0, &log);
	}
}

std::unique_ptr<Deck> Deck::construct(std::string name, base::Location* position,
		std::map<std::string, std::string> attributes, const Log* caller) {
	std::unique_ptr<Deck> result;
	auto log = function<decltype(result)>(caller, "game", typeid(Deck), name,
			"", base::variable(name, "name"),
			base::variable(position, "position"),
			base::Variable<std::map<std::string, std::string>&>(lister, "attributes",
					attributes));

	result.reset(new Deck(name, position, attributes, &log));

	return log.unique_ptr(std::move(result));
}

Deck::Deck(std::string name, base::Location* position,
		std::map<std::string, std::string> attributes, const Log* caller) :
		Object(position, attributes, caller, "game", true, ""), container(this, caller) {
	this->name = name;
}
}
