/*
 * components.cpp
 *
 *  Created on: 30 dic 2017
 *      Author: m1r
 */

#include "components.h"/*
#include <chrono>

#define OCCUPATIONS_DB "occupations.txt"
#define IMPROVEMENTS_DB "improvements.txt"

//Farmyard
const std::string Farmyard::type = "Farmyard";

std::string Farmyard::what() const {
	std::clog << base::Log().tracker() << "std::string Farmyard{" << this
			<< "}.what()=" << typeid(*this).name() << std::endl;

	return typeid(*this).name();
}
base::Object* Farmyard::enter(std::string content, size_t number,
		unsigned derived) const {
	return Location::enter(content, number, derived);
}
std::string Farmyard::field(std::string variable, unsigned derived) const {
	if (variable == "owner")
		return Owned::field(variable, derived);
	else
		return Location::field(variable, derived);
}
void Farmyard::field(std::string variable, size_t position,
		std::string value, unsigned derived) {
	if (variable == "owner")
		Owned::field(variable, position, value, derived);
	else
		Location::field(variable, position, value, derived);
}

Farmyard::PersonalSupply* Farmyard::personal_supply() const {
	PersonalSupply* result = nullptr;

	std::clog << base::Log().tracker() << "Farmyard::PersonalSupply* "
			"Farmyard::PersonalSupply{" << this << "}.personal_supply()";
	std::clog << "=" << (result =
			dynamic_cast<PersonalSupply*>(Location::operator [](0)))
			<< std::endl;

	return result;
}
Farmyard::Space* Farmyard::space(size_t row, size_t column) const {
	Space* result = nullptr;
	base::Log track;
	std::ostringstream log;

	log << track.tracker() << "Farmyard::Space* Farmyard{" << this
			<< "}.space(size_t row=" << row << ", size_t column=" << column
			<< ")";
	std::clog << log.str() << " {" << std::endl;
	if (row > 0 && row < 4 && column > 0 && column < 6)
		result = dynamic_cast<Space*>(Location::operator [](
				(row - 1) * 5 + column));
	else {
		std::string message =
				"= ERROR invalid arguments: row must be between 1 and 3"
						" while column must be between 1 and 5.";

		std::clog << track() << "}" << message << std::endl;
		std::cerr << log.str() << message << std::endl;
		throw std::domain_error(log.str() + message);
	}
	std::clog << track() << "}" << result << std::endl;

	return result;
}
Farmyard::Space* Farmyard::fence(size_t row, size_t column, bool vertical,
		bool advanced) const {
	Space* result = nullptr;
	base::Log track;
	std::ostringstream log;

	log << track.tracker() << "Farmyard::Space* Farmyard{" << this
			<< "}.fence(size_t row=" << row << ", size_t column=" << column
			<< ", bool vertical=" << vertical << ", bool advanced=" << advanced
			<< ")";
	std::clog << log.str() << " {" << std::endl;
	if (row > 0 && row < 4 && column > 0 && column < 6)
		result = dynamic_cast<Space*>(Location::operator [](
				11 * row + column + (5 + advanced) * vertical + 11 * advanced
						+ 4));
	else {
		std::string message =
				"= ERROR invalid arguments: row must be between 1 and 3"
						" while column must be between 1 and 5.";

		std::clog << track() << "}" << message << std::endl;
		std::cerr << log.str() << message << std::endl;
		throw std::domain_error(log.str() + message);
	}
	std::clog << track() << "}" << result << std::endl;

	return result;
}
Farmyard::Row Farmyard::operator [](size_t row) const {
	std::clog << base::Log().tracker() << "Farmyard::Row Farmyard{" << this
			<< "}[" << row << "]" << std::endl;

	return Row(row, const_cast<Farmyard*>(this));
}

Farmyard::Farmyard(short unsigned player, base::Log track) :
		Owned(player, track), base::Location(
				nullptr, track) {
	std::ostringstream log;

	log << track.tracker() << "Farmyard::Farmyard(short unsigned player=" << player << ")";
	std::clog << log.str() << " {" << std::endl;
	Location::insert(0, PersonalSupply::name,
			std::unique_ptr<Object>(new PersonalSupply(this, track)), track);
	for (size_t r = 1; r < 4; ++r)
		for (size_t c = 1; c < 6; ++c)
			Location::insert_back(
					Space::space_name + "(" + std::to_string(r) + ";"
							+ std::to_string(c) + ")",
					std::unique_ptr<Object>(new Space(this, track)), track);
	for (size_t s = 1; s < 39; ++s)
		Location::insert_back(Space::fence_name + std::to_string(s),
				std::unique_ptr<Object>(new Space(this, track)), track);
	std::clog << "=" << this << std::endl;
}

//Farmyard::PersonalSupply
const std::string Farmyard::PersonalSupply::name = "Personal Supply";

std::string Farmyard::PersonalSupply::what() const {
	std::clog << base::Log().tracker()
			<< "std::string Farmyard::PersonalSupply{" << this << "}.what()="
			<< typeid(*this).name() << std::endl;

	return typeid(*this).name();
}
base::Object* Farmyard::PersonalSupply::enter(std::string content,
		size_t number, unsigned derived) const {
	return Location::enter(content, number, derived);
}
std::string Farmyard::PersonalSupply::field(std::string variable,
		unsigned derived) const {
	return Location::field(variable, derived);
}
void Farmyard::PersonalSupply::field(std::string variable, size_t position,
		std::string value, unsigned derived) {
	Location::field(variable, position, value, derived);
}

Farmyard::PersonalSupply::PersonalSupply(Farmyard* farmyard,
		base::Log track) :
		Location(farmyard, track) {
	std::clog << track.tracker()
			<< "Farmyard::PersonalSupply::PersonalSupply(Farmyard* farmyard="
			<< farmyard << ")=" << this << std::endl;
}

//Farmyard::Space
const std::string Farmyard::Space::space_name = "Space";
const std::string Farmyard::Space::fence_name = "Fence";

std::string Farmyard::Space::what() const {
	std::clog << base::Log().tracker() << "std::string Farmyard::Space{"
			<< this << "}.what()=" << typeid(*this).name() << std::endl;

	return typeid(*this).name();
}
base::Object* Farmyard::Space::enter(std::string content, size_t number,
		unsigned derived) const {
	return Location::enter(content, number, derived);
}
std::string Farmyard::Space::field(std::string variable,
		unsigned derived) const {
	return Location::field(variable, derived);
}
void Farmyard::Space::field(std::string variable, size_t position,
		std::string value, unsigned derived) {
	Location::field(variable, position, value, derived);
}

Farmyard::Space::Space(Farmyard* farmyard, base::Log track) :
		Location(farmyard, track) {
	std::clog << track.tracker() << "Farmyard::Space::Space(Farmyard* farmyard="
			<< farmyard << ")=" << this << std::endl;
}

//Farmyard::Spaces::Row
Farmyard::Space* Farmyard::Row::operator [](size_t column) {
	Space* result = nullptr;
	base::Log track;

	std::clog << track.tracker() << "Farmyard::Space* Farmyard::Row{" << row << ";"
			<< this << "}[" << column << "] {" << std::endl;
	result = owner->space(row, column);
	std::clog << track() << "}=" << result << std::endl;

	return result;
}

base::Object* Farmyard::Row::enter(std::string content, size_t number,
		unsigned derived) const {
	if (content == "owner")
		return owner;
	else {
		std::ostringstream log;

		log << "base::Object* Farmyard::Row{" << this
				<< "}.enter(std::string content=\"" << content
				<< "\", size_t number=" << number << " [unsigned derived="
				<< derived << "])=0 ERROR invalid argument: invalid content.";
		std::clog << log.str() << std::endl;
		std::cerr << log.str() << std::endl;
		throw std::invalid_argument(log.str());
	}
}
std::string Farmyard::Row::field(std::string variable, unsigned derived) const {
	if (variable == "owner")
		return Prompt::address(owner);
	else if (variable == "row")
		return std::to_string(row);
	else {
		std::ostringstream log;

		log << "base::Object* Farmyard::Row{" << this
				<< "}.field(std::string variable=\"" << variable
				<< "\" [unsigned derived=" << derived
				<< "])=0 ERROR invalid argument: invalid variable.";
		std::clog << log.str() << std::endl;
		std::cerr << log.str() << std::endl;
		throw std::invalid_argument(log.str());
	}
}
void Farmyard::Row::field(std::string variable, size_t position,
		std::string value, unsigned derived) {
		if (variable == "owner")
			owner = (Farmyard*) std::stoull(value);
		else if (variable == "row")
			row = position;
		else {
			std::ostringstream result;

			result << "void Farmyard::Row{" << this
				<< "}.field(std::string variable=\"" << variable
				<< "\", size_t position=" << position
				<< ", std::string value=\"" << value << "\", derived="
				<< derived << ")= ERROR invalid argument: invalid variable.";
			std::cerr << result.str() << std::endl;
			throw std::invalid_argument(result.str());
		}
}

Farmyard::Row::Row(size_t row, Farmyard* owner) {
	std::ostringstream log;
	base::Log track;

	log << track.tracker() << "Farmyard::Row::Row(size_t row=" << row
			<< ", Farmyard* owner=" << owner << ")";
	std::clog << log.str() << " {" << std::endl;
	if (row < 1 || row > 3) {
		std::string message = "= ERROR row must be between 1 and 3.";

		std::clog << track() << "}" << message << std::endl;
		std::cerr << log.str() << message << std::endl;
		throw std::domain_error(log.str() + message);
	}
	this->row = row;
	this->owner = owner;
	std::clog << track() << "}=" << this << std::endl;
}

