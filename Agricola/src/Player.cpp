/*
 * Player.cpp
 *
 *  Created on: 25 nov 2018
 *      Author: m1rma
 */

#include "Player.h"

namespace agr {

//Color
template<> std::function<std::ostringstream(const std::set<base::Element*>&)> base::Class<
		std::set<base::Element*>>::printer = print_std__set<Element*>;
template<> std::function<std::ostringstream(const std::vector<Player*>&)> base::Class<
		std::vector<Player*>>::printer = print_std__vector<Player*>;

base::Primitive<bool> Color::operator ==(Color operand) const {
	return base::binary_primitive(*this, __func__, operand,
			which == operand.which);
}
base::Primitive<bool> Color::operator !=(Color operand) const {
	return base::binary_primitive(*this, __func__, operand,
			which != operand.which);
}
std::ostringstream Color::prints() const {
	return std::ostringstream(COLOR(which));
}

Color::Color(const base::Log* caller) :
		Object(caller, __func__) {
	which = Which::No;
}
Color::Color(Which which, const base::Log* caller) :
		Object(caller, __func__) {
	this->which = which;
}
Color::Color(const Color& copy) :
		Object(nullptr, __func__) {
	which = copy.which;
}
Color& Color::operator =(const Color& copy) {
	which = copy.which;
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
base::Class<std::set<base::Element*>> Player::owns(const Log* caller) {
	return as_method(__func__, caller,
			typeid(base::Class<std::set<base::Element*>>)).returns(
			base::Class<std::set<base::Element*>>(pieces));
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

Player::Player(base::Class<std::string> name, Color color, const Log* caller,
		base::Fields attributes) :
		ENSEMBLE(true, base::make_scopes(AGR, __func__), caller, attributes), name(
				name), color(color), pieces( { }) {
	as_constructor<false>(AGR, __func__, caller, name, color, attributes);
}
Player::~Player() {
	as_destructor(AGR, __func__);
}

} /* namespace agr */
