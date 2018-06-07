/*
 * base.cpp
 *
 *  Created on: 15 dic 2017
 *      Author: m1r
 */

#define DBUSER "agricola"
#define DBINSTANCE "localhost:1521/xe"

#include "base.h"
#include "include/dpi.h"
#include <stdexcept>
#include <chrono>
#include <fstream>
#include <random>

namespace dpi {
dpiContext* context = nullptr;
dpiConn* conn = nullptr;
dpiErrorInfo errorInfo;
int initialize() {
	int result = dpiContext_create(DPI_MAJOR_VERSION, DPI_MINOR_VERSION,
			&context, &errorInfo);
	const char* mainUserName = DBUSER;
	uint32_t mainUserNameLength = std::char_traits<char>::length(mainUserName);
	const char* mainPassword = DBUSER;
	uint32_t mainPasswordLength = std::char_traits<char>::length(mainPassword);
	const char* connectString = DBINSTANCE;
	uint32_t connectStringLength = std::char_traits<char>::length(
			connectString);

	if (result > 0)
		return dpiConn_create(context, mainUserName, mainUserNameLength,
				mainPassword, mainPasswordLength, connectString,
				connectStringLength, nullptr, nullptr, &conn);
	else
		return result;

}
dpiConn* connection() {
	return conn;
}
int insert_into(std::string table,
		std::forward_list<std::pair<std::string, std::string>> parameters) {
	dpiStmt *stmt = nullptr;
	const char* sql = "insert into " + table + " values(";

	return dpiConn_prepareStmt(connection(), 0, sql, 1, nullptr, 0, &stmt);
}
}

