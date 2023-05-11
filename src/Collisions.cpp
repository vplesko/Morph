#include <Morph/CollMask.h>
#include <Morph/Collisions.h>
#include <Morph/CollMasks/CollRect.h>
#include <Morph/CollMasks/CollCirc.h>
#include <Morph/CollMasks/CollMulti.h>
#include <vector>
#include <utility>
using namespace morph;

std::vector<std::pair<ObjType, ObjType>> typesColliding;

void setTypesCollide(ObjType type1, ObjType type2, bool symmetrical) {
	//TODO: what if they're already present?
	typesColliding.push_back(std::pair<ObjType, ObjType>(type1, type2));

	if (symmetrical && type1 != type2)
		typesColliding.push_back(std::pair<ObjType, ObjType>(type2, type1));
}

bool CollisionMask::canCollide(ObjType t1, ObjType t2) {
	//TODO: improvement to search
	for (unsigned i = 0; i < typesColliding.size(); ++i)
		if (typesColliding[i].first == t1 && typesColliding[i].second == t2)
			return true;

	return false;
}

float getDistSqr(const sf::Vector2f &v1, const sf::Vector2f &v2) {
	return (v1.x - v2.x) * (v1.x - v2.x) + (v1.y - v2.y) * (v1.y - v2.y);
}

float getDistSqr(const sf::Vector2f &v1, const float &v2x, const float &v2y) {
	return (v1.x - v2x) * (v1.x - v2x) + (v1.y - v2y) * (v1.y - v2y);
}

float getSqr(float x) {
	return x * x;
}

bool areCollidingOneIsMulti(const CollMulti *m1, const CollisionMask *m2) {
	for (unsigned i=0; i<m1->masks.size(); ++i)
		if (CollisionMask::areColliding(m1->masks[i], m2))
			return true;

	return false;
}

bool lineIntersectsCircleArea(const CollCirc *c, float x1, float y1, float x2, float y2) {
	//line is parallel to x or y axis

	if (c->containsPoint(sf::Vector2f(x1, y1)) || c->containsPoint(sf::Vector2f(x2, y2)))
		return true;

	if (x1 == x2) {
		if (x1 > c->getCenter().x + c->getRadius() || x1 < c->getCenter().x - c->getRadius())
			return false;

		return (y1 - c->getCenter().y) * (y2 - c->getCenter().y) < 0;
	} else {
		if (y1 > c->getCenter().y + c->getRadius() || y1 < c->getCenter().y - c->getRadius())
			return false;

		return (x1 - c->getCenter().x) * (x2 - c->getCenter().x) < 0;
	}
}

bool areCollidingCircRect(const CollCirc *c, const CollRect *r) {
	//rectangle sides are paralles to the x and y axis

	sf::Rect<float> rect = r->getRect();
	return rect.contains(c->getCenter()) || 
		lineIntersectsCircleArea(c, rect.left, rect.top, rect.left, rect.top + rect.height) || 
		lineIntersectsCircleArea(c, rect.left, rect.top + rect.height, rect.left + rect.width, rect.top + rect.height) || 
		lineIntersectsCircleArea(c, rect.left + rect.width, rect.top + rect.height, rect.left + rect.width, rect.top) || 
		lineIntersectsCircleArea(c, rect.left + rect.width, rect.top, rect.left, rect.top);
}

bool CollisionMask::areColliding(const CollisionMask *m1, const CollisionMask *m2) {
	if (m1 == 0 || m2 == 0)
		return false;

	if (m1->getType() == C_RECT && m2->getType() == C_RECT) {
		return ((CollRect*)m1)->getRect().intersects(((CollRect*)m2)->getRect());
	}

	if (m1->getType() == C_CIRC && m2->getType() == C_CIRC) {
		CollCirc *c1 = (CollCirc*)m1, *c2 = (CollCirc*)m2;
		return getDistSqr(c1->getCenter(), c2->getCenter()) < getSqr(c1->getRadius() + c2->getRadius());
	}

	if (m1->getType() == C_CIRC && m2->getType() == C_RECT) {
		return areCollidingCircRect((CollCirc*)m1, (CollRect*)m2);
	}
	if (m1->getType() == C_RECT && m2->getType() == C_CIRC) {
		return areCollidingCircRect((CollCirc*)m2, (CollRect*)m1);
	}

	if (m1->getType() == C_MULT && m2->getType() != C_MULT) {
		return areCollidingOneIsMulti((CollMulti*)m1, m2);
	}
	if (m1->getType() != C_MULT && m2->getType() == C_MULT) {
		return areCollidingOneIsMulti((CollMulti*)m2, m1);
	}

	if (m1->getType() == C_MULT && m2->getType() == C_MULT) {
		CollMulti *mult1 = (CollMulti*)m1, *mult2 = (CollMulti*)m2;

		for (unsigned i=0; i<mult1->masks.size(); ++i)
			for (unsigned j=0; j<mult2->masks.size(); ++j)
				if (areColliding(mult1->masks[i], mult2->masks[j]))
					return true;

		return false;
	}

	morph::ErrCollCombinationUndefined e(m1->getType(), m2->getType());
	throw e;
}

ErrCollCombinationUndefined::ErrCollCombinationUndefined(CollType C1, CollType C2) 
	: Error("Collision combination not defined!"), c1(C1), c2(C2) {
}