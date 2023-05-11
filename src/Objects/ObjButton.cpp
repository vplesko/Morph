#include <Morph/Objects/ObjButton.h>
#include <Morph/Room.h>
#include <Morph/Flow.h>
using namespace morph;

ObjButton::ObjButton(int Depth, bool testCollisionMask) 
	: Object(Depth), testCollMask(testCollisionMask) {
}

ObjButton::ObjButton(sf::Vector2f startCoords, int Depth, bool testCollisionMask) 
	: Object(startCoords, Depth), testCollMask(testCollisionMask) {
}

void ObjButton::onCreate() {
	if (isEnabled() && getRoom() != 0) {
		++button_cnt;
	}
}

void ObjButton::onDestroy() {
	if (isEnabled() && getRoom() != 0) {
		--button_cnt;
	}
}

void ObjButton::onEnabledChange() {
	if (getRoom() == 0) return;

	if (isEnabled()) {
		++button_cnt;
	} else {
		--button_cnt;
	}
}

void ObjButton::onMouseButtonPressed(const sf::Event::MouseButtonEvent &e) {
	b = e.button;
	x = e.x;
	y = e.y;

	if (!triggered && (!testCollMask || collMaskContainsPoint(e, true))) {
		triggered = true;
		onClick();
	}

	if (++button_ind == button_cnt) {
		triggered = false;
		button_ind = 0;
	}
}

ObjButton::~ObjButton() {
}

bool ObjButton::trigger_only_one = true;
bool ObjButton::triggered = false;

unsigned ObjButton::button_cnt = 0;
unsigned ObjButton::button_ind = 0;