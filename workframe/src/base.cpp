/*
 * base.cpp
 *
 *  Created on: 15 dic 2017
 *      Author: m1r
 */

#include "base.h"
#include <chrono>
/*#include <stdexcept>
 #include <fstream>
 #include <random>
 */
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
Variable<Log&> Log::variable() const {
	return Variable<Log&>("", ns, const_cast<Log&>(*this), transcoder);
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
std::string Log::transcoder(Log& derived) {
	return std::to_string(derived.track);
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
	if (open)
		std::clog << tracking() << "  }" << std::endl;
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
			Variable<const std::map<std::string, std::string>&>(lister,
					"attributes", attributing));
}
void Object::attribute(std::map<std::string, std::string> attributing,
		const Log* caller) {
	auto log = method<void>(caller, "attribute", "",
			Variable<std::map<std::string, std::string>&>(lister, "attributing",
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
	auto log = method<modifications>(nullptr, "what", "");

	result.first = modification;
	for (auto change : changes)
		result.second[change.first] = std::make_pair(change.second,
				attributing.at(change.first));

	return log.returning<modifications&>(result, changing);
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
std::string Object::lister(structure s) {
	return base::lister(s, ",", true, true, false,
			[](std::pair<std::string, std::string> content) {
				return content.first + "=" + content.second;
			});
}
std::string Object::lister(set s) {
	return base::lister(s, ";", false, true, false, [](Object* content) {
		return content;
	});
}
Object::set & Object::all() {
	function<std::type_index>("", nullptr, typeid(everything), "base",
			typeid(Object), "all").returned(lister(everything).c_str());

	return everything;
}
Object::set Object::root() {
	std::set<Object*> result;
	auto log = function<std::type_index>("", nullptr, typeid(result), "base",
			typeid(Object), "root");

	for (auto object : everything)
		if (!object->position)
			result.insert(object);
	log.returned(lister(result).c_str());

	return result;
}
std::string Object::logging(
		std::pair<time_t,
				std::map<std::string, std::pair<std::string, std::string>>>w) {
	std::string log = "{" + std::to_string(w.first) + ";\n\t{";

	for (auto pair : w.second)
	log += "\n\t\t" + pair.first + ": " + pair.second.first + "->"
	+ pair.second.second + ",";
	if (log.back() == ',') {
		log.pop_back();
		log+= "\n\t";
	} else
	log += " ";
	log += "}\n}";

	return log;
}
Object::Object(Object* position, structure attributes, const Log* caller) :
		Log(caller, "", false, "base", "position", position, "attributes",
				lister(attributes)) {
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
/*
 //Location
 std::string Location::naming(std::string name) {
 std::string candidate = name;
 Log log(
 method<std::type_index>(nullptr, "naming", typeid(candidate), "",
 "name", name));

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
 message << "[" << index << "]" << result->second.get();
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
 log.returned(
 base::lister(result, ",", false, true, false,
 [&message](decltype(current) found) {
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
 log.returned(
 base::lister(result, ",", false, true, false,
 [&message](decltype(current) found) {
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
 "[" + std::to_string(which(*iterator->second)) + "]"
 + std::to_string((size_t) iterator->second.get())));

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
 "[" + std::to_string(which(*iterator->second)) + "]"
 + std::to_string((size_t) iterator->second.get())));

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
 log.returned(
 base::lister(result, ",", true, true, false,
 [](std::pair<size_t, Object*> content) {
 std::ostringstream text;

 text << "[" << content.first << "]" << content.second;

 return text.str();
 }));

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
 log.returned(
 base::lister(result, ",", true, true, false,
 [](std::pair<size_t, Object*> content) {
 std::ostringstream text;

 text << "[" << content.first << "]" << content.second;

 return text.str();
 }));

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
 std::vector<Object*> Location::path(const Object& instance) {
 std::vector<Object*> result;
 Log log(
 function<std::type_index>(nullptr, "", typeid(result), "base",
 typeid(Location), "path", "instance", &instance));

 if (instance.position)
 result = path(*instance.position);
 result.push_back(&const_cast<Object&>(instance));
 log.returned(
 base::lister(result, ",", false, true, false, [](Object* content) {
 return content;
 }).c_str());

 return result;
 }

 Location::Location(Location* position, structure attributing, const Log* caller) :
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
 long long unsigned result = 0;
 Log log(method<std::type_index>("", nullptr, "who", typeid(result)));

 log.returned(result = Object::who());

 return result;
 }
 base::Location* Card::where() const {
 base::Location* result = nullptr;
 Log log(method<std::type_index>("", nullptr, "where", typeid(result)));

 log.returned(result = dynamic_cast<Location*>(Object::where()));

 return result;
 }
 time_t Card::when() const {
 time_t result;
 Log log(method<std::type_index>("", nullptr, "when", typeid(result)));

 log.returned(result = Object::when());

 return result;
 }
 void Card::attribute(structure attributing, const Log* caller) {
 Log log(
 method<std::type_index>(nullptr, "attribute", typeid(void), "",
 "attributing", lister(attributing)));

 Object::attribute(attributing, &log);
 }
 base::Object::structure Card::attributes() const {
 structure result;
 Log log(method<std::type_index>("", nullptr, "attributes", typeid(result)));

 log.returned(lister(result = Object::attributes()).c_str());

 return result;
 }
 std::pair<time_t, std::map<std::string, std::pair<std::string, std::string>>>Card::what() {
 std::pair<time_t, std::map<std::string, std::pair<std::string, std::string>>> result;
 Log log(
 method<std::type_index>("", nullptr, "what", typeid(result)));

 result = Object::what();
 log.returned("...");

 return result;
 }
 bool Card::operator ==(const Card& righthand) const {
 bool result = false;
 Log log(
 binary<std::type_index>(nullptr, typeid(result), "==", "game",
 &righthand, ""));

 log.returned(result = Object::operator ==(righthand));

 return result;
 }
 bool Card::operator !=(const Card& righthand) const {
 bool result = false;
 Log log(
 binary<std::type_index>(nullptr, typeid(result), "==", "game",
 &righthand, ""));

 log.returned(result = Object::operator !=(righthand));

 return result;
 }
 base::Object::structure Card::evaluate(structure attributing) const {
 structure result;
 Log log(
 method<std::type_index>(nullptr, "evaluate", typeid(result), "",
 "attributing", lister(attributing).c_str()));
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
 log.returned(lister(result));

 return result;
 }
 base::Object& Card::operator ()() const {
 Object* result = nullptr;
 Log log(method<std::type_index>("", nullptr, "", typeid(Object&)));

 log.returned(result);

 return *result;
 }
 bool Card::facing() const {
 Log log(method<bool>("", nullptr, "facing", !covered));

 return !covered;
 }
 void Card::facing(const Log* caller) {
 Log log(method<void>("", caller, "facing", typeid(void)));

 covered = false;
 modification = std::chrono::system_clock::to_time_t(
 std::chrono::system_clock::now());
 }
 bool Card::covering() const {
 Log log(method<bool>("", nullptr, "covering", covered));

 return covered;
 }
 void Card::covering(const Log* caller) {
 Log log(method<void>("", caller, "covering", typeid(void)));

 covered = true;
 modification = std::chrono::system_clock::to_time_t(
 std::chrono::system_clock::now());
 }
 void Card::flip(const Log* caller) {
 Log log(method<void>("", caller, "flip", typeid(void)));

 covered = !covered;
 modification = std::chrono::system_clock::to_time_t(
 std::chrono::system_clock::now());
 }
 std::unique_ptr<Card> Card::construct(std::unique_ptr<Object>&& cover,
 std::unique_ptr<Object>&& face, bool covered, Location* position,
 structure attributing, const Log* caller) {
 std::unique_ptr<Card> result;
 Log log(
 function<std::type_index>(caller, "", typeid(result), "game",
 typeid(Card), "construct", "cover", cover.get(), "face",
 face.get(), "covered", covered, "position", position,
 "structure", lister(attributing).c_str()));

 result.reset(
 new Card(std::move(cover), std::move(face), covered, position,
 attributing, &log));
 log.returned(result.get());

 return std::move(result);
 }

 Card::Card(std::unique_ptr<Object>&& cover, std::unique_ptr<Object>&& face,
 bool covered, Location* position, structure attributing,
 const Log* caller) :
 Location(position, attributing, caller) {
 std::ostringstream log;

 log << "+arguments: std::unique_ptr<Object>&& cover=" << cover.get()
 << ", std::unique_ptr<Object>&& face=" << face.get()
 << ", bool covered=" << covered;
 message(log.str());
 this->covered = covered;
 insert_front("cover", std::move(cover), this);
 insert_back("face", std::move(face), this);
 }

 //Deck
 long long unsigned Deck::who() const {
 long long unsigned result = 0;
 Log log(method<std::type_index>("", nullptr, "who", typeid(result)));

 log.returned(result = Object::who());

 return result;
 }
 base::Location* Deck::where() const {
 base::Location* result = nullptr;
 Log log(method<std::type_index>("", nullptr, "where", typeid(result)));

 log.returned(result = dynamic_cast<Location*>(Object::where()));

 return result;
 }
 time_t Deck::when() const {
 time_t result;
 Log log(method<std::type_index>("", nullptr, "when", typeid(result)));

 log.returned(result = Object::when());

 return result;
 }
 void Deck::attribute(structure attributing, const Log* caller) {
 Log log(
 method<std::type_index>(nullptr, "attribute", typeid(void), "",
 "attributing", lister(attributing)));

 Object::attribute(attributing, &log);
 }
 base::Object::structure Deck::attributes() const {
 structure result;
 Log log(method<std::type_index>("", nullptr, "attributes", typeid(result)));

 log.returned(lister(result = Object::attributes()).c_str());

 return result;
 }
 std::pair<time_t, std::map<std::string, std::pair<std::string, std::string>>>Deck::what() {
 std::pair<time_t, std::map<std::string, std::pair<std::string, std::string>>> result;
 Log log(
 method<std::type_index>("", nullptr, "what", typeid(result)));

 result = Object::what();
 log.returned("...");

 return result;
 }
 bool Deck::operator ==(const Deck& righthand) const {
 bool result = false;
 Log log(
 binary<std::type_index>(nullptr, typeid(result), "==", "game",
 &righthand, ""));

 log.returned(result = Object::operator ==(righthand));

 return result;
 }
 bool Deck::operator !=(const Deck& righthand) const {
 bool result = false;
 Log log(
 binary<std::type_index>(nullptr, typeid(result), "==", "game",
 &righthand, ""));

 log.returned(result = Object::operator !=(righthand));

 return result;
 }
 size_t Deck::size() const {
 size_t result = 0;
 Log log(method<std::type_index>("", nullptr, "size", typeid(result)));

 log.returned(result = Location::size());

 return result;
 }
 std::unique_ptr<Card> Deck::draw(const Log* caller) {
 std::unique_ptr<Card> result;
 Log log(method<std::type_index>("", caller, "draw", typeid(result)));

 result.reset(dynamic_cast<Card*>(Location::extract(1, &log).release()));
 log.returned(result.get());

 return std::move(result);
 }
 std::unique_ptr<Card> Deck::extract(const Log* caller) {
 std::unique_ptr<Card> result;
 Log log(method<std::type_index>("", caller, "extract", typeid(result)));
 std::default_random_engine generator;
 auto size = Location::size();
 size_t random = std::uniform_int_distribution<size_t>(size ? 1 : 0, size)(
 generator);

 result.reset(
 dynamic_cast<Card*>(Location::extract(random, &log).release()));
 log.returned(result.get());

 return std::move(result);
 }
 std::unique_ptr<Card> Deck::get_bottom(const Log* caller) {
 std::unique_ptr<Card> result;
 Log log(method<std::type_index>("", caller, "get_bottom", typeid(result)));

 result.reset(
 dynamic_cast<Card*>(Location::extract(Location::size(), &log).release()));
 log.returned(result.get());

 return std::move(result);
 }
 void Deck::put_up(std::string name, std::unique_ptr<Card>&& card,
 const Log* caller) {
 Log log(
 method<std::type_index>(caller, "", typeid(void), "put_up", "name",
 name, "card", card.get()));

 Location::insert_front(name, card.release(), &log);
 }
 void Deck::insert(std::string name, std::unique_ptr<Card>&& card,
 const Log* caller) {
 Log log(
 method<std::type_index>(caller, "", typeid(void), "insert", "name",
 name, "card", card.get()));
 std::default_random_engine generator;

 Location::insert(
 std::uniform_int_distribution<size_t>(1, Location::size() + 1)(
 generator), name, card.release(), &log);
 }
 void Deck::put_down(std::string name, std::unique_ptr<Card>&& card,
 base::Log track) {
 Log log(
 method<std::type_index>(caller, "", typeid(void), "put_down",
 "name", name, "card", card.get()));

 Location::insert_back(name, card.release(), &log);
 }
 void Deck::shuffle(base::Log track) {
 std::default_random_engine generator;
 size_t current = 0;
 size_t length = 0;
 std::uniform_int_distribution<size_t> distribution;

 std::clog << track.tracker() << "void game::Deck{" << this
 << "}.shuffle() {" << std::endl;
 length = Location::size();
 distribution.param(
 std::uniform_int_distribution<size_t>(current,
 length ? length - 1 : current).param());
 for (std::string card; length > 0; --length) {
 std::clog << track() << "current="
 << (current = distribution(generator)) << std::endl;
 card = who(*Location::operator [](current));
 Location::insert(0, card, Location::extract(current, track), track);
 }
 std::clog << track() << "}" << std::endl;
 }

 Deck* Deck::construct(std::string label, base::Log track) {
 Deck* result = nullptr;

 std::clog << track.tracker()
 << "game::Deck* game::Deck::construct(std::string label=\"" << label
 << "\") {" << std::endl;
 result = new Deck(label, nullptr, track);
 std::clog << track() << "}=" << result << std::endl;

 return result;
 }

 Deck::Deck(std::string label, Location* position, base::Log track) :
 Location(position, track) {
 name = label;
 ddddd std::clog
 << track.tracker()
 << "game::Deck::Deck(std::string label=\"" << label
 << "\", base::Location* position=" << position << ")="
 << this << std::endl;
 }
 /**/
}
