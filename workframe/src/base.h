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
#include <list>
#include <type_traits>
#include <set>
#include <map>////
#include <iostream>
#include <vector>
#include <memory>
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
		Container&& container, Function function, std::string separator = ",",
		bool cascading = false, bool embracing = false,
		bool enumerating = false) {
	std::ostringstream result;
	size_t i = 0;
	auto length = log10(truth(container.size(), (size_t) 1));
	std::string space = cascading ? "\n\t" : " ";

	for (auto content : container) {
		result << space << separator;
		if (enumerating)
			result << std::string(length, ' ') << ++i << ":\t"
					<< function(content);
	}
	result.str(
			"{" + result.str().substr(separator.length())
					+ (cascading ? "\n" : " ") + "}");

	return result.str();
}
template<typename Container> std::string lister(Container&& container,
		std::string separator = ",", bool cascading = false, bool embracing =
				false, bool enumerating = false) {
	std::ostringstream result;
	size_t i = 0;
	auto length = log10(truth(container.size(), (size_t) 1));
	std::string space = cascading ? "\n\t" : " ";

	for (auto content : container) {
		result << space << separator;
		if (enumerating)
			result << std::string(length, ' ') << ++i << ":\t" << content;
	}
	result.str(
			"{" + result.str().substr(separator.length())
					+ (cascading ? "\n" : " ") + "}");

	return result.str();
}

class Log {
	using variable = std::pair<std::pair<
	std::type_index, std::string>, std::string>;
	using list = std::list<variable>;

	std::string legacy;
	long long unsigned track;
	std::string ns;
	bool open;
	variable returning;
	std::string logger;
	static long long unsigned tracker;
	friend class Object;

	static std::string tracking(const Log*);
	static std::string messaging(std::string);
	template<typename Type> static std::string type(Type&& object) {
		std::ostringstream result;

		if (std::is_same<typename std::decay<Type>::type, std::string>::value)
			result << "\"" << object << "\"";
		else
			result << object;

		return result.str();
	}
	template<typename Type, typename Return> static std::type_index returnType(
			Return&& returning) {
		return typeid(Return);
	}
	template<typename Type, typename Return> static std::string returnValue(
			Return&& returning) {
		return "=" + type(returning);
	}
	template<typename Argument, typename ... Arguments> static list arguments(
			std::string name, Argument&& argument, Arguments&& ... rest) {
		list result = arguments(rest ...);

		result.emplace_front(name, typeid(Argument), type(argument));

		return result;
	}
	template<typename Argument, typename ... Arguments> static list parameters(
			Argument&& argument, Arguments&& ... rest) {
		list result = parameters(rest ...);

		result.emplace_front(typeid(Argument), type(argument));

		return result;
	}
	static list arguments();
	static list parameters();

