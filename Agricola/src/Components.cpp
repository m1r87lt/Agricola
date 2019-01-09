/*
 * Components.cpp
 *
 *  Created on: 13 nov 2018
 *      Author: m1r
 */

#include "Components.h"

#include "Cards.h"
#define OCCUPATION_NUMBER(number, caller, attributes) occupation##number(caller, attributes)

namespace base {

std::ostringstream print_std__tuple_Farmyard__short_short__(
		const std::tuple<agr::Farmyard*, short, short>& position) {
	return std::ostringstream(
			Container_Printer(std::string(), position, "{ ", ": ", ", \"",
					"\" }")(false).str().substr(1));
}
template<> std::function<
		std::ostringstream(const std::tuple<agr::Farmyard*, short, short>&)> Class<
		std::tuple<agr::Farmyard*, short, short>>::printer =
		print_std__tuple_Farmyard__short_short__;
template<> std::function<
		std::ostringstream(const std::array<agr::Farmyard::Space*, 2>&)> Class<
		std::array<agr::Farmyard::Space*, 2>>::printer = agr::print_std__array<
		agr::Farmyard::Space*, 2>;
template<> std::function<
		std::ostringstream(const std::array<agr::Farmyard::Space::Fence*, 4>&)> Class<
		std::array<agr::Farmyard::Space::Fence*, 4>>::printer =
		agr::print_std__array<agr::Farmyard::Space::Fence*, 4>;
template<> std::function<std::ostringstream(const std::vector<agr::Condition*>&)> Class<
		std::vector<agr::Condition*>>::printer = print_std__vector<
		agr::Condition*>;
template<> std::function<std::ostringstream(const std::vector<Ensemble*>&)> Class<
		std::vector<Ensemble*>>::printer = print_std__vector<Ensemble*>;
template<> std::function<std::ostringstream(const std::pair<short, bool>&)> Class<
		std::pair<short, bool>>::printer = agr::print_std__pair<short, bool>;
} /* namespace base */

namespace agr {

//Board
Board::Board(std::string label, const Log* caller, base::Fields attributes) :
		ENSEMBLE(
				false, label, caller, attributes) {
			as_constructor(AGR, __func__, caller);
		}
		Board::Board(Board&& moving) :
		ENSEMBLE(
				false, moving.has_label(), &moving, moving.gives_attributes(&moving)) {
			as_constructor(AGR, __func__, &moving);
		}

//Farmyard
		base::Ensemble& Farmyard::personal_supply(const Log* caller) const {
			return as_method<false>(__func__, caller, typeid(Ensemble&)).returns(
					dynamic_cast<Ensemble&>(Ensemble::operator [](0)));
		}
		Farmyard::Row Farmyard::operator [](base::Primitive<short> row) const {
			auto log = as_binary(__func__, row, typeid(Row));

			if (row == 0 || row > 3)
			throw base::throw_out_of_range_0(
					base::Primitive<size_t>((short) row, &log),
					base::Primitive<size_t>(3, &log), log);

			return log.returns(Row(row, *this, &log));
		}
		base::Ensemble::Unique_ptr Farmyard::construct(const Log* caller) {
			auto log = as_method(base::make_scopes(AGR, __func__, TYPEID(Farmyard)),
					false, caller, typeid(base::Unique_ptr));

			return log.returns(Ensemble::Unique_ptr::construct<Farmyard>(&log, &log));
		}

