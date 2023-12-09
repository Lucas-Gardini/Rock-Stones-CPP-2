#include <iostream>
#include <SFML/Graphics.hpp>
#include "./.env.h"

#define MAX_X 144
#define SPRITE_WIDTH 48
#define SPRITE_HEIGHT 64

class Personagem {
public:
	sf::Texture texture;
	sf::IntRect rectSourceSprite;
	sf::Sprite sprite;

	Direcoes direcaoAtual;

	Personagem(int x, int y) {
		this->texture.loadFromFile(ASSETS_FOLDER + "sprites/personagem/dwarf1.png");
		this->rectSourceSprite = sf::IntRect(0, 0, SPRITE_WIDTH, SPRITE_HEIGHT);
		this->sprite = sf::Sprite(this->texture, this->rectSourceSprite);
		this->sprite.setPosition(static_cast<float>(x), static_cast<float>(y));
	}

	void animateWalk(Direcoes direcao) {
		switch (direcao)
		{
		case ESQUERDA:
			if (direcaoAtual != ESQUERDA) rectSourceSprite.left = 0;
			direcaoAtual = ESQUERDA;

			rectSourceSprite.top = SPRITE_HEIGHT * 3;
			break;
		case DIREITA:
			if (direcaoAtual != DIREITA) rectSourceSprite.left = 0;
			direcaoAtual = DIREITA;

			rectSourceSprite.top = SPRITE_HEIGHT * 1;
			break;
		case CIMA:
			if (direcaoAtual != CIMA) rectSourceSprite.left = 0;
			direcaoAtual = CIMA;

			rectSourceSprite.top = SPRITE_HEIGHT * 0;
			break;
		case BAIXO:
			if (direcaoAtual != BAIXO) rectSourceSprite.left = 0;
			direcaoAtual = BAIXO;

			rectSourceSprite.top = SPRITE_HEIGHT * 2;
			break;
		default:
			break;
		}

		if (rectSourceSprite.left == (MAX_X - SPRITE_WIDTH))
			rectSourceSprite.left = 0;
		else
			rectSourceSprite.left += SPRITE_WIDTH;
	}

	sf::Sprite getCurrentSprite() {
		this->sprite.setTextureRect(this->rectSourceSprite);

		return this->sprite;
	}

	void move(const sf::Vector2f& offset, const sf::Vector2u& windowSize) {

		// Verifique se o novo local não ultrapassa os limites da janela
		sf::Vector2f newPosition = sprite.getPosition() + offset;
		if (newPosition.x >= 0 && newPosition.x <= windowSize.x - SPRITE_WIDTH &&
			newPosition.y >= 0 && newPosition.y <= windowSize.y - SPRITE_HEIGHT) {
			sprite.move(offset);
		}
	}

	sf::Vector2f getPosition() const {
		return sprite.getPosition();
	}

	void setPosition(const sf::Vector2f& position) {
		sprite.setPosition(position);
	}
};