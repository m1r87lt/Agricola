/*
 * Board.h
 *
 *  Created on: 2 gen 2019
 *      Author: m1r
 */

#ifndef BOARD_H_
#define BOARD_H_

#include "Components.h"

namespace agr {

void construct_board();

struct CardSpace: public base::Ensemble {
	virtual Fields shows() const;
	virtual std::string prints() const;
};

} /* namespace agr */

#endif /* BOARD_H_ */
