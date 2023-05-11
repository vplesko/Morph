#include <Morph/Room.h>
#include <Morph/Object.h>
#include <Morph/Flow.h>
#include <Morph/CollMask.h>
#include <algorithm>
using namespace morph;

Room::Room() {
	owner = 0;
}

void Room::changeRoom(Room *r) {
	owner->changeRoom(r);
}

unsigned Room::objCount(bool enabledOnly) const {
	if (enabledOnly == false)
		return objs.size();

	unsigned cnt = 0;
	for (unsigned i = 0; i < objs.size(); ++i) {
		if (/*enabledOnly && */!objs[i]->isEnabled())
			continue;

		++cnt;
	}

	return cnt;
}

unsigned Room::objCount(morph::ObjType t, bool enabledOnly) const {
	unsigned cnt = 0;
	for (unsigned i = 0; i < objs.size(); ++i) {
		if (enabledOnly && !objs[i]->isEnabled())
			continue;

		if (objs[i]->getType() == t)
			++cnt;
	}

	return cnt;
}

unsigned Room::objCount(std::vector<morph::ObjType> T, bool enabledOnly) const {
	unsigned cnt = 0;
	for (unsigned i = 0; i < objs.size(); ++i) {
		if (enabledOnly && !objs[i]->isEnabled())
			continue;

		unsigned j;
		for (j = 0; j < T.size(); ++j)
			if (objs[i]->getType() == T[j])
				break;
		if (j == T.size()) continue;

		++cnt;
	}

	return cnt;
}

std::vector<Object*> Room::getObjects(bool enabledOnly) {
	std::vector<Object*> toRet;

	for (unsigned i = 0; i < objs.size(); ++i) {
		if (enabledOnly && !objs[i]->isEnabled()) continue;

		toRet.push_back(objs[i]);
	}

	return toRet;
}

std::vector<Object*> Room::getObjects(morph::ObjType t, bool enabledOnly) {
	std::vector<Object*> toRet;

	for (unsigned i = 0; i < objs.size(); ++i) {
		if (enabledOnly && !objs[i]->isEnabled()) continue;

		if (objs[i]->getType() == t)
			toRet.push_back(objs[i]);
	}

	return toRet;
}

std::vector<Object*> Room::getObjects(std::vector<morph::ObjType> T, bool enabledOnly) {
	std::vector<Object*> toRet;

	for (unsigned i = 0; i < objs.size(); ++i) {
		if (enabledOnly && !objs[i]->isEnabled())
			continue;

		unsigned j;
		for (j = 0; j < T.size(); ++j)
			if (objs[i]->getType() == T[j])
				break;
		if (j == T.size())
			continue;

		toRet.push_back(objs[i]);
	}

	return toRet;
}

void Room::moveObjs(const sf::Vector2f &offset, bool enabledOnly) {
	for (unsigned i = 0; i < objs.size(); ++i) {
		if (enabledOnly && !objs[i]->isEnabled())
			continue;

		objs[i]->move(offset);
	}
}

void Room::moveObjs(const sf::Vector2f &offset, morph::ObjType t, bool enabledOnly) {
	for (unsigned i = 0; i < objs.size(); ++i) {
		if (enabledOnly && !objs[i]->isEnabled())
			continue;

		if (objs[i]->getType() == t)
			objs[i]->move(offset);
	}
}

void Room::moveObjs(const sf::Vector2f &offset, const std::vector<morph::ObjType> &T, bool enabledOnly) {
	for (unsigned i = 0; i < objs.size(); ++i) {
		if (enabledOnly && !objs[i]->isEnabled())
			continue;

		unsigned j;
		for (j = 0; j < T.size(); ++j)
			if (objs[i]->getType() == T[j])
				break;
		if (j == T.size())
			continue;

		objs[i]->move(offset);
	}
}

void Room::checkCollisions() {
	for (unsigned i = 0; i + 1 < objs.size(); ++i) {
		if (objs[i]->getCollMask() == 0 || 
			!objs[i]->isEnabled() || 
			!objs[i]->getCheckCollisionsForThis())
			continue;

		for (unsigned j = i + 1; j < objs.size(); ++j) {
			if (objs[j]->getCollMask() == 0 || 
				!objs[j]->isEnabled() || 
				!objs[i]->getCheckCollisionsForThis())
				continue;

			if (objs[i]->isColliding(objs[j])) {
				objs[i]->onCollision(objs[j]);
				objs[j]->onCollision(objs[i]);
			}
		}
	}
}

