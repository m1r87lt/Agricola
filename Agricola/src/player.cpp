/*
 * Player.cpp
 *
 *  Created on: 03 gen 2018
 *      Author: MLaRosa
 */

#include "player.h"/*
#include <chrono>*/

//Color
const std::map<Color::Which, std::string> names = { { Color::Which::No, "No" },
		{ Color::Which::White, "White" }, { Color::Which::Gray, "Gray" }, {
				Color::Which::Black, "Black" }, { Color::Which::Crimson,
				"Crimson" }, { Color::Which::Red, "Red" }, {
				Color::Which::Brown, "Brown" },
		{ Color::Which::Orange, "Orange" }, { Color::Which::Yellow, "Yellow" },
		{ Color::Which::Green, "Green" }, { Color::Which::Blue, "Blue" }, {
				Color::Which::Purple, "Purple" } };

std::string Color::has_name() const {
	return as_method("", nullptr,
			base::Variable<std::string>("has_name", "", name(instance)));
}
Color::operator Color::Which() const {
	return as_unary(nullptr,
			base::Variable<const Which&>("(Color::Which)", "", instance, name),
			"");
}
base::Variable<const Color&> Color::gives_variable(std::string label) const {
	return base::Variable<const Color&>(label, "", *this, transcode);
}
Color Color::which_color(std::string which) {
	Color result;
	auto log = as_function<decltype(result)>(nullptr, "", typeid(Color),
			"which_color", "");

	for (auto n : names)
		if (n.second == which)
			return log.returns<decltype(result)>(Color(n.first, &log),
					transcode);

	return log.returns<decltype(result)&>(result, transcode);
}
std::string Color::transcode(const Color& which) {
	return typeid(Color).name() + std::string("::") + typeid(Which).name()
			+ "::" + name(which.instance);
}
std::string Color::name(const Which& which) {
	return names.find(which)->second;
}

Color::Color() :
		Log(nullptr, "", true, "") {
	as_constructor(nullptr, "", typeid(Color), "");
	instance = Which::No;
}
Color::Color(Which which, const Log* caller) :
		Log(caller, "", true, "",
				base::Variable<decltype(which)&>("which", "", which, name)) {
	as_constructor(nullptr, "", typeid(Color), "",
			base::Variable<decltype(which)&>("which", "", which, name));
	instance = which;
}
Color::Color(std::string which, const Log* caller) :
		Log(caller, "", true, "",
				base::Variable<decltype(which)&>("which", "", which)) {
	as_constructor(nullptr, "", typeid(Color), "",
			base::Variable<decltype(which)&>("which", "", which));
	instance = which_color(which);
}
Color::Color(const Color& copy) :
		Log(nullptr, "", false, "") {
	instance = copy.instance;
}
Color& Color::operator =(const Color& copy) {
	auto log = as_binary<Color&>(nullptr, "=", copy.gives_variable("copy"), "");
	instance = copy.instance;

	return log.returns<Color&>(*this, transcode);
}

//Player
std::vector<std::unique_ptr<Player>> Player::players;

const std::string& Player::has_identity() const {
	return as_method("", nullptr,
			base::Variable<const std::string&>(name, "identity"));
}
short unsigned Player::is_number() const {
	short unsigned result = 0;
	auto log = as_method<short unsigned>(nullptr, "number", "");

	while (players[result - 1].get() != this)
		--result;

	return log.returns(result);
}
const Color& Player::has_color() const {
	return as_method("", nullptr,
			base::Variable<const Color&>("color", "", color, Color::transcode));
}
base::Location* Player::played() const {
	auto log = as_method<Location*>(nullptr, "played", "");

	return log.returns(dynamic_cast<Location*>(operator ()("played").at(0)));
}

