/*
 * Components.h
 *
 *  Created on: 13 nov 2018
 *      Author: m1rma
 */

#ifndef COMPONENTS_H_
#define COMPONENTS_H_

//#include <src/Gamecard.h>
//#include <array>
#include "Logics.h"
#define NEW_BOARD(caller, label, open, attributes) Object(caller, label), Log(caller, label, open), Board(label, caller, attributes)
#define SPACE_NAME "Space"
#define PERSONAL_SUPPLY_NAME "Personal Supply"
/*#define CARD_MAJOR_IMPROVEMENT "Major Improvement"
 #define CARD_MINOR_IMPROVEMENT "Minor Improvement"
 #define CARD_OCCUPATION "Occupation"
 #define CARD_ROUND "Round"*/

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
//	friend Space;
//	friend Row;
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
template<typename First, typename Second> std::ostringstream unprint_std__map_std__type_info_second_(
		const std::map<First, Second>& container) {
	std::ostringstream result("{");

	for (auto content : container)
		result << "\n" << content.first.name() << ": " << content.second;
	result << (result.str() == "{" ? " " : "\n") << "}";

	return result;
}
class Face: public base::Ensemble, public Colored {
	base::Class<std::vector<Condition*>> prerequisite;
	base::Class<std::string> name;
	base::Class<std::map<std::type_index, int>> cost;
	base::Primitive<char> deck;
	base::Class<std::vector<Event*>> events;
	base::Primitive<bool> bonus_points;

	Face(base::Class<std::vector<Condition*>>, base::Class<std::string>,
			base::Class<std::map<std::type_index, int>>, base::Primitive<char>,
			base::Class<std::vector<Event*>>, base::Primitive<bool>, Color,
			std::string, const Log* = nullptr, base::Fields = nullptr);
public:
	base::Class<std::vector<Condition*>> has_prerequisites(
			const Log* = nullptr) const;
	base::Class<std::string> has_name(const Log* = nullptr) const;
	base::Class<std::map<std::type_index, int>> has_cost(
			const Log* = nullptr) const;
	base::Primitive<char> belongs(const Log* = nullptr) const;
	base::Class<std::vector<Event*>> has_events(const Log* = nullptr) const;
	base::Primitive<bool> has_bonus_points(const Log* = nullptr) const;

	virtual ~Face() = default;
};
class Numbered: virtual public base::Log {
	base::Primitive<unsigned> number;
public:
	base::Primitive<unsigned> is_number(const Log* = nullptr);
	virtual std::ostringstream prints() const = 0;

	Numbered(base::Primitive<unsigned> number, const Log* = nullptr);
	virtual ~Numbered() = default;
	Numbered(const Numbered&);
	Numbered& operator =(const Numbered&);
};
class Cover: public base::Ensemble, virtual public Colored {
	base::Class<std::string> name;

	Cover(base::Class<std::string>, Color, const Log* = nullptr, base::Fields =
			nullptr);
public:
	base::Class<std::string> has_name(const Log* = nullptr) const;
	virtual std::ostringstream prints() const;
};
/* class Actions: public base::Ensemble, virtual public Colored {
 std::vector<a::Action*> actions;
 static const a::Color color = a::Color::Green;

 Actions(base::Class<std::vector<a::Action*>>, const Log* = nullptr,
 base::Fields = nullptr);
 public:
 base::Class<std::vector<a::Action*>> has_actions(const Log* = nullptr);
 virtual std::ostringstream prints() const;

 virtual ~Actions();
 };

 /*struct WoodenPiece: public base::Element, virtual public Colored {
 enum class Shape {
 disc, house, bar, cube
 };
 enum class Type {
 Family_member, Stable, Fence, token, counter
 };

 };

 } /* namespace agr */
/*
 namespace card {

 class Occupation final: public a::Face, public virtual a::CardNumber {
 short player_number;
 bool plus;
 static const std::string type;
 static const a::Color color = a::Color::Yellow;

 Occupation(base::Primitive<unsigned>, const Log* = nullptr, base::Fields =
 nullptr);
 public:
 base::Class<std::pair<short, bool>> has_player_number(
 const Log* = nullptr) const;
 virtual std::ostringstream prints() const;
 static game::Deck::Unique_ptr construct(base::Primitive<unsigned>,
 const Log* = nullptr, base::Fields = nullptr);

 ~Occupation();
 };
 class Improvement: public a::Face, public virtual a::CardNumber {
 short victory_points;
 bool oven;
 bool kitchen;

 Improvement(base::Primitive<unsigned>, base::Class<a::Color>, const Log* =
 nullptr, base::Fields = nullptr);
 public:
 base::Primitive<short> has_victory_points(const Log* = nullptr) const;
 base::Primitive<bool> is_oven(const Log* = nullptr) const;
 base::Primitive<bool> is_kitchen(const Log* = nullptr) const;
 virtual std::ostringstream prints() const;

 virtual ~Improvement();
 };
 class MinorImprovement final: public Improvement {
 static const std::string type;
 static const a::Color color = a::Color::Orange;

 MinorImprovement(base::Primitive<unsigned>, const Log* = nullptr,
 base::Fields = nullptr);
 public:
 virtual std::ostringstream prints() const;
 static game::Deck::Unique_ptr construct(base::Primitive<unsigned>,
 const Log* = nullptr, base::Fields = nullptr);

 ~MinorImprovement();
 };
 class MajorImprovement final: public Improvement {
 static const std::string type;
 static const a::Color color = a::Color::Red;

 MajorImprovement(base::Primitive<unsigned>, const Log* = nullptr,
 base::Fields = nullptr);
 public:
 virtual std::ostringstream prints() const;
 static game::Deck::Unique_ptr construct(base::Primitive<unsigned>,
 const Log* = nullptr, base::Fields = nullptr);

 ~MajorImprovement();
 };
 class Round final: public a::Actions {
 static const std::string type;
 static const a::Color color = a::Color::Blue;

 Round(base::Class<std::vector<a::Action*>>, base::Class<a::Color>,
 const Log* = nullptr, base::Fields = nullptr);
 public:
 virtual std::ostringstream prints() const;
 static game::Deck::Unique_ptr construct(
 base::Class<std::vector<a::Action*>>, base::Class<a::Color>,
 const Log* = nullptr, base::Fields = nullptr);

 ~Round();
 };
 class Action final: public a::Actions {
 Action(base::Class<std::vector<a::Action*>>,
 base::Class<std::vector<a::Action*>>, const Log* = nullptr,
 base::Fields = nullptr);
 public:
 virtual std::ostringstream prints() const;
 static game::Deck::Unique_ptr construct(
 base::Class<std::vector<a::Action*>>,
 base::Class<std::vector<a::Action*>>, const Log* = nullptr,
 base::Fields = nullptr);

 ~Action();
 };
 class Begging final: public a::Face {
 static const a::Color color = a::Color::Grey;

 Begging(const Log*, base::Fields = nullptr);
 public:
 virtual std::ostringstream prints() const;
 static game::Deck::Unique_ptr construct(const Log* = nullptr, base::Fields =
 nullptr);

 ~Begging();
 };
 /***/
} /* namespace card */

#endif /* COMPONENTS_H_ */