//Gameboard
std::string Gameboard::what() const {
	std::clog << base::Log().tracker() << "std::string Gameboard{" << this
			<< "}.what()=" << typeid(*this).name() << std::endl;

	return typeid(*this).name();
}
base::Object* Gameboard::enter(std::string content, size_t number,
		unsigned derived) const {
	return Location::enter(content, number, derived);
}
std::string Gameboard::field(std::string variable, unsigned derived) const {
	return Location::field(variable, derived);
}
void Gameboard::field(std::string variable, size_t position,
		std::string value, unsigned derived) {
	Location::field(variable, position, value, derived);
}

Gameboard::Gameboard(base::Log track) :
		base::Location(nullptr, track) {
	std::clog << track.tracker() << "Gameboard::Gameboard()=" << this << std::endl;
}

//MajorImprovementBoard
std::string MajorImprovementBoard::what() const {
	std::clog << base::Log().tracker() << "std::string MajorImprovementBoard{"
			<< this << "}.what()=" << typeid(*this).name() << std::endl;

	return typeid(*this).name();
}
base::Object* MajorImprovementBoard::enter(std::string content, size_t number,
		unsigned derived) const {
	return Location::enter(content, number, derived);
}
std::string MajorImprovementBoard::field(std::string variable,
		unsigned derived) const {
	return Location::field(variable, derived);
}
void MajorImprovementBoard::field(std::string variable, size_t position,
		std::string value, unsigned derived) {
	Location::field(variable, position, value, derived);
}

std::list<std::string> MajorImprovementBoard::MajorImprovements() const {
	std::list<std::string> result;
	size_t quantity = 0;
	base::Log track;

	std::clog << track.tracker() << "std::list< std::string > "
			"MajorImprovementBoard{" << this << "}.MajorImprovements() {"
			<< std::endl;
	quantity = size();
	for (size_t c = 0; c < quantity; ++c)
		result.emplace_back(who(*operator [](c)));
	std::clog << track() << "}=" << list(result) << std::endl;

	return result;
}

MajorImprovementBoard::MajorImprovementBoard(base::Log track) :
		Location(nullptr, track) {
	std::clog << track.tracker() << "MajorImprovementBoard::MajorImprovementBoard()="
			<< this << std::endl;
}

//Data
Data::operator const char*() const {
	std::ostringstream result;

	if (quantity) {
		result << quantity << " ";
		if (player)
			result << "player " << player << "'s ";
		result << label;
		if (!note.empty())
			result << ": " << note;
	}

	return result.str().c_str();
}

Data::Data(base::Log track) {
	quantity = 0;
	player = 0;
	std::clog << track.tracker() << "Data::Data()" << std::endl;
}
Data::Data(int quantity, std::string label, unsigned player, std::string note,
		base::Log track) {
	this->quantity = quantity;
	this->label = label;
	this->player = player;
	this->note = note;
	std::clog << track.tracker() << "Data::Data(int quantity="
			<< quantity << ", std::string label=" << label
			<< ", unsigned player=" << player << ", std::string note=\"" << note
			<< "\")" << std::endl;
}

//Cover
const std::string& Cover::name() const {
	std::clog << base::Log().tracker() << "const std::string& Cover{" << this
			<< "}.name()=" << label << std::endl;

	return label;
}
const std::string& Cover::caption() const {
	std::clog << base::Log().tracker() << "const std::string& Cover{" << this
			<< "}.caption()=" << text << std::endl;

	return text;
}

std::string Cover::what() const {
	std::clog << base::Log().tracker() << "const std::string& Cover{" << this
			<< "}.what()=" << type << std::endl;

	return type;
}
base::Object* Cover::enter(std::string content, size_t number,
		unsigned derived) const {
	return Location::enter(content, number, derived);
}
std::string Cover::field(std::string variable, unsigned derived) const {
	if (variable == "label")
		return label;
	else if (variable == "text")
		return text;
	else if (variable == "type")
		return type;
	else
		return Location::field(variable, derived);
}
void Cover::field(std::string variable, size_t position,
		std::string value, unsigned derived) {
	if (variable == "label") {
		label = value;
		modification = std::chrono::system_clock::to_time_t(
					std::chrono::system_clock::now());
	} else if (variable == "text") {
		text = value;
		modification = std::chrono::system_clock::to_time_t(
					std::chrono::system_clock::now());
	} else if (variable == "type") {
		type = value;
		modification = std::chrono::system_clock::to_time_t(
					std::chrono::system_clock::now());
	} else
		Location::field(variable, position, value, derived);
}

Cover::Cover(std::string name, std::string caption, std::string type,
		base::Log track) :
		Location(nullptr, track) {
	label = name;
	text = caption;
	this->type = type;
	std::clog << track.tracker() << "Cover::Cover(std::string name=\"" << name
			<< "\", std::string caption=\"" << caption << "\", std::string type=\""
			<< type << "\")=" << this << std::endl;
}

//Face
std::map<unsigned, Face*> Face::cards = { { 0, nullptr } };

std::string Face::list_cards() {
	std::ostringstream text;

	for (auto content : cards)
		text << ",\n\t" << content.first << ": " << content.second;
	text.str(text.str().empty() ? "{ }" : "{" + text.str().substr(1) + "\n}");

	return text.str();
}

base::Object* Face::enter(std::string content, size_t number,
		unsigned derived) const {
	if (content == "cards" && number < cards.size())
		return cards[number];
	else
		return Location::enter(content, number, derived);
}
std::string Face::field(std::string variable, unsigned derived) const {
	if (variable == "pr")
		return list(pr);
	else if (variable == "l")
		return l;
	else if (variable == "c")
		return list(c);
	else if (variable == "n")
		return std::to_string(n);
	else if (variable == "e")
		return std::string(e, 1);
	else if (variable == "cards")
		return list_cards();
	else if (variable == "caption")
		return caption;
	else
		return Location::field(variable, derived);
}
void Face::field(std::string variable, size_t position,
		std::string value, unsigned derived) {
	std::istringstream command(value);
	int quantity = 0;
	std::string label;
	short unsigned player = 0;
	std::string note;
	std::ostringstream result;

	if (variable == "pr") {
		command >> value;
		if (value == "insert" && position <= pr.size()) {
			command >> quantity >> label >> player;
			std::getline(command, note);
			pr.emplace(pr.begin() + position, quantity, label, player, note, base::Log());
		} else if (value == "erase" && position < pr.size())
			pr.erase(pr.begin() + position);
		else {
			result << "void Face{" << this
					<< "}.field(std::string variable=\"" << variable
					<< "\", size_t position=" << position
					<< ", std::string value=\"" << value << "\", derived="
					<< derived << ")= ERROR invalid argument: invalid variable.";
			std::cerr << result.str() << std::endl;
			throw std::invalid_argument(result.str());
		}
		modification = std::chrono::system_clock::to_time_t(
					std::chrono::system_clock::now());
	} else if (variable == "l") {
		l = value;
		modification = std::chrono::system_clock::to_time_t(
					std::chrono::system_clock::now());
	} else if (variable == "c") {
		command >> value;
		if (value == "insert" && position <= c.size()) {
			command >> quantity >> label >> player;
			std::getline(command, note);
			c.emplace(c.begin() + position, quantity, label, player, note, base::Log());
		} else if (value == "erase" && position < c.size())
			c.erase(c.begin() + position);
		else {
			result << "void Face{" << this
					<< "}.field(std::string variable=\"" << variable
					<< "\", size_t position=" << position
					<< ", std::string value=\"" << value << "\", derived="
					<< derived << ")= ERROR invalid argument: invalid value (command).";
			std::cerr << result.str() << std::endl;
			throw std::invalid_argument(result.str());
		}
		modification = std::chrono::system_clock::to_time_t(
					std::chrono::system_clock::now());
	} else if (variable == "n") {
		command >> n;
		modification = std::chrono::system_clock::to_time_t(
					std::chrono::system_clock::now());
	} else if (variable == "e") {
		e = value.front();
		modification = std::chrono::system_clock::to_time_t(
					std::chrono::system_clock::now());
	} else if (variable == "events") {
		void* event = nullptr;

		command >> value;
		if (value == "insert" && position <= events.size()) {
			command >> event;
			events.emplace(events.begin() + position, (Executable*) event);
		} else if (value == "erase" && position < events.size())
			events.erase(events.begin() + position);
		else {
			result << "void Face{" << this
					<< "}.field(std::string variable=\"" << variable
					<< "\", size_t position=" << position
					<< ", std::string value=\"" << value << "\", derived="
					<< derived << ")= ERROR invalid argument: invalid value (command).";
			std::cerr << result.str() << std::endl;
			throw std::invalid_argument(result.str());
		}
		modification = std::chrono::system_clock::to_time_t(
					std::chrono::system_clock::now());
	} else if (variable == "cards") {
		void* face = nullptr;

		command >> value;
		if (value == "insert" && position <= cards.size()) {
			command  >> face;
			cards[position] = (Face*) face;
		} else if (value == "erase" && position < cards.size())
			cards.erase(position);
		else {
			result << "void Face{" << this
					<< "}.field(std::string variable=\"" << variable
					<< "\", size_t position=" << position
					<< ", std::string value=\"" << value << "\", derived="
					<< derived << ")= ERROR invalid argument: invalid value (command).";
			std::cerr << result.str() << std::endl;
			throw std::invalid_argument(result.str());
		}
		modification = std::chrono::system_clock::to_time_t(
					std::chrono::system_clock::now());
	} else if (variable == "caption") {
		caption = value;
		modification = std::chrono::system_clock::to_time_t(
					std::chrono::system_clock::now());
	} else
		Location::field(variable, position, value, derived);
}

