/*
 * Components.h
 *
 *  Created on: 13 nov 2018
 *      Author: m1r
 */

#ifndef COMPONENTS_H_
#define COMPONENTS_H_

//#include <array>
#include "Logics.h"
#include <src/Gamecard.h>
#define BOARD(caller, label, open, attributes) Object(caller, label), Log(caller, label, open), Board(label, caller, attributes)
#define NUMBERED(caller, label, open, number) Object(caller, label), Log(caller, label, open), Numbered(number, caller)
#define WOODEN_SHAPE(caller, label, open, color, shape, attributes) Object(caller, label), Log(caller, label, open), WoodenPiece(color, shape, #shape, label, caller, attributes)
#define RESOURCE(color, label, open, caller, attributes) Object(caller, label), Log(caller, label, open), Resource(color, label, caller, attributes)
#define BUILDRESOURCE(color, label, open, caller, attributes) Object(caller, label), Log(caller, label, open), BuildResource(color, label, caller, attributes)
#define ANIMAL(caller, label, open, color, shape, attributes) Object(caller, label), Log(caller, label, open), WoodenPiece(color, shape, #shape, label, caller, attributes)
#define TILE(caller, label, open, attributes) Object(caller, label), Log(caller, label, open), Tile(label, caller, attributes)
#define SPACE_NAME "Space"
#define PERSONAL_SUPPLY_NAME "Personal Supply"
#define CARD_MAJOR_IMPROVEMENT "Major Improvement"
#define CARD_MINOR_IMPROVEMENT "Minor Improvement"
#define CARD_OCCUPATION "Occupation"

namespace agr {

struct Board: public base::Ensemble {
	virtual ~Board() = default;
	Board(Board&&);
protected:
	Board(std::string, const Log* = nullptr, base::Fields = nullptr);
	friend Board construct_gameboard(const Log*);
};

template<typename Type, size_t N> std::ostringstream print_std__array(
		const std::array<Type, N>& container) {
	return base::Container_Printer(container, "\t", "")();
}
template<typename First, typename Second> std::ostringstream print_std__pair(
		const std::pair<First, Second>& pair) {
	return std::ostringstream(
			base::Container_Printer(std::string(), pair, "{ ", "; ", " }")(
					false).str().substr(1));
}
struct Farmyard final: private Board {
	struct Space final: public Ensemble {
		struct Fence: public Log {
			base::Primitive<Element*> which;
			base::Primitive<bool> vertical;
			base::Class<std::array<Space*, 2>> spaces;

			Fence(const Log*);
		};
		enum Direction {
			N, S, W, E
		};

		base::Class<std::tuple<Farmyard*, short, short>> is_located(const Log* =
				nullptr) const;
		base::Primitive<Element*> operator ()(base::Primitive<Direction>,
				const Log* = nullptr);
	private:
		base::Class<std::array<Fence*, 4>> fences;
		friend class Row;
		friend Farmyard;
		friend Ensemble::Unique_ptr;
		Space(const Log*, base::Fields = nullptr);
	};
	class Row final: public Log {
		base::Primitive<short> row;
		base::Primitive<Farmyard*> owner;
		friend Farmyard;
		Row(base::Primitive<short>, const Farmyard&, const Log* = nullptr);
		Row(const Row&);
		Row& operator =(const Row&);
	public:
		Space& operator [](base::Primitive<short>) const;
	};
	Ensemble& personal_supply(const Log* = nullptr) const;
	Row operator [](base::Primitive<short>) const;
	static Ensemble::Unique_ptr construct(const Log* = nullptr);
private:
	Farmyard(const Log*, base::Fields = nullptr);
	Farmyard(Farmyard&&) = delete;
	friend Ensemble::Unique_ptr;
	friend Farmyard& give_farmyard(Player&, const Log*);
};

class Colored: virtual public base::Log {
	Color color;
public:
	Color has_color(const Log* = nullptr) const;
	virtual std::ostringstream prints() const = 0;