void Room::addInPlaceForDepth(std::vector<Object*> &vect, Object *o, bool updateIndexInCont) {
	//TODO: faster search

	unsigned i;
	for (i = 0; i < vect.size(); ++i)
		if (vect[i]->getDepth() >= o->getDepth())
			break;

	if (i == vect.size()) {
		vect.push_back(o);
		if (updateIndexInCont) o->indexInCont = vect.size() - 1;
	} else {
		Object *prev = o;
		for (unsigned j = i; j < vect.size(); ++j) {
			Object *temp = vect[j];
			vect[j] = prev;
			prev = temp;

			if (updateIndexInCont) vect[j]->indexInCont = j;
		}

		vect.push_back(prev);
		if (updateIndexInCont) prev->indexInCont = vect.size() - 1;
	}
}

void Room::remove(unsigned indexInCont) {
	for (unsigned i = indexInCont; i < objs.size() - 1; ++i) {
		objs[i] = objs[i + 1];
		objs[i]->indexInCont = i;
	}

	objs.pop_back();
}

void Room::replaceForDepth(std::vector<Object*> &vect, Object *o, bool rising) {
	if (vect.size() <= 1) return;

	unsigned index;
	for (index = 0; index < vect.size(); ++index)
		if (vect[index] == o)
			break;

	if (index >= vect.size()) return;

	if (index < vect.size() - 1 
		&& (rising ? vect[index]->depth > vect[index + 1]->depth : vect[index]->depth < vect[index + 1]->depth)) {
		unsigned i;
		Object *toReplace = vect[index];
		for (i = index; i < vect.size() - 1; ++i) {
			if (rising ? toReplace->depth <= vect[i + 1]->depth : toReplace->depth >= vect[i + 1]->depth) break;

			vect[i] = vect[i + 1];
		}
		vect[i] = toReplace;
	} else if (index > 0 
		&& (rising ? vect[index]->depth < vect[index - 1]->depth : vect[index]->depth > vect[index - 1]->depth)) {
		unsigned i;
		Object *toReplace = vect[index];
		for (i = index; i > 0; --i) {
			if (rising ? toReplace->depth >= vect[i - 1]->depth : toReplace->depth <= vect[i - 1]->depth) break;

			vect[i] = vect[i - 1];
		}
		vect[i] = toReplace;
	}
}

void Room::replaceForDepth(unsigned index) {
	if (index >= objs.size() || objs.size() <= 1) return;

	Object *toReplace = objs[index];

	// if this object had the greatest depth and it increased, 
	// or it had the lowest depth and it decreased, 
	// no replacing is necessary in any container
	bool replaceInOthers = false;

	if (index < objs.size() - 1 && objs[index]->depth > objs[index + 1]->depth) {
		unsigned i;
		for (i = index; i < objs.size() - 1; ++i) {
			if (toReplace->depth <= objs[i + 1]->depth) break;

			objs[i] = objs[i + 1];
			objs[i]->indexInCont = i;
		}
		objs[i] = toReplace;
		objs[i]->indexInCont = i;

		replaceInOthers = true;
	} else if (index > 0 && objs[index]->depth < objs[index - 1]->depth) {
		unsigned i;
		for (i = index; i > 0; --i) {
			if (toReplace->depth >= objs[i - 1]->depth) break;

			objs[i] = objs[i - 1];
			objs[i]->indexInCont = i;
		}
		objs[i] = toReplace;
		objs[i]->indexInCont = i;

		replaceInOthers = true;
	}

	if (replaceInOthers) {
		replaceForDepth(reactingTick, toReplace, true);
		replaceForDepth(reactingKeyPressed, toReplace, true);
		replaceForDepth(reactingKeyReleased, toReplace, true);
		replaceForDepth(reactingMouseButtonPressed, toReplace, true);
		replaceForDepth(reactingMouseButtonReleased, toReplace, true);
		replaceForDepth(reactingTextEntered, toReplace, true);
		replaceForDepth(reactingOther, toReplace, true);
	}
}