	Log(std::type_index);
	Log(Log&);
	Log(Log&&);
public:
	std::string log() const;
	template<typename Return> void returned(Return&& returning) {
		std::ostringstream result;

		if (this->returning.second == "\"\"")
			result << "\"" << returning << "\"";
		else
			result << returning;
		this->returning.second = result.str();
	}
	void message(std::string) const;
	template<typename Type, typename Return, typename Instance> static Log unary(
			const Log* caller, Return&& returning, std::string operation,
			std::string ns, Instance&& object, std::string message) {
		auto t = returnType<Type>(returning);
		Log result(t);
		auto r = returnValue<Type>(returning);

		result.legacy = tracking(caller);
		if ((result.open = r == " {"))
			result.returning.first = std::make_pair(t, operation);
		if (std::is_same<typename std::decay<Return>::type, std::string>::value)
			result.returning.second = "\"\"";
		if ((result.logger = type(&object)).back() == '}' && ns.empty())
			result.logger = ns + "::" + result.logger;
		result.logger = t.name() + " " + operation + result.logger;
		std::clog << result.log() + r + messaging(message) << std::endl;

		return result;
	}
	template<typename Type, typename Return, typename Lefthand,
			typename Righthand> static Log binary(const Log* caller,
			Return&& returning, std::string lns, Lefthand&& lefthand,
			std::string operation, std::string rns, Righthand&& righthand,
			std::string message) {
		auto t = returnType<Type>(returning);
		Log result(t);
		auto L = type(lefthand);
		auto R = type(righthand);
		auto r = returnValue<Type>(returning);

		result.legacy = tracking(caller);
		if ((result.open = r == " {"))
			result.returning.first = std::make_pair(t, operation);
		if (std::is_same<typename std::decay<Return>::type, std::string>::value)
			result.returning.second = "\"\"";
		if (L.back() == '}' && lns.empty())
			L = lns + "::" + L;
		if (R.back() == '}' && rns.empty())
			R = rns + "::" + R;
		result.logger = t.name() + " " + L + " " + operation + " " + R;
		std::clog << result.log() + r + messaging(message) << std::endl;

		return result;
	}
	template<typename Type, typename Return, typename ... Arguments> static Log function(
			const Log* caller, std::string message, Return&& returning,
			std::string ns, std::type_index object, std::string name,
			std::string argname, Arguments&& ... args) {
		auto t = returnType<Type>(returning);
		Log result(t);
		auto r = returnValue<Type>(returning);
		std::string o;
		auto a = arguments(argname, args ...);

		result.legacy = tracking(caller);
		if (object == typeid(void))
			result.ns = ns;
		else
			o = std::string(object.name()) + "::";
		if ((result.open = r == " {"))
			result.returning.first = std::make_pair(t, name);
		if (std::is_same<typename std::decay<Return>::type, std::string>::value)
			result.returning.second = "\"\"";
		result.logger =
				t.name() + " " + result.ns + "::" + o + name + "("
						+ lister(a,
								[](variable v) {
									return std::string(v.first.first.name()) + " " + v.first.second + "=" + v.second;
								}) + ")";
		std::clog << result.log() + r + messaging(message) << std::endl;

		return result;
	}
	template<typename Type, typename Return, typename ... Parameters> static Log function(
			std::string message, const Log* caller, Return&& returning,
			std::string ns, std::type_index object, std::string name,
			Parameters&& ... params) {
		auto t = returnType<Type>(returning);
		Log result(t);
		auto r = returnValue<Type>(returning);
		std::string o;
		auto p = parameters(params ...);

		result.legacy = tracking(caller);
		if (object == typeid(void))
			result.ns = ns;
		else
			o = std::string(object.name()) + "::";
		if ((result.open = r == " {"))
			result.returning.first = std::make_pair(t, name);
		if (std::is_same<typename std::decay<Return>::type, std::string>::value)
			result.returning.second = "\"\"";
		result.logger =
				std::string(t.name()) + " " + result.ns + "::" + o + name + "("
						+ lister(p,
								[](variable v) {
									return std::string(v.first.first.name()) + " " + v.first.second + "=" + v.second;
								}) + ")";
		std::clog << result.log() + r + messaging(message) << std::endl;

		return result;
	}

	virtual ~Log();
protected:
	template<typename Type, typename Return> Log unary(const Log* caller,
			Return&& returning, std::string operation,
			std::string message) const {
		auto t = returnType<Type>(returning);
		Log result(t);
		auto r = returnValue<Type>(returning);

		result.legacy = tracking(caller);
		if ((result.open = r == " {"))
			result.returning.first = std::make_pair(t, operation);
		if (std::is_same<typename std::decay<Return>::type, std::string>::value)
			result.returning.second = "\"\"";
		result.logger = t.name() + " " + operation + ns + "::" + type(this);
		std::clog << result.log() + r + messaging(message) << std::endl;

		return result;
	}
	template<typename Type, typename Return, typename Righthand> Log binary(
			const Log* caller, Return&& returning, std::string operation,
			std::string ns, Righthand&& righthand, std::string message) const {
		auto t = returnType<Type>(returning);
		Log result(t);
		auto R = type(righthand);
		auto r = returnValue<Type>(returning);

		result.legacy = tracking(caller);
		if ((result.open = r == " {"))
			result.returning.first = std::make_pair(t, operation);
		if (std::is_same<typename std::decay<Return>::type, std::string>::value)
			result.returning.second = "\"\"";
		if (R.back() == '}' && ns.empty())
			R = ns + "::" + R;
		result.logger = std::string(t.name()) + " " + ns + "::" + type(this)
				+ " " + operation + " " + R;
		std::clog << result.log() + r + messaging(message) << std::endl;

		return result;
	}
	template<typename Type, typename Return, typename ... Arguments> Log method(
			const Log* caller, std::string function, Return&& returning,
			std::string message, std::string name, Arguments&& ... args) const {
		auto t = returnType<Type>(returning);
		Log result(t);
		auto r = returnValue<Type>(returning);
		auto a = arguments(name, args ...);

		result.legacy = tracking(caller);
		if ((result.open = r == " {"))
			result.returning.first = std::make_pair(t, function);
		if (std::is_same<typename std::decay<Return>::type, std::string>::value)
			result.returning.second = "\"\"";
		result.logger =
				t.name() + " " + ns + "::" + type(this) + "." + function + "("
						+ lister(a,
								[](variable v) {
									return std::string(v.first.first.name()) + " " + v.first.second + "=" + v.second;
								}) + ")";
		std::clog << result.log() + r + messaging(message) << std::endl;

		return result;
	}
	template<typename Type, typename Return, typename ... Parameters> Log method(
			std::string message, const Log* caller, std::string function,
			Return&& returning, Parameters&& ... params) const {
		auto t = returnType<Type>(returning);
		Log result(t);
		auto r = returnValue<Type>(returning);
		auto p = parameters(params ...);

		result.legacy = tracking(caller);
		if ((result.open = r == " {"))
			result.returning.first = std::make_pair(t, function);
		if (std::is_same<typename std::decay<Return>::type, std::string>::value)
			result.returning.second = "\"\"";
		result.logger = std::string(t.name()) + " " + ns + "::" + type(this)
				+ "." + function + "(" + lister(p, [](variable v) {
					return std::string(v.first.first.name()) + "=" + v.second;
				}) + ")";
		std::clog << result.log() + r + messaging(message) << std::endl;

		return result;
	}