	Colored(Color, const Log* = nullptr);
	virtual ~Colored() = default;
	Colored(const Colored&);
	Colored& operator =(const Colored&);
};
class Face: public base::Ensemble, public Colored {
	base::Class<std::vector<Condition*>> prerequisite;
	base::Class<std::string> name;
	base::Quantity cost;
	base::Primitive<char> deck;
	base::Class<std::vector<Ensemble*>> events;
	base::Primitive<bool> bonus_points;
protected:
	Face(base::Class<std::vector<Condition*>>, base::Class<std::string>,
			base::Quantity, base::Primitive<char>,
			base::Class<std::vector<Ensemble*>>, base::Primitive<bool>, Color,
			std::string, const Log* = nullptr, base::Fields = nullptr);
public:
	base::Class<std::vector<Condition*>> has_prerequisites(
			const Log* = nullptr) const;
	base::Class<std::string> has_name(const Log* = nullptr) const;
	base::Quantity has_cost(const Log* = nullptr) const;
	base::Primitive<char> belongs(const Log* = nullptr) const;
	base::Class<std::vector<Ensemble*>> has_events(const Log* = nullptr) const;
	base::Primitive<bool> has_bonus_points(const Log* = nullptr) const;

	virtual ~Face() = default;
};
class Cover final: public base::Ensemble, public Colored {
	base::Class<std::string> name;
	friend Ensemble::Unique_ptr;
	Cover(base::Class<std::string>, Color, const Log* = nullptr, base::Fields =
			nullptr);
public:
	base::Class<std::string> has_name(const Log* = nullptr) const;
	virtual std::ostringstream prints() const;
	static Ensemble::Unique_ptr construct(base::Class<std::string>, Color,
			const Log* = nullptr, base::Fields = nullptr);
};
class Numbered: virtual public base::Log {
	base::Primitive<unsigned> number;
public:
	base::Primitive<unsigned> is_number(const Log* = nullptr) const;
	virtual std::ostringstream prints() const = 0;

	Numbered(base::Primitive<unsigned> number, const Log* = nullptr);
	virtual ~Numbered() = default;
	Numbered(const Numbered&);
	Numbered& operator =(const Numbered&);
};

struct WoodenPiece: public base::Element, public Colored {
	enum class Shape {
		disc, house, bar, cube, cylinder
	};

	base::Primitive<bool> has_same_shape_of(const WoodenPiece&) const;
	virtual std::ostringstream prints() const;

	virtual ~WoodenPiece() = default;
protected:
	WoodenPiece(Color, Shape, base::Primitive<const char*>, std::string,
			const Log* = nullptr, base::Fields = nullptr);
private:
	Shape shape;
	const char* label;
};
class FamilyMember: public WoodenPiece, public Owned {
	FamilyMember(Player&, const Log* = nullptr, base::Fields = nullptr);
public:
	virtual std::ostringstream prints() const;
	friend base::Unique_ptr;
	static base::Unique_ptr construct(Player&, const Log* = nullptr,
			base::Fields = nullptr);
};
class Stable: public WoodenPiece, public Owned {
	Stable(Player&, const Log* = nullptr, base::Fields = nullptr);
public:
	virtual std::ostringstream prints() const;
	friend base::Unique_ptr;
	static base::Unique_ptr construct(Player&, const Log* = nullptr,
			base::Fields = nullptr);
};
class Fence: public WoodenPiece, public Owned {
	Fence(Player&, const Log* = nullptr, base::Fields = nullptr);
public:
	virtual std::ostringstream prints() const;
	friend base::Unique_ptr;
	static base::Unique_ptr construct(Player&, const Log* = nullptr,
			base::Fields = nullptr);
};

struct Resource: public WoodenPiece {
	virtual ~Resource() = default;
protected:
	Resource(Color, std::string, const Log* = nullptr, base::Fields = nullptr);
};
struct BuildResource: public Resource {
	virtual ~BuildResource() = default;
protected:
	BuildResource(Color, std::string, const Log* = nullptr, base::Fields =
			nullptr);
};
class Wood: public BuildResource {
	Wood(const Log*, base::Fields = nullptr);
public:
	friend base::Unique_ptr;
	static base::Unique_ptr construct(const Log* = nullptr, base::Fields =
			nullptr);
};
class Clay: public BuildResource {
	Clay(const Log*, base::Fields = nullptr);
public:
	friend base::Unique_ptr;
	static base::Unique_ptr construct(const Log* = nullptr, base::Fields =
			nullptr);
};
class Reed: public BuildResource {
	Reed(const Log*, base::Fields = nullptr);
public:
	friend base::Unique_ptr;
	static base::Unique_ptr construct(const Log* = nullptr, base::Fields =
			nullptr);
};
class Stone: public BuildResource {
	Stone(const Log*, base::Fields = nullptr);
public:
	friend base::Unique_ptr;
	static base::Unique_ptr construct(const Log* = nullptr, base::Fields =
			nullptr);
};
class Grain: public Resource {
	Grain(const Log*, base::Fields = nullptr);
public:
	friend base::Unique_ptr;
	static base::Unique_ptr construct(const Log* = nullptr, base::Fields =
			nullptr);
};
class Vegetable: public Resource {
	Vegetable(const Log*, base::Fields = nullptr);
public:
	friend base::Unique_ptr;
	static base::Unique_ptr construct(const Log* = nullptr, base::Fields =
			nullptr);
};

class Animal: public WoodenPiece {
	virtual ~Animal() = default;
protected:
	Animal(Color, std::string, const Log* = nullptr, base::Fields = nullptr);
};
class Sheep: public Animal {
	Sheep(const Log*, base::Fields = nullptr);
public:
	friend base::Unique_ptr;
	static base::Unique_ptr construct(const Log* = nullptr, base::Fields =
			nullptr);
};
class WildBoar: public Animal {
	WildBoar(const Log*, base::Fields = nullptr);
public:
	friend base::Unique_ptr;
	static base::Unique_ptr construct(const Log* = nullptr, base::Fields =
			nullptr);
};
class Cattle: public Animal {
	Cattle(const Log*, base::Fields = nullptr);
public:
	friend base::Unique_ptr;
	static base::Unique_ptr construct(const Log* = nullptr, base::Fields =
			nullptr);
};
class StartingPlayer: public WoodenPiece {
	StartingPlayer(const Log*);
public:
	friend base::Unique_ptr;
	static base::Unique_ptr construct(const Log* = nullptr);
};
struct Tile: public base::Element {
	virtual std::ostringstream prints() const;

