/*
 * base.h
 *
 *  Created on: 14 dic 2017
 *      Author: m1r
 */

#ifndef BASE_H_
#define BASE_H_

#include <functional>
#include <utility>
#include <string>
#include <sstream>
#include <iostream>
#include <typeindex>
#include <map>
#include <set>
#include <memory>
#include <list>
#include <chrono>
#include <vector>

namespace base {
bool is_running();
void end();

template<typename Parameter, typename Class> Class get_former_if(
		Parameter&& object, Class&& otherwise,
		std::function<bool(Parameter&)> evaluate,
		std::function<Class(Parameter&)> forward) {
	return evaluate(object) ? forward(object) : std::forward<Class&&>(otherwise);
}
template<typename Class> Class& get_former_not_null(Class* object,
		const Class& otherwise) {
	return object ? *object : otherwise;
}

template<typename Class> class Variable {
	std::string name;
	std::string ns;
	std::function<std::string(const Class&)> transcoder;

	static std::string transcodes(const Class& transcoded) {
		std::ostringstream result;

		result << instance;

		return result.str();
	}
public:
	Class instance;

	std::string has_type() const {
		return ns + typeid(Class).name();
	}
	std::string has_label() const {
		return name;
	}
	std::string is() const {
		return has_type() + "{" + transcoder(instance) + "}";
	}
	std::string logs() const {
		std::ostringstream result;

		result << has_type() << " " << has_label() << "="
				<< transcoder(instance);

		return result.str();
	}
	operator Class() const {
		return std::move(instance);
	}
	const Class& operator ()() const {
		return instance;
	}
	Class& operator ()() {
		return instance;
	}

	Variable(std::string name, std::string ns, Class&& assigned,
			std::function<std::string(const Class&)> transcoder) :
			instance(assigned) {
		this->name = name;
		this->ns = ns + "::";
		this->transcoder = transcoder;
	}
	Variable(std::string name, std::string ns, Class&& assigned) :
			instance(assigned) {
		this->name = name;
		this->ns = ns + "::";
		this->transcoder = transcodes;
	}
	Variable(std::string ns, Class&& assigned) :
			instance(assigned) {
		this->ns = ns + "::";
		this->transcoder = transcodes;
	}
	Variable(Class&& assigned) :
			instance(assigned) {
		this->ns = "::";
		this->transcoder = transcodes;
	}
};

class Log {
	std::string legacy;
	long long unsigned track;
	bool open;
	std::string ns;
	std::string logging;
	static long long unsigned tracker;

	friend class Object;

	std::string tracks() const;
	std::string logs() const;
	template<typename Argument, typename ... Rest> static std::string transcodes_arguments(
			Variable<Argument> argument, Rest&& ... rest) {
		return ", " + argument.logger() + transcodes_arguments(rest ...);
	}
	static std::string transcodes_arguments();
	static std::string tracks(const Log*);
	static std::string messages(std::string);
	static std::string transcodes(const Log&);