void Room::addInReacting(Object *o) {
	if (o->reactTick) addInPlaceForDepth(reactingTick, o, false);
	if (o->reactKeyPressed) addInPlaceForDepth(reactingKeyPressed, o, false);
	if (o->reactKeyReleased) addInPlaceForDepth(reactingKeyReleased, o, false);
	if (o->reactMouseButtonPressed) addInPlaceForDepth(reactingMouseButtonPressed, o, false);
	if (o->reactMouseButtonReleased) addInPlaceForDepth(reactingMouseButtonReleased, o, false);
	if (o->reactTextEntered) addInPlaceForDepth(reactingTextEntered, o, false);
	if (o->reactOther) addInPlaceForDepth(reactingOther, o, false);
}

void eraseObjectElement(std::vector<Object*> &vector, const Object *o) {
	// erase-remove idiom
	vector.erase(std::remove(vector.begin(), vector.end(), o), vector.end());
}

void Room::commitOrderedChanges() {
	// while loop was recently introduced, not tested thoroughly
	while (!toDestroy.empty() || !toAdd.empty()) {
		destroyObjs();
		addObjs();
	}

	for (unsigned i = 0; i < objs.size(); ++i)
		objs[i]->commitOrderedChanges();
}

void Room::addObjs() {
	if (toAdd.empty()) return;

	// this temporary storage vector is used because objects may add new objects in their onCreate
	std::vector<Object*> currAdd;
	for (unsigned i = 0; i < toAdd.size(); ++i) {
		currAdd.push_back(toAdd[i]);
	}
	toAdd.clear();

	for (unsigned i = 0; i < currAdd.size(); ++i) {
		// prevent object from being put in the room multiple times
		// a problem is that even then its onCreate is still called
		if (currAdd[i]->owner == this) continue;

		addInPlaceForDepth(objs, currAdd[i], true);
		addInReacting(currAdd[i]);
		currAdd[i]->owner = this;
	}

	while (currAdd.size() > 0) {
		currAdd[currAdd.size() - 1]->onCreate();
		currAdd.pop_back();
	}
}

std::vector<Object*> Room::getPersistentObjects() {
	std::vector<Object*> ret;

	for (unsigned i = 0; i < objs.size(); ++i)
		if (objs[i]->isPersistent())
			ret.push_back(objs[i]);

	return ret;
}

void Room::putPersistentObjects(std::vector<Object*> &persObjs) {
	if (persObjs.empty()) return;

	for (unsigned i = 0; i < persObjs.size(); ++i) {
		// prevent object from being put in the room multiple times
		if (persObjs[i]->owner == this) continue;

		addInPlaceForDepth(objs, persObjs[i], true);
		addInReacting(persObjs[i]);
		persObjs[i]->owner = this;
	}
}

void Room::removeFromReacting(Object *o) {
	eraseObjectElement(reactingTick, o);
	o->setReactTick(false);

	eraseObjectElement(reactingKeyPressed, o);
	o->setReactKeyPressed(false);
	
	eraseObjectElement(reactingKeyReleased, o);
	o->setReactKeyReleased(false);
	
	eraseObjectElement(reactingMouseButtonPressed, o);
	o->setReactMouseButtonPressed(false);
	
	eraseObjectElement(reactingMouseButtonReleased, o);
	o->setReactMouseButtonReleased(false);
	
	eraseObjectElement(reactingTextEntered, o);
	o->setReactTextEntered(false);
	
	eraseObjectElement(reactingOther, o);
	o->setReactOther(false);
}

void Room::destroyObjNowForObjsDestructor(Object *O) {
	if (O->owner != this) return;

	unsigned index = O->indexInCont;
	if (index < 0 || index >= objs.size() 
		|| objs[O->indexInCont] != O) // it used to be == but I'm not sure why
		return;

	O->onDestroy();
	removeFromReacting(O);
	remove(O->indexInCont);
}

void Room::leavingRoom() {
	for (unsigned i = 0; i < objs.size(); ++i) {
		if (!objs[i]->isPersistent()) objs[i]->onDestroy();
	}

	onLeave();
}

