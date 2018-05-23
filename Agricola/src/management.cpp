/*
 * management.cpp
 *
 *  Created on: 06 gen 2018
 *      Author: m1r
 */

#include "management.h"
#include <src/ui.h>
#include <algorithm>
#include <random>
#define CARDS 365
#define M_HARVEST "harvesting"
#define M_NEWBORN "newborn"
#define M_STALL "stall"

unsigned foodPerPerson = 2;
std::set<std::string> KEYS;
std::set<std::string>& keys() {
	std::clog << base::Log().tracker() << "unsigned keys()="
			<< base::Prompt::list(KEYS) << std::endl;

	return KEYS;
}
std::set<std::string>::iterator find_key_containing(std::string text,
		bool beginning) {
	auto k = KEYS.begin();
	auto K = KEYS.end();
	base::Log track;

	std::clog << track.tracker() << "std::set<std::string>::iterator "
			"find_key_containing(std::string text=\"" << text
			<< "\", bool beginning=" << beginning << ") {"
			<< std::endl;
	while (k != K) {
		auto p = k->find(text);

		if (p < (beginning ? 1 : std::string::npos))
			K = k;
		else
			++k;
	}
	std::clog << track() << "}=" << *k << std::endl;

	return k;
}

//Stack
base::Object* Stack::enter(std::string content, size_t number,
		unsigned derived) const {
	return Location::enter(content, number, derived);
}
std::string Stack::field(std::string variable, unsigned derived) const {
	return Location::field(variable, derived);
}
void Stack::field(std::string variable, size_t position, std::string value,
		unsigned derived) {
	Location::field(variable, position, value, derived);
}
std::string Stack::what() const {
	std::clog << base::Log().tracker() << "std::string Stack{" << this
			<< "}.what()=" << typeid(Stack).name() << std::endl;

	return typeid(Stack).name();
}

base::Object* Stack::operator ()() const {
	base::Object* result = nullptr;
	base::Log track;

	std::clog << track.tracker() << "base::Object* Stack{" << this << "}() {"
			<< std::endl;
	result = Location::operator [](size() - 1);
	std::clog << track() << "}=" << result << std::endl;

	return result;
}
std::unique_ptr<base::Object> Stack::extract(base::Log track) {
	std::unique_ptr<base::Object> result;
	std::ostringstream log;

	log << track.tracker() << "std::string Stack{" << this << "}.extract()";
	std::clog << log.str() << " {" << std::endl;
	if (use) {
		std::ostringstream message;

		message << "= ERROR domain error: this function cannot be "
				"called when the Stack is already used.";
		std::clog << track() << "}" << message.str() << std::endl;
		std::cerr << log.str() << message.str() << std::endl;
		throw std::domain_error(log.str() + message.str());
	} else {
		result.reset(Location::extract(*operator ()(), track).release());
		use = true;
	}
	std::clog << track() << "}=" << result.get() << std::endl;

	return result;
}
bool Stack::used() const {
	std::clog << base::Log().tracker() << "std::string Stack{" << this
			<< "}.used()=" << use << std::endl;

	return use;
}
void Stack::clear(base::Log track) {
	std::set<const Object*> all_objects;

	std::clog << track.tracker() << "void Stack::clear() {" << std::endl;
	for (auto object : (all_objects = all()))
		if (object->what() == typeid(Stack).name())
			dynamic_cast<Stack*>(const_cast<base::Object*>(object))->use =
					false;
	std::clog << track() << "}" << std::endl;
}

Stack::Stack(base::Location* position, base::Log track) :
		Location(position, track) {
	use = false;
	std::clog << track.tracker() << "Stack{" << this
			<< "}::Stack(base::Location* position=" << position << ")"
			<< std::endl;
}