		Farmyard::Farmyard(const Log* caller, base::Fields attributes) :
		BOARD(caller, base::make_scopes(AGR, __func__), true, attributes) {
			auto log = as_constructor(AGR, __func__, caller);
			base::Primitive<const Log*> it(&log, &log);

			for (short row = 3; row; --row)
			for (short column = 5; column; --column)
			generates<Space>(base::Class<std::string>(
							SPACE_NAME + std::to_string(row) + std::to_string(column), &log),
					base::Primitive<size_t>(1, &log), &log, it);
			generates<Ensemble>(base::Class<std::string>(PERSONAL_SUPPLY_NAME, &log),
					base::Primitive<size_t>(1, &log), &log,
					base::Primitive<std::string>(PERSONAL_SUPPLY_NAME, &log), it);
		}

//Farmyard::Space
		base::Class<std::tuple<Farmyard*,
short, short>> Farmyard::Space::is_located(
		const Log* caller) const {
	using Result = base::Class<std::tuple<Farmyard*, short, short>>;
	auto log = as_method(__func__, caller, typeid(Result));
	auto position = Ensemble::localize(*this, &log).is();
	auto location = std::get<0>(position);
	auto offset = std::get<1>(position) - 1;
	auto column = std::get<1>(position) % 5;

	if (!location)
	throw base::throw_not_root(*this, log);
	if (offset == 0 && offset > 15)
	throw base::throw_out_of_range_0(base::Primitive<size_t>(offset, &log),
			base::Primitive<size_t>(15, &log), log);

	return log.returns(
			Result(
					std::make_tuple(dynamic_cast<Farmyard*>(location),
							offset / 3 + 1, column ? column : 5), &log));
}
base::Primitive<base::Element*> Farmyard::Space::operator ()(
		base::Primitive<Direction> direction, const Log* caller) {
	return as_method<false>("", caller, typeid(base::Primitive<base::Element*>),
			direction).returns(fences.is()[direction]->which);
}

Farmyard::Space::Space(const Log* caller, base::Fields attributes) :
		ENSEMBLE(false, base::make_scopes(AGR, TYPEID(Farmyard), __func__),
				caller, attributes), fences( { nullptr, nullptr, nullptr,
				nullptr }, this) {
	as_constructor<false>(AGR, base::make_scopes(TYPEID(Farmyard), __func__),
			caller);
}

//Farmyard::Space::Fence
Farmyard::Space::Fence::Fence(const Log* caller) :
		NEW_LOG(caller, base::make_scopes(AGR, TYPEID(Space), __func__), false), which(
				nullptr, caller), vertical(false, caller), spaces( { nullptr,
				nullptr }, caller) {
	as_constructor<false>(AGR, base::make_scopes(TYPEID(Space), __func__),
			caller);
}

//Farmyard::Row
Farmyard::Space& Farmyard::Row::operator [](
		base::Primitive<short> column) const {
	auto log = as_binary(__func__, column, typeid(Space&));

	if (column == 0 || column > 5)
		throw base::throw_out_of_range_0(
				base::Primitive<size_t>((short) column, &log),
				base::Primitive<size_t>(5, &log), log);

	return log.returns(
			dynamic_cast<Space&>(((Farmyard*) owner)->Ensemble::operator [](
					(row - 1) * 3 + column + 1)));
}

Farmyard::Row::Row(base::Primitive<short> row, const Farmyard& owner,
		const Log* caller) :
		NEW_LOG(caller, base::make_scopes(AGR, TYPEID(Farmyard), __func__), false),
		row(row), owner(
				base::Primitive<Farmyard*>(const_cast<Farmyard*>(&owner), this)) {
	as_constructor<false>(AGR, base::make_scopes(TYPEID(Farmyard), __func__),
			caller, row, dynamic_cast<const Object&>(owner));
	this->row = row;
	;
}
Farmyard::Row::Row(const Row& copy) :
		Object(copy), Log(&copy,
				base::make_scopes(AGR, TYPEID(Farmyard), __func__), false), row(
				copy.row), owner(copy.owner) {
	as_constructor<false>(AGR, base::make_scopes(TYPEID(Farmyard), __func__),
			&copy, copy);
}
Farmyard::Row& Farmyard::Row::operator =(const Row& copy) {
	auto log = as_binary(__func__, copy, typeid(Row&));

	row = copy.row;
	owner = copy.owner;

	return log.returns(*this);
}

//Colored
Color Colored::has_color(const Log* caller) const {
	return as_method<false>(__func__, caller, typeid(Color)).returns(color);
}

Colored::Colored(Color color, const Log* caller) :
		NEW_LOG(caller, base::make_scopes(AGR, __func__), false), color(color) {
	as_constructor<false>(AGR, __func__, caller, color);
}
Colored::Colored(const Colored& copy) :
		Object(copy), Log(&copy, base::make_scopes(AGR, __func__), false), color(
				copy.color) {
	as_constructor<false>(AGR, __func__, &copy, copy);
}
Colored& Colored::operator =(const Colored& copy) {
	auto log = as_binary(__func__, copy, typeid(Colored));

	color = copy.color;

	return log.returns(*this);
}

//Face
base::Class<std::vector<Condition*>> Face::has_prerequisites(
		const Log* caller) const {
	return as_method<false>(__func__, caller,
			typeid(base::Class<std::vector<Condition*>>)).returns(prerequisite);
}
base::Class<std::string> Face::has_name(const Log* caller) const {
	return as_method<false>(__func__, caller, typeid(base::Class<std::string>)).returns(
			name);
}
base::Quantity Face::has_cost(const Log* caller) const {
	return as_method<false>(__func__, caller, typeid(base::Quantity)).returns(
			cost);
}
base::Primitive<char> Face::belongs(const Log* caller) const {
	return as_method<false>(__func__, caller, typeid(base::Primitive<char>)).returns(
			deck);
}
base::Class<std::vector<base::Ensemble*>> Face::has_events(
		const Log* caller) const {
	return as_method<false>(__func__, caller,
			typeid(base::Class<std::vector<base::Ensemble*>>)).returns(events);
}
base::Primitive<bool> Face::has_bonus_points(const Log* caller) const {
	return as_method<false>(__func__, caller, typeid(base::Primitive<bool>)).returns(
			bonus_points);
}

Face::Face(base::Class<std::vector<Condition*>> prerequisite,
		base::Class<std::string> name, base::Quantity cost,
		base::Primitive<char> deck, base::Class<std::vector<Ensemble*>> events,
		base::Primitive<bool> bonus_points, Color color, std::string label,
		const Log* caller, base::Fields attributes) :
		ENSEMBLE(false, label, caller, attributes), Colored(color, caller), prerequisite(
				prerequisite), name(name), cost(cost), deck(deck), events(
				events), bonus_points(bonus_points) {
	as_constructor<false>(AGR, __func__, caller, prerequisite, name, cost, deck,
			events, bonus_points, color);
}

//Cover
base::Class<std::string> Cover::has_name(const Log* caller) const {
	return as_method<false>(__func__, caller, typeid(base::Class<std::string>)).returns(
			name);
}
std::ostringstream Cover::prints() const {
	return Ensemble::prints();
}
base::Ensemble::Unique_ptr Cover::construct(base::Class<std::string> name,
		Color color, const Log* caller, base::Fields attributes) {
	return std::move(
			Unique_ptr::construct<Cover>(caller, name, color, caller,
					attributes));
}

Cover::Cover(base::Class<std::string> name, Color color, const Log* caller,
		base::Fields attributes) :
		ENSEMBLE(false, __func__, caller, attributes), Colored(color, caller), name(
				name) {
	as_constructor<false>(AGR, __func__, caller, name, color);
}

//Numbered
base::Primitive<unsigned> Numbered::is_number(const Log* caller) const {
	return as_method<false>(__func__, caller, typeid(base::Primitive<unsigned>)).returns(
			number);
}

Numbered::Numbered(base::Primitive<unsigned> number, const Log* caller) :
		NEW_LOG(caller, base::make_scopes(AGR, __func__), false), number(number) {
	as_constructor<false>(AGR, __func__, caller, number);
}
Numbered::Numbered(const Numbered& copy) :
		Object(copy), Log(&copy, base::make_scopes(AGR, __func__), false), number(
				copy.number) {
	as_constructor<false>(AGR, __func__, &copy, copy);
}
Numbered& Numbered::operator =(const Numbered& copy) {
	auto log = as_binary(__func__, copy, typeid(Numbered));

	number = copy.number;

	return log.returns(*this);
}

//WoodenPiece
base::Primitive<bool> WoodenPiece::has_same_shape_of(
		const WoodenPiece& righthand) const {
	auto log = as_binary(__func__, righthand, typeid(base::Primitive<bool>));

	return log.returns(base::Primitive<bool>(righthand.shape == shape, &log));
}
std::ostringstream WoodenPiece::prints() const {
	std::ostringstream result(has_label());

	result << "[" << label << "]{" << this << "}";

	return result;
}

WoodenPiece::WoodenPiece(Color color, Shape shape,
		base::Primitive<const char*> shape_label, std::string label,
		const Log* caller, base::Fields attributes) :
		ELEMENT(false, label, caller, attributes), Colored(color, caller), label(
				shape_label) {
	as_constructor<false>(AGR, __func__, caller, color, shape_label,
			attributes);
	this->shape = shape;
}

//FamilyMember
std::ostringstream FamilyMember::prints() const {
	return WoodenPiece::prints();
}
base::Unique_ptr FamilyMember::construct(Player& player, const Log* caller,
		base::Fields attributes) {
	auto log = Log::as_method(
			base::make_scopes(AGR, TYPEID(FamilyMember), __func__), true,
			caller, typeid(base::Unique_ptr), player, attributes);

	return log.returns(
			base::Unique_ptr::construct<FamilyMember>(&log, player,
					base::Primitive<const Log*>(&log, &log), attributes));
}

FamilyMember::FamilyMember(Player& player, const Log* caller,
		base::Fields attributes) :
		WOODEN_SHAPE(caller, base::make_scopes(AGR, __func__), false,
				player.how_is(caller), Shape::disc, attributes), Owned(player,
				caller) {
	as_constructor<false>(AGR, __func__, caller, player, attributes);
}

//Stable
std::ostringstream Stable::prints() const {
	return WoodenPiece::prints();
}
base::Unique_ptr Stable::construct(Player& player, const Log* caller,
		base::Fields attributes) {
	auto log = Log::as_method(base::make_scopes(AGR, TYPEID(Stable), __func__),
			true, caller, typeid(base::Unique_ptr), player, attributes);

	return log.returns(
			base::Unique_ptr::construct<Stable>(&log, player,
					base::Primitive<const Log*>(&log, &log), attributes));
}

Stable::Stable(Player& player, const Log* caller, base::Fields attributes) :
		WOODEN_SHAPE(caller, base::make_scopes(AGR, __func__), false,
				player.how_is(caller), Shape::house, attributes), Owned(player,
				caller) {
	as_constructor<false>(AGR, __func__, caller, player, attributes);
}

//Fence
std::ostringstream Fence::prints() const {
	return WoodenPiece::prints();
}
base::Unique_ptr Fence::construct(Player& player, const Log* caller,
		base::Fields attributes) {
	auto log = Log::as_method(base::make_scopes(AGR, TYPEID(Fence), __func__),
			true, caller, typeid(base::Unique_ptr), player, attributes);

	return log.returns(
			base::Unique_ptr::construct<Fence>(&log, player,
					base::Primitive<const Log*>(&log, &log), attributes));
}

Fence::Fence(Player& player, const Log* caller, base::Fields attributes) :
		WOODEN_SHAPE(caller, base::make_scopes(AGR, __func__), false,
				player.how_is(caller), Shape::bar, attributes), Owned(player,
				caller) {
	as_constructor<false>(AGR, __func__, caller, player, attributes);
}

//Resource
Resource::Resource(Color color, std::string label, const Log* caller,
		base::Fields attributes) :
		WOODEN_SHAPE(caller, label, false,
				color, Shape::disc, attributes) {
	as_constructor<false>(AGR, __func__, caller, color, attributes);
}

//BuildResource
BuildResource::BuildResource(Color color, std::string label, const Log* caller,
		base::Fields attributes) :
		RESOURCE(color, label, false, caller, attributes) {
	as_constructor<false>(AGR, __func__, caller, color, attributes);
}

//Wood
base::Unique_ptr Wood::construct(const Log* caller, base::Fields attributes) {
	auto log = Log::as_method(base::make_scopes(AGR, TYPEID(Wood), __func__),
			true, caller, typeid(base::Unique_ptr), attributes);

	return log.returns(
			base::Unique_ptr::construct<Wood>(&log,
					base::Primitive<const Log*>(&log, &log), attributes));
}

Wood::Wood(const Log* caller, base::Fields attributes) :
		BUILDRESOURCE(LOGGED_COLOR(agr::Color::Which::Brown, caller),
				base::make_scopes(AGR, __func__), false, caller, attributes) {
	as_constructor<false>(AGR, __func__, caller, attributes);
}

//Clay
base::Unique_ptr Clay::construct(const Log* caller, base::Fields attributes) {
	auto log = Log::as_method(base::make_scopes(AGR, TYPEID(Clay), __func__),
			true, caller, typeid(base::Unique_ptr), attributes);

	return log.returns(
			base::Unique_ptr::construct<Clay>(&log,
					base::Primitive<const Log*>(&log, &log), attributes));
}

Clay::Clay(const Log* caller, base::Fields attributes) :
		BUILDRESOURCE(LOGGED_COLOR(agr::Color::Which::Red, caller),
		base::make_scopes(AGR, __func__), false, caller, attributes) {
	as_constructor<false>(AGR, __func__, caller, attributes);
}

//Reed
base::Unique_ptr Reed::construct(const Log* caller, base::Fields attributes) {
	auto log = Log::as_method(base::make_scopes(AGR, TYPEID(Reed), __func__),
			true, caller, typeid(base::Unique_ptr), attributes);

	return log.returns(
			base::Unique_ptr::construct<Reed>(&log,
					base::Primitive<const Log*>(&log, &log), attributes));
}

Reed::Reed(const Log* caller, base::Fields attributes) :
		BUILDRESOURCE(LOGGED_COLOR(agr::Color::Which::White, caller),
		base::make_scopes(AGR, __func__), false, caller, attributes) {
	as_constructor<false>(AGR, __func__, caller, attributes);
}

//Stone
base::Unique_ptr Stone::construct(const Log* caller, base::Fields attributes) {
	auto log = Log::as_method(base::make_scopes(AGR, TYPEID(Stone), __func__),
			true, caller, typeid(base::Unique_ptr), attributes);

	return log.returns(
			base::Unique_ptr::construct<Stone>(&log,
					base::Primitive<const Log*>(&log, &log), attributes));
}

Stone::Stone(const Log* caller, base::Fields attributes) :
		BUILDRESOURCE(LOGGED_COLOR(agr::Color::Which::Black, caller),
		base::make_scopes(AGR, __func__), false, caller, attributes) {
	as_constructor<false>(AGR, __func__, caller, attributes);
}

//Grain
base::Unique_ptr Grain::construct(const Log* caller, base::Fields attributes) {
	auto log = Log::as_method(base::make_scopes(AGR, TYPEID(Grain), __func__),
			true, caller, typeid(base::Unique_ptr), attributes);

	return log.returns(
			base::Unique_ptr::construct<Grain>(&log,
					base::Primitive<const Log*>(&log, &log), attributes));
}

Grain::Grain(const Log* caller, base::Fields attributes) :
		RESOURCE(LOGGED_COLOR(agr::Color::Which::Yellow, caller),
		base::make_scopes(AGR, __func__), false, caller, attributes) {
	as_constructor<false>(AGR, __func__, caller, attributes);
}

//Vegetable
base::Unique_ptr Vegetable::construct(const Log* caller,
		base::Fields attributes) {
	auto log = Log::as_method(
			base::make_scopes(AGR, TYPEID(Vegetable), __func__), true, caller,
			typeid(base::Unique_ptr), attributes);

	return log.returns(
			base::Unique_ptr::construct<Vegetable>(&log,
					base::Primitive<const Log*>(&log, &log), attributes));
}

Vegetable::Vegetable(const Log* caller, base::Fields attributes) :
		RESOURCE(LOGGED_COLOR(agr::Color::Which::Orange, caller),
		base::make_scopes(AGR, __func__), false, caller, attributes) {
	as_constructor<false>(AGR, __func__, caller, attributes);
}

//Resource
Animal::Animal(Color color, std::string label, const Log* caller,
		base::Fields attributes) :
		WOODEN_SHAPE(caller, label, false,
				color, Shape::cube, attributes) {
	as_constructor<false>(AGR, __func__, caller, color, attributes);
}

//Sheep
base::Unique_ptr Sheep::construct(const Log* caller, base::Fields attributes) {
	auto log = Log::as_method(base::make_scopes(AGR, TYPEID(Sheep), __func__),
			true, caller, typeid(base::Unique_ptr), attributes);

	return log.returns(
			base::Unique_ptr::construct<Sheep>(&log,
					base::Primitive<const Log*>(&log, &log), attributes));
}

Sheep::Sheep(const Log* caller, base::Fields attributes) :
		WOODEN_SHAPE(caller, base::make_scopes(AGR, __func__), false,
				LOGGED_COLOR(agr::Color::Which::White, caller), Shape::cube, attributes) {
	as_constructor<false>(AGR, __func__, caller, attributes);
}

//WildBoar
base::Unique_ptr WildBoar::construct(const Log* caller,
		base::Fields attributes) {
	auto log = Log::as_method(
			base::make_scopes(AGR, TYPEID(WildBoar), __func__), true, caller,
			typeid(base::Unique_ptr), attributes);

	return log.returns(
			base::Unique_ptr::construct<WildBoar>(&log,
					base::Primitive<const Log*>(&log, &log), attributes));
}

WildBoar::WildBoar(const Log* caller, base::Fields attributes) :
		WOODEN_SHAPE(caller, base::make_scopes(AGR, __func__), false,
				LOGGED_COLOR(agr::Color::Which::Black, caller), Shape::cube, attributes) {
	as_constructor<false>(AGR, __func__, caller, attributes);
}

//Cattle
base::Unique_ptr Cattle::construct(const Log* caller, base::Fields attributes) {
	auto log = Log::as_method(base::make_scopes(AGR, TYPEID(Cattle), __func__),
			true, caller, typeid(base::Unique_ptr), attributes);

	return log.returns(
			base::Unique_ptr::construct<Cattle>(&log,
					base::Primitive<const Log*>(&log, &log), attributes));
}

Cattle::Cattle(const Log* caller, base::Fields attributes) :
		WOODEN_SHAPE(caller, base::make_scopes(AGR, __func__), false,
				LOGGED_COLOR(agr::Color::Which::Brown, caller), Shape::cube, attributes) {
	as_constructor<false>(AGR, __func__, caller, attributes);
}

//StartingPlayer
base::Unique_ptr StartingPlayer::construct(const Log* caller) {
	auto log = Log::as_method(
			base::make_scopes(AGR, TYPEID(StartingPlayer), __func__), true,
			caller, typeid(base::Unique_ptr));

	return log.returns(
			base::Unique_ptr::construct<StartingPlayer>(&log,
					base::Primitive<const Log*>(&log, &log)));
}

StartingPlayer::StartingPlayer(const Log* caller) :
		WOODEN_SHAPE(caller, base::make_scopes(AGR, __func__), false,
				LOGGED_COLOR(agr::Color::Which::Yellow, caller), Shape::cylinder, nullptr) {
	as_constructor<false>(AGR, __func__, caller);
}

//Tile
std::ostringstream Tile::prints() const {
	std::ostringstream result(has_label());

	result << "{" << this << "}";

	return result;
}

Tile::Tile(std::string label, const Log* caller, base::Fields attributes) :
		ELEMENT(false, label, caller, attributes) {
	as_constructor<false>(AGR, __func__, caller, attributes);
}

//WoodHut
base::Unique_ptr WoodHut::construct(const Log* caller,
		base::Fields attributes) {
	auto log = as_method(base::make_scopes(AGR, TYPEID(WoodHut), __func__),
			true, caller, typeid(base::Unique_ptr));

	return log.returns(
			base::Unique_ptr::construct<WoodHut>(&log,
					base::Primitive<const Log*>(&log, &log), attributes));
}

WoodHut::WoodHut(const Log* caller, base::Fields attributes) :
		TILE(caller, base::make_scopes(AGR, __func__), false, attributes) {
	as_constructor<false>(AGR, __func__, caller, attributes);
}

//ClayHut
base::Unique_ptr ClayHut::construct(const Log* caller,
		base::Fields attributes) {
	auto log = as_method(base::make_scopes(AGR, TYPEID(ClayHut), __func__),
			true, caller, typeid(base::Unique_ptr));

	return log.returns(
			base::Unique_ptr::construct<ClayHut>(&log,
					base::Primitive<const Log*>(&log, &log), attributes));
}

ClayHut::ClayHut(const Log* caller, base::Fields attributes) :
		TILE(caller, base::make_scopes(AGR, __func__), false, attributes) {
	as_constructor<false>(AGR, __func__, caller, attributes);
}

//StoneHouse
base::Unique_ptr StoneHouse::construct(const Log* caller,
		base::Fields attributes) {
	auto log = as_method(base::make_scopes(AGR, TYPEID(StoneHouse), __func__),
			true, caller, typeid(base::Unique_ptr));

	return log.returns(
			base::Unique_ptr::construct<StoneHouse>(&log,
					base::Primitive<const Log*>(&log, &log), attributes));
}

StoneHouse::StoneHouse(const Log* caller, base::Fields attributes) :
		TILE(caller, base::make_scopes(AGR, __func__), false, attributes) {
	as_constructor<false>(AGR, __func__, caller, attributes);
}

//Food
base::Unique_ptr Food::construct(const Log* caller, base::Fields attributes) {
	auto log = as_method(base::make_scopes(AGR, TYPEID(Food), __func__), true,
			caller, typeid(base::Unique_ptr));

	return log.returns(
			base::Unique_ptr::construct<Food>(&log,
					base::Primitive<const Log*>(&log, &log), attributes));
}

Food::Food(const Log* caller, base::Fields attributes) :
		ELEMENT(false, base::make_scopes(AGR, __func__), caller, attributes) {
	as_constructor<false>(AGR, __func__, caller, attributes);
}

} /* namespace agr */

