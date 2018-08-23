//============================================================================
// Name        : Agricola.cpp
// Author      : mlarosa
// Version     :
// Copyright   : m1r
// Description : Hello World in C++, Ansi-style
//============================================================================
/*
#define INPUT "input.txt"
#define LOG "log.txt"
#include <iostream>*/
using namespace std;
/*
#include <fstream>
#include "state_machine.h"
#include <src/ui.h>
#include <chrono>
#include <typeinfo>
#define S_GAMEOVER "end of the game"*/

int main() {/*
	bool family = false;
	vector<string> player_names;
	std::string parameter;
	unsigned derivation = 0;
#ifdef LOG
	ofstream log(LOG, ios_base::trunc);
	auto clog_stderr = clog.rdbuf(log.rdbuf());
#endif
#ifdef INPUT
	ifstream input(INPUT);
	auto cin_stdin = cin.rdbuf(input.rdbuf());
#endif
	base::Log track;

	cout << "Agricola" << endl; // prints Agricola

	cout << boolalpha;
	clog << boolalpha;
	cerr << boolalpha;
	clog << track.tracker() << "Agricola" << endl;
	family = base::dialog("Play a family game?").first;
	clog << track() << "Family game " << family << endl;
	for (auto p = 1; p < 6; ++p) {
		map<string, pair<set<string>, string>> variables;
		pair<bool, map<string, string>> names;

		variables.emplace("name", make_pair(set<string>(), string()));
		names = base::dialog("Player " + to_string(p) + ":", variables);
		clog << track() << "player " << p << ": " << names.first << "->name=\""
				<< names.second["name"] << "\"" << endl;
		if (names.first)
			player_names.push_back(names.second["name"]);
		else
			p = 5;
	}
	try {
		preparing_to_play(player_names, family);
		cards(family);
		starting_player();
		for (auto k = keys(); k.find(S_GAMEOVER) == k.end(); k = keys()) {
			if (harvest())
				switch (step()) {
				case 1:
					switch (phase()) {
					case 1:
						harvest_start();
						break;
					default:
						phase_start__1();
					}
					break;
				case 2:
					switch (phase()) {
					case 3:
						stage_end();
						break;
					default:
						phase_end__2();
					}
					break;
				case 3:
					switch (phase()) {
					case 1:
						harvest_phase1_field_1_3();
						break;
					case 2:
						harvest_phase2_feeding_the_family_2_3();
						break;
					case 3:
						harvest_phase3_breeding_3_3();
					}
					break;
				default:
					switch (phase()) {
					case 1:
						harvest_phase1_field_1_();
						break;
					case 2:
						harvest_phase2_feeding_the_family_2_();
						break;
					case 3:
						harvest_phase3_breeding_3_();
					}
				}
			else
				switch (step()) {
				case 1:
					switch (phase()) {
					case 1:
						if (new_stage())
							stage_start();
						else
							round_start();
						break;
					default:
						phase_start__1();
					}
					break;
				case 2:
					switch (phase()) {
					case 4:
						round_end();
						break;
					default:
						phase_end__2();
					}
					break;
				case 3:
					switch (phase()) {
					case 1:
						phase1_draw_a_new_round_card_1_3();
						break;
					case 2:
						phase2_replenish_2_3();
						break;
					case 3:
						phase3_work_3_3();
						break;
					case 4:
						phase4_return_home_4_3();
					}
					break;
				default:
					switch (phase()) {
					case 1:
						phase1_start_the_round_1_4();
						break;
					case 3:
						phase3_work_3_();
						break;
					case 4:
						phase4_return_home_4_();
					}
				}
			//TODO valutazione triggers
		}
		end_of_the_game();
	} catch (exception& e) {
		clog << track.close();
		cout << e.what() << endl;
	}
#ifdef INPUT
	cin.rdbuf(cin_stdin);
	input.close();
#endif
	parameter.clear();
	for (base::Object* pointer = nullptr; parameter != "quit";
			getline(cin, parameter)) {
		try {
			if (parameter == "status")
				cout << status() << endl;
			else if (parameter.substr(0, 6) == "field " && pointer)
				try {
					cout << pointer->field(parameter.substr(6), derivation)
							<< endl;
				} catch (exception& e) {
					cout << e.what() << endl;
				}
			else if (parameter == "where is" && pointer) {
				cout << path(base::Location::path(*pointer)) << endl;
			} else if (parameter == "who is root") {
				cout << base::Prompt::list(base::Object::root()) << endl;
			} else if (parameter == "everything") {
				cout << base::Prompt::list(base::Object::all()) << endl;
			} else if (parameter == "when is created" && pointer) {
				auto t = pointer->when();

				cout << ctime(&t) << endl;
			} else if (parameter == "when is modified" && pointer) {
				auto t = pointer->since();

				cout << ctime(&t);
			} else if (parameter == "exit" && pointer) {
				pointer = pointer->where();
				derivation = 0;
			} else if (parameter.substr(0, 6) == "enter " && pointer) {
				size_t i = 0;
				auto n = stoull(parameter.substr(6), &i);
				auto p = pointer->enter(parameter.substr(6 + i), n, derivation);

				if (p) {
					pointer = p;
					derivation = 0;
				}
			} else if (parameter.substr(0, 11) == "enter root ") {
				auto root = base::Object::root();
				auto r = root.begin();

				for (auto p = stoull(parameter.substr(11)); p > 0; --p)
					++r;
				pointer = const_cast<base::Object*>(*r);
				derivation = 0;
			} else if (parameter == "?")
				cout << "\ncheck {\n\tquit\n\tfield [*field]"
						"\n\twhere is\n\twho is root"
						"\n\twhen is created(*)\n\twhen is modified(*)"
						"\n\texit(*)\n\tenter [#][*field]"
						"\n\tenter root [#]\n\tcontains(*)"
						"\n\tderivation\n\tup(derivation)"
						"\n\tdown(derivation)\n\tdisplay\n} ... ";
			else if (parameter == "up")
				cout << ++derivation << endl;
			else if (parameter == "down" && derivation)
				cout << --derivation << endl;
			else if (parameter == "contains" && pointer) {
				try {
					cout << pointer->field("contained", derivation) << endl;
				} catch (exception& e) {
					cout << e.what() << endl;
				}
			} else if (parameter == "display") {
				auto root = base::Object::root();

				cout << "/" << endl;
				for (auto r : root)
					cout << tree(r);
			} else
				cout << "\ncheck ";
			if (pointer)
				cout << pointer->what() << " " << base::Location::who(*pointer);
		} catch (...) {
			cout << "error" << endl;
		}
		cout << "? ";
	}
	cout << "\ngame over" << endl;
#ifdef LOG
	clog.flush();
	clog.rdbuf(clog_stderr);
	log.close();
#endif
	base::end();
*/
	return 0;
}

