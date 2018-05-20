/*
 * executable.h
 *
 *  Created on: 22 gen 2018
 *      Author: MLaRosa
 */

#ifndef EXECUTABLE_H_
#define EXECUTABLE_H_

#include "player.h"

#define EX_A_BUILD1ROOM_TRAVELINGPLAYERS "Build 1 Room+Traveling Players"
#define EX_A_DAYLABORER "Day Laborer"
#define EX_A_FISHING "Fishing"
#define EX_A_OCCUPATION_0_1_ "Occupation(0,1+)"
#define EX_A_OCCUPATION_1_1_2 "Occupation(1,1,2+)"
#define EX_A_OCCUPATION_1_1_2_FAMILYGROWTH "Occupation(1,1,2+)+Family Growth"
#define EX_A_OCCUPATION_2 "Occupation(2+)"
#define EX_A_ROOMS_STABLES "Rooms+Stables"
#define EX_A_STARTINGPLAYER_MINORIMPROVEMENT "Starting player+Minor Improvement"
#define EX_A_TAKEANIMALS "Take animals"
#define EX_A_TAKE1BUILDINGRESOURCE "Take 1 Building Resource"
#define EX_A_TAKE1GRAIN "Take 1 Grain"
#define EX_A_TAKE2DIFFERENTBUILDINGRESOURCES "Take 2 different Building Resources"
#define EX_A_TAKE_1REED_1STONE_1FOOD "Take 1 Reed, 1 Stone and 1 Food"
#define EX_A_TRAVELINGPLAYERS "Traveling Players"
#define EX_A_1CLAY "1 Clay"
#define EX_A_2CLAY "2 Clay"
#define EX_A_3CLAY "3 Clay"
#define EX_A_1FIELD "1 Field"
#define EX_A_1REED "1 Reed"
#define EX_A_1REED_TAKE1STONE_1WOOD "1 Reed+Take 1 Stone and 1 Wood"
#define EX_A_1WOOD "1 Wood"
#define EX_A_2WOOD "2 Wood"
#define EX_A_3WOOD "3 Wood"
#define EX_A_4WOOD "4 Wood"
#define EX_AF_BUILDINGRESOURCES_FAMILYGROWTH "Building Resources+Family Growth"
#define EX_AF_DAYLABORER "Day Laborer[F]"
#define EX_AF_STABLE "Stable"
#define EX_AF_STARTINGPLAYER_SHOP "Starting player+Shop"
#define EX_R1_FENCES "Fences"
#define EX_R1_IMPROVEMENT "Improvement"
#define EX_R1_SHEEP "1 Sheep"
#define EX_R1_SOW_BAKEBREAD "Sow+bake bread"
#define EX_R2_FAMILYGROWTH_MINORIMPROVEMENT "Family growth+Minor Improvement"
#define EX_R2_RENOVATION_IMPROVEMENT "Renovation+Improvement"
#define EX_R2_STONE "1 Stone[2]"
#define EX_R3_TAKEVEGETABLE "Take 1 Vegetable"
#define EX_R3_WILDBOAR "1 Wild Boar"
#define EX_R4_CATTLE "1 Cattle"
#define EX_R4_STONE "1 Stone[4]"
#define EX_R5_FAMILYGROWTH "Family growth without room"
#define EX_R5_FIELD_SOW "1 Field+Sow"
#define EX_R6_RENOVATION_FENCES "Renovation+Fences"

class Executable {
	short unsigned active;
	std::string name;
protected:
	Executable(std::string, base::Log);
public:
	virtual std::string description() const = 0;
	virtual std::string field(std::string, unsigned) const;
	virtual void field(std::string, size_t, std::string, unsigned);

	short unsigned used() const;
	operator std::string() const;
	int operator ()(short unsigned, bool, base::Log);

	static std::map<std::string,
			std::function<int(short unsigned, bool, base::Log)>> operations;

	static int null(short unsigned, bool, base::Log);

	template<typename Container> static std::string list(
			Container&& container) {
		std::ostringstream result;
		auto C = container.end();

		for (auto c = container.begin(); c != C; ++c)
			result << ",\n\t" << (std::string) *c->get();
		result
				<< (result.str().empty() ?
						"{ }" : "{" + result.str().substr(1) + "\n}");

		return result.str();
	}

	virtual ~Executable() = default;
};

#endif /* EXECUTABLE_H_ */