	virtual ~Tile() = default;
protected:
	Tile(std::string, const Log*, base::Fields = nullptr);
};
class WoodHut: public Tile {
	WoodHut(const Log*, base::Fields = nullptr);
public:
	friend base::Unique_ptr;
	base::Unique_ptr construct(const Log* = nullptr, base::Fields = nullptr);
};
class ClayHut: public Tile {
	ClayHut(const Log*, base::Fields = nullptr);
public:
	friend base::Unique_ptr;
	base::Unique_ptr construct(const Log* = nullptr, base::Fields = nullptr);
};
class StoneHouse: public Tile {
	StoneHouse(const Log*, base::Fields = nullptr);
public:
	friend base::Unique_ptr;
	base::Unique_ptr construct(const Log* = nullptr, base::Fields = nullptr);
};
class Field: public Tile {
	Field(const Log*, base::Fields = nullptr);
public:
	friend base::Unique_ptr;
	base::Unique_ptr construct(const Log* = nullptr, base::Fields = nullptr);
};
class Food: public base::Element {
	Food(const Log*, base::Fields = nullptr);
public:
	friend base::Unique_ptr;
	static base::Unique_ptr construct(const Log* = nullptr, base::Fields =
			nullptr);
};

template<typename Base, typename Derived> std::string log_wrong_deriving(
		base::Log& logger) {
	std::ostringstream result("ERROR '");

	result << typeid(Derived).name() << "' is not derived from '"
			<< typeid(Base).name() << "'.";
	logger.logs_error(result);

	return result.str();
}
template<typename Base, typename Derived> std::domain_error throw_wrong_deriving(
		base::Log& logger) {
	return std::domain_error(log_wrong_deriving<Base, Derived>(logger));
}
template<typename Base, typename Derived> void derive(base::Log& caller) {
	if (!std::is_base_of<Base, Derived>::value)
		throw_wrong_deriving<Base, Derived>(caller);
}

} /* namespace agr */

namespace card {
#define CARD "card"

class Occupation final: public agr::Numbered, public agr::Face {
	base::Class<std::pair<short, bool>> player_number;
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

		this->gets(action, std::move(action), base::Primitive<size_t>(1, &log),
				&log);
		adds(&log, std::move(actions ...));
	}
	void adds(const Log* caller);

	template<typename ... Unique_pointers> Action(const Log* caller,
			base::Fields attributes, Ensemble::Unique_ptr&& action,
			Unique_pointers&& ... actions) :
			ENSEMBLE(true, base::make_scopes(CARD, __func__), caller, attributes), Colored(
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