void preparing_to_play(std::vector<std::string> playerNames, bool family,
		base::Log track) {
	auto P = playerNames.size();
	std::set<std::string> playerColors = { colorNames.at(Color::White),
			colorNames.at(Color::Red), colorNames.at(Color::Green),
			colorNames.at(Color::Blue), colorNames.at(Color::Purple) };
	auto gameboard0 = game_board(0);
	auto gameboard1 = game_board(1);
	auto gameboard2 = game_board(2);
	std::ostringstream log;

	log << track.tracker()
			<< "void preparing_to_play(std::vector< std::string > player_names="
			<< base::Prompt::list(playerNames) << ", bool family=" << family
			<< ")";
	std::clog << log.str() << " {" << std::endl;
	/* Place the three game boards */
	// as shown in the illustration to the right.
	if (family) {
		gameboard0->emplace_back < Action > (EX_A_ROOMS_STABLES, track,
		EX_A_ROOMS_STABLES, Data(track), "{t}Build room(s){t}"
				"{r}Wooden hut:#5(W)+2(R)#\n"
				"Clay hut:#5(C)+2(R)#\n"
				"Stone house:#5(S)+2(R)#{r}"
				"and/or"
				"{t}Build Stable(s){t}"
				"2 Wood per stable", track);
		gameboard0->emplace_back < Action > (EX_AF_STARTINGPLAYER_SHOP, track,
		EX_AF_STARTINGPLAYER_SHOP, Data(track), "{t}Starting Player{t}"
				"and{t}Shop{t}1 Food{t}#1(f)#", track);
		gameboard0->emplace_back < Action > (EX_A_TAKE1GRAIN, track,
		EX_A_TAKE1GRAIN, Data(track), "{t}Take 1 Grain{t}"
				"and place in your personal supply"
				"{t}#+(G)#{t}", track);
		gameboard0->emplace_back < Action
				> (EX_A_1FIELD, track, EX_A_1FIELD, Data(track), "{t}Plow\n1 Field{t}", track);
		gameboard0->emplace_back < Action
				> (EX_AF_STABLE, track, EX_AF_STABLE, Data(track), "{t}Build Stable{t}"
						"Build 1 Stable per 1 Wood\nand/or"
						"{t}Bake bread{t}", track);
		gameboard0->emplace_back < Action > (EX_AF_DAYLABORER, track,
		EX_AF_DAYLABORER, Data(track), "{t}Day Laborer{t}"
				"Take 1 Food and 1 Building resource (Wood/Clay/Reed/Stone)"
				"and place them in your personal supply."
				"{t}#+(f){(W)(C)(R)(S)}#{t}", track);
	} else {
		gameboard0->emplace_back < Action > (EX_A_ROOMS_STABLES, track,
		EX_A_ROOMS_STABLES, Data(track), "{t}Build room(s){t}"
				"{r}Wooden hut:#5(W)+2(R)#\n"
				"Clay hut:#5(C)+2(R)#\n"
				"Stone house:#5(S)+2(R)#{r}"
				"and/or"
				"{t}Build Stable(s){t}"
				"2 Wood per stable", track);
		gameboard0->emplace_back < Action
				> (EX_A_STARTINGPLAYER_MINORIMPROVEMENT, track, EX_A_STARTINGPLAYER_MINORIMPROVEMENT, Data(
						track), "{t}Starting Player{t}"
						"and/or\n1 Minor"
						"{t}Improvement{t}", track);
		gameboard0->emplace_back < Action > (EX_A_TAKE1GRAIN, track,
		EX_A_TAKE1GRAIN, Data(track), "{t}Take 1 Grain{t}"
				"and place in your personal supply"
				"{t}#+(G)#{t}", track);
		gameboard0->emplace_back < Action
				> (EX_A_1FIELD, track, EX_A_1FIELD, Data(track), "{t}Plow\n1 Field{t}", track);
		gameboard0->emplace_back < Action > (EX_A_OCCUPATION_0_1_, track,
		EX_A_OCCUPATION_0_1_, Data(track), "{t}1 Occupation{t}"
				"(A player's first Occupation is free, "
				"each additional one costs 1 Food)", track);
		gameboard0->emplace_back < Action > (EX_A_DAYLABORER, track,
		EX_A_DAYLABORER, Data(track), "{t}Day Laborer{t}"
				"Take 2 Foods and place them in your personal supply"
				"{t}#+(f)(f)#{t}", track);
	}
	gameboard1->emplace_back < Card::Round > (B_R1, track, 1, 1, 4, track);
	gameboard1->emplace_back < Action
			> (EX_A_3WOOD, track, EX_A_3WOOD, Data(3, Wooden::woodType, 0, "",
					track), "{t}3 Wood\n#(W)(W)(W)#{t}", track);
	gameboard1->emplace_back < Action
			> (EX_A_1CLAY, track, EX_A_1CLAY, Data(1, Wooden::clayType, 0, "",
					track), "{t}1 Clay\n#(C)#{t}", track);
	gameboard1->emplace_back < Action
			> (EX_A_1REED, track, EX_A_1REED, Data(1, Wooden::reedType, 0, "",
					track), "{t}1 Reed\n#(R)#{t}", track);
	gameboard1->emplace_back < Action
			> (EX_A_FISHING, track, EX_A_FISHING, Data(1, Food::type, 0, "",
					track), "{t}Fishing{t}1 Food{t}#(f)#{t}", track);
	gameboard1->emplace_back < Card::Round > (B_R2, track, 1, 1, 4, track);
	gameboard1->emplace_back < Card::Round > (B_R3, track, 1, 1, 4, track);
	gameboard1->emplace_back < Card::Round > (B_R4, track, 1, 1, 4, track);
	gameboard1->emplace_back < Card::Round > (B_R5, track, 2, 5, 7, track);
	gameboard1->emplace_back < Card::Round > (B_R6, track, 2, 5, 7, track);
	gameboard1->emplace_back < Card::Round > (B_R7, track, 2, 5, 7, track);
	gameboard2->emplace_back < Card::Round > (B_R8, track, 3, 8, 9, track);
	gameboard2->emplace_back < Card::Round > (B_R9, track, 3, 8, 9, track);
	gameboard2->emplace_back < Card::Round > (B_R10, track, 4, 10, 11, track);
	gameboard2->emplace_back < Card::Round > (B_R11, track, 4, 10, 11, track);
	gameboard2->emplace_back < Card::Round > (B_R12, track, 5, 12, 13, track);
	gameboard2->emplace_back < Card::Round > (B_R13, track, 5, 12, 13, track);
	gameboard2->emplace_back < Card::Round > (B_R14, track, 6, 14, 14, track);
	/* Each player */

	for (size_t p = 0; p < P; ++p) {
		/* chooses a color */
		Player* player = nullptr;
		Farmyard* farmyard = nullptr;
		Farmyard::Space* space = nullptr;
		std::map<std::string, std::pair<std::set<std::string>, std::string>> variables;
		std::string choose;
		std::map<Color, std::string>::const_iterator color;

		variables.emplace("color", std::make_pair(playerColors, std::string()));
		std::clog << track() << "Player " << p + 1 << " chose the color "
				<< (choose = base::dialog(
						"The Player " + std::to_string(p + 1) + " "
								+ playerNames[p] + " chooses own color:",
						variables).second["color"]) << std::endl;
		if ((color = find_color(choose)) == colorNames.end()) {
			std::ostringstream result;

			result << "= ERROR the chosen color '" << choose
					<< "' does not exist.";
			std::clog << track() << "}" << result.str() << std::endl;
			std::cerr << log.str() << result.str() << std::endl;
			throw std::range_error(log.str() + result.str());
		}
		playerColors.erase(choose);
		player = Player::player(
				Player::construct(playerNames[p], color->first, track));
		/* and takes the playing pieces in that color, */
		// These are placed in front of the player (facing whichever direction
		// the player chooses).
		for (auto n = 0; n < 5; ++n)
			player->insert_back(Wooden::familyMemberType,
					Wooden::family_member(p + 1, track), track);
		for (auto n = 0; n < 4; ++n)
			player->insert_back(Wooden::stableType,
					Wooden::stable(p + 1, track), track);
		for (auto n = 0; n < 15; ++n)
			player->insert_back(Wooden::fenceType, Wooden::fence(p + 1, track),
					track);
		/* as well as one farmyard. */
		player->emplace<Farmyard>(0, Farmyard::type, track, p + 1, track);
		/* On each of the two building spaces on this farmyard, players first
		 * place a Wooden hut room tile */
		(space =
				(farmyard = dynamic_cast<Farmyard*>(player->operator [](0)))->space(
						1, 1))->insert(0, Tile::woodenHutType,
				Tile::wooden_hut(track), track);
		dynamic_cast<Tile*>(space->operator [](0))->insert_back(
				Wooden::familyMemberType,
				player->extract(*player->find_former(Wooden::familyMemberType),
						track), track);
		/* and then (in each of these rooms) one of their Family members. */
		(space = farmyard->space(2, 1))->insert(0, Tile::woodenHutType,
				Tile::wooden_hut(track), track);
		dynamic_cast<Tile*>(space->operator [](0))->insert_back(
				Wooden::familyMemberType,
				player->extract(*player->find_former(Wooden::familyMemberType),
						track), track);
		// The remaining playing pieces (additional Family members, fences and
		// stables) remain in the bag for now or are placed to one side. Sort
		// the remaining house and hut tiles and the rest of the game components
		// and place them beside the playing area.
		std::clog << track() << "}" << std::endl;
	}
}
// A.
void a(base::Log track) {
	std::default_random_engine generator;
	std::list<std::list<std::string>> rounds = { { EX_R1_SOW_BAKEBREAD,
	EX_R1_IMPROVEMENT, EX_R1_SHEEP, EX_R1_FENCES }, { EX_R2_STONE,
	EX_R2_RENOVATION_IMPROVEMENT, EX_R2_FAMILYGROWTH_MINORIMPROVEMENT }, {
	EX_R3_TAKEVEGETABLE, EX_R3_WILDBOAR }, { EX_R4_STONE, EX_R4_CATTLE }, {
	EX_R5_FIELD_SOW, EX_R5_FAMILYGROWTH }, { EX_R6_RENOVATION_FENCES } };

	std::clog << track.tracker() << "void a() {" << std::endl;
	/* Sort the blue Round cards according to the Stage of the game.
	 * Shuffle each small pile and place the piles on top of each other
	 * with the cards for Stage 6 at the bottom, Stage 5 on top of that,
	 * etc – finishing with the four cards for Stage 1 on the top. */
	// The Round cards make new actions available during the game.
	for (auto stage = rounds.end(); rounds.size();
			stage = rounds.erase(stage)) {
		auto round = (--stage)->begin();

		for (auto n = stage->size(); n > 0; n = stage->size()) {
			round = stage->begin();

			for (n = std::uniform_int_distribution<unsigned>(1, n)(generator);
					n > 1; --n)
				++round;
			deck(B_ROUND_CARDS)->put_down(*round, Card::round(*round, track),
					track);
			stage->erase(round);
		}
	}
	std::clog << track() << "}" << std::endl;
}
// B.
void b(bool family, base::Log track) {
	auto gameboard0 = game_board(0);

	std::clog << track.tracker() << "void b(bool family=" << family << ") {"
			<< std::endl;
	/* If you are playing with 3, 4 or 5 players, take the corresponding set
	 * of green Action cards and place them face up on the spaces to the left
	 * of the first game board. The order in which the cards are laid out is
	 * irrelevant. In a 3-player game, there are 4 cards, in 4- and 5-player
	 * games there are 6 cards. */
	switch (Player::quantity()) {
	case 3:
		gameboard0->insert(0, Card::Action::type + "[3]",
				Card::action(3,
						{ std::make_tuple(
						EX_A_OCCUPATION_2, Data(track),
								"{t}1 Occupation{t}it costs 2 Foods") }, 3,
						{ std::make_tuple(
						EX_A_TAKE2DIFFERENTBUILDINGRESOURCES, Data(track),

						"Take 2 different{t}Building Resources{t}"
								"and place them in your personal supply\n#+$$#") },
						track), track);
		gameboard0->insert(1, Card::Action::type + "[3]",
				Card::action(3,
						{ std::make_tuple( EX_A_2WOOD,
								Data(2, Wooden::woodType, 0, "", track),
								"2 Wood\n#(W)(W)#") }, 3,
						{ std::make_tuple( EX_A_2WOOD,
								Data(2, Wooden::woodType, 0, "", track),
								"2 Wood\n#(W)(W)#") }, track), track);
		gameboard0->insert(2, Card::Action::type + "[3]",
				Card::action(3,
						{ std::make_tuple( EX_A_1CLAY,
								Data(1, Wooden::clayType, 0, "", track),
								"1 Clay\n#(C)#") }, 3,
						{ std::make_tuple( EX_A_1CLAY,
								Data(1, Wooden::clayType, 0, "", track),
								"1 Clay\n#(C)#") }, track), track);
		gameboard0->insert(3, "Action[3]",
				Card::action(3,
						{
								std::make_tuple(EX_A_TAKE1BUILDINGRESOURCE,
										Data(track),
										"Take 1{t}Building Resource{t}of your choosing"
												"\nand place it in your personal supply\n#+$#") },
						3,
						{
								std::make_tuple(EX_A_TAKE1BUILDINGRESOURCE,
										Data(track),
										"Take 1{t}Building Resource{t}of your choosing"
												"\nand place it in your personal supply\n#+$#") },
						track), track);
		break;
	case 4:
		gameboard0->insert(0, Card::Action::type + "[4]",
				Card::action(4,
						{ std::make_tuple( EX_A_1WOOD,
								Data(1, Wooden::woodType, 0, "", track),
								"1 Wood\n#(W)#") }, 4,
						{ std::make_tuple( EX_A_1WOOD,
								Data(1, Wooden::woodType, 0, "", track),
								"1 Wood\n#(W)#") }, track), track);
		gameboard0->insert(1, Card::Action::type + "[4]",
				Card::action(4,
						{ std::make_tuple( EX_A_2WOOD,
								Data(2, Wooden::woodType, 0, "", track),
								"2 Wood\n#(W)(W)#") }, 4,
						{ std::make_tuple( EX_A_2WOOD,
								Data(2, Wooden::woodType, 0, "", track),
								"2 Wood\n#(W)(W)#") }, track), track);
		gameboard0->insert(2, Card::Action::type + "[4]",
				Card::action(4,
						{ std::make_tuple( EX_A_2CLAY,
								Data(2, Wooden::clayType, 0, "", track),
								"2 Clay\n#(C)(C)#") }, 4,
						{ std::make_tuple( EX_A_2CLAY,
								Data(2, Wooden::clayType, 0, "", track),
								"2 Clay\n#(C)(C)#") }, track), track);
		gameboard0->insert(3, Card::Action::type + "[4]",
				Card::action(4,
						{ std::make_tuple(
						EX_A_TRAVELINGPLAYERS,
								Data(1, Food::type, 0, "", track),
								"{t}Traveling Players{t}1 Food\n#(f)#") }, 4,
						{ std::make_tuple(
						EX_A_TRAVELINGPLAYERS,
								Data(1, Food::type, 0, "", track),
								"{t}Traveling Players{t}1 Food\n#(f)#") },
						track), track);
		gameboard0->insert(4, Card::Action::type + "[4]",
				Card::action(4,
						{
								std::make_tuple(EX_A_TAKE_1REED_1STONE_1FOOD,
										Data(track),
										"{t}Take 1 Reed, 1 Stone and 1 Food{t}"
												"and place them in your personal supply\n#+(R)(S)(f)#") },
						4,
						{
								std::make_tuple(EX_A_TAKE_1REED_1STONE_1FOOD,
										Data(track),
										"{t}Take 1 Reed, 1 Stone and 1 Food{t}"
												"and place them in your personal supply\n#+(R)(S)(f)#") },
						track), track);
		gameboard0->insert(5, Card::Action::type + "[4]",
				Card::action(4,
						{
								std::make_tuple(EX_A_OCCUPATION_1_1_2,
										Data(track),
										"{t}1 Occupation{t}"
												"(Playe's first and second Occupations costs 1 Food,"
												"all subsequent ones cost 2 Food)") },
						4,
						{
								std::make_tuple(
								EX_A_TAKE2DIFFERENTBUILDINGRESOURCES,
										Data(track),
										"Take 2 different{t}Building Resources{t}"
												"and place them in your personal supply\n#+$$#") },
						track), track);
		break;
	case 5:
		gameboard0->insert(0, Card::Action::type + "[5]",
				Card::action(5,
						{ std::make_tuple( EX_A_4WOOD,
								Data(4, Wooden::woodType, 0, "", track),
								"4 Wood\n#(W)(W)(W)(W)#") }, 5,
						{ std::make_tuple( EX_A_4WOOD,
								Data(4, Wooden::woodType, 0, "", track),
								"4 Wood\n#(W)(W)(W)(W)#") }, track), track);
		gameboard0->insert(0, Card::Action::type + "[5]",
				Card::action(5,
						{ std::make_tuple( EX_A_3CLAY,
								Data(3, Wooden::clayType, 0, "", track),
								"3 Clay\n#(C)(C)(C)#") }, 5,
						{ std::make_tuple( EX_A_3CLAY,
								Data(3, Wooden::clayType, 0, "", track),
								"3 Clay\n#(C)(C)(C)#") }, track), track);
		gameboard0->insert(0, Card::Action::type + "[5]",
				Card::action(5,
						{
								std::make_tuple(EX_A_1REED_TAKE1STONE_1WOOD,
										Data(1, Wooden::reedType, 0, "", track),
										"{t}1 Reed\n#(R)#"
												"\nFurthermore,\ntake 1 Stone and 1 Wood{t}"
												"and place them in your personal supply{t}#+(S)(W)#{t}") },
						5,
						{
								std::make_tuple(EX_A_1REED_TAKE1STONE_1WOOD,
										Data(1, Wooden::reedType, 0, "", track),
										"{t}1 Reed\n#(R)#"
												"\nFurthermore,\ntake 1 Stone and 1 Wood{t}"
												"and place them in your personal supply{t}#+(S)(W)#{t}") },
						track), track);
		gameboard0->insert(0, Card::Action::type + "[5]",
				Card::action(5,
						{
								std::make_tuple(
								EX_A_TAKEANIMALS, Data(track),
										"{t}Take 1 Sheep and 1 Food{t}#+[S](W)#\nOr"
												"{t}Take\n1 Wild boar{t}#+[B]#\nOr"
												"{t}Pay 1 Food for 1 Cattle{t}#(f)->[C]#") },
						5,
						{
								std::make_tuple(
								EX_A_TAKEANIMALS, Data(track),
										"{t}Take 1 Sheep and 1 Food{t}#+[S](W)#\nOr"
												"{t}Take\n1 Wild boar{t}#+[B]#\nOr"
												"{t}Pay 1 Food for 1 Cattle{t}#(f)->[C]#") },
						track), track);
		gameboard0->insert(0, Card::Action::type + "[5]",
				Card::action(5,
						{
								std::make_tuple(
								EX_A_BUILD1ROOM_TRAVELINGPLAYERS,
										Data(1, Food::type, 0, "", track),
										"{t}Build 1 Room{t}{r}Wooden hut:#5(W)+2(R)#\n"
												"Clay hut:#5(C)+2(R)#\n"
												"Stone house:#5(S)+2(R)#{r}"
												"Or{t}Traveling Players{t}1 Food{t}(f){t}") },
						5,
						{
								std::make_tuple(
								EX_A_BUILD1ROOM_TRAVELINGPLAYERS,
										Data(1, Food::type, 0, "", track),
										"{t}Build 1 Room{t}{r}Wooden hut:#5(W)+2(R)#\n"
												"Clay hut:#5(C)+2(R)#\n"
												"Stone house:#5(S)+2(R)#{r}"
												"Or{t}Traveling Players{t}1 Food{t}(f){t}") },
						track), track);
		gameboard0->insert(0, Card::Action::type + "[5]",
				Card::action(5,
						{
								std::make_tuple(
								EX_A_OCCUPATION_1_1_2_FAMILYGROWTH, Data(track),
										"Either{t}1 Occupation{t}(Player's first and second Occupation"
												"cost 1 Food, all subsequent ones cost 2 Food)\n"
												"Or, from Round 5,{t}Family Growth{t}") },
						5,
						{
								std::make_tuple(
								EX_AF_BUILDINGRESOURCES_FAMILYGROWTH,
										Data(track),
										"Either\ntake 2 different{t}Building Resources{t}of your choosing\n"
												"and place them in your personal supply\n#+$$#"
												"Or, from Round 5,{t}Family Growth{t}") },
						track), track);
		break;
	}
	/* In solo and 2-player games, no green Action cards are used. */
	std::clog << track() << "}" << std::endl;
}
// C.
void c(base::Log track) {
	unsigned short P = 0;
	auto occupations = deck(B_OCCUPATIONS);

	std::clog << track.tracker() << "void c() {" << std::endl;
	// The purple symbol on the left side of the yellow Occupation cards
	// shows how many players the card is used for: [1+] means for 1-5
	// players, [3+] for 3-5 players, [4+] for 4-5 players.
	//TODO Occupation cards
	/* Cards that are not in use are removed from the game; the full deck
	 * of Occupation cards is only available in a 4- or 5-player game. */
	for (unsigned c = 11; c < CARDS; ++c)
		try {
			occupations->put_down(Card::Occupation::type,
					Card::occupation(c, track), track);
		} catch (std::invalid_argument& e) {
			std::clog << track.close();
		}
	/* Shuffle the cards. */
	occupations->shuffle(track);
	/* Each player is dealt a hand of 7 Occupation cards and may look
	 * through them. */
	P = Player::quantity();
	for (short unsigned p = 1; p <= P; ++p) {
		auto player = Player::player(p);

		player->insert_back(Card::Occupation::type, occupations->draw(track),
				track);
		player->insert_back(Card::Occupation::type, occupations->draw(track),
				track);
		player->insert_back(Card::Occupation::type, occupations->draw(track),
				track);
		player->insert_back(Card::Occupation::type, occupations->draw(track),
				track);
		player->insert_back(Card::Occupation::type, occupations->draw(track),
				track);
		player->insert_back(Card::Occupation::type, occupations->draw(track),
				track);
		player->insert_back(Card::Occupation::type, occupations->draw(track),
				track);
	}
	// The remaining Occupation cards are put to one side.
	std::clog << track() << "}" << std::endl;
}
// D.
void d(base::Log track) {
	short unsigned P = 0;
	auto minorImprovements = deck(B_MINORIMPROVEMENTS);

	std::clog << track.tracker() << "void d() {" << std::endl;
	P = Player::quantity();
	// TODO create Minor Improvement cards
	for (unsigned c = 11; c < CARDS; ++c)
		try {
			minorImprovements->put_down(Card::Improvement::minorType,
					Card::improvement(c, track), track);
		} catch (std::invalid_argument& e) {
			std::clog << track.close();
		}
	/* Shuffle the orange Minor Improvement cards. */
	minorImprovements->shuffle(track);
	/* Each player
	 * is dealt a hand of 7 Minor Improvement cards and may look
	 * through them. The remaining Minor Improvement cards are put
	 * to one side. */
	for (short unsigned p = 1; p <= P; ++p) {
		auto player = Player::player(p);

		player->insert(0, Card::Improvement::minorType,
				minorImprovements->draw(track), track);
		player->insert(0, Card::Improvement::minorType,
				minorImprovements->draw(track), track);
		player->insert(0, Card::Improvement::minorType,
				minorImprovements->draw(track), track);
		player->insert(0, Card::Improvement::minorType,
				minorImprovements->draw(track), track);
		player->insert(0, Card::Improvement::minorType,
				minorImprovements->draw(track), track);
		player->insert(0, Card::Improvement::minorType,
				minorImprovements->draw(track), track);
		player->insert(0, Card::Improvement::minorType,
				minorImprovements->draw(track), track);
	}
	std::clog << track() << "}" << std::endl;
}
// E.
void e(base::Log track) {
	std::clog << track.tracker() << "void e() {" << std::endl;
	/* Place the 10 red Major Improvement cards face up on*/
	for (unsigned c = 1; c < 10; ++c)
		major_improvement_board()->insert_back(Card::Improvement::majorType,
				Card::improvement(c, track), track);
	// As soon as 9 Major Improvements have been bought,
	// the board is turned over to show the scoring overview
	// and the remaining Major Improvement is placed on the
	// space on the reverse of the board.
	std::clog << track() << "}" << std::endl;
}

