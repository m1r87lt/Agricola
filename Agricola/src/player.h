/*
 * Player.h
 *
 *  Created on: 03 gen 2018
 *      Author: MLaRosa
 */

#ifndef PLAYER_H_
#define PLAYER_H_

#include <src/base.h>

struct Color final: private base::Log {
	enum class Which {
		no,
		white,
		gray,
		black,
		crimson,
		red,
		brown,
		orange,
		yellow,
		green,
		blue,
		purple
	} instance;

	std::string has_name() const;
	operator Which() const;
	base::Variable<const Color&> gives_variable(std::string) const;
	static Color which_color(std::string);
	static std::string transcode(const Color&);

	Color(const Log*);
	Color(Which, const Log*);
	Color(std::string, const Log*);
	Color(const Color&);
	Color& operator =(const Color& copy);
private:
	static const std::map<Which, std::string> names;

	static std::string name(const Which&);

	Color();
};

class Player: public base::Location {
	std::string name;
	Color color;
	static std::vector<std::unique_ptr<Player>> players;

	Player(std::string, Color, const Log*);
public:
	const std::string& has_identity() const;
	const Color& has_color() const;
	short unsigned is_number() const;
	Location* played() const;
	static Player* player(std::string);
	static Player* player(Color);
	static Player* player(short unsigned);
	static short unsigned quantity();
	static short unsigned construct(std::string, Color, const Log*,
			std::string);
	static void construct(std::initializer_list<std::pair<std::string, Color>>,
			const Log*, std::string);
};
class Owned: private base::Log {
	Player* owner;

	static std::string transcode(const Owned&);
public:
	Player* player() const;

	Owned(short unsigned, const base::Log*);
	Owned(std::string, const base::Log*);
	Owned(Color, const base::Log*);
	Owned(const Player&, const base::Log*);
	Owned(const Owned&);
	Owned& operator =(const Owned&);
};

#endif /* PLAYER_H_ */
