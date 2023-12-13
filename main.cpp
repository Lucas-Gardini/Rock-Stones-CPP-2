#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cstdlib>
#include <ctime>
#include "modules/utils.h"
#include "modules/.env.h"
#include "modules/Personagem.cpp"
#include "modules/Item.cpp"
#include "modules/Monstro.cpp"

const int* MAPAS[NUMERO_TOTAL_MAPAS] = { &MAPA_INICIAL[0][0], &MAPA_2[0][0], &MAPA_3[0][0] };

class Window {
public:
	sf::Texture textureWall;
	sf::Texture textureGround;
	sf::Texture textureGrass;
	vector<sf::Texture*> personagensSelecoes;
	sf::Texture emBatalha;
	Item item;
	int transitionAlpha; // Intensidade do efeito de transicao

	sf::SoundBuffer menuSound;

	bool selectedPersonagem = false;
	int telaAtualPersonagem = 0;
	bool batalhando = false;

	int dadosFramesMax = 30;
	int dadosFrames = 0;

	Window() {
		this->textureWall.loadFromFile(ASSETS_FOLDER + "sprites/texturas/wall.png");
		this->textureGround.loadFromFile(ASSETS_FOLDER + "sprites/texturas/ground.jpg");
		this->textureGrass.loadFromFile(ASSETS_FOLDER + "sprites/texturas/grass.png");
		this->transitionAlpha = 255; // Comeca completamente opaco (sem efeito)

		this->menuSound.loadFromFile(ASSETS_FOLDER + "click.wav");

		sf::Texture* personagem1 = new sf::Texture();
		if (!personagem1->loadFromFile(ASSETS_FOLDER + "screens/personagens1.png")) {
			std::cerr << "Erro ao carregar a textura do personagem 1." << std::endl;
		}

		sf::Texture* personagem2 = new sf::Texture();
		if (!personagem2->loadFromFile(ASSETS_FOLDER + "screens/personagens2.png")) {
			std::cerr << "Erro ao carregar a textura do personagem 2." << std::endl;
		}

		sf::Texture* personagem3 = new sf::Texture();
		if (!personagem3->loadFromFile(ASSETS_FOLDER + "screens/personagens3.png")) {
			std::cerr << "Erro ao carregar a textura do personagem 3." << std::endl;
		}

		sf::Texture* personagem4 = new sf::Texture();
		if (!personagem4->loadFromFile(ASSETS_FOLDER + "screens/personagens4.png")) {
			std::cerr << "Erro ao carregar a textura do personagem 4." << std::endl;
		}

		sf::Texture* personagem5 = new sf::Texture();
		if (!personagem5->loadFromFile(ASSETS_FOLDER + "screens/personagens5.png")) {
			std::cerr << "Erro ao carregar a textura do personagem 5." << std::endl;
		}

		if (!this->emBatalha.loadFromFile(ASSETS_FOLDER + "screens/embatalha.png")) {
			std::cerr << "Erro ao carregar a textura da tela de batalha." << std::endl;
		}

		personagensSelecoes.push_back(personagem1);
		personagensSelecoes.push_back(personagem2);
		personagensSelecoes.push_back(personagem3);
		personagensSelecoes.push_back(personagem4);
		personagensSelecoes.push_back(personagem5);
	}

