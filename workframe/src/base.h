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
#include <cmath>
#include <utility>
#include <typeindex>
#include <forward_list>
#include <type_traits>////
#include <set>
#include <map>
#include <iostream>
#include <vector>
#include <memory>
#include <list>
#include <functional>

namespace base {
bool running();
void end();

template<typename Class> Class nillable(Class* object, Class&& otherwise) {
	return object ? *object : otherwise;
}
template<typename Class> Class truth(Class&& object, Class&& otherwise) {
	return object ? object : otherwise;
}
template<typename Container, typename Function> std::string lister(
		Container&& container, Function function = [](auto content) {
			return content;
		}, std::string separator = ",", bool cascading = false, bool embracing =
				false, bool enumerating = false) {
	std::ostringstream result;
	size_t i = 0;
	auto length = log10(truth(container.size(), (size_t) 1));
	std::string space = cascading ? "\n\t" : " ";

	for (auto content : container) {
		result << space << separator;
		if (enumerating)
			result << std::string(length, ' ') << ++i << ":\t";
		if (function)
			result << function(content);
	}
	result.str(
			"{" + result.str().substr(separator.length())
					+ (cascading ? "\n" : " ") + "}");

	return result.str();
}

class Log {
	using variable = std::pair<std::pair<
	std::type_index, std::string>, std::string>;
	using list = std::forward_list<variable>;

	std::string legacy;
	long long unsigned track;
	std::string ns;
	bool open;
	variable returning;
	static long long unsigned tracker;

	static std::string tracking(const Log*);
	static std::string messaging(std::string);
	template<typename Type> static std::string type(Type&& object) {
		std::ostringstream result;

		if (std::is_pointer<Type>::value && std::is_base_of<Log*, Type>::value)
			result
					<< typeid(std::decay<
									typename std::remove_pointer<Type>::type>::type).name()
					<< "{" << static_cast<Log*>(object)->track << "}";
		else if (std::is_same<typename std::decay<Type>::type, std::string>::value)
			result << "\"" << object << "\"";
		else
			result << object;

		return result.str();
	}
	template<typename Type, typename Return> static std::string returnType(
			Return&& returning) {
		return typeid(Return).name();
	}
	template<std::type_index> static std::string returnType<std::type_index>(
			std::type_index&& returning) {
		return returning.name();
	}
	template<std::type_index&> static std::string returnType<std::type_index>(
			std::type_index& returning) {
		return returning.name();
	}
	template<typename Type, typename Return> static std::string returnValue(
			Return&& returning) {
		return "=" + argument(returning);
	}
	template<std::type_index> static std::string returnValue<std::type_index>(
			std::type_index&& returning) {
		return " {";
	}
	template<std::type_index&> static std::string returnValue<std::type_index>(
			std::type_index& returning) {
		return " {";
	}
	template<typename Argument, typename ... Arguments> static list arguments(
			std::string name, Argument&& argument, Arguments&& ... rest) {
		list result = arguments(rest ...);

		result.emplace_front(name, typeid(Argument), type(argument));

		return result;
	}
	static list arguments();

	Log(const Log&);
	Log(Log&&);
protected:
	template<typename Type, typename Return> void unary(const Log* caller,
			Return&& returning, std::string operation,
			std::string message) const {
		std::clog
				<< tracking(caller) + returnType<Type>(returning) + " "
						+ operation + caller->ns + "::" + type(this)
						+ messaging(message) + returnValue<Type>(returning)
				<< std::endl;
	}
	template<typename Type, typename Return, typename Righthand> void binary(
			const Log* caller, Return&& returning, std::string operation,
			std::string ns, Righthand&& righthand, std::string message) const {
		auto logging = type(righthand);

		if (logging.back() == '}' && ns.empty())
			logging = ns + "::" + logging;
		std::clog
				<< tracking(caller) + returnType<Type>(returning) + " "
						+ caller->ns + "::" + type(this) + operation + logging
						+ messaging(message) + returnValue<Type>(returning)
				<< std::endl;
	}
	template<typename Type, typename Return, typename ... Arguments> void method(
			const Log* caller, std::string message, Return&& returning,
			Arguments&& ... arguments) const {
		list parameters;

		if (std::is_same<typename std::decay<Return>::type, std::string>::value)
			parameters = arguments(returning, arguments ...);
		else
			parameters = arguments("", returning, arguments ...);

		std::clog
				<< tracking(caller) + parameters.front().first.first.name() + " "
						+ caller->ns + "::" + type(this) + operation + logging
						+ messaging(message) + returnValue<Type>(returning)
				<< std::endl;
	}

