#include <Morph/Looks/LookMulti.h>
using namespace morph;

LookMulti::LookMulti(Look *l, bool clone) {
	looks.push_back(clone ? l->clone() : l);
}

LookMulti::LookMulti(std::vector<Look*> L, bool clone) : looks(L) {
	if (clone) {
		for (unsigned i = 0; i < looks.size(); ++i)
			looks[i] = looks[i]->clone();
	}
}

void LookMulti::add(Look *l, bool clone) {
	looks.push_back(clone ? l->clone() : l);
}

bool LookMulti::popLast() {
	if (looks.empty()) return false;

	delete looks[looks.size() - 1];
	looks.pop_back();
	return true;
}

Look* LookMulti::clone() const {
	LookMulti *lm = new LookMulti();

	lm->setOrigin(getOrigin());
	lm->setPosition(getPosition());
	lm->setRotation(getRotation());
	lm->setScale(getScale());

	for (unsigned i = 0; i < looks.size(); ++i)
		lm->add(looks[i], true);

	return lm;
}

void LookMulti::onTick() {
	for (unsigned i = 0; i < looks.size(); ++i)
		looks[i]->onTick();
}

void LookMulti::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	sf::Transform combinedTransform = states.transform * getTransform();
	states.transform = combinedTransform;

	for (unsigned i = 0; i < looks.size(); ++i)
		looks[i]->draw(target, states);
}

LookMulti::~LookMulti() {
	for (unsigned i = 0; i < looks.size(); ++i)
		delete looks[i];
}