/*
 * Components.h
 *
 *  Created on: 13 nov 2018
 *      Author: m1rma
 */

#ifndef COMPONENTS_H_
#define COMPONENTS_H_

//#include <array>
#include "Logics.h"
#include <src/Gamecard.h>
#define BOARD(caller, label, open, attributes) Object(caller, label), Log(caller, label, open), Board(label, caller, attributes)
#define NUMBERED(caller, label, open, number) Object(caller, label), Log(caller, label, open), Numbered(number, caller)
#define SPACE_NAME "Space"
#define PERSONAL_SUPPLY_NAME "Personal Supply"
#define CARD_MAJOR_IMPROVEMENT "Major Improvement"
#define CARD_MINOR_IMPROVEMENT "Minor Improvement"
#define CARD_OCCUPATION "Occupation"
#define CARD_ROUND "Round"

namespace agr {

struct Board: public base::Ensemble {
	virtual ~Board() = default;
protected:
	Board(std::string, const Log*, base::Fields = nullptr);
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
		friend base::Unique_ptr;
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
	static base::Unique_ptr construct(const Log* = nullptr);
private:
	Farmyard(const Log*, base::Fields = nullptr);
	friend base::Unique_ptr;
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
	base::Class<std::map<std::type_index, int>> cost;
	base::Primitive<char> deck;
	base::Class<std::vector<Event*>> events;
	base::Primitive<bool> bonus_points;
protected:
	Face(base::Class<std::vector<Condition*>>, base::Class<std::string>,
			base::Quantity, base::Primitive<char>,
			base::Class<std::vector<Event*>>, base::Primitive<bool>, Color,
			std::string, const Log* = nullptr, base::Fields = nullptr);
public:
	base::Class<std::vector<Condition*>> has_prerequisites(
			const Log* = nullptr) const;
	base::Class<std::string> has_name(const Log* = nullptr) const;
	base::Quantity has_cost(const Log* = nullptr) const;
	base::Primitive<char> belongs(const Log* = nullptr) const;
	base::Class<std::vector<Event*>> has_events(const Log* = nullptr) const;
	base::Primitive<bool> has_bonus_points(const Log* = nullptr) const;

	virtual ~Face() = default;
};
class Cover final: public base::Ensemble, public Colored {
	base::Class<std::string> name;
	friend base::Unique_ptr;
	Cover(base::Class<std::string>, Color, const Log* = nullptr, base::Fields =
			nullptr);
public:
	base::Class<std::string> has_name(const Log* = nullptr) const;
	virtual std::ostringstream prints() const;
	static base::Unique_ptr construct(base::Class<std::string>, Color,
			const Log* = nullptr, base::Fields = nullptr);
};
class Actions final: public Colored, private base::Ensemble {
	static const agr::Color color;
	static const std::string action;

