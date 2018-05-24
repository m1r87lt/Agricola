/*
 * state_machine.cpp
 *
 *  Created on: 03 apr 2018
 *      Author: MLaRosa
 */

#include "state_machine.h"
#include <algorithm>
#define SM_ACTION "action"
#define SM_BEG "beg"
#define SM_BRED "bred"
#define SM_BREEDING "breeding"
#define SM_COOKED "cooked"
#define SM_FEEDING "feeding"
#define SM_FREED "freed"
#define SM_HARVEST "harvest"
#define SM_HARVESTED "harvested"
#define SM_HARVEST_END "harvest end"
#define SM_HARVEST_START "harvest start"
#define SM_MEMBERLESS "memberless"
#define SM_NEW_ROUND_CARD "new Round card"
#define SM_PHASE_END "phase end"
#define SM_PHASE_START "phase start"
#define SM_ROUND_END "round end"
#define SM_ROUND_START "round start"
#define SM_REPLENISH "replenish"
#define SM_RETURN_HOME "return home"
#define SM_STAGE_END "stage end"
#define SM_STAGE_START "stage start"
#define SM_WORK "work"

unsigned STAGE = 1;
unsigned ROUND = 1;
bool HARVEST = false;
unsigned PHASE = 1;
unsigned STEP = 1;
std::list<std::string> TRIGGERS;
const std::set<unsigned> harvestTimes = { 0, 4, 7, 9, 11, 13, 14 };

bool new_stage() {
	return harvestTimes.find(ROUND - 1) == harvestTimes.end();
}
std::string status() {
	std::ostringstream result;
	base::Log track;

	std::clog << track.tracker() << "std::string status() {" << std::endl;
	result << "\nSTAGE=" << STAGE << "\nROUND=" << STAGE << "\nHARVEST="
			<< HARVEST << "\nPHASE=" << PHASE << "\nSTEP=" << STEP
			<< "\nTRIGGERS=" << base::Prompt::list(TRIGGERS) << "\nKEYS="
			<< base::Prompt::list(keys()) << std::endl;
	std::clog << track() << "}" << result.str().substr(1) << std::endl;

	return result.str();
}
std::string instant() {
	std::ostringstream result("{");
	if (HARVEST) {
		if (ROUND < 5)
			result << "H1";
		else if (ROUND < 8)
			result << "H2";
		else if (ROUND < 10)
			result << "H3";
		else if (ROUND < 12)
			result << "H4";
		else if (ROUND < 14)
			result << "H5";
		else
			result << "H6";
	} else
		result << STAGE << ";" << ROUND;
	result << ";" << PHASE << ";" << STEP << "}";

	return result.str();
}
unsigned stage() {
	std::clog << base::Log().tracker() << "unsigned stage()=" << STAGE
			<< std::endl;

	return STAGE;
}
unsigned round() {
	std::clog << base::Log().tracker() << "unsigned round()=" << ROUND
			<< std::endl;

	return ROUND;
}
bool harvest() {
	std::clog << base::Log().tracker() << "bool harvest()=" << HARVEST
			<< std::endl;

	return HARVEST;
}
unsigned phase() {
	std::clog << base::Log().tracker() << "unsigned phase()=" << PHASE
			<< std::endl;

	return PHASE;
}
unsigned step() {
	std::clog << base::Log().tracker() << "unsigned step()=" << STEP
			<< std::endl;

	return STEP;
}
std::list<std::string>& triggers() {
	std::clog << base::Log().tracker() << "unsigned triggers()="
			<< base::Prompt::list(TRIGGERS) << std::endl;

	return TRIGGERS;
}

