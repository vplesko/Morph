#ifndef _MORPH_ROOM_H_
#define _MORPH_ROOM_H_

#include <SFML/Graphics.hpp>
#include <Morph/Collisions.h>
#include <vector>

namespace morph {
	class Object;
	class CollisionMask;

	class Room : public sf::Drawable, public sf::Transformable {
		//no copying
		Room(const Room&);
		void operator=(const Room&);

		void checkCollisions();
		bool isColliding(const CollisionMask *modifObjMask, unsigned indexInCont, morph::ObjType t, bool enabledOnly) const;
		bool isColliding(const CollisionMask *modifObjMask, unsigned indexInCont, const std::vector<morph::ObjType> &T, bool enabledOnly) const;

		//objects in objs are stored relative to their depth, from lowest depth to highest
		// TODO: use lists instead of vectors
		std::vector<Object*> objs, toAdd, toDestroy;

		void commitOrderedChanges();
		void addObjs();
		void destroyObjs();
		void remove(unsigned indexInCont);
				
		//@Morph REACTS@
		// TODO: use lists instead of vectors?

		// TODO: should draw use the same system as events? 
		// the assumption is that most object need to be drawn, 
		// while a few will react to specific events
		std::vector<Object*> reactingTick, 
			reactingKeyPressed, 
			reactingKeyReleased, 
			reactingMouseButtonPressed, 
			reactingMouseButtonReleased, 
			reactingTextEntered, 
			reactingOther;

		//@Morph REACTS@
		void addInReacting(Object *o);
		void removeFromReacting(Object *o);

	private:
		friend class Object;
		void replaceForDepth(unsigned indexInCont);
		void destroyObjNowForObjsDestructor(Object *O);

		void leavingRoom();
		
		static void addInPlaceForDepth(std::vector<Object*> &vect, Object *o, bool updateIndexInCont);
		static void replaceForDepth(std::vector<Object*> &vect, Object *o, bool rising);
		
	protected:
		friend class Flow;
	private:
		Flow *owner;

		std::vector<Object*> getPersistentObjects();
		void putPersistentObjects(std::vector<Object*> &persObjs);
	private:
		void freeObjMemory(bool deletePersistent);

	public:
		Room();

		Flow* getFlow() { return owner; }

		void changeRoom(Room *r);
		
		unsigned objCount(bool enabledOnly) const;
		unsigned objCount(morph::ObjType t, bool enabledOnly) const;
		unsigned objCount(std::vector<morph::ObjType> T, bool enabledOnly) const;
		std::vector<Object*> getObjects(bool enabledOnly);
		std::vector<Object*> getObjects(morph::ObjType t, bool enabledOnly);
		std::vector<Object*> getObjects(std::vector<morph::ObjType> T, bool enabledOnly);

		void moveObjs(const sf::Vector2f &offset, bool enabledOnly);
		void moveObjs(const sf::Vector2f &offset, morph::ObjType t, bool enabledOnly);
		void moveObjs(const sf::Vector2f &offset, const std::vector<morph::ObjType> &T, bool enabledOnly);
		void moveObjs(float offX, float offY, bool enabledOnly) { moveObjs(sf::Vector2f(offX, offY), enabledOnly); }
		void moveObjs(float offX, float offY, morph::ObjType t, bool enabledOnly) { moveObjs(sf::Vector2f(offX, offY), t, enabledOnly); }
		void moveObjs(float offX, float offY, const std::vector<morph::ObjType> &T, bool enabledOnly) { moveObjs(sf::Vector2f(offX, offY), T, enabledOnly); }

		bool isColliding(const Object *o, morph::ObjType t, bool enabledOnly) const;
		bool isColliding(const Object *o, const std::vector<morph::ObjType> &T, bool enabledOnly) const;
		bool wouldCollide(const Object *o, const sf::Vector2f &offset, const Object *other, const sf::Vector2f &otherOffset = sf::Vector2f()) const;
		bool wouldCollide(const Object *o, const sf::Vector2f &offset, morph::ObjType t, bool enabledOnly) const;
		bool wouldCollide(const Object *o, const sf::Vector2f &offset, const std::vector<morph::ObjType> &T, bool enabledOnly) const;

		void addObject(Object *o);
		void destroyObject(Object *o);
		void destroyObjects(morph::ObjType t, bool enabledOnly);
		void destroyObjects(const std::vector<morph::ObjType> &T, bool enabledOnly);
		void destroyObjectsAt(const sf::Vector2f &p, bool enabledOnly);
		void destroyObjectsAt(const sf::Vector2f &p, morph::ObjType t, bool enabledOnly);
		void destroyObjectsAt(const sf::Vector2f &p, const std::vector<morph::ObjType> &T, bool enabledOnly);

		void setEnabled(bool enabled, morph::ObjType t, bool affectObjsAboutToBePutIn = false);
		void setEnabled(bool enabled, const std::vector<morph::ObjType> &T, bool affectObjsAboutToBePutIn = false);

		//@Morph REACTS@
		//events and ticks are first called in objects with the lowest depth, and lastly on those with the highest
		void onEvent(const sf::Event &e);
		void onTick();

		//objects are drawn in reverse of their depth order, so objects with the highest depth are drawn 'behind' all others
		void draw(sf::RenderTarget& target, sf::RenderStates states) const;

		virtual void onEnter() {}
		virtual void onLeave() {}
		virtual void drawSelf(sf::RenderTarget& target, sf::RenderStates states) const {};
		virtual void preEvent(const sf::Event &e) {}
		virtual void preTick() {}
		virtual void postEvent(const sf::Event &e) {}
		virtual void postTick() {}

		void endGame();

		virtual ~Room();
	};
}

#endif