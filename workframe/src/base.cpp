/*
 * base.cpp
 *
 *  Created on: 15 dic 2017
 *      Author: m1r
 */

#include "base.h"
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

std::string Log::tracks() const {
	return legacy + std::to_string(track);
}
std::string Log::logs() const {
	return tracks() + ": " + logging;
}
std::string Log::transcodes_arguments() {
	return "";
}
std::string Log::tracks(const Log* caller) {
	if (caller)
		return caller->legacy + "." + std::to_string(caller->track) + ".";
	else
		return ".";
}
std::string Log::messages(std::string message) {
	if (message.length())
		message = " \"" + message + "\"";

	return message;
}
std::string Log::transcodes(const Log& log) {
	return std::to_string(log.track);
}
Log Log::as_destructor(std::string ns, std::type_index type,
		std::string message) const {
	Log result(nullptr, ns, false);
	auto typer = type.name();

	result.logging = ns + "::" + typer + "::~" + typer;
	std::clog << result.logs() << messages(message) << std::endl;

	return result;
}
std::string Log::derives_transcode(const Log* derived) {
	return transcodes(*derived);
}
void Log::notes(std::string text) const {
	std::clog << tracks() + " " + messages(text) << std::endl;
}
void Log::logs_error(std::string message) const {
	std::cerr << logs() << messages(message) << std::endl;
}
Variable<const Log&> Log::gets_variable(std::string name) const {
	return Variable<const Log&>(name, ns, *this, transcodes);
}

Log::Log(const Log* caller, std::string ns, bool open) {
	legacy = tracks(caller);
	track = ++tracker;
	this->open = open;
	this->ns = ns;
}
Log::Log(const Log* caller, std::string ns, bool open, std::string message) {
	legacy = tracks(caller);
	track = ++tracker;
	this->ns = ns;
	logging = gets_variable("").is() + "::" + typeid(*this).name() + "()";
	std::clog << logs() << messages(message)
			<< (((this->open = open)) ? " {" : "") << std::endl;
}
Log::~Log() {
	if (std::is_same<typename std::decay<decltype(*this)>::type, Log>::value
			&& open)
		std::clog << tracks() << "  }" << std::endl;
	else if (!open)
		std::clog << "~" << gets_variable("").is() << std::endl;
}
Log::Log(Log& copy) {
	legacy = copy.legacy;
	track = copy.track;
	open = copy.open;
	ns = copy.ns;
	logging = copy.logging;
	copy.open = false;
}
Log::Log(Log&& moving) {
	legacy = moving.legacy;
	track = moving.track;
	open = moving.open;
	ns = moving.ns;
	logging = moving.logging;
	moving.open = false;
}
Log& Log::operator =(Log&& assigning) {
	legacy = assigning.legacy;
	track = assigning.track;
	open = assigning.open;
	ns = assigning.ns;
	logging = assigning.logging;
	assigning.open = false;

	return *this;
}

//Object
std::set<Object*> Object::everything;

