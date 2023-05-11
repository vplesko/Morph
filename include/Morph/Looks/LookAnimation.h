#ifndef _MORPH_ANIMATION_H_
#define _MORPH_ANIMATION_H_

#include <Morph/Look.h>
#include <vector>

namespace morph {
	class LookAnimation : public Look {
		//TODO: set direction, loop, speed

		unsigned currFrame;

		std::vector<Look*> frames;
		sf::Vector2f coords, origin;

	public:
		// Note: when using this class in an object you need to override onTick
		// TODO: fix that
		LookAnimation();

		Look* clone() const { return new LookAnimation(*this); }

		void onTick();

		void addFrame(Look *L, bool clone) { frames.push_back(clone ? L->clone() : L); }
		const Look* getFrame(unsigned index) const { return frames[index]; }
		void setFrame(unsigned index, Look *L) { frames[index] = L; }
		void removeFrame(unsigned index);

		void draw(sf::RenderTarget& target, sf::RenderStates states) const;

		~LookAnimation();
	};
}

#endif