void Room::destroyObjs() {
	if (toDestroy.empty()) return;

	// the reason for this vector is that onDestroy() may call destroyObject()
	std::vector<Object*> currDestroy;
	for (unsigned i = 0; i < toDestroy.size(); ++i) {
		currDestroy.push_back(toDestroy[i]);
	}
	toDestroy.clear();

	for (unsigned i = 0; i < currDestroy.size(); ++i)
		currDestroy[i]->onDestroy();

	while (currDestroy.size() > 0) {
		Object *d = currDestroy[currDestroy.size() - 1];
		removeFromReacting(d);
		remove(d->indexInCont);
		currDestroy.pop_back();
		d->owner = 0;
		delete d;
	}
}

void Room::addObject(Object *o) {
	//TODO: add so that toAdd is always sorted by depth
	toAdd.push_back(o);
}

void Room::destroyObject(Object *o) {
	//TODO: add so that toDestroy is always sorted by depth
	toDestroy.push_back(o);
}

void Room::destroyObjects(morph::ObjType t, bool enabledOnly) {
	for (unsigned i = 0; i < objs.size(); ++i) {
		if (enabledOnly && !objs[i]->isEnabled())
			continue;

		if (objs[i]->getType() == t)
			destroyObject(objs[i]);
	}
}

void Room::destroyObjects(const std::vector<morph::ObjType> &T, bool enabledOnly) {
	for (unsigned i = 0; i < objs.size(); ++i) {
		if (enabledOnly && !objs[i]->isEnabled())
			continue;

		unsigned j;
		for (j = 0; j < T.size(); ++j)
			if (objs[i]->getType() == T[j])
				break;
		if (j == T.size())
			continue;

		destroyObject(objs[i]);
	}
}

void Room::destroyObjectsAt(const sf::Vector2f &p, bool enabledOnly) {
	for (unsigned i = 0; i < objs.size(); ++i) {
		if (enabledOnly && !objs[i]->isEnabled())
			continue;

		if (!objs[i]->getCollMask())
			continue;

		if (objs[i]->getCollMask()->containsPoint(p))
			destroyObject(objs[i]);
	}
}

void Room::destroyObjectsAt(const sf::Vector2f &p, morph::ObjType t, bool enabledOnly) {
	for (unsigned i = 0; i < objs.size(); ++i) {
		if (enabledOnly && !objs[i]->isEnabled())
			continue;

		if (!objs[i]->getCollMask())
			continue;

		if (objs[i]->getType() == t && objs[i]->getCollMask()->containsPoint(p))
			destroyObject(objs[i]);
	}
}

void Room::destroyObjectsAt(const sf::Vector2f &p, const std::vector<morph::ObjType> &T, bool enabledOnly) {
	for (unsigned i = 0; i < objs.size(); ++i) {
		if (enabledOnly && !objs[i]->isEnabled())
			continue;

		if (!objs[i]->getCollMask())
			continue;

		unsigned j;
		for (j = 0; j < T.size(); ++j)
			if (objs[i]->getType() == T[j])
				break;
		if (j == T.size())
			continue;

		if (objs[i]->getCollMask()->containsPoint(p))
			destroyObject(objs[i]);
	}
}

bool Room::isColliding(const CollisionMask *modifObjMask, unsigned indexInCont, morph::ObjType t, bool enabledOnly) const {
	for (unsigned i = 0; i < objs.size(); ++i) {
		if (enabledOnly && !objs[i]->isEnabled())
			continue;

		if (indexInCont == i || objs[i]->getType() != t)
			continue;

		if (morph::CollisionMask::areColliding(modifObjMask, objs[i]->getCollMask()))
			return true;
	}

	return false;
}

bool Room::isColliding(const CollisionMask *modifObjMask, unsigned indexInCont, const std::vector<morph::ObjType> &T, bool enabledOnly) const {
	for (unsigned i = 0; i < objs.size(); ++i) {
		if (enabledOnly && !objs[i]->isEnabled())
			continue;

		if (indexInCont == i)
			continue;

		unsigned j;
		for (j = 0; j < T.size(); ++j)
			if (objs[i]->getType() == T[j])
				break;
		if (j == T.size())
			continue;

		if (morph::CollisionMask::areColliding(modifObjMask, objs[i]->getCollMask()))
			return true;
	}

	return false;
}

