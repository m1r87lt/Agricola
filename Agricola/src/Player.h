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
	friend base::Class<Color>;
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

class Player final: public base::Ensemble {
	std::string name;
	Color color;
	std::set<Object*> pieces;
	static std::vector<Player*> players;
	friend class Owned;
	Player(std::string, Color, Fields = Fields());
public:
	short unsigned which_is() const;
	std::string who_is() const;
	Color how_is() const;
	std::set<Object*> owns();
	virtual Fields shows() const;
	virtual std::string prints() const;
	static short unsigned give_number();
	static Player& give(short unsigned);
	static Player* give(std::string);
	static Player* give(Color);
	static void construct_all(std::vector<std::pair<std::string, Color>>);
};

class Owned: virtual public base::Object {
	Player* owner;
public:
	Player& gives_owner() const;
	Owned(Player&);
	virtual ~Owned() = default;
};

} /* namespace agr */

#endif /* PLAYER_H_ */