const std::vector<Data>& Face::prerequisites() const {
	std::clog << base::Log().tracker() << "const std::vector< Data >& Face{"
			<< this << "}.prerequisites()=" << list(pr) << std::endl;

	return pr;
}
const std::string& Face::name() const {
	std::clog << base::Log().tracker() << "const std::string& Face{" << this
			<< "}.name()=" << l << std::endl;

	return l;
}
const std::vector<Data>& Face::costs() const {
	std::clog << base::Log().tracker() << "const std::vector< Data >& Face{"
			<< this << "}.costs()=" << list(c) << std::endl;

	return c;
}
unsigned Face::number() const {
	std::clog << base::Log().tracker() << "unsigned Face{" << this
			<< "}.number()=" << n << std::endl;

	return n;
}
char Face::edition() const {
	std::clog << base::Log().tracker() << "char Face{" << this
			<< "}.edition()=" << e << std::endl;

	return e;
}
std::vector<std::string> Face::executables() const {
	std::vector<std::string> result;
	auto event = events.begin();
	base::Log track;

	std::clog << track.tracker() << "std::vector< std::string > Face{" << this
			<< "}.executables() {" << std::endl;
	for (auto end = events.end(); event != end; ++event)
		result.emplace_back(*event->get());
	std::clog << track() << "}=" << list(result) << std::endl;

	return result;
}
Executable& Face::operator ()(size_t e) {
	base::Log track;
	std::ostringstream log;

	log << track.tracker() << "Executable& Face{" << this << "}[" << e << "]";
	std::clog << log.str() << " {" << std::endl;
	if (e < events.size()) {
		modification = std::chrono::system_clock::to_time_t(
					std::chrono::system_clock::now());
		std::clog << track() << "}=" << (std::string) *events[e] << std::endl;

		return *events[e];
	} else {
		std::ostringstream message;

		message << "= ERROR invalid argument: e >= event number ="
				<< events.size() << ".";
		std::clog << track() << "}=" << message.str() << std::endl;
		std::cerr << log.str() << message.str() << std::endl;
		throw std::out_of_range(log.str() + message.str());
	}
}

Face::Face(std::vector<Data> prerequisite, std::string name,
		std::vector<Data> costs, unsigned number, char edition,
		std::string caption, std::vector<std::unique_ptr<Executable>> events,
		Location* position, base::Log track) :
		Location(position, track) {
	std::ostringstream log;

	log << track.tracker() << "Face::Face(std::initializer_list< Data > prerequisite="
			<< list(prerequisite) << ", std::string name=\"" << name
			<< "\", std::initializer_list< Data > costs=" << list(costs)
			<< ", unsigned number=" << number << ", char edition='" << edition
			<< "', std::string caption=\"" << caption
			<< "\", std::vector< std::unique_ptr< Executable > > events="
			<< Executable::list(events) << ", base::Location* position="
			<< position << ")";
	std::clog << log.str() << " {" << std::endl;
	pr = prerequisite;
	l = name;
	c = costs;
	e = edition;
	this->events = std::move(events);
	this->caption = caption;
	if ((n = number)) {
		if (cards.find(number) != cards.end()) {
			std::string message =
					"= ERROR a card with this number does already exist.";

			std::cerr << log.str() << message << std::endl;
			std::clog << track() << "}" << message << std::endl;
			std::domain_error(log.str() + message);
		}
		cards[n] = this;
	}
	std::clog << track() << "}=" << this << std::endl;
}
Face::~Face() {
	if (n && base::running())
		cards.erase(n);
	std::clog << base::Log().tracker() << "Face::~Face(){" << this << "}"
			<< std::endl;
}

//Action
const Data& Action::collect() const {
	std::clog << base::Log().tracker() << "const Data& Action{" << this
			<< "}.collect()=" << (std::string) collection << std::endl;

	return collection;
}

std::string Action::what() const {
	std::clog << base::Log().tracker() << "std::string Action{" << this
			<< "}.what()=" << typeid(*this).name() << std::endl;

	return typeid(*this).name();
}
std::string Action::description() const {
	std::clog << base::Log().tracker() << "std::string Action{" << this
			<< "}.description()=" << caption << std::endl;

	return caption;
}
base::Object* Action::enter(std::string content, size_t number,
		unsigned derived) const {
	return Location::enter(content, number, derived);
}
std::string Action::field(std::string variable, unsigned derived) const {
	if (variable == "collection")
		return std::string(collection);
	else if (variable == "caption")
		return caption;
	else if (variable == "name")
		return Executable::field(variable, derived);
	else if (variable == "active")
		return Executable::field(variable, derived);
	else
		return Location::field(variable, derived);
}
void Action::field(std::string variable, size_t position,
		std::string value, unsigned derived) {
	std::istringstream command(value);
	int quantity = 0;
	std::string label;
	short unsigned player = 0;
	std::string note;

	if (variable == "collection") {
		command >> quantity >> label >> player;
		std::getline(command, note);
		collection = Data(quantity, label, player, note, base::Log());
		modification = std::chrono::system_clock::to_time_t(
				std::chrono::system_clock::now());
	} else if (variable == "caption") {
		caption = value;
		modification = std::chrono::system_clock::to_time_t(
					std::chrono::system_clock::now());
	} else if (variable == "active" || variable == "name")
		Executable::field(variable, position, value, derived);
	else
		Location::field(variable, position, value, derived);
}

Action::Action(std::string label, Data collection, std::string caption,
		base::Log track) :
		Location(nullptr, track), Executable(
				label, track), collection(collection) {
	this->caption = caption;
	std::clog << track.tracker() << "Action::Action(std::string label=\"" << label
			<< "\", Data collection=" << (std::string) collection
			<< ", std::string caption=\"" << caption << "\")=" << this << std::endl;
}

//Effect
std::string Effect::description() const {
	std::clog << base::Log().tracker() << "std::string Effect{" << this
			<< "}.description()=" << caption << std::endl;

	return caption;
}
std::string Effect::field(std::string variable, unsigned derived) const {
	if (variable == "caption")
		return caption;
	else
		return Executable::field(variable, derived);
}
void Effect::field(std::string variable, size_t position,
		std::string value, unsigned derived) {
	if (variable == "caption")
		caption = value;
	else
		Executable::field(variable, position, value, derived);
}

Effect::Effect(std::string label, std::string caption, base::Log track) :
		Executable(label, track) {
	std::clog << track.tracker() << "Action::Action(std::string label=" << label
			<< ", std::string caption=" << caption << ")=" << this << std::endl;
}

//Card
const char Card::base = 'E';
const char Card::interactive = 'I';
const char Card::complex = 'K';

std::tuple<std::vector<std::unique_ptr<Executable>>, std::vector<Data>,
		std::vector<Data>> Card::face(std::map<std::string, std::string> db,
		base::Log track) {
	std::vector<std::unique_ptr<Executable>> effects;
	std::vector<Data> prerequisites;
	std::vector<Data> costs;
	std::string buffer;

	std::clog << track.tracker()
			<< "std::tuple< std::vector< std::unique_ptr< Executable > >, "
					"std::vector< Data >, std::vector< Data > > "
					"Card::face(std::map< std::string, std::string > db=";
	for (auto element : db)
		buffer += ",\n" + element.first + "=" + element.second;
	if (buffer.empty())
		buffer = "{ }";
	else {
		buffer.erase(0, 1);
		buffer = "{" + buffer + "\n}";
	}
	std::clog << buffer << ") {" << std::endl;

	buffer.clear();
	for (auto e = 0; db.find((buffer = "effect" + std::to_string(e) + "/label"))
			!= db.end(); ++e)
		effects.emplace_back(new Effect(db[buffer], db["effect" + std::to_string(e)
				+ "/caption"], track));
	for (auto a = 0; db.find((buffer = "action" + std::to_string(a) + "/label"))
			!= db.end(); ++a) {
		Data data(track);

		if (db.find("action" + std::to_string(a) + "/Data/label") != db.end()) {
			data.quantity = std::stoi(db["action" + std::to_string(a) + "/Data/quantity"]);
			data.label = db["action" + std::to_string(a) + "/Data/label"];
			data.player = std::stoul(db["action" + std::to_string(a) + "/Data/player"]);
			data.note = db["action" + std::to_string(a) + "/Data/note"];
		}
		effects.emplace_back(new ::Action(db[buffer], data, db["action" + std::to_string(a)
				+ "/caption"], track));
	}
	for (auto p = 0; db.find((buffer = "prerequisite" + std::to_string(p) + "/label"))
			!= db.end(); ++p) {
		prerequisites.emplace_back(track);
		prerequisites.back().quantity = std::stoi(db["prerequisite" + std::to_string(p) + "/quantity"]);
		prerequisites.back().label = db["prerequisite" + std::to_string(p) + "/label"];
		prerequisites.back().player = std::stoul(db["prerequisite" + std::to_string(p) + "/player"]);
		prerequisites.back().note = db["prerequisite" + std::to_string(p) + "/note"];
	}
	for (auto c = 0; db.find((buffer = "cost" + std::to_string(c) + "/label"))
			!= db.end(); ++c) {
		costs.emplace_back(track);
		costs.back().quantity = std::stoi(db["cost" + std::to_string(c) + "/quantity"]);
		costs.back().label = db["cost" + std::to_string(c) + "/label"];
		costs.back().player = std::stoul(db["cost" + std::to_string(c) + "/player"]);
		costs.back().note = db["cost" + std::to_string(c) + "/note"];
	}
	std::clog << track() << "}={ " << list_unique_ptrs(effects)
			<< " " << list(prerequisites) << " " << list(costs) << " }" << std::endl;

	return std::make_tuple(std::move(effects), prerequisites, costs);
}

