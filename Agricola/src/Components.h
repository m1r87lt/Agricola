/*
 * Components.h
 *
 *  Created on: 13 nov 2018
 *      Author: m1r
 */

#ifndef COMPONENTS_H_
#define COMPONENTS_H_

#include "Player.h"
#include <src/Gamecard.h>
#include <src/Logics.h>
#include <array>
#define WOODENPIECE(shape, color, attributes) WoodenPiece(shape, #shape, color, attributes)

namespace agr {

struct Board: public base::Ensemble {
	virtual Fields shows() const;
	virtual std::string prints() const;
	Board(Board&&);
protected:
	Board() = default;
	Board(Fields);
	friend Board construct_gameboard(Fields);
};

struct Farmyard final: private Board {
	struct Space final: public Ensemble {
		class Fence final: public Ensemble {
			bool vertical;
			std::array<Space*, 2> spaces;
			friend Farmyard;
			friend base::Ensemble;
			Fence(bool);
		public:
			bool is_vertical() const;
			Space* operator ()(bool) const;
			virtual Fields shows() const;
			virtual std::string prints() const;
		};
		enum Direction {
			S, E, W, N
		};

		std::tuple<Farmyard*, short unsigned, short unsigned> is_located() const;
		Ensemble* operator ()(Direction, bool = false) const;
		virtual Fields shows() const;
		virtual std::string prints() const;
	private:
		std::array<Space::Fence*, 4> fences;
		std::array<Space*, 4> adjacents;
		friend Ensemble;
		friend Farmyard;
		Space(std::array<Space::Fence*, 2>, std::array<Space**, 2>,
				std::array<Space::Fence**, 2>);
	};
	class Row final: public Object {
		short unsigned row;
		const Farmyard* owner;
		friend Farmyard;
		virtual Fields shows() const;
		virtual std::string prints() const;
		Row(short unsigned, const Farmyard&);
	public:
		Space& operator [](short unsigned) const;
	};
	class PersonalSupply final: public Ensemble {
		PersonalSupply() = default;
		friend Ensemble;
	public:
		virtual Fields shows() const;
		virtual std::string prints() const;
	};
	friend Space;
	PersonalSupply& gives_personal_supply() const;
	Row operator [](short unsigned) const;
	virtual Fields shows() const;
	virtual std::string prints() const;
	static Unique_ptr construct(Fields = Fields());
	static Board* cast(const Farmyard*);
private:
	Space* puts_next_space();
	Space* puts_next_space(Space&);
	void initializes();
	Farmyard();
	Farmyard(Fields);
};

class Colored: virtual public base::Object {
	Color color;
public:
	Color has_color() const;
	virtual Fields shows() const;
	Colored() = default;
	Colored(Color);
	virtual ~Colored() = default;
};
class Face: public base::Ensemble, public Colored {
	std::vector<game::Trigger*> prerequisite;
	std::string name;
	std::vector<Object*> cost;
	char deck;
	std::vector<game::Simulator::Choice*> events;
	bool bonus_points;
protected:
	Face(std::vector<game::Trigger*>, std::string, std::vector<Object*>, char,
			std::vector<game::Simulator::Choice*>, bool, Color, Fields =
					Fields());
public:
	std::vector<game::Trigger*> has_prerequisites() const;
	std::string has_name() const;
	std::vector<Object*> has_cost() const;
	char belongs() const;
	std::vector<game::Simulator::Choice*> has_events() const;
	bool has_bonus_points() const;
	virtual Fields shows() const;
	virtual ~Face() = default;
};
class Cover final: public base::Ensemble, public Colored {
	std::string name;
	Cover(std::string, Color, Fields = Fields());
public:
	std::string has_name() const;
	virtual Fields shows() const;
	virtual std::string prints() const;
	static Unique_ptr construct(std::string, Color, Fields = Fields());
};
class Numbered: virtual public base::Object {
	unsigned number;
public:
	unsigned is_number() const;
	virtual Fields shows() const;
	Numbered(unsigned number);
	virtual ~Numbered() = default;
};
class VictoryPointed: virtual public base::Object {
	short victory_points;
protected:
	VictoryPointed(short);
public:
	short has_victory_points() const;
	virtual Fields shows() const;
	virtual ~VictoryPointed() = default;
};
struct WoodenPiece: public virtual base::Element, public Colored {
	enum class Shape {
		disc, house, bar, cube, cylinder
	};

