/*
 * Components.cpp
 *
 *  Created on: 13 nov 2018
 *      Author: m1r
 */

#include "Components.h"

namespace base {

template<> std::function<std::string(const agr::Color*)> Class<agr::Color>::printer =
		[](const agr::Color* color) {
			return color->label;
		};
template<> std::function<std::string(const std::vector<agr::Operation*>*)> Class<
		std::vector<agr::Operation*>>::printer = print_std__vector<
		agr::Operation*>;
template<> std::function<std::string(agr::Operation* const *)> Class<
		agr::Operation*>::printer = print_fundamental<agr::Operation*>;
template<> std::function<std::string(const std::vector<agr::Conditional*>*)> Class<
		std::vector<agr::Conditional*>>::printer = print_std__vector<
		agr::Conditional*>;
template<> std::function<std::string(agr::Conditional* const *)> Class<
		agr::Conditional*>::printer = print_fundamental<agr::Conditional*>;
template<> std::function<
		std::string(const std::array<agr::Farmyard::Space*, 2>*)> Class<
		std::array<agr::Farmyard::Space*, 2> >::printer = print_std__array<
		agr::Farmyard::Space*, 2>;
template<> std::function<std::string(agr::Farmyard::Space* const *)> Class<
		agr::Farmyard::Space*>::printer = print_fundamental<
		agr::Farmyard::Space*>;
template<> std::function<
		std::string(const std::array<agr::Farmyard::Space*, 4>*)> Class<
		std::array<agr::Farmyard::Space*, 4> >::printer = print_std__array<
		agr::Farmyard::Space*, 4>;
template<> std::function<
		std::string(const std::array<agr::Farmyard::Space::Fence*, 4>*)> Class<
		std::array<agr::Farmyard::Space::Fence*, 4> >::printer =
		print_std__array<agr::Farmyard::Space::Fence*, 4>;
template<> std::function<std::string(agr::Farmyard::Space::Fence* const *)> Class<
		agr::Farmyard::Space::Fence*>::printer = print_fundamental<
		agr::Farmyard::Space::Fence*>;
template<> std::function<std::string(const short unsigned *)> Class<
		unsigned short>::printer = print_fundamental<short unsigned>;
template<> std::function<std::string(const agr::Farmyard* const *)> Class<
		const agr::Farmyard*>::printer = print_fundamental<const agr::Farmyard*>;
template<> std::function<std::string(const std::map<std::type_index, unsigned>*)> Class<
		std::map<std::type_index, unsigned>>::printer = print_std__map<
		std::type_index, unsigned>;
template<> std::function<std::string(const std::type_index*)> Class<
		const std::type_index>::printer = [](const std::type_index* index) {
	return std::string("< ") + index->name() + " >";
};
template<> std::function<std::string(const unsigned*)> Class<unsigned>::printer =
		print_fundamental<unsigned>;
template<> std::function<std::string(const char*)> Class<char>::printer =
		[](const char* character) {
			return "'" + std::string(*character, 1) + "' ( " + std::to_string(*character) + " )";
		};
template<> std::function<std::string(const short*)> Class<short>::printer =
		print_fundamental<short>;

} /*namespace base */