	template<typename ... Arguments> Log(const Log* caller, std::string message,
			bool open, std::string ns, std::string name, Arguments&& ... args) :
			returning(std::make_pair(std::make_pair(typeid(void), ""), "")) {
		auto result = arguments(name, args...);

		legacy = tracking(caller);
		track = ++tracker;
		logger =
				(this->ns = ns) + "::" + type(this) + "::"
						+ typeid(*this).name() + "("
						+ lister(result,
								[](variable v) {
									return std::string(v.first.first.name()) + " " + v.first.second + "=" + v.second;
								}) + ")";
		std::clog
				<< log() + (((this->open = open)) ? " {" : "")
						+ messaging(message) << std::endl;
	}
	template<typename ... Parameters> Log(std::string message,
			const Log* caller, bool open, std::string ns,
			Parameters&& ... params) :
			returning(std::make_pair(std::make_pair(typeid(void), ""), "")) {
		auto result = parameters(params...);

		legacy = tracking(caller);
		track = ++tracker;
		logger = (this->ns = ns) + "::" + type(this) + "::"
				+ typeid(*this).name() + "("
				+ lister(result, [](variable v) {
					return std::string(v.first.first.name()) + "=" + v.second;
				}) + ")";
		std::clog
				<< log() + (((this->open = open)) ? " {" : "")
						+ messaging(message) << std::endl;
	}
};

class Object: public Log {
	time_t creation;
	Object* position;
	static std::set<Object*> everything;
	friend class Location;
protected:
	time_t modification;
	std::map<std::string, std::string> changes;

	Object(Object*, std::map<std::string, std::string>, const Log*);
public:
	std::map<std::string, std::string> attributing;

	time_t when() const;
	Object* where() const;
	long long unsigned who() const;
	std::pair<time_t,
			std::map<std::string, std::pair<std::string, std::string>>>what();
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

class Location: public Object {
	using content = std::pair<std::string, std::unique_ptr<Object>>;
	using container = std::list<content>;

	container containing;

	std::string naming(std::string);
	container::iterator locate(size_t) const;
	container::iterator locate(std::string) const;
	container::iterator locate(const Object&) const;
	void remove(container::const_iterator, Log);
	std::unique_ptr<Object> extract(container::iterator, Log);
protected:
	Location(Location*, std::map<std::string, std::string>, Log*);
public:
	Object* operator [](size_t) const;
	std::map<size_t, Object*> operator ()(std::string) const;
	std::map<size_t, Object*> operator ()(std::type_index) const;
	void insert_front(std::string, std::unique_ptr<Object>&&, Log*);
	void insert(size_t, std::string, std::unique_ptr<Object>&&, Log*);
	void insert_back(std::string, std::unique_ptr<Object>&&, Log*);
	template<typename ObjectDerived, typename ... Arguments> void emplace_front(
			size_t offset, std::string name, Log* track,
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
	template<typename ObjectDerived, typename ... Arguments> void emplace(
			size_t offset, std::string name, Log* track,
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
			std::string name, Log* track, Arguments&& ... arguments) {
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
	bool remove(size_t, Log*);
	bool remove(std::string, Log*);
	bool remove(const Object&, Log*);
	std::unique_ptr<Object> extract(size_t, Log*);
	std::unique_ptr<Object> extract(std::string, Log*);
	std::unique_ptr<Object> extract(const Object&, Log*);
	size_t size() const;

	static size_t who(const Object&);
	static std::string which(const Object&);
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
};
*/
}

#endif /* BASE_H_ */

