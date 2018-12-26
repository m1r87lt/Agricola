/*
 * Components.cpp
 *
 *  Created on: 13 nov 2018
 *      Author: m1rma
 */

#include "Components.h"

namespace base {

std::ostringstream print_std__tuple_Farmyard__short_short__(
		const std::tuple<agr::Farmyard*, short, short>& position) {
	return Container_Printer(std::string(), position, "{ ", ": ", ", \"",
			"\" }")(false);
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
template<> std::function<std::ostringstream(const std::vector<agr::Event*>&)> Class<
		std::vector<agr::Event*>>::printer = print_std__vector<agr::Event*>;
template<> std::function<std::ostringstream(const std::vector<agr::Action*>&)> Class<
		std::vector<agr::Action*>>::printer = print_std__vector<agr::Action*>;

} /* namespace base */

namespace agr {

//Board
Board::Board(std::string label, const Log* caller = nullptr,
		base::Fields attributes) :
		ENSEMBLE(
				false, label, caller, attributes) {
			as_constructor(AGR, __func__, caller);
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
		base::Unique_ptr Farmyard::construct(const Log* caller) {
			auto log = as_method(base::make_scopes(AGR, __func__, TYPEID(Farmyard)),
					false, caller, typeid(base::Unique_ptr));

			return log.returns(base::Unique_ptr::construct<Farmyard>(&log, &log));
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
	return as_method("", caller, typeid(base::Primitive<base::Element*>),
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
	return as_method(__func__, caller, typeid(Color)).returns(color);
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
base::Class<std::vector<Event*>> Face::has_events(const Log* caller) const {
	return as_method<false>(__func__, caller,
			typeid(base::Class<std::vector<Event*>>)).returns(events);
}
base::Primitive<bool> Face::has_bonus_points(const Log* caller) const {
	return as_method<false>(__func__, caller, typeid(base::Primitive<bool>)).returns(
			bonus_points);
}

Face::Face(base::Class<std::vector<Condition*>> prerequisite,
		base::Class<std::string> name, base::Quantity cost,
		base::Primitive<char> deck, base::Class<std::vector<Event*>> events,
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
	return as_method(__func__, caller, typeid(base::Class<std::string>)).returns(
			name);
}
std::ostringstream Cover::prints() const {
	return Ensemble::prints();
}
base::Unique_ptr Cover::construct(base::Class<std::string> name, Color color,
		const Log* caller, base::Fields attributes) {
	return std::move(
			base::Unique_ptr::construct<Cover>(caller, name, color, caller,
					attributes));
}

Cover::Cover(base::Class<std::string> name, Color color, const Log* caller,
		base::Fields attributes) :
		ENSEMBLE(false, __func__, caller, attributes), Colored(color, caller), name(
				name) {
	as_constructor<false>(AGR, __func__, caller, name, color);
}

//Actions
const agr::Color Actions::color = COLOR(agr::Color::Which::Green);
const std::string Actions::action = "action";

void Actions::adds(const Log* caller) {
	as_method<false>(__func__, caller, typeid(void));
}
base::Class<std::vector<Action*>> Actions::includes(const Log* caller) {
	auto log = as_method(__func__, caller,
			typeid(base::Class<std::vector<Action*>>));
	base::Class<std::vector<Action*>> result(std::vector<Action*>(), &log);
	auto index = has_size(&log);

	while ((size_t) index)
		result.is().push_back(dynamic_cast<Action*>(&operator [](index--)));

	return log.returns(result);
}

//Numbered
base::Primitive<unsigned> Numbered::is_number(const Log* caller) {
	return as_method(__func__, caller, typeid(base::Primitive<unsigned>)).returns(
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

} /* namespace agr */
/*
 namespace card {

 const std::string Occupation::type = CARD_OCCUPATION;
 const std::string MinorImprovement::type = CARD_MINOR_IMPROVEMENT;
 const std::string MajorImprovement::type = CARD_MAJOR_IMPROVEMENT;
 const std::string Round::type = CARD_ROUND;

 } /*namespace card */
