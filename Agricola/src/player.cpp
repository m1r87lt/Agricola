/*
 * Player.cpp
 *
 *  Created on: 03 gen 2018
 *      Author: MLaRosa
 */

#include "player.h"
#include <chrono>

std::map<Color, std::string>::const_iterator find_color(std::string color) {
	auto c = colorNames.begin();
	auto C = colorNames.end();

	while (c->second != color && c != C)
		++c;

	return c;
}

//Played
std::string Played::what() const {
	std::clog << base::Log().tracker() << "std::string Played{" << this
			<< "}.what()=" << typeid(*this).name() << std::endl;

	return typeid(*this).name();
}
base::Object* Played::enter(std::string content, size_t number,
		unsigned derived) const {
	return Location::enter(content, number, derived);
}
std::string Played::field(std::string variable, unsigned derived) const {
	return Location::field(variable, derived);
}
void Played::field(std::string variable, size_t position, std::string value,
		unsigned derived) {
	Location::field(variable, position, value, derived);
}
Played::Played(base::Log track):
		Location(nullptr, track) {
	std::clog << track.tracker() << "Played::Played()" << std::endl;
}

//Player
std::vector<std::unique_ptr<Player>> Player::players;

base::Object* Player::enter(std::string content, size_t number,
		unsigned derived) const {
	if (content == "players" && number < players.size())
		return players[number].get();
	else
		return Location::enter(content, number, derived);
}
std::string Player::field(std::string variable, unsigned derived) const {
	if (variable == "name")
		return name;
	else if (variable == "c")
		return colorNames.at(c);
	else if (variable == "players")
		return list_unique_ptrs(players);
	else
		return Location::field(variable, derived);
}
void Player::field(std::string variable, size_t position, std::string value,
		unsigned derived) {
	if (variable == "name")
		name = value;
	else if (variable == "c") {
		auto color = find_color(value);
		std::ostringstream result;

		if (color != colorNames.end())
			c = color->first;
		else {
			result << "void Player{" << this
					<< "}.field(std::string variable=\"" << variable
					<< "\", size_t position=" << position
					<< ", std::string value=\"" << value << "\", derived="
					<< derived
					<< ")= ERROR invalid argument: invalid value (color name).";
			std::cerr << result.str() << std::endl;
			throw std::invalid_argument(result.str());
		}
	} else if (variable == "players") {
		auto color = colorNames.begin();
		auto C = colorNames.end();
		std::istringstream command(value);
		std::ostringstream result;

		command >> variable >> value;
		while (color->second != value && color != C)
			++color;
		if (color != C) {
			construct(variable, color->first, base::Log());
		} else {
			result << "void Player{" << this
					<< "}.field(std::string variable=\"" << variable
					<< "\", size_t position=" << position
					<< ", std::string value=\"" << value << "\", derived="
					<< derived
					<< ")= ERROR invalid argument: invalid value (color name).";
			std::cerr << result.str() << std::endl;
			throw std::invalid_argument(result.str());
		}
	} else
		Location::field(variable, position, value, derived);
	modification = std::chrono::system_clock::to_time_t(
			std::chrono::system_clock::now());
}
std::string Player::what() const {
	std::clog << base::Log().tracker() << "std::string Player{" << this
			<< "}.what()=" << typeid(*this).name() << std::endl;

	return typeid(*this).name();
}

const std::string& Player::identity() const {
	std::clog << base::Log().tracker() << "const std::string& Player{" << this
			<< "}.identity()=" << name << std::endl;

	return name;
}
short unsigned Player::number() const {
	auto result = players.size();

	while (players[result - 1].get() != this)
		--result;
	std::clog << base::Log().tracker() << "short unsigned Player{" << this
			<< "}.number()=" << result << std::endl;

	return result;
}
Color Player::color() const {
	std::clog << base::Log().tracker() << "Color Player{" << this
			<< "}.color()=" << colorNames.at(c) << std::endl;

	return c;
}
Played& Player::played() const {
	Played* result = nullptr;
	base::Log track;

	std::clog << track.tracker() << "Played& Player{" << this
			<< "}.played() {" << std::endl;
	result = dynamic_cast<Played*>((*this)("played"));
	std::clog << track() << "}" << result << std::endl;

	return *result;
}

