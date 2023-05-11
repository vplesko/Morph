#include <Morph/Object.h>
#include <Morph/CollMask.h>
#include <Morph/Look.h>
#include <Morph/Room.h>
#include <Morph/Flow.h>
using namespace morph;

Object::Object(int Depth) {
	setStartValues(Depth);
}

Object::Object(sf::Vector2f startPosition, int Depth) : position(startPosition) {
	setStartValues(Depth);
}

void Object::setStartValues(int Depth) {
	owner = 0;
	enabled = true;
	persistent = false;
	depth = Depth;
	rendered = true;
	l = 0;
	m = 0;
	checkCollisionsForThis = true;
	setReactToDefault();
	setOrderedChangesToFalse();
}

void Object::setReactToDefault() {
	reactTick = true;
	reactKeyPressed = true;
	reactKeyReleased = true;
	reactMouseButtonPressed = true;
	reactMouseButtonReleased = true;
	reactTextEntered = true;
	reactOther = true;
}

void Object::setReactsToFalse() {
	reactTick = false;
	reactKeyPressed = false;
	reactKeyReleased = false;
	reactMouseButtonPressed = false;
	reactMouseButtonReleased = false;
	reactTextEntered = false;
	reactOther = false;
}

void Object::setOrderedChangesToFalse() {
	enabledToChange = false;
	positionToChange = false;
	depthToChange = false;
	collMaskToChange = false;
	lookToChange = false;
	originToChange = false;
}

void Object::commitOrderedChangesToEnabled() {
	if (enabledToChange) {
		bool realChange = enabled != enabledNext;

		enabled = enabledNext;
		enabledToChange = false;

		if (owner != 0 && realChange) onEnabledChange();
	}
}

void Object::commitOrderedChangesToPosition() {
	if (positionToChange) {
		if (m) m->move(positionNext - position);
		if (l) l->move(positionNext - position);
		position = positionNext;

		positionToChange = false;
	}
}

void Object::commitOrderedChangesToOrigin() {
	if (originToChange) {
		if (m) m->setOrigin(m->getOrigin() + originNext - origin);
		if (l) l->setOrigin(l->getOrigin() + originNext - origin);
		origin = originNext;

		originToChange = false;
	}
}

void Object::commitOrderedChangesToDepth() {
	if (depthToChange) {
		depth = depthNext;
		if (owner) owner->replaceForDepth(indexInCont);

		depthToChange = false;
	}
}

void Object::commitOrderedChangesToCollMask() {
	if (collMaskToChange) {
		if (m) delete m;

		m = nextMask;
		if (m) {
			m->move(position);
			m->setOrigin(m->getOrigin() + origin);
		}

		collMaskToChange = false;
	}
}

void Object::commitOrderedChangesToLook() {
	if (lookToChange) {
		if (l) delete l;

		l = nextLook;
		if (l) {
			l->move(position);
			l->setOrigin(l->getOrigin() + origin);
		}

		lookToChange = false;
	}
}

void Object::commitOrderedChanges() {
	commitOrderedChangesToEnabled();
	commitOrderedChangesToPosition();
	commitOrderedChangesToOrigin();
	commitOrderedChangesToDepth();
	commitOrderedChangesToCollMask();
	commitOrderedChangesToLook();
}

void Object::changeRoom(Room *r) {
	owner->changeRoom(r);
}

bool Object::isColliding(const Object *o) const {
	if (CollisionMask::canCollide(getType(), o->getType()))
		return CollisionMask::areColliding(m, o->m);
	else
		return false;
}

bool Object::isColliding(const Object *o, morph::ObjType t, bool enabledOnly) {
	return o->owner->isColliding(o, t, enabledOnly);
}

bool Object::isColliding(const Object *o, std::vector<morph::ObjType> T, bool enabledOnly) {
	return o->owner->isColliding(o, T, enabledOnly);
}

bool Object::wouldCollide(const sf::Vector2f &offset, const Object *other, const sf::Vector2f &otherOffset) const {
	return owner->wouldCollide(this, offset, other, otherOffset);
}

bool Object::wouldCollide(const sf::Vector2f &offset, morph::ObjType t, bool enabledOnly) const {
	return owner->wouldCollide(this, offset, t, enabledOnly);
}

bool Object::wouldCollide(const sf::Vector2f &offset, std::vector<morph::ObjType> T, bool enabledOnly) const {
	return owner->wouldCollide(this, offset, T, enabledOnly);
}

unsigned Object::objCount(morph::ObjType t, bool enabledOnly) const {
	return owner->objCount(t, enabledOnly);
}

unsigned Object::objCount(std::vector<morph::ObjType> T, bool enabledOnly) const {
	return owner->objCount(T, enabledOnly);
}

void Object::setCollMask(CollisionMask *cm) {
	if (collMaskToChange) delete nextMask;
	collMaskToChange = true;
	nextMask = cm;

	if (owner == 0)
		commitOrderedChangesToCollMask();
}

void Object::setLook(Look *ol) {
	if (lookToChange) delete nextLook;
	lookToChange = true;
	nextLook = ol;

	if (owner == 0)
		commitOrderedChangesToLook();
}

bool Object::collMaskContainsPoint(const sf::Vector2f &p) const {
	return m ? m->containsPoint(p) : false;
}

bool Object::collMaskContainsPoint(const sf::Event::MouseButtonEvent &e, bool mapViewPixelToCoords) const {
	if (mapViewPixelToCoords) {
		return collMaskContainsPoint(owner->getFlow()->mapViewPixelToCoords(e));
	} else {
		return collMaskContainsPoint(sf::Vector2f((float)e.x, (float)e.y));
	}
}

void Object::setDepth(int Depth) {
	depthNext = Depth;
	depthToChange = true;

	if (owner == 0)
		commitOrderedChangesToDepth();
}

void Object::setEnabled(bool Enabled) {
	enabledNext = Enabled;
	enabledToChange = true;

	if (owner == 0)
		commitOrderedChangesToEnabled();
}

void Object::setEnabled(bool enabled, morph::ObjType t, bool affectObjsAboutToBePutIn) {
	owner->setEnabled(enabled, t, affectObjsAboutToBePutIn);
}

void Object::setEnabled(bool enabled, std::vector<morph::ObjType> T, bool affectObjsAboutToBePutIn) {
	owner->setEnabled(enabled, T, affectObjsAboutToBePutIn);
}

void Object::onTickMaskLookAndObj() {
	if (m) m->onTick();
	if (l) l->onTick();

	onTick();
}

void Object::setPosition(const sf::Vector2f &Coords) {
	positionToChange = true;
	positionNext = Coords;

	if (owner == 0)
		commitOrderedChangesToPosition();
}

void Object::move(const sf::Vector2f &offset) {
	if (!positionToChange) positionNext = position;

	positionToChange = true;
	positionNext += offset;

	if (owner == 0)
		commitOrderedChangesToPosition();
}

void Object::setOrigin(float x, float y) {
	originToChange = true;
	originNext = sf::Vector2f(x, y);

	if (owner == 0)
		commitOrderedChangesToOrigin();
}

void Object::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	preDraw(target, states);
	if (rendered && l) target.draw(*l, states);
	postDraw(target, states);
}

void Object::endGame() {
	owner->endGame();
}

Object::~Object() {
	if (getRoom())
		getRoom()->destroyObjNowForObjsDestructor(this);

	if (l) delete l;
	if (m) delete m;
}