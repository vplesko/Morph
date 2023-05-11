#ifndef _MORPH_OBJECT_H_
#define _MORPH_OBJECT_H_

#include <SFML/Graphics.hpp>
#include <Morph/Collisions.h>

namespace morph {
	class CollisionMask;
	class Look;

	class Object : public sf::Drawable {
		//no copying
		Object(const Object&);
		void operator=(const Object&);
		
	protected:
		friend class Room;
	private:
		Room *owner;
		unsigned indexInCont;
	protected:
		void onTickMaskLookAndObj();

	private:
		//@Morph REACTS@
		bool reactTick, 
			reactKeyPressed, 
			reactKeyReleased, 
			reactMouseButtonPressed, 
			reactMouseButtonReleased, 
			reactTextEntered, 
			reactOther;

	private:
		void setReactTick(bool b) { reactTick = b; }
		bool getReactTick() const { return reactTick; }
		void setReactKeyPressed(bool b) { reactKeyPressed = b; }
		bool getReactKeyPressed() const { return reactKeyPressed; }
		void setReactKeyReleased(bool b) { reactKeyReleased = b; }
		bool getReactKeyReleased() const { return reactKeyReleased; }
		void setReactMouseButtonPressed(bool b) { reactMouseButtonPressed = b; }
		bool getReactMouseButtonPressed() const { return reactMouseButtonPressed; }
		void setReactMouseButtonReleased(bool b) { reactMouseButtonReleased = b; }
		bool getReactMouseButtonReleased() const { return reactMouseButtonReleased; }
		void setReactTextEntered(bool b) { reactTextEntered = b; }
		bool getReactTextEntered() const { return reactTextEntered; }
		void setReactOther(bool b) { reactOther = b; }
		bool getReactOther() const { return reactOther; }

		void setReactToDefault();
		void setReactsToFalse();

	private:
		bool checkCollisionsForThis;
	protected:
		void setCheckCollisionsForThis(bool c) { checkCollisionsForThis = c; }
	public:
		bool getCheckCollisionsForThis() const { return checkCollisionsForThis; }

	public:
		//@Morph REACTS@
		virtual void onTick() { setReactTick(false); }
		virtual void onKeyPressed(const sf::Event::KeyEvent&) { setReactKeyPressed(false); }
		virtual void onKeyReleased(const sf::Event::KeyEvent&) { setReactKeyReleased(false); }
		virtual void onMouseButtonPressed(const sf::Event::MouseButtonEvent&) { setReactMouseButtonPressed(false); }
		virtual void onMouseButtonReleased(const sf::Event::MouseButtonEvent&) { setReactMouseButtonReleased(false); }
		virtual void onTextEntered(const sf::Event::TextEvent&) { setReactTextEntered(false); }
		virtual void onEventOther(const sf::Event &e) { setReactOther(false); }

	private:
		bool enabled;
		bool persistent;
		sf::Vector2f position;
		sf::Vector2f origin;
		int depth;
		CollisionMask *m;
		bool rendered;
		Look *l;

		bool enabledToChange;
		bool enabledNext;
		void commitOrderedChangesToEnabled();
		bool positionToChange;
		sf::Vector2f positionNext;
		void commitOrderedChangesToPosition();
		bool originToChange;
		sf::Vector2f originNext;
		void commitOrderedChangesToOrigin();
		bool depthToChange;
		int depthNext;
		void commitOrderedChangesToDepth();
		bool collMaskToChange;
		CollisionMask *nextMask;
		void commitOrderedChangesToCollMask();
		bool lookToChange;
		Look *nextLook;
		void commitOrderedChangesToLook();
		void setOrderedChangesToFalse();
		void commitOrderedChanges();

		void setStartValues(int Depth);
	public:
		explicit Object(sf::Vector2f startPosition = sf::Vector2f(), int Depth = 0);
		explicit Object(int Depth);

		void changeRoom(Room *r);
		Room* getRoom() { return owner; }

		bool isColliding(const Object *o) const;
		static bool isColliding(const Object *o, morph::ObjType t, bool enabledOnly);
		bool isColliding(morph::ObjType t, bool enabledOnly) const { return isColliding(this, t, enabledOnly); }
		static bool isColliding(const Object *o, std::vector<morph::ObjType> T, bool enabledOnly);
		bool isColliding(std::vector<morph::ObjType> T, bool enabledOnly) const { return isColliding(this, T, enabledOnly); }
		bool wouldCollide(const sf::Vector2f &offset, const Object *other, const sf::Vector2f &otherOffset = sf::Vector2f()) const;
		bool wouldCollide(const sf::Vector2f &offset, morph::ObjType t, bool enabledOnly) const;
		bool wouldCollide(const sf::Vector2f &offset, std::vector<morph::ObjType> T, bool enabledOnly) const;

		unsigned objCount(morph::ObjType t, bool enabledOnly) const;
		unsigned objCount(std::vector<morph::ObjType> T, bool enabledOnly) const;

		virtual ObjType getType() const { return T_UNDEFINED; }

		const CollisionMask* getCollMask() const { return m; }
		const CollisionMask* getCollMaskNext() const { return collMaskToChange ? nextMask : getCollMask(); }
		void setCollMask(CollisionMask *cm);

		bool isRendered() const { return rendered; }
		void setRendered(bool r) { rendered = r; }
		const Look* getLook() const { return l; }
		const Look* getLookNext() const { return lookToChange ? nextLook : getLook(); }
		void setLook(Look *ol);

		bool collMaskContainsPoint(const sf::Vector2f &p) const;
		bool collMaskContainsPoint(float px, float py) const { return collMaskContainsPoint(sf::Vector2f(px, py)); }
		bool collMaskContainsPoint(const sf::Event::MouseButtonEvent &e, bool mapViewPixelToCoords) const;

		int getDepth() const { return depth; }
		void setDepth(int Depth);

		bool isEnabled() const { return enabled; }
		bool getEnabled() const { return isEnabled(); }
		bool getEnabledNext() const { return enabledToChange ? enabledNext : isEnabled(); }
		void setEnabled(bool Enabled);
		void setEnabled(bool enabled, morph::ObjType t, bool affectObjsAboutToBePutIn = false);
		void setEnabled(bool enabled, std::vector<morph::ObjType> T, bool affectObjsAboutToBePutIn = false);

		bool isPersistent() const { return persistent; }
		void setPersistent(bool p) { persistent = p; }

		virtual void onCollision(Object *o) {}
		virtual void onCreate() {}
		virtual void onDestroy() {}
		virtual void onEnabledChange() {}
		virtual void preDraw(sf::RenderTarget& target, sf::RenderStates states) const {}
		virtual void postDraw(sf::RenderTarget& target, sf::RenderStates states) const {}

		const sf::Vector2f& getPosition() const { return position; }
		const sf::Vector2f& getPositionNext() const { return positionToChange ? positionNext : getPosition(); }
		void setPosition(const sf::Vector2f &Position);
		void setPosition(float X, float Y) { setPosition(sf::Vector2f(X, Y)); }
		void move(const sf::Vector2f &offset);
		void move(float offX, float offY) { move(sf::Vector2f(offX, offY)); }
		const sf::Vector2f& getOrigin() const { return origin; }
		const sf::Vector2f& getOriginNext() const { return originToChange ? originNext : getOrigin(); }
		void setOrigin(float x, float y);
		void setOrigin(const sf::Vector2f &orig) { setOrigin(orig.x, orig.y); }
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

		void endGame();

		//if you want to destroy an Object, you should call destroyObject method from Room
		//but you may call this from a destructor of another Object
		virtual ~Object();
	};
}

#endif