bool Room::isColliding(const Object *o, morph::ObjType t, bool enabledOnly) const {
	for (unsigned i = 0; i < objs.size(); ++i) {
		if (enabledOnly && !objs[i]->isEnabled())
			continue;

		if (o->indexInCont == i || objs[i]->getType() != t)
			continue;

		if (o->isColliding(objs[i]))
			return true;
	}

	return false;
}

bool Room::isColliding(const Object *o, const std::vector<morph::ObjType> &T, bool enabledOnly) const {
	for (unsigned i = 0; i < objs.size(); ++i) {
		if (enabledOnly && !objs[i]->isEnabled())
			continue;

		if (o->indexInCont == i)
			continue;

		unsigned j;
		for (j = 0; j < T.size(); ++j)
			if (objs[i]->getType() == T[j])
				break;
		if (j == T.size())
			continue;

		if (o->isColliding(objs[i]))
			return true;
	}

	return false;
}

bool Room::wouldCollide(const Object *o, const sf::Vector2f &offset, const Object *other, const sf::Vector2f &otherOffset) const {
	if (!o->getCollMask() || !other->getCollMask())
		return false;

	CollisionMask *m1 = o->getCollMask()->clone(), *m2 = other->getCollMask()->clone();
	m1->move(offset);
	m2->move(otherOffset);

	bool toRet = m1->collidesWith(m2);

	delete m1;
	delete m2;
	return toRet;
}

bool Room::wouldCollide(const Object *o, const sf::Vector2f &offset, morph::ObjType t, bool enabledOnly) const {
	if (!o->getCollMask())
		return false;

	CollisionMask *m = o->getCollMask()->clone();
	m->move(offset);

	bool toRet = isColliding(m, o->indexInCont, t, enabledOnly);

	delete m;
	return toRet;
}

bool Room::wouldCollide(const Object *o, const sf::Vector2f &offset, const std::vector<morph::ObjType> &T, bool enabledOnly) const {
	if (!o->getCollMask())
		return false;

	CollisionMask *m = o->getCollMask()->clone();
	m->move(offset);

	bool toRet = isColliding(m, o->indexInCont, T, enabledOnly);

	delete m;
	return toRet;
}

void Room::setEnabled(bool enabled, morph::ObjType t, bool affectObjsAboutToBePutIn) {
	for (unsigned i = 0; i < objs.size(); ++i) {
		if (objs[i]->getType() == t)
			objs[i]->setEnabled(enabled);
	}

	if (affectObjsAboutToBePutIn) {
		for (unsigned i = 0; i < toAdd.size(); ++i) {
			if (toAdd[i]->getType() == t)
				toAdd[i]->setEnabled(enabled);
		}
	}
}

void Room::setEnabled(bool enabled, const std::vector<morph::ObjType> &T, bool affectObjsAboutToBePutIn) {
	for (unsigned i = 0; i < objs.size(); ++i) {
		if (objs[i]->isEnabled() == enabled)
			continue;

		unsigned j;
		for (j = 0; j < T.size(); ++j)
			if (objs[i]->getType() == T[j])
				break;
		if (j == T.size())
			continue;

		objs[i]->setEnabled(enabled);
	}

	if (affectObjsAboutToBePutIn) {
		for (unsigned i = 0; i < toAdd.size(); ++i) {
			if (toAdd[i]->isEnabled() == enabled)
				continue;

			unsigned j;
			for (j = 0; j < T.size(); ++j)
				if (toAdd[i]->getType() == T[j])
					break;
			if (j == T.size())
				continue;

			toAdd[i]->setEnabled(enabled);
		}
	}
}

