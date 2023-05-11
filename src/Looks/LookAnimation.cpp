#include <Morph/Looks/LookAnimation.h>
using namespace morph;

LookAnimation::LookAnimation() {
	currFrame = 0;
}

void LookAnimation::onTick() {
	if (currFrame < frames.size() - 1) ++currFrame;
}

void LookAnimation::removeFrame(unsigned index) {
	frames.erase(frames.begin() + index);
}

void LookAnimation::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	sf::Transform combinedTransform = states.transform * getTransform();
	states.transform = combinedTransform;

	if (currFrame < frames.size() && frames[currFrame] != 0)
		frames[currFrame]->draw(target, states);
}

LookAnimation::~LookAnimation() {
	for (unsigned i = 0; i < frames.size(); ++i)
		delete frames[i];
}