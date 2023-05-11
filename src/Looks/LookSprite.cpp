#include <Morph/Looks/LookSprite.h>
using namespace morph;

LookSprite::LookSprite(const sf::Texture &texture, bool resetRect) {
	sprite.setTexture(texture, resetRect);
}

void LookSprite::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	sf::Transform combinedTransform = states.transform * getTransform();
	states.transform = combinedTransform;
	target.draw(sprite, states);
}