	template<typename ... Arguments> Log(const Log* caller, std::string message,
			std::string ns, std::string name1, Arguments&& ... args) :
			type(typeid(void)) {
		auto params = arguments(name1, args...);

		this->caller = caller;
		track = ++tracking;
		open = false;
		this->ns = ns;
		param0 = 0;
		logger(typeid(*this), this, typeid(*this).name(), params, message);
	}
	template<typename ... Arguments> Log(std::string message, const Log* caller,
			std::string ns, Arguments&& ... args) :
			type(typeid(void)) {
		auto params = parameters(args...);

		this->caller = caller;
		track = ++tracking;
		open = false;
		this->ns = ns;
		param0 = 0;
		logger(typeid(*this), this, typeid(*this).name(), params, message);
	}
public:
	template<typename Argument> void returned(Argument&& argument) {
		std::forward_list<std::pair<std::type_index, std::string>> values;

		returning = std::get<2>(parameters(argument).front());
		values.emplace_front(typeid(returning), returning);
		values.emplace_front(typeid(param0), std::to_string(param0));
		values.emplace_front(typeid(track), std::to_string(track));
		dpi::log_insert("log_arguments", values);
		param0 = 0;
	}
	void message(std::string) const;
	template<typename Return, typename Instance> static Log unary(
			const Log* caller, std::string message, bool open,
			std::type_index type, Return&& returning, std::string operation,
			std::string ns, Instance&& object) {
		auto params = arguments(returning, object);
		auto r = std::get<2>(params.front());

		params.pop_front();

		return std::forward<Log&&>(
				Log(caller, message, open, type, r, operation, ns,
						std::get<1>(params.front()),
						std::get<2>(params.front())));
	}
	template<typename Return> static Log unary(const Log* caller,
			std::string message, bool open, std::type_index rType,
			Return&& returning, std::string operation, std::string ns,
			std::type_index iType, std::string instance) {
		auto params = arguments(returning);

		return std::forward<Log&&>(
				Log(caller, message, open, rType, std::get<2>(params.front()),
						operation, ns, iType, instance));
	}
	template<typename Return, typename Lefthand, typename Righthand> static Log binary(
			const Log* caller, std::string message, bool open,
			std::type_index type, Return&& returning, std::string lns,
			Lefthand&& lObject, std::string operation, std::string rns,
			Righthand&& rObject) {
		auto params = arguments(returning, lObject, rObject);
		auto r = std::get<2>(params.front());
		std::type_index lt(typeid(void));
		std::string lv;

		params.pop_front();
		lt = std::get<1>(params.front());
		lv = std::get<2>(params.front());
		params.pop_front();

		return std::forward<Log&&>(
				Log(caller, message, open, type, r, lns, lt, lv, operation, rns,
						std::get<1>(params.front()),
						std::get<2>(params.front())));
	}
	template<typename Return> static Log binary(const Log* caller,
			std::string message, bool open, std::type_index type,
			Return&& returning, std::string lns, std::type_index lType,
			std::string lValue, std::string operation, std::string rns,
			std::type_index rType, std::string rValue) {
		auto params = arguments(returning);

		return std::forward<Log&&>(
				Log(caller, message, open, type, std::get<2>(params.front()),
						lns, lType, lValue, operation, rns, rType, rValue));
	}

	template<typename Return, typename ... Arguments> Log(const Log* caller,
			std::string message, bool open, std::type_index type,
			std::string ns, std::type_index object, std::string function,
			Return&& returning, std::string name1, Arguments&& ... args) :
			type(type) {
		auto params = arguments("", returning, name1, args ...);

		this->caller = caller;
		track = ++tracking;
		this->open = open;
		this->returning = std::get<2>(params.front());
		param0 = 0;
		params.pop_front();
		logger(object, nullptr, function, params, message);
	}
	template<typename ... Arguments> Log(std::string message, const Log* caller,
			bool open, std::type_index type, std::string ns,
			std::type_index object, std::string function, Arguments&& ... args) :
			type(type) {
		auto params = parameters(args ...);

		this->caller = caller;
		track = ++tracking;
		this->open = open;
		this->returning = std::get<2>(params.front());
		param0 = 0;
		params.pop_front();
		logger(object, nullptr, function, params, message);
	}
	template<typename Return, typename ... Arguments> Log(const Log* caller,
			std::string message, bool open, std::type_index type,
			std::string ns, std::string function, Return&& returning,
			std::string name1, Arguments&& ... args) :
			type(type) {
		auto params = arguments("", returning, name1, args ...);

		this->caller = caller;
		track = ++tracking;
		this->open = open;
		this->returning = std::get<2>(params.front());
		param0 = 0;
		params.pop_front();
		logger(typeid(void), nullptr, function, params, message);
	}
	template<typename ... Arguments> Log(std::string message, const Log* caller,
			bool open, std::type_index type, std::string ns,
			std::string function, Arguments&& ... args) :
			type(type) {
		auto params = parameters(args ...);

		this->caller = caller;
		track = ++tracking;
		this->open = open;
		this->returning = std::get<2>(params.front());
		param0 = 0;
		params.pop_front();
		logger(typeid(void), nullptr, function, params, message);
	}
	virtual ~Log();
	Log(const Log&) = delete;
	Log(Log&&);
	Log& operator =(const Log&) = delete;
	Log& operator =(Log&&) = delete;
};

class Object: public Log {
	time_t creation;
	Object* position;
	static std::set<Object*> everything;
	friend class Location;
protected:
	time_t modification;
	std::map<std::string, std::string> olds;

	Object(Object*, std::map<std::string, std::string>, const Log*);
public:
	std::map<std::string, std::string> attributing;

	time_t when() const;
	Object* where() const;
	std::pair<time_t,
			std::map<std::string, std::pair<std::string, std::string>>>modifications() const;
	bool operator ==(const Object&) const;
	bool operator !=(const Object&) const;
	static std::set<Object*>& all();
	static std::set<Object*> root();

	virtual ~Object();
	Object(const Object&) = delete;
	Object(Object&&) = delete;
	Object& operator =(const Object&) = delete;
	Object& operator =(Object&&) = delete;
};
/*
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