std::string Card::what() const {
	std::clog << base::Log().tracker() << "std::string Card{" << this
			<< "}.what()=" << typeid(*this).name() << std::endl;

	return typeid(*this).name();
}
base::Object* Card::enter(std::string content, size_t number,
		unsigned derived) const {
	return game::Card::enter(content, number, derived);
}
std::string Card::field(std::string variable, unsigned derived) const {
	if (variable == "type")
		return type;
	else if (variable == "c")
		return colorNames.at(c);
	else if (variable == "base")
		return std::string(base, 1);
	else if (variable == "interactive")
		return std::string(interactive, 1);
	else if (variable == "complex")
		return std::string(complex, 1);
	else
		return game::Card::field(variable, derived);
}
void Card::field(std::string variable, size_t position,
		std::string value, unsigned derived) {
	if (variable == "type") {
		type = value;
		modification = std::chrono::system_clock::to_time_t(
					std::chrono::system_clock::now());
	} else if (variable == "c") {
		auto color = find_color(value);
		std::ostringstream result;

		if (color == colorNames.end()) {
			result << "void Card{" << this
					<< "}.field(std::string variable=\"" << variable
					<< "\", size_t position=" << position
					<< ", std::string value=\"" << value << "\", derived="
					<< derived
					<< ")= ERROR invalid argument: invalid value (color name).";
			std::cerr << result.str() << std::endl;
			throw std::invalid_argument(result.str());
		} else {
			c = color->first;
			modification = std::chrono::system_clock::to_time_t(
						std::chrono::system_clock::now());
		}
	} else
		game::Card::field(variable, position, value, derived);
}

Color Card::color() const {
	std::clog << base::Log().tracker() << "std::string Card{" << this
			<< "}.color()=" << colorNames.at(c) << std::endl;

	return c;
}

std::unique_ptr<Card> Card::occupation(unsigned number, base::Log track) {
	std::tuple<std::vector<std::unique_ptr<Executable>>, std::vector<Data>,
			std::vector<Data>> parameters;
	std::map<std::string, std::string> db;
	std::unique_ptr<Card> result;

	std::clog << track.tracker() << "std::unique_ptr<Card> "
			"Card::occupation(unsigned number=" << number << ") {" << std::endl;
	db = base::datafile(std::to_string(number), OCCUPATIONS_DB, track);
	parameters = face(db, track);
	if (!db.empty())
		result.reset(
			new Card(Occupation::type, Color::Yellow,
					std::unique_ptr<Location>(
							new Cover(Occupation::type, Occupation::caption,
									Occupation::cover, track)),
					std::unique_ptr<Location>(
							new Occupation(std::stoul(db["players"]),
									db["bp"] == "true",
									std::move(std::get<0>(parameters)),
									std::get<1>(parameters), db["name"],
									std::get<2>(parameters), number,
									db["edition"].front(), db["caption"],
									nullptr, track)), nullptr, track));
	std::clog << track() << "}=" << result.get() << std::endl;

	return std::move(result);
}
std::unique_ptr<Card> Card::improvement(unsigned number, base::Log track) {
	std::unique_ptr<Card> result;
	std::tuple<std::vector<std::unique_ptr<Executable>>, std::vector<Data>,
				std::vector<Data>> parameters;
	std::map<std::string, std::string> db;
	bool major = false;

	std::clog << track.tracker()
			<< "std::unique_ptr< Card > Card::occupation(unsigned number="
			<< number << ") {" << std::endl;
	db = base::datafile(std::to_string(number), IMPROVEMENTS_DB, track);
	parameters = face(db, track);
	if ((major = db["major"] == "true"))
		result.reset(
				new Card(Improvement::majorType, Color::Crimson,
						std::unique_ptr<Location>(
								new Cover(Improvement::majorType,
										Improvement::majorCaption,
										Improvement::majorCover, track)),
						std::unique_ptr<Location>(
								new Improvement(major, std::stoi(db["vp"]),
										db["oven"] == "true", db["bp"] == "true",
										db["kitchen"] == "true",
										std::move(std::get<0>(parameters)),
										std::get<1>(parameters), db["name"],
										std::get<2>(parameters), number,
										db["edition"].front(), db["caption"],
										nullptr, track)), nullptr, track));
	else
		result.reset(
				new Card(Improvement::minorType, Color::Orange,
						std::unique_ptr<Location>(
								new Cover(Improvement::minorType,
										Improvement::minorCaption,
										Improvement::minorCover, track)),
						std::unique_ptr<Location>(
								new Improvement(major, std::stoi(db["vp"]),
										db["oven"] == "true", db["bp"] == "true",
										db["kitchen"] == "true",
										std::move(std::get<0>(parameters)),
										std::get<1>(parameters), db["name"],
										std::get<2>(parameters), number,
										db["edition"].front(), db["caption"],
										nullptr, track)), nullptr, track));
	std::clog << track() << "}" << result.get() << std::endl;

	return std::move(result);
}
std::unique_ptr<Card> Card::round(std::string label, base::Log track) {
	std::unique_ptr<Round> cover;
	std::unique_ptr<Card::Action> action;
	std::unique_ptr<Card> result;
	std::vector<std::unique_ptr<::Action>> actions;
	std::ostringstream log;

	log << track.tracker()
			<< "std::unique_ptr< Card > Card::round(std::string label="
			<< label << ")";
	std::clog << log.str() << " {" << std::endl;
	if (label == EX_R1_SOW_BAKEBREAD) {
		cover.reset(new Round(1, 1, 4, track));
		actions.emplace_back(
				new ::Action(label, Data(track),
						"{t}Sow\n#^(G)(G)->(G)^^(V)->(V)^#{t}"
								"and/or{t}Bake bread{t}", track));
		action.reset(
				new Action(0, false, "{R}Stage 1{R}", std::move(actions),
						nullptr, track));
	} else if (label == EX_R1_IMPROVEMENT) {
		cover.reset(new Round(1, 1, 4, track));
		actions.emplace_back(
				new ::Action(label, Data(track),
						"1 Major or Minor{t}Improvement{t}", track));
		action.reset(
				new Action(0, false, "{R}Stage 1{R}", std::move(actions),
						nullptr, track));
	} else if (label == EX_R1_SHEEP) {
		cover.reset(new Round(1, 1, 4, track));
		actions.emplace_back(
				new ::Action(label, Data(1, "Sheep", 0, "", track),
						"1 Sheep\n#[S]#", track));
		action.reset(
				new Action(0, false, "{R}Stage 1{R}", std::move(actions),
						nullptr, track));
	} else if (label == EX_R1_FENCES) {
		cover.reset(new Round(1, 1, 4, track));
		actions.emplace_back(new ::Action(label, Data(track), "{t}Fences{t}"
				"1 Wood per Fence", track));
		action.reset(
				new Action(0, false, "{R}Stage 1{R}", std::move(actions),
						nullptr, track));
	} else if (label == EX_R2_STONE) {
		cover.reset(new Round(2, 5, 7, track));
		actions.emplace_back(
				new ::Action(label, Data(1, "Stone", 0, "", track),
						"1 Stone#(S)#", track));
		action.reset(
				new Card::Action(0, false, "{R}Stage 2{R}", std::move(actions),
						nullptr, track));
	} else if (label == EX_R2_RENOVATION_IMPROVEMENT) {
		cover.reset(new Round(2, 5, 7, track));
		actions.emplace_back(
				new ::Action(label, Data(track),
						"After{t}Renovation{t}#1(R)#plus"
								"{l}#1(C)x#room - Clay Hut\n"
								"#1(S)x#room - Stone House{l}"
								"also\n1 Major or Minor{t}Improvement{t}",
						track));
		action.reset(
				new Action(0, false, "{R}Stage 2{R}", std::move(actions),
						nullptr, track));
	} else if (label == EX_R2_FAMILYGROWTH_MINORIMPROVEMENT) {
		cover.reset(new Round(2, 5, 7, track));
		actions.emplace_back(
				new ::Action(label, Data(track),
						"After{t}Family Growth{t}also\n"
								"1 Minor{t}Improvement{t}", track));
		action.reset(
				new Action(0, false, "{R}Stage 2{R}", std::move(actions),
						nullptr, track));
	} else if (label == EX_R3_TAKEVEGETABLE) {
		cover.reset(new Round(3, 8, 9, track));
		actions.emplace_back(
				new ::Action(label, Data(track), "{t}Take 1 Vegetable{t}"
						"and place it in your personal supply{t}#+(V)#{t}",
						track));
		action.reset(
				new Action(0, false, "{R}Stage 3{R}", std::move(actions),
						nullptr, track));
	} else if (label == EX_R3_WILDBOAR) {
		cover.reset(new Round(3, 8, 9, track));
		actions.emplace_back(
				new ::Action(label, Data(1, "Wild Boar", 0, "", track),
						"1 Wild boar\n#[B]#", track));
		action.reset(
				new Action(0, false, "{R}Stage 3{R}", std::move(actions),
						nullptr, track));
	} else if (label == EX_R4_STONE) {
		cover.reset(new Round(4, 10, 11, track));
		actions.emplace_back(
				new ::Action(label, Data(1, "Stone", 0, "", track),
						"1 Stone\n#(S)#", track));
		action.reset(
				new Action(0, false, "{R}Stage 4{R}", std::move(actions),
						nullptr, track));
	} else if (label == EX_R4_CATTLE) {
		cover.reset(new Round(4, 10, 11, track));
		actions.emplace_back(
				new ::Action(label, Data(1, "Cattle", 0, "", track),
						"1 Cattle\n#[C]#", track));
		action.reset(
				new Action(0, false, "{R}Stage 4{R}", std::move(actions),
						nullptr, track));
	} else if (label == EX_R5_FIELD_SOW) {
		cover.reset(new Round(5, 12, 13, track));
		actions.emplace_back(
				new ::Action(label, Data(track),
						"{t}Plow 1 Field{t}and/or{t}Sow"
								"#^(G)(G)->(G)^^(V)->(V)^#{t}", track));
		action.reset(
				new Action(0, false, "{R}Stage 5{R}", std::move(actions),
						nullptr, track));
	} else if (label == EX_R5_FAMILYGROWTH) {
		cover.reset(new Round(5, 12, 13, track));
		actions.emplace_back(
				new ::Action(label, Data(track),
						"{t}Family growth{t}even without a room", track));
		action.reset(
				new Action(0, false, "{R}Stage 5{R}", std::move(actions),
						nullptr, track));
	} else if (label == EX_R6_RENOVATION_FENCES) {
		cover.reset(new Round(6, 14, 14, track));
		actions.emplace_back(
				new ::Action(label, Data(track),
						"After{t}Renovation{t}#1(R)#plus"
								"{l}#1(C)x#room - Clay Hut\n"
								"#1(S)x#room - Stone House{l}"
								"also{t}Fences{t}1 Wood per Fence", track));
		action.reset(
				new Action(0, false, "{R}Stage 6{R}", std::move(actions),
						nullptr, track));
	} else {
		std::string message = "= ERROR invalid argument: unknown label.";

		std::clog << track() << "}" << message << std::endl;
		std::cerr << log.str() << message << std::endl;
		throw std::invalid_argument(log.str() + message);
	}
	result.reset(
			new Card(Round::type, Color::Blue, std::move(cover),
					std::move(action), nullptr, track));
	std::clog << track() << "}=" << result.get() << std::endl;

	return std::move(result);
}
std::unique_ptr<Card> Card::action(short unsigned players,
		std::vector<std::tuple<std::string, Data, std::string>> actions,
		short unsigned familyPlayers,
		std::vector<std::tuple<std::string, Data, std::string>> familyActions,
		base::Log track) {
	std::vector<std::unique_ptr<::Action>> actioning;
	std::vector<std::unique_ptr<::Action>> familyActioning;
	std::unique_ptr<Card> result;
	std::ostringstream a;
	std::ostringstream fa;
	auto Ac = actions.end();
	auto FAc = familyActions.end();

	for (auto ac = actions.begin(); ac != Ac; ++ac)
		a << ", (" << std::get<0>(*ac) << "; " << (std::string) std::get<1>(*ac)
				<< "; " << std::get<2>(*ac) << ")";
	a.str(actions.size() ? "{" + a.str().substr(1) : "{");
	a << " }";
	for (auto fac = actions.begin(); fac != FAc; ++fac)
		fa << ", (" << std::get<0>(*fac) << "; "
				<< (std::string) std::get<1>(*fac) << "; " << std::get<2>(*fac)
				<< ")";
	fa.str(actions.size() ? "{" + a.str().substr(1) : "{");
	fa << " }";
	std::clog << track.tracker()
			<< "std::unique_ptr< Card > Card::action(short unsigned players="
			<< players
			<< ", std::vector< std::tuple< std::string, std::vector< Data >,"
					" std::string > > actions=" << a.str()
			<< ", short unsigned family_players=" << familyPlayers
			<< ", std::vector< std::tuple< std::string, std::vector< Data >,"
					" std::string > > family_actions=" << fa.str() << ") {"
			<< std::endl;
	for (auto ac = actions.begin(); ac != Ac; ++ac)
		actioning.emplace_back(
				new ::Action(std::get<0>(*ac), std::get<1>(*ac),
						std::get<2>(*ac), track));
	for (auto fac = actions.begin(); fac != FAc; ++fac)
		familyActioning.emplace_back(
				new ::Action(std::get<0>(*fac), std::get<1>(*fac),
						std::get<2>(*fac), track));
	result.reset(
			new Card(Action::type, Color::Green,
					std::unique_ptr<Location>(
							new Action(familyPlayers, true, "",
									std::move(familyActioning), nullptr,
									track)),
					std::unique_ptr<Location>(
							new Action(players, false, "", std::move(actioning),
									nullptr, track)), nullptr, track));
	std::clog << track() << "}" << result.get() << std::endl;

	return std::move(result);
}
std::unique_ptr<Card> Card::begging(base::Log track) {
	std::unique_ptr<Card> result;

	std::clog << track.tracker() << "Card::begging()";
	result.reset(
			new Card(Begging::type, Color::Brown,
					std::unique_ptr<Location>(new Begging(nullptr, track)),
					std::unique_ptr<Location>(new Begging(nullptr, track)),
					nullptr, track));
	std::clog << track() << "}" << result.get() << std::endl;

	return std::move(result);
}

