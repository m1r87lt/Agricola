/*
 * Components.h
 *
 *  Created on: 13 nov 2018
 *      Author: m1r
 */

#ifndef COMPONENTS_H_
#define COMPONENTS_H_

#include "Logics.h"
#include <src/Gamecard.h>
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
	std::vector<Conditional*> prerequisite;
	std::string name;
	Quantity cost;
	char deck;
	std::vector<Operation*> events;
	bool bonus_points;
protected:
	Face(std::vector<Conditional*>, std::string, Quantity, char,
			std::vector<Operation*>, bool, Color, Fields = Fields());
public:
	std::vector<Conditional*> has_prerequisites() const;
	std::string has_name() const;
	Quantity has_cost() const;
	char belongs() const;
	std::vector<Operation*> has_events() const;
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
struct WoodenPiece: public base::Element, public Colored {
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
struct Tile: public base::Element {
	virtual Fields shows() const;
	virtual ~Tile() = default;
protected:
	Tile() = default;
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

template<typename Trigger, typename Operator> class Occupation final: public agr::Face,
		public agr::Numbered,
		public agr::Event<Trigger, Operator> {
	std::pair<short unsigned, bool> player_number;

	Occupation(std::vector<agr::Conditional*> prerequisites, std::string name,
			agr::Quantity cost, char deck, std::vector<agr::Operation*> events,
			bool bonus_points, unsigned number, short unsigned player_number,
			bool more = true, Fields attributes = Fields()) :
			Face(prerequisites, name, cost, deck, events, bonus_points,
					COLOR(agr::Color::Which::Yellow), attributes), Numbered(
					number), agr::Event<Trigger, Operator>() {
		this->player_number = std::make_pair(player_number, more);
	}
public:
	std::pair<short unsigned, bool> has_player_number() const {
		return player_number;
	}
	virtual Fields shows() const {
		auto result = Face::shows();
		auto numbered = Numbered::shows();
		auto event = agr::Event<Trigger, Operator>::shows();

		result.insert(numbered.begin(), numbered.end());
		result.insert(event.begin(), event.end());
		result.insert(VARIABLE(player_number));

		return result;
	}
	virtual std::string prints() const {
		return std::string("card::Occupation< ")
				+ std::type_index(typeid(Trigger)).name() + ", "
				+ std::type_index(typeid(Operator)).name() + " >{ "
				+ std::to_string(is_number()) + " }";
	}
	static Ensemble::Unique_ptr construct(
			std::vector<agr::Conditional*> prerequisites, std::string name,
			agr::Quantity cost, char deck, std::vector<agr::Operation*> events,
			bool bonus_points, unsigned number, short unsigned player_number,
			bool more = true, Fields attributes = Fields()) {
		return Ensemble::Unique_ptr(
				new Occupation(prerequisites, name, cost, deck, events,
						bonus_points, number, player_number, more, attributes));
	}
};

template<typename Trigger, typename Operator> class Improvement: public agr::Face,
		public agr::Numbered,
		public agr::VictoryPointed,
		public agr::Event<Trigger, Operator> {
	bool oven;
	bool kitchen;
protected:
	Improvement(std::vector<agr::Conditional*> prerequisites, std::string name,
			agr::Quantity cost, char deck, std::vector<agr::Operation*> events,
			bool bonus_points, agr::Color color, unsigned number,
			short victory_points, bool oven, bool kitchen, Fields attributes =
					Fields()) :
			agr::Face(prerequisites, name, cost, deck, events, bonus_points,
					color, attributes), Numbered(number), VictoryPointed(
					victory_points), agr::Event<Trigger, Operator>() {
		this->oven = oven;
		this->kitchen = kitchen;
	}
public:
	short has_victory_points() const {
		return victory_points;
	}
	bool is_oven() const {
		return oven;
	}
	bool is_kitchen() const {
		return kitchen;
	}
	virtual Fields shows() const {
		auto result = Face::shows();
		auto numbered = Numbered::shows();
		auto event = agr::Event<Trigger, Operator>::shows();

		result.insert(numbered.begin(), numbered.end());
		result.insert(event.begin(), event.end());
		result.insert(VARIABLE(victory_points));
		result.insert(VARIABLE(oven));
		result.insert(VARIABLE(kitchen));

		return result;
	}
	virtual ~Improvement() = default;
};
template<typename Trigger, typename Operator> class MinorImprovement final: public Improvement<
		Trigger, Operator> {
	MinorImprovement(std::vector<agr::Conditional*> prerequisites,
			std::string name, agr::Quantity cost, char deck,
			std::vector<agr::Operation*> events, bool bonus_points,
			unsigned number, short victory_points, bool oven, bool kitchen,
			base::Object::Fields attributes = base::Object::Fields()) :
			Improvement<Trigger, Operator>(prerequisites, name, cost, deck,
					events, bonus_points, number, victory_points, oven, kitchen,
					attributes) {
	}
public:
	virtual std::string prints() const {
		return std::string("card::MinorImprovement< ")
				+ std::type_index(typeid(Trigger)).name() + ", "
				+ std::type_index(typeid(Operator)).name() + " >{ "
				+ std::to_string(agr::Numbered::is_number()) + " }";
	}
	static base::Ensemble::Unique_ptr construct(
			std::vector<agr::Conditional*> prerequisites, std::string name,
			agr::Quantity cost, char deck, std::vector<agr::Operation*> events,
			bool bonus_points, unsigned number, short victory_points, bool oven,
			bool kitchen, base::Object::Fields attributes =
					base::Object::Fields()) {
		return base::Ensemble::Unique_ptr(
				new MinorImprovement<Trigger, Operator>(prerequisites, name,
						cost, deck, events, bonus_points, number,
						victory_points, oven, kitchen, attributes));
	}
};
template<typename Trigger, typename Operator> class MajorImprovement final: public Improvement<
		Trigger, Operator> {
	MajorImprovement(std::vector<agr::Conditional*> prerequisites,
			std::string name, agr::Quantity cost, char deck,
			std::vector<agr::Operation*> events, bool bonus_points,
			unsigned number, short victory_points, bool oven, bool kitchen,
			base::Object::Fields attributes = base::Object::Fields()) :
			Improvement<Trigger, Operator>(prerequisites, name, cost, deck,
					events, bonus_points, number, victory_points, oven, kitchen,
					attributes) {
	}
public:
	virtual std::string prints() const {
		return std::string("card::MajorImprovement< ")
				+ std::type_index(typeid(Trigger)).name() + ", "
				+ std::type_index(typeid(Operator)).name() + " >{ "
				+ std::to_string(agr::Numbered::is_number()) + " }";
	}
	static base::Ensemble::Unique_ptr construct(
			std::vector<agr::Conditional*> prerequisites, std::string name,
			agr::Quantity cost, char deck, std::vector<agr::Operation*> events,
			bool bonus_points, unsigned number, short victory_points, bool oven,
			bool kitchen, base::Object::Fields attributes =
					base::Object::Fields()) {
		return base::Ensemble::Unique_ptr(
				new MajorImprovement<Trigger, Operator>(prerequisites, name,
						cost, deck, events, bonus_points, number,
						victory_points, oven, kitchen, attributes));
	}
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

