#ifndef _MORPH_LOOK_MULTI_H_
#define _MORPH_LOOK_MULTI_H_

#include <Morph/Look.h>

namespace morph {
	class LookMulti : public Look {
		LookMulti(const LookMulti&);
		void operator=(const LookMulti&);

	public:
		std::vector<Look*> looks;

		LookMulti() {}
		explicit LookMulti(Look *l, bool clone);
		explicit LookMulti(std::vector<Look*> L, bool clone);

		void add(Look *l, bool clone);
		bool popLast();

		Look* clone() const;

		void onTick();
		
		void draw(sf::RenderTarget& target, sf::RenderStates states) const;

		~LookMulti();
	};
}

#endif