	bool has_same_shape_of(const WoodenPiece&) const;
	virtual Fields shows() const;
	virtual ~WoodenPiece() = default;
protected:
	WoodenPiece(Shape, const char*, Color, Fields = Fields());
private:
	Shape shape;
	std::string label;
};
struct Tile: public base::Element {
	virtual Fields shows() const;
	virtual ~Tile() = default;
protected:
	Tile() = default;
};

class FamilyMember: public WoodenPiece, public Owned {
	FamilyMember(Player&, Fields = Fields());
public:
	virtual Fields shows() const;
	virtual std::string prints() const;
	static base::Ensemble::Unique_ptr construct(Player&, Fields = Fields());
};
class Stable: public WoodenPiece, public Owned {
	Stable(Player&, Fields = Fields());
public:
	virtual Fields shows() const;
	virtual std::string prints() const;
	static base::Ensemble::Unique_ptr construct(Player&, Fields = Fields());
};
class Fence: public WoodenPiece, public Owned {
	Fence(Player&, Fields = Fields());
public:
	virtual Fields shows() const;
	virtual std::string prints() const;
	static base::Ensemble::Unique_ptr construct(Player&, Fields = Fields());
};

struct Resource: public WoodenPiece {
	virtual Fields shows() const;
	virtual ~Resource() = default;
protected:
	Resource(Color, Fields = Fields());
};
struct BuildResource: public Resource {
	virtual Fields shows() const;
	virtual ~BuildResource() = default;
protected:
	BuildResource(Color, Fields = Fields());
};
class Wood: public BuildResource {
	Wood();
public:
	virtual Fields shows() const;
	virtual std::string prints() const;
	static base::Ensemble::Unique_ptr construct(Fields = Fields());
};
class Clay: public BuildResource {
	Clay();
public:
	virtual Fields shows() const;
	virtual std::string prints() const;
	static base::Ensemble::Unique_ptr construct(Fields = Fields());
};
class Reed: public BuildResource {
	Reed();
public:
	virtual Fields shows() const;
	virtual std::string prints() const;
	static base::Ensemble::Unique_ptr construct(Fields = Fields());
};
class Stone: public BuildResource {
	Stone();
public:
	virtual Fields shows() const;
	virtual std::string prints() const;
	static base::Ensemble::Unique_ptr construct(Fields = Fields());
};

class Grain: public Resource {
	Grain();
public:
	virtual Fields shows() const;
	virtual std::string prints() const;
	static base::Ensemble::Unique_ptr construct(Fields = Fields());
};
class Vegetable: public Resource {
	Vegetable();
public:
	virtual Fields shows() const;
	virtual std::string prints() const;
	static base::Ensemble::Unique_ptr construct(Fields = Fields());
};

struct Animal: public WoodenPiece {
	virtual Fields shows() const;
	virtual ~Animal() = default;
protected:
	Animal(Color, Fields = Fields());
};
class Sheep: public Animal {
	Sheep();
public:
	virtual Fields shows() const;
	virtual std::string prints() const;
	static base::Ensemble::Unique_ptr construct(Fields = Fields());
};
class WildBoar: public Animal {
	WildBoar();
public:
	virtual Fields shows() const;
	virtual std::string prints() const;
	static base::Ensemble::Unique_ptr construct(Fields = Fields());
};
class Cattle: public Animal {
	Cattle();
public:
	virtual Fields shows() const;
	virtual std::string prints() const;
	static base::Ensemble::Unique_ptr construct(Fields = Fields());
};

class StartingPlayer: public WoodenPiece {
	StartingPlayer();
public:
	virtual Fields shows() const;
	virtual std::string prints() const;
	static base::Ensemble::Unique_ptr construct();
};

class WoodHut: public Tile {
	WoodHut() = default;
public:
	virtual Fields shows() const;
	virtual std::string prints() const;
	static base::Ensemble::Unique_ptr construct(Fields = Fields());
};
class ClayHut: public Tile {
	ClayHut() = default;
public:
	virtual Fields shows() const;
	virtual std::string prints() const;
	static base::Ensemble::Unique_ptr construct(Fields = Fields());
};
class StoneHouse: public Tile {
	StoneHouse() = default;
public:
	virtual Fields shows() const;
	virtual std::string prints() const;
	static base::Ensemble::Unique_ptr construct(Fields = Fields());
};
class Field: public Tile {
	Field() = default;
public:
	virtual Fields shows() const;
	virtual std::string prints() const;
	static base::Ensemble::Unique_ptr construct(Fields = Fields());
};

class Food: public base::Element {
	Food() = default;
public:
	virtual Fields shows() const;
	virtual std::string prints() const;
	static base::Ensemble::Unique_ptr construct(Fields = Fields());
};

} /* namespace agr */

