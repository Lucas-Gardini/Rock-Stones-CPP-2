#include <iostream>
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>
#include "modules/.env.h"
#include "modules/Personagem.cpp"

class Window {
public:
    bool createWindow()
    {
        Personagem player;
        Personagem npc;

        int npcCounter = 0;

        // Set a seed for random number generation
        std::srand(static_cast<unsigned int>(std::time(nullptr)));

        // Create the window
        sf::RenderWindow window(sf::VideoMode(800, 600), "Deep Rock Galactic");

        // run the program as long as the window is open
        while (window.isOpen())
        {
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
                    switch (event.key.code)
                    {
                    case sf::Keyboard::Left:
                        player.animateWalk(ESQUERDA);
                        player.move(sf::Vector2f(-24.0f, 0.0f), window.getSize());
                        break;
                    case sf::Keyboard::Right:
                        player.animateWalk(DIREITA);
                        player.move(sf::Vector2f(24.0f, 0.0f), window.getSize());
                        break;
                    case sf::Keyboard::Up:
                        player.animateWalk(CIMA);
                        player.move(sf::Vector2f(0.0f, -24.0f), window.getSize());
                        break;
                    case sf::Keyboard::Down:
                        player.animateWalk(BAIXO);
                        player.move(sf::Vector2f(0.0f, 24.0f), window.getSize());
                        break;
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

            // Draw the background
            window.draw(player.getCurrentSprite());
            window.draw(npc.getCurrentSprite());

            // end the current frame
            window.display();
        }

        return true;
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
