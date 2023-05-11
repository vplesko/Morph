#ifndef _MORPH_ERROR_H_
#define _MORPH_ERROR_H_

#include <string>

namespace morph {
	struct Error {
		std::string desc;

		Error() {}
		explicit Error(const std::string &s) : desc(s) {}
	};
}

#endif