#ifndef _MORPH_LOOK_TEXT_H_
#define _MORPH_LOOK_TEXT_H_

#include <Morph/Look.h>

namespace morph {
	class LookText : public Look {
	public:
		sf::Text txt;

	public:
		explicit LookText(const sf::Font &font, const std::string &text = std::string());

		Look* clone() const { return new LookText(*this); }

		void appendText(const std::string &text) { txt.setString(txt.getString() + text); }
		void appendChar(char c) { appendText(std::string(1, c)); }
		void removeLastCharacter();

		void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	};
}

#endif