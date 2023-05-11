#ifndef _MORPH_COLLISIONS_H_
#define _MORPH_COLLISIONS_H_

#include <Morph/Error.h>

namespace morph {
	typedef unsigned ObjType;
	const ObjType T_UNDEFINED = 0;

	void setTypesCollide(ObjType type1, ObjType type2, bool symmetrical);

	enum CollType {
		C_RECT,
		C_CIRC,
		C_MULT
	};

	struct ErrCollCombinationUndefined : public Error {
		CollType c1, c2;

		ErrCollCombinationUndefined(CollType C1, CollType C2);
	};
}

#endif