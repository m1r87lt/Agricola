/*
 * management.h
 *
 *  Created on: 06 gen 2018
 *      Author: m1r
 */

#ifndef MANAGEMENT_H_
#define MANAGEMENT_H_

#include "board.h"
#include <array>

std::set<std::string>& keys();

class Stack: public base::Location {
	bool use;
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
	template<typename ObjectDerived, typename ... Arguments> void emplace(
			size_t, std::string, base::Log, Arguments&& ...) = delete;
	bool remove(size_t, base::Log) = delete;
	bool remove(std::string, base::Log) = delete;
	bool remove(const Object&, base::Log) = delete;
	std::unique_ptr<Object> extract(size_t, base::Log) = delete;
	std::unique_ptr<Object> extract(std::string, base::Log) = delete;
	std::unique_ptr<Object> extract(const Object&, base::Log) = delete;
	size_t which(const Object&) const = delete;
	Object* operator ()() const;
	std::unique_ptr<Object> extract(base::Log);
	bool used() const;
	static void clear(base::Log);

	Stack(base::Location*, base::Log);
};

void preparing_to_play(std::vector<std::string>, bool, base::Log track =
		base::Log());

void cards(bool, base::Log track = base::Log());
void a(base::Log track = base::Log());
void b(bool, base::Log track = base::Log());
void c(base::Log track = base::Log());
void d(base::Log track = base::Log());
void e(base::Log track = base::Log());

void starting_player(base::Log track = base::Log());

std::unique_ptr<base::Object> serialize(Data good, short unsigned player = 0,
		base::Log track = base::Log());
void collection(base::Log track = base::Log());
short unsigned first_player();
Farmyard& player_farmyard(Player&);
std::set<Tile*> player_rooms(Player&);
base::Object* waiting_member(short unsigned);
Action* choose_an_action(short unsigned, base::Log track = base::Log());
Wooden* worker(short unsigned, base::Log track = base::Log());
Tile& bedroom(short unsigned);
Farmyard::PersonalSupply& personal_supply(Player&);
Data harvest(short unsigned, base::Log track = base::Log());
std::list<Data> cook(short unsigned, base::Log track = base::Log());
long unsigned feed(short unsigned, base::Log track = base::Log());
std::set<Data> player_animals(Farmyard&, base::Log = base::Log());
std::set<Data> breed(short unsigned, base::Log track = base::Log());
void clear_keys(base::Log track = base::Log());

void end_of_the_game(base::Log track = base::Log());

#endif /* MANAGEMENT_H_ */

