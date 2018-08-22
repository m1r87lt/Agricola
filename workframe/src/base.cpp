/*
 * base.cpp
 *
 *  Created on: 15 dic 2017
 *      Author: m1r
 */

#include "base.h"
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
Variable<const Log&> Log::gives_variable(std::string name) const {
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
	logging = gives_variable("").is() + "::" + typeid(*this).name() + "()";
	std::clog << logs() << messages(message)
			<< (((this->open = open)) ? " {" : "") << std::endl;
}
Log::~Log() {
	if (std::is_same<typename std::decay<decltype(*this)>::type, Log>::value
			&& open)
		std::clog << tracks() << "  }" << std::endl;
	else if (!open)
		std::clog << "~" << gives_variable("").is() << std::endl;
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
std::map<std::string, std::string> Object::gives_attributes() const {
	return as_method("", nullptr,
			Variable<const decltype(attributing)&>("gives_attributes", "std",
					attributing, write_string_map));
}
void Object::gets_attributes(std::map<std::string, std::string> attributing,
		const Log* caller) {
	auto log = as_method<void>(caller, "gets_attributes", "",
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
Object::modifications Object::gives_modifications() {
	modifications result;
	auto log = as_method<decltype(result)>(nullptr, "gives_modifications", "");

	result.first = modification;
	for (auto change : changes)
		result.second[change.first] = std::make_pair(change.second,
				attributing.at(change.first));

	return log.returns<decltype(result)&>(result, transcode_modifications);
}
Object& Object::operator =(std::map<std::string, std::string> map) {
	auto log = as_binary<Object&>(nullptr, "=",
			Variable<const decltype(map)&>("map", "std", map, write_string_map),
			"");

	gets_attributes(map, &log);

	return log.returns<Object&>(*this, transcodes);
}
bool Object::operator ==(const Object& than) const {
	return as_binary(nullptr,
			Variable<const bool>("==", "", attributing == than.attributing),
			than.gives_variable("than"), "");
}
bool Object::operator !=(const Object& than) const {
	return as_binary(nullptr,
			Variable<const bool>("!=", "", attributing != than.attributing),
			than.gives_variable("than"), "");
}
Variable<const Object&> Object::gives_variable(std::string name) const {
	return std::forward<Variable<const Object&>>(
			Variable<const Object&>(name, "base", *this, transcodes));
}
std::set<Object*>& Object::get_all() {
	Variable<decltype(everything)&> e("get_all", "std", everything, write_set);

	return as_function("", nullptr, e, "base", typeid(Object));
}
std::set<Object*> Object::get_roots() {
	std::set<Object*> result;
	auto log = as_function<decltype(result)>(nullptr, "base", typeid(Object),
			"get_roots", "");
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
	auto log = as_binary<Object&>(nullptr, "=", moving.gives_variable("moving"),
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
			Variable<decltype(name)>("name", "std",
					std::forward<decltype(name)&&>(name)));

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

	return log.returns<decltype(result)&>(result, write_iterator);
}
std::map<size_t, Location::container::iterator> Location::locates(
		std::string name) const {
	std::map<size_t, Location::container::iterator> result;
	auto log = as_method<decltype(result)&>(nullptr, "locates", "",
			Variable<decltype(name)>("name", "std",
					std::forward<decltype(name)&&>(name)));
	auto current = std::make_pair(1, containing->begin());

	for (auto end = containing->end(); current.second != end; ++current.first) {
		if (current.second->first.substr(0,
				current.second->first.find_last_of("_")) == name)
			result.emplace(current);
		++current.second;
	}
	if (result.empty()) {
		std::string message = "WARNING no Object exists here with that name.";
		log.notes(message);
		log.logs_error(message);
	}

	return log.returns<decltype(result)&>(result, write_map);
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
		log.notes(message);
		log.logs_error(message);
	}

	return log.returns<decltype(result)&>(result, write_map);
}
std::pair<size_t, Location::container::iterator> Location::locates(
		const Object& instance) const {
	auto result = std::make_pair(1, containing->begin());
	auto log = as_method<decltype(result)>(nullptr, "locates", "",
			instance.gives_variable("instance"));
	auto end = containing->end();

	while (result.second != end && result.second->second.get() != &instance) {
		++result.first;
		++result.second;
	}
	if (result.second == end) {
		std::string message;

		message = "WARNING invalid argument: the instance is not located here.";
		log.notes(message);
		log.logs_error(message);
	}

	return log.returns<std::pair<size_t, Location::container::iterator>>(result,
			write_pair);
}
std::unique_ptr<Object> Location::extracts(container::iterator iterator,
		const Log* caller) {
	std::unique_ptr<Object> result;
	auto log = as_method<decltype(result)>(caller, "extracts", "",
			Variable<container::iterator>("iterator",
					std::string("std::") + typeid(container).name(),
					std::forward<decltype(iterator)&&>(iterator),
					write_iterator));

	if (iterator == containing->end()) {
		std::string message =
				"ERROR invalid argument: iterator is out of range.";

		log.notes(message);
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
void Location::removes(container::iterator iterator, const Log* caller) {
	auto log = as_method<void>(caller, "removes", "",
			Variable<container::iterator>("iterator",
					std::string("std::") + typeid(container).name(),
					std::forward<decltype(iterator)>(iterator),
					write_iterator));

	if (iterator == containing->end()) {
		std::string message =
				"WARNING invalid argument: iterator is out of range.";

		log.notes(message);
		log.logs_error(message);

		throw std::invalid_argument(message);
	} else {
		containing->erase(iterator);
		modification = std::chrono::system_clock::to_time_t(
				std::chrono::system_clock::now());
	}
}
std::string Location::write_iterator(container::const_iterator iterator) {
	std::ostringstream result;

	result << iterator->second.get();

	return result.str();
}
std::string Location::write_map(
		const std::map<size_t, container::iterator>& map) {
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
std::string Location::write_pair(
		const std::pair<size_t, container::iterator>& pair) {
	std::ostringstream result("[");

	result << pair.first << "]" << pair.second->second.get();

	return result.str();
}
Object* Location::operator [](size_t offset) const {
	Object* result = nullptr;
	auto log = as_binary<decltype(result)>(nullptr, "[]",
			Variable<decltype(offset)>("offset", "",
					std::forward<decltype(offset)&&>(offset)), "");
	auto iterator = locates(offset);

	if (iterator != containing->end())
		result = iterator->second.get();

	return log.returns(result);
}
std::map<size_t, Object*> Location::operator ()(std::string name) const {
	std::map<size_t, Object*> result;
	auto log = as_method<decltype(result)>(nullptr, "", "",
			Variable<decltype(name)>("name", "std",
					std::forward<decltype(name)&&>(name)));
	auto located = locates(name);

	for (auto found : located)
		result.emplace(found.first, found.second->second.get());

	return log.returns<decltype(result)&>(result, write_object_map);
}
std::map<size_t, Object*> Location::operator ()(std::type_index type) const {
	std::map<size_t, Object*> result;
	auto log = as_method<decltype(result)>(nullptr, "", "",
			Variable<decltype(type)>("type", type));
	auto located = locates(type);

	for (auto found : located)
		result.emplace(found.first, found.second->second.get());

	return log.returns<decltype(result)&>(result, write_object_map);
}
void Location::inserts_front(std::string name,
		std::unique_ptr<Object>&& instance, const Log* caller) {
	auto log = as_method<void>(caller, "inserts_front", "",
			Variable<decltype(name)>("name", "std",
					std::forward<decltype(name)&&>(name)),
			Variable<Object*>("instance", "", instance.get()));

	inserts(1, name, std::move(instance), &log);
}
void Location::inserts(size_t offset, std::string name,
		std::unique_ptr<Object>&& instance, const Log* caller) {
	auto log = as_method<void>(caller, "inserts", "",
			Variable<decltype(offset)&>("offset", "", offset),
			Variable<decltype(name)>("name", "std",
					std::forward<decltype(name)&&>(name)),
			(Variable<Object*> ) Reference<Object>("instance", instance));
	std::ostringstream message;

	if (instance.get()) {
		if (offset > containing->size() + 1)
			message << "WARNING invalid argument: offset > 1 + size="
					<< containing->size() << ".";
		else if (offset) {
			auto result = locates(offset);

			result = containing->emplace(result, names(name), nullptr);
			result->second.swap(instance);
			result->second->position = this;
			modification = result->second->modification =
					std::chrono::system_clock::to_time_t(
							std::chrono::system_clock::now());
		} else
			message << "WARNING invalid argument: offset > 1 + size="
					<< containing->size() << ".";
	} else
		message << "ERROR invalid argument: instance cannot be undefined.";
	if (message.str().length()) {
		log.notes(message.str());
		log.logs_error(message.str());

		throw std::invalid_argument(message.str());
	}
}
void Location::inserts_back(std::string name,
		std::unique_ptr<Object>&& instance, const Log* caller) {
	auto log = as_method<void>(caller, "inserts_back", "",
			Variable<decltype(name)&>("name", "std", name),
			(Variable<Object*> ) Reference<Object>("instance", instance));

	inserts(containing->size() + 1, name, std::move(instance), &log);
}
void Location::removes(size_t offset, const Log* caller) {
	auto log = as_method<void>(caller, "removes", "",
			Variable<size_t&>("offset", "", offset));

	removes(locates(offset), &log);
}
void Location::removes(std::string name, const Log* caller) {
	auto log = as_method<void>(caller, "removes", "",
			Variable<decltype(name)&>("name", "std", name));
	auto located = locates(name);

	for (auto found : located)
		removes(found.second, &log);
}
void Location::removes(std::type_index type, const Log* caller) {
	auto log = as_method<void>(caller, "removes", "",
			Variable<decltype(type)>("type", type));

	auto located = locates(type);

	for (auto found : located)
		removes(found.second, &log);
}
void Location::removes(const Object& instance, const Log* caller) {
	auto log = as_method<void>(caller, "removes", "",
			instance.gives_variable("instance"));

	removes(locates(instance).second, &log);
}
std::unique_ptr<Object> Location::extracts(size_t offset, const Log* caller) {
	std::unique_ptr<Object> result;
	auto log = as_method<decltype(result)>(caller, "extracts", "",
			Variable<decltype(offset)&>("offset", "", offset));

	extracts(locates(offset), &log).swap(result);

	return log.returns_reference(std::move(result));
}
std::unique_ptr<Object> Location::extracts(const Object& instance,
		const Log* caller) {
	std::unique_ptr<Object> result;
	auto log = as_method<decltype(result)>(caller, "extracts", "",
			instance.gives_variable("instance"));

	extracts(locates(instance).second, &log).swap(result);

	return log.returns_reference(std::move(result));
}
void Location::takes(size_t source, Location& location, size_t destination,
		const Log* caller) {
	auto log = as_method<void>(caller, "takes", "",
			Variable<decltype(source)&>("source", "", source),
			location.gives_variable("location"),
			Variable<decltype(destination)&>("destination", "", destination));
	auto moved = locates(source);

	location.inserts(destination, moved->first, Location::extracts(moved, &log),
			&log);
}
void Location::takes(const Object& instance, Location& location,
		size_t position, const Log* caller) {
	auto log = as_method<void>(caller, "takes", "",
			instance.gives_variable("instance"),
			location.gives_variable("location"),
			Variable<decltype(position)&>("position", "", position));
	auto moved = locates(instance);

	location.inserts(position, moved.second->first,
			Location::extracts(moved.second, &log), &log);
}
size_t Location::has_size() const {
	return as_method("", nullptr,
			Variable<size_t>("has_size", "", containing->size()));
}
size_t Location::get_position(const Object& instance) {
	size_t result = 0;
	auto log = as_function<decltype(result)>(nullptr, "base", typeid(Object),
			"get_position", "", instance.gives_variable("instance"));
	if (instance.position)
		result =
				dynamic_cast<Location*>(instance.position)->locates(instance).first;

	return log.returns(result);
}
std::string Location::get_name(const Object& instance) {
	std::string result;
	auto log = as_function<decltype(result)>(nullptr, "base", typeid(Location),
			"get_name", "", instance.gives_variable("instance"));

	if (instance.position)
		result =
				dynamic_cast<Location*>(instance.position)->locates(instance).second->first;

	return log.returns(result);
}
std::vector<Object*> Location::get_path(const Object& instance) {
	std::vector<Object*> result;
	auto log = as_function<decltype(result)>(nullptr, "base", typeid(Location),
			"get_path", "", instance.gives_variable("instance"));

	if (instance.position)
		result = get_path(*instance.position);
	result.push_back(&const_cast<Object&>(instance));

	return log.returns<decltype(result)&>(result, write_object_vector);
}
std::unique_ptr<Location> Location::construct(
		std::map<std::string, std::string> attributes, const Log* caller,
		std::string message) {
	auto log = as_function<std::unique_ptr<Location>>(caller, "base",
			typeid(Location), "construct", "",
			Variable<decltype(attributes)&>("attributes", "std", attributes,
					write_string_map));

	return log.returns_reference(
			std::unique_ptr<Location>(
					new Location(nullptr, attributes, &log, "base", false,
							message)));
}
std::string Location::write_object_map(std::map<size_t, Object*>& map) {
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
std::string Location::write_object_vector(std::vector<Object*>& vector) {
	std::stringstream result("{");

	for (auto v : vector)
		result << "\n\t" << v << ";";
	result.seekg(0, std::ios::end);
	result.unget();
	if (result.peek() == ';') {
		result.get();
		result.put('\n');
	} else
		result.put(' ');

	return result.str() + "}";
}
/*}
 namespace game {

 //Card
 base::Object& Card::gives_face__uncovered(bool covered) const {
 Object* result = nullptr;
 auto log = as_method<Object&>(nullptr, "gives_face__uncovered", "",
 base::Variable<decltype(covered)>("covered", "", covered));

 if ((result = (*container)[covered ? 0 : 1]))
 return log.returns<Object&>(*result, transcodes);
 else {
 std::string message = "ERROR the "
 + std::string(covered ? "cover" : "face")
 + " is not instanced.";

 log.messages(message);
 log.logs_error(message);

 throw std::length_error(message);
 }
 }
 base::Object& Card::operator ()() const {
 auto log = as_method<Object&>(nullptr, "", "");

 return log.returns<Object&>(gives_face__uncovered(covered), transcodes);
 }
 bool Card::is_facing() const {
 return as_method("", nullptr,
 base::Variable<bool>("is_facing", "", !covered));
 }
 void Card::shows_face(const Log* caller) {
 auto log = as_method<void>(caller, "shows_face", "");

 covered = false;
 modification = std::chrono::system_clock::to_time_t(
 std::chrono::system_clock::now());
 }
 bool Card::is_covering() const {
 return as_method("", nullptr,
 base::Variable<bool>("is_covering", "", covered));
 }
 void Card::shows_cover(const Log* caller) {
 auto log = as_method<void>(caller, "shows_cover", "");

 covered = true;
 modification = std::chrono::system_clock::to_time_t(
 std::chrono::system_clock::now());
 }
 void Card::flips(const Log* caller) {
 auto log = as_method<void>(caller, "flips", "");

 covered = !covered;
 modification = std::chrono::system_clock::to_time_t(
 std::chrono::system_clock::now());
 }
 std::unique_ptr<Card> Card::construct(std::unique_ptr<Object>&& cover,
 std::unique_ptr<Object>&& face, bool covered,
 std::map<std::string, std::string> attributing, const Log* caller,
 std::string message) {
 auto log = as_function<std::unique_ptr<Card>>(caller, "game", typeid(Card),
 "construct", "", Reference<Object>("cover", cover),
 Reference<Object>("face", face),
 base::Variable<bool>("covered", "", covered),
 base::Variable<decltype(attributing)&>(
 "attributing", "std", attributing));

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
 /**/
}
