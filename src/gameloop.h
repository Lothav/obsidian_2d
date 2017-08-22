#include <chrono>
#include <SFML/Graphics.hpp>
#include <cmath>
#include "Player.h"

#define MS_PER_UPDATE 16

static unsigned long long getCurrentTime()
{
    return std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1);
}

void run()
{
    auto window = new sf::RenderWindow(sf::VideoMode(1280, 720), "Obsidian2D", sf::Style::Default, sf::ContextSettings(24,8,16));
    
    unsigned long long previous = getCurrentTime();
    unsigned long long lag = 0;

    bool running = true;

    Player player;

    sf::Texture backgroundTexture;
    backgroundTexture.loadFromFile("assets/bg.png");
    sf::Sprite background;
    background.setTexture(backgroundTexture);
    background.setScale({3, 3});

    sf::Texture cratesTexture;
    cratesTexture.loadFromFile("assets/tiles/dungeon/woodenCrates_E.png");
    sf::Sprite crates;
    crates.setTexture(cratesTexture);

    sf::Texture groundTexture;
    groundTexture.loadFromFile("assets/tiles/dungeon/stone_E.png");
    groundTexture.setSmooth(1);
    sf::Sprite ground;
    ground.setTexture(groundTexture);

    float moveDiff = 0;

    while (running)
    {
        unsigned long long current = getCurrentTime();
        unsigned long long elapsed = current - previous;
        previous = current;
        lag += elapsed;

        sf::Event event;
        while (window->pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                window->close();
                running = false;
            }

            player.input(event);
        }

        while (lag >= MS_PER_UPDATE)
        {
            background.move({-0.1f, -0.1f});
            moveDiff = static_cast<float>(sin(current * 0.005));

            player.update(elapsed);

            lag -= MS_PER_UPDATE;
        }

        window->draw(background);

        ground.setPosition({128*7, -128*2});
        ground.move({0, 0 + (moveDiff * 15.f)});
        window->draw(ground);

        crates.setPosition(ground.getPosition());
        window->draw(crates);


        ground.move({-128, 75 + (moveDiff * 15.f)});
        window->draw(ground);
        ground.move({-128, 75 + (moveDiff * 15.f)});
        window->draw(ground);

        ground.setPosition({0, -300});
        ground.move({0, 0});
        window->draw(ground);
        ground.move({128, 75});
        window->draw(ground);
        ground.move({128, 75});
        window->draw(ground);
        ground.move({128, 75});
        window->draw(ground);
        ground.move({128, 75});
        window->draw(ground);
        ground.move({128, 75});
        window->draw(ground);
        ground.move({128, 75});
        window->draw(ground);
        ground.move({128, 75});
        window->draw(ground);
        ground.move({128, 75});
        window->draw(ground);
        ground.move({128, 75});
        window->draw(ground);

        ground.setPosition({-128, -(75*3)});
        ground.move({0, 0});
        window->draw(ground);
        ground.move({128, 75});
        window->draw(ground);
        ground.move({128, 75});
        window->draw(ground);
        ground.move({128, 75});
        window->draw(ground);
        ground.move({128, 75});
        window->draw(ground);
        ground.move({128, 75});
        window->draw(ground);
        ground.move({128, 75});
        window->draw(ground);
        ground.move({128, 75});
        window->draw(ground);
        ground.move({128, 75});
        window->draw(ground);
        ground.move({128, 75});
        window->draw(ground);

        window->display();
    }
}
