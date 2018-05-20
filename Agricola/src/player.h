/*
 * Player.h
 *
 *  Created on: 03 gen 2018
 *      Author: MLaRosa
 */

#ifndef PLAYER_H_
#define PLAYER_H_

#include <src/base.h>

enum class Color {
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
};
const std::map<Color, std::string> colorNames = { { Color::No, "No" }, {
		Color::White, "White" }, { Color::Gray, "Gray" }, { Color::Black,
		"Black" }, { Color::Crimson, "Crimson" }, { Color::Red, "Red" }, {
		Color::Brown, "Brown" }, { Color::Orange, "Orange" }, { Color::Yellow,
		"Yellow" }, { Color::Green, "Green" }, { Color::Blue, "Blue" }, {
		Color::Purple, "Purple" } };
std::map<Color, std::string>::const_iterator find_color(std::string);
template<typename Container> std::string list_unique_ptrs(
		const Container& container) {
	std::ostringstream result;

	for (auto c = container.begin(); c != container.end(); ++c)
		result << ",\n\t" << c->get();
	result << (result.str().empty() ? "{ " : "\n");

	return "{" + result.str().substr(1) + "}";
}

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
