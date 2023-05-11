#include <Morph/Looks/LookText.h>
using namespace morph;

LookText::LookText(const sf::Font &font, const std::string &text) 
	: txt(text, font) {
}

void LookText::removeLastCharacter() {
	sf::String t(txt.getString());
	if (t.getSize() == 0) return;
	t.erase(t.getSize() - 1);
	txt.setString(t);
}

void LookText::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	sf::Transform combinedTransform = states.transform * getTransform();
	states.transform = combinedTransform;
	target.draw(txt, states);
}