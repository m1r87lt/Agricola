/*
 * Components.h
 *
 *  Created on: 13 nov 2018
 *      Author: m1rma
 */

#ifndef COMPONENTS_H_
#define COMPONENTS_H_

#include <src/Gamecard.h>
#include <array>
#include "Logics.h"
#define CARD_MAJOR_IMPROVEMENT "Major Improvement"
#define CARD_MINOR_IMPROVEMENT "Minor Improvement"
#define CARD_OCCUPATION "Occupation"
#define CARD_ROUND "Round"

namespace a {

struct Board: public base::Ensemble {
	virtual ~Board();
protected:
	Board(base::Class<std::string>, const Log*, base::Fields = nullptr);
};
struct Farmyard final: protected Board {
	class Space final: public Ensemble {
		std::array<Element*, 4> fences;

		Space(const Log*, base::Fields = nullptr);
		friend Farmyard;
		friend base::Unique_ptr;
	public:
		enum Direction {
			N, S, W, E
		};

		base::Class<std::tuple<Farmyard*, short, short>> is_located(const Log* =
				nullptr) const;
		base::Primitive<Element*> operator ()(base::Primitive<Direction>,
				const Log* = nullptr);

		~Space();
	};
	class Row final: public Log {
		short row;
		Farmyard* owner;
		friend Farmyard;
		Row(base::Primitive<short>, base::Primitive<Farmyard*>, const Log* =
				nullptr);
		Row(const Row&);
		Row& operator =(const Row&);
	public:
		Space& operator [](base::Primitive<short>) const;

		~Row();
	};
	friend Space;
	friend Row;
	Ensemble& personal_supply(const Log* = nullptr) const;
	Row operator [](base::Primitive<short>) const;
	static base::Unique_ptr construct(const Log* = nullptr);

	~Farmyard();
private:
	Farmyard(const Log*, base::Fields = nullptr);
	friend base::Unique_ptr;
};

class Face: public base::Ensemble, virtual public Colored {
	std::vector<Condition*> prerequisite;
	std::string name;
	std::vector<Quantity*> cost;
	char edition;
	std::vector<Event*> events;
	bool bonus_points;

	Face(base::Class<std::vector<Condition*>>, base::Class<std::string>,
			base::Class<std::vector<Quantity*>>, base::Primitive<char>,
			base::Class<std::vector<Event*>>, base::Primitive<bool>,
			base::Class<Color>, base::Class<std::string>, const Log* = nullptr,
			base::Fields = nullptr);
public:
	base::Class<std::vector<Condition*>> has_prerequisites(
			const Log* = nullptr) const;
	base::Class<std::string> has_name(const Log* = nullptr) const;
	base::Class<std::vector<Quantity*>> has_cost(const Log* = nullptr) const;
	base::Primitive<char> is_edtion(const Log* = nullptr) const;
	base::Class<std::vector<Event*>> has_events(const Log* = nullptr) const;
	base::Primitive<bool> has_bonus_points(const Log* = nullptr) const;
	virtual std::ostringstream prints() const = 0;

	virtual ~Face();
};
class CardNumber: virtual public base::Log {
	unsigned number;
public:
	base::Primitive<unsigned> is_number(const Log* = nullptr);
	virtual std::ostringstream prints() const = 0;

	CardNumber(base::Primitive<unsigned> number, const Log* = nullptr);
	~CardNumber();
	CardNumber(const CardNumber&);
	CardNumber& operator =(const CardNumber&);
	CardNumber(CardNumber&&);
	CardNumber& operator =(CardNumber&&);
};
class Cover: public base::Ensemble, virtual public Colored {
	std::string label;

	Cover(base::Class<Color>, base::Class<std::string>, const Log* = nullptr,
			base::Fields = nullptr);
public:
	base::Class<std::string> is_label(const Log* = nullptr) const;
	virtual std::ostringstream prints() const;

	~Cover();
};
class Actions: public base::Ensemble, virtual public Colored {
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

 };*/

} /* namespace a */

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

} /* namespace card */

#endif /* COMPONENTS_H_ */

