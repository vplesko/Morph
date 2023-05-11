#ifndef _MORPH_OBJ_BUTTON_H_
#define _MORPH_OBJ_BUTTON_H_

#include <Morph/Object.h>

namespace morph {
	class ObjButton : public Object {
		sf::Mouse::Button b;
		int x, y;

		bool testCollMask;

	protected:
		sf::Mouse::Button getMouseButton() const { return b; }
		int getMouseX() const { return x; }
		int getMouseY() const { return y; }

	public:
		explicit ObjButton(int Depth, bool testCollisionMask = true);
		explicit ObjButton(sf::Vector2f startCoords = sf::Vector2f(), 
			int Depth = 0, 
			bool testCollisionMask = true);

		// If you are overriding this, you need to make a call to the ObjButton::onCreate()
		virtual void onCreate();
		
		// If you are overriding this, you need to make a call to the ObjButton::onDestroy()
		virtual void onDestroy();
		
		// If you are overriding this, you need to make a call to the ObjButton::onEnabledChange()
		virtual void onEnabledChange();
		
		virtual void onClick() =0;

		void onMouseButtonPressed(const sf::Event::MouseButtonEvent &e);

		bool isTestingCollisionMask() const { return testCollMask; }
		void setTestingCollisionMask(bool testCollisionMask) { testCollMask = testCollisionMask; }

		virtual ~ObjButton();

	private:
		static bool trigger_only_one, triggered;
		static unsigned button_cnt, button_ind;

	public:
		static bool getTriggerOnlyOneButtonPerClick() { return trigger_only_one; }
		static void setTriggerOnlyOneButtonPerClick(bool t) { trigger_only_one = t; }
	};
}

#endif