#include <iostream>
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>
#include "modules/.env.h"
#include "modules/utils.h"
#include "modules/Personagem.cpp"
#include "modules/Monstro.cpp"

const int* MAPAS[NUMERO_TOTAL_MAPAS] = { &MAPA_INICIAL[0][0], &MAPA_2[0][0], &MAPA_3[0][0]};

class Window {
public:
	bool createWindow()
	{
		Personagem npc(20, 50);

		int npcCounter = 0;

		// Set a seed for random number generation
		std::srand(static_cast<unsigned int>(std::time(nullptr)));

		// run the program as long as the window is open
		while (window.isOpen())
		{
			const int* mapa = this->getMapa(this->currentMap);

			// check all the window's events that were triggered since the last iteration of the loop
			sf::Event event;
			while (window.pollEvent(event))
			{
				// "close requested" event: we close the window
				if (event.type == sf::Event::Closed)
					window.close();

				// check for key pressed event
				if (event.type == sf::Event::KeyPressed)
				{
					sf::Vector2f nextPosition = player.getPosition(); // Obter a próxima posição antes de mover

					switch (event.key.code)
					{
					case sf::Keyboard::Left:
						player.animateWalk(ESQUERDA);
						nextPosition.x -= 24.0f;
						break;
					case sf::Keyboard::Right:
						player.animateWalk(DIREITA);
						nextPosition.x += 24.0f;
						break;
					case sf::Keyboard::Up:
						player.animateWalk(CIMA);
						nextPosition.y -= 24.0f;
						break;
					case sf::Keyboard::Down:
						player.animateWalk(BAIXO);
						nextPosition.y += 24.0f;
						break;
					}

					// Verificar se a próxima posição é válida (não é uma parede)
					if (!isWall(nextPosition, mapa)) {
						player.move(sf::Vector2f(nextPosition.x - player.getPosition().x, nextPosition.y - player.getPosition().y), window.getSize());
						this->handleDoorCollision(player.getPosition());

            this->handleMonsterCollision(player.getPosition());
					}
				}
			}

			if (npcCounter == 600) {
				// Move NPC randomly
				int randomDirection = std::rand() % 4; // 0: left, 1: right, 2: up, 3: down
				switch (randomDirection)
				{
				case 0:
					npc.animateWalk(ESQUERDA);
					npc.move(sf::Vector2f(-24.0f, 0.0f), window.getSize());
					break;
				case 1:
					npc.animateWalk(DIREITA);
					npc.move(sf::Vector2f(24.0f, 0.0f), window.getSize());
					break;
				case 2:
					npc.animateWalk(CIMA);
					npc.move(sf::Vector2f(0.0f, -24.0f), window.getSize());
					break;
				case 3:
					npc.animateWalk(BAIXO);
					npc.move(sf::Vector2f(0.0f, 24.0f), window.getSize());
					break;
				}

				npcCounter = 0;
			}
			else {
				npcCounter = npcCounter + 1;
			}

			// clear the window with black color
			window.clear(sf::Color::Black);

			// Renderizar quadrados amarelos nas paredes e cinza nos chãos
			for (int i = 0; i < ALTURA_MAPA; ++i)
			{
				for (int j = 0; j < LARGURA_MAPA; ++j)
				{
					sf::RectangleShape tile(sf::Vector2f(24.0f, 24.0f));

					// Se for parede (1), renderiza amarelo; se for dois, renderiza marrom, senão, renderiza cinza
					if (mapa[i * LARGURA_MAPA + j] == 1) {
						tile.setFillColor(sf::Color::Yellow);
					}
					else if (mapa[i * LARGURA_MAPA + j] == 2) {
						tile.setFillColor(sf::Color(139, 69, 19)); // Marrom
					}
					else {
						if (SHOW_MONSTERS && mapa[i * LARGURA_MAPA + j] == 3)
							tile.setFillColor(sf::Color(255, 0, 0)); // Cinza
						else
							tile.setFillColor(sf::Color(128, 128, 128)); // Cinza
					}

					tile.setPosition(j * 24.0f, i * 24.0f);
					window.draw(tile);
				}
			}

			// Draw the background
			window.draw(player.getCurrentSprite());
			window.draw(npc.getCurrentSprite());

			// end the current frame
			window.display();
		}

		return true;
	}

private:
	sf::RenderWindow window{ sf::VideoMode(815, 600), "Deep Rock Galactic" };
	Personagem player{ 50, 50 };
	int currentMap = 0;

