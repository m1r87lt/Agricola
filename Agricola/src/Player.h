/*
 * Player.h
 *
 *  Created on: 25 nov 2018
 *      Author: m1rma
 */

#ifndef PLAYER_H_
#define PLAYER_H_

#include <src/Element.h>
#define COLOR(which) #which

namespace agr {
#define AGR "agr"

struct Color final: public base::Object {
	enum class Which {
		No, Black, Brown, Red, Orange, Yellow, Grey, Green, Blue, Purple, White
	};

	base::Primitive<bool> operator ==(Color) const;
	base::Primitive<bool> operator !=(Color) const;
	virtual std::ostringstream prints() const;

	Color(const base::Log*);
	Color(Which, const base::Log* = nullptr);
	~Color() = default;
	Color(const Color&);
	Color& operator =(const Color&);
private:
	Which which;
};

class Owned;
class Player final: public base::Ensemble {
	base::Class<std::string> name;
	Color color;
	base::Class<std::set<Owned*>> pieces;
	friend class Owned;
	static base::Class<std::vector<Player*>> players;
	Player(base::Class<std::string>, Color, const Log* = nullptr, base::Fields =
			nullptr);
public:
	base::Primitive<short> which_is(const Log* = nullptr) const;
	base::Class<std::string> who_is(const Log* = nullptr) const;
	Color how_is(const Log* = nullptr) const;
	base::Class<std::set<Owned*>> owns(const Log* = nullptr);
	virtual std::ostringstream prints() const;
	static base::Primitive<short> give_number(const Log* = nullptr);
	static Player& give(base::Primitive<short>, const Log* = nullptr);
	static Player& give(base::Class<std::string>, const Log* = nullptr);
	static Player& give(Color, const Log* = nullptr);
	static void construct_all(
			base::Class<std::vector<std::pair<std::string, Color>>>,
			const Log* = nullptr);

	~Player();
};

class Owned: virtual public base::Log {
	base::Primitive<Player*> owner;
public:
	Player& gives_owner(const Log* = nullptr);
	virtual std::ostringstream prints() const = 0;

	Owned(Player&, const Log* = nullptr);
	virtual ~Owned();
	Owned(const Owned&);
	Owned& operator =(const Owned&);
	Owned(Owned&&);
	Owned& operator =(Owned&&);
};

} /* namespace agr */

#endif /* PLAYER_H_ */
