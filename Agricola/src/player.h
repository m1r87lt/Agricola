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
	static Which color(std::string) const;

	Color(Which);
	Color(std::string);
private:
	static const std::map<Color, std::string> names = {
			{ Which::No, "No" }, { Which::White, "White" }, { Which::Gray,
					"Gray" }, { Which::Black, "Black" }, { Which::Crimson,
					"Crimson" }, { Which::Red, "Red" },
			{ Which::Brown, "Brown" }, { Which::Orange, "Orange" }, {
					Which::Yellow, "Yellow" }, { Which::Green, "Green" }, {
					Which::Blue, "Blue" }, { Which::Purple, "Purple" } };
};

class Played: public base::Location {
	Played(base::Log);
public:
	friend class Player;
	virtual std::string what() const;
	virtual Object* enter(std::string, size_t, unsigned) const;
	virtual std::string field(std::string, unsigned) const;
	virtual void field(std::string, size_t, std::string, unsigned);
};

class Player: public base::Location {
	std::string name;
	Color c;

	static std::vector<std::unique_ptr<Player>> players;

	Player(std::string, Color, base::Log);
public:
	virtual Object* enter(std::string, size_t, unsigned) const;
	virtual std::string field(std::string, unsigned) const;
	virtual void field(std::string, size_t, std::string, unsigned);
	virtual std::string what() const;

	const std::string& identity() const;
	short unsigned number() const;
	Color color() const;
	Played& played() const;

	static Player* player(short unsigned);
	static Player* player(std::string);
	static Player* player(Color);
	static short unsigned quantity();
	static short unsigned construct(std::string, Color, base::Log);
	static void construct(std::initializer_list<std::pair<std::string, Color>>,
			base::Log);
};

class Owned {
	short unsigned owner;
public:
	virtual std::string field(std::string, unsigned) const;
	virtual void field(std::string, size_t, std::string, unsigned);

	short unsigned player() const;

	Owned(short unsigned, base::Log);

	virtual ~Owned() = default;
};

#endif /* PLAYER_H_ */
