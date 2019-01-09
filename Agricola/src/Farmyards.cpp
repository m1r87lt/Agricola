/*
 * Farmyards.cpp
 *
 *  Created on: 3 gen 2019
 *      Author: m1r
 */

#include "Farmyards.h"

namespace base {

template<> std::function<
		std::ostringstream(const std::pair<std::type_index, short unsigned>&)> Class<
		std::pair<std::type_index, short unsigned>>::printer =
		unprint_std__pair_std__type_info_second_<std::type_index, short unsigned>;

} /* namespace base */

namespace agr {

Farmyard& give_farmyard(const Player& player,
		const base::Log* caller = nullptr) {
	return base::Log::as_function(base::make_scopes(AGR, __func__), false,
			caller, typeid(Farmyard&), player).returns(
			*dynamic_cast<Farmyard*>(player[TYPEID(Farmyard)].becomes().front().second));
}
base::Class<std::type_index> give_room_type(const Player& player,
		const base::Log* caller = nullptr) {
	auto log = base::Log::as_function(base::make_scopes(AGR, __func__), true,
			caller, typeid(base::Class<std::type_index>), player);

	return log.returns(
			base::Class<std::type_index>(
					typeid(give_farmyard(player, &log)[1][1][1]), &log));
}
template<typename Searched> base::Primitive<short unsigned> have_resources(
		const Player& player, const base::Log* caller = nullptr) {
	auto log = base::Log::as_function(base::make_scopes(AGR, __func__), true,
			caller, typeid(base::Primitive<short unsigned>));

	derive<Resource, Searched>(log);

	return log.returns(
			give_farmyard(player, &log).personal_supply(&log).count<Resource>(
					&log));
}

base::Class<std::pair<std::type_index, short unsigned>> how_many_rooms_may_it_build(
		const Player& player, const base::Log* caller) {
	using Result = base::Class<std::pair<std::type_index, short unsigned>>;
	auto log = base::Log::as_function(base::make_scopes(AGR, __func__), true,
			caller, typeid(Result), player);
	Result({give_room_type(player, &log).becomes(), have_resources<Wood>(player, &log)}, &log);

	if (roomType == typeid(WoodHut))
		have_resources<Wood>(player, &log);

	return log.returns(
			base::Class<std::pair<std::type_index, short unsigned>>());
}

} /* namespace agr */
