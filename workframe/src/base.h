/*
 * base.h
 *
 *  Created on: 14 dic 2017
 *      Author: m1r
 */

#ifndef BASE_H_
#define BASE_H_

#include <string>
#include <sstream>
#include <forward_list>
#include <tuple>
#include <map>
#include <set>
#include <utility>
#include <iostream>
#include <vector>
#include <memory>
#include <list>
#include <functional>

namespace base {
bool running();
void end();

template<typename Class> Class nillable(Class&& object, Class&& otherwise) {
	return std::forward(object ? object : otherwise);
}
template<typename Container> std::string lister(Container& container,
		std::string separator = ",", bool cascading = false, bool embracing =
				false, bool enumerating = false) {
	std::ostringstream result;
	size_t i = 0;
	auto length = log10(nillable(container.size(), 1));
	std::string space = cascading ? "\n\t" : " ";

	for (auto content : container) {
		result << space << separator;
		if (enumerating)
			result << std::string(length, ' ') << ++i << ":\t";
		result << content;
	}
	result.str(
			"{" + result.str().substr(separator.length())
					+ (cascading ? "\n" : " ") + "}");

	return result.str();
}
template<typename Container> std::string textual(Container& container,
		std::string separator = ",", bool cascading = false, bool embracing =
				false, bool enumerating = false) {
	std::ostringstream result;
	size_t i = 0;
	auto length = log10(nillable(container.size(), 1));
	std::string space = cascading ? "\n\t" : " ";

	for (auto content : container) {
		result << space << separator;
		if (enumerating)
			result << std::string(length, ' ') << ++i << ":\t";
		result << (std::string) content;
	}
	result.str(
			"{" + result.str().substr(separator.length())
					+ (cascading ? "\n" : " ") + "}");

	return result.str();
}

class Log {
	using list = std::forward_list<
	std::tuple<std::string, std::string, std::string>>;

	Log* caller;
	std::string ns;
	std::string object;
	long long unsigned track;
	static long long unsigned tracking;

	void initialize(Log*, std::string, std::string) {
		std::ostringstream object;

		this->caller = caller;
		if (typeid(*this) != typeid(Log)) {
			object << typeid(*this).name() << "{" << this << "}";
			this->object = object.str();
		}
		tracker = ++tracking;
	}
	void log() const;
	template<typename Argument, typename ... Arguments> static list arguments(
			std::string name, Argument& argument, Arguments& ... rest) {
		std::ostringstream text;
		auto result = arguments(rest ...);

		text << argument;
		result.push_front(
				std::make_tuple(std::string(typeid(Argument).name()), name,
						text.str()));

		return result;
	}
	template<typename Parameter, typename ... Parameters> static list parameters(
			Parameter& parameter, Parameters& ... rest) {
		std::ostringstream text;
		auto result = parameters(rest ...);

		text << parameter;
		result.push_front(
				std::make_tuple(std::string(typeid(Parameter).name()), "",
						parameter.str()));

		return result;
	}
	static list arguments();
	static list parameters();
	static std::string lister(list);
protected:
	template<typename ... Arguments> Log(std::string message, bool open,
			Log* caller, std::string ns, bool templating, Arguments& ... args) {
		list params;

		initialize(caller, ns, "");
		if (templating)
			params = parameters(args ...);
		else
			params = arguments(args ...);
		std::clog << track << ": " << ns << "::" << object << "::"
				<< typeid(*this).name() << "(" << Log::lister(params)
				<< (open ? ") {" : ")");
	}
public:
	template<typename ... Arguments> Log method(std::string message, bool open,
			std::string function, bool templating, Arguments& ... args) const {
		Log called(function, message, this, ns, templating, args ...);

		if (templating)
			params = parameters(args ...);
		else
			params = arguments(args ...);
		std::clog << track << "->" << ": " << ns << "::" << object << "::"
				<< typeid(*this).name() << "(" << Log::lister(params)
				<< (open ? ") {" : ")");
	}
	template<typename Argument> void returned(Argument& argument) const;
	void returned() const;

