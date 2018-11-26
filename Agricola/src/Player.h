/*
 * Player.h
 *
 *  Created on: 25 nov 2018
 *      Author: m1rma
 */

#ifndef PLAYER_H_
#define PLAYER_H_

#include <src/Element.h>

namespace a {

enum class Color {
	Black, Brown, Red, Orange, Yellow, Grey, Green, Blue, Purple, White
};
class Colored: public base::Log {
	Color color;
public:
	base::Class<Color> has_color(const Log* = nullptr) const;

	Colored(base::Class<Color>, base::Class<std::string>, const Log* = nullptr);
	~Colored(base::Class<Color>, base::Class<std::string>,
			const Log* = nullptr);
	Colored(const Colored&);
	Colored& operator =(const Colored&);
	Colored(Colored&&);
	Colored& operator =(Colored&&);
};

class Player final: public base::Ensemble, virtual public Colored {
	std::string name;
	short number;
	std::set<base::Element*> pieces;
	static std::vector<Player*> players;

	Player(base::Class<std::string>, base::Class<Color>, const Log* = nullptr,
			base::Fields = nullptr);
public:
	base::Class<std::string> who_is(const Log* = nullptr) const;
	base::Primitive<short> which_is(const Log* = nullptr) const;
	base::Class<std::set<base::Element*>> owns(const Log* = nullptr);
	static base::Primitive<short> give_number(const Log* = nullptr) const;
	static Player& give(base::Primitive<short>, const Log* = nullptr) const;
	static Player& give(base::Class<Color>, const Log* = nullptr) const;
	static Player& give(base::Class<std::string>, const Log* = nullptr) const;
	template<short N> static void construct_all(
			base::Class<std::array<N, std::pair<Color, std::string>>>,
			const Log* = nullptr);

	~Player();
};

} /* namespace a */

#endif /* PLAYER_H_ */