namespace card {
#define CARD "card"

//Occupation
base::Class<std::pair<short, bool>> Occupation::has_player_number(
		const Log* caller) const {
	return as_method<false>(__func__, caller,
			typeid(base::Class<std::pair<short, bool>>)).returns(player_number);
}
std::ostringstream Occupation::prints() const {
	std::ostringstream result;

	result << has_label() << "{" << is_number() << "}";

	return result;
}

Occupation::Occupation(base::Class<std::vector<agr::Condition*>> prerequisite,
		base::Class<std::string> name, base::Quantity cost,
		base::Primitive<char> deck,
		base::Class<std::vector<base::Ensemble*>> events,
		base::Primitive<bool> bonus_points, base::Primitive<unsigned> number,
		base::Class<std::pair<short, bool>> player_number, const Log* caller,
		base::Fields attributes) :
		NUMBERED(caller, base::make_scopes(CARD, __func__), false, number), Face(
				prerequisite, name, cost, deck, events, bonus_points,
				LOGGED_COLOR(agr::Color::Which::Yellow, caller),
				base::make_scopes(CARD, __func__), caller, attributes), player_number(
				player_number) {
	as_constructor<false>(AGR, __func__, caller, prerequisite, name, cost, deck,
			events, bonus_points, number, player_number, attributes);
}

//Improvement
base::Primitive<short> Improvement::has_victory_points(
		const Log* caller) const {
	return as_method<false>(__func__, caller, typeid(base::Primitive<short>)).returns(
			victory_points);
}
base::Primitive<bool> Improvement::is_oven(const Log* caller) const {
	return as_method<false>(__func__, caller, typeid(base::Primitive<bool>)).returns(
			oven);
}
base::Primitive<bool> Improvement::is_kitchen(const Log* caller) const {
	return as_method<false>(__func__, caller, typeid(base::Primitive<bool>)).returns(
			kitchen);
}
std::ostringstream Improvement::prints() const {
	std::ostringstream result;

	result << has_label() << "{" << is_number() << "}";

	return result;
}

Improvement::Improvement(base::Class<std::vector<agr::Condition*>> prerequisite,
		base::Class<std::string> name, base::Quantity cost,
		base::Primitive<char> deck,
		base::Class<std::vector<base::Ensemble*>> events,
		base::Primitive<bool> bonus_points, agr::Color color,
		base::Primitive<unsigned> number, base::Primitive<short> victory_points,
		base::Primitive<bool> oven, base::Primitive<bool> kitchen,
		const Log* caller, base::Fields attributes) :
		NUMBERED(caller, base::make_scopes(CARD, __func__), false, number), Face(
				prerequisite, name, cost, deck, events, bonus_points, color,
				base::make_scopes(CARD, __func__), caller, attributes), victory_points(
				victory_points), oven(oven), kitchen(kitchen) {
	as_constructor<false>(AGR, __func__, caller, prerequisite, name, cost, deck,
			events, bonus_points, color, number, victory_points, oven, kitchen,
			attributes);
}

//Minor Improvement
MinorImprovement::MinorImprovement(
		base::Class<std::vector<agr::Condition*>> prerequisite,
		base::Class<std::string> name, base::Quantity cost,
		base::Primitive<char> deck, base::Class<std::vector<Ensemble*>> events,
		base::Primitive<bool> bonus_points, base::Primitive<unsigned> number,
		base::Primitive<short> victory_points, base::Primitive<bool> oven,
		base::Primitive<bool> kitchen, const Log* caller,
		base::Fields attributes) :
		NEW_LOG(caller, base::make_scopes(AGR, __func__), false), Improvement(
				prerequisite, name, cost, deck, events, bonus_points,
				LOGGED_COLOR(agr::Color::Which::Blue, caller), number,
				victory_points, oven, kitchen, caller, attributes) {
	as_constructor<false>(AGR, __func__, caller, prerequisite, name, cost, deck,
			events, bonus_points, number, victory_points, oven, kitchen,
			attributes);
}

//Major Improvement
MajorImprovement::MajorImprovement(
		base::Class<std::vector<agr::Condition*>> prerequisite,
		base::Class<std::string> name, base::Quantity cost,
		base::Primitive<char> deck,
		base::Class<std::vector<base::Ensemble*>> events,
		base::Primitive<bool> bonus_points, base::Primitive<unsigned> number,
		base::Primitive<short> victory_points, base::Primitive<bool> oven,
		base::Primitive<bool> kitchen, const Log* caller,
		base::Fields attributes) :
		NEW_LOG(caller, base::make_scopes(AGR, __func__), false), Improvement(
				prerequisite, name, cost, deck, events, bonus_points,
				LOGGED_COLOR(agr::Color::Which::Blue, caller), number,
				victory_points, oven, kitchen, caller, attributes) {
	as_constructor<false>(AGR, __func__, caller, prerequisite, name, cost, deck,
			events, bonus_points, number, victory_points, oven, kitchen,
			attributes);
}

//Action
const std::string Action::type = "action";

void Action::adds(const Log* caller) {
	as_method<false>(__func__, caller, typeid(void));
}
base::Class<std::vector<base::Ensemble*>> Action::includes(const Log* caller) {
	auto log = as_method(__func__, caller,
			typeid(base::Class<std::vector<Ensemble*>>));
	base::Class<std::vector<Ensemble*>> result(std::vector<Ensemble*>(), &log);
	auto index = has_size(&log);

	while ((size_t) index)
		result.is().push_back(dynamic_cast<Ensemble*>(&operator [](index--)));

	return log.returns(result);
}

std::ostringstream Action::prints() const {
	return Ensemble::prints();
}
//Begging
std::ostringstream Begging::prints() const {
	return Ensemble::prints();
}
base::Ensemble::Unique_ptr Begging::construct(const Log* caller,
		base::Fields attributes) {
	auto log = as_method(base::make_scopes(AGR, __func__), true, caller,
			typeid(Ensemble::Unique_ptr), attributes);

	return log.returns(
			Ensemble::Unique_ptr::construct<Begging>(&log,
					base::Primitive<const Log*>(&log, &log), attributes));
}

Begging::Begging(const Log* caller, base::Fields attributes) :
		NEW_LOG(caller, base::make_scopes(AGR, __func__), false), Face(
				base::Class<std::vector<agr::Condition*>>(
						std::vector<agr::Condition*>(), caller),
				base::Class<std::string>(__func__, caller),
				base::Quantity(std::map<std::type_index, int>(), caller),
				base::Primitive<char>('\0', caller),
				base::Class<std::vector<Ensemble*>>(std::vector<Ensemble*>(),
						caller), base::Primitive<bool>(false, caller),
				LOGGED_COLOR(agr::Color::Which::Grey, caller),
				base::make_scopes(AGR, __func__), caller, attributes) {
	as_constructor(AGR, __func__, caller, attributes);
}

} /*namespace card */
