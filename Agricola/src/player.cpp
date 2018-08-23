/*
 * Player.cpp
 *
 *  Created on: 03 gen 2018
 *      Author: MLaRosa
 */

#include "player.h"/*
#include <chrono>*/

//Color
const std::map<Color, std::string> names = { { Color::Which::No, "No" }, {
		Color::Which::White, "White" }, { Color::Which::Gray, "Gray" }, {
		Color::Which::Black, "Black" }, { Color::Which::Crimson, "Crimson" }, {
		Color::Which::Red, "Red" }, { Color::Which::Brown, "Brown" }, {
		Color::Which::Orange, "Orange" }, { Color::Which::Yellow, "Yellow" }, {
		Color::Which::Green, "Green" }, { Color::Which::Blue, "Blue" }, {
		Color::Which::Purple, "Purple" } };

std::string Color::has_name() const {
	return as_method("", nullptr,
			base::Variable<std::string>("has_name", "", name(instance)));
}
Color::operator Color::Which() const {
	return as_unary(nullptr,
			base::Variable<Which>("(Color::Which)", "", instance, transcode), "");
}
base::Variable<Color> Color::gives_variable() const {
	return base::Variable<Color>("", "", *this, transcode);
}
base::Variable<Color> Color::gives_variable(std::string label) const {
	return base::Variable<Color>(label, "", *this, transcode);
}
Color::Which Color::which_color(std::string which) {
	auto log = as_function<Which>(nullptr, "", typeid(Color), "which_color",
			"");

	for (auto n : names)
		if (n.second == which)
			return log.returns(n.first, name);
}
std::string Color::transcode(Color which) {
	return typeid(Color).name() + std::string("::") + name(instance);
}
std::string Color::name(Which which) {
	return names.find(which)->second;
}