void Room::onEvent(const sf::Event &e) {
	this->preEvent(e);

	commitOrderedChanges();

	if (e.type == sf::Event::KeyPressed) {
		for (unsigned i = 0; i < reactingKeyPressed.size();) {
			if (reactingKeyPressed[i]->isEnabled() && reactingKeyPressed[i]->getReactKeyPressed()) {
				reactingKeyPressed[i]->onKeyPressed(e.key);
				++i;
			} else if (reactingKeyPressed[i]->isEnabled()) {
				reactingKeyPressed.erase(reactingKeyPressed.begin() + i);
			}
		}
	} else if (e.type == sf::Event::KeyReleased) {
		for (unsigned i = 0; i < reactingKeyReleased.size();) {
			if (reactingKeyReleased[i]->isEnabled() && reactingKeyReleased[i]->getReactKeyReleased()) {
				reactingKeyReleased[i]->onKeyReleased(e.key);
				++i;
			} else if (reactingKeyReleased[i]->isEnabled()) {
				reactingKeyReleased.erase(reactingKeyReleased.begin() + i);
			}
		}
	} else if (e.type == sf::Event::MouseButtonPressed) {
		for (unsigned i = 0; i < reactingMouseButtonPressed.size();) {
			if (reactingMouseButtonPressed[i]->isEnabled() && reactingMouseButtonPressed[i]->getReactMouseButtonPressed()) {
				reactingMouseButtonPressed[i]->onMouseButtonPressed(e.mouseButton);
				++i;
			} else if (reactingMouseButtonPressed[i]->isEnabled()) {
				reactingMouseButtonPressed.erase(reactingMouseButtonPressed.begin() + i);
			}
		}
	} else if (e.type == sf::Event::MouseButtonReleased) {
		for (unsigned i = 0; i < reactingMouseButtonReleased.size();) {
			if (reactingMouseButtonReleased[i]->isEnabled() && reactingMouseButtonReleased[i]->getReactMouseButtonReleased()) {
				reactingMouseButtonReleased[i]->onMouseButtonReleased(e.mouseButton);
				++i;
			} else if (reactingMouseButtonReleased[i]->isEnabled()) {
				reactingMouseButtonReleased.erase(reactingMouseButtonReleased.begin() + i);
			}
		}
	} else if (e.type == sf::Event::TextEntered) {
		for (unsigned i = 0; i < reactingTextEntered.size();) {
			if (reactingTextEntered[i]->isEnabled() && reactingTextEntered[i]->getReactTextEntered()) {
				reactingTextEntered[i]->onTextEntered(e.text);
				++i;
			} else if (reactingTextEntered[i]->isEnabled()) {
				reactingTextEntered.erase(reactingTextEntered.begin() + i);
			}
		}
	} else {
		for (unsigned i = 0; i < reactingOther.size();) {
			if (reactingOther[i]->isEnabled() && reactingOther[i]->getReactOther()) {
				reactingOther[i]->onEventOther(e);
				++i;
			} else if (reactingOther[i]->isEnabled()) {
				reactingOther.erase(reactingOther.begin() + i);
			}
		}
	}

	checkCollisions();

	this->postEvent(e);
}

void Room::onTick() {
	this->preTick();

	commitOrderedChanges();

	for (unsigned i = 0; i < reactingTick.size();) {
		if (reactingTick[i]->isEnabled() && reactingTick[i]->getReactTick()) {
			reactingTick[i]->onTickMaskLookAndObj();
			++i;
		} else if (reactingTick[i]->isEnabled()) {
			reactingTick.erase(reactingTick.begin() + i);
		}
	}

	checkCollisions();

	this->postTick();
}

void Room::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	this->drawSelf(target, states);

	if (objs.size() > 0) {
		unsigned i = objs.size() - 1;
		while (i > 0) {
			if (objs[i]->isEnabled())
				target.draw(*objs[i], states);

			--i;
		}
		if (objs[0]->isEnabled())
			target.draw(*objs[0], states);
	}
}

void Room::freeObjMemory(bool deletePersistent) {
	for (unsigned i = 0; i < objs.size(); ++i)
		if (!objs[i]->isPersistent() || deletePersistent) {
			objs[i]->owner = 0;
			delete objs[i];
		}

	for (unsigned i = 0; i < toAdd.size(); ++i) {
		toAdd[i]->owner = 0;
		delete toAdd[i];
	}
}

void Room::endGame() {
	owner->endGame();
}

Room::~Room() {
	freeObjMemory(getFlow() ? getFlow()->isEnded() : true);
}