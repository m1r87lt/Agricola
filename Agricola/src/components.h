/*
 * components.h
 *
 *  Created on: 30 dic 2017
 *      Author: m1r
 */

#ifndef COMPONENTS_H_
#define COMPONENTS_H_

#include "executable.h"

#define C_FARMYARD "Farmyard"
#define C_PERSONAL_SUPPLY "Personal Supply"
#define C_SPACE "Space"
#define C_FARMYARD_FENCE_SPACE C_FARMYARD + std::string("Fence") + C_SPACE
#define C_FARMYARD_SPACE C_FARMYARD + std::string(C_SPACE)

class Farmyard final: public base::Object, public Owned {
	Farmyard(Player*, const Log*);
	friend Player;
public:
	struct PersonalSupply final: public base::Location {
		PersonalSupply(const Log*);
	};
	struct Space final: public base::Location {
		Space(const Log);
	};
	struct FenceSpace final: public base::Location {
		FenceSpace(const Log*);
	};
	class Row: public Log {
		Farmyard* owner;
		short unsigned row;

		Row(short unsigned, Farmyard*, const Log*);
		friend Farmyard;
	public:
		base::Location* operator [](short unsigned);
	};

	base::Location* personal_supply() const;
	base::Location* space(short unsigned, short unsigned) const;
	base::Location* fence(short unsigned, short unsigned, bool, bool) const;
	Row operator [](short unsigned) const;
};
struct Board: public base::Location {
	Board(const Log*);
};
class MajorImprovements final: public Board {
	MajorImprovements(const Log*);
public:
	std::vector<std::string> gives_them() const;
	static MajorImprovements* construct(const Log*);
};

struct Data {
	int quantity;
	std::type_index type;
	short unsigned player;
	std::string note;

	operator const char*() const;
	static std::string transcode(const Data&);
	static Data interpret(std::string);

	Data();
	Data(int, std::type_index, short unsigned, std::string, const base::Log*);
};
struct Description {
	std::vector<std::string> text;
	std::vector<Data> symbols;

	operator const char*() const;
	static std::string transcode(const Description&);

	Description();
	Description(std::string, const base::Log*);
};
class Slot: public Executable, public base::Location {
	Description caption;
	std::function<int(const Log*)> action;
public:
	virtual std::string description() const = 0;
	virtual int executes(const Log*);
	virtual int attempts(const Log*);

	Slot(std::string, std::string, int*(const Log*), const Log*);
	virtual ~Slot() = default;
};
class Action: public Slot {
	std::vector<Data> collection;
public:
	std::vector<Data>& is_collecting() const;
	void collects(const Log*);
	virtual std::string description() const;

	Action(std::string, std::string, int*(const Log*), std::string, const Log*);
	virtual ~Action();
};
class Effect: public Slot {
	std::map<std::string, std::string> conditions;
public:
	bool verifies() const;
	virtual std::string description() const;

	Effect(std::string, std::string, int*(const Log*),
			std::map<std::string, std::string>, const base::Log*);
	virtual ~Effect();
};

struct Cover: public base::Location {
	enum class Type {
		no, round, occupation, minorImprovement, majorImprovement
	};
	std::string has_name() const;
	const std::string& caption() const;
	operator Type() const;
	base::Variable<const Cover&> gives_variable(std::string) const;
	static Type which(std::string);
	static std::string transcode(const Cover&);

	Cover(const Log*);
	Cover(Type, const Log*);
	Cover(std::string, const Log*);
	Cover(const Cover&);
	Cover& operator =(const Cover&);
private:
	Type type;
	Description description;
	static const std::map<Type, std::string> names;

	static std::string name(const Type&);

	Cover();
};
class Face: public base::Location {
	std::vector<Data> prerequisites;
	std::string name;
	std::vector<Data> costs;
	unsigned label;
	char edition;
	static std::map<unsigned, Face*> cards;
protected:
	Description caption;

	Face(std::vector<Data>, std::string, std::vector<Data>, unsigned, char,
			std::string, Location*, std::map<std::string, std::string>,
			const base::Log*);
public:
	const std::vector<Data>& has_prerequisites() const;
	const std::string& has_name() const;
	const std::vector<Data>& has_costs() const;
	unsigned has_label() const;
	char has_edition() const;
	std::string has_caption() const;
	virtual Executable& operator ()(size_t) = 0;

	virtual ~Face();
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