void cards(bool family, base::Log track) {
	std::clog << track.tracker() << "void cards(bool family=" << family << ") {"
			<< std::endl;
	// Sort the cards according to the color of the reverse side. Depending on
	// the number of players, different green Action cards (B) are used. There
	// are also blue Round cards (A), yellow Occupation cards (C), orange
	// “Minor Improvement” cards (D), red “Major Improvement” cards (E), grey
	// Begging cards (F) and Summary cards (G).
	a(track);
	b(family, track);
	c(track);
	d(track);
	e(track);
	// F. Place the grey Begging cards face-up beside the playing area.
	// G. Each player takes a Summary card and places it in their playing area.
	// One side of the card gives an overview of the game phases; the other
	// explains the scoring at the end of the game. There are no scoring rounds
	// during the game.
	std::clog << track() << "}" << std::endl;
}

void starting_player(base::Log track) {
	Player* p = nullptr;
	Farmyard::PersonalSupply* fps = nullptr;

	std::clog << track.tracker() << "void starting_player() {" << std::endl;
	(p = Player::player(1))->insert_back(Wooden::firstPlayerType,
			Wooden::first_player(track), track);
	(fps = dynamic_cast<Farmyard*>((*p)(Farmyard::type))->personal_supply())->emplace_back<
			Food>(Food::type, track, track);
	fps->emplace_back<Food>(Food::type, track, track);
	for (auto P = Player::quantity(); P > 1; --P) {
		(fps =
				dynamic_cast<Farmyard*>((*Player::player(P))(Farmyard::type))->personal_supply())->emplace_back<
				Food>(Food::type, track, track);
		fps->emplace_back<Food>(Food::type, track, track);
		fps->emplace_back<Food>(Food::type, track, track);
	}
	std::clog << track() << "}" << std::endl;
}

