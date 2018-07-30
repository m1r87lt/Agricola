/*
 * Player.h
 *
 *  Created on: 03 gen 2018
 *      Author: MLaRosa
 */

#ifndef PLAYER_H_
#define PLAYER_H_

#include <src/base.h>

struct Color {
	enum class Which {
		No,
		White,
		Gray,
		Black,
		Crimson,
		Red,
		Brown,
		Orange,
		Yellow,
		Green,
		Blue,
		Purple
	} instance;

	std::string name() const;
	operator Which() const;
	base::Variable<Color> variable() const;
	base::Variable<Color> variable(std::string label) const;
	static Which color(std::string);

	Color(Which);
	Color(std::string);
private:
	static const std::map<Color, std::string> names;

	static std::string transcode(Color);
	static std::string naming(Which);
};

class Player: public base::Location {
	std::string name;
	Color colored;
	static std::vector<std::unique_ptr<Player>> players;

	Player(std::string, Color, const Log*, std::string);
public:
	const std::string& identity() const;
	short unsigned number() const;
	const Color& color() const;
	Location* played() const;
	static Player* player(short unsigned);
	static Player* player(std::string);
	static Player* player(Color);
	static short unsigned quantity();
	static short unsigned construct(std::string, Color, const Log*);
	static void construct(std::initializer_list<std::pair<std::string, Color>>,
			const Log*, std::string);
};

class Owned {
	Player* owner;

	static std::string transcode(Owned);
public:
	Player* player() const;

	Owned(short unsigned, const base::Log*);
	Owned(std::string, const base::Log*);
	Owned(Color, const base::Log*);
	Owned(const Player&, const base::Log*);
	virtual ~Owned() = default;
};

#endif /* PLAYER_H_ */