namespace agr {

//Board
Board::Fields Board::shows() const {
	return Ensemble::shows();
}
std::string Board::prints() const {
	std::ostringstream result;

	result << NAME(agr::Board) << "{ " << this << " }";

	return result.str();
}

Board::Board(Fields attributes) :
		Ensemble(attributes) {
}
Board::Board(Board&& moving) :
		Ensemble(moving.gives_attributes()) {
}

//Farmyard
Farmyard::PersonalSupply& Farmyard::gives_personal_supply() const {
	return dynamic_cast<PersonalSupply&>(Ensemble::operator [](0));
}
Farmyard::Row Farmyard::operator [](short unsigned row) const {
	return Row(row, *this);
}
Farmyard::Space* Farmyard::puts_next_space() {
	std::array<Space**, 2> previous = { nullptr, nullptr };
	std::array<Space::Fence*, 2> temporary = { generates<Space::Fence>(
			NAME(Space::Fence), has_size(), true), generates<Space::Fence>(
			NAME(Space::Fence), has_size(), false) };
	std::array<Space::Fence*, 2> fences = { generates<Space::Fence>(
			NAME(Space::Fence), has_size(), true), generates<Space::Fence>(
			NAME(Space::Fence), has_size(), false) };
	std::array<Space::Fence**, 2> bounds = { &temporary[0], &temporary[1] };
	Space* result = nullptr;

	temporary[0]->spaces[1] = temporary[1]->spaces[1] = fences[1]->spaces[0] =
			fences[0]->spaces[0] = (result = generates<Space>(NAME(Space)+ "1,1",
	has_size(), fences, previous, bounds));

	return result;
}
Farmyard::Space* Farmyard::puts_next_space(Space& space) {
	auto position = space.is_located();
	auto row = std::get<1>(position);
	auto column = std::get<2>(position);
	std::array<Space**, 2> previous = { nullptr, nullptr };
	std::array<Space::Fence**, 2> bounds = { nullptr, nullptr };
	std::array<Space::Fence*, 2> fences = { generates<Space::Fence>(
			NAME(Space::Fence), has_size(), true), generates<Space::Fence>(
			NAME(Space::Fence), has_size(), false) };
	Space* result = nullptr;

	if (std::get<0>(position) != this)
		throw throw_wrong_position(space, this);
	if (column == 5) {
		++row;
		column = 1;
	}
	fences[1]->spaces[0] = fences[0]->spaces[0] = (result = generates<Space>(
	NAME(Space)+std::to_string(row) + "," + std::to_string(column), has_size(),
	fences, previous, bounds));
	if (row == 1)
		(*bounds[0] = generates<Space::Fence>(NAME(Space::Fence), has_size(),
				false))->spaces[1] = result;
	else
		(*bounds[0] = dynamic_cast<Space::Fence*>((*previous[0] = &operator [](
				row - 1)[column])->operator ()(Space::Direction::N, true)))->spaces[1] =
				result;
	if (column == 1)
		(*bounds[1] = generates<Space::Fence>(NAME(Space::Fence), has_size(),
				true))->spaces[1] = result;
	else
		(*bounds[1] = dynamic_cast<Space::Fence*>((*previous[1] = &operator [](
				row)[column - 1])->operator ()(Space::Direction::W, true)))->spaces[1] =
				result;

	return result;
}
void Farmyard::initializes() {
	auto current = puts_next_space();

	generates<PersonalSupply>(NAME(PersonalSupply));
	for (unsigned short column = 2; column < 6; ++column)
		current = puts_next_space(*current);
	for (unsigned row = 2; row < 4; ++row)
		for (unsigned column = 1; column < 6; ++column)
			current = puts_next_space(*current);
}

Farmyard::Fields Farmyard::shows() const {
	return Ensemble::shows();
}
std::string Farmyard::prints() const {
	auto player = std::get<0>(localize(*this));

	if (!player)
		throw_wrong_position(*this, player);

	return NAME(agr::Farmyard)+ "{ " + player->prints() + " }";
}

Farmyard::Unique_ptr Farmyard::construct(Fields attributes) {
	return Unique_ptr(new Farmyard(attributes));
}
Board* Farmyard::cast(const Farmyard* farmyard) {
	return dynamic_cast<Board*>(const_cast<Farmyard*>(farmyard));
}

Farmyard::Farmyard() {
	initializes();
}
Farmyard::Farmyard(Fields attributes) :
		Board(attributes) {
	initializes();
}

//Farmyard::Space
std::tuple<Farmyard*, short unsigned, short unsigned> Farmyard::Space::is_located() const {
	auto position = Ensemble::localize(*this);
	auto name = std::get<2>(position);

	try {
		auto location = dynamic_cast<Farmyard*>(std::get<0>(position));
		short unsigned row = std::stoi(name.substr(5));
		short unsigned column = std::stoi(name.substr(7));

		return std::make_tuple(location, row, column);
	} catch (std::bad_cast& e) {
		throw base::Throw::not_allowed(
				"Expected a " + NAME(Farmyard) + " for " + NAME(Space)
						+ prints() + ".");
	} catch (std::exception& e) {
		throw base::Throw::not_allowed(
				"\"" + name + "\" expected as \"" + NAME(Space) + "\" + #,#.");
	}
}
base::Ensemble* Farmyard::Space::operator ()(Direction direction,
		bool as_fence) const {
	if (as_fence)
		return fences[direction];
	else
		return adjacents[direction];
}

Farmyard::Fields Farmyard::Space::shows() const {
	auto result = Ensemble::shows();

	result.insert(VARIABLE(adjacents));
	result.insert(VARIABLE(fences));

	return result;
}
std::string Farmyard::Space::prints() const {
	return "agr::" + std::get<2>(localize(*this));
}

Farmyard::Space::Space(std::array<Fence*, 2> fences,
		std::array<Space**, 2> previous, std::array<Fence**, 2> bounds) {
	if (previous[0])
		adjacents[0] = *previous[0];
	previous[0] = &adjacents[0];
	if (previous[1])
		adjacents[1] = *previous[1];
	previous[1] = &adjacents[1];
	if (bounds[0])
		fences[0] = *bounds[0];
	bounds[0] = &fences[0];
	if (bounds[1])
		fences[1] = *bounds[1];
	bounds[1] = &fences[1];
}

//Farmyard::Space::Fence
bool Farmyard::Space::Fence::is_vertical() const {
	return vertical;
}
Farmyard::Space* Farmyard::Space::Fence::operator ()(bool former) const {
	return spaces[0];
}

Farmyard::Fields Farmyard::Space::Fence::shows() const {
	auto result = Ensemble::shows();

	result.insert(VARIABLE(spaces));

	return result;
}
std::string Farmyard::Space::Fence::prints() const {
	std::string result = NAME(agr::Farmyard::Space::Fence)+ "{ ";

	if (spaces[0])
	result += std::string(vertical ? "W" : "N") + " }" + std::get<2>(localize(*spaces[0])).substr(5);
	else
	result += std::string(vertical ? "E" : "S") + " }" + std::get<2>(localize(*spaces[0])).substr(5);

	return result;
}

Farmyard::Space::Fence::Fence(bool vertical) {
	this->vertical = vertical;
}

//Farmyard::Row
Farmyard::Space& Farmyard::Row::operator [](short unsigned column) const {
	auto space =
			&dynamic_cast<Farmyard::Space&>(owner->Ensemble::operator [](2));

	if (row > 1)
		space = dynamic_cast<Farmyard::Space*>(space->operator ()(
				Space::Direction::N, false));
	if (row > 2)
		space = dynamic_cast<Farmyard::Space*>(space->operator ()(
				Space::Direction::N, false));
	for (auto c = 1; c < column; ++c)
		space = dynamic_cast<Farmyard::Space*>(space->operator ()(
				Space::Direction::W, false));

	return *space;
}
Farmyard::Fields Farmyard::Row::shows() const {
	auto result = Object::shows();

	result.insert(VARIABLE(row));
	result.insert(VARIABLE(owner));

	return result;
}
std::string Farmyard::Row::prints() const {
	return NAME(agr::Farmyard::Row)+ "{ " + std::to_string(row) + " }";
}
Farmyard::Row::Row(short unsigned row, const Farmyard& owner) {
	this->row = row;
	this->owner = &owner;
}

//Farmyard::PersonalSupply
Farmyard::Fields Farmyard::PersonalSupply::shows() const {
	return Ensemble::shows();
}
std::string Farmyard::PersonalSupply::prints() const {
	return NAME(agr::Farmyard::PersonalSupply)+ " {" + std::get<0>(localize(*this))->prints() + " }";
}

//Colored
Color Colored::has_color() const {
	return color;
}
Colored::Fields Colored::shows() const {
	auto result = Object::shows();

	result.insert(VARIABLE(color));

	return result;
}

Colored::Colored(Color color) :
		color(color) {
}

//Face
std::vector<Conditional*> Face::has_prerequisites() const {
	return prerequisite;
}
std::string Face::has_name() const {
	return name;
}
Quantity Face::has_cost() const {
	return cost;
}
char Face::belongs() const {
	return deck;
}
std::vector<Operation*> Face::has_events() const {
	return events;
}
bool Face::has_bonus_points() const {
	return bonus_points;
}

Face::Fields Face::shows() const {
	auto result = Ensemble::shows();
	auto colored = Colored::shows();

	result.insert(colored.begin(), colored.end());
	result.insert(VARIABLE(prerequisite));
	result.insert(VARIABLE(name));
	result.insert(VARIABLE(cost));
	result.insert(VARIABLE(deck));
	result.insert(VARIABLE(events));
	result.insert(VARIABLE(bonus_points));

	return result;
}

Face::Face(std::vector<Conditional*> prerequisite, std::string name,
		Quantity cost, char deck, std::vector<Operation*> events,
		bool bonus_points, Color color, Fields attributes) :
		Ensemble(attributes), Colored(color) {
	this->prerequisite = prerequisite;
	this->name = name;
	this->cost = cost;
	this->deck = deck;
	this->events = events;
	this->bonus_points = bonus_points;
}

//Cover
std::string Cover::has_name() const {
	return name;
}

Cover::Fields Cover::shows() const {
	auto result = Ensemble::shows();
	auto colored = Colored::shows();

	result.insert(colored.begin(), colored.end());
	result.insert(VARIABLE(name));

	return result;
}
std::string Cover::prints() const {
	return NAME(Cover)+ "{ " + name + " }";
}

Cover::Unique_ptr Cover::construct(std::string name, Color color,
		Fields attributes) {
	return Unique_ptr(new Cover(name, color, attributes));
}

Cover::Cover(std::string name, Color color, Fields attributes) :
		Ensemble(attributes), Colored(color) {
	this->name = name;
}

//Numbered
unsigned Numbered::is_number() const {
	return number;
}

Numbered::Fields Numbered::shows() const {
	auto result = Object::shows();

	result.insert(VARIABLE(number));

	return result;
}

Numbered::Numbered(unsigned number) {
	this->number = number;
}

//VictoryPointed
short VictoryPointed::has_victory_points() const {
	return victory_points;
}

VictoryPointed::Fields VictoryPointed::shows() const {
	auto result = Object::shows();

	result.insert(VARIABLE(victory_points));

	return result;
}

VictoryPointed::VictoryPointed(short victory_points) {
	this->victory_points = victory_points;
}

//WoodenPiece
bool WoodenPiece::has_same_shape_of(const WoodenPiece& righthand) const {
	return righthand.shape == shape;
}

WoodenPiece::Fields WoodenPiece::shows() const {
	auto result = Element::shows();
	auto colored = Colored::shows();

	result.insert(colored.begin(), colored.end());
	result.emplace(NAME(Shape), label);

	return result;
}

WoodenPiece::WoodenPiece(Shape shape, const char* shape_label, Color color,
		Fields attributes) :
		Element(attributes), Colored(color) {
	this->shape = shape;
	label = shape_label;
}

//FamilyMember
FamilyMember::Fields FamilyMember::shows() const {
	auto result = WoodenPiece::shows();
	auto owned = Owned::shows();

	result.insert(owned.begin(), owned.end());

	return result;
}
std::string FamilyMember::prints() const {
	std::ostringstream result(NAME(FamilyMember));

	result << "( " << Owned::gives_owner().prints() << " ){ " << this << " }";

	return result.str();
}

base::Ensemble::Unique_ptr FamilyMember::construct(Player& player,
		Fields attributes) {
	return base::Ensemble::Unique_ptr(new FamilyMember(player, attributes));
}

FamilyMember::FamilyMember(Player& player, Fields attributes) :
		WOODENPIECE(agr::WoodenPiece::Shape::disc, player.how_is(), attributes), Owned(
				player) {
}

//Stable
Stable::Fields Stable::shows() const {
	auto result = WoodenPiece::shows();
	auto owned = Owned::shows();

	result.insert(owned.begin(), owned.end());

	return result;
}
std::string Stable::prints() const {
	std::ostringstream result(NAME(FamilyMember));

	result << "( " << gives_owner().prints() << " ){ " << this << " }";

	return result.str();
}

base::Ensemble::Unique_ptr Stable::construct(Player& player,
		Fields attributes) {
	return base::Ensemble::Unique_ptr(new Stable(player, attributes));
}

Stable::Stable(Player& player, Fields attributes) :
		WOODENPIECE(agr::WoodenPiece::Shape::house, player.how_is(), attributes), Owned(
				player) {
}

//Fence
Fence::Fields Fence::shows() const {
	auto result = WoodenPiece::shows();
	auto owned = Owned::shows();

	result.insert(owned.begin(), owned.end());

	return result;
}
std::string Fence::prints() const {
	std::ostringstream result(NAME(FamilyMember));

	result << "( " << gives_owner().prints() << " ){ " << this << " }";

	return result.str();
}

base::Ensemble::Unique_ptr Fence::construct(Player& player, Fields attributes) {
	return base::Ensemble::Unique_ptr(new Fence(player, attributes));
}

Fence::Fence(Player& player, Fields attributes) :
		WOODENPIECE(agr::WoodenPiece::Shape::bar, player.how_is(), attributes), Owned(
				player) {
}

//Resource
Resource::Fields Resource::shows() const {
	return WoodenPiece::shows();
}

Resource::Resource(Color color, Fields attributes) :
		WOODENPIECE(agr::WoodenPiece::Shape::disc, color, attributes) {
}

//BuildResource
BuildResource::Fields BuildResource::shows() const {
	return Resource::shows();
}

BuildResource::BuildResource(Color color, Fields attributes) :
		Resource(color, attributes) {
}

//Wood
Wood::Fields Wood::shows() const {
	return BuildResource::shows();
}
std::string Wood::prints() const {
	std::ostringstream result(NAME(Wood));

	result << "{ " << this << " }";

	return result.str();
}

base::Ensemble::Unique_ptr Wood::construct(Fields attributes) {
	auto build_resource = new Wood;

	build_resource->gets_attributes(attributes);

	return base::Ensemble::Unique_ptr(build_resource);
}

Wood::Wood() :
		BuildResource(COLOR(agr::Color::Which::Brown)) {
}

//Clay
Clay::Fields Clay::shows() const {
	return BuildResource::shows();
}
std::string Clay::prints() const {
	std::ostringstream result(NAME(Clay));

	result << "{ " << this << " }";

	return result.str();
}

base::Ensemble::Unique_ptr Clay::construct(Fields attributes) {
	auto build_resource = new Clay;

	build_resource->gets_attributes(attributes);

	return base::Ensemble::Unique_ptr(build_resource);
}

Clay::Clay() :
		BuildResource(COLOR(agr::Color::Which::Red)) {
}

//Reed
Reed::Fields Reed::shows() const {
	return BuildResource::shows();
}
std::string Reed::prints() const {
	std::ostringstream result(NAME(Reed));

	result << "{ " << this << " }";

	return result.str();
}

base::Ensemble::Unique_ptr Reed::construct(Fields attributes) {
	auto build_resource = new Reed;

	build_resource->gets_attributes(attributes);

	return base::Ensemble::Unique_ptr(build_resource);
}

Reed::Reed() :
		BuildResource(COLOR(agr::Color::Which::White)) {
}

//Stone
Stone::Fields Stone::shows() const {
	return BuildResource::shows();
}
std::string Stone::prints() const {
	std::ostringstream result(NAME(Stone));

	result << "{ " << this << " }";

	return result.str();
}

base::Ensemble::Unique_ptr Stone::construct(Fields attributes) {
	auto build_resource = new Stone;

	build_resource->gets_attributes(attributes);

	return base::Ensemble::Unique_ptr(build_resource);
}

Stone::Stone() :
		BuildResource(COLOR(agr::Color::Which::Black)) {
}

//Grain
Grain::Fields Grain::shows() const {
	return Resource::shows();
}
std::string Grain::prints() const {
	std::ostringstream result(NAME(Grain));

	result << "{ " << this << " }";

	return result.str();
}

base::Ensemble::Unique_ptr Grain::construct(Fields attributes) {
	auto resource = new Grain;

	resource->gets_attributes(attributes);

	return base::Ensemble::Unique_ptr(resource);
}

Grain::Grain() :
		Resource(COLOR(agr::Color::Which::Yellow)) {
}

//Vegetable
Vegetable::Fields Vegetable::shows() const {
	return Resource::shows();
}
std::string Vegetable::prints() const {
	std::ostringstream result(NAME(Vegetable));

	result << "{ " << this << " }";

	return result.str();
}

base::Ensemble::Unique_ptr Vegetable::construct(Fields attributes) {
	auto resource = new Vegetable;

	resource->gets_attributes(attributes);

	return base::Ensemble::Unique_ptr(resource);
}

Vegetable::Vegetable() :
		Resource(COLOR(agr::Color::Which::Orange)) {
}

//Animal
Animal::Fields Animal::shows() const {
	return WoodenPiece::shows();
}

Animal::Animal(Color color, Fields attributes) :
		WOODENPIECE(agr::WoodenPiece::Shape::cube, color, attributes) {
}

//Sheep
Sheep::Fields Sheep::shows() const {
	return Animal::shows();
}
std::string Sheep::prints() const {
	std::ostringstream result(NAME(Sheep));

	result << "{ " << this << " }";

	return result.str();
}

base::Ensemble::Unique_ptr Sheep::construct(Fields attributes) {
	auto animal = new Sheep;

	animal->gets_attributes(attributes);

	return base::Ensemble::Unique_ptr(animal);
}

Sheep::Sheep() :
		Animal(COLOR(agr::Color::Which::White)) {
}

//WildBoar
WildBoar::Fields WildBoar::shows() const {
	return Animal::shows();
}
std::string WildBoar::prints() const {
	std::ostringstream result(NAME(WildBoar));

	result << "{ " << this << " }";

	return result.str();
}

base::Ensemble::Unique_ptr WildBoar::construct(Fields attributes) {
	auto animal = new WildBoar;

	animal->gets_attributes(attributes);

	return base::Ensemble::Unique_ptr(animal);
}

WildBoar::WildBoar() :
		Animal(COLOR(agr::Color::Which::Black)) {
}

//Cattle
Cattle::Fields Cattle::shows() const {
	return Animal::shows();
}
std::string Cattle::prints() const {
	std::ostringstream result(NAME(Cattle));

	result << "{ " << this << " }";

	return result.str();
}

base::Ensemble::Unique_ptr Cattle::construct(Fields attributes) {
	auto animal = new Cattle;

	animal->gets_attributes(attributes);

	return base::Ensemble::Unique_ptr(animal);
}

Cattle::Cattle() :
		Animal(COLOR(agr::Color::Which::Brown)) {
}

//StartingPlayer
StartingPlayer::Fields StartingPlayer::shows() const {
	return WoodenPiece::shows();
}
std::string StartingPlayer::prints() const {
	std::ostringstream result(NAME(StartingPlayer));

	result << "{ " << this << " }";

	return result.str();
}

base::Ensemble::Unique_ptr StartingPlayer::construct() {
	return base::Ensemble::Unique_ptr(new StartingPlayer);
}

StartingPlayer::StartingPlayer() :
		WOODENPIECE(agr::WoodenPiece::Shape::cylinder, COLOR(agr::Color::Which::Yellow), Fields()) {
}

//Tile
Tile::Fields Tile::shows() const {
	return Element::shows();
}

//WoodHut
WoodHut::Fields WoodHut::shows() const {
	return Tile::shows();
}
std::string WoodHut::prints() const {
	std::ostringstream result(NAME(WoodHut));

	result << "{ " << this << " }";

	return result.str();
}

base::Ensemble::Unique_ptr WoodHut::construct(Fields attributes) {
	auto tile = new WoodHut;

	tile->gets_attributes(attributes);

	return base::Ensemble::Unique_ptr(tile);
}

//ClayHut
ClayHut::Fields ClayHut::shows() const {
	return Tile::shows();
}
std::string ClayHut::prints() const {
	std::ostringstream result(NAME(ClayHut));

	result << "{ " << this << " }";

	return result.str();
}

base::Ensemble::Unique_ptr ClayHut::construct(Fields attributes) {
	auto tile = new ClayHut;

	tile->gets_attributes(attributes);

	return base::Ensemble::Unique_ptr(tile);
}

//StoneHouse
StoneHouse::Fields StoneHouse::shows() const {
	return Tile::shows();
}
std::string StoneHouse::prints() const {
	std::ostringstream result(NAME(StoneHouse));

	result << "{ " << this << " }";

	return result.str();
}

base::Ensemble::Unique_ptr StoneHouse::construct(Fields attributes) {
	auto tile = new StoneHouse;

	tile->gets_attributes(attributes);

	return base::Ensemble::Unique_ptr(tile);
}

//Food
Food::Fields Food::shows() const {
	return Element::shows();
}
std::string Food::prints() const {
	std::ostringstream result(NAME(Food));

	result << "{ " << this << " }";

	return result.str();
}

base::Ensemble::Unique_ptr Food::construct(Fields attributes) {
	auto food = new Food;

	food->gets_attributes(attributes);

	return base::Ensemble::Unique_ptr(food);
}

} /* namespace agr */