	bool isWall(sf::Vector2f position, const int* mapa)
	{
		// Converter a posição em coordenadas da matriz
		int mapX = static_cast<int>(position.x / 24);
		int mapY = static_cast<int>(position.y / 24) + 2;

		// Verificar se a posição está dentro dos limites da matriz e se é uma parede
		return (mapX >= 0 && mapX < LARGURA_MAPA && mapY >= 0 && mapY < ALTURA_MAPA && mapa[mapY * LARGURA_MAPA + mapX] == 1);
	}

	bool isDoor(sf::Vector2f position)
	{
		int mapX = static_cast<int>(position.x / 24);
		int mapY = static_cast<int>(position.y / 24) + 2;

		// Verificar se a posição atual está sobre o chão 2
		return (mapX >= 0 && mapX < LARGURA_MAPA &&
			mapY >= 0 && mapY < ALTURA_MAPA &&
			MAPAS[currentMap][mapY * LARGURA_MAPA + mapX] == 2);
	}

  bool isMonster(sf::Vector2f position)
  {
    int mapX = static_cast<int>(position.x / 24);
    int mapY = static_cast<int>(position.y / 24) + 2;

    // Verificar se a posição atual está sobre o chão 3
    return (mapX >= 0 && mapX < LARGURA_MAPA &&
      mapY >= 0 && mapY < ALTURA_MAPA &&
      MAPAS[currentMap][mapY * LARGURA_MAPA + mapX] == 3);
  }

	const int* getMapa(int index) {
		if (index >= 0 && index < NUMERO_TOTAL_MAPAS) {
			currentMap = index;
			return MAPAS[currentMap];
		}
		return nullptr;
	}

	void handleDoorCollision(sf::Vector2f playerPosition)
	{
		std::cout << "Is porta?: " << isDoor(playerPosition) << std::endl; // "Is door?: 1

		// Adapte isso conforme necessário para a posição da porta
		if (isDoor(playerPosition)) {
			int nextMapIndex = currentMap + 1;
			const int* nextMap = getMapa(nextMapIndex);

			std::cout << "Next map: " << nextMap << std::endl; // "Next map: 0x7ff8a4c00000"
			std::cout << "Next map index: " << nextMapIndex << std::endl; // "Next map index: 1"

			// Se houver um próximo mapa, atualize o mapa atual
			if (nextMap != nullptr) {
				std::cout << "Tocou na porta! Mudando para o próximo mapa." << std::endl;
				currentMap = nextMapIndex;

				// Resetar a posição do jogador para a posição inicial do novo mapa
				this->player.setPosition(sf::Vector2f(50, 50)); // ou a posição inicial desejada
			}
		}
	}

  void handleMonsterCollision(sf::Vector2f playerPosition)
  {
    if (isMonster(playerPosition)) {
      std::cout << "Area de Monstro!" << std::endl;

      if (gerarNumeroAleatorio(1, 5) == 3) {
        std::cout << "Começar uma batalha!" << std::endl;

        int IndiceMonstro = gerarNumeroAleatorio(0, 2);

        Monstro *novoMonstro = nullptr;

        switch (IndiceMonstro) {
        case ARANHA:
          novoMonstro = new Aranha();
          break;
        case ARANHA_GRANDE:
          novoMonstro = new AranhaGrande();
          break;
        case ESCORPIAO:
          novoMonstro = new Escorpiao();
          break;
        }

         cout << novoMonstro->getNomeMonstro();

         delete novoMonstro;
      }
    }
  }
};

int main()
{
	Window window;

	if (!window.createWindow()) {
		std::cout << "Erro ao criar a janela" << std::endl;
		return 1;
	}

	return 0;
}