Player* Player::player(short unsigned number) {
	Player* result = nullptr;
	auto log = as_function<decltype(result)>(nullptr, "", typeid(Player),
			"player", "",
			base::Variable<decltype(number)&>("number", "", number));

	if (number <= players.size())
		result = players[number - 1].get();
	else {
		std::string message = "WARNING invalid argument: number="
				+ std::to_string(number) + " > player number="
				+ std::to_string(players.size()) + ".";

		log.logs_error(message);
		log.notes(message);
	}

	return log.returns(result);
}
Player* Player::player(std::string name) {
	Player* result = nullptr;
	auto log = as_function<decltype(result)>(nullptr, "", typeid(Player),
			"player", "", base::Variable<decltype(name)&>("name", "std", name));
	auto p = players.begin();
	auto end = players.end();

	while (p != end && p->get()->name != name)
		++p;
	if (p == end) {
		std::string message =
				"WARNING invalid argument: no player exists there with this name.";

		log.logs_error(message);
		log.notes(message);
	} else
		result = p->get();

	return log.returns(result);
}
Player* Player::player(Color color) {
	Player* result = nullptr;
	auto log = as_function<decltype(result)>(nullptr, "", typeid(Player),
			"player", "", color.gives_variable("color"));
	auto p = players.begin();
	auto end = players.end();

	while (p != end && p->get()->color != color)
		++p;
	if (p == end) {
		std::string message =
				"WARNING invalid argument: no player chose this color.";

		log.logs_error(message);
		log.notes(message);
	} else
		result = p->get();

	return log.returns(result);
}
short unsigned Player::quantity() {
	return as_function("", nullptr,
			base::Variable<short unsigned>("quantity", "", players.size()), "",
			typeid(Player));
}
short unsigned Player::construct(std::string name, Color color,
		const Log* caller, std::string message) {
	short unsigned result = players.size();
	auto log = as_function<decltype(result)>(caller, "", typeid(Player),
			"construct", message,
			base::Variable<decltype(name)&>("name", "std", name),
			color.gives_variable("color"));

	if (result++ < 5)
		players.emplace_back(new Player(name, color, &log));
	else {
		message = "ERROR more than 5 players are not possible.";
		log.logs_error(message);
		log.notes(message);

		throw std::out_of_range(message);
	}

	return log.returns(result);
}
void Player::construct(
		std::initializer_list<std::pair<std::string, Color>> players,
		const Log* caller, std::string message) {
	using list = std::list<std::pair<std::string, Color>>;
	auto log =
			as_function<void>(caller, "", typeid(Player), "construct", message,
					base::Variable<list>("players", "std", list(players),
							[](list l) {
								std::string result = "{";

								for (auto pair : l)
								result += "\n\t\"" + pair.first + "\": " + pair.second.has_name() + ",";
								if (result.back() == ',')
								result.back() = '\n';
								else
								result += " ";

								return result + "}";
							}));

	if (players.size() > 0 && players.size() < 6)
		for (auto player : players)
			Player::players.emplace_back(
					new Player(player.first, player.second, caller));
	else {
		message = "ERROR the player number must be between 1 and 5.";
		log.notes(message);
		log.logs_error(message);

		throw std::out_of_range(message);
	}
}

Player::Player(std::string name, Color color, const Log* caller) :
		Location(nullptr, std::map<std::string, std::string>(), caller, "",
				true, ""), color(color) {
	auto log = as_constructor(caller, "", typeid(Player), "",
			base::Variable<decltype(name)&>("name", "std", name),
			base::Variable<decltype(color)&>("color", "", color,
					Color::transcode));

	if (color != Color::Which::White && color != Color::Which::Red
			&& color != Color::Which::Green && color != Color::Which::Blue
			&& color != Color::Which::Purple) {
		std::string message =
				"ERROR invalid arguent: possible player colors only are"
						" 'White'/'Red'/'Green'/'Blue'/'Purple'.";

		log.notes(message);
		log.logs_error(message);

		throw std::domain_error(message);
	}
	this->name = name;
	this->color = color;
	inserts_front("played",
			Location::construct(std::map<std::string, std::string>(), &log, ""),
			&log);
}

//Owned
std::string Owned::transcode(const Owned& owned) {
	return Log::transcode(owned) + "("
			+ std::to_string(owned.owner->is_number()) + ")";
}
Player* Owned::player() const {
	return as_method("", nullptr,
			base::Variable<Player* const &>("player", "", owner));
}

Owned::Owned(short unsigned number, const base::Log* caller) :
		Log(caller, "", true, "",
				base::Variable<decltype(number)&>("number", "", number)) {
	auto log = as_constructor(this, "", typeid(Owned), "",
			base::Variable<decltype(number)&>("number", "", number));

	owner = Player::player(number);
}
Owned::Owned(std::string name, const base::Log* caller) :
		Log(caller, "", true, "",
				base::Variable<decltype(name)&>("name", "", name)) {
	auto log = as_constructor(this, "", typeid(Owned), "",
			base::Variable<decltype(name)&>("name", "", name));

	owner = Player::player(name);
}
Owned::Owned(Color color, const base::Log* caller) :
		Log(caller, "", true, "", color.gives_variable("color")) {
	auto log = as_constructor(this, "", typeid(Owned), "",
			color.gives_variable("color"));

	owner = Player::player(color);
}
Owned::Owned(const Player& player, const base::Log* caller) :
		Log(caller, "", true, "", player.gives_variable("player")) {
	as_constructor(this, "", typeid(Owned), "",
			player.gives_variable("player"));

	owner = const_cast<Player*>(&player);
}
Owned::Owned(const Owned& copy) :
		Log(nullptr, "", false, "") {
	owner = copy.owner;
}
Owned& Owned::operator =(const Owned& copy) {
	auto log = as_binary<Owned&>(nullptr, "=", copy.gives_variable("copy"), "");
	owner = copy.owner;

	return log.returns<Owned&>(*this, transcode);
}

