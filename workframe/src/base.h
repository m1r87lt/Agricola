/*
 * base.h
 *
 *  Created on: 14 dic 2017
 *      Author: m1r
 */

#ifndef BASE_H_
#define BASE_H_

#include <string>
#include <iostream>
#include <vector>
#include <sstream>
#include <set>
#include <utility>
#include <memory>
#include <list>
#include <functional>
#include <map>

namespace base {
bool running();
void end();
class Log {
	bool call;
	long long unsigned track;
	std::string nest;

	static bool calling;
	static long long unsigned tracking;
	static std::string nesting;
	static void parameters();
public:
	std::string tracker() const;
	std::string operator()() const;
	std::string close() const;
	operator const char*() const;
	template<typename Argument, typename ... Arguments> static void parameters(
			Argument&& argument, Arguments&& ... arguments) {
		std::clog << ", " << typeid(Argument).name() << "=" << argument;
		parameters(arguments ...);
	}

	Log();
	~Log();
	Log(const Log&);
};

struct Prompt {
	virtual Prompt* enter(std::string, size_t, unsigned) const = 0;
	virtual std::string field(std::string, unsigned) const = 0;
	virtual void field(std::string, size_t, std::string, unsigned) = 0;

	static std::string address(void*);
	template<typename Container> static std::string list(
			Container&& container) {
		std::ostringstream text;
		auto i = 0;

		for (auto content : container)
			text << ",\n\t" << i++ << ": " << content;
		text.str(
				text.str().empty() ?
						"{ }" : "{" + text.str().substr(1) + "\n}");

		return text.str();
	}

	virtual ~Prompt() = default;
};
class Object: public Prompt {
	time_t creation;
	Object* position;
	friend class Location;
	static std::set<const Object*> everything;
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
};
}

#endif /* BASE_H_ */