namespace card {

//Action
void Action::adds() {
}
std::vector<base::Ensemble*> Action::includes() {
	std::vector<base::Ensemble*> result;
	auto index = has_size();

	while (index)
		result.push_back(dynamic_cast<base::Ensemble*>(&operator [](index--)));

	return result;
}
Action::Fields Action::shows() const {
	auto result = Ensemble::shows();
	auto colored = Colored::shows();

	result.insert(colored.begin(), colored.end());

	return result;
}
std::string Action::prints() const {
	auto result = NAME(card::Action)+ "{";
	auto size = has_size();
	decltype(size) action = 1;

	while (action < size)
	result += "\n\t" + operator [](action++).prints();

	return result + (size ? "\n" : " ") + "}";
}

//Begging
Begging::Fields Begging::shows() const {
	auto result = Face::shows();
	auto vp = VictoryPointed::shows();

	result.insert(vp.begin(), vp.end());

	return result;
}
std::string Begging::prints() const {
	std::ostringstream result(NAME(card::Begging)+ "{ ");

	result << this << " }";

	return result.str();
}

Begging::Unique_ptr Begging::construct(Fields attributes) {
	return Unique_ptr(new Begging(attributes));
}

Begging::Begging(Fields attributes) :
		Face( { }, NAME(Begging), { }, 0, { }, false,
				COLOR(agr::Color::Which::Grey), attributes), VictoryPointed(-3) {
}

} /*namespace card */