std::unique_ptr<base::Object> serialize(std::string good, short unsigned player,
		base::Log track) {
	std::unique_ptr<base::Object> result;

	std::clog << track.tracker() << "std::unique_ptr< base::Object"
			" > serialize(std::string good=" << good << ") {" << std::endl;
	if (good == Wooden::cattleType)
		result = Wooden::cattle(track);
	else if (good == Wooden::clayType)
		result = Wooden::clay(track);
	else if (good == Wooden::familyMemberType)
		result = Wooden::family_member(player, track);
	else if (good == Wooden::fenceType)
		result = Wooden::fence(player, track);
	else if (good == Wooden::grainType)
		result = Wooden::grain(track);
	else if (good == Wooden::reedType)
		result = Wooden::reed(track);
	else if (good == Wooden::sheepType)
		result = Wooden::sheep(track);
	else if (good == Wooden::stableType)
		result = Wooden::stable(player, track);
	else if (good == Wooden::stoneType)
		result = Wooden::stone(track);
	else if (good == Wooden::vegetableType)
		result = Wooden::vegetable(track);
	else if (good == Wooden::wildBoarType)
		result = Wooden::wildBoar(track);
	else if (good == Wooden::woodType)
		result = Wooden::wood(track);
	else if (good == Tile::clayHutType)
		result = Tile::clay_hut(track);
	else if (good == Tile::fieldType)
		result = Tile::field(track);
	else if (good == Tile::stoneHouseType)
		result = Tile::stone_house(track);
	else if (good == Tile::woodenHutType)
		result = Tile::wooden_hut(track);
	else if (good == Food::type)
		result.reset(new Food(track));
	std::clog << track() << "}=" << result.get() << std::endl;

	return std::move(result);
}
void collection(base::Log track) {
	std::set<const base::Object*> all;

	std::clog << track.tracker() << "void board_collection() {" << std::endl;
	all = base::Object::all();
	for (auto object : all)
		if (object->what() == typeid(Action).name()) {
			Action& action =
					dynamic_cast<Action&>(*const_cast<base::Object*>(object));
			auto path = base::Location::path(action);
			auto ancestor = path.begin();
			auto root = ancestor++;

			if (!((*root)->what() == typeid(Player).name()
					&& ((*ancestor)->what() != typeid(Played).name()
							|| (*ancestor)->what() != typeid(Farmyard).name()))) {
				auto collection = action.collect();

				for (int good = 0; good < collection.quantity; ++good)
					action.insert_back(collection.label,
							serialize(collection.label, collection.player,
									track), track);
			}
		}
	std::clog << track() << "}" << std::endl;
}
short unsigned first_player() {
	std::set<const base::Object*> all;
	base::Log track;
	std::ostringstream log;
	std::ostringstream result;

	log << track.tracker() << "short unsigned first_player()";
	std::clog << " {" << std::endl;
	for (auto object : (all = base::Object::all()))
		if (object->what() == Wooden::firstPlayerType) {
			auto position = object->where();

			if (position) {
				if (typeid(Farmyard).name() == position->what()) {
					Player& player = dynamic_cast<Player&>(*position->where());

					std::clog << track() << "}=" << player.number()
							<< std::endl;

					return player.number();
				} else
					result << "= ERROR domain error: the first player"
							" token is not assigned to a player.";
			} else
				result << "= ERROR domain error: the first player"
						" token is not localized.";
		}
	std::clog << track() << "}" << result.str() << std::endl;
	std::cerr << log.str() << result.str() << std::endl;
	throw std::domain_error(log.str() + result.str());
}
Farmyard& player_farmyard(Player& player) {
	Farmyard* result = nullptr;
	base::Log track;
	std::ostringstream log;

	log << track.tracker() << "Farmyard* player_farmyard(Player& player="
			<< &player << ")";
	std::clog << log.str() << " {" << std::endl;
	result = dynamic_cast<Farmyard*>(player.find_former(Farmyard::type));
	std::clog << track() << "}=" << result << std::endl;

	return *result;
}
std::set<Tile*> player_rooms(Farmyard& farmyard) {
	std::set<Tile*> result;
	base::Log track;
	std::ostringstream log;

	log << track.tracker()
			<< "std::set< Tile* > player_rooms(Farmyard& farmyard=" << &farmyard
			<< ")";
	std::clog << log.str() << " {" << std::endl;
	for (size_t row = 1; row < 4; ++row)
		for (size_t column = 1; column < 6; ++column) {
			Farmyard::Space* space = farmyard[row][column];

			if (space->size()) {
				auto content = (*space)[0];

				if (content->what() == Tile::woodenHutType
						|| content->what() == Tile::clayHutType
						|| content->what() == Tile::stoneHouseType)
					result.insert(dynamic_cast<Tile*>(content));
			}
		}
	std::clog << track() << "}=" << base::Prompt::list(result) << std::endl;

	return result;
}
std::set<Tile*> player_rooms(Player& player) {
	std::set<Tile*> result;
	base::Log track;
	std::ostringstream log;

	log << track.tracker() << "std::set< Tile* > player_rooms(Player& player="
			<< &player << ")";
	std::clog << log.str() << " {" << std::endl;
	result = player_rooms(
			*(dynamic_cast<Farmyard*>(player.find_former(Farmyard::type))));
	std::clog << track() << "}=" << base::Prompt::list(result) << std::endl;

	return result;
}
base::Object* waiting_member(short unsigned player) {
	base::Object* result = nullptr;
	std::set<Tile*> rooms;
	std::set<Tile*>::iterator end;
	Farmyard* farmyard = nullptr;
	Player* playerObject = nullptr;
	std::map<std::string, std::pair<std::set<std::string>, std::string>> variables;
	Token* guest = nullptr;
	std::pair<bool, std::map<std::string, std::string>> chosen = { true, { } };
	base::Log track;
	std::ostringstream log;

	log << track.tracker()
			<< "base::Object* waiting_member(short unsigned player=" << player
			<< ")";
	std::clog << log.str() << " {" << std::endl;
	if (player < 1 || player > Player::quantity()) {
		std::ostringstream message;

		message << "= ERROR out of range: the player number " << player
				<< " does not exist.";
		std::clog << track() << "}" << message.str() << std::endl;
		std::cerr << log.str() << message.str() << std::endl;
		throw std::out_of_range(log.str() + message.str());
	}
	variables["Worker"] = { {}, ""};
	playerObject = Player::player(player);
	end = (rooms = player_rooms(*playerObject)).end();
	for (auto room = rooms.begin(); !result && room != end; ++room) {
		auto member = (Wooden*) (*room)->find_former(Wooden::familyMemberType);

		if (member)
			variables["Worker"].first.insert(
					base::path(base::Location::path(*member)));
	}
	if ((guest = (Token*) farmyard->find_former(Token::guestType)))
		variables["Worker"].first.insert(
				base::path(base::Location::path(*guest)));
	if (variables["Worker"].first.size())
		chosen = base::dialog(
				"Player " + std::to_string(player) + " "
						+ playerObject->identity() + ": choose the active "
						+ Wooden::familyMemberType + " between", variables,
				track);
	if (!chosen.first) {
		std::ostringstream message;

		message << "= ERROR run time: canceled by user.";
		std::clog << track() << "}" << message.str() << std::endl;
		std::cerr << log.str() << message.str() << std::endl;
		throw std::runtime_error(log.str() + message.str());
	}
	if (chosen.second.size())
		result = (base::Object*) std::stoull(
				chosen.second["Worker"].substr(
						chosen.second["Worker"].find_last_of('{') + 1), nullptr,
				16);
	std::clog << track() << "}=" << result << std::endl;

	return result;
}
Action* choose_an_action(short unsigned player, base::Log track) {
	Action* result = nullptr;
	std::set<const base::Object*> all;
	std::set<Action*> availableActions;
	std::map<std::string, std::pair<std::set<std::string>, std::string>> variables;
	std::pair<bool, std::map<std::string, std::string>> chosen = { false, { } };
	std::ostringstream log;

	log << track.tracker() << "Action* choose_an_action(short unsigned player="
			<< player << ")";
	std::clog << log.str() << " {" << std::endl;
	if (player < 1 || player > Player::quantity()) {
		std::ostringstream message;

		message << "= ERROR out of range: the player number " << player
				<< " does not exist.";
		std::clog << track() << "}" << message.str() << std::endl;
		std::cerr << log.str() << message.str() << std::endl;
		throw std::out_of_range(log.str() + message.str());
	}
	variables["action"] = std::pair<std::set<std::string>, std::string>();
	for (auto object : (all = base::Object::all()))
		if ((result = const_cast<Action*>(dynamic_cast<const Action*>(object)))) {
			auto path = base::Location::path(*result);
			auto ancestor = path.begin();
			auto root = ancestor++;

			/* Each Action space can only be used by one Person in one round. A
			 * Family member may never occupy an Action space without performing
			 * its action. */
			// Some Action spaces offer players several choices of action or
			// require a player to take one action before (optionally) taking a
			// second action. Whenever a player takes building resources, Grain,
			// Vegetables or Food, the tokens are placed in a player’s personal
			// supply in view of the other players. Animals may not be placed in
			// the supply; they must be placed directly into the farmyard.
			// Animals that cannot be placed into the farmyard must be returned
			// to the general supply or immediately transformed into Food using
			// an Improvement with the symbol (K). A player who plays a card
			// from their hand or buys a Major Improvement must read the text
			// on the card aloud so that all the other players are aware of its
			// effects.
			// Players are not allowed to hide their personal supply from other
			// players or to completely cover cards that they have played.
			if (!result->find_former(Wooden::familyMemberType)
					&& (!((*root)->what() == typeid(Player).name()
							&& ((*ancestor)->what() != typeid(Played).name()
									|| (*ancestor)->what()
											!= typeid(Farmyard).name())))
					&& !dynamic_cast<Executable&>(*result)(player, true,
							track)) {
				availableActions.insert(result);
				variables["action"].first.insert(*result);
			}
		}
	if ((chosen = base::dialog(
			"Player " + std::to_string(player) + " "
					+ Player::player(player)->identity()
					+ ": choose the action between", variables, track)).first) {
		auto a = availableActions.begin();

		while (chosen.second["action"] != (std::string) **a)
			++a;
	}
	std::clog << track() << "}=" << result << std::endl;

	return result;
}
Wooden* worker(short unsigned player, base::Log track) {
	Wooden* result = nullptr;
	std::set<const base::Object*> all;
	std::map<std::string, std::pair<std::set<std::string>, std::string>> variables;
	std::pair<bool, std::map<std::string, std::string>> chosen = { false, { } };
	std::ostringstream log;

	log << track.tracker() << "Wooden* worker(short unsigned player=" << player
			<< ")";
	std::clog << log.str() << " {" << std::endl;
	if (player < 1 || player > Player::quantity()) {
		std::ostringstream message;

		message << "= ERROR out of range: the player number " << player
				<< " does not exist.";
		std::clog << track() << "}" << message.str() << std::endl;
		std::cerr << log.str() << message.str() << std::endl;
		throw std::out_of_range(log.str() + message.str());
	}
	variables["worker"] = std::pair<std::set<std::string>, std::string>();
	for (auto object : (all = base::Object::all()))
		if (object->what() == Wooden::familyMemberType
				&& object->where()->what() == typeid(Action).name()
				&& dynamic_cast<const Wooden*>(object)->player() == player)
			variables["worker"].first.emplace(
					base::path(base::Location::path(*object)));
	if (variables["worker"].first.size())
		chosen = base::dialog(
				"Player " + std::to_string(player) + " "
						+ Player::player(player)->identity()
						+ ": choose the worker to return home", variables,
				track);
	if (!chosen.first) {
		std::ostringstream message;

		message << "= ERROR run time: canceled by user.";
		std::clog << track() << "}" << message.str() << std::endl;
		std::cerr << log.str() << message.str() << std::endl;
		throw std::runtime_error(log.str() + message.str());
	}
	if (chosen.second.size())
		result = (Wooden*) std::stoull(
				chosen.second["Worker"].substr(
						chosen.second["Worker"].find_last_of('{') + 1), nullptr,
				16);
	std::clog << track() << "}=" << result << std::endl;

	return result;
}
Tile& bedroom(short unsigned player) {
	Tile* result = nullptr;
	std::set<Tile*> rooms;
	Player* playerObject = nullptr;
	std::map<std::string, std::pair<std::set<std::string>, std::string>> variables;
	std::pair<bool, std::map<std::string, std::string>> chosen = { false, { } };
	std::ostringstream log;
	base::Log track;

	log << track.tracker() << "Tile* bedroom(short unsigned player=" << player
			<< ")";
	std::clog << log.str() << " {" << std::endl;
	if (player < 1 || player > Player::quantity()) {
		std::ostringstream message;

		message << "= ERROR out of range: the player number " << player
				<< " does not exist.";
		std::clog << track() << "}" << message.str() << std::endl;
		std::cerr << log.str() << message.str() << std::endl;
		throw std::out_of_range(log.str() + message.str());
	}
	variables["bedroom"] = std::pair<std::set<std::string>, std::string>();
	variables["room"] = variables["bedroom"];
	for (auto room : (rooms = player_rooms(*playerObject)))
		if (room->find_former(Wooden::familyMemberType))
			variables["room"].first.emplace(
					base::path(base::Location::path(*room)));
		else
			variables["bedroom"].first.emplace(
					base::path(base::Location::path(*room)));
	if (variables["bedroom"].first.size())
		variables.erase("room");
	else
		variables.erase("bedroom");
	chosen = base::dialog(
			"Player " + std::to_string(player) + " "
					+ Player::player(player)->identity()
					+ ": choose the bedroom.", variables, track);
	if (!chosen.first) {
		std::ostringstream message;

		message << "= ERROR run time: canceled by user.";
		std::clog << track() << "}" << message.str() << std::endl;
		std::cerr << log.str() << message.str() << std::endl;
		throw std::runtime_error(log.str() + message.str());
	}
	result = (Tile*) std::stoull(
			chosen.second["Worker"].substr(
					chosen.second["Worker"].find_last_of('{') + 1), nullptr,
			16);
	std::clog << track() << "}=" << result << std::endl;

	return *result;
}
Farmyard::PersonalSupply& personal_supply(Player& player) {
	Farmyard::PersonalSupply* result = nullptr;
	base::Log track;
	std::ostringstream log;

	std::clog << track.tracker()
			<< "Farmyard::PersonalSupply& personal_supply(Player& player="
			<< &player << ") {" << std::endl;
	result = player_farmyard(player).personal_supply();
	std::clog << track() << "}=" << result << std::endl;

	return *result;
}
Data harvest(short unsigned player, base::Log track) {
	Data result(track);
	Player* playerObject = nullptr;
	std::map<std::string, std::pair<std::set<std::string>, std::string>> variables;
	std::pair<bool, std::map<std::string, std::string>> chosen = { false, { } };
	std::ostringstream log;

	std::clog << track.tracker() << "Data harvest(short unsigned player="
			<< player << ") {";
	if (player < 1 || player > Player::quantity()) {
		std::ostringstream message;

		message << "= ERROR out of range: the player number " << player
				<< " does not exist.";
		std::clog << track() << "}" << message.str() << std::endl;
		std::cerr << log.str() << message.str() << std::endl;
		throw std::out_of_range(log.str() + message.str());
	}
	variables["harvest"] = std::pair<std::set<std::string>, std::string>();
	for (auto key : KEYS)
		if (!key.find(M_HARVEST))
			variables["harvest"].first.emplace(
					key.substr(std::string(M_HARVEST).length() + 1));
	if (variables["harvest"].first.size())
		chosen = base::dialog(
				"Player " + std::to_string(player) + " " + (playerObject =
						Player::player(player))->identity()
						+ ": choose a stack to harvest.", variables, track);
	if (!chosen.first) {
		std::ostringstream message;

		message << "= ERROR run time: canceled by user.";
		std::clog << track() << "}" << message.str() << std::endl;
		std::cerr << log.str() << message.str() << std::endl;
		throw std::runtime_error(log.str() + message.str());
	}
	if (chosen.second.size()) {
		base::Object& object = *((base::Object*) std::stoull(
				chosen.second["harvest"].substr(
						chosen.second["harvest"].find_last_of('{') + 1),
				nullptr, 16));
		auto name = base::Location::who(object);

		personal_supply(*playerObject).insert_back(name,
				dynamic_cast<base::Location*>(object.where())->extract(object,
						track), track);
		result.quantity = 1;
		result.label = object.what();
		result.player = player;
	}
	std::clog << track() << "}=" << result << std::endl;

	return result;
}
std::list<Data> cook(short unsigned player, base::Log track) {
	std::list<Data> result;
	Player* playerObject = nullptr;
	Farmyard::PersonalSupply* ps = nullptr;
	std::string command = "up to ";
	long long unsigned grains = 0;
	long long unsigned vegetables = 0;
	std::map<std::string, std::pair<std::set<std::string>, std::string>> variables;
	std::pair<bool, std::map<std::string, std::string>> chosen = { false, { } };
	std::ostringstream log;

	std::clog << track.tracker()
			<< "std::list< Data > cook(short unsigned player=" << player
			<< ") {";
	if (player < 1 || player > Player::quantity()) {
		std::ostringstream message;

		message << "= ERROR out of range: the player number " << player
				<< " does not exist.";
		std::clog << track() << "}" << message.str() << std::endl;
		std::cerr << log.str() << message.str() << std::endl;
		throw std::out_of_range(log.str() + message.str());
	}
	variables[Wooden::grainType] = {std::set<std::string>(), command +
		std::to_string((grains = ps->find_each(Wooden::grainType).size()))};
	variables[Wooden::vegetableType] = {std::set<std::string>(), command +
		std::to_string((vegetables = ps->find_each(Wooden::vegetableType).size()))};
	ps = &personal_supply(*(playerObject = Player::player(player)));
	if (variables[Wooden::grainType].first.size()
			|| variables[Wooden::vegetableType].first.size())
		chosen = base::dialog(
				"Player " + std::to_string(player) + " "
						+ playerObject->identity()
						+ ": specify how many grains and vegetables "
								"are to cook for 1 Food each one."
								" If the specified number is greater "
								"than the correspondent resource quantity "
								"then that maximum will be considered.",
				variables, track);
	if (chosen.first) {
		long long unsigned resource = std::stoull(
				chosen.second[Wooden::grainType]);

		if (resource < grains)
			grains = resource;
		result.emplace_back(grains, Wooden::grainType, player, "", track);
		if ((resource = std::stoull(chosen.second[Wooden::vegetableType]))
				< vegetables)
			vegetables = resource;
		result.emplace_back(vegetables, Wooden::vegetableType, player, "",
				track);
		resource = grains + vegetables;
		result.emplace_front(resource, Food::type, player, "", track);
	}
	std::clog << track() << "}=" << base::Prompt::list(result) << std::endl;

	return result;
}
long unsigned feed(short unsigned player, base::Log track) {
	long unsigned result = 0;
	Player* playerObject = nullptr;
	Farmyard::PersonalSupply* ps = nullptr;
	long unsigned food = 0;
	std::list<base::Object*> resources;
	auto newborn = std::string(M_NEWBORN) + " - player "
			+ std::to_string(player) + ": ";
	std::set<std::string>::iterator key;
	std::map<std::string, std::pair<std::set<std::string>, std::string>> variables;
	std::pair<bool, std::map<std::string, std::string>> chosen = { false, { } };
	std::ostringstream log;

	std::clog << track.tracker() << "unsigned feed(short unsigned player="
			<< player << ") {";
	if (player < 1 || player > Player::quantity()) {
		std::ostringstream message;

		message << "= ERROR out of range: the player number " << player
				<< " does not exist.";
		std::clog << track() << "}" << message.str() << std::endl;
		std::cerr << log.str() << message.str() << std::endl;
		throw std::out_of_range(log.str() + message.str());
	}
	food = foodPerPerson
			* (5
					- (playerObject = Player::player(player))->find_each(
							Wooden::familyMemberType).size());
	if (KEYS.end() != (key = find_key_containing(newborn)))
		food -= std::stoul(key->substr(newborn.length()));
	if ((result =
			(resources = (ps = &personal_supply(*playerObject))->find_each(
					Food::type)).size()) < food)
		result = food - result;
	variables["feed"] = {std::set<std::string>(), std::to_string(food - result)};
	chosen = base::dialog(
			"Player " + std::to_string(player) + " " + playerObject->identity()
					+ ": specify how much food is to "
							"use to feed the Family Members."
							" If the specified number is greater "
							"than the maximum limit  "
							"then that limit will be considered.", variables,
			track);
	if (chosen.first)
		for (auto f : resources)
			ps->remove(*f, track);
	for (auto beg = result; beg; --beg)
		playerObject->played().insert_back(Card::Begging::type,
				Card::begging(track), track);
	std::clog << track() << "}=" << result << std::endl;

	return result;
}
std::set<Data> player_animals(Player& player) {
	std::set<Data> result;
	std::map<std::string, unsigned> animals;
	std::set<const base::Object*> all;
	base::Log track;
	std::ostringstream log;

	std::clog << track.tracker() << "std::set<Data> breed(Player& player="
			<< &player << ") {";
	animals = { {Wooden::sheepType, 0},
		{	Wooden::wildBoarType, 0},
		{	Wooden::cattleType, 0}};
	for (auto object : (all = base::Object::all())) {
		auto path = base::Location::path(*object);
		auto end = path.end();

		if (std::find(path.begin(), path.end(),
				dynamic_cast<base::Object*>(&player)) != end) {
			if (object->what() == Wooden::sheepType)
				++animals[Wooden::sheepType];
			else if (object->what() == Wooden::wildBoarType)
				++animals[Wooden::wildBoarType];
			else if (object->what() == Wooden::cattleType)
				++animals[Wooden::cattleType];
		}
	}
	result = {Data(animals[Wooden::sheepType], Wooden::sheepType,
				player.number(), "gotten", track),
		Data(animals[Wooden::wildBoarType], Wooden::wildBoarType,
				player.number(), "gotten", track),
		Data(animals[Wooden::cattleType], Wooden::cattleType,
				player.number(), "gotten", track)};
	std::clog << track() << "}=" << base::Prompt::list(result) << std::endl;

	return result;
}
std::set<Data> breed(short unsigned player, base::Log track) {
	std::set<Data> result;
	std::set<Data>::iterator r;
	std::set<Data>::iterator R;
	Player* playerObject = nullptr;
	std::map<std::string, std::pair<std::set<std::string>, std::string>> variables;
	std::pair<bool, std::map<std::string, std::string>> chosen = { false, { } };
	std::ostringstream log;

	std::clog << track.tracker()
			<< "std::set<Data> breed(short unsigned player=" << player << ") {";
	if (player < 1 || player > Player::quantity()) {
		std::ostringstream message;

		message << "= ERROR out of range: the player number " << player
				<< " does not exist.";
		std::clog << track() << "}" << message.str() << std::endl;
		std::cerr << log.str() << message.str() << std::endl;
		throw std::out_of_range(log.str() + message.str());
	}
	r = (result = player_animals(*(playerObject = Player::player(player)))).begin();
	R = result.end();
	while (r != R)
		if (r->quantity > 1 && r->note == "gotten") {
			auto k = KEYS.begin();
			auto K = KEYS.end();
			bool search = true;
			std::string animal = r->label;

			while (search && k != K)
				if (!k->find(M_STALL)) {
					std::stringstream key(*k);
					base::Location* place = nullptr;
					std::string type;
					short unsigned p = 0;
					unsigned actual = 0;
					unsigned possible = 0;

					key >> type >> p >> place >> actual >> type >> possible;
					if (p == player && type == animal && actual < possible) {
						key.clear();
						place->insert_back(type, serialize(type, 0, track), track);
						KEYS.erase(k);
						key << M_STALL << " " << p << " " << place << " " << ++actual << type << possible;
						KEYS.emplace(key.str());
						K = k = KEYS.end();
					} else
						++k;
				}
			if (search) {
				r->quantity -= 2;
				result.emplace(3, r->label, r->player, "freed");

			}
		}
	variables["feed"] = {std::set<std::string>(), std::string()};
	if (true)
		chosen = base::dialog(
				"Player " + std::to_string(player) + " "
						+ playerObject->identity()
						+ ": specify how much food is to "
								"use to feed the Family Members."
								" If the specified number is greater "
								"than the maximum limit  "
								"then that limit will be considered.",
				variables, track);
	if (chosen.first)
		chosen.first = true;
	std::clog << track() << "}=" << base::Prompt::list(result) << std::endl;

	return result;
}

void end_of_the_game(base::Log track) {
	std::clog << track.tracker() << "void end_of_the_game() {" << std::endl;
	// TODO: end of the game
	std::clog << track() << "}" << std::endl;
}

