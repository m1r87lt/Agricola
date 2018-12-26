/*
 * Player.cpp
 *
 *  Created on: 25 nov 2018
 *      Author: m1rma
 */

#include "Player.h"

namespace base {

template<> std::function<std::ostringstream(const std::vector<agr::Player*>&)> Class<
		std::vector<agr::Player*>>::printer = print_std__vector<agr::Player*>;

template<> std::function<std::ostringstream(const std::set<agr::Owned*>&)> Class<
		std::set<agr::Owned*>>::printer = print_std__set<agr::Owned*>;

std::ostringstream print_std__vector_std__pair_std__string_agr__color__(
		const std::vector<std::pair<std::string, agr::Color>>& array) {
	std::ostringstream result("{");

	for (auto pair : array)
		result << "\n\t{\"" << pair.first << "\", "
				<< pair.second.prints().str() << "}";
	if (result.str() == "{")
		result << " ";
	else
		result << "\n";
	result << "}";

	return result;
}
template<> std::function<
		std::ostringstream(
				const std::vector<std::pair<std::string, agr::Color>>&)> Class<
		std::vector<std::pair<std::string, agr::Color>>>::printer =
		print_std__vector_std__pair_std__string_agr__color__;

} /* namespace base */

namespace agr {

//Color
base::Primitive<bool> Color::operator ==(Color operand) const {
	return base::binary_primitive(*this, __func__, operand,
			which == operand.which);
}
base::Primitive<bool> Color::operator !=(Color operand) const {
	return base::binary_primitive(*this, __func__, operand,
			which != operand.which);
}
std::ostringstream Color::prints() const {
	return std::ostringstream(name);
}

Color::Color(Which which, char* name, const base::Log* caller) :
		Object(caller, __func__) {
	this->which = which;
	this->name = name;
}
Color::~Color() {
	delete name;
}
Color::Color(const Color& copy) :
		Object(nullptr, __func__) {
	which = copy.which;
	name = copy.name;
}
Color& Color::operator =(const Color& copy) {
	which = copy.which;
	name = copy.name;
	Object::operator =(Color(copy));

	return *this;
}

//Player
base::Class<std::vector<Player*>> Player::players = nullptr;

base::Primitive<short> Player::which_is(const Log* caller) const {
	auto log = as_method(__func__, caller, typeid(base::Primitive<short>));
	base::Primitive<short> result(0, &log);

	while (players.is()[result] != this)
		++result;

	return result;
}
base::Class<std::string> Player::who_is(const Log* caller) const {
	return as_method(__func__, caller, typeid(base::Class<std::string>)).returns(
			base::Class<std::string>(name));
}
Color Player::how_is(const Log* caller) const {
	return as_method(__func__, caller, typeid(Color)).returns(Color(color));
}
base::Class<std::set<Owned*>> Player::owns(const Log* caller) {
	return as_method(__func__, caller, typeid(base::Class<std::set<Owned*>>)).returns(
			base::Class<std::set<Owned*>>(pieces));
}
std::ostringstream Player::prints() const {
	std::ostringstream result(Ensemble::prints());
	std::string print = result.str();

	if (print.back() == '}')
		result.str(print.substr(0, print.length() - 1));
	result << " - Player#" << which_is(this) << " \"" << who_is(this).becomes()
			<< "\"";

	return result;
}
base::Primitive<short> Player::give_number(const Log* caller) {
	return base::Method::return_primitive((short) players.is().size(),
			base::make_scopes(AGR, TYPEID(Player), __func__), caller);
}
Player& Player::give(base::Primitive<short> number, const Log* caller) {
	return as_method(base::make_scopes(AGR, TYPEID(Player), __func__), false,
			caller, typeid(Player&)).returns(*players.is()[--number]);
}
Player& Player::give(base::Class<std::string> name, const Log* caller) {
	auto log = as_method(base::make_scopes(AGR, TYPEID(Player), __func__), true,
			caller, typeid(Player&));
	auto player = players.is().begin();
	auto end = players.is().end();

	while ((*player)->name != name && player != end)
		++player;
	if (player == end) {
		std::string message = "ERROR: no player exists with name \"" + name.is()
				+ "\".";

		log.logs_error(std::ostringstream(message));

		throw std::domain_error(message);
	}

	return log.returns(**player);
}
Player& Player::give(Color color, const Log* caller) {
	auto log = as_method(base::make_scopes(AGR, TYPEID(Player), __func__), true,
			caller, typeid(Player&));
	auto player = players.is().begin();
	auto end = players.is().end();

	while ((*player)->color != color && player != end)
		++player;
	if (player == end) {
		std::string message = "ERROR: no player exists with "
				+ color.prints().str() + ".";

		log.logs_error(std::ostringstream(message));

		throw std::domain_error(message);
	}

	return log.returns(**player);
}
void Player::construct_all(
		base::Class<std::vector<std::pair<std::string, Color>>> players,
		const Log* caller) {
	auto log = as_method(base::make_scopes(AGR, TYPEID(Player), __func__), true,
			caller, typeid(void), players);

	for (auto player : players.is())
		Player::players.is().emplace_back(
				new Player(base::Class<std::string>(player.first, &log),
						player.second, &log));
}

Player::Player(base::Class<std::string> name, Color color, const Log* caller,
		base::Fields attributes) :
		ENSEMBLE(true, base::make_scopes(AGR, __func__), caller, attributes), name(
				name), color(color), pieces( { }) {
	as_constructor<false>(AGR, __func__, caller, name, color, attributes);
}

//Owned
Player& Owned::gives_owner(const Log* caller) {
	return as_method<false>(__func__, caller, typeid(Player&)).returns(
			*(Player*) owner);
}

Owned::Owned(Player& owner, const Log* caller) :
		NEW_LOG(caller, __func__, false), owner(&owner, caller) {
	as_constructor<false>(AGR, __func__, caller, owner);
	owner.pieces.is().emplace(this);
}
Owned::~Owned() {
	as_destructor(AGR, __func__);
	((Player*) owner)->pieces.is().erase(this);
}
Owned::Owned(const Owned& copy) :
		Object(copy), Log(&copy, __func__, false), owner(copy.owner) {
	((Player*) owner)->pieces.is().emplace(this);
}
Owned& Owned::operator =(const Owned& copy) {
	auto log = as_binary(__func__, copy, typeid(Owned&));

	((Player*) (owner = copy.owner))->pieces.is().emplace(this);

	return log.returns(*this);
}
Owned::Owned(Owned&& moving) :
		ASSIGN_LOG(std::move(moving)), owner(moving.owner) {
	((Player*) owner)->pieces.is().emplace(this);
}
Owned& Owned::operator =(Owned&& moving) {
	auto log = as_binary(__func__, moving, typeid(Owned&));

	Log::operator =(std::move(moving));
	((Player*) (owner = moving.owner))->pieces.is().emplace(this);

	return log.returns(*this);
}

} /* namespace agr */

