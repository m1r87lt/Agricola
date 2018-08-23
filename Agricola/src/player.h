/*
 * Player.h
 *
 *  Created on: 03 gen 2018
 *      Author: MLaRosa
 */

#ifndef PLAYER_H_
#define PLAYER_H_

#include <src/base.h>

struct Color: private base::Log {
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

	std::string has_name() const;
	operator Which() const;
	static Color which_color(std::string);
	static std::string transcode(const Color&);
	static std::string name(const Which&);

	Color(const Log*);
	Color(Which, const Log*);
	Color(std::string, const Log*);
	Color(const Color&);
	Color& operator =(const Color& copy);
private:
	static const std::map<Color::Which, std::string> names;

	Color();
};
/*
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
 base::Variable<Owned> variable() const;
 base::Variable<Owned> variable(std::string) const;
 public:
 Player* player() const;

 Owned(short unsigned, const base::Log*);
 Owned(std::string, const base::Log*);
 Owned(Color, const base::Log*);
 Owned(const Player&, const base::Log*);
 virtual ~Owned() = default;
 };
 */
#endif /* PLAYER_H_ */
