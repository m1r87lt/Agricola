/*
 * components.h
 *
 *  Created on: 30 dic 2017
 *      Author: m1r
 */

#ifndef COMPONENTS_H_
#define COMPONENTS_H_

#include "executable.h"

#define C_FARMYARD "Farmyard"
#define C_PERSONAL_SUPPLY "Personal Supply"
#define C_SPACE "Space"
#define C_FARMYARD_FENCE_SPACE C_FARMYARD + std::string("Fence") + C_SPACE
#define C_FARMYARD_SPACE C_FARMYARD + std::string(C_SPACE)

class Farmyard final: private base::Location, public Owned {
	Farmyard(const Player&, const Log*);
	friend Player;
public:
	class PersonalSupply final: public base::Location {
		PersonalSupply(Farmyard*, const Log*);
		friend Farmyard;
	};
	class Space final: public base::Location {
		Space(Farmyard*, const Log*);
		friend Farmyard;
	};
	class FenceSpace final: public base::Location {
		FenceSpace(Farmyard*, const Log*);
		friend Farmyard;
	};
	class Row: public Log {
		Farmyard* owner;
		short unsigned row;

		Row(short unsigned, Farmyard*, const Log*);
		friend Farmyard;
	public:
		Space* operator [](short unsigned);
	};

	PersonalSupply* personal_supply() const;
	Space* space(short unsigned, short unsigned) const;
	FenceSpace* fence(short unsigned, short unsigned, bool, bool) const;
	Row operator [](short unsigned) const;
};
class Board: public base::Location {
	Board(const Log*);
public:
	static Board* construct(const Log*);
};
class MajorImprovements final: public Board {
	MajorImprovements(const Log*);
public:
	std::vector<std::string> gives_them() const;
	static MajorImprovements* construct(const Log*);
};

struct Data {
	int quantity;
	std::type_index type;
	short unsigned player;
	std::string note;

	operator const char*() const;
	static std::string transcode(const Data&);
	static Data interpret(std::string);

	Data();
	Data(int, std::type_index, short unsigned, std::string, const base::Log*);
};
struct Description {
	std::vector<std::string> text;
	std::vector<Data> symbols;

	operator const char*() const;
	static std::string transcode(const Description&);

	Description();
	Description(std::string, const base::Log*);
};
struct Event: public Executable, public base::Location {
	virtual std::string has_caption() const;
protected:
	Description caption;

	Event(std::string, std::string, int (*)(bool, Player*, const Log*),
			Location*, std::map<std::string, std::string>, const Log*);
};
class Action: public Event {
	std::vector<Data> collection;

	Action(std::string, std::vector<Data>, std::string,
			int (*)(bool, Player*, const Log*), Location*,
			std::map<std::string, std::string>, const Log*);
public:
	std::vector<Data>& is_collecting() const;
	void collects(const Log*);
	static std::unique_ptr<Action> construct(std::string, std::vector<Data>,
			std::string, int (*)(bool, Player*, const Log*),
			std::map<std::string, std::string>, const Log*);
};
struct Effect: public Event {
	virtual bool verifies() const = 0;

	virtual ~Effect() = default;
protected:
	Effect(std::string, std::string, int (*)(bool, Player*, const Log*),
			Location*, std::map<std::string, std::string>, const Log*);
};

class Face: public base::Location {
	Color color;
	short unsigned minimal_players;

	Face(Color, std::string, Location*, std::map<std::string, std::string>,
			const Log*);
protected:
	Description caption;
public:
	short unsigned with_players() const;
	const Color& has_color() const;
	std::string has_caption() const;
	base::Variable<const Face&> gives_variable(std::string) const;
	virtual std::string has_name() const = 0;
	static std::unique_ptr<Location> construct(Color, short unsigned,
			std::string, std::map<std::string, std::string>, const Log*);
};
struct Cover final: public Face {
	enum class Type {
		no, round, occupation, minorImprovement, majorImprovement
	};

	virtual std::string has_name() const;
	operator Type() const;
	base::Variable<const Cover&> gives_variable(std::string) const;
	static Type correspond(std::string);
	static std::string transcode(const Cover&);
	static std::unique_ptr<Location> construct(const Log*);
	static std::unique_ptr<Location> construct(Type, const Log*);
	static std::unique_ptr<Location> construct(std::string, const Log*);

	Cover(const Log*);
	Cover(Type, const Log*);
	Cover(std::string, const Log*);
	Cover(const Cover&);
	Cover& operator =(const Cover&);
private:
	Type type;
	static const std::map<Type, std::string> names;

	Cover();
};
class Sheet: public Face {
	std::vector<Data> prerequisites;
	std::string name;
	std::vector<Data> costs;
	short vp;
	unsigned label;
	char edition;
	static std::map<unsigned, Face*> cards;
protected:
	Sheet(std::vector<Data>, std::string, std::vector<Data>, unsigned, char,
			std::string, Location*, std::map<std::string, std::string>,
			const Log*);
public:
	const std::vector<Data>& has_prerequisites() const;
	const std::vector<Data>& has_costs() const;
	short is_worth() const;
	unsigned has_label() const;
	char has_edition() const;
	base::Variable<const Sheet&> gives_variable(std::string) const;
	virtual std::string has_name() const;
	virtual Event& happens(size_t) = 0;
	static std::string transcode(const Sheet&);

