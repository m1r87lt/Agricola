/*
 * Components.cpp
 *
 *  Created on: 13 nov 2018
 *      Author: m1rma
 */
/*
#include "Components.h"

namespace a {
// Board
Board::Board(base::Class<std::string> label, const Log* caller = nullptr,
		base::Fields attributes) :
		base::Object(caller, label.is()), base::Log(caller, label.is(), false), base::Ensemble(
				label, caller, attributes) {
	as_constructor("a", __func__, caller, label, attributes);
}
Board::~Board() {
	as_destructor("a", __func__);
}

// Farmyard
template<typename Primitive> std::out_of_range throw_out_of_range(
		base::Primitive<Primitive>& value, Primitive threshold,
		const base::Log& log) {
	return std::out_of_range(
			log.logs_error(
					std::ostringstream(
							"ERROR: " + value.prints().str() + " "
									+ (value < threshold ?
											"<" :
											(value > threshold ? ">" : "="))
									+ " " + std::to_string(threshold) + ".")));
}
base::Ensemble& Farmyard::personal_supply(const Log* caller) const {
	auto log = as_method(__func__, caller, typeid(Ensemble&));

	return log.returns(dynamic_cast<Ensemble&>(Ensemble::operator [](0)));
}
Farmyard::Row Farmyard::operator [](base::Primitive<short> row) const {
	auto log = as_binary("[]", row, nullptr, typeid(Row));

	if (row == 0 || row > 3)
		throw throw_out_of_range<short>(row, row > 3 ? 3 : 0, log);

	return log.returns(
			Row(row,
					base::Primitive<Farmyard*>(const_cast<Farmyard*>(this),
							&log), &log));
}
template<> std::function<
		std::ostringstream(const std::map<std::string, std::string>&)> base::Class<
		std::map<std::string, std::string>>::printer = base::print_std__map<
		std::string, std::string>;
base::Unique_ptr Farmyard::construct(const Log* caller) {
	return as_method(base::make_scopes(__func__, "a", typeid(Farmyard).name()),
			false, caller, typeid(base::Unique_ptr)).returns(
			base::Unique_ptr::construct<Farmyard>(caller, caller));
}

Farmyard::Farmyard(const Log* caller, base::Fields attributes) :
		Object(caller, base::make_scopes(__func__, "a")), Log(caller,
				base::make_scopes(__func__, "a"), true), Board(
				base::Class<std::string>(base::make_scopes(__func__, "a"),
						caller), caller, attributes) {
	base::Primitive<const Log*> it(this, this);

	for (short row = 3; row; --row)
		for (short column = 5; column; --column)
			generates<Space>(
					"Space" + std::to_string(row) + std::to_string(column), 1,
					this, it);
	generates<Ensemble>(base::Class<std::string>("Personal Supply", this),
			base::Primitive<size_t>(1, this), this,
			base::Class<std::string>(typeid(Ensemble).name(), this), it);
}
Farmyard::~Farmyard() {
	as_destructor("a", __func__);
}

// Farmyard::Space
std::ostringstream class_std__tuple_Farmyard__short_short__(
		const std::tuple<Farmyard*, short, short>& position) {
	std::ostringstream result("{ ");

	result << std::get<0>(position) << "; " << std::get<1>(position) << "; "
			<< std::get<2>(position) << " }";

	return result;
}
template<> std::function<
		std::ostringstream(const std::tuple<Farmyard*, short, short>&)> base::Class<
		std::tuple<Farmyard*, short, short>>::printer =
		a::class_std__tuple_Farmyard__short_short__;
base::Class<std::tuple<Farmyard*, short, short>> Farmyard::Space::is_located(
		const Log* caller) const {
	using Result = base::Class<std::tuple<Farmyard*, short, short>>;
	auto log = as_method(__func__, caller, typeid(Result));
	auto position = Ensemble::localize(*this, &log).is();
	auto location = std::get<0>(position);
	auto offset = std::get<1>(position) - 1;
	auto column = std::get<1>(position) % 5;

	if (location)
		throw base::throw_root_element(*this, log);
	if (offset == 0 && offset > 15)
		throw std::domain_error(
				log.logs_error(
						std::ostringstream(
								"ERROR: the element " + prints().str()
										+ " is in a wrong position="
										+ std::to_string(++offset)
										+ " in the location "
										+ location->prints().str() + ".")));

	return log.returns(
			Result(
					std::make_tuple(dynamic_cast<Farmyard*>(location),
							offset / 3 + 1, column ? column : 5), &log));
}
base::Primitive<base::Element*> Farmyard::Space::operator ()(
		base::Primitive<Direction> direction, const Log* caller) {
	return base::method_primitive(fences[direction], *this, "", caller,
			direction);
}

Farmyard::Space::Space(const Log* caller, base::Fields attributes) :
		Object(caller,
				base::make_scopes(__func__, "a", typeid(Farmyard).name())), Log(
				caller,
				base::make_scopes(__func__, "a", typeid(Farmyard).name()),
				false), Ensemble(
				base::make_scopes(__func__, "a", typeid(Farmyard).name()),
				caller, attributes), fences { nullptr, nullptr, nullptr, nullptr } {
}
Farmyard::Space::~Space() {
	as_destructor("a", base::make_scopes(__func__, typeid(Farmyard).name()));
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

} /* namespace a */
/*
namespace card {

const std::string Occupation::type = CARD_OCCUPATION;
const std::string MinorImprovement::type = CARD_MINOR_IMPROVEMENT;
const std::string MajorImprovement::type = CARD_MAJOR_IMPROVEMENT;
const std::string Round::type = CARD_ROUND;

} /*namespace card */