	Log(const Log*, std::string, bool);
	Log(Log&);
public:
	void notes(std::string) const;
	void logs_error(std::string) const;
	template<typename Return> Return returns(Return&& returning) {
		open = false;
		std::clog << tracks() << "  }=" << returning;

		return std::forward<Return&&>(returning);
	}
	template<typename Return> Return returns(Return&& returning,
			std::function<std::string(const Return&)> logger) {
		open = false;
		std::clog << tracks() << "  }=" << logger(returning);

		return std::forward<Return&&>(returning);
	}
	template<typename Return> std::unique_ptr<Return> returns_refence(
			std::unique_ptr<Return>&& reference) {
		open = false;
		std::clog << tracks() << "  }=" << reference.get();

		return std::move(reference);
	}
	virtual Variable<Log&> get_variable() const;
	template<typename Return, typename Argument> static Log as_unary(
			const Log* caller, std::string operation,
			Variable<Argument> argument, std::string message) {
		Log result(caller, "", true);

		result.logging = typeid(Return).name();
		result.logging += " " + operation + argument.is();
		std::clog << result.logs() << messages(message) << " {" << std::endl;

		return result;
	}
	template<typename Return, typename Argument> static Return as_unary(
			const Log* caller, Variable<Return> returning,
			Variable<Argument> argument, std::string message) {
		Log result(caller, "", false);

		result.logging = returning.has_type() + " " + returning.has_label()
				+ argument.is();
		std::clog << result.log() << messaging(message) << "=" << returning()
				<< std::endl;

		return result;
	}
	template<typename Return, typename Lefthand, typename Righthand> static Log binary(
			const Log* caller, Variable<Lefthand> lefthand,
			std::string operation, Variable<Righthand> righthand,
			std::string message) {
		Log result(caller, "", true);

		result.logging = typeid(Return).name();
		result.logging += " " + lefthand() + " " + operation + " "
				+ righthand();
		std::clog << result.log() << messaging(message) << " {" << std::endl;

		return result;
	}
	template<typename Return, typename Lefthand, typename Righthand> static Return binary(
			const Log* caller, Variable<Return> returning,
			Variable<Lefthand> lefthand, Variable<Righthand> righthand,
			std::string message) {
		Log result(caller, "", false);

		result.logging = returning.type() + " " + lefthand() + " "
				+ returning.label() + " " + righthand();
		std::clog << result.log() << messaging(message) << "=" << returning()
				<< std::endl;

		return returning;
	}
	template<typename Return> static Log function(const Log* caller,
			std::string ns, std::type_index object, std::string name,
			std::string message) {
		Log result(caller, ns, true);

		result.logging = typeid(Return).name();
		result.logging += " " + result.ns + "::";
		if (object != typeid(void))
			result.logging += std::string(object.name()) + "::";
		result.logging += name + "()";
		std::clog << result.log() << messaging(message) << " {" << std::endl;

		return result;
	}
	template<typename Return, typename Argument, typename ... Arguments> static Log function(
			const Log* caller, std::string ns, std::type_index object,
			std::string name, std::string message, Variable<Argument> argument,
			Arguments&& ... rest) {
		Log result(caller, ns, true);

		result.logging = typeid(Return).name();
		result.logging += " " + result.ns + "::";
		if (object != typeid(void))
			result.logging += std::string(object.name()) + "::";
		result.logging += name + "(" + argument.logger() + arguments(rest ...)
				+ ")";
		std::clog << result.log() << messaging(message) << " {" << std::endl;

		return result;
	}
	template<typename Return> static Return function(std::string message,
			const Log* caller, Variable<Return> returning, std::string ns,
			std::type_index object) {
		Log result(caller, ns, false);

		result.logging = returning.type() + " " + result.ns + "::";
		if (object != typeid(void))
			result.logging += std::string(object.name()) + "::";
		result.logging += returning.label() + "()";
		std::clog << result.log() << messaging(message) << "=" << returning()
				<< std::endl;

		return returning;
	}
	template<typename Return, typename Argument, typename ... Arguments> static Return function(
			std::string message, const Log* caller, Variable<Return> returning,
			std::string ns, std::type_index object, Variable<Argument> argument,
			Arguments&& ... rest) {
		Log result(caller, ns, false);

		result.logging = returning.type() + " " + result.ns + "::";
		if (object != typeid(void))
			result.logging += std::string(object.name()) + "::";
		result.logging += returning.label() + "(" + argument.logger()
				+ arguments(rest ...) + ")";
		std::clog << result.log() << messaging(message) << "=" << returning()
				<< std::endl;

		return returning;
	}
	template<typename Return, typename ... Arguments> static Log function(
			std::type_index object, const Log* caller, std::string ns,
			std::string name, std::string message, Arguments&& ... rest) {
		Log result(caller, ns, true);

		result.logging = typeid(Return).name();
		result.logging += " " + result.ns + "::";
		if (object != typeid(void))
			result.logging += std::string(object.name()) + "::";
		result.logging += name + "(";
		+parameters(rest ...) + ")";
		std::clog << result.log() << messaging(message) << " {" << std::endl;

		return result;
	}