Card::Card(std::string type, Color color, std::unique_ptr<Location>&& cover,
		std::unique_ptr<Location>&& face, Location* position,
		base::Log track) :
		game::Card(std::move(cover), std::move(face), position, track
				) {
	this->type = type;
	c = color;
	std::clog << track.tracker() << "Card::Card(std::string type=\"" << type
			<< "\", Color color=" << colorNames.at(color)
			<< ", std::unique_ptr< base::Location >&& cover="
			<< operator ()(true)
			<< ", std::unique_ptr< base::Location >&& face="
			<< operator ()(false) << ", base::Location* position=" << position
			<< ")=" << this << std::endl;
}

//Card::Occupation
const std::string Card::Occupation::type = "Occupation";
const std::string Card::Occupation::caption = "";
const std::string Card::Occupation::cover = "Occupation_cover";

std::string Card::Occupation::what() const {
	std::clog << base::Log().tracker() << "std::string Card::Occupation{"
			<< this << "}.what()=" << typeid(*this).name() << std::endl;

	return typeid(*this).name();
}
std::string Card::Occupation::description() const {
	std::clog << base::Log().tracker() << "std::string Card::Occupation{"
			<< this << "}.description()=" << caption << std::endl;

	return caption;
}
base::Object* Card::Occupation::enter(std::string content, size_t number,
		unsigned derived) const {
	return Face::enter(content, number, derived);
}
std::string Card::Occupation::field(std::string variable,
		unsigned derived) const {
	if (variable == "p")
		return std::to_string(p);
	else if (variable == "bp")
		return bp ? "true" : "false";
	else if (variable == "type")
		return type;
	else if (variable == "caption" && derived == 0)
		return caption;
	else if (variable == "cover")
		return cover;
	else
		return Face::field(variable, derived);
}
void Card::Occupation::field(std::string variable, size_t position,
		std::string value, unsigned derived) {
	if (variable == "p") {
		p = std::stoul(value);
		modification = std::chrono::system_clock::to_time_t(
					std::chrono::system_clock::now());
	} else if (variable == "bp") {
		bp = value == "true";
		modification = std::chrono::system_clock::to_time_t(
				std::chrono::system_clock::now());
	} else
		Face::field(variable, position, value, derived);
}

short unsigned Card::Occupation::players() const {
	std::clog << base::Log().tracker() << "short unsigned Card::Occupation{"
			<< this << "}.players()=" << p << std::endl;

	return p;
}
bool Card::Occupation::bonus_points() const {
	std::clog << base::Log().tracker() << "bool Card::Occupation{" << this
			<< "}.bonus_points()=" << bp << std::endl;

	return bp;
}

Card::Occupation::Occupation(short unsigned players,
		bool bonus_points, std::vector<std::unique_ptr<Executable>> events,
		std::vector<Data> prerequisites, std::string label,
		std::vector<Data> costs, unsigned number, char edition,
		std::string caption, Location* position, base::Log track) :
		Face(prerequisites, label, costs, number, edition, caption,
				std::move(events), position,
				track) {
	bp = bonus_points;
	p = players;
	std::clog << track.tracker()
			<< "Card::Occupation::Occupation(short unsigned players="
			<< players << ", bool bonus_points=" << bonus_points
			<< ", std::vector< std::unique_ptr< Executable > > events="
			<< list(executables())
			<< ", std::initializer_list< Data > prerequisites="
			<< list(prerequisites) << ", std::string label=\"" << label
			<< "\", std::initializer_list< Data > costs=" << list(costs)
			<< ", unsigned number=" << number << ", char edition='" << edition
			<< "', std::string caption=\"" << caption
			<< "\", base::Location* position=" << position << ")=" << this
			<< std::endl;
}

