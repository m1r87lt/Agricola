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
/*#include <cmath>
 #include <type_traits>
 //#include <list>
 /*#include <vector>////
 #include <memory>
 #include <functional>*/

namespace base {
bool running();
void end();

template<typename Parameter, typename Class> Class nillable(Parameter&& object,
		Class&& otherwise, std::function<bool(Parameter&)> evaluate =
				[](Parameter& evaluated) {
					return evaluated;
				}, std::function<Class(Parameter&)> forward =
				[](Parameter& forwarded) {
					return forwarded;
				}) {
	return evaluate(object) ? forward(object) : std::forward<Class&&>(otherwise);
}
template<typename Class> Class& nullable(Class* object, Class& otherwise) {
	return object ? *object : otherwise;
}

template<typename Type> class Variable {
	std::string name;
	std::string ns;
	std::function<std::string(Type&)> transcoder;
public:
	Type value;

	std::string type() const {
		return ns + typeid(Type).name();
	}
	std::string label() const {
		return name;
	}
	std::string operator ()() const {
		return type() + "{" + transcoder(value) + "}";
	}
	std::string logger() const {
		std::ostringstream result;

		result << type() << " " << label() << "=" << transcoder(value);

		return result.str();
	}
	operator Type() const {
		return std::forward<Type&&>(value);
	}

	Variable(std::string name, std::string ns, Type&& value,
			std::function<std::string(Type&)> transcoder) :
			value(std::forward<Type&&>(value)) {
		this->name = name;
		this->ns = ns + "::";
		this->transcoder = transcoder;
	}
	Variable(std::function<std::string(Type&)> transcoder, std::string name,
			Type&& value) :
			value(std::forward<Type&&>(value)) {
		this->name = name;
		this->transcoder = transcoder;
	}
	Variable(const Variable<Type>& copy) :
			value(std::forward<Type&&>(copy.value)) {
		name = copy.name;
		ns = copy.ns;
		transcoder = copy.transcoder;
	}
	Variable(Variable<Type> && moved) :
			value(std::forward<Type&&>(moved.value)) {
		name = moved.name;
		ns = moved.ns;
		transcoder = moved.transcoder;
	}
};
template<typename Type> Variable<Type> variable(std::string name,
		std::string ns, Type&& value,
		std::function<std::string(Type&)> transcoder = [](Type& value) {
			std::ostringstream result;

			result << value;

			return result.str();
		}) {
	return std::move(
			Variable<Type>(name, ns, std::forward<Type&&>(value), transcoder));
}
template<typename Type> Variable<Type> variable(
		std::function<std::string(Type&)> transcoder, std::string name,
		Type&& value) {
	return std::move(
			Variable<Type>(transcoder, name, std::forward<Type&&>(value)));
}
template<typename Type> Variable<Type> variable(Type&& value, std::string name =
		"") {
	return std::move(Variable<Type>([](Type& value) {
		std::ostringstream result;

		result << value;

		return result.str();
	}, name, std::forward<Type&&>(value)));
}

class Log {
	std::string legacy;
	long long unsigned track;
	bool open;
	std::string ns;
	std::string logging;
	static long long unsigned tracker;
	friend class Object;
	friend Variable<Log&> ;

	std::string tracking() const;
	std::string log() const;
	Variable<Log&> variable() const;
	template<typename Type, typename ... Arguments> static std::string arguments(
			Variable<Type> argument, Arguments&& ... rest) {
		return ", " + argument.logger()
				+ arguments(std::forward<Arguments&& ...>(rest ...));
	}
	static std::string arguments();
	static std::string tracking(const Log*);
	static std::string messaging(std::string);
	static std::string transcoder(Log&);