	virtual ~Log();
	Log(Log&&);
protected:
	Variable<Log&> variable(std::string) const;
	template<typename Return> Log unary(const Log* caller,
			std::string operation, std::string message) const {
		Log result(caller, "", true);

		result.logging = typeid(Return).name();
		result.logging += " " + operation + variable()();
		std::clog << result.log() << messaging(message) << " {" << std::endl;

		return result;
	}
	template<typename Return> Return unary(const Log* caller,
			Variable<Return> returning, std::string message) const {
		Log result(caller, "", false);

		result.logging = returning.type() + " " + returning.label()
				+ variable()();
		std::clog << result.log() << messaging(message) << "=" << returning()
				<< std::endl;

		return result;
	}
	template<typename Return, typename Righthand> Log binary(const Log* caller,
			std::string operation, Variable<Righthand> righthand,
			std::string message) const {
		Log result(caller, "", true);

		result.logging = typeid(Return).name();
		result.logging += " " + variable()() + " " + operation + " "
				+ righthand();
		std::clog << result.log() << messaging(message) << " {" << std::endl;

		return result;
	}
	template<typename Return, typename Righthand> Return binary(
			const Log* caller, Variable<Return> returning,
			Variable<Righthand> righthand, std::string message) const {
		Log result(caller, "", false);

		result.logging = returning.type() + " " + variable()() + " "
				+ returning.label() + " " + righthand();
		std::clog << result.log() << messaging(message) << "=" << returning()
				<< std::endl;

		return returning;
	}
	template<typename Return> Log method(const Log* caller, std::string name,
			std::string message) const {
		Log result(caller, "", true);

		result.logging = typeid(Return).name();
		result.logging += " " + variable()();
		if (name.size())
			result.logging += ".";
		result.logging += name + "()";
		std::clog << result.log() << messaging(message) << " {" << std::endl;

		return result;
	}
	template<typename Return, typename Argument, typename ... Arguments> Log method(
			const Log* caller, std::string name, std::string message,
			Variable<Argument> argument, Arguments&& ... rest) const {
		Log result(caller, "", true);

		result.logging = typeid(Return).name();
		result.logging += " " + variable()();
		if (name.size())
			result.logging += ".";
		result.logging += name + "(" + argument.logger() + arguments(rest ...)
				+ ")";
		std::clog << result.log() << messaging(message) << " {" << std::endl;

		return result;
	}
	template<typename Return> Return method(std::string message,
			const Log* caller, Variable<Return> returning) const {
		Log result(caller, "", false);

		result.logging = returning.type() + " " + variable()();
		if (returning.label().size())
			result.logging += ".";
		result.logging += returning.label() + "()";
		std::clog << result.log() << messaging(message) << "=" << returning()
				<< std::endl;

		return returning;
	}
	template<typename Return, typename Argument, typename ... Arguments> Return method(
			std::string message, const Log* caller, Variable<Return> returning,
			Variable<Argument> argument, Arguments&& ... rest) const {
		Log result(caller, "", false);

		result.logging = returning.type() + " " + variable()();
		if (returning.label().size())
			result.logging += ".";
		result.logging += returning.label() + "(" + argument.logger()
				+ arguments(rest ...) + ")";
		std::clog << result.log() << messaging(message) << "=" << returning()
				<< std::endl;

		return returning;
	}
	template<typename Return, typename ... Arguments> Log tmethod(
			const Log* caller, std::string name, std::string message,
			Arguments&& ... rest) const {
		Log result(caller, "", true);

		result.logging = typeid(Return).name();
		result.logging += " " + variable()() + "." + name + "("
				+ parameters(rest ...) + ")";
		std::clog << result.log() << messaging(message) << " {" << std::endl;

		return result;
	}
	template<typename Parameter, typename ... Parameters> static std::string parameters(
			Parameter&& parameter, Parameters&& ... rest) {
		std::ostringstream result;

		result << ", " << parameter << parameters(rest ...);

		return result.str();
	}
	static std::string parameters();