//Card::Improvement
const std::string Card::Improvement::minorType = "Minor Improvement";
const std::string Card::Improvement::majorType = "Major Improvement";
const std::string Card::Improvement::minorCaption = "";
const std::string Card::Improvement::majorCaption = "";
const std::string Card::Improvement::majorCover = "MinorImprovement_cover";
const std::string Card::Improvement::minorCover = "MajorImprovement_cover";

std::string Card::Improvement::what() const {
	std::clog << base::Log().tracker() << "std::string Card::Improvement{"
			<< this << "}.what()=" << typeid(*this).name() << std::endl;

	return typeid(*this).name();
}
std::string Card::Improvement::description() const {
	std::clog << base::Log().tracker() << "std::string Card::Improvement{"
			<< this << "}.description()=" << caption << std::endl;

	return caption;
}
base::Object* Card::Improvement::enter(std::string content, size_t number,
		unsigned derived) const {
	return Face::enter(content, number, derived);
}
std::string Card::Improvement::field(std::string variable,
		unsigned derived) const {
	if (variable == "vp")
		return std::to_string(vp);
	else if (variable == "o")
		return o ? "true" : "false";
	else if (variable == "bp")
		return bp ? "true" : "false";
	else if (variable == "k")
		return k ? "true" : "false";
	else if (variable == "order")
		return order ? "true" : "false";
	else if (variable == "minorType")
		return minorType;
	else if (variable == "majorType")
		return majorType;
	else if (variable == "minorCaption")
		return minorCaption;
	else if (variable == "majorCaption")
		return majorCaption;
	else if (variable == "minorCover")
		return minorCover;
	else if (variable == "majorCover")
		return majorCover;
	else
		return Face::field(variable, derived);
}
void Card::Improvement::field(std::string variable, size_t position,
		std::string value, unsigned derived) {
	if (variable == "vp") {
		vp = std::stoi(value);
		modification = std::chrono::system_clock::to_time_t(
					std::chrono::system_clock::now());
	} else if (variable == "o") {
		o = value == "true";
		modification = std::chrono::system_clock::to_time_t(
				std::chrono::system_clock::now());
	} else if (variable == "bp") {
		bp = value == "true";
		modification = std::chrono::system_clock::to_time_t(
				std::chrono::system_clock::now());
	} else if (variable == "k") {
		k = value == "true";
		modification = std::chrono::system_clock::to_time_t(
				std::chrono::system_clock::now());
	} else if (variable == "order") {
		order = value == "true";
		modification = std::chrono::system_clock::to_time_t(
				std::chrono::system_clock::now());
	} else
		Face::field(variable, position, value, derived);
}

int Card::Improvement::victory_points() const {
	std::clog << base::Log().tracker() << "int Card::Improvement{" << this
			<< "}.victory_points()=" << vp << std::endl;

	return vp;
}
bool Card::Improvement::oven() const {
	std::clog << base::Log().tracker() << "bool Card::Improvement{" << this
			<< "}.oven()=" << o << std::endl;

	return o;
}
bool Card::Improvement::bonus_points() const {
	std::clog << base::Log().tracker() << "bool Card::Improvement{" << this
			<< "}.bonus_points()=" << bp << std::endl;

	return bp;
}
bool Card::Improvement::kitchen() const {
	std::clog << base::Log().tracker() << "bool Card::Improvement{" << this
			<< "}.kitchen()=" << k << std::endl;

	return k;
}

Card::Improvement::Improvement(bool major, int vp, bool oven, bool bp,
		bool kitchen, std::vector<std::unique_ptr<Executable>> events,
		std::vector<Data> prerequisites, std::string label,
		std::vector<Data> costs, unsigned number, char edition,
		std::string caption, Location* position, base::Log track) :
		Face(prerequisites, label, costs, number, edition, caption,
				std::move(events), position, track) {
	this->vp = vp;
	o = oven;
	this->bp = bp;
	k = kitchen;
	order = major;
	std::clog << track.tracker() << "Card::Improvement::Improvement(bool major="
			<< major << ", int vp=" << vp << ", bool oven=" << oven
			<< ", bool bp=" << bp << ", bool kitchen=" << kitchen
			<< ", std::vector< std::unique_ptr< Executable > > events="
			<< list(executables())
			<< ", std::initializer_list< Data > prerequisites="
			<< list(prerequisites) << ", std::string label=\"" << label
			<< "\", std::initializer_list< Data > costs=" << list(costs)
			<< ", unsigned number=" << number << ", char edition='" << edition
			<< "', std::string caption=\"" << caption
			<< "\", base::Location* position=" << position << ")=" << this
			<< std::endl;
}

//Card::Round
const std::string Card::Round::type = "Round card";

unsigned Card::Round::stage() const {
	std::clog << base::Log().tracker() << "unsigned Card::Round{" << this
			<< "}.stage()=" << staging << std::endl;

	return staging;
}
std::pair<unsigned, unsigned> Card::Round::round() const {
	std::clog << base::Log().tracker()
			<< "std::pair< unsigned, unsigned > Card::Round{" << this
			<< "}.round()=(" << min << "; " << max << std::endl;

	return std::make_pair(min, max);
}

base::Object* Card::Round::enter(std::string content, size_t number,
		unsigned derived) const {
	return Cover::enter(content, number, derived);
}
std::string Card::Round::field(std::string variable, unsigned derived) const {
	if (variable == "staging")
		return std::to_string(staging);
	else if (variable == "min")
		return std::to_string(min);
	else if (variable == "max")
		return std::to_string(max);
	else if (variable == "type" && derived == 0)
		return type;
	else
		return Cover::field(variable, derived);
}
void Card::Round::field(std::string variable, size_t position,
		std::string value, unsigned derived) {
	if (variable == "staging") {
		staging = std::stoul(value);
		modification = std::chrono::system_clock::to_time_t(
					std::chrono::system_clock::now());
	} else if (variable == "min") {
		min = std::stoul(value);
		modification = std::chrono::system_clock::to_time_t(
				std::chrono::system_clock::now());
	} else if (variable == "max") {
		max = std::stoul(value);
		modification = std::chrono::system_clock::to_time_t(
				std::chrono::system_clock::now());
	} else
		Cover::field(variable, position, value, derived);
}

Card::Round::Round(unsigned stage, unsigned min, unsigned max,
		base::Log track) :
		Cover(std::to_string(stage),
				std::to_string(min) + "-" + std::to_string(max), type, track) {
	staging = stage;
	this->min = min;
	this->max = max;
	std::clog << track.tracker() << "Card::Round::Round(unsigned stage=" << stage
			<< ", unsigned min=" << min << ", unsigned max=" << max << ")="
			<< this << std::endl;
}

//Card::Action
const std::string Card::Action::type = "Action card";

short unsigned Card::Action::players() const {
	std::clog << base::Log().tracker() << "short unsigned Card::Action{"
			<< this << "}.players()=" << p << std::endl;

	return p;
}
bool Card::Action::family() const {
	std::clog << base::Log().tracker() << "bool Card::Action{" << this
			<< "}.family()=" << f << std::endl;

	return f;
}
std::string Card::Action::description() const {
	std::clog << base::Log().tracker() << "std::string Card::Action{" << this
			<< "}.description()=" << caption << std::endl;

	return caption;
}

std::string Card::Action::what() const {
	std::clog << base::Log().tracker() << "std::string Card::Action{" << this
			<< "}.what()=" << typeid(*this).name() << std::endl;

	return typeid(*this).name();
}
base::Object* Card::Action::enter(std::string content, size_t number,
		unsigned derived) const {
	return Location::enter(content, number, derived);
}
std::string Card::Action::field(std::string variable, unsigned derived) const {
	if (variable == "p")
		return std::to_string(p);
	else if (variable == "f")
		return f ? "true" : "false";
	else if (variable == "caption")
		return caption;
	else if (variable == "type" && derived == 0)
		return type;
	else
		return Location::field(variable, derived);
}
void Card::Action::field(std::string variable, size_t position,
		std::string value, unsigned derived) {
	if (variable == "p") {
		p = std::stoul(value);
		modification = std::chrono::system_clock::to_time_t(
					std::chrono::system_clock::now());
	} else if (variable == "f") {
		f = value == "true";
		modification = std::chrono::system_clock::to_time_t(
				std::chrono::system_clock::now());
	} else if (variable == "caption") {
		caption = value;
		modification = std::chrono::system_clock::to_time_t(
				std::chrono::system_clock::now());
	} else
		Location::field(variable, position, value, derived);
}

Card::Action::Action(short unsigned players, bool family, std::string caption,
		std::vector<std::unique_ptr<::Action>> actions, Location* position,
		base::Log track) :
		Location(position, track) {
	auto A = actions.size();

	std::clog << track.tracker() << "Card::Action::Action(short unsigned players="
			<< players << ", bool family=" << family << ", std::string caption=\""
			<< caption << "\", std::vector< std::unique_ptr< Action > > actions="
			<< list_unique_ptrs(actions) << ", base::Location* position="
			<< position << ") {" << std::endl;
	p = players;
	f = family;
	this->caption = caption;
	for (size_t a = 0; a < A; ++a) {
		std::string label = *actions[a];

		Location::insert_back(label,
				std::unique_ptr<Object>(actions[a].release()), track);
	}
	std::clog << track() << "}=" << this << std::endl;
}

