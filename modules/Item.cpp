#include <iostream>
#include <SFML/Graphics.hpp>
#include "./.env.h"

#define SPRITE_WIDTH 70
#define SPRITE_HEIGHT 33

class Item {
public:
	sf::Texture texture;
	sf::Sprite sprite;
	sf::IntRect rectSourceSprite;

	Item() {
		this->texture.loadFromFile(ASSETS_FOLDER + "sprites/armas/armas.png");
		this->rectSourceSprite = sf::IntRect(0, (SPRITE_HEIGHT - 11) * 3, SPRITE_WIDTH, SPRITE_HEIGHT);
		this->sprite = sf::Sprite(this->texture, this->rectSourceSprite);
	}

	sf::Sprite getCurrentSprite(int i, int x, int y) {
		if (i == SHOOTGUN) {
			rectSourceSprite.left = 0;
			rectSourceSprite.top = 0;
			sprite.setTextureRect(rectSourceSprite);
		}
		else if (i == REVOLVER) {
			rectSourceSprite.left = (SPRITE_WIDTH - 25) * 3;
			rectSourceSprite.top = 0;
			sprite.setTextureRect(rectSourceSprite);
		}
		this->sprite.setPosition(static_cast<float>(x), static_cast<float>(y));
		return this->sprite;
	}
};