	bool createWindow()
	{
		/*  Personagem npc(20, 50, 0);

		  int npcCounter = 0;*/

		std::srand(static_cast<unsigned int>(std::time(nullptr)));

		while (window.isOpen())
		{
			const int* mapa = this->getMapa(this->currentMap);

			sf::Event event;
			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					window.close();

				if (!selectedPersonagem) {
					sf::Sprite telaSelecaoSprite(*personagensSelecoes[this->telaAtualPersonagem]);

					// Esticar para cobrir toda a janela
					telaSelecaoSprite.setScale(
						static_cast<float>(window.getSize().x) / telaSelecaoSprite.getLocalBounds().width,
						static_cast<float>(window.getSize().y) / telaSelecaoSprite.getLocalBounds().height
					);

					// Limpe a janela
					window.clear();

					// Desenhar a tela de seleo na janela
					window.draw(telaSelecaoSprite);

					// Atualize a janela
					window.display();
				}

				if (batalhando) {
					sf::Sprite telaEmBatalha(this->emBatalha);

					// Esticar para cobrir toda a janela
					telaEmBatalha.setScale(
						static_cast<float>(window.getSize().x) / telaEmBatalha.getLocalBounds().width,
						static_cast<float>(window.getSize().y) / telaEmBatalha.getLocalBounds().height
					);

					// Limpe a janela
					window.clear();

					// Desenhar a tela de seleo na janela
					window.draw(telaEmBatalha);

					// Atualize a janela
					window.display();
				}

				if (event.type == sf::Event::KeyPressed)
				{
					sf::Vector2f nextPosition;

					if (selectedPersonagem && !batalhando) nextPosition = player->getPosition();

					if (!batalhando) {
						switch (event.key.code)
						{
						case sf::Keyboard::Left:
							if (!selectedPersonagem) {
								this->telaAtualPersonagem--;

								if (this->telaAtualPersonagem < 0) {
									this->telaAtualPersonagem = 0;
								}

								// Crie um objeto de som e defina seu buffer
								sf::Sound sound;
								sound.setBuffer(this->menuSound);

								// Reproduza o som
								sound.play();

								// Aguarde at que o som termine de ser reproduzido (opcional)
								while (sound.getStatus() == sf::Sound::Playing) {
									// Aguarde
								}

								continue;
							}

							player->animateWalk(ESQUERDA);
							nextPosition.x -= 24.0f;
							break;
						case sf::Keyboard::Right:
							if (!selectedPersonagem) {
								this->telaAtualPersonagem++;

								if (this->telaAtualPersonagem > 4) {
									this->telaAtualPersonagem = 4;
								}

								// Crie um objeto de som e defina seu buffer
								sf::Sound sound;
								sound.setBuffer(this->menuSound);

								// Reproduza o som
								sound.play();

								// Aguarde at que o som termine de ser reproduzido (opcional)
								while (sound.getStatus() == sf::Sound::Playing) {
									// Aguarde
								}

								continue;
							}

							player->animateWalk(DIREITA);
							nextPosition.x += 24.0f;
							break;
						case sf::Keyboard::Up:
							player->animateWalk(CIMA);
							nextPosition.y -= 24.0f;
							break;
						case sf::Keyboard::Down:
							player->animateWalk(BAIXO);
							nextPosition.y += 24.0f;
							break;
						case sf::Keyboard::Enter:
							if (!selectedPersonagem) {
								selectedPersonagem = true;

								if (this->telaAtualPersonagem == BATEDOR) {
									this->player = new Batedor(BATEDOR);
								}
								else if (this->telaAtualPersonagem == MEDICO) {
									this->player = new Medico(MEDICO);
								}
								else if (this->telaAtualPersonagem == ESCAVADOR) {
									this->player = new Escavador(ESCAVADOR);
								}
								else if (this->telaAtualPersonagem == GUERREIRO) {
									this->player = new Guerreiro(GUERREIRO);
								}
								else if (this->telaAtualPersonagem == ENGENHEIRO) {
									this->player = new Engenheiro(ENGENHEIRO);
								}

								continue;
							}
							break;
						}

						if (!isWall(nextPosition, mapa)) {
							player->move(sf::Vector2f(nextPosition.x - player->getPosition().x, nextPosition.y - player->getPosition().y), window.getSize());
							this->handleDoorCollision(player->getPosition());
							this->handleMonsterCollision(player->getPosition());
						}
					}
				}
			}

			if (!selectedPersonagem) continue;
			if (batalhando) continue;

			// clear the window with black color
			window.clear(sf::Color::Black);

			// Renderizar quadrados amarelos nas paredes e cinza nos chaos
			for (int i = 0; i < ALTURA_MAPA; ++i)
			{
				for (int j = 0; j < LARGURA_MAPA; ++j)
				{
					sf::RectangleShape tile(sf::Vector2f(24.0f, 24.0f));

					if (mapa[i * LARGURA_MAPA + j] == 1) {
						tile.setTexture(&this->textureWall);
					}
					else if (mapa[i * LARGURA_MAPA + j] == 2) {
						tile.setFillColor(sf::Color(139, 69, 19)); // Marrom
					}
					else {
						if (SHOW_MONSTERS && mapa[i * LARGURA_MAPA + j] == 3)
							tile.setTexture(&this->textureGrass);
						else
							tile.setTexture(&this->textureGround);
					}

					tile.setPosition(j * 24.0f, i * 24.0f);
					window.draw(tile);
				}
			}

			// Draw the background with transition effect
			sf::RectangleShape transitionRect(sf::Vector2f(window.getSize().x, window.getSize().y));
			transitionRect.setFillColor(sf::Color(0, 0, 0, static_cast<sf::Uint8>(transitionAlpha)));
			window.draw(transitionRect);

			window.draw(player->getCurrentSprite());
			if (currentMap == 1 && player->getUltArm() != METRALHADORA) {
				window.draw(item.getCurrentSprite(currentMap, 50, 510));
				handleGunCollision(item.getCurrentSprite(currentMap, 50, 510));
			}
			else if (currentMap == 2 && player->getUltArm() != SHOOTGUN) {
				window.draw(item.getCurrentSprite(currentMap, 20, 510));
				handleGunCollision(item.getCurrentSprite(currentMap, 20, 510));
			}
			else {
				if (player->getUltArm() != UZI && currentMap < 1) {
					window.draw(item.getCurrentSprite(currentMap, 350, 540));
					handleGunCollision(item.getCurrentSprite(currentMap, 350, 540));
				}
			}

			// Gradualmente diminua a intensidade do efeito
			if (transitionAlpha > 0) {
				transitionAlpha -= 5;
			}

			// end the current frame
			window.display();

			// Verifique se a transio est completa
			if (transitionAlpha <= 0) {
				// Realize qualquer ao adicional aps o trmino da transio
				// Por exemplo, voc pode querer recarregar o mapa aqui ou realizar outras aes
			}
		}