namespace card {

class Occupation final: public agr::Face, public agr::Numbered {
	std::pair<short unsigned, bool> player_number;

	Occupation(std::vector<game::Trigger*>, std::string, std::vector<Object*>, char,
			std::vector<game::Simulator::Choice*>, bool, unsigned,
			short unsigned, bool = true, Fields = Fields());
public:
	std::pair<short unsigned, bool> has_player_number() const;
	virtual Fields shows() const;
	virtual std::string prints() const;
	static Unique_ptr construct(std::vector<game::Trigger*>, std::string,
			std::vector<Object*>, char, std::vector<game::Simulator::Choice*>, bool,
			unsigned, short unsigned, bool = true, Fields = Fields());
};

class Improvement: public agr::Face,
		public agr::Numbered,
		public agr::VictoryPointed {
	bool oven;
	bool kitchen;
protected:
	Improvement(std::vector<game::Trigger*>, std::string, std::vector<Object*>, char,
			std::vector<game::Simulator::Choice*>, bool, agr::Color, unsigned,
			short, bool, bool, Fields = Fields());
public:
	bool is_oven() const;
	bool is_kitchen() const;
	virtual Fields shows() const;
	virtual ~Improvement() = default;
};
class MinorImprovement final: public Improvement {
	MinorImprovement(std::vector<game::Trigger*>,
			std::string, std::vector<Object*>, char,
			std::vector<game::Simulator::Choice*>, bool,
			unsigned, short, bool, bool,
			Fields = Fields());
public:
	virtual std::string prints() const;
	static Unique_ptr construct(
			std::vector<game::Trigger*>, std::string,
			std::vector<Object*>, char,
			std::vector<game::Simulator::Choice*>, bool,
			unsigned, short, bool, bool,
			Fields = Fields());
	virtual ~MinorImprovement() = default;
};
class MajorImprovement final: public Improvement {
	MajorImprovement(std::vector<game::Trigger*>,
			std::string, std::vector<Object*>, char,
			std::vector<game::Simulator::Choice*>, bool,
			unsigned, short, bool, bool,
			Fields = Fields());
public:
	virtual ~MajorImprovement() = default;
	virtual std::string prints() const;
	static Unique_ptr construct(
			std::vector<game::Trigger*>, std::string,
			std::vector<Object*>, char,
			std::vector<game::Simulator::Choice*>, bool,
			unsigned, short, bool, bool,
			Fields = Fields());
};

class Action final: private base::Ensemble, public agr::Colored {
	template<typename ... Unique_pointers> void adds(Unique_ptr&& action,
			Unique_pointers&& ... actions) {
		gets(action->prints(), std::move(action));
		adds(std::move(actions ...));
	}
	void adds();
	template<typename ... Unique_pointers> Action(Fields attributes,
			Ensemble::Unique_ptr&& action, Unique_pointers&& ... actions) :
			Ensemble(attributes), Colored(COLOR(agr::Color::Which::Green)) {
		adds(std::move(action), std::move(actions) ...);
	}
public:
	std::vector<base::Ensemble*> includes();
	virtual Fields shows() const;
	virtual std::string prints() const;
	template<typename ... Unique_pointers> static Unique_ptr construct(
			Fields attributes, Unique_ptr&& action,
			Unique_pointers&& ... actions) {
		return Unique_ptr(
				new Action(attributes, std::move(action),
						std::move(actions) ...));
	}
};

class Begging final: public agr::Face, public agr::VictoryPointed {
	Begging(Fields);
public:
	virtual Fields shows() const;
	virtual std::string prints() const;
	static Unique_ptr construct(Fields = Fields());
};

} /* namespace card */

#endif /* COMPONENTS_H_ */

