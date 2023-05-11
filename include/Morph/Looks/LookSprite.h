#ifndef _MORPH_SPRITE_H_
#define _MORPH_SPRITE_H_

#include <Morph/Look.h>

namespace morph {
	class LookSprite : public Look {
	public:
		sf::Sprite sprite;

	public:
		LookSprite() {}
		explicit LookSprite(const sf::Texture &texture, bool resetRect=false);

		Look* clone() const { return new LookSprite(*this); }

		void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	};
}

#endif