Player* Player::player(short unsigned number) {
	Player* result = nullptr;
	std::ostringstream log;

	log << base::Log().tracker()
			<< "Player* Player::player(short unsigned number=" << number << ")";
	if (number <= players.size())
		result = players[number - 1].get();
	else
		std::cerr << log.str() << "=" << result
				<< " WARNING invalid argument: number > player number="
				<< players.size() << "." << std::endl;
	std::clog << log.str() << "=" << result << std::endl;

	return result;
}
Player* Player::player(std::string name) {
	Player* result = nullptr;
	auto p = players.begin();
	auto end = players.end();
	std::ostringstream log;

	log << base::Log().tracker() << "Player* Player::player(std::string name="
			<< name << ")";
	while (p != end && p->get()->name != name)
		++p;
	if (p == end)
		std::cerr << log.str() << "=" << result
				<< " WARNING invalid argument: no player is there with this name."
				<< std::endl;
	else
		result = p->get();
	std::clog << log.str() << "=" << result << std::endl;

	return result;
}
Player* Player::player(Color color) {
	Player* result = nullptr;
	auto p = players.begin();
	auto end = players.end();
	std::ostringstream log;

	log << base::Log().tracker() << "Player* Player::player(Color color="
			<< colorNames.at(color) << ")";
	while (p != end && p->get()->c != color)
		++p;
	if (p == end)
		std::cerr << log.str() << "=" << result
				<< " WARNING invalid argument: no player chose this color.";
	else
		result = p->get();
	std::clog << log.str() << "=" << result << std::endl;

	return result;
}
short unsigned Player::quantity() {
	std::clog << base::Log().tracker() << "short unsigned Player::quantity()"
			<< "=" << players.size() << std::endl;

	return players.size();
}
short unsigned Player::construct(std::string name, Color color,
		base::Log track) {
	short unsigned result = 0;
	std::ostringstream log;

	log << track.tracker()
			<< "short unsigned Player::construct(std::string name=\"" << name
			<< "\", Color color=" << colorNames.at(color) << ")";
	std::clog << log.str() << " {" << std::endl;
	if (players.size() < 5) {
		players.emplace_back(new Player(name, color, track));
		result = players.size();
	} else {
		std::string message = "= ERROR more than 5 players are not possible.";

		std::clog << track() << "}" << message << std::endl;
		std::clog << log.str() << message << std::endl;
		throw std::out_of_range(log.str() + message);
	}
	std::clog << track() << "}=" << result << std::endl;

	return result;
}
void Player::construct(
		std::initializer_list<std::pair<std::string, Color>> players,
		base::Log track) {
	std::ostringstream argument;
	std::ostringstream log;

	for (auto player : players)
		argument << ",\n\t( " << player.first << "; "
				<< colorNames.at(player.second) << " )";
	argument.str(
			players.size() ? "{" + argument.str().substr(1) + " }" : "{ }");
	log << track.tracker()
			<< "bool Player::construct(std::initializer_list< std::pair"
					"< std::string, Color > > players=" << argument.str()
			<< ")";
	std::clog << log.str() << " {";
	if (players.size() > 0 && players.size() < 6)
		for (auto player : players)
			Player::players.emplace_back(
					new Player(player.first, player.second, track));
	else {
		std::string message =
				"= ERROR the player number must be between 1 and 5.";

		std::clog << track() << "}" << message << std::endl;
		std::cerr << log.str() << message << std::endl;
		throw std::out_of_range(log.str() + message);
	}
	std::clog << track() << "}" << std::endl;
}

Player::Player(std::string name, Color color, base::Log track) :
		Location(nullptr, track) {
	std::ostringstream log;

	log << track.tracker() << "Player::Player(std::string name=\"" << name
			<< "\", Color color=" << colorNames.at(color) << ")";
	std::clog << log.str() << std::endl;
	if (color != Color::White && color != Color::Red && color != Color::Green
			&& color != Color::Blue && color != Color::Purple) {
		std::string message = "= ERROR invalid arguent: player colors are"
				" 'White'/'Red'/'Green'/'Blue'/'Purple'.";

		std::clog << message << std::endl;
		std::cerr << log.str() << message << std::endl;
		throw std::domain_error(log.str() + message);
	}
	this->name = name;
	c = color;
	insert(0, "played", std::unique_ptr<Object>(new Played(track)), track);
	std::clog << "=" << this << std::endl;
}

//Owned
std::string Owned::field(std::string variable, unsigned derived) const {
	if (variable == "owner")
		return std::to_string(owner);
	else {
		std::ostringstream result;

		result << "std::string Owned{" << this
				<< "}.field(std::string variable=\"" << variable
				<< "\", [derived=" << derived
				<< "])= ERROR invalid argument: invalid variable.";
		std::cerr << result.str() << std::endl;
		throw std::invalid_argument(result.str());
	}
}
void Owned::field(std::string variable, size_t position, std::string value,
		unsigned derived) {
	if (variable == "owner")
		owner = std::stoul(value);
	else {
		std::ostringstream result;

		result << "void Owned{" << this << "}.field(std::string variable=\""
				<< variable << "\", size_t position=" << position
				<< ", std::string value=\"" << value << "\", derived="
				<< derived
				<< ")= ERROR invalid argument: invalid value (color name).";
		std::cerr << result.str() << std::endl;
		throw std::invalid_argument(result.str());
	}
}

short unsigned Owned::player() const {
	std::clog << base::Log().tracker() << "short unsigned Owned{" << this
			<< "}.player()=" << owner << std::endl;

	return owner;
}

Owned::Owned(short unsigned player, base::Log track) {
	std::ostringstream log;

	log << track.tracker() << "Owned::Owned(short unsigned player=" << player
			<< ")";
	std::clog << log.str() << std::endl;
	owner = player;
	if (player > Player::quantity()) {
		std::ostringstream message;

		message << "= ERROR invalid argument: player > player number="
				<< Player::quantity() << ".";
		std::clog << message.str() << std::endl;
		std::cerr << log.str() << message.str() << std::endl;
		throw std::domain_error(log.str() + message.str());
	}
	std::clog << "=" << this << std::endl;
}