long long unsigned Object::who_is() const {
	return as_method("", nullptr,
			Variable<const decltype(track)&>("who_is", "", track));
}
Object* Object::where_is() const {
	return as_method("", nullptr,
			Variable<const decltype(position)&>("where_is", "base", position));
}
time_t Object::exists_since() const {
	return as_method("", nullptr,
			Variable<const decltype(creation)&>("exists_since", "", creation));
}
std::map<std::string, std::string> Object::gets_attributes() const {
	return as_method("", nullptr,
			Variable<const decltype(attributing)&>("gets_attributes", "std",
					attributing, write_string_map));
}
void Object::sets_attributes(std::map<std::string, std::string> attributing,
		const Log* caller) {
	auto log = as_method<void>(caller, "sets_attributes", "",
			Variable<const decltype(attributing)&>("attributing", "",
					attributing, write_string_map));

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
Object::modifications Object::gets_modifications() {
	modifications result;
	auto log = as_method<decltype(result)>(nullptr, "gets_modifications", "");

	result.first = modification;
	for (auto change : changes)
		result.second[change.first] = std::make_pair(change.second,
				attributing.at(change.first));

	return log.returns<decltype(result)&>(result, transcode_modifications);
}
Object& Object::operator =(std::map<std::string, std::string> map) {
	auto log = as_binary<Object&>(nullptr, "=",
			Variable<const std::map<std::string, std::string>&>("map", "std",
					map, write_string_map), "");

	sets_attributes(map, &log);

	return log.returns<Object&>(*this, transcodes);
}
bool Object::operator ==(const Object& than) const {
	return as_binary(nullptr,
			Variable<const bool>("==", "", attributing == than.attributing),
			than.gets_variable("than"), "");
}
bool Object::operator !=(const Object& than) const {
	return as_binary(nullptr,
			Variable<const bool>("!=", "", attributing != than.attributing),
			than.gets_variable("than"), "");
}
Variable<const Object&> Object::gets_variable(std::string name) const {
	return std::forward<Variable<const Object&>>(
			Variable<const Object&>(name, "base", *this, transcodes));
}
std::set<Object*>& Object::gets_all() {
	Variable<decltype(everything)&> e("gets_all", "std", everything, write_set);

	return as_function("", nullptr, e, "base", typeid(Object));
}
std::set<Object*> Object::gets_roots() {
	std::set<Object*> result;
	auto log = as_function<decltype(result)>(nullptr, "base", typeid(Object),
			"gets_roots", "");
	decltype(result) e = everything;

	for (auto object : e)
		if (!object->position)
			result.insert(object);

	return log.returns<decltype(result)&>(result, write_set);
}
std::string Object::write_string_map(
		const std::map<std::string, std::string>& map) {
	std::string result = "{";

	for (auto mapped : map)
		result += "\n\t" + mapped.first + "=" + mapped.second + ",";
	if (result.back() == ',')
		result.back() = '\n';
	else
		result += " ";

	return result + "}";
}
std::string Object::transcode_modifications(const modifications& mods) {
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
std::string Object::write_set(const std::set<Object*>& set) {
	std::stringstream result("{");

	for (auto object : set)
		result << " " << object << ";";
	result.seekg(0, std::ios::end);
	result.unget();
	if (result.peek() == ';')
		result.get();

	return result.str() + " }";
}
Object::~Object() {
	if (running())
		everything.erase(this);
	if (!open)
		as_destructor("base", typeid(Object), "");
}
Object::Object(Object&& moving) :
		Log(std::move(moving)) {
	auto log = as_constructor(this, "base", typeid(Object), "",
			moving.derives_variable("moving"));

	position = moving.position;
	creation = moving.creation;
	attributing = moving.attributing;
	attributing = moving.changes;
	modification = std::chrono::system_clock::to_time_t(
			std::chrono::system_clock::now());
	everything.emplace(this);
}
Object& Object::operator =(Object&& moving) {
	auto log = as_binary<Object&>(nullptr, "=", moving.gets_variable("moving"),
			"");

	position = moving.position;
	creation = moving.creation;
	attributing = moving.attributing;
	attributing = moving.changes;
	modification = std::chrono::system_clock::to_time_t(
			std::chrono::system_clock::now());
	Log::operator =(std::move(moving));

	return log.returns<Object&>(*this, transcodes);
}

//Location
std::string Location::names(std::string name) {
	std::string candidate = name;
	auto log = as_method<decltype(name)>(nullptr, "names", "",
			Variable<decltype(name)&>("name", "std", name));

	if (candidate.empty())
		candidate = name = "content";
	for (auto suffix = 0; operator ()(candidate).size(); ++suffix)
		candidate = name + "_" + std::to_string(suffix);

	return log.returns(candidate);
}
Location::container::iterator Location::locates(size_t offset) const {
	auto result = const_cast<container*>(containing)->begin();
	auto log = as_method<decltype(result)>(nullptr, "locates", "",
			Variable<decltype(offset)&>("offset", "", offset));
	auto end = const_cast<container*>(containing)->end();
	size_t index = 1;

	if (offset)
		while (result != end && ++index < offset)
			++result;
	else
		result = end;
	if (result == end) {
		std::ostringstream message;

		message << "WARNING invalid argument: offset > size="
				<< containing->size() << ".";
		log.logs_error(message.str());
	}

	return log.returns<decltype(result)&>(result, writes_iterator);
}
std::map<size_t, Location::container::iterator> Location::locates(
		std::string name) const {
	std::map<size_t, Location::container::iterator> result;
	auto log = as_method<decltype(result)&>(nullptr, "locates", "",
			Variable<std::string>("name", "std", name));
	auto current = std::make_pair(1, containing->begin());

	for (auto end = containing->end(); current.second != end; ++current.first) {
		if (current.second->first.substr(0,
				current.second->first.find_last_of("_")) == name)
			result.emplace(current);
		++current.second;
	}
	if (result.empty()) {
		std::string message = "WARNING no Object exists here with that name.";
		log.messages(message);
		log.logs_error(message);
	}

	return log.returns<decltype(result)&>(result, writes_map);
}
std::map<size_t, Location::container::iterator> Location::locates(
		std::type_index type) const {
	std::map<size_t, Location::container::iterator> result;
	auto log = as_method<decltype(result)>(nullptr, "locates", "",
			Variable<decltype(type)>("type", type));
	auto current = std::make_pair(1, containing->begin());

	for (auto end = containing->end(); current.second != end; ++current.first) {
		if (type == typeid(*current.second->second))
			result.emplace(current);
		++current.second;
	}
	if (result.empty()) {
		std::string message =
				"WARNING invalid argument: the instance is not located here.";
		log.messages(message);
		log.logs_error(message);
	}

	return log.returns<decltype(result)&>(result, writes_map);
}
std::pair<size_t, Location::container::iterator> Location::locates(
		const Object& instance) const {
	auto result = std::make_pair(1, containing->begin());
	auto log = as_method<decltype(result)>(nullptr, "locates", "",
			instance.gets_variable("instance"));
	auto end = containing->end();

	while (result.second != end && result.second->second.get() != &instance) {
		++result.first;
		++result.second;
	}
	if (result.second == end) {
		std::string message;

		message = "WARNING invalid argument: the instance is not located here.";
		log.messages(message);
		log.logs_error(message);
	}

	return log.returns<std::pair<size_t, Location::container::iterator>>(result,
			writes_pair);
}
std::unique_ptr<Object> Location::extracts(container::iterator iterator,
		const Log* caller) {
	std::unique_ptr<Object> result;
	auto log = as_method<decltype(result)>(caller, "extracts", "",
			Variable<container::iterator&>("iterator",
					std::string("std::") + typeid(container).name(), iterator,
					writes_iterator));

	if (iterator == containing->end()) {
		std::string message =
				"ERROR invalid argument: iterator is out of range.";

		log.messages(message);
		log.logs_error(message);

		throw std::invalid_argument(message);
	}
	iterator->second.swap(result);
	result->position = nullptr;
	containing->erase(iterator);
	modification = result->modification = std::chrono::system_clock::to_time_t(
			std::chrono::system_clock::now());

	return returns_reference(std::move(result));
}
void Location::removes(container::const_iterator iterator, const Log* caller) {
	auto log = as_method<void>(caller, "removes", "",
			Variable<container::const_iterator&>(iterator, "", "iterator",
					writes_iterator));

	if (iterator == containing->end()) {
		std::string message =
				"WARNING invalid argument: iterator is out of range.";

		log.messages(message);
		log.logs_error(message);

		throw std::invalid_argument(message);
	} else {
		containing->erase(iterator);
		modification = std::chrono::system_clock::to_time_t(
				std::chrono::system_clock::now());
	}
}
std::string Location::writes_iterator(container::const_iterator iterator) {
	std::ostringstream result;

	result << iterator->second.get();

	return result.str();
}
std::string Location::writes_map(
		std::map<size_t, container::const_iterator> map) {
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
std::string Location::writes_pair(
		std::pair<size_t, container::const_iterator> pair) {
	std::ostringstream result("[");

	result << pair.first << "]" << pair.second->second.get();

	return result.str();
}
Object* Location::operator [](size_t offset) const {
	Object* result = nullptr;
	auto log = as_binary<decltype(result)>(nullptr, "[]",
			Variable<>(offset, "offset"), "");
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
std::unique_ptr<Location> Location::construct(
		std::map<std::string, std::string> attributes, const Log* caller,
		std::string message) {
	auto log =
			Log::function < std::unique_ptr
					< Location
							>> (caller, "base", typeid(Location), "construct", "", Variable<
									std::map<std::string, std::string>&>(lister,
									"attributes", attributes));

	return log.unique_ptr(
			std::unique_ptr<Location>(
					new Location(nullptr, attributes, &log, "base", false,
							message)));
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

	if ((result = (*container)[covered ? 0 : 1]))
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
		std::unique_ptr<Object>&& face, bool covered,
		std::map<std::string, std::string> attributing, const Log* caller,
		std::string message) {
	auto log = function<std::unique_ptr<Card>>(caller, "game", typeid(Card),
			"construct", "", base::variable(cover.get(), "cover"),
			base::variable(face.get(), "face"),
			base::variable(covered, "covered"),
			base::Variable<std::map<std::string, std::string>&>(lister,
					"attributing", attributing));

	return log.unique_ptr(
			std::unique_ptr<Card>(
					new Card(std::move(cover), std::move(face), covered,
							nullptr, attributing, &log, message)));
}

Card::Card(std::unique_ptr<Object>&& cover, std::unique_ptr<Object>&& face,
		bool covered, base::Location* position,
		std::map<std::string, std::string> attributing, const Log* caller,
		std::string message) :
		base::Object(position, attributing, caller, "game", true, message) {
	base::Location::construct(attributing, this, "").swap(container);
	this->covered = covered;
	container->insert_front("cover", std::move(cover), this);
	container->insert_back("face", std::move(face), this);
}

//Deck
size_t Deck::size() const {
	auto log = method < size_t > (nullptr, "size", "");

	return log.returning(container->size());
}
const std::string& Deck::label() const {
	return method<const std::string&>("", nullptr, base::variable(name, "name"));
}
std::unique_ptr<Card> Deck::draw(const Log* caller) {
	std::unique_ptr<Card> result;
	auto log = method<decltype(result)>(caller, "draw", "");

	result.reset(dynamic_cast<Card*>(container->extract(1, &log).release()));

	return log.unique_ptr(std::move(result));
}
std::unique_ptr<Card> Deck::extract(const Log* caller) {
	std::unique_ptr<Card> result;
	auto log = method<decltype(result)>(caller, "extract", "");
	auto size = container->size();
	std::default_random_engine generator;

	result.reset(
			dynamic_cast<Card*>(container->extract(
					std::uniform_int_distribution<size_t>(size ? 1 : 0, size)(
							generator), &log).release()));

	return log.unique_ptr(std::move(result));
}
std::unique_ptr<Card> Deck::get_bottom(const Log* caller) {
	std::unique_ptr<Card> result;
	auto log = method<decltype(result)>(caller, "draw", "");

	result.reset(
			dynamic_cast<Card*>(container->extract(container->size(), &log).release()));

	return log.unique_ptr(std::move(result));
}
void Deck::put_up(std::string name, std::unique_ptr<Card>&& card,
		const Log* caller) {
	auto log = method<void>(caller, "put_up", "", base::variable(name, "name"),
			base::variable(card.get(), "card"));

	container->insert_front(name, std::unique_ptr<Object>(card.release()),
			&log);
}
void Deck::insert(std::string name, std::unique_ptr<Card>&& card,
		const Log* caller) {
	auto log = method<void>(caller, "insert", "", base::variable(name, "name"),
			base::variable(card.get(), "card"));
	std::default_random_engine generator;

	container->insert(
			std::uniform_int_distribution<size_t>(1, container->size() + 1)(
					generator), name, std::unique_ptr<Object>(card.release()),
			&log);
}
void Deck::put_down(std::string name, std::unique_ptr<Card>&& card,
		const Log* caller) {
	auto log = method<void>(caller, "put_down", "",
			base::variable(name, "name"), base::variable(card.get(), "card"));

	container->insert_back(name, std::unique_ptr<Object>(card.release()), &log);
}
void Deck::shuffle(const Log* caller) {
	auto log = method<void>(caller, "shuffle", "");
	auto length = container->size();
	decltype(length) current = 1;
	std::uniform_int_distribution<size_t> distribution(length ? current : 0,
			length);

	for (std::default_random_engine generator; length > 0; --length) {
		log.message(
				"current=" + std::to_string(current = distribution(generator)));
		container->take(current, *container, 0, &log);
	}
}

std::unique_ptr<Deck> Deck::construct(std::string name,
		std::map<std::string, std::string> attributes, const Log* caller,
		std::string message) {
	function<std::unique_ptr<Deck>>(caller, "game", typeid(Deck), name, "",
			base::variable(name, "name"),
			base::Variable<std::map<std::string, std::string>&>(lister,
					"attributes", attributes)).unique_ptr();

	return log.unique_ptr(
			std::unique_ptr<Deck>(
					new Deck(name, nullptr, attributes, &log, message)));
}

Deck::Deck(std::string name, base::Location* position,
		std::map<std::string, std::string> attributes, const Log* caller,
		std::string message) :
		Object(position, attributes, caller, "game", true, message) {
	base::Location::construct(attributes, this, "").swap(container);
	this->name = name;
}
*/
}
