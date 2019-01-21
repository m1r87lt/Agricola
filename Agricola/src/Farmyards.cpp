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
base::Class<std::type_index> give_room_type(const Farmyard& farmyard,
		const base::Log* caller = nullptr) {
	auto log = base::Log::as_function(base::make_scopes(AGR, __func__), true,
			caller, typeid(base::Class<std::type_index>), farmyard);
	using RoomType = std::decay<decltype(farmyard[1][1][1])>::type;
	base::Class<std::type_index> result(typeid(void), &log);

	if (std::is_same<RoomType, WoodHut>::value)
		result = typeid(Wood);
	else if (std::is_same<RoomType, ClayHut>::value)
		result = typeid(Clay);
	if (std::is_same<RoomType, StoneHouse>::value)
		result = typeid(Stone);

	return log.returns(result);
}
template<typename Searched> base::Primitive<short unsigned> have_resources(
		const Farmyard& farmyard, const base::Log* caller = nullptr) {
	auto log = base::Log::as_function(base::make_scopes(AGR, __func__), true,
			caller, typeid(base::Primitive<short unsigned>), farmyard);

	derive<Resource, Searched>(log);

	return log.returns(farmyard.personal_supply(&log).count<Resource>(&log));
}

base::Class<std::pair<std::type_index, short unsigned>> how_many_rooms_may_it_build(
		const Player& player, const base::Log* caller) {
	using Result = base::Class<std::pair<std::type_index, short unsigned>>;
	auto log = base::Log::as_function(base::make_scopes(AGR, __func__), true,
			caller, typeid(Result), player);
	auto farmyard = give_farmyard(player, &log);
	auto roomResource = give_room_type(farmyard, &log).becomes();
	Result result( { roomResource, 0 }, &log);
	auto attributes = farmyard.gives_attributes(&log).becomes();
	base::Class<std::pair<short unsigned, short unsigned>> roomResourceQuantity(
			{ 5, 2 }, &log);

	if (roomResource == typeid(Wood))
		result.is().second = ha;

	return log.returns(
			base::Class<std::pair<std::type_index, short unsigned>>());
}

} /* namespace agr */
