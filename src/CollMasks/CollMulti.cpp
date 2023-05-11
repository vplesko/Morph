#include <Morph/CollMasks/CollMulti.h>
using namespace morph;

void CollMulti::justMove(const sf::Vector2f &offset) {
	for (unsigned i=0; i<masks.size(); ++i)
		masks[i]->move(offset);
}

void CollMulti::setOrigin(float x, float y) {
	sf::Vector2f offset(x - origin.x, y - origin.y);

	origin += offset;

	justMove(-offset);
	coords += -offset;
}

CollMulti::CollMulti(CollisionMask *m, bool clone) {
	masks.push_back(clone ? m->clone() : m);
}

CollMulti::CollMulti(std::vector<CollisionMask*> M, bool clone) : masks(M) {
	if (clone) {
		for (unsigned i = 0; i < masks.size(); ++i)
			masks[i] = masks[i]->clone();
	}
}

void CollMulti::add(CollisionMask *m, bool clone) {
	masks.push_back(clone ? m->clone() : m);
}

CollisionMask* CollMulti::clone() const {
	CollMulti *cm = new CollMulti();

	cm->coords = coords;
	cm->origin = origin;

	for (unsigned i=0; i<masks.size(); ++i)
		cm->add(masks[i], true);

	return cm;
}

bool CollMulti::containsPoint(const sf::Vector2f &p) const {
	for (unsigned i=0; i<masks.size(); ++i)
		if (masks[i]->containsPoint(p))
			return true;

	return false;
}

void CollMulti::onTick() {
	for (unsigned i=0; i<masks.size(); ++i)
		masks[i]->onTick();
}

CollMulti::~CollMulti() {
	for (unsigned i=0; i<masks.size(); ++i)
		delete masks[i];
}