	virtual ~Sheet();
};
namespace card {
class Occupation final: public Sheet, public Effect {
	bool bp;

	Occupation(bool, std::vector<Data>, std::string, std::vector<Data>,
			unsigned, char, std::string, int (*)(bool, Player*, const Log*),
			std::string, Location*, std::map<std::string, std::string>,
			const Log*);
public:
	bool bonus_points() const;
	virtual bool verifies() const;
	virtual Event& happens(size_t) const;
	static std::unique_ptr<game::Card> construct(unsigned, const Log*);
};
class Improvement final: public Sheet {
	std::vector<std::unique_ptr<Event>> events;
	bool o;
	bool k;

	Improvement(bool, bool, short, std::vector<std::unique_ptr<Event>>,
			std::vector<Data>, std::string, std::vector<Data>, unsigned, char,
			std::string, Location*, std::map<std::string, std::string>,
			const Log*);
public:
	bool oven() const;
	bool kitchen() const;
	virtual bool verifies() const;
	virtual Event& happens(size_t) const;
	static std::unique_ptr<game::Card> construct(unsigned, const Log*);
};
class Round final: public Face, public Action {
	short unsigned staging;
	short unsigned min;
	short unsigned max;

	Round(unsigned, unsigned, unsigned, Color, short unsigned, std::string,
			std::vector<Data>, std::string, int (*)(bool, Player*, const Log*),
			Location*, std::map<std::string, std::string>, const Log*);
public:
	short unsigned is_stage() const;
	std::pair<short unsigned, short unsigned> between_rounds() const;
	static std::unique_ptr<Round> construct(std::string, const Log*);
};
class Action final: public Face, public ::Action {
	bool family;

	Action(bool, Color, short unsigned, std::string, std::vector<Data>,
			std::string, int (*)(bool, Player*, const Log*), Location*,
			std::map<std::string, std::string>, const Log*);
public:
	bool for_family() const;
	static std::unique_ptr<card::Action> construct(std::string, const Log*);
};
struct Begging final: public Sheet {
	virtual Event& happens(size_t) const;
	static std::unique_ptr<game::Card> construct(const Log*);

	Begging(Location*, const Log*);
};
}

struct Wooden: public base::Object, public Owned {
	enum class Shape {
		disc, house, line, coin, cube, cylinder
	};
	enum class Type {
		Wood,
		Clay,
		Reed,
		Stone,
		Grain,
		Vegetable,
		Sheep,
		WildBoar,
		Cattle,
		FamilyMember,
		Stable,
		Fence,
		Token
	};

	const Color& has_color() const;
	Shape has_shape() const;
	Type is_type() const;

	static Shape get_shape(std::string);
	static Type get_type(std::string);
	static std::string transcode(const Wooden&);
	static std::unique_ptr<Wooden> wood(const Log*);
	static std::unique_ptr<Wooden> clay(const Log*);
	static std::unique_ptr<Wooden> reed(const Log*);
	static std::unique_ptr<Wooden> stone(const Log*);
	static std::unique_ptr<Wooden> grain(const Log*);
	static std::unique_ptr<Wooden> vegetable(const Log*);
	static std::unique_ptr<Wooden> sheep(const Log*);
	static std::unique_ptr<Wooden> wild_boar(const Log*);
	static std::unique_ptr<Wooden> cattle(const Log*);
	static std::unique_ptr<Wooden> family_member(short unsigned, const Log*);
	static std::unique_ptr<Wooden> stable(short unsigned, const Log*);
	static std::unique_ptr<Wooden> fence(short unsigned, const Log*);
	static Wooden* token(const Log*);
private:
	Color color;
	Shape shape;
	Type type;
	static const std::map<Shape, std::string> shapeNames;
	static const std::map<Type, std::string> typeNames;

	Wooden(short unsigned, Color, Shape, Type, base::Location*,
			std::map<std::string, std::string>, const Log*);
};

struct Tile: public base::Location {
	enum class Type {
		WoodenHut, ClayHut, StoneHouse, Field
	};

	const Color& has_color() const;
	Type has_type() const;

	static std::unique_ptr<Tile> wooden_hut(const Log*);
	static std::unique_ptr<Tile> clay_hut(const Log*);
	static std::unique_ptr<Tile> stone_house(const Log*);
	static std::unique_ptr<Tile> field(const Log*);
private:
	Color c;
	Type type;

	Tile(Color, Type, base::Location*, std::map<std::string, std::string>,
			const Log*);
};

class Food: public base::Object {
	Food(base::Location*, std::map<std::string, std::string>, const Log*);
public:
	static std::unique_ptr<Food> construct(const Log*);
};
class Token: public base::Object {
	bool guest;

	Token(bool, base::Location*, std::map<std::string, std::string>,
			const Log*);
public:
	static std::unique_ptr<Token> construct(bool,
			std::map<std::string, std::string>, const Log*);
};

#endif /* COMPONENTS_H_ */