//Card::Begging
const std::string Card::Begging::type = "Begging card";

std::string Card::Begging::name() const {
	std::clog << base::Log().tracker() << "std::string Card::Begging{" << this
			<< "}.name()=" << type << std::endl;

	return type;
}
int Card::Begging::victory_points() const {
	int result = -3;

	std::clog << base::Log().tracker() << "int Card::Begging{" << this
			<< "}.victory_points()=" << result << std::endl;

	return result;
}
std::string Card::Begging::description() const {
	std::string result = "During the Feeding phase of the Harvest,"
			" whenever you cannot or choose not to produce enough"
			" Food to feed your family, you must take 1 Begging"
			" card for each missing Food.";

	std::clog << base::Log().tracker() << "std::string Card::Begging{" << this
			<< "}::description()=" << result << std::endl;

	return result;
}

std::string Card::Begging::what() const {
	std::clog << base::Log().tracker() << "std::string Card::Begging{" << this
			<< "}.what()=" << type << std::endl;

	return type;
}
base::Object* Card::Begging::enter(std::string content, size_t number,
		unsigned derived) const {
	return Location::enter(content, number, derived);
}
std::string Card::Begging::field(std::string variable, unsigned derived) const {
	return Location::field(variable, derived);
}
void Card::Begging::field(std::string variable, size_t position,
		std::string value, unsigned derived) {
	Location::field(variable, position, value, derived);
}

Card::Begging::Begging(Location* position, base::Log track) :
		Location(position, track) {
	std::clog << track.tracker() << "Card::Begging::Begging(Location* position="
			<< position << ")=" << this << std::endl;
}

//Wooden
const std::map<Wooden::Shape, std::string> Wooden::shapeNames = { {
		Wooden::Shape::disc, "disc" }, { Wooden::Shape::house, "house" }, {
		Wooden::Shape::line, "line" }, { Wooden::Shape::coin, "coin" }, {
		Wooden::Shape::cube, "cube" }, { Wooden::Shape::cylinder, "cylinder" } };
const std::string Wooden::woodType = "Wood";
const std::string Wooden::clayType = "Clay";
const std::string Wooden::reedType = "Reed";
const std::string Wooden::stoneType = "Stone";
const std::string Wooden::grainType = "Grain";
const std::string Wooden::vegetableType = "Vegetable";
const std::string Wooden::sheepType = "Sheep";
const std::string Wooden::wildBoarType = "Wild Boar";
const std::string Wooden::cattleType = "Cattle";
const std::string Wooden::familyMemberType = "Family Member";
const std::string Wooden::stableType = "Stable";
const std::string Wooden::fenceType = "Fence";
const std::string Wooden::firstPlayerType = "First Player";

Color Wooden::color() const {
	std::clog << base::Log().tracker() << "Color Wooden{" << this
			<< "}.color()=" << colorNames.at(c) << std::endl;

	return c;
}
Wooden::Shape Wooden::shape() const {
	std::clog << base::Log().tracker() << "Wooden::Shape Wooden{" << this
			<< "}.shape()=" << shapeNames.at(s) << std::endl;

	return s;
}

std::string Wooden::what() const {
	std::clog << base::Log().tracker() << "std::string Wooden{" << this
			<< "}.what()=" << type << std::endl;

	return type;
}
base::Object* Wooden::enter(std::string content, size_t number,
		unsigned derived) const {
	return Object::enter(content, number, derived);
}
std::string Wooden::field(std::string variable, unsigned derived) const {
	if (variable == "type")
		return type;
	else if (variable == "c")
		return colorNames.at(c);
	else if (variable == "s")
		return shapeNames.at(s);
	else if (variable == "owner")
		return Owned::field(variable, derived);
	else
		return Object::field(variable, derived);
}
void Wooden::field(std::string variable, size_t position,
		std::string value, unsigned derived) {
	if (variable == "type") {
		type = value;
		modification = std::chrono::system_clock::to_time_t(
					std::chrono::system_clock::now());
	} else if (variable == "c") {
		auto color = find_color(value);
		std::ostringstream result;

		if (color == colorNames.end()) {
			result << "void Wooden{" << this
					<< "}.field(std::string variable=\"" << variable
					<< "\", size_t position=" << position
					<< ", std::string value=\"" << value << "\", derived="
					<< derived
					<< ")= ERROR invalid argument: invalid value (color name).";
			std::cerr << result.str() << std::endl;
			throw std::invalid_argument(result.str());
		} else {
			c = color->first;
			modification = std::chrono::system_clock::to_time_t(
						std::chrono::system_clock::now());
		}
	} else if (variable == "s") {
		auto shape = find_shape(value);
		std::ostringstream result;

		if (shape == shapeNames.end()) {
			result << "void Wooden{" << this
					<< "}.field(std::string variable=\"" << variable
					<< "\", size_t position=" << position
					<< ", std::string value=\"" << value << "\", derived="
					<< derived
					<< ")= ERROR invalid argument: invalid value (shape name).";
			std::cerr << result.str() << std::endl;
			throw std::invalid_argument(result.str());
		} else {
			s = shape->first;
			modification = std::chrono::system_clock::to_time_t(
						std::chrono::system_clock::now());
		}
	} else if (variable == "owner")
		Owned::field(variable, position, value, derived);
	else
		Object::field(variable, position, value, derived);
}

std::map<Wooden::Shape, std::string>::const_iterator Wooden::find_shape(std::string shape) {
	auto s = shapeNames.begin();
	auto S = shapeNames.end();

	while (s->second != shape && s != S)
		++s;

	return s;
}
std::unique_ptr<Wooden> Wooden::wood(base::Log track) {
	std::unique_ptr<Wooden> result;

	std::clog << track.tracker()
			<< "std::unique_ptr< Wooden > Wooden::wood() {" << std::endl;
	result.reset(
			new Wooden(0, woodType, Shape::coin, Color::Brown, nullptr, track));
	std::clog << track() << "}=" << result.get() << std::endl;

	return std::move(result);
}
std::unique_ptr<Wooden> Wooden::clay(base::Log track) {
	std::unique_ptr<Wooden> result;

	std::clog << track.tracker()
			<< "std::unique_ptr< Wooden > Wooden::clay() {" << std::endl;
	result.reset(
			new Wooden(0, clayType, Shape::coin, Color::Red, nullptr, track));
	std::clog << track() << "}=" << result.get() << std::endl;

	return std::move(result);
}
std::unique_ptr<Wooden> Wooden::reed(base::Log track) {
	std::unique_ptr<Wooden> result;

	std::clog << track.tracker()
			<< "std::unique_ptr< Wooden > Wooden::.reed() {" << std::endl;
	result.reset(
			new Wooden(0, reedType, Shape::coin, Color::White, nullptr, track));
	std::clog << track ()<< "}=" << result.get() << std::endl;

	return std::move(result);
}
std::unique_ptr<Wooden> Wooden::stone(base::Log track) {
	std::unique_ptr<Wooden> result;

	std::clog << track.tracker()
			<< "std::unique_ptr< Wooden > Wooden::stone() {" << std::endl;
	result.reset(
			new Wooden(0, stoneType, Shape::coin, Color::Black, nullptr,
					track));
	std::clog << track ()<< "}=" << result.get() << std::endl;

	return std::move(result);
}
std::unique_ptr<Wooden> Wooden::grain(base::Log track) {
	std::unique_ptr<Wooden> result;

	std::clog << track.tracker()
			<< "std::unique_ptr< Wooden > Wooden::grain() {" << std::endl;
	result.reset(
			new Wooden(0, grainType, Shape::coin, Color::Yellow, nullptr,
					track));
	std::clog << track ()<< "}=" << result.get() << std::endl;

	return std::move(result);
}
std::unique_ptr<Wooden> Wooden::vegetable(base::Log track) {
	std::unique_ptr<Wooden> result;

	std::clog << track.tracker()
			<< "std::unique_ptr< Wooden > Wooden::vegetable() {" << std::endl;
	result.reset(
			new Wooden(0, vegetableType, Shape::coin, Color::Orange, nullptr,
					track));
	std::clog << track ()<< "}=" << result.get() << std::endl;

	return std::move(result);
}
std::unique_ptr<Wooden> Wooden::sheep(base::Log track) {
	std::unique_ptr<Wooden> result;

	std::clog << track .tracker()
			<< "std::unique_ptr< Wooden > Wooden::sheep() {" << std::endl;
	result.reset(
			new Wooden(0, sheepType, Shape::cube, Color::White, nullptr,
					track));
	std::clog << track ()<< "}=" << result.get() << std::endl;

	return std::move(result);
}
std::unique_ptr<Wooden> Wooden::wildBoar(base::Log track) {
	std::unique_ptr<Wooden> result;

	std::clog << track .tracker()
			<< "std::unique_ptr< Wooden > Wooden::wildBoar() {" << std::endl;
	result.reset(
			new Wooden(0, wildBoarType, Shape::cube, Color::Black, nullptr,
					track));
	std::clog << track ()<< "}=" << result.get() << std::endl;

	return std::move(result);
}
std::unique_ptr<Wooden> Wooden::cattle(base::Log track) {
	std::unique_ptr<Wooden> result;

	std::clog << track .tracker()
			<< "std::unique_ptr< Wooden > Wooden:cattle() {" << std::endl;
	result.reset(
			new Wooden(0, cattleType, Shape::cube, Color::Brown, nullptr,
					track));
	std::clog << track ()<< "}=" << result.get() << std::endl;

	return std::move(result);
}
std::unique_ptr<Wooden> Wooden::family_member(short unsigned player,
		base::Log track) {
	std::unique_ptr<Wooden> result;
	Player* p = nullptr;
	std::ostringstream log;

	log << track .tracker()
			<< "std::unique_ptr< Wooden > Wooden::family_member()";
	std::clog << log.str() << " {" << std::endl;
	if ((p = Player::player(player)))
		result.reset(
				new Wooden(0, familyMemberType, Shape::disc, p->color(),
						nullptr, track));
	else {
		std::ostringstream message;

		message << "= ERROR the player number=" << player << " not exists.";
		std::clog << track ()<< "}" << message.str() << std::endl;
		std::cerr << log.str() << message.str() << std::endl;
		throw std::length_error(log.str() + message.str());
	}
	std::clog << track ()<< "}=" << result.get() << std::endl;

	return std::move(result);
}
std::unique_ptr<Wooden> Wooden::stable(short unsigned player,
		base::Log track) {
	std::unique_ptr<Wooden> result;
	Player* p = nullptr;
	std::ostringstream log;

	log << track .tracker()
			<< "std::unique_ptr< Wooden > Wooden::stable()";
	std::clog << log.str() << " {" << std::endl;
	if ((p = Player::player(player)))
		result.reset(
				new Wooden(0, stableType, Shape::house, p->color(), nullptr,
						track));
	else {
		std::ostringstream message;

		message << "= ERROR the player number=" << player << " not exists.";
		std::clog << track ()<< "}" << message.str() << std::endl;
		std::cerr << log.str() << message.str() << std::endl;
		throw std::length_error(log.str() + message.str());
	}
	std::clog << track ()<< "}=" << result.get() << std::endl;

	return std::move(result);
}
std::unique_ptr<Wooden> Wooden::fence(short unsigned player,
		base::Log track) {
	std::unique_ptr<Wooden> result;
	Player* p = nullptr;
	std::ostringstream log;

	log << track .tracker()
			<< "std::unique_ptr< Wooden > Wooden::fence()";
	std::clog << log.str() << " {" << std::endl;
	if ((p = Player::player(player)))
		result.reset(
				new Wooden(0, fenceType, Shape::line, p->color(), nullptr,
						track));
	else {
		std::ostringstream message;

		message << "= ERROR the player number=" << player << " not exists.";
		std::clog << track ()<< "}" << message.str() << std::endl;
		std::cerr << log.str() << message.str() << std::endl;
		throw std::length_error(log.str() + message.str());
	}
	std::clog << track ()<< "}=" << result.get() << std::endl;

	return std::move(result);
}
std::unique_ptr<Wooden> Wooden::first_player(base::Log track) {
	std::unique_ptr<Wooden> result;

	std::clog << track .tracker()
			<< ": std::unique_ptr< Wooden > Wooden::family_member() {"
			<< std::endl;
	result.reset(
			new Wooden(0, firstPlayerType, Shape::cylinder, Color::Yellow,
					nullptr, track));
	std::clog << track ()<< "}=" << result.get() << std::endl;

	return std::move(result);
}