	template<typename ... Arguments> Log(std::string function,
			std::string message, bool open, Log* caller, std::string ns,
			bool templating, Arguments& ... arguments) {
		auto params = parameters(arguments ...);

		initialize(caller, ns, function);
	}
	~Log();
	Log(const Log&);
};
/*
 struct Prompt {

 static std::string address(void*);

 virtual ~Prompt() = default;
 };
 class Object {
 std::map<std::string, std::string> attributing;
 time_t creation;
 Object* position;
 friend class Location;
 static std::set<Object*> everything;
 protected:
 time_t modification;

 Object(Object*, Log);
 public:
 virtual Object* enter(std::string, size_t, unsigned) const;
 virtual std::string field(std::string, unsigned) const;
 virtual void field(std::string, size_t, std::string, unsigned);
 virtual std::string what() const = 0;

 time_t when() const;
 Object* where() const;
 time_t since() const;
 bool operator ==(const Object&) const;
 bool operator !=(const Object&) const;

 static std::set<const Object*>& all();
 static std::set<const Object*> root();

 virtual ~Object();
 Object(const Object&) = delete;
 Object(Object&&) = delete;
 Object& operator =(const Object&) = delete;
 Object& operator =(Object&&) = delete;
 };
 class Location: public Object {
 using content = std::pair<std::string, std::unique_ptr<Object>>;
 using container = std::list<content>;
 friend Object;
 container contained;

 std::string list_contained() const;
 std::string naming(std::string) const;
 container::iterator locate(size_t) const;
 container::iterator locate(std::string) const;
 container::iterator locate(const Object&) const;
 bool remove(container::const_iterator, Log);
 std::unique_ptr<Object> extract(container::iterator, Log);
 protected:
 Location(Location*, Log);
 public:
 virtual Object* enter(std::string, size_t, unsigned) const;
 virtual std::string field(std::string, unsigned) const;
 virtual void field(std::string, size_t, std::string, unsigned);

 Object* operator [](size_t) const;
 Object* operator ()(std::string) const;
 Object* find_former(std::string) const;
 std::list<Object*> find_each(std::string) const;
 Object* find_former(std::function<bool(Object&)>) const;
 std::list<Object*> find_each(std::function<bool(Object&)>) const;
 bool insert(size_t, std::string, std::unique_ptr<Object>&&, Log);
 void insert_back(std::string, std::unique_ptr<Object>&&, Log);
 template<typename ObjectDerived, typename ... Arguments> void emplace(
 size_t offset, std::string name, Log track,
 Arguments&& ... arguments) {
 std::clog << track.tracker()
 << "void base::Location::emplace(size_t offset=" << offset
 << ", std::string name=\"" << name << "\"";
 Log::parameters(arguments ...);
 std::clog << ") {" << std::endl;
 insert(offset, name,
 std::unique_ptr<Object>(new ObjectDerived(arguments ...)),
 track);
 std::clog << track() << "}" << std::endl;
 }
 template<typename ObjectDerived, typename ... Arguments> void emplace_back(
 std::string name, Log track, Arguments&& ... arguments) {
 std::clog << track.tracker()
 << "void base::Location::emplace_back(std::string name=\""
 << name << "\"";
 Log::parameters(arguments ...);
 std::clog << ") {" << std::endl;
 insert_back(name,
 std::unique_ptr<Object>(new ObjectDerived(arguments ...)),
 track);
 std::clog << track() << "}" << std::endl;
 }
 bool remove(size_t, Log);
 bool remove(std::string, Log);
 bool remove(const Object&, Log);
 std::unique_ptr<Object> extract(size_t, Log);
 std::unique_ptr<Object> extract(std::string, Log);
 std::unique_ptr<Object> extract(const Object&, Log);
 size_t size() const;
 size_t which(const Object&) const;

 static std::string who(const Object&);
 static std::list<const Object*> path(const Object&);

 virtual ~Location() = default;
 };

 std::map<std::string, std::string> datafile(std::string, std::string,
 Log track = Log());
 }

 namespace game {
 class Card: public base::Location {
 bool covered;
 protected:
 base::Location* operator ()(bool) const;

 Card(std::unique_ptr<base::Location>&&, std::unique_ptr<base::Location>&&,
 base::Location*, base::Log);
 public:
 virtual Object* enter(std::string, size_t, unsigned) const;
 virtual std::string field(std::string, unsigned) const;
 virtual void field(std::string, size_t, std::string, unsigned);

 Object* operator [](size_t) const = delete;
 Object* operator ()(std::string) const = delete;
 Object* find_former(std::string) const = delete;
 std::list<Object*> find_each(std::string) const = delete;
 Object* find_former(std::function<bool(Object&)>) const = delete;
 std::list<Object*> find_each(std::function<bool(Object&)>) const = delete;
 bool insert(size_t, std::string, std::unique_ptr<Object>&&, base::Log) = delete;
 void insert_back(std::string, std::unique_ptr<Object>&&, base::Log) = delete;
 template<typename ObjectDerived, typename ... Arguments> void emplace(
 size_t, std::string, base::Log, Arguments&& ...) = delete;
 template<typename ObjectDerived, typename ... Arguments> void emplace_back(
 std::string, base::Log, Arguments&& ...) = delete;
 bool remove(size_t, base::Log) = delete;
 bool remove(std::string, base::Log) = delete;
 bool remove(const Object&, base::Log) = delete;
 std::unique_ptr<Object> extract(size_t, base::Log) = delete;
 std::unique_ptr<Object> extract(std::string, base::Log) = delete;
 std::unique_ptr<Object> extract(const Object&, base::Log) = delete;
 size_t size() const = delete;
 size_t which(const Object&) const = delete;
 bool facing() const;
 void facing(bool, base::Log);
 base::Location& operator ()() const;
 bool flip(base::Log);
 };
 class Deck: public base::Location {
 std::string name;
 protected:
 Deck(std::string, Location*, base::Log);
 public:
 virtual Object* enter(std::string, size_t, unsigned) const;
 virtual std::string field(std::string, unsigned) const;
 virtual void field(std::string, size_t, std::string, unsigned);
 virtual std::string what() const;

 Object* operator [](size_t) const = delete;
 Object* operator ()(std::string) const = delete;
 Object* find_former(std::string) const = delete;
 std::list<Object*> find_each(std::string) const = delete;
 Object* find_former(std::function<bool(Object&)>) const = delete;
 std::list<Object*> find_each(std::function<bool(Object&)>) const = delete;
 bool insert(size_t, std::string, std::unique_ptr<Object>&&, base::Log) = delete;
 void insert_back(std::string, std::unique_ptr<Object>&&, base::Log) = delete;
 template<typename ObjectDerived, typename ... Arguments> void emplace(
 size_t, std::string, base::Log, Arguments&& ...) = delete;
 template<typename ObjectDerived, typename ... Arguments> void emplace_back(
 std::string, base::Log, Arguments&& ...) = delete;
 bool remove(size_t, base::Log) = delete;
 bool remove(std::string, base::Log) = delete;
 bool remove(const Object&, base::Log) = delete;
 std::unique_ptr<Object> extract(size_t, base::Log) = delete;
 std::unique_ptr<Object> extract(std::string, base::Log) = delete;
 std::unique_ptr<Object> extract(const Object&, base::Log) = delete;
 size_t which(const Object*) const = delete;
 const std::string& label() const;
 std::unique_ptr<Card> draw(base::Log);
 std::unique_ptr<Card> extract(base::Log);
 std::unique_ptr<Card> get_bottom(base::Log);
 void put_up(std::string, std::unique_ptr<Card>&&, base::Log);
 void insert(std::string, std::unique_ptr<Card>&&, base::Log);
 void put_down(std::string, std::unique_ptr<Card>&&, base::Log);
 template<typename CardDerived, typename ... Arguments> void emplace_up(
 std::string name, base::Log track, Arguments&& ... arguments) {
 std::clog << track.tracker() << "void game::Deck::emplace_up< "
 << typeid(CardDerived).name() << " >(std::string name=" << name;
 base::Log::parameters(arguments ...);
 std::clog << ") {" << std::endl;
 Location::emplace(0, name, track, arguments ...);
 std::clog << track() << " }" << std::endl;
 }
 template<typename CardDerived, typename ... Arguments> void emplace(
 std::string name, base::Log track, Arguments&& ... arguments) {
 std::clog << track.tracker() << "void game::Deck::emplace< "
 << typeid(CardDerived).name() << " >(std::string name=" << name;
 base::Log::parameters(arguments ...);
 std::clog << ") {" << std::endl;
 insert(name, std::unique_ptr<Card>(new CardDerived(arguments ...)),
 track);
 std::clog << track() << "}" << std::endl;
 }
 template<typename CardDerived, typename ... Arguments> void emplace_down(
 std::string name, base::Log track, Arguments&& ... arguments) {
 std::clog << track.tracker() << "void game::Deck::emplace_down< "
 << typeid(CardDerived).name() << " >(std::string name=" << name;
 base::Log::parameters(arguments ...);
 std::clog << ") {" << std::endl;
 Location::emplace_back(name, track, arguments ...);
 std::clog << track() << "}" << std::endl;
 }
 void shuffle(base::Log);

 static Deck* construct(std::string, base::Log);
 };*/
}

#endif /* BASE_H_ */

