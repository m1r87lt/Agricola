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
		std::ostringstream(const std::tuple<agr::Farmyard*, short, short>&)> base::Class<
		std::tuple<agr::Farmyard*, short, short>>::printer =
		print_std__tuple_Farmyard__short_short__;

} /* namespace base */

namespace agr {

// Board
Board::Board(std::string label, const Log* caller = nullptr,
		base::Fields attributes) :
		ENSEMBLE(
				false, label, caller, attributes) {
			as_constructor(AGR, __func__, caller, label, attributes);
		}
		Board::~Board() {
			as_destructor(AGR, __func__);
		}

// Farmyard
		base::Ensemble& Farmyard::personal_supply(const Log* caller) const {
			return as_method<false>(__func__, caller, typeid(Ensemble&)).returns(
					dynamic_cast<Ensemble&>(Ensemble::operator [](0)));
		}
		Farmyard::Row Farmyard::operator [](base::Primitive<short> row) const {
			auto log = as_binary(__func__, row, typeid(Row));

			if (row == 0 || row > 3)
			throw base::throw_out_of_range_0(
					base::Primitive<long long unsigned>(row, &log),
					base::Primitive<long long unsigned>(3, &log), log);

			return log.returns(Row(row, *this, &log));
		}
		base::Unique_ptr Farmyard::construct(const Log* caller) {
			auto log = as_method(base::make_scopes(AGR, __func__, TYPEID(Farmyard)),
					false, caller, typeid(base::Unique_ptr));

			return log.returns(base::Unique_ptr::construct<Farmyard>(&log, &log));
		}

		Farmyard::Farmyard(const Log* caller, base::Fields attributes) :
		NEW_BOARD(caller, base::make_scopes(AGR, __func__), true, attributes) {
			auto log = as_constructor(AGR, __func__, caller);
			base::Primitive<const Log*> it(&log, &log);

			for (short row = 3; row; --row)
			for (short column = 5; column; --column)
			generates<Space>(base::Class<std::string>(
							SPACE_NAME + std::to_string(row) + std::to_string(column), &log),
					base::Primitive<size_t>(1, &log), &log, it);
			generates<Ensemble>(base::Class<std::string>(PERSONAL_SUPPLY_NAME, &log),
					base::Primitive<size_t>(1, &log), &log, TYPEID(Ensemble));
		}
		Farmyard::~Farmyard() {
			as_destructor(AGR, __func__);
		}

// Farmyard::Space
		base::Class<std::tuple<Farmyard*,
short, short>> Farmyard::Space::is_located(const Log* caller) const {
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
				caller, attributes), fences { nullptr, nullptr, nullptr, nullptr } {
}
Farmyard::Space::~Space() {
	as_destructor(AGR, base::make_scopes(__func__, TYPEID(Farmyard)));
}

// Row
Farmyard::Space& Farmyard::Row::operator [](
		base::Primitive<short> column) const {
	auto log = as_binary("[]", column, nullptr, typeid(Space&));

	if (column == 0 || column > 5)
		throw throw_out_of_range<short>(column, column > 5 ? 5 : 0, log);

	return log.returns(
			dynamic_cast<Space&>(owner->Board::operator [](
					(row - 1) * 3 + column + 1)));
}
Farmyard::Row::Row(base::Primitive<short> row, base::Primitive<Farmyard*> owner,
		const Log* caller) :
		Object(caller, __func__), Log(caller,
				base::make_scopes(__func__, "a", typeid(Farmyard).name()),
				false) {
	this->row = row;
	this->owner = owner;
}
Farmyard::Row::Row(const Row& copy) :
		Object(copy), Log(nullptr,
				base::make_scopes(__func__, "a", typeid(Farmyard).name()),
				false) {
	row = copy.row;
	owner = copy.owner;
}
Farmyard::Row& Farmyard::Row::operator =(const Row& copy) {
	Object::operator =(copy);
	row = copy.row;
	owner = copy.owner;

	return returns(*this);
}
Farmyard::Row::~Row() {
	as_destructor("a", base::make_scopes(__func__, typeid(Farmyard).name()));
}

} /* namespace agr */
/*
 namespace card {

 const std::string Occupation::type = CARD_OCCUPATION;
 const std::string MinorImprovement::type = CARD_MINOR_IMPROVEMENT;
 const std::string MajorImprovement::type = CARD_MAJOR_IMPROVEMENT;
 const std::string Round::type = CARD_ROUND;

 } /*namespace card */
