/*
 * Player.cpp
 *
 *  Created on: 25 nov 2018
 *      Author: m1r
 */

#include "Player.h"

namespace agr {

//Color
bool Color::operator ==(Color& righthand) const {
	return which == righthand.which;
}
bool Color::operator !=(Color& righthand) const {
	return which != righthand.which;
}

Color::Fields Color::shows() const {
	auto result = Object::shows();

	result.emplace(TYPE(which), label);

	return result;
}
std::string Color::prints() const {
	return NAME(agr::Color)+ "{ " + label + " }";
}

Color::Color() {
	which = Which::No;
	label = TYPE(agr::Color::Which::No);
}
Color::Color(Which which, const char* label) {
	this->which = which;
	this->label = label;
}

//Player
std::vector<Player*> Player::players;

short unsigned Player::which_is() const {
	short unsigned result = 0;

	while (players[result] != this)
		++result;

	return ++result;
}
std::string Player::who_is() const {
	return name;
}
Color Player::how_is() const {
	return color;
}
std::set<base::Object*> Player::owns() {
	return pieces;
}

Player::Fields Player::shows() const {
	auto result = Ensemble::shows();

	result.insert(VARIABLE(name));
	result.insert(VARIABLE(color));
	result.insert(VARIABLE(pieces));

	return result;
}
std::string Player::prints() const {
	std::string result = NAME(agr::Player)+ "#" + std::to_string(which_is()) + ": \"" + who_is()
	+ "\" " + color.prints();

	return result;
}

short unsigned Player::give_number() {
	return players.size();
}
Player& Player::give(short unsigned number) {
	if (number && number <= players.size())
		return *players[--number];
	else
		throw throw_out_of_range_0(number, players.size());
}
Player* Player::give(std::string name) {
	auto player = players.begin();
	auto end = players.end();

	while (player != end && (*player)->name != name)
		++player;
	if (player == end) {
		std::string message = "No player exists with name \"" + name + "\".";

		std::cerr << message << std::endl;

		throw std::domain_error(message);
	}

	return *player;
}
Player* Player::give(Color color) {
	auto player = players.begin();
	auto end = players.end();

	while (player != end && (*player)->color != color)
		++player;
	if (player == end) {
		std::string message = "No player exists with " + color.prints() + ".";

		std::cerr << message << std::endl;

		throw std::domain_error(message);
	}

	return *player;
}
void Player::construct_all(std::vector<std::pair<std::string, Color>> players) {
	for (auto player : players)
		Player::players.emplace_back(new Player(player.first, player.second));
}

Player::Player(std::string name, Color color, Fields attributes) :
		Ensemble(attributes) {
	this->name = name;
	this->color = color;
}

//Owned
Player& Owned::gives_owner() {
	return *owner;
}

Owned::Owned(Player& owner) {
	(this->owner = &owner)->pieces.emplace(this);
}

} /* namespace agr */