	Log(const Log*, std::string, bool, std::string);
	template<typename Argument, typename ... Arguments> Log(const Log* caller,
			std::string ns, bool open, std::string message,
			Variable<Argument> argument, Arguments&& ... rest) {
		legacy = tracking(caller);
		track = ++tracker;
		this->ns = ns;
		logging = variable()() + "::" + typeid(*this).name() + "("
				+ argument.logger() + arguments(rest ...) + ")";
		std::clog << log() << messaging(message)
				<< (((this->open = open)) ? " {" : "") << std::endl;
	}
};

struct Object: public Log {
	using modifications = std::pair<time_t,
	std::map<std::string, std::pair<std::string, std::string>>>;

	long long unsigned who() const;
	Object* where() const;
	time_t when() const;
	std::map<std::string, std::string> attributes() const;
	void attribute(std::map<std::string, std::string>, const Log*);
	modifications what();
	bool operator ==(const Object&) const;
	bool operator !=(const Object&) const;
	static std::set<Object*>& all();
	static std::set<Object*> root();
	static std::unique_ptr<Object> construct(std::map<std::string, std::string>,
			const Log*, std::string);
	static std::string lister(const std::map<std::string, std::string>&);
	static std::string changing(modifications&);
	static std::string objects(std::set<Object*>&);

	virtual ~Object() = default;
	Object(const Object&) = delete;
	Object(Object&&) = delete;
	Object& operator =(const Object&) = delete;
	Object& operator =(Object&&) = delete;
private:
	Object* position;
	time_t creation;
	std::map<std::string, std::string> attributing;
	std::map<std::string, std::string> changes;
	static std::set<Object*> everything;

	friend class Location;
protected:
	time_t modification;

	template<typename ... Arguments> Object(Object* position,
			std::map<std::string, std::string> attributes, const Log* caller,
			std::string ns, bool open, std::string message, Arguments ... rest) :
			Log(caller, ns, open, message, base::variable(position, "position"),
					Variable<decltype(attributes)&>(lister, "attributes",
							attributes), rest ...) {
		modification = creation = std::chrono::system_clock::to_time_t(
				std::chrono::system_clock::now());
		this->position = position;
		attributing = attributes;
		everything.emplace(this);
	}
};

class Location: public Object {
	using content = std::pair<std::string, std::unique_ptr<Object>>;
	using container = std::list<content>;

	container containing;

	std::string naming(std::string);
	container::iterator locate(size_t) const;
	std::map<size_t, container::iterator> locate(std::string) const;
	std::map<size_t, container::iterator> locate(std::type_index) const;
	std::pair<size_t, container::iterator> locate(const Object&) const;
	std::unique_ptr<Object> extract(container::iterator, const Log*);
	void remove(container::const_iterator, const Log*);
	static std::string iterate(container::iterator&);
	static std::string mapper(std::map<size_t, container::iterator>&);
	static std::string locater(std::pair<size_t, container::iterator>&);
protected:
	template<typename ... Arguments> Location(Location* position,
			std::map<std::string, std::string> attributing, const Log* caller,
			std::string ns, bool open, std::string message, Arguments ... rest) :
			Object(position, attributing, caller, ns, open, message, rest ...) {
	}
public:
	Object* operator [](size_t) const;
	std::map<size_t, Object*> operator ()(std::string) const;
	std::map<size_t, Object*> operator ()(std::type_index) const;
	void insert_front(std::string, std::unique_ptr<Object>&&, const Log*);
	void insert(size_t, std::string, std::unique_ptr<Object>&&, const Log*);
	void insert_back(std::string, std::unique_ptr<Object>&&, const Log*);
	template<typename ObjectDerived, typename ... Arguments> void emplace_front(
			std::string name, const Log* caller, Arguments&& ... arguments) {
		auto log = tmethod<void>(caller, "emplace_front", "",
				base::variable(name, "name"), arguments...);

		insert_front(name, new ObjectDerived(arguments ...), &log);
	}
	template<typename ObjectDerived, typename ... Arguments> void emplace(
			size_t offset, std::string name, const Log* caller,
			Arguments&& ... arguments) {
		auto log = tmethod<void>(caller, "emplace", "",
				base::variable(offset, "offset"), base::variable(name, "name"),
				arguments...);

		insert(offset, name, new ObjectDerived(arguments ...), &log);
	}
	template<typename ObjectDerived, typename ... Arguments> void emplace_back(
			std::string name, const Log* caller, Arguments&& ... arguments) {
		auto log = tmethod<void>(caller, "emplace_front", "",
				base::variable(name, "name"), arguments...);

		insert_back(name, new ObjectDerived(arguments ...), &log);
	}
	void remove(size_t, const Log*);
	void remove(std::string, const Log*);
	void remove(std::type_index, const Log*);
	void remove(const Object&, const Log*);
	std::unique_ptr<Object> extract(size_t, const Log*);
	std::unique_ptr<Object> extract(const Object&, const Log*);
	void take(size_t, Location&, size_t, const Log*);
	void take(const Object&, Location&, size_t, const Log*);
	size_t size() const;
	static size_t which(const Object&);
	static std::string who(const Object&);
	static std::vector<Object*> path(const Object&);
	static std::unique_ptr<Location> construct(
			std::map<std::string, std::string>, const Log*, std::string);
	static std::string map_loc(std::map<size_t, Object*>&);