		return true;
	}

private:
	sf::RenderWindow window{ sf::VideoMode(815, 600), "Deep Rock Galactic", sf::Style::Close | sf::Style::Titlebar };
	Personagem* player = nullptr;
	int currentMap = 0;

	bool isWall(sf::Vector2f position, const int* mapa)
	{
		int mapX = static_cast<int>(position.x / 24);
		int mapY = static_cast<int>(position.y / 24) + 2;

		return (mapX >= 0 && mapX < LARGURA_MAPA && mapY >= 0 && mapY < ALTURA_MAPA && mapa[mapY * LARGURA_MAPA + mapX] == 1);
	}

	bool isDoor(sf::Vector2f position)
	{
		int mapX = static_cast<int>(position.x / 24);
		int mapY = static_cast<int>(position.y / 24) + 2;

		return (mapX >= 0 && mapX < LARGURA_MAPA &&
			mapY >= 0 && mapY < ALTURA_MAPA &&
			MAPAS[currentMap][mapY * LARGURA_MAPA + mapX] == 2);
	}

	bool isMonster(sf::Vector2f position)
	{
		int mapX = static_cast<int>(position.x / 24);
		int mapY = static_cast<int>(position.y / 24) + 2;

		// Verificar se a posi  o atual est  sobre o ch o 3
		return (mapX >= 0 && mapX < LARGURA_MAPA &&
			mapY >= 0 && mapY < ALTURA_MAPA &&
			MAPAS[currentMap][mapY * LARGURA_MAPA + mapX] == 3);
	}

	bool isColliding(const sf::Sprite& sprite1, const sf::Sprite& sprite2) {
		return sprite1.getGlobalBounds().intersects(sprite2.getGlobalBounds());
	}

	void handleGunCollision(sf::Sprite itemSprite) {
		if (isColliding(player->getCurrentSprite(), itemSprite) && currentMap != player->getUltArm()) {
			player->addItem(currentMap);
			return;
		}
		return;
	}

	const int* getMapa(int index) {
		if (index >= 0 && index < NUMERO_TOTAL_MAPAS) {
			currentMap = index;
			return MAPAS[currentMap];
		}
		return nullptr;
	}

	void handleMonsterCollision(sf::Vector2f playerPosition)
	{
		if (isMonster(playerPosition)) {
			std::cout << "Area de Monstro!" << std::endl;

			if (gerarNumeroAleatorio(1, 4) == 2) {
				std::cout << "Comecar uma batalha!" << std::endl;

					int IndiceMonstro = gerarNumeroAleatorio(0, 2);

					Monstro* novoMonstro = nullptr;

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

					this->batalhando = true;

					// BATALHAR AQUI

					this->window.clear();

					player->atacar();
					// DESENHAR TODOS OS FRAMES DO DADO AQUI
					int i = 1;
					while (i <= 30) {
						sf::Texture textura;
						textura.loadFromFile(ASSETS_FOLDER + "dado/dado (" + to_string(i) + ").jpg");

						sf::Sprite dadoSprite;
						dadoSprite.setTexture(textura);
						dadoSprite.setTextureRect(sf::IntRect(0, 0, 400, 300));
						dadoSprite.setPosition(0, 0);

						// Esticar para cobrir toda a janela
						dadoSprite.setScale(
							static_cast<float>(window.getSize().x) / dadoSprite.getLocalBounds().width,
							static_cast<float>(window.getSize().y) / dadoSprite.getLocalBounds().height
						);

						// Limpe a janela
						window.clear();

						// Desenhar a tela de seleo na janela
						window.draw(dadoSprite);

						// Atualize a janela
						window.display();

						if (i == 30) {
							return;
						}
						else {
							i++;
						}
					}

					delete novoMonstro;
			}
		}
	}

	void handleDoorCollision(sf::Vector2f playerPosition)
	{
		std::cout << "Is porta?: " << isDoor(playerPosition) << std::endl;

		if (isDoor(playerPosition)) {
			int nextMapIndex = currentMap + 1;
			const int* nextMap = getMapa(nextMapIndex);

			if (nextMap != nullptr) {
				std::cout << "Tocou na porta! Mudando para o prximo mapa." << std::endl;

				// Iniciar a transio (aumentar a intensidade do efeito)
				transitionAlpha = 255;

				// Agora voc pode realizar outras aes relacionadas  transio se necessrio

				// Esperar um pouco para que o jogador veja o efeito
				sf::sleep(sf::milliseconds(500));

				currentMap = nextMapIndex;
				this->player->setPosition(sf::Vector2f(50, 50));
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