namespace base {
bool run = true;

bool running() {
	return run;
}
void end() {
	run = false;
}

//Log
long long unsigned Log::tracking = 0;

void Log::operator()(list params, std::string message, std::string name) {
	std::ostringstream result;

	if (typeid(*this) != typeid(Log)) {
		result << name << "::" << typeid(*this).name() << "{" << this << "}";
		object = result.str();
		result.clear();
	}
	result << track << ": ";
	if (object.empty())
		result << name;
	else
		result << object << "::" << typeid(*this).name();
	result << "(" << Log::lister(params) << ")";
	logging = result.str();
	if (message.length())
		result << " \"" + message + "\"";
	if (open)
		result << " {";
	std::clog << result.str() << std::endl;
}
void Log::operator ()() const {
//TODO
}
Log::list Log::arguments() {
	return list();
}
Log::list Log::parameters() {
	return list();
}
std::string Log::lister(list params) {
	std::string result;

	for (auto param : params)
		result += ", " + std::get<0>(param) + " " + std::get<0>(param) + "="
				+ std::get<0>(param);
	if (result.length())
		result.erase(0, 2);

	return result;
}
void Log::operator ()(std::string message) const {
//TODO
	std::clog << track << "  " << message << std::endl;
}

Log::~Log() {
//TODO
	std::clog << track << (open ? "  }" : "  ")
			<< (returning.empty() ? "" : "=") << returning << std::endl;
}
/*
 //Object
 std::set<const Object*> Object::everything;

 Object* Object::enter(std::string content, size_t number,
 unsigned derived) const {
 if (content == "position")
 return position;
 else if (content == "everything") {
 if (number < everything.size()) {
 auto e = everything.begin();

 while (number--)
 ++e;

 return const_cast<Object*>(*e);
 } else {
 std::ostringstream result;

 result << "= ERROR invalid argument: number >= size"
 << everything.size() << ".";
 std::cerr << result.str() << std::endl;
 throw std::invalid_argument(result.str());
 }
 } else {
 std::ostringstream result;

 result << "base::Object* base::Object{" << this
 << "}.enter(std::string content=\"" << content << "\", number="
 << number << ", derived=" << derived
 << ")= ERROR input argument: invalid content.";
 std::cerr << result.str() << std::endl;
 throw std::invalid_argument(result.str());
 }
 }
 std::string Object::field(std::string variable, unsigned derived) const {
 if (variable == "creation")
 return ctime(&creation);
 else if (variable == "position")
 return Prompt::address(position);
 else if (variable == "modification")
 return ctime(&modification);
 else if (variable == "everything")
 return list(everything);
 else {
 std::ostringstream result;

 result << "std::string base::Object{" << this
 << "}.field(std::string variable=\"" << variable
 << "\", derived=" << derived
 << ")= ERROR invalid argument: invalid variable.";
 std::cerr << result.str() << std::endl;
 throw std::invalid_argument(result.str());
 }
 }
 void Object::field(std::string variable, size_t position, std::string value,
 unsigned derived) {
 std::istringstream command(value);
 std::ostringstream result;

 if (variable == "position") {
 command >> position;
 this->position = (Object*) position;
 } else if (variable == "everything") {
 if (value == "insert")
 everything.emplace((const Object*) position);
 else if (value == "erase")
 everything.erase((const Object*) position);
 else
 result << "void base::Object{" << this
 << "}.field(std::string variable=\"" << variable
 << "\", size_t position=" << position
 << ", std::string value=\"" << value << "\", derived="
 << derived
 << ")= ERROR invalid argument: invalid value (command).";
 } else
 result << "void base::Object{" << this
 << "}.field(std::string variable=\"" << variable
 << "\", size_t position=" << position
 << ", std::string value=\"" << value << "\", derived="
 << derived << ")= ERROR invalid argument: invalid variable.";
 if (result.good()) {
 std::cerr << result.str() << std::endl;
 throw std::invalid_argument(result.str());
 }
 modification = std::chrono::system_clock::to_time_t(
 std::chrono::system_clock::now());
 }

 time_t Object::when() const {
 std::clog << Log().tracker() << "time_t base::Object{" << this
 << "}.when()=" << ctime(&creation);

 return creation;
 }
 Object* Object::where() const {
 std::clog << Log().tracker() << "Object* base::Object{" << this
 << "}.where()=" << position;

 return position;
 }
 time_t Object::since() const {
 std::clog << Log().tracker() << "time_t base::Object{" << this
 << "}.since()=" << ctime(&modification);

 return modification;
 }
 bool Object::operator ==(const Object& than) const {
 auto result = false;
 Log track;

 std::clog << track.tracker() << "bool base::Object{" << this << "} == "
 << &than << "{" << std::endl;
 result = what() == than.what();
 std::clog << track() << "}=" << (result ? "true" : "false") << std::endl;

 return result;
 }
 bool Object::operator !=(const Object& than) const {
 auto result = false;
 Log track;

 std::clog << track.tracker() << ": bool base::Object{" << this << "} != "
 << &than << "{" << std::endl;
 result = what() != than.what();
 std::clog << track() << "}=" << (result ? "true" : "false") << std::endl;

 return result;
 }

 std::set<const Object*>& Object::all() {
 std::clog << Log().tracker()
 << ": std::set<const base::Object*> base::Object::all()="
 << list(everything) << std::endl;

 return everything;
 }
 std::set<const Object*> Object::root() {
 std::set<const Object*> result;

 std::clog << Log().tracker()
 << ": std::set<const base::Object*> base::Object::root()";
 for (auto object : everything)
 if (!object->position)
 result.insert(object);
 std::clog << "=" << list(result) << std::endl;

 return result;
 }

 Object::Object(Object* location, Log track) {
 modification = creation = std::chrono::system_clock::to_time_t(
 std::chrono::system_clock::now());
 position = location;
 everything.emplace(this);
 std::clog << track.tracker()
 << "base::Object::Object(base::Object location=" << location << ")="
 << this << std::endl;
 }
 Object::~Object() {
 if (run)
 everything.erase(this);
 std::clog << Log().tracker() << "base::Object::~Object(){" << this << "}"
 << std::endl;
 }

 //Location
 std::string Location::list_contained() const {
 auto content = contained.begin();
 std::ostringstream text;
 auto i = 0;
 std::string result;

 for (auto end = contained.end(); content != end; ++content)
 text << ",\n\t" << i++ << ": " << content->second.get();
 result = text.str().empty() ? "{ }" : "{" + text.str().substr(1) + "\n}";

 return result;
 }
 std::string Location::naming(std::string name) const {
 Log track;

 std::clog << track.tracker() << "std::string base::Location{" << this
 << "}.naming(std::string name=\"" << name << "\") {" << std::endl;
 if (name.empty())
 name = "content";
 if (operator ()(name)) {
 auto suffix = 0;
 std::string attempt;

 while (operator ()(attempt = name + "_" + std::to_string(suffix)))
 ++suffix;
 name = attempt;
 }
 std::clog << track() << "}=\"" << name << "\"" << std::endl;

 return name;
 }
 Location::container::iterator Location::locate(size_t offset) const {
 auto result = const_cast<container&>(contained).begin();
 auto end = contained.end();
 size_t index = 0;
 std::ostringstream log;
 Log track;

 log << track.tracker()
 << "base::Location::container::iterator base::Location{" << this
 << "}.locate(size_t offset=" << offset << ")";
 std::clog << log.str() << " {" << std::endl;
 while (result != end && index++ < offset)
 ++result;
 if (result == end) {
 std::ostringstream message;

 message << "=0 WARNING invalid argument: offset >= size="
 << contained.size() << ".";
 std::clog << track() << "}" << message.str() << std::endl;
 std::cerr << log.str() << message.str() << std::endl;
 } else
 std::clog << track() << "}=" << result->second.get() << std::endl;

 return result;
 }
 Location::container::iterator Location::locate(std::string name) const {
 auto result = const_cast<container&>(contained).begin();
 auto end = contained.end();
 std::ostringstream log;
 Log track;

 log << track.tracker()
 << "base::Location::container::iterator base::Location{" << this
 << "}.locate(std::string name=\"" << name << "\")";
 std::clog << log.str() << " {" << std::endl;
 while (result != end && name != result->first)
 ++result;
 if (result == end) {
 std::ostringstream message;

 message << "=0 WARNING no Object exists here with that name.";
 std::clog << track() << "}" << message.str() << std::endl;
 std::cerr << log.str() << message.str() << std::endl;
 } else
 std::clog << track() << "}=" << result->second.get() << std::endl;

 return result;
 }
 Location::container::iterator Location::locate(const Object& instance) const {
 auto result = const_cast<container&>(contained).begin();
 auto end = contained.end();
 std::ostringstream log;
 Log track;

 log << track.tracker()
 << "base::Location::container::iterator base::Location{" << this
 << "}.locate(const Object& instance=" << &instance << ")";
 std::clog << log.str() << " {" << std::endl;
 while (result != end && result->second.get() != &instance)
 ++result;
 if (result == end) {
 std::string message;

 message = "=0 WARNING invalid argument: instance is not located here.";
 std::clog << track() << "}" << message << std::endl;
 std::cerr << log.str() << message << std::endl;
 } else
 std::clog << track() << "}=" << result->second.get() << std::endl;

 return result;
 }
 bool Location::remove(container::const_iterator iterator, Log track) {
 auto result = true;
 std::ostringstream log;
 std::string message;

 log << track.tracker() << "bool base::Location{" << this
 << "}.remove(base::Location::container::iterator iterator=";
 std::clog << log.str();
 if ((result = iterator != contained.end())) {
 std::clog << iterator->second.get();
 contained.erase(iterator);
 modification = std::chrono::system_clock::to_time_t(
 std::chrono::system_clock::now());
 } else {
 std::clog << 0;
 message = " WARNING invalid argument: iterator is out of range.";

 std::cerr << log.str() << std::endl;
 }
 std::clog << ")=" << result << message << std::endl;

 return result;
 }
 std::unique_ptr<Object> Location::extract(container::iterator iterator,
 Log track) {
 std::unique_ptr<Object> result;
 std::ostringstream log;

 log << track.tracker() << "std::unique_ptr<base::Object> base::Location{"
 << this
 << "}.extract(base::Location::container::iterator iterator=";
 std::clog << log.str();
 if (iterator == contained.end()) {
 log << 0 << ")= ERROR invalid argument: iterator is out of range.";

 std::cerr << log.str() << std::endl;
 throw std::invalid_argument(log.str());
 }
 std::clog << iterator->second.get() << ") {";
 iterator->second.swap(result);
 result->position = nullptr;
 contained.erase(iterator);
 modification = result->modification = std::chrono::system_clock::to_time_t(
 std::chrono::system_clock::now());
 std::clog << track() << "}=" << result.get() << std::endl;

 return std::move(result);
 }

 Object* Location::enter(std::string content, size_t number,
 unsigned derived) const {
 if (content == "contained") {
 if (number < contained.size()) {
 auto c = contained.begin();

 while (number--)
 ++c;

 return c->second.get();
 } else {
 std::ostringstream log;

 log << "base::Object* base::Location{" << this
 << "}.enter(std::string content=\"" << content
 << "\", number=" << number << " [derived=" << derived
 << "])= ERROR invalid argument: number >= size"
 << contained.size() << ".";
 std::cerr << log.str() << std::endl;
 throw std::invalid_argument(log.str());
 }
 } else
 return Object::enter(content, number, derived);
 }
 std::string Location::field(std::string variable, unsigned derived) const {
 if (variable == "contained")
 return list_contained();
 else
 return Object::field(variable, derived);
 }
 void Location::field(std::string variable, size_t position, std::string value,
 unsigned derived) {
 std::istringstream command(value);
 std::ostringstream result;

 if (variable == "contained") {
 command >> value;
 if (value == "insert" && position <= contained.size()) {
 command >> value >> variable;
 insert(position, value,
 std::unique_ptr<Object>(
 (Object*) std::stoull(variable, nullptr, 16)),
 Log());
 } else if (value == "extract")
 extract(value, Log());
 else {
 result << "void base::Location{" << this
 << "}.field(std::string variable=\"" << variable
 << "\", size_t position=" << position
 << ", std::string value=\"" << value << "\", derived="
 << derived
 << ")= ERROR invalid argument: invalid value (command).";
 std::cerr << result.str() << std::endl;
 throw std::invalid_argument(result.str());
 }
 } else
 Object::field(variable, position, value, derived);
 modification = std::chrono::system_clock::to_time_t(
 std::chrono::system_clock::now());
 }

 Object* Location::operator [](size_t offset) const {
 container::iterator iterator;
 Object* result = nullptr;
 Log track;

 std::clog << track.tracker() << "base::Object* base::Location{" << this
 << "}[" << offset << "] {" << std::endl;
 if ((iterator = locate(offset)) != contained.end())
 result = iterator->second.get();
 std::clog << track() << "}=" << result << std::endl;

 return result;
 }
 Object* Location::operator ()(std::string name) const {
 container::iterator iterator;
 Object* result = nullptr;
 Log track;

 std::clog << track.tracker() << "base::Object* base::Location{" << this
 << "}(std::string name=\"" << name << "\") {" << std::endl;
 if ((iterator = locate(name)) != contained.end())
 result = iterator->second.get();
 std::clog << track() << "}=" << result << std::endl;

 return result;
 }
 Object* Location::find_former(std::string type) const {
 auto result = contained.begin();
 auto end = contained.end();
 std::ostringstream log;
 Log track;

 log << track.tracker() << "base::Object* base::Location{" << this
 << "}.find(std::string type=\"" << type << "\")";
 std::clog << log.str() << " {" << std::endl;
 while (result != end && type != result->second->what())
 ++result;
 if (result == end) {
 std::ostringstream message;

 message << "=0 WARNING no Object of this type exists here.";
 std::clog << track() << "}" << message.str() << std::endl;
 std::cerr << log.str() << message.str() << std::endl;

 return nullptr;
 } else
 std::clog << track() << "}=" << result->second.get() << std::endl;

 return result->second.get();
 }
 std::list<Object*> Location::find_each(std::string type) const {
 std::list<Object*> result;
 auto content = contained.begin();
 Log track;

 std::clog << track.tracker() << "base::Object* base::Location{" << this
 << "}.find_each(std::string type=" << type << ") {" << std::endl;
 for (auto end = contained.end(); content != end; ++content)
 if(content->second->what() == type)
 result.push_back(content->second.get());
 std::clog << track() << "}=" << Prompt::list(result) << std::endl;

 return result;
 }
 Object* Location::find_former(std::function<bool(Object&)> condition) const {
 auto result = contained.begin();
 auto end = contained.end();
 std::ostringstream log;
 Log track;

 log << track.tracker() << "base::Object* base::Location{" << this
 << "}.find_former(std::function< bool(base::Object&) > condition)";
 std::clog << log.str() << " {" << std::endl;
 while (result != end && !condition(*result->second))
 ++result;
 if (result == end) {
 std::ostringstream message;

 message << "=0 WARNING no Objects verify the condition.";
 std::clog << track() << "}" << message.str() << std::endl;
 std::cerr << log.str() << message.str() << std::endl;

 return nullptr;
 } else
 std::clog << track() << "}=" << result->second.get() << std::endl;

 return result->second.get();
 }
 std::list<Object*> Location::find_each(std::function<bool(Object&)> condition) const {
 std::list<Object*> result;
 auto content = contained.begin();
 Log track;

 std::clog << track.tracker() << "base::Object* base::Location{" << this
 << "}.find_each(std::function< bool(base::Object&) > condition) {" << std::endl;
 for (auto end = contained.end(); content != end; ++content)
 if (condition(*content->second))
 result.push_back(content->second.get());
 std::clog << track() << "}=" << Prompt::list(result) << std::endl;

 return result;
 }
 bool Location::insert(size_t offset, std::string name,
 std::unique_ptr<Object>&& instance, Log track) {
 std::ostringstream log;

 log << track.tracker() << "bool base::Location{" << this
 << "}.insert(size_t offset=" << offset << ", std::string name=\""
 << name << "\", std::unique_ptr<base::Object>&& instance="
 << instance.get() << ")";
 std::clog << log.str() << " {" << std::endl;
 if (instance.get()) {
 if (offset <= contained.size()) {
 auto result = locate(offset);

 result = contained.emplace(result,
 std::make_pair(naming(name), nullptr));
 result->second.swap(instance);
 result->second->position = this;
 modification = result->second->modification =
 std::chrono::system_clock::to_time_t(
 std::chrono::system_clock::now());
 std::clog << track() << "}=true" << std::endl;

 return true;
 } else {
 std::ostringstream message;

 message << "=false WARNING invalid argument: offset >= size="
 << contained.size() << ".";
 std::clog << track() << "}" << message.str() << std::endl;
 std::cerr << log.str() << message.str() << std::endl;

 return false;
 }
 } else {
 std::string message =
 "= ERROR invalid argument: instance cannot be undefined.";

 std::clog << track() << "}" << message << std::endl;
 std::cerr << log.str() << message << std::endl;
 throw std::invalid_argument(log.str() + message);
 }
 }
 void Location::insert_back(std::string name, std::unique_ptr<Object>&& instance,
 Log track) {
 std::clog << track.tracker() << "void base::Location{" << this
 << "}.insert_back(std::string name=\"" << name
 << "\", std::unique_ptr<base::Object>&& instance=" << instance.get()
 << ") {" << std::endl;
 insert(contained.size(), name, std::move(instance), track);
 std::clog << track() << "}" << std::endl;
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
 }
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
 << "}.facing(bool face="
 << face << ")" << std::endl;
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
 std::unique_ptr<Card> result;

 std::clog << track.tracker() << "std::unique_ptr<game::Card> game::Deck{"
 << this << "}.draw() {" << std::endl;
 result.reset(
 dynamic_cast<Card*>(Location::extract((size_t) 0, track).release()));
 std::clog << track() << "}=" << result.get() << std::endl;

 return std::move(result);
 }
 std::unique_ptr<Card> Deck::extract(base::Log track) {
 std::unique_ptr<Card> result;
 std::default_random_engine generator;
 size_t random = 0;

 std::clog << track.tracker() << "std::unique_ptr<game::Card> game::Deck{"
 << this << "}.extract() {" << std::endl;
 std::clog << track() << "random="
 << (random = std::uniform_int_distribution<size_t>(0,
 Location::size() - 1)(generator)) << std::endl;
 result.reset(
 dynamic_cast<Card*>(Location::extract(random, track).release()));
 std::clog << track() << "}=" << result.get() << std::endl;

 return std::move(result);
 }
 std::unique_ptr<Card> Deck::get_bottom(base::Log track) {
 std::unique_ptr<Card> result;

 std::clog << track.tracker() << "std::unique_ptr<game::Card> game::Deck{"
 << this << "}.get_bottom() {" << std::endl;
 result.reset(
 dynamic_cast<Card*>(Location::extract(Location::size() - 1, track).release()));
 std::clog << track() << "}=" << result.get() << std::endl;

 return std::move(result);
 }
 void Deck::put_up(std::string name, std::unique_ptr<Card>&& card,
 base::Log track) {
 std::clog << track.tracker() << "void game::Deck{" << this
 << "}.put_up(std::string name=\"" << name
 << "\", std::unique_ptr< game::Card >&& card=" << card.get()
 << ") {" << std::endl;
 Location::insert(0, name, std::unique_ptr<Object>(card.release()), track);
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
 << (random = std::uniform_int_distribution<size_t>(0,
 Location::size())(generator)) << std::endl;
 Location::insert(random, name, std::unique_ptr<Object>(card.release()),
 track);
 std::clog << track() << "}" << std::endl;
 }
 void Deck::put_down(std::string name, std::unique_ptr<Card>&& card,
 base::Log track) {
 std::clog << track.tracker() << "void game::Deck{" << this
 << "}.put_down(std::string name=\"" << name
 << "\", std::unique_ptr< game::Card >&& card=" << card.get()
 << ") {" << std::endl;
 Location::insert_back(name, std::unique_ptr<Object>(card.release()), track);
 std::clog << track() << "}" << std::endl;
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
 std::clog << track.tracker() << "game::Deck::Deck(std::string label=\""
 << label << "\", base::Location* position=" << position << ")="
 << this << std::endl;
 }*/
}