	Log(const Log*, std::string, bool);
	Log(Log&);
public:
	template<typename Return, typename Argument> static Log unary(
			const Log* caller, std::string operation,
			Variable<Argument> argument, std::string message) {
		Log result(caller, "", true);

		result.logging = typeid(Return).name();
		result.logging += " " + operation + argument();
		std::clog << result.log() << messaging(message) << " {" << std::endl;

		return result;
	}
	template<typename Return, typename Argument> static Return unary(
			const Log* caller, Variable<Return> returning,
			Variable<Argument> argument, std::string message) {
		Log result(caller, "", false);

		result.logging = returning.type() + " " + returning.label()
				+ argument();
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
	template<typename Return, typename Type, typename ... Arguments> static Log function(
			std::string message, const Log* caller, std::string ns,
			std::type_index object, std::string name, Variable<Type> argument,
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
			const Log* caller, Variable<Return> returning, std::string ns,
			std::type_index object, std::string message,
			Variable<Argument> argument, Arguments&& ... rest) {
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
	void message(std::string) const;
	template<typename Return> Return returning(Return&& returned) {
		open = false;
		std::clog << tracking() << "  }=" << returned;

		return std::forward<Return&&>(returned);
	}
	template<typename Return> Return returning(Return&& returned, std::function<std::string(Return&)> logger) {
		open = false;
		std::clog << tracking() << "  }=" << logger(returned);

		return std::forward<Return&&>(returned);
	}

	virtual ~Log();
	Log(Log&&);
protected:
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
		result.logging += " " + variable()() + "." + name + "()";
		std::clog << result.log() << messaging(message) << " {" << std::endl;

		return result;
	}
	template<typename Return, typename Argument, typename ... Arguments> Log method(
			const Log* caller, std::string name, std::string message,
			Variable<Argument> argument, Arguments&& ... rest) const {
		Log result(caller, "", true);

		result.logging = typeid(Return).name();
		result.logging += " " + variable()() + "." + name + "("
				+ argument.logger() + arguments(rest ...) + ")";
		std::clog << result.log() << messaging(message) << " {" << std::endl;

		return result;
	}
	template<typename Return> Return method(std::string message,
			const Log* caller, Variable<Return> returning) const {
		Log result(caller, "", false);

		result.logging = returning.type() + " " + variable()() + "."
				+ returning.label() + "()";
		std::clog << result.log() << messaging(message) << "=" << returning()
				<< std::endl;

		return returning;
	}
	template<typename Return, typename Argument, typename ... Arguments> Return method(
			std::string message, const Log* caller, Variable<Return> returning,
			Variable<Argument> argument, Arguments&& ... rest) const {
		Log result(caller, "", false);

		result.logging = returning.type() + " " + variable()() + "."
				+ returning.label() + "(" + argument.logger()
				+ arguments(rest ...) + ")";
		std::clog << result.log() << messaging(message) << "=" << returning()
				<< std::endl;

		return returning;
	}

	Log(const Log*, std::string, bool, std::string);
	template<typename Argument, typename ... Arguments> Log(const Log* caller,
			std::string ns, bool open, std::string message,
			Variable<Argument> argument, Arguments&& ... rest) {
		legacy = tracking(caller);
		track = ++tracker;
		this->ns = ns;
		logging = variable()() + "::" + typeid(*this).name() + "("
				+ argument.logger()
				+ arguments(std::forward<Arguments&& ...>(rest ...)) + ")";
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

	virtual ~Object();
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

	static std::string lister(const std::map<std::string, std::string>&);
	static std::string changing(modifications);
	friend class Location;
	friend Variable<const std::map<std::string, std::string>&> ;
protected:
	time_t modification;

	Object(Object*, std::map<std::string, std::string>, const Log*);
};
/*
 class Location: public Object {
 using content = std::pair<std::string, std::unique_ptr<Object>>;
 using container = std::list<content>;

 container containing;
 friend Object;

 std::string naming(std::string);
 container::iterator locate(size_t) const;
 std::map<size_t, Location::container::iterator> locate(std::string) const;
 std::map<size_t, Location::container::iterator> locate(
 std::type_index) const;
 std::pair<size_t, Location::container::iterator> locate(
 const Object&) const;
 std::unique_ptr<Object> extract(container::iterator, const Log*);
 void remove(container::const_iterator, const Log*);
 protected:
 Location(Location*, structure, const Log*);
 public:
 Object* operator [](size_t) const;
 std::map<size_t, Object*> operator ()(std::string) const;
 std::map<size_t, Object*> operator ()(std::type_index) const;
 void insert_front(std::string, std::unique_ptr<Object>&&, const Log*);
 void insert(size_t, std::string, std::unique_ptr<Object>&&, const Log*);
 void insert_back(std::string, std::unique_ptr<Object>&&, const Log*);
 template<typename ObjectDerived, typename ... Arguments> void emplace_front(
 std::string name, const Log* caller, Arguments&& ... arguments) {
 insert_front(name,
 std::unique_ptr<Object>(new ObjectDerived(arguments ...)),
 &method<std::type_index>(
 std::string("<") + typeid(ObjectDerived).name() + ">",
 caller, "emplace_front", typeid(void), name,
 arguments...));
 }
 template<typename ObjectDerived, typename ... Arguments> void emplace(
 size_t offset, std::string name, const Log* caller,
 Arguments&& ... arguments) {
 insert(offset, name,
 std::unique_ptr<Object>(new ObjectDerived(arguments ...)),
 &method<std::type_index>(
 std::string("<") + typeid(ObjectDerived).name() + ">",
 caller, "emplace", typeid(void), offset, name,
 arguments...));
 }
 template<typename ObjectDerived, typename ... Arguments> void emplace_back(
 std::string name, const Log* caller, Arguments&& ... arguments) {
 insert_back(name,
 std::unique_ptr<Object>(new ObjectDerived(arguments ...)),
 &method<std::type_index>(
 std::string("<") + typeid(ObjectDerived).name() + ">",
 caller, "emplace_back", typeid(void), name,
 arguments...));
 }
 void remove(size_t, const Log*);
 void remove(std::string, const Log*);
 void remove(std::type_index, const Log*);
 void remove(const Object&, const Log*);
 std::unique_ptr<Object> extract(size_t, const Log*);
 std::unique_ptr<Object> extract(const Object&, const Log*);
 size_t size() const;

 static size_t which(const Object&);
 static std::string who(const Object&);
 static std::vector<Object*> path(const Object&);

 virtual ~Location() = default;
 };
 }
 namespace game {
 class Card: private base::Location {
 bool covered;

 Object& side(bool) const;
 protected:
 Card(std::unique_ptr<Object>&&, std::unique_ptr<Object>&&, bool, Location*,
 structure, const Log*);
 public:
 long long unsigned who() const;
 Location* where() const;
 time_t when() const;
 void attribute(structure, const Log*);
 structure attributes() const;
 std::pair<time_t,
 std::map<std::string, std::pair<std::string, std::string>>>what();
 bool operator ==(const Card&) const;
 bool operator !=(const Card&) const;
 virtual structure evaluate(structure) const;
 Object& operator ()() const;
 bool facing() const;
 void facing(const Log*);
 bool covering() const;
 void covering(const Log*);
 void flip(const Log*);
 static std::unique_ptr<Card> construct(std::unique_ptr<Object>&&,
 std::unique_ptr<Object>&&, bool, Location*, structure, const Log*);

 virtual ~Card() = default;
 };
 class Deck: private base::Location {
 std::string name;
 protected:
 Deck(std::string, Location*, structure, const Log*);
 public:
 long long unsigned who() const;
 Location* where() const;
 time_t when() const;
 structure attributes() const;
 void attribute(structure, const Log*);
 std::pair<time_t,
 std::map<std::string, std::pair<std::string, std::string>>>what();
 bool operator ==(const Deck&) const;
 bool operator !=(const Deck&) const;
 size_t size() const;
 virtual structure evaluate(structure) const;
 const std::string& label() const;
 std::unique_ptr<Card> draw(const Log*);
 std::unique_ptr<Card> extract(const Log*);
 std::unique_ptr<Card> get_bottom(const Log*);
 void put_up(std::string, std::unique_ptr<Card>&&, const Log*);
 void insert(std::string, std::unique_ptr<Card>&&, const Log*);
 void put_down(std::string, std::unique_ptr<Card>&&, const Log*);
 template<typename CardDerived, typename ... Arguments> void emplace_up(
 std::string name, const Log* caller, Arguments&& ... arguments) {
 Log log(
 method<std::type_index>("", caller, "emplace_up", typeid(void),
 name, arguments...));

 Location::emplace_front<CardDerived>(name, &log, arguments ...);
 }
 template<typename CardDerived, typename ... Arguments> void emplace(
 size_t offset, std::string name, const Log* caller,
 Arguments&& ... arguments) {
 Log log(
 method<std::type_index>("", caller, "emplace", typeid(void),
 offset, name, arguments...));

 Location::emplace<CardDerived>(offset, name, &log, arguments ...);
 }
 template<typename CardDerived, typename ... Arguments> void emplace_down(
 std::string name, const Log* caller, Arguments&& ... arguments) {
 Log log(
 method<std::type_index>("", caller, "emplace_down",
 typeid(void), name, arguments...));

 Location::emplace_back<CardDerived>(name, &log, arguments ...);
 }
 void shuffle(const Log*);

 static std::unique_ptr<Deck> construct(std::string, Location*, structure,
 const Log*);
 };/**/
}

#endif /* BASE_H_ */