void phase_start__1(base::Log track) {
	std::set<std::string> keying;
	std::set<std::string>::iterator k;
	std::set<std::string>::iterator K;
	std::ostringstream log;

	log << track.tracker() << "void phase_start__1()";
	std::clog << " {" << std::endl;
	if ((k = (keying = keys()).find(SM_PHASE_START)) == (K = keying.end())
			&& STEP == 1) {
		keying.emplace(SM_PHASE_START);
		std::clog << track() << instant() << " " << SM_PHASE_START << std::endl;
	} else if (STEP == 1 && k != K) {
		keying.erase(k);
		STEP = 3;
		std::clog << track() << "STEP=3" << std::endl;
	} else {
		std::string message =
				"= ERROR range error: this function is called with STEP != 1.";

		std::clog << track() << "}" << message << std::endl;
		std::cerr << log.str() << message << std::endl;
		throw std::range_error(log.str() + message);
	}
	keys() = keying;
	std::clog << track() << "}" << std::endl;
}
void phase_end__2(base::Log track) {
	std::set<std::string> keying;
	std::set<std::string>::iterator k;
	std::set<std::string>::iterator K;
	std::ostringstream log;

	log << track.tracker() << "void phase_end__2()";
	std::clog << " {" << std::endl;
	if ((k = (keying = keys()).find(SM_PHASE_END)) == (K = keying.end())
			&& STEP == 2) {
		keying.emplace(SM_PHASE_END);
		std::clog << track() << instant() << " " << SM_PHASE_END << std::endl;
	} else if (STEP == 2 && k != K) {
		keying.erase(k);
		++PHASE;
		STEP = 1;
		std::clog << track() << instant() << std::endl;
	} else {
		std::string message =
				"= ERROR range error: this function is called with STEP != 2.";

		std::clog << track() << "}" << message << std::endl;
		std::cerr << log.str() << message << std::endl;
		throw std::range_error(log.str() + message);
	}
	keys() = keying;
	std::clog << track() << "}" << std::endl;
}
void phase1_draw_a_new_round_card_1_3(base::Log track) {
	std::set<std::string> keying;
	std::set<std::string>::iterator k;
	std::set<std::string>::iterator K;
	std::ostringstream log;

	log << track.tracker() << "phase1_draw_a_new_round_card_1_3()";
	std::clog << " {" << std::endl;
	/* Turn over the top Round card and place it on
	 * the appropriate space on the board. The action
	 * on this card is available to all players, and
	 * can be used not only in this round but in all
	 * subsequent rounds. */
	if ((k = (keying = keys()).find(SM_NEW_ROUND_CARD)) == (K = keying.end()) && PHASE == 1 && STEP == 3 && !HARVEST) {
		auto roundDeck = deck(B_ROUND_CARDS);
		Card::Round* roundSpace = nullptr;
		Card::Round* roundFace = nullptr;
		std::ostringstream result;
		std::exception* exception = nullptr;

		if (roundDeck->size()) {
			auto card = roundDeck->draw(track);
			Card& roundCard = dynamic_cast<Card&>(*card);

			roundCard.facing(true, track);
			if ((roundFace = dynamic_cast<Card::Round*>(&roundCard()))) {
				auto rounds = roundFace->round();

				roundSpace = former_empty(rounds.first, rounds.second);
				if (roundSpace) {
					roundSpace->insert(0,
							std::string(B_ROUND) + " " + std::to_string(ROUND),
							std::unique_ptr<base::Object>(card.release()),
							track);
				} else {
					result << "= ERROR range error: no Round space in "
							<< roundFace->caption() << " is empty.";
					exception = new std::range_error(log.str() + result.str());
				}
			} else {
				result << "= ERROR domain error: the Card \""
						<< base::Location::who(roundCard) << "\" is not a "
						<< Card::Round::type << ".";
				exception = new std::domain_error(log.str() + result.str());
			}
		} else {
			result << "= ERROR domain error: the deck \"" << B_ROUND_CARDS
					<< "\" is empty.";
			exception = new std::domain_error(log.str() + result.str());
		}
		if (!exception) {
			std::clog << track() << "}" << result.str() << std::endl;
			std::cerr << log.str() << result.str() << std::endl;
			throw *exception;
		}
		keying.emplace(
				std::string(SM_NEW_ROUND_CARD) + " \"" + roundFace->name()
						+ "\"");
		std::clog << track() << instant() << " " << SM_NEW_ROUND_CARD << " \""
				<< roundFace->name() << "\"" << std::endl;
	} else if (PHASE == 1 && STEP == 3 && !HARVEST && k != K) {
		keying.erase(k);
		++STEP;
		std::clog << track() << "STEP=4" << std::endl;
	} else {
		std::string message =
				"= ERROR range error: this function is called with "
						"PHASE != 1 or STEP != 3 or HARVEST as true.";

		std::clog << track() << "}" << message << std::endl;
		std::cerr << log.str() << message << std::endl;
		throw std::range_error(log.str() + message);
	}
	keys() = keying;
	std::clog << track() << "}" << std::endl;
}
void phase1_start_the_round_1_4(base::Log track) {
	std::set<std::string> keying;
	std::set<std::string>::iterator k;
	std::set<std::string>::iterator K;
	std::ostringstream log;

	log << track.tracker() << "void phase1_start_the_round_1_4()";
	std::clog << log.str() << " {" << std::endl;
	/* All actions which occur at the beginning
	 * of a particular round or at the beginning
	 * of every round occur now.
	 * The text on some cards instructs players
	 * to place items on the Round card spaces.
	 * If there are tiles, Food and/or other playing
	 * pieces on the space for the current round,
	 * these are distributed to the appropriate
	 * players (who earned them by playing an
	 * Occupation or Improvement). */
	if ((k = (keying = keys()).find(SM_ROUND_START)) == (K = keying.end())
			&& PHASE == 1 && STEP == 4 && !HARVEST) {
		keying.emplace(SM_ROUND_START);
		std::clog << track() << instant() << " " << SM_ROUND_START << std::endl;
	} else if (PHASE == 1 && STEP == 4 && !HARVEST && k != K) {
		keying.erase(k);
		++STEP;
		std::clog << track() << "PHASE=1 is completed" << std::endl;
	} else {
		std::string message =
				"= ERROR range error: this function is called with "
						"PHASE != 1 or STEP != 4 or HARVEST as true.";

		std::clog << track() << "}" << message << std::endl;
		std::cerr << log.str() << message << std::endl;
		throw std::range_error(log.str() + message);
	}
	keys() = keying;
	std::clog << track() << "}" << std::endl;
}
void phase2_replenish_2_3(base::Log track) {
	std::set<std::string> keying;
	std::set<std::string>::iterator k;
	std::set<std::string>::iterator K;
	std::ostringstream log;

	log << track.tracker() << "void phase2_replenish_2_3()";
	std::clog << log.str() << " {" << std::endl;
	/* Place new goods and Food on any Action spaces on the
	 * board that require them (on the printed spaces as well
	 * as on Action and Round cards). These spaces are shown
	 * by an arrow (see Illustration). If goods or Food are
	 * already on a space, the new goods/Food are added to them. */
	// 3 Wood means that 3 Wood tokens are placed on that space
	// each round, 1 Cattle means that 1 Cattle token is placed
	// on the space each round, etc. The Fishing and Traveling
	// Players Action spaces receive 1 Food each round. These
	// goods and Food are taken from the general supply and can
	// build up over several rounds – there is no upper limit.
	if ((k = (keying = keys()).find(SM_REPLENISH)) == (K = keying.end())
			&& PHASE == 2 && STEP == 3 && !HARVEST) {
		keying.emplace(SM_REPLENISH);
		std::clog << track() << instant() << " " << SM_REPLENISH << std::endl;
	} else if (PHASE == 2 && STEP == 3 && !HARVEST && k != K) {
		collection(track);
		keying.erase(k);
		STEP = 2;
		std::clog << track() << "PHASE=2 is completed" << std::endl;
	} else {
		std::string message =
				"= ERROR range error: this function is called with "
						"PHASE != 2 or STEP != 3 or HARVEST as true.";

		std::clog << track() << "}" << message << std::endl;
		std::cerr << log.str() << message << std::endl;
		throw std::range_error(log.str() + message);
	}
	keys() = keying;
	std::clog << track() << "}" << std::endl;
}
void phase3_work_3_3(base::Log track) {
	std::set<std::string> keying;
	std::set<std::string>::iterator k;
	std::set<std::string>::iterator K;
	std::ostringstream log;

	log << track.tracker() << "void phase3_work_3_3()";
	std::clog << log.str() << " {" << std::endl;
	/* In clockwise order, starting with the Starting player,*/
	if ((k = (keying = keys()).find(SM_WORK)) == (K = keying.end()) && PHASE == 3 && STEP == 3 && !HARVEST) {
		auto fp = first_player();

		keying.emplace(SM_WORK);
		STEP = 3 + fp;
		std::clog << track() << instant() << " " << SM_WORK << "(" << fp << ")"
				<< std::endl;
	} else if (PHASE == 3 && STEP == 3 && !HARVEST && k != K) {
		keying.erase(k);
		STEP = 2;
		std::clog << track() << "PHASE=3 is completed" << std::endl;
	} else {
		std::string message =
				"= ERROR range error: this function is called with "
						"PHASE != 3 or STEP != 3 or HARVEST as true.";

		std::clog << track() << "}" << message << std::endl;
		std::cerr << log.str() << message << std::endl;
		throw std::range_error(log.str() + message);
	}
	keys() = keying;
	std::clog << track() << "}" << std::endl;
}
void phase3_work_3_(base::Log track) {
	std::set<std::string> keying;
	std::set<std::string>::iterator k;
	std::set<std::string>::iterator K;
	short unsigned player = STEP - 3;
	std::ostringstream log;

	log << track.tracker() << "void phase3_work_3_()";
	std::clog << log.str() << " {" << std::endl;
	/* players take turns taking a single Family member from their
	 * farmyard, placing it on an unoccupied Action space and taking
	 * that action. Play continues until all Family members have
	 * been placed. A player may only ever place one Family member
	 * at a time. */
	if ((k = (keying = keys()).find(SM_WORK)) != (K = keying.end())
			&& PHASE == 3 && STEP > 3 && !HARVEST) {
		if (player > Player::quantity())
			STEP = (player = 1) + 3;
		if ((k = find_key_containing(std::string(SM_ACTION) + " ")) == K) {
			auto member = waiting_member(player);

			if (member) {
				auto action = choose_an_action(player, track);

				if (!action) {
					std::string message =
							"= ERROR user cancel: the user does not choose a free action.";

					std::clog << track() << "}" << message << std::endl;
					std::cerr << log.str() << message << std::endl;
					throw std::runtime_error(log.str() + message);
				}
				action->insert(0, "worker",
						dynamic_cast<base::Location*>(member->where())->extract(
								*member, track), track);
				keying.erase(
						std::string(SM_MEMBERLESS) + " "
								+ std::to_string(player));
				keying.emplace(
						std::string(SM_ACTION) + " "
								+ base::Prompt::address(action));
				std::clog << track() << " player " << player
						<< ": choose the Action \"" << (std::string) *action
						<< "\"." << std::endl;
			} else if ((k = keying.find(
					std::string(SM_MEMBERLESS) + " " + std::to_string(player)))
					== K) {
				keying.emplace(
						std::string(SM_MEMBERLESS) + " "
								+ std::to_string(player));
				++STEP;
				std::clog << track() << " player " << player
						<< ": has no Family Members." << std::endl;
			} else {
				k = keying.begin();
				while (k != keying.end())
					if (!k->find(SM_MEMBERLESS))
						k = keying.erase(k);
					else
						++k;
				STEP = 3;
				std::clog << track()
						<< "there are no players with Family Members."
						<< std::endl;
			}
		} else {
			keying.erase(k);
			++STEP;
			std::clog << track() << "player " << player
					<< "'s turn is terminated." << std::endl;
		}
	} else {
		std::string message =
				"= ERROR range error: this function is called with "
						"PHASE != 3 or STEP < 4 or STEP > "
						+ std::to_string(Player::quantity() + 3)
						+ " or HARVEST as true or without KEY \""
						+ std::string(SM_WORK) + "\".";

		std::clog << track() << "}" << message << std::endl;
		std::cerr << log.str() << message << std::endl;
		throw std::range_error(log.str() + message);
	}
	keys() = keying;
	std::clog << track() << "}" << std::endl;
}
void phase4_return_home_4_3(base::Log track) {
	std::set<std::string> keying;
	std::set<std::string>::iterator k;
	std::set<std::string>::iterator K;
	std::ostringstream log;

	log << track.tracker() << "void phase4_return_home_4_3()";
	std::clog << log.str() << " {" << std::endl;
	/* Players remove their Family members from the game
	 * boards and return them to their home. */
	if ((k = (keying = keys()).find(SM_RETURN_HOME)) == (K = keying.end())
			&& PHASE == 4 && STEP == 3 && !HARVEST) {
		keying.emplace(SM_RETURN_HOME);
		STEP = 3 + first_player();
		std::clog << track() << instant() << " " << SM_RETURN_HOME << std::endl;
	} else if (PHASE == 4 && STEP == 3 && !HARVEST && k != K) {
		keying.erase(k);
		STEP = 2;
		std::clog << track() << "PHASE=4 is completed" << std::endl;
	} else {
		std::string message =
				"= ERROR range error: this function is called with "
						"PHASE != 4 or STEP != 3 or HARVEST as true.";

		std::clog << track() << "}" << message << std::endl;
		std::cerr << log.str() << message << std::endl;
		throw std::range_error(log.str() + message);
	}
	keys() = keying;
	std::clog << track() << "}" << std::endl;
}
void phase4_return_home_4_(base::Log track) {
	std::set<std::string> keying;
	std::set<std::string>::iterator k;
	std::set<std::string>::iterator K;
	std::ostringstream log;

	log << track.tracker() << "void phase4_return_home_4_()";
	std::clog << log.str() << " {" << std::endl;
	/* Players remove their Family members from the game
	 * boards and return them to their home. */
	if ((k = (keying = keys()).find(SM_RETURN_HOME)) != (K = keying.end())
			&& PHASE == 4 && STEP > 3 && !HARVEST) {
		if ((k = find_key_containing(std::string(SM_RETURN_HOME) + " ")) != K)
			if (++STEP - 3 > Player::quantity())
				STEP = 4;
		short unsigned player = STEP - 3;
		auto key = std::string(SM_RETURN_HOME) + " " + std::to_string(player);
		auto worker_member = worker(player, track);

		if (worker_member) {
			bedroom(player).insert(0, Wooden::familyMemberType,
					dynamic_cast<base::Location*>(worker_member->where())->extract(
							*worker_member, track), track);
			keying.erase(k);
			keying.emplace(key);
			std::clog << track() << "player" << player << "'s worker{"
					<< worker_member << "} returned home." << std::endl;
		} else if (*k == key) {
			++STEP;
			std::clog << track() << "no player" << player
					<< "'s worker is waiting to return home." << std::endl;
		} else {
			keying.erase(k);
			STEP = 3;
			std::clog << track() << "no worker is waiting to return home yet."
					<< std::endl;
		}
	} else {
		std::string message =
				"= ERROR range error: this function is called with "
						"PHASE != 4 or STEP < 4 OR step > "
						+ std::to_string(Player::quantity() + 3)
						+ " or HARVEST as true or without KEY \""
						+ std::string(SM_RETURN_HOME) + "\".";

		std::clog << track() << "}" << message << std::endl;
		std::cerr << log.str() << message << std::endl;
		throw std::range_error(log.str() + message);
	}
	keys() = keying;
	std::clog << track() << "}" << std::endl;
}
void harvest_phase1_field_1_3(base::Log track) {
	std::set<std::string> keying;
	std::set<std::string>::iterator k;
	std::set<std::string>::iterator K;
	std::ostringstream log;

	log << track.tracker() << "void harvest_phase1_field_1_3()";
	std::clog << log.str() << " {" << std::endl;
	if ((k = (keying = keys()).find(SM_HARVEST)) == (K = keying.end())
			&& PHASE == 1 && STEP == 3 && HARVEST) {
		auto fp = first_player();

		keying.emplace(std::string(SM_HARVEST) + " " + std::to_string(fp));
		STEP = 3 + fp;
		std::clog << track() << instant() << " " << SM_HARVEST << " " << fp
				<< std::endl;
	} else if (PHASE == 1 && STEP == 3 && HARVEST && k != K) {
		keying.erase(k);
		STEP = 2;
		std::clog << track() << "PHASE=1 is completed." << std::endl;
	} else {
		std::string message =
				"= ERROR range error: this function is called with "
						"PHASE != 1 or STEP != 3 or not HARVEST as false.";

		std::clog << track() << "}" << message << std::endl;
		std::cerr << log.str() << message << std::endl;
		throw std::range_error(log.str() + message);
	}
	keys() = keying;
	std::clog << track() << "}" << std::endl;
}
void harvest_phase1_field_1_(base::Log track) {
	std::set<std::string> keying;
	std::set<std::string>::iterator k;
	std::set<std::string>::iterator K;
	short unsigned player = STEP - 3;
	std::ostringstream log;

	log << track.tracker() << "void harvest_phase1_field_1_()";
	std::clog << log.str() << " {" << std::endl;
	/* Players remove 1 Grain or Vegetable token from each Sown field
	 * in their farmyard and place them in their personal supply. */
	// Players may also receive additional Food from Occupation or
	// Improvement cards that they have played.
	if (player > Player::quantity())
		STEP = (player = 1) + 3;
	if ((k = (keying = keys()).find(SM_HARVEST)) != (K = keying.end())
			&& PHASE == 1 && STEP > 3 && HARVEST) {
		if ((k = find_key_containing(SM_HARVESTED)) == K) {
			auto harvested = harvest(player, track);

			k = find_key_containing(std::string(SM_HARVEST) + " ");
			if (harvested.quantity) {
				keying.erase(k);
				keying.emplace(
						std::string(SM_HARVEST) + " " + std::to_string(player));
				keying.emplace(
						std::string(SM_HARVESTED) + " "
								+ (std::string) harvested);
				std::clog << track() << " player " << player << " harvests: "
						<< (std::string) harvested << "." << std::endl;
			} else if (*k
					== std::string(SM_HARVEST) + " " + std::to_string(player)) {
				STEP = 3;
				std::clog << track() << "The Field phase is terminated."
						<< std::endl;
			} else
				++STEP;
		} else {
			keying.erase(k);
			++STEP;
		}
	} else {
		std::string message =
				"= ERROR range error: this function is called with "
						"PHASE != 1 or STEP < 4 or STEP > "
						+ std::to_string(Player::quantity() + 3)
						+ " or HARVEST as false or without KEY \""
						+ std::string(SM_HARVEST) + "\".";

		std::clog << track() << "}" << message << std::endl;
		std::cerr << log.str() << message << std::endl;
		throw std::range_error(log.str() + message);
	}
	keys() = keying;
	std::clog << track() << "}" << std::endl;
}
void harvest_phase2_feeding_the_family_2_3(base::Log track) {
	std::set<std::string> keying;
	std::set<std::string>::iterator k;
	std::set<std::string>::iterator K;
	std::ostringstream log;

	log << track.tracker() << "void harvest_phase2_feeding_the_family_2_3()";
	std::clog << log.str() << " {" << std::endl;
	if ((k = (keying = keys()).find(SM_FEEDING)) == (K = keying.end())
			&& PHASE == 2 && STEP == 3 && HARVEST) {
		auto fp = first_player();

		STEP = 3 + fp;
		std::clog << track() << instant() << " " << SM_FEEDING << " " << fp
				<< std::endl;
	} else if (PHASE == 2 && STEP == 3 && HARVEST && k != K) {
		keying.erase(k);
		STEP = 2;
		std::clog << track() << "PHASE=2 is completed." << std::endl;
	} else {
		std::string message =
				"= ERROR range error: this function is called with "
						"PHASE != 2 or STEP != 3 or HARVEST as false.";

		std::clog << track() << "}" << message << std::endl;
		std::cerr << log.str() << message << std::endl;
		throw std::range_error(log.str() + message);
	}
	keys() = keying;
	std::clog << track() << "}" << std::endl;
}
void harvest_phase2_feeding_the_family_2_(base::Log track) {
	std::set<std::string> keying;
	std::set<std::string>::iterator k;
	std::set<std::string>::iterator K;
	short unsigned player = STEP - 3;
	std::ostringstream log;

	log << track.tracker() << "void harvest_phase2_feeding_the_family_2_()";
	std::clog << log.str() << " {" << std::endl;
	/* Each unprocessed Grain or Vegetable may be converted to 1 Food at any time.
	 * Fireplaces and Cooking Hearths, as well as other specific Occupations and
	 * Improvements, allow players to convert Vegetables at any time, at a better
	 * exchange rate. Improvements with the (K) symbol can be used to convert animals
	 * to Food at any time. Improvements with the (O) symbol can be used to Bake
	 * bread, but only when the player takes a Bake bread action during a round.
	 * Unprocessed animals have no Food value. At the end of this phase, each player
	 * must feed his or her family by paying 2 Food per Family member. Offspring that
	 * were born during the current round only consume 1 Food for this round, but
	 * will require 2 Food in future Harvests. */
	if (player > Player::quantity())
		STEP = (player = 1) + 3;
	if ((k = (keying = keys()).find(SM_FEEDING)) != (K = keying.end())
			&& PHASE == 2 && STEP > 3 && HARVEST) {
		auto feeding = std::string(SM_FEEDING) + " " + std::to_string(player);

		if ((k = find_key_containing(std::string(SM_FEEDING) + " ")) == K) {
			keying.emplace(feeding);
			keying.emplace(SM_COOKED);
		} else if (keying.find(SM_COOKED) == K) {
			if ((k = find_key_containing(std::string(SM_BEG) + " ")) == K) {
				auto beg = feed(player, track);

				keying.emplace(SM_BEG + std::to_string(beg));
				std::clog << track() << " player " << player << " begs for "
						<< beg << " Food." << std::endl;
			} else {
				keying.erase(k);
				if ((k = keying.find(feeding)) == K)
					++STEP;
				else {
					keying.erase(feeding);
					STEP = 3;
				}
			}
		} else {
			if ((k = find_key_containing(std::string(SM_COOKED) + " ")) == K) {
				auto cooked = cook(player, track);

				if (cooked.size() && cooked.front().quantity) {
					auto c = cooked.begin();

					feeding = " " + (std::string) *c++ + "<-{";
					for (auto C = cooked.end(); c != C; ++c)
						feeding += (std::string) *c + ";";
					feeding.back() = '}';
				} else
					feeding = " player " + std::to_string(player);
				keying.emplace(SM_COOKED + feeding);
				std::clog << track() << " player " << player << " cooks: "
						<< (std::string) cooked.front() << "." << std::endl;
			} else {
				keying.erase(k);
				++STEP;
				if ((k = keying.find(feeding)) != K)
					keying.erase(SM_COOKED);
			}
		}
	} else {
		std::string message =
				"= ERROR range error: this function is called with "
						"PHASE != 3 or STEP < 4 or STEP > "
						+ std::to_string(Player::quantity() + 3)
						+ " or HARVEST as false or without KEY \""
						+ std::string(SM_FEEDING) + "\".";

		std::clog << track() << "}" << message << std::endl;
		std::cerr << log.str() << message << std::endl;
		throw std::range_error(log.str() + message);
	}
	keys() = keying;
	std::clog << track() << "}" << std::endl;
}
void harvest_phase3_breeding_3_3(base::Log track) {
	std::set<std::string> keying;
	std::set<std::string>::iterator k;
	std::set<std::string>::iterator K;
	std::ostringstream log;

	log << track.tracker() << "void harvest_phase3_breeding_3_3()";
	std::clog << log.str() << " {" << std::endl;
	if ((k = (keying = keys()).find(SM_BREEDING)) == (K = keying.end())
			&& PHASE == 3 && STEP == 3 && HARVEST) {
		auto fp = first_player();

		keying.emplace(std::string(SM_BREEDING) + " " + std::to_string(fp));
		STEP = 3 + fp;
		std::clog << track() << instant() << " " << SM_BREEDING << " " << fp
				<< std::endl;
	} else if (PHASE == 3 && STEP == 3 && HARVEST && k != K) {
		keying.erase(k);
		STEP = 2;
		std::clog << track() << "PHASE=3 is completed." << std::endl;
	} else {
		std::string message =
				"= ERROR range error: this function is called with "
						"PHASE != 3 or STEP != 3 or not HARVEST as false.";

		std::clog << track() << "}" << message << std::endl;
		std::cerr << log.str() << message << std::endl;
		throw std::range_error(log.str() + message);
	}
	keys() = keying;
	std::clog << track() << "}" << std::endl;
}
//TODO adjust the next function
void harvest_phase3_breeding_3_(base::Log track) {
	std::set<std::string> keying;
	std::set<std::string>::iterator k;
	std::set<std::string>::iterator K;
	short unsigned player = STEP - 3;
	std::ostringstream log;

	log << track.tracker() << "void harvest_phase3_breeding_3_()";
	std::clog << log.str() << " {" << std::endl;
	/* Lastly, any player with at least 2 animals of the same type
	 * receives exactly one additional (baby) animal of that type –
	 * but only if the lamb, the shoat or the calf can be
	 * accommodated in the farmyard . Baby animals and parent
	 * animals may not be converted into Food immediately after the
	 * birth; they simply run away if they cannot be accommodated.
	 * The animals breed regardless of where the parent animals are
	 * placed – the parents may be in separate areas. */
	if (player > Player::quantity())
		STEP = (player = 1) + 3;
	if ((k = (keying = keys()).find(SM_BREEDING)) != (K = keying.end()) && PHASE == 3 && STEP > 3 && HARVEST && k != K) {
		auto freed = find_key_containing(SM_FREED);
		auto bred = find_key_containing(SM_BRED);

		if (freed == bred) {
			auto breeding = breed(player, track);

			k = find_key_containing(std::string(SM_BREEDING) + " ");
			if (breeding.size()) {
				std::string freed_animals;
				std::string bred_animals;

				for (auto b : breeding) {
					if (b.note == "bred")
						bred_animals += (std::string) b + ";";
					if (b.note == "freed")
						freed_animals += (std::string) b + ";";
				}
				freed_animals.pop_back();
				if (k != K)
					keying.erase(k);
				std::clog << track() << " player " << player << " get: ";
				if (bred_animals.length()) {
					bred_animals.pop_back();
					keying.emplace(std::string(SM_BRED) + " " + bred_animals);
					std::clog << bred_animals << std::endl;
				}
				std::clog << "; ";
				if (freed_animals.length()) {
					freed_animals.pop_back();
					keying.emplace(std::string(SM_FREED) + " " + freed_animals);
					std::clog << freed_animals << std::endl;
				}
				keying.emplace(
						std::string(SM_BREEDING) + " "
								+ std::to_string(player));
				std::clog << "." << std::endl;
			} else if (*k
					== std::string(SM_BREEDING) + " "
							+ std::to_string(player)) {
				STEP = 3;
				std::clog << track() << "The Breeding phase is terminated."
						<< std::endl;
			} else
				++STEP;
		} else {
			keying.erase(k);
			++STEP;
		}
	} else {
		std::string message =
				"= ERROR range error: this function is called with "
						"PHASE != 3 or STEP < 4 or STEP > "
						+ std::to_string(Player::quantity() + 3)
						+ " or HARVEST as false or without KEY \""
						+ std::string(SM_BREEDING) + "\".";

		std::clog << track() << "}" << message << std::endl;
		std::cerr << log.str() << message << std::endl;
		throw std::range_error(log.str() + message);
	}
	keys() = keying;
	std::clog << track() << "}" << std::endl;
}
void round_start(base::Log track) {
	std::set<std::string> keying;
	std::set<std::string>::iterator k;
	std::set<std::string>::iterator K;
	std::ostringstream log;

	log << track.tracker() << "void round_start()";
	std::clog << " {" << std::endl;
	if ((k = (keying = keys()).find(SM_ROUND_START)) == (K = keying.end())
			&& !HARVEST && PHASE == 1 && STEP == 1) {
		keying.emplace(SM_ROUND_START);
		phase_start__1(track);
		std::clog << track() << instant() << " " << SM_ROUND_START << std::endl;
	} else if (!HARVEST && PHASE == 1 && STEP == 1 && k != K) {
		keying.erase(k);
		phase_start__1(track);
		std::clog << track() << "the round is started." << std::endl;
	} else {
		std::string message =
				"= ERROR range error: this function is called with "
						"PHASE != 1 or STEP != 1 or HARVEST as true.";

		std::clog << track() << "}" << message << std::endl;
		std::cerr << log.str() << message << std::endl;
		throw std::range_error(log.str() + message);
	}
	keys() = keying;
	std::clog << track() << "}" << std::endl;
}
void round_end(base::Log track) {
	std::set<std::string> keying;
	std::set<std::string>::iterator k;
	std::set<std::string>::iterator K;
	std::ostringstream log;

	log << track.tracker() << "void round_end()";
	std::clog << " {" << std::endl;
	if ((k = (keying = keys()).find(SM_ROUND_END)) == (K = keying.end())
			&& !HARVEST && PHASE == 4 && STEP == 2) {
		keying.emplace(SM_ROUND_END);
		phase_end__2(track);
		std::clog << track() << instant() << " " << SM_ROUND_END << std::endl;
	} else if (!HARVEST && PHASE == 4 && STEP == 2 && k != K) {
		keying.erase(k);
		phase_end__2(track);
		if (!(HARVEST = harvestTimes.find(ROUND) != harvestTimes.end()))
			++ROUND;
		std::clog << track() << "the round is ended." << std::endl;
	} else {
		std::string message = "= ERROR range error: this function is called "
				"with PHASE != 4 or STEP != 2 or HARVEST as true.";

		std::clog << track() << "}" << message << std::endl;
		std::cerr << log.str() << message << std::endl;
		throw std::range_error(log.str() + message);
	}
	keys() = keying;
	std::clog << track() << "}" << std::endl;
}
void harvest_start(base::Log track) {
	std::set<std::string> keying;
	std::set<std::string>::iterator k;
	std::set<std::string>::iterator K;
	std::ostringstream log;

	log << track.tracker() << "void harvest_start()";
	std::clog << " {" << std::endl;
	k = (keying = keys()).find(SM_HARVEST_START);
	K = keying.end();
	if (HARVEST && PHASE == 1 && STEP == 1
			&& harvestTimes.find(ROUND) != harvestTimes.end() && k == K) {
		keying.emplace(SM_HARVEST_START);
		phase_start__1(track);
		std::clog << track() << instant() << " " << SM_HARVEST_START
				<< std::endl;
	} else if (HARVEST && PHASE == 1 && STEP == 1
			&& harvestTimes.find(ROUND) != harvestTimes.end() && k != K) {
		keying.erase(k);
		phase_start__1(track);
		std::clog << track() << "the harvest time is started." << std::endl;
	} else {
		std::string message =
				"= ERROR range error: this function is called with PHASE != 1 "
						"or STEP != 1 or HARVEST as false or after an invalid round.";

		std::clog << track() << "}" << message << std::endl;
		std::cerr << log.str() << message << std::endl;
		throw std::range_error(log.str() + message);
	}
	keys() = keying;
	std::clog << track() << "}" << std::endl;
}
void harvest_end(base::Log track) {
	std::set<std::string> keying;
	std::set<std::string>::iterator k;
	std::set<std::string>::iterator K;
	std::ostringstream log;

	log << track.tracker() << "void harvest_end()";
	std::clog << " {" << std::endl;
	if ((k = (keying = keys()).find(SM_HARVEST_END)) == (K = keying.end())
			&& HARVEST && PHASE == 3 && STEP == 2
			&& harvestTimes.find(ROUND) != harvestTimes.end()) {
		keying.emplace(SM_HARVEST_END);
		phase_end__2(track);
		std::clog << track() << instant() << " " << SM_ROUND_START << std::endl;
	} else if (HARVEST && PHASE == 3 && STEP == 2
			&& harvestTimes.find(ROUND) != harvestTimes.end() && k != K) {
		keying.erase(k);
		phase_end__2(track);
		++ROUND;
		std::clog << track() << "the harvest time is ended." << std::endl;
	} else {
		std::string message =
				"= ERROR range error: this function is called with PHASE != 3 "
						"or STEP != 2 or HARVEST as false or after an invalid round.";

		std::clog << track() << "}" << message << std::endl;
		std::cerr << log.str() << message << std::endl;
		throw std::range_error(log.str() + message);
	}
	keys() = keying;
	std::clog << track() << "}" << std::endl;
}
void stage_start(base::Log track) {
	std::set<std::string> keying;
	std::set<std::string>::iterator k;
	std::set<std::string>::iterator K;
	std::ostringstream log;

	log << track.tracker() << "void stage_start()";
	std::clog << " {" << std::endl;
	if ((k = (keying = keys()).find(SM_STAGE_START)) == (K = keying.end())
			&& !HARVEST && PHASE == 1 && STEP == 1
			&& harvestTimes.find(ROUND - 1) != harvestTimes.end()) {
		keying.emplace(SM_STAGE_START);
		round_start(track);
		std::clog << track() << instant() << " " << SM_STAGE_START << std::endl;
	} else if (HARVEST && PHASE == 1 && STEP == 1
			&& harvestTimes.find(ROUND - 1) != harvestTimes.end() && k != K) {
		keying.erase(k);
		round_start(track);
		std::clog << track() << "a new stage is started." << std::endl;
	} else {
		std::string message =
				"= ERROR range error: this function is called with PHASE != 1 "
						"or STEP != 1 or HARVEST as false or after an invalid round.";

		std::clog << track() << "}" << message << std::endl;
		std::cerr << log.str() << message << std::endl;
		throw std::range_error(log.str() + message);
	}
	keys() = keying;
	std::clog << track() << "}" << std::endl;
}
void stage_end(base::Log track) {
	std::set<std::string> keying;
	std::set<std::string>::iterator k;
	std::set<std::string>::iterator K;
	std::ostringstream log;

	log << track.tracker() << "void stage_end()";
	std::clog << " {" << std::endl;
	if ((k = (keying = keys()).find(SM_STAGE_END)) == (K = keying.end())
			&& HARVEST && PHASE == 3 && STEP == 2
			&& harvestTimes.find(ROUND) != harvestTimes.end()) {
		keying.emplace(SM_STAGE_END);
		harvest_end(track);
		std::clog << track() << instant() << " " << SM_STAGE_END << std::endl;
	} else if (!(HARVEST = !(HARVEST && PHASE == 3 && STEP == 2
			&& harvestTimes.find(ROUND) != harvestTimes.end() && k != K))) {
		keying.erase(k);
		harvest_end(track);
		++STAGE;
		std::clog << track() << "the stage is ended." << std::endl;
	} else {
		std::string message =
				"= ERROR range error: this function is called with PHASE != 3 "
						"or STEP != 2 or HARVEST as false or after an invalid round.";

		std::clog << track() << "}" << message << std::endl;
		std::cerr << log.str() << message << std::endl;
		throw std::range_error(log.str() + message);
	}
	keys() = keying;
	std::clog << track() << "}" << std::endl;
}

