#include <chrono>
#include <SFML/Graphics.hpp>
#include <cmath>

#include "Animation.h"

#define MS_PER_UPDATE 16

static unsigned long long getCurrentTime()
{
    return std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1);
}

void run()
{
    //Flush to stdout as soon as possible for debug;
    std::cout.setf( std::ios_base::unitbuf );

    auto window = new sf::RenderWindow(sf::VideoMode(1920, 1080), "Obsidian2D", sf::Style::Fullscreen, sf::ContextSettings(24,8,16));
    
    unsigned long long previous = getCurrentTime();
    unsigned long long lag = 0;

    std::vector<Animation::AnimationFrame> walkRight = Animation::getSequence("assets/characters/cowboy/walk/right", 31);
    std::vector<Animation::AnimationFrame> walkRightDown = Animation::getSequence("assets/characters/cowboy/walk/right_down", 31);
    std::vector<Animation::AnimationFrame> walkDown = Animation::getSequence("assets/characters/cowboy/walk/down", 31);
    std::vector<Animation::AnimationFrame> walkLeftDown = Animation::getSequence("assets/characters/cowboy/walk/left_down", 31);
    std::vector<Animation::AnimationFrame> walkLeft = Animation::getSequence("assets/characters/cowboy/walk/left", 31);
    std::vector<Animation::AnimationFrame> walkLeftUp = Animation::getSequence("assets/characters/cowboy/walk/left_up", 31);
    std::vector<Animation::AnimationFrame> walkUp = Animation::getSequence("assets/characters/cowboy/walk/up", 31);
    std::vector<Animation::AnimationFrame> walkRightUp = Animation::getSequence("assets/characters/cowboy/walk/right_up", 31);


    std::vector<Animation::AnimationFrame> standRight = Animation::getSequence("assets/characters/cowboy/stand/v2/right", 31);
    std::vector<Animation::AnimationFrame> standRightDown = Animation::getSequence("assets/characters/cowboy/stand/v2/right_down", 31);
    std::vector<Animation::AnimationFrame> standDown = Animation::getSequence("assets/characters/cowboy/stand/v2/down", 31);
    std::vector<Animation::AnimationFrame> standLeftDown = Animation::getSequence("assets/characters/cowboy/stand/v2/left_down", 31);
    std::vector<Animation::AnimationFrame> standLeft = Animation::getSequence("assets/characters/cowboy/stand/v2/left", 31);
    std::vector<Animation::AnimationFrame> standLeftUp = Animation::getSequence("assets/characters/cowboy/stand/v2/left_up", 31);
    std::vector<Animation::AnimationFrame> standUp = Animation::getSequence("assets/characters/cowboy/stand/v2/up", 31);
    std::vector<Animation::AnimationFrame> standRightUp = Animation::getSequence("assets/characters/cowboy/stand/v2/right_up", 31);

    std::vector<Animation::AnimationFrame>* currentAnimation = &walkRight;
    std::vector<Animation::AnimationFrame>* standAnimation = &standRight;
    unsigned int animationIndex = 0;

    bool running = true;
    sf::Sprite sprite;

    sf::Vector2f playerPosition;

    sf::Texture backgroundTexture;
    backgroundTexture.loadFromFile("assets/bg.png");
    sf::Sprite background;
    background.setTexture(backgroundTexture);
    background.setScale({3, 3});

    sf::Texture luizTexture;
    luizTexture.loadFromFile("assets/luiz.png");
    sf::Sprite luiz;
    luiz.setTexture(luizTexture);
    luiz.setPosition({1920/2, 1080/2});

    sf::Texture cratesTexture;
    cratesTexture.loadFromFile("assets/tiles/dungeon/woodenCrates_E.png");
    sf::Sprite crates;
    crates.setTexture(cratesTexture);

    sf::Texture groundTexture;
    groundTexture.loadFromFile("assets/tiles/dungeon/stone_E.png");
    groundTexture.setSmooth(1);
    sf::Sprite ground;
    ground.setTexture(groundTexture);

    int timeHack = 0;
    bool standing = false;
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
            } else if (event.type == sf::Event::KeyPressed) {

            }
        }

        while (lag >= MS_PER_UPDATE)
        {
            standing = false;
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
                currentAnimation = &walkRightDown;
                standAnimation   = &standRightDown;

                playerPosition.x += 2.f;
                playerPosition.y += 2.f;
            } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
                currentAnimation = &walkRightUp;
                standAnimation   = &standRightUp;
                playerPosition.x += 2.f;
                playerPosition.y -= 2.f;
            } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
                currentAnimation = &walkLeftDown;
                standAnimation   = &standLeftDown;
                playerPosition.x -= 2.f;
                playerPosition.y += 2.f;
            } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
                currentAnimation = &walkLeftUp;
                standAnimation   = &standLeftUp;
                playerPosition.x -= 2.f;
                playerPosition.y -= 2.f;
            } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                currentAnimation = &walkLeft;
                standAnimation   = &standLeft;
                playerPosition.x -= 3.f;
            } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
                currentAnimation = &walkUp;
                standAnimation   = &standUp;
                playerPosition.y -= 3.f;
            } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
                currentAnimation = &walkDown;
                standAnimation   = &standDown;
                playerPosition.y += 3.f;
            } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                currentAnimation = &walkRight;
                standAnimation   = &standRight;
                playerPosition.x += 3.f;
            } else {
                currentAnimation = standAnimation;
                standing = true;
            }



            lag -= MS_PER_UPDATE;
            //std::cout << "#";
            
            if(!standing || (standing && !(++timeHack%4)))
                animationIndex++;

            if(animationIndex > 30) {
                animationIndex = 0;
            }

            sprite.setTexture((*currentAnimation)[animationIndex].texture);
            background.move({-0.1f, -0.1f});
            moveDiff = static_cast<float>(sin(current * 0.005));
            luiz.setPosition({1920/2 + moveDiff*100.f, 1080/2 + moveDiff*40.f});

            if(sf::Keyboard::isKeyPressed(sf::Keyboard::K)) {
                if(sprite.getGlobalBounds().intersects(luiz.getGlobalBounds())) {
                    luiz.setColor(sf::Color(255, 150, 150));
                    playerPosition = {0, 0};
                }
            }
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


        sprite.setPosition(playerPosition);
        window->draw(sprite);

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::K)) {
            window->draw(luiz);
        }

        window->display();
        //std::cout << ".";
    }
}