Wooden::Wooden(short unsigned owner, std::string type, Shape shape, Color color,
		base::Location* position, base::Log track) :
		base::Object(position, track ), Owned(
				owner, track) {
	this->type = type;
	c = color;
	s = shape;
	std::clog << track .tracker()<< "Wooden::Wooden(short unsigned owner=" << owner
			<< ", std::string type" << type << ", Shape shape="
			<< shapeNames.at(shape) << ", Color color=" << colorNames.at(color)
			<< ", base::Location* position=" << position << ")=" << this
			<< std::endl;
}

//Tile
const std::string Tile::woodenHutType = "Wooden Hut";
const std::string Tile::clayHutType = "Clay Hut";
const std::string Tile::stoneHouseType = "Stone House";
const std::string Tile::fieldType = "Field";

Color Tile::color() const {
	std::clog << base::Log().tracker() << "Tile{" << this << "}.color()="
			<< colorNames.at(c) << std::endl;

	return c;
}

std::string Tile::what() const {
	std::clog << base::Log().tracker() << "Tile{" << this << "}.what()="
			<< type << std::endl;

	return type;
}
base::Object* Tile::enter(std::string content, size_t number,
		unsigned derived) const {
	return Location::enter(content, number, derived);
}
std::string Tile::field(std::string variable, unsigned derived) const {
	if (variable == "type")
		return type;
	else if (variable == "c")
		return colorNames.at(c);
	else
		return Location::field(variable, derived);
}
void Tile::field(std::string variable, size_t position,
		std::string value, unsigned derived) {
	if (variable == "type") {
		type = value;
		modification = std::chrono::system_clock::to_time_t(
					std::chrono::system_clock::now());
	} else if (variable == "c") {
		auto color = find_color(value);
		std::ostringstream result;

		if (color == colorNames.end()) {
			result << "void Wooden{" << this
					<< "}.field(std::string variable=\"" << variable
					<< "\", size_t position=" << position
					<< ", std::string value=\"" << value << "\", derived="
					<< derived
					<< ")= ERROR invalid argument: invalid value (color name).";
			std::cerr << result.str() << std::endl;
			throw std::invalid_argument(result.str());
		} else {
			c = color->first;
			modification = std::chrono::system_clock::to_time_t(
						std::chrono::system_clock::now());
		}
	} else
		Location::field(variable, position, value, derived);
}

std::unique_ptr<Tile> Tile::wooden_hut(base::Log track) {
	std::unique_ptr<Tile> result;

	std::clog << track.tracker()
			<< "std::unique_ptr< Tile > Tile::wooden_hut() {" << std::endl;
	result.reset(new Tile(woodenHutType, Color::Brown, nullptr, track));
	std::clog << track ()<< "}=" << result.get() << std::endl;

	return std::move(result);
}
std::unique_ptr<Tile> Tile::clay_hut(base::Log track) {
	std::unique_ptr<Tile> result;

	std::clog << track.tracker()
			<< "std::unique_ptr< Tile > Tile::clay_hut() {" << std::endl;
	result.reset(new Tile(clayHutType, Color::Red, nullptr, track));
	std::clog << track ()<< "}=" << result.get() << std::endl;

	return std::move(result);
}
std::unique_ptr<Tile> Tile::stone_house(base::Log track) {
	std::unique_ptr<Tile> result;

	std::clog << track.tracker()
			<< "std::unique_ptr< Tile > Tile::stone_house() {" << std::endl;
	result.reset(new Tile(stoneHouseType, Color::Gray, nullptr, track));
	std::clog << track ()<< "}=" << result.get() << std::endl;

	return std::move(result);
}
std::unique_ptr<Tile> Tile::field(base::Log track) {
	std::unique_ptr<Tile> result;

	std::clog << track.tracker()
			<< "std::unique_ptr< Tile > Tile::field() {" << std::endl;
	result.reset(new Tile(fieldType, Color::Brown, nullptr, track));
	std::clog << track ()<< "}=" << result.get() << std::endl;

	return std::move(result);
}

Tile::Tile(std::string type, Color color, Location* position,
		base::Log track) :
		Location(position, track) {
	this->type = type;
	c = color;
	std::clog << track.tracker() << "Tile::Tile(std::string type=\"" << type
			<< "\", Color color=" << colorNames.at(color)
			<< ", base::Location* position=" << position << ")=" << this
			<< std::endl;
}

//Food
const std::string Food::type = "Food";

std::string Food::what() const {
	std::clog << base::Log().tracker() << "Food{" << this << "}.what()="
			<< typeid(*this).name() << std::endl;

	return typeid(*this).name();
}
base::Object* Food::enter(std::string content, size_t number,
		unsigned derived) const {
	return Object::enter(content, number, derived);
}
std::string Food::field(std::string variable, unsigned derived) const {
	return Object::field(variable, derived);
}
void Food::field(std::string variable, size_t position,
		std::string value, unsigned derived) {
	Object::field(variable, position, value, derived);
}

Food::Food(base::Log track) :
		Object(nullptr, track) {
	std::clog << track.tracker() << "Food::Food()=" << this << std::endl;
}

//Token
const std::string Token::guestType = "Guest";
const std::string Token::tokenType = "token";

std::string Token::what() const {
	std::clog << base::Log().tracker() << "Token{" << this << "}.what()="
			<< type << std::endl;

	return type;
}
base::Object* Token::enter(std::string content, size_t number,
		unsigned derived) const {
	return Object::enter(content, number, derived);
}
std::string Token::field(std::string variable, unsigned derived) const {
	return Object::field(variable, derived);
}
void Token::field(std::string variable, size_t position,
		std::string value, unsigned derived) {
	Object::field(variable, position, value, derived);
}

Token::Token(std::string type, base::Log track) :
		Object(nullptr, track) {
	this->type = type;
	std::clog << track.tracker() << "Token::Token(std::string type="
			<< type << ")=" << this << std::endl;
}
*/
