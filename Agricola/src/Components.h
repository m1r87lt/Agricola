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
	virtual ~Board() = default;
	virtual Fields shows() const;
	virtual std::string prints() const;
	static Board construct_gameboard(Fields = Fields());
	Board(Board&&);
protected:
	Board() = default;
	Board(Fields);
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
		std::array<Fence*, 4> fences;
		std::array<Space*, 4> adjacents;
		friend Ensemble;
		friend Farmyard;
		Space(std::array<Fence*, 2>, std::array<Space**, 2>,
				std::array<Fence**, 2>);
	};
	class Row final: public Object {
		short unsigned row;
		const Farmyard* owner;
		friend Farmyard;
		virtual Fields shows() const;
		virtual std::string prints() const;
		Row(short unsigned, const Farmyard*);
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
class Face: public base::Element, public Colored {
	std::vector<Object*> prerequisite;
	std::string name;
	Quantity cost;
	char deck;
	std::vector<Object*> events;
	bool bonus_points;
protected:
	Face(std::vector<Object*>, std::string, Quantity, char,
			std::vector<Object*>, bool, Color, Fields = Fields());
public:
	std::vector<Object*> has_prerequisites() const;
	std::string has_name() const;
	Quantity has_cost() const;
	char belongs() const;
	std::vector<Object*> has_events() const;
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

class Occupation final: public agr::Face, public agr::Numbered {
	std::pair<short unsigned, bool> player_number;
	friend base::Ensemble::Unique_ptr construct_occupation(
			base::Primitive<unsigned>);
	Occupation(base::Class<std::vector<agr::Condition*>>,
			base::Class<std::string>, base::Quantity, base::Primitive<char>,
			base::Class<std::vector<Ensemble*>>, base::Primitive<bool>,
			base::Primitive<unsigned>, base::Class<std::pair<short, bool>>,
			const Log* = nullptr, base::Fields = nullptr);
public:
	base::Class<std::pair<short, bool>> has_player_number(
			const Log* = nullptr) const;
	virtual std::ostringstream prints() const;
};

class Improvement: public agr::Numbered, public agr::Face {
	base::Primitive<short> victory_points;
	base::Primitive<bool> oven;
	base::Primitive<bool> kitchen;
protected:
	Improvement(base::Class<std::vector<agr::Condition*>>,
			base::Class<std::string>, base::Quantity, base::Primitive<char>,
			base::Class<std::vector<Ensemble*>>, base::Primitive<bool>,
			agr::Color, base::Primitive<unsigned>, base::Primitive<short>,
			base::Primitive<bool>, base::Primitive<bool>, const Log* = nullptr,
			base::Fields = nullptr);
public:
	base::Primitive<short> has_victory_points(const Log* = nullptr) const;
	base::Primitive<bool> is_oven(const Log* = nullptr) const;
	base::Primitive<bool> is_kitchen(const Log* = nullptr) const;
	virtual std::ostringstream prints() const;

	virtual ~Improvement() = default;
};
class MinorImprovement: public Improvement {
	friend base::Ensemble::Unique_ptr construct_minor_improvement(
			base::Primitive<unsigned>);
	MinorImprovement(base::Class<std::vector<agr::Condition*>>,
			base::Class<std::string>, base::Quantity, base::Primitive<char>,
			base::Class<std::vector<Ensemble*>>, base::Primitive<bool>,
			base::Primitive<unsigned>, base::Primitive<short>,
			base::Primitive<bool>, base::Primitive<bool>, const Log* = nullptr,
			base::Fields = nullptr);
};
class MajorImprovement: public Improvement {
	friend base::Ensemble::Unique_ptr construct_major_improvement(
			base::Primitive<unsigned>);
	MajorImprovement(base::Class<std::vector<agr::Condition*>>,
			base::Class<std::string>, base::Quantity, base::Primitive<char>,
			base::Class<std::vector<Ensemble*>>, base::Primitive<bool>,
			base::Primitive<unsigned>, base::Primitive<short>,
			base::Primitive<bool>, base::Primitive<bool>, const Log* = nullptr,
			base::Fields = nullptr);
};

class Action final: public agr::Colored, private base::Ensemble {
	static const std::string type;
	friend Unique_ptr;
	template<typename ... Unique_pointers> void adds(const Log* caller,
			Ensemble::Unique_ptr&& action, Unique_pointers&& ... actions) {
		auto log = as_method(__func__, caller, typeid(void), action,
				actions ...);

		this->gets(action, std::move(action),
				base::Primitive < size_t > (1, &log), &log);
		adds(&log, std::move(actions ...));
	}
	void adds(const Log* caller);

	template<typename ... Unique_pointers> Action(const Log* caller,
			base::Fields attributes, Ensemble::Unique_ptr&& action,
			Unique_pointers&& ... actions) :
			ENSEMBLE(true, base::make_scopes(CARD, __func__), caller,
					attributes), Colored(
					LOGGED_COLOR(agr::Color::Which::Green, caller), caller) {
		auto log = as_constructor(CARD, __func__, caller, action, actions ...);

		adds(&log, std::move(action), std::move(actions) ...);
	}
public:
	base::Class<std::vector<Ensemble*>> includes(const Log* = nullptr);
	virtual std::ostringstream prints() const;
	template<typename ... Unique_pointers> static Unique_ptr construct(
			const Log* caller, base::Fields attributes,
			Ensemble::Unique_ptr&& action, Unique_pointers&& ... actions) {
		auto log = as_method(base::make_scopes(CARD, TYPEID(Action), __func__),
				true, caller, typeid(Unique_ptr), attributes, action,
				actions ...);

		return Unique_ptr::construct<Action>(&log,
				base::Primitive<const Log*>(&log, &log), attributes,
				std::move(action), std::move(actions) ...);
	}
};

class Begging final: public agr::Face {
	Begging(const Log*, base::Fields = nullptr);
	friend Unique_ptr;
public:
	virtual std::ostringstream prints() const;
	static Unique_ptr construct(const Log* = nullptr, base::Fields = nullptr);
};

} /* namespace card */

#endif /* COMPONENTS_H_ */