	virtual ~Location() = default;
};
std::string typer(std::type_index&);
}
namespace game {
class Card final: public base::Object {
	bool covered;
	std::unique_ptr<base::Location> container;

	Object& side(bool) const;
protected:
	Card(std::unique_ptr<Object>&&, std::unique_ptr<Object>&&, bool,
			base::Location*, std::map<std::string, std::string>, const Log*,
			std::string);
public:
	Object& operator ()() const;
	bool facing() const;
	void facing(const Log*);
	bool covering() const;
	void covering(const Log*);
	void flip(const Log*);
	static std::unique_ptr<Card> construct(std::unique_ptr<Object>&&,
			std::unique_ptr<Object>&&, bool, std::map<std::string, std::string>,
			const Log*, std::string);
};

class Deck: public base::Object {
	std::string name;
	std::unique_ptr<base::Location> container;
protected:
	Deck(std::string, base::Location*, std::map<std::string, std::string>,
			const Log*, std::string);
public:
	size_t size() const;
	const std::string& label() const;
	std::unique_ptr<Card> draw(const Log*);
	std::unique_ptr<Card> extract(const Log*);
	std::unique_ptr<Card> get_bottom(const Log*);
	void put_up(std::string, std::unique_ptr<Card>&&, const Log*);
	void insert(std::string, std::unique_ptr<Card>&&, const Log*);
	void put_down(std::string, std::unique_ptr<Card>&&, const Log*);
	template<typename CardDerived, typename ... Arguments> void emplace_up(
			std::string name, const Log* caller, Arguments&& ... arguments) {
		auto log = tmethod<void>(caller, "emplace_up", "", name, arguments...);

		container->emplace_front<CardDerived>(name, &log, arguments ...);
	}
	template<typename CardDerived, typename ... Arguments> void emplace(
			size_t offset, std::string name, const Log* caller,
			Arguments&& ... arguments) {
		auto log = method<void>(caller, "emplace", "", offset, name,
				arguments...);

		container->emplace<CardDerived>(offset, name, &log, arguments ...);
	}
	template<typename CardDerived, typename ... Arguments> void emplace_down(
			std::string name, const Log* caller, Arguments&& ... arguments) {
		auto log = method<void>(caller, "emplace_down", "", name, arguments...);

		container->emplace_back<CardDerived>(name, &log, arguments ...);
	}
	void shuffle(const Log*);
	static std::unique_ptr<Deck> construct(std::string,
			std::map<std::string, std::string>, const Log*, std::string);

	virtual ~Deck() = default;
};
}

#endif /* BASE_H_ */