	template<typename ... Unique_pointers> void adds(const Log* caller,
			base::Unique_ptr&& action, Unique_pointers&& ... actions) {
		auto log = as_method(__func__, caller, typeid(void), action,
				actions ...);

		this->gets(action, std::move(action), base::Primitive<size_t>(1, &log),
				&log);
		adds(&log, actions ...);
	}
	void adds(const Log* caller);
	friend base::Unique_ptr;
	template<typename ... Unique_pointers> Actions(const Log* caller,
			base::Fields attributes = nullptr, Unique_pointers&& ... actions) :
			ENSEMBLE(true, base::make_scopes(AGR, __func__), caller, attributes), Colored(
					LOGGED_COLOR(color, caller), caller) {
		auto log = as_constructor(AGR, __func__, caller, actions ...);

		adds(&log, actions ...);
	}
public:
	base::Class<std::vector<agr::Action*>> includes(const Log* = nullptr);
	template<typename ... Unique_pointers> static base::Unique_ptr construct(
			const Log* caller, base::Fields attributes = nullptr,
			Unique_pointers&& ... actions) {
		auto log = as_method(base::make_scopes(AGR, TYPEID(Action), __func__),
				true, caller, typeid(base::Unique_ptr), attributes,
				actions ...);

		return log.returns(
				base::Unique_ptr::construct<Actions>(&log,
						base::Primitive<const Log*>(&log, &log), attributes,
						actions ...));
	}
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

/*struct WoodenPiece: public base::Element, virtual public Colored {
 enum class Shape {
 disc, house, bar, cube
 };
 enum class Type {
 Family_member, Stable, Fence, token, counter
 };

 };
 */
} /* namespace agr */

namespace card {

class Occupation: public agr::Face, public virtual agr::Numbered {
	base::Class<std::pair<short, bool>> player_number;
	static const agr::Color color;
protected:
	Occupation(base::Class<std::vector<agr::Condition*>>,
			base::Class<std::string>, base::Quantity, base::Primitive<char>,
			base::Class<std::vector<agr::Event*>>, base::Primitive<bool>,
			base::Primitive<unsigned>, base::Class<std::pair<short, bool>>,
			const Log* = nullptr, base::Fields = nullptr);
public:
	base::Class<std::pair<short, bool>> has_player_number(
			const Log* = nullptr) const;
	virtual std::ostringstream prints() const;
	static game::Deck::Unique_ptr construct(base::Primitive<unsigned>,
			const Log* = nullptr, base::Fields = nullptr);
};

class Improvement: public agr::Face, public virtual agr::Numbered {
	base::Primitive<short> victory_points;
	base::Primitive<bool> oven;
	base::Primitive<bool> kitchen;
protected:
	Improvement(base::Class<std::vector<agr::Condition*>>,
			base::Class<std::string>, base::Quantity, base::Primitive<char>,
			base::Class<std::vector<agr::Event*>>, base::Primitive<bool>,
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
	static const agr::Color color;
protected:
	MinorImprovement(base::Class<std::vector<agr::Condition*>>,
			base::Class<std::string>, base::Quantity, base::Primitive<char>,
			base::Class<std::vector<agr::Event*>>, base::Primitive<bool>,
			agr::Color, base::Primitive<unsigned>, base::Primitive<short>,
			base::Primitive<bool>, base::Primitive<bool>, const Log* = nullptr,
			base::Fields = nullptr);
public:
	virtual std::ostringstream prints() const;
	static game::Deck::Unique_ptr construct(base::Primitive<unsigned>,
			const Log* = nullptr, base::Fields = nullptr);
};
class MajorImprovement: public Improvement {
	static const agr::Color color;
protected:
	MajorImprovement(base::Primitive<unsigned>, const Log* = nullptr,
			base::Fields = nullptr);
public:
	virtual std::ostringstream prints() const;
	static game::Deck::Unique_ptr construct(base::Primitive<unsigned>,
			const Log* = nullptr, base::Fields = nullptr);
};
class Round final: public agr::Actions {
	static const agr::Color color;

	Round(base::Class<std::vector<agr::Action*>>, const Log* = nullptr,
			base::Fields = nullptr);
public:
	virtual std::ostringstream prints() const;
	static game::Deck::Unique_ptr construct(
			base::Class<std::vector<agr::Action*>>, const Log* = nullptr,
			base::Fields = nullptr);
};
class Action final: public agr::Actions {
	static const agr::Color color;

	Action(base::Class<std::vector<agr::Action*>>,
			base::Class<std::vector<agr::Action*>>, const Log* = nullptr,
			base::Fields = nullptr);
public:
	virtual std::ostringstream prints() const;
	static game::Deck::Unique_ptr construct(
			base::Class<std::vector<agr::Action*>>,
			base::Class<std::vector<agr::Action*>>, const Log* = nullptr,
			base::Fields = nullptr);
};
class Begging final: public agr::Face {
	static const agr::Color color;

	Begging(const Log*, base::Fields = nullptr);
public:
	virtual std::ostringstream prints() const;
	static game::Deck::Unique_ptr construct(const Log* = nullptr, base::Fields =
			nullptr);
};

} /* namespace card */

#endif /* COMPONENTS_H_ */

