/*
 * components.h
 *
 *  Created on: 30 dic 2017
 *      Author: m1r
 */

#ifndef COMPONENTS_H_
#define COMPONENTS_H_
/*
#define FARMYARD_PERSONALSUPPLY "Personal Supply"
#define FARMYARD_SPACE_FENCE "Farmyard Fence Space"
#define FARMYARD_SPACE_SPACE "Farmyard Space"
#define FARMYARD "Farmyard"

#include "executable.h"

struct Farmyard: public Owned, public base::Object {
	class PersonalSupply: public base::Location {
		PersonalSupply(Farmyard*, const Log*);
		friend Farmyard;
	public:
		virtual std::map<std::string, std::string> evaluate(
				std::map<std::string, std::string>);
	};
	class Space: public base::Location {
		Space(Farmyard*, const Log*);
		friend Farmyard;
	public:
		virtual std::map<std::string, std::string> evaluate(
				std::map<std::string, std::string>);
	};
	class Row: public Log {
		Farmyard* owner;
		size_t row;

		Row(size_t, Farmyard*, const Log*);
		friend Farmyard;
	public:
		Space* operator [](size_t);
	};

	PersonalSupply* personal_supply() const;
	Space* space(size_t, size_t) const;
	Space* fence(size_t, size_t, bool, bool) const;
	Row operator [](size_t) const;

	Farmyard(Player*, const Log*);
	virtual ~Farmyard() = default;
};
struct Gameboard: public base::Location {
	virtual std::string what() const;
	virtual Object* enter(std::string, size_t, unsigned) const;
	virtual std::string field(std::string, unsigned) const;
	virtual void field(std::string, size_t, std::string, unsigned);

	Gameboard(base::Log);

	virtual ~Gameboard() = default;
};
struct MajorImprovementBoard: public base::Location {
	MajorImprovementBoard(base::Log);

	virtual ~MajorImprovementBoard() = default;
public:
	virtual std::string what() const;
	virtual Object* enter(std::string, size_t, unsigned) const;
	virtual std::string field(std::string, unsigned) const;
	virtual void field(std::string, size_t, std::string, unsigned);

	std::list<std::string> MajorImprovements() const;

	static MajorImprovementBoard* construct(base::Log);
};

struct Data {
	int quantity;
	std::string label;
	unsigned player;
	std::string note;

	operator const char*() const;

	Data(base::Log);
	Data(int, std::string, unsigned, std::string, base::Log);
};
class Cover: public base::Location {
	std::string label;
	std::string text;
	std::string type;
public:
	const std::string& name() const;
	const std::string& caption() const;

	virtual std::string what() const;
	virtual Object* enter(std::string, size_t, unsigned) const;
	virtual std::string field(std::string, unsigned) const;
	virtual void field(std::string, size_t, std::string, unsigned);

	Cover(std::string, std::string, std::string, base::Log);

	virtual ~Cover() = default;
};
class Face: public base::Location {
	std::vector<Data> pr;
	std::string l;
	std::vector<Data> c;
	unsigned n;
	char e;
	std::vector<std::unique_ptr<Executable>> events;

	static std::map<unsigned, Face*> cards;
protected:
	std::string caption;

	static std::string list_cards();

	virtual Object* enter(std::string, size_t, unsigned) const;
	virtual std::string field(std::string, unsigned) const;
	virtual void field(std::string, size_t, std::string, unsigned);

	Face(std::vector<Data>, std::string, std::vector<Data>, unsigned, char,
			std::string, std::vector<std::unique_ptr<Executable>>, Location*,
			base::Log);
public:
	const std::vector<Data>& prerequisites() const;
	const std::string& name() const;
	const std::vector<Data>& costs() const;
	unsigned number() const;
	char edition() const;
	std::vector<std::string> executables() const;
	Executable& operator ()(size_t);

	virtual ~Face();
};
class Action: public base::Location, public Executable {
	Data collection;
	std::string caption;
public:
	const Data& collect() const;

	virtual std::string what() const;
	virtual std::string description() const;
	virtual Object* enter(std::string, size_t, unsigned) const;
	virtual std::string field(std::string, unsigned) const;
	virtual void field(std::string, size_t, std::string, unsigned);

	Action(std::string, Data, std::string, base::Log);

	virtual ~Action() = default;
};
class Effect: public Executable {
	std::string caption;
public:
	virtual std::string description() const;
	virtual std::string field(std::string, unsigned) const;
	virtual void field(std::string, size_t, std::string, unsigned);

	Effect(std::string, std::string, base::Log);
};
class Card: public game::Card {
	std::string type;
	Color c;

	static std::tuple<std::vector<std::unique_ptr<Executable>>,
			std::vector<Data>, std::vector<Data>>
	face(std::map<std::string, std::string>, base::Log);

	Card(std::string, Color, std::unique_ptr<Location>&&,
			std::unique_ptr<Location>&&, Location*, base::Log);
public:
	class Occupation: public Face {
		short unsigned p;
		bool bp;

		Occupation(short unsigned, bool,
				std::vector<std::unique_ptr<Executable>>, std::vector<Data>,
				std::string, std::vector<Data>, unsigned, char, std::string,
				Location*, base::Log);
		friend Card;
	public:
		static const std::string type;
		static const std::string caption;
		static const std::string cover;

		virtual std::string what() const;
		virtual std::string description() const;
		virtual Object* enter(std::string, size_t, unsigned) const;
		virtual std::string field(std::string, unsigned) const;
		virtual void field(std::string, size_t, std::string, unsigned);

		short unsigned players() const;
		bool bonus_points() const;

		virtual ~Occupation() = default;
	};
	class Improvement: public Face {
		int vp;
		bool o;
		bool bp;
		bool k;
		bool order;

		Improvement(bool, int, bool, bool, bool,
				std::vector<std::unique_ptr<Executable>>, std::vector<Data>,
				std::string, std::vector<Data>, unsigned, char, std::string,
				Location*, base::Log);
		friend Card;
	public:
		static const std::string minorType;
		static const std::string majorType;
		static const std::string minorCaption;
		static const std::string majorCaption;
		static const std::string majorCover;
		static const std::string minorCover;

		virtual std::string what() const;
		virtual std::string description() const;
		virtual base::Object* enter(std::string, size_t, unsigned) const;
		virtual std::string field(std::string, unsigned) const;
		virtual void field(std::string, size_t, std::string, unsigned);

		int victory_points() const;
		bool oven() const;
		bool bonus_points() const;
		bool kitchen() const;

		virtual ~Improvement() = default;
	};
	class Round: public Cover {
		unsigned staging;
		unsigned min;
		unsigned max;
	public:
		static const std::string type;

		unsigned stage() const;
		std::pair<unsigned, unsigned> round() const;

		virtual base::Object* enter(std::string, size_t, unsigned) const;
		virtual std::string field(std::string, unsigned) const;
		virtual void field(std::string, size_t, std::string, unsigned);

		Round(unsigned, unsigned, unsigned, base::Log);

		virtual ~Round() = default;
	};
	class Action: public base::Location {
		short unsigned p;
		bool f;
		std::string caption;

		Action(short unsigned, bool, std::string,
				std::vector<std::unique_ptr<::Action>>, Location*, base::Log);
		friend Card;
	public:
		static const std::string type;

		short unsigned players() const;
		bool family() const;
		std::string description() const;
		bool insert(size_t, std::string, std::unique_ptr<Object>&&, base::Log) = delete;
		void insert_back(std::string, std::unique_ptr<Object>&&, base::Log) = delete;
		template<typename ObjectDerived, typename ... Arguments> void emplace(
				size_t, std::string, base::Log, Arguments&& ...) = delete;
		template<typename ObjectDerived, typename ... Arguments> void emplace_back(
				std::string, base::Log, Arguments&& ...) = delete;
		bool remove(size_t, base::Log) = delete;
		bool remove(std::string, base::Log) = delete;
		bool remove(const Object&, base::Log) = delete;
		std::unique_ptr<Object> extract(size_t, base::Log) = delete;
		std::unique_ptr<Object> extract(std::string, base::Log) = delete;
		std::unique_ptr<Object> extract(const Object&, base::Log) = delete;

		virtual std::string what() const;
		virtual base::Object* enter(std::string, size_t, unsigned) const;
		virtual std::string field(std::string, unsigned) const;
		virtual void field(std::string, size_t, std::string, unsigned);

		virtual ~Action() = default;
	};
	class Begging: public base::Location {
		Begging(Location*, base::Log);
		friend Card;
	public:
		std::string name() const;
		int victory_points() const;
		std::string description() const;

		static const std::string type;

		virtual std::string what() const;
		virtual base::Object* enter(std::string, size_t, unsigned) const;
		virtual std::string field(std::string, unsigned) const;
		virtual void field(std::string, size_t, std::string, unsigned);

		virtual ~Begging() = default;
	};

	static const char base;
	static const char interactive;
	static const char complex;

	virtual std::string what() const;
	virtual base::Object* enter(std::string, size_t, unsigned) const;
	virtual std::string field(std::string, unsigned) const;
	virtual void field(std::string, size_t, std::string, unsigned);

	Color color() const;

	static std::unique_ptr<Card> occupation(unsigned, base::Log);
	static std::unique_ptr<Card> improvement(unsigned, base::Log);
	static std::unique_ptr<Card> round(std::string, base::Log);
	static std::unique_ptr<Card> action(short unsigned,
			std::vector<std::tuple<std::string, Data, std::string>>,
			short unsigned,
			std::vector<std::tuple<std::string, Data, std::string>>, base::Log);
	static std::unique_ptr<Card> begging(base::Log);

	virtual ~Card() = default;
};

struct Wooden: public base::Object, public Owned {
	enum class Shape {
		disc, house, line, coin, cube, cylinder
	};

	static const std::map<Shape, std::string> shapeNames;
	static const std::string woodType;
	static const std::string clayType;
	static const std::string reedType;
	static const std::string stoneType;
	static const std::string grainType;
	static const std::string vegetableType;
	static const std::string sheepType;
	static const std::string wildBoarType;
	static const std::string cattleType;
	static const std::string familyMemberType;
	static const std::string stableType;
	static const std::string fenceType;
	static const std::string firstPlayerType;

	Color color() const;
	Shape shape() const;

	virtual std::string what() const;
	virtual Object* enter(std::string, size_t, unsigned) const;
	virtual std::string field(std::string, unsigned) const;
	virtual void field(std::string, size_t, std::string, unsigned);

	static std::map<Shape, std::string>::const_iterator find_shape(std::string);
	static std::unique_ptr<Wooden> wood(base::Log);
	static std::unique_ptr<Wooden> clay(base::Log);
	static std::unique_ptr<Wooden> reed(base::Log);
	static std::unique_ptr<Wooden> stone(base::Log);
	static std::unique_ptr<Wooden> grain(base::Log);
	static std::unique_ptr<Wooden> vegetable(base::Log);
	static std::unique_ptr<Wooden> sheep(base::Log);
	static std::unique_ptr<Wooden> wildBoar(base::Log);
	static std::unique_ptr<Wooden> cattle(base::Log);
	static std::unique_ptr<Wooden> family_member(short unsigned, base::Log);
	static std::unique_ptr<Wooden> stable(short unsigned, base::Log);
	static std::unique_ptr<Wooden> fence(short unsigned, base::Log);
	static std::unique_ptr<Wooden> first_player(base::Log);

	virtual ~Wooden() = default;
private:
	std::string type;
	Color c;
	Shape s;

	Wooden(short unsigned, std::string, Shape, Color, base::Location*,
			base::Log);
};

class Tile: public base::Location {
	std::string type;
	Color c;

	Tile(std::string, Color, Location*, base::Log);
public:
	Color color() const;

	static const std::string woodenHutType;
	static const std::string clayHutType;
	static const std::string stoneHouseType;
	static const std::string fieldType;

	virtual std::string what() const;
	virtual Object* enter(std::string, size_t, unsigned) const;
	virtual std::string field(std::string, unsigned) const;
	virtual void field(std::string, size_t, std::string, unsigned);

	static std::unique_ptr<Tile> wooden_hut(base::Log);
	static std::unique_ptr<Tile> clay_hut(base::Log);
	static std::unique_ptr<Tile> stone_house(base::Log);
	static std::unique_ptr<Tile> field(base::Log);

	virtual ~Tile() = default;
};

struct Food: public base::Object {
	static const std::string type;

	virtual std::string what() const;
	virtual Object* enter(std::string, size_t, unsigned) const;
	virtual std::string field(std::string, unsigned) const;
	virtual void field(std::string, size_t, std::string, unsigned);

	Food(base::Log);
};
class Token: public base::Object {
	std::string type;
public:
	static const std::string guestType;
	static const std::string tokenType;

	virtual std::string what() const;
	virtual Object* enter(std::string, size_t, unsigned) const;
	virtual std::string field(std::string, unsigned) const;
	virtual void field(std::string, size_t, std::string, unsigned);

	Token(std::string, base::Log);
};
*/
#endif /* COMPONENTS_H_ */
