/*
 * ui.h
 *
 *  Created on: 04 mar 2018
 *      Author: MLaRosa
 */

#ifndef UI_H_
#define UI_H_

#include "base.h"
#include <map>

namespace base {
std::pair<bool, std::map<std::string, std::string>> dialog(std::string,
		std::map<std::string, std::pair<std::set<std::string>, std::string>> variables = { }, Log track = Log());

std::string display(const base::Object*);
std::string path(std::list<const base::Object*>);
std::string tree(const base::Object*, size_t nesting = 0);
} /* namespace base */

#endif /* UI_H_ */
