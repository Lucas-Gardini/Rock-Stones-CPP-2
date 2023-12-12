#include <iostream>
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>
#include "modules/utils.h"
#include "modules/.env.h"
#include "modules/Personagem.cpp"
#include "modules/Item.cpp"

const int* MAPAS[NUMERO_TOTAL_MAPAS] = { &MAPA_INICIAL[0][0], &MAPA_2[0][0], &MAPA_3[0][0] };

class Window {
public:
    sf::Texture textureWall;
    sf::Texture textureGround;
    Item item;
    int transitionAlpha; // Intensidade do efeito de transicao

    Window() {
        this->textureWall.loadFromFile(ASSETS_FOLDER + "sprites/texturas/wall.png");
        this->textureGround.loadFromFile(ASSETS_FOLDER + "sprites/texturas/ground.jpg");
        this->transitionAlpha = 255; // Comeca completamente opaco (sem efeito)
    }

    bool createWindow()
    {
        Personagem npc(20, 50, 0);

        int npcCounter = 0;

        std::srand(static_cast<unsigned int>(std::time(nullptr)));

        while (window.isOpen())
        {
            const int* mapa = this->getMapa(this->currentMap);

            sf::Event event;
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                    window.close();

                if (event.type == sf::Event::KeyPressed)
                {
                    sf::Vector2f nextPosition = player.getPosition();

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

                    if (!isWall(nextPosition, mapa)) {
                        player.move(sf::Vector2f(nextPosition.x - player.getPosition().x, nextPosition.y - player.getPosition().y), window.getSize());
                        this->handleDoorCollision(player.getPosition());
                        //this->handleMonsterCollision(player.getPosition());
                    }
                }
            }

            if (npcCounter == 600) {
                int randomDirection = std::rand() % 4;
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
                            tile.setFillColor(sf::Color(255, 0, 0)); // Cinza
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

            window.draw(player.getCurrentSprite());
            window.draw(npc.getCurrentSprite());
            if (currentMap == 1) {
                window.draw(item.getCurrentSprite(currentMap, 50, 510));
                handleGunCollision(item.getCurrentSprite(currentMap, 50, 510));
            }
            else if (currentMap == 2) {
                window.draw(item.getCurrentSprite(currentMap, 20, 510));
                handleGunCollision(item.getCurrentSprite(currentMap, 20, 510));
            }
            else {
                window.draw(item.getCurrentSprite(currentMap, 350, 540));
                handleGunCollision(item.getCurrentSprite(currentMap, 350, 540));
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
    sf::RenderWindow window{ sf::VideoMode(815, 600), "Deep Rock Galactic" };
    Personagem player{ 50, 50, 2 };
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

    //bool isMonster(sf::Vector2f position)
    //{
    //    int mapX = static_cast<int>(position.x / 24);
    //    int mapY = static_cast<int>(position.y / 24) + 2;

    //    // Verificar se a posi  o atual est  sobre o ch o 3
    //    return (mapX >= 0 && mapX < LARGURA_MAPA &&
    //        mapY >= 0 && mapY < ALTURA_MAPA &&
    //        MAPAS[currentMap][mapY * LARGURA_MAPA + mapX] == 3);
    //}


    bool isColliding(const sf::Sprite& sprite1, const sf::Sprite& sprite2) {
        return sprite1.getGlobalBounds().intersects(sprite2.getGlobalBounds());
    }

    void handleGunCollision(sf::Sprite itemSprite) {
        if (isColliding(player.getCurrentSprite(), itemSprite)) {
            std::cout << "Personagem colidiu com o item!" << std::endl;
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

    /*void handleMonsterCollision(sf::Vector2f playerPosition)
    {
        if (isMonster(playerPosition)) {
            std::cout << "Area de Monstro!" << std::endl;

            if (gerarNumeroAleatorio(1, 5) == 3) {
                std::cout << "Come ar uma batalha!" << std::endl;

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

                delete novoMonstro;
            }
        }
    }*/

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
                this->player.setPosition(sf::Vector2f(50, 50));
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