Color::Color(Which which) {
	instance = which;
	base::Log::function("", nullptr, variable("::"), "", typeid(Color),
			base::Variable<Which>(naming, "which", which));
}
Color::Color(std::string which) {
	instance = color(which);
	base::Log::function("", nullptr, variable("::"), "", typeid(Color),
			base::variable(which, "which"));
}
/*
//Player
std::vector<std::unique_ptr<Player>> Player::players;

const std::string& Player::identity() const {
	return method<const std::string&>("", nullptr,
			base::variable(name, "identity"));
}
short unsigned Player::number() const {
	auto result = players.size();
	auto log = method<short unsigned>(nullptr, "number", "");

	while (players[result - 1].get() != this)
		--result;

	return log.returning(result);
}
const Color& Player::color() const {
	return method<const Color&>("", nullptr, variable("color"));
}
base::Location* Player::played() const {
	return method<Location*>("", nullptr,
			base::variable(operator ()("played"), "played"));
}

Player* Player::player(short unsigned number) {
	Player* result = nullptr;
	auto log = function<Player*>(nullptr, "", typeid(Player), "player", "",
			base::variable(number, "number"));

	if (number <= players.size())
		result = players[number - 1].get();
	else {
		std::string message = "WARNING invalid argument: number="
				+ std::to_string(number) + " > player number=" + players.size()
				+ ".";

		log.error(message);
		log.message(message);
	}

	return log.returning(result);
}
Player* Player::player(std::string name) {
	Player* result = nullptr;
	auto log = function<Player*>(nullptr, "", typeid(Player), "player", "",
			base::variable(name, "name"));
	auto p = players.begin();
	auto end = players.end();

	while (p != end && p->get()->name != name)
		++p;
	if (p == end) {
		std::string message =
				"WARNING invalid argument: no player exists there with this name.";

		log.error(message);
		log.message(message);
	} else
		result = p->get();

	return log.returning(result);
}
Player* Player::player(Color color) {
	Player* result = nullptr;
	auto log = function<Player*>(nullptr, "", typeid(Player), "player", "",
			color.variable("color"));
	auto p = players.begin();
	auto end = players.end();

	while (p != end && p->get()->colored != color)
		++p;
	if (p == end) {
		std::string message =
				"WARNING invalid argument: no player chose this color.";

		log.error(message);
		log.message(message);
	} else
		result = p->get();

	return log.returning(result);
}
short unsigned Player::quantity() {
	return function<short unsigned>("", nullptr,
			base::variable(players.size(), "quantity"), "", typeid(Player));
}
short unsigned Player::construct(std::string name, Color color,
		const Log& caller, std::string message) {
	short unsigned result = players.size();
	auto log = function<decltype(result)>(caller, "", typeid(Player),
			"construct", "", base::variable(name, "name"),
			color.variable("color"));

	if (result++ < 5)
		players.emplace_back(new Player(name, color, &log, message));
	else {
		std::string message = "ERROR more than 5 players are not possible.";

		log.error(message);
		log.message(message);

		throw std::out_of_range(message);
	}

	return log.returning(result);
}
void Player::construct(
		std::initializer_list<std::pair<std::string, Color>> players,
		const Log& caller, std::string message) {
	auto log =
			function<void>(caller, "", typeid(Player), "construct", "",
					base::Variable < std::list<std::pair<std::string, Color>>
							> ([](std::list<std::pair<std::string, Color>> list) {
								std::string result = "{";

								for (auto pair : list)
								result += "\n\t\"" + pair.first + "\": " + pair.second.name() + ",";
								if (result.back() == ",")
								result.back() = "\n";
								else
								result += " ";

								return result + "}";
							}, "players", std::list<
									std::pair<std::string, Color>>(players)));

	if (players.size() > 0 && players.size() < 6)
		for (auto player : players)
			Player::players.emplace_back(
					new Player(player.first, player.second, caller, message));
	else {
		std::string message = "ERROR the player number must be between 1 and 5.";

		log.message(message);
		log.error(message);

		throw std::out_of_range(message);
	}
}

Player::Player(std::string name, Color color, const Log* caller,
		std::string message) :
		Location(nullptr, std::map<std::string, std::string>(), caller, "",
				true, message) {
	if (color != Color::Which::White && color != Color::Which::Red
			&& color != Color::Which::Green && color != Color::Which::Blue
			&& color != Color::Which::Purple) {
		std::string message =
				"ERROR invalid arguent: possible player colors only are"
						" 'White'/'Red'/'Green'/'Blue'/'Purple'.";

		log.message(message);
		log.error(message);

		throw std::domain_error(message);
	}
	this->name = name;
	colored = color;
	insert_front("played",
			Location::construct(std::map<std::string, std::string>, &log, ""),
			&log);
}

//Owned
std::string Owned::transcode(Owned owned) {
	std::ostringstream result;

	result << typeid(Owned).name() << "{" << owner << "}";

	return result.str();
}
base::Variable<Owned> Owned::variable() const {
	return std::move(base::Variable < Owned > (transcode, "", *this));
}
base::Variable<Owned> Owned::variable(std::string label) const {
	return std::move(base::Variable < Owned > (transcode, label, *this));
}
Player* Owned::player() const {
	return base::Log::function("", nullptr,
			base::variable(owner, transcode(*this) + ".player"), "",
			typeid(void));
}

Owned::Owned(short unsigned number, const base::Log* caller) {
	auto log =
			base::Log::function < Owned
					> (caller, "", typeid(void), "::" + typeid(Owned).name(), "", base::variable(
							number, "number"));

	owner = Player::player(number);
}
Owned::Owned(std::string name, const base::Log* caller) {
	auto log =
			base::Log::function < Owned
					> (caller, "", typeid(void), "::" + typeid(Owned).name(), "", base::variable(
							name, "name"));

	owner = Player::player(name);
}
Owned::Owned(Color color, const base::Log* caller) {
	auto log =
			base::Log::function < Owned
					> (caller, "", typeid(void), "::" + typeid(Owned).name(), "", color.variable(
							"color"));

	owner = Player::player(color);
}
Owned::Owned(const Player& player, const base::Log* caller) {
	auto log =
			base::Log::function < Owned
					> (caller, "", typeid(Owned), typeid(Owned).name(), "", player.variable(
							"player"));

	owner = &player;
}
