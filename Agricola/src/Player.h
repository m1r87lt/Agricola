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

struct Color: public base::Log {
	enum class Which {
		No, Black, Brown, Red, Orange, Yellow, Grey, Green, Blue, Purple, White
	};

	virtual std::ostringstream prints() const;

	Color(const Log*);
	Color(Which, std::string, const Log* = nullptr);
	~Color();
	Color(const Color&);
	Color& operator =(const Color&);
private:
	Which which;
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
	virtual std::ostringstream prints() const;
	static base::Primitive<short> give_number(const Log* = nullptr);
	static Player& give(base::Primitive<short>, const Log* = nullptr);
	static Player& give(base::Class<Color>, const Log* = nullptr);
	static Player& give(base::Class<std::string>, const Log* = nullptr);
	template<short N> static void construct_all(
			base::Class<std::array<std::pair<Color, std::string>, N>>,
			const Log* = nullptr);

	~Player();
};

} /* namespace a */

#endif /* PLAYER_H_ */
