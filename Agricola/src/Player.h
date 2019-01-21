/*
 * Player.h
 *
 *  Created on: 25 nov 2018
 *      Author: m1r
 */

#ifndef PLAYER_H_
#define PLAYER_H_

#include <src/Existence.h>
#define COLOR(color) agr::Color(color, #color)
#define COLOR_OBJECT(object, color) agr::Color #object(color, #color)

namespace agr {

struct Color: public base::Object {
	enum class Which {
		No, Black, Grey, White, Brown, Red, Orange, Yellow, Green, Blue, Purple
	};

	bool operator ==(Color&) const;
	bool operator !=(Color&) const;
	virtual Fields shows() const;
	virtual std::string prints() const;
	Color();
	Color(Which, const char*);
private:
	Which which;
	std::string label;
};

class Owned;
class Player final: public base::Ensemble {
	std::string name;
	Color color;
	std::set<Owned*> pieces;
	static std::vector<Player*> players;

	Player(std::string, Color, Fields = Fields());
public:
	short unsigned which_is() const;
	std::string who_is() const;
	Color how_is() const;
	std::set<Owned*> owns();
	virtual Fields shows() const;
	virtual std::string prints() const;
	static short unsigned give_number();
	static Player& give(short unsigned);
	static Player* give(std::string);
	static Player* give(Color);
	static void construct_all(std::vector<std::pair<std::string, Color>>);
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
