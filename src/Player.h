#ifndef _PlayerClass_
#define _PlayerClass_

#include "Positionable.h"
#include "Animation.h"

class Player : public Positionable
{
private:
protected:
    Animation animation;
public:
    Player()
    {
        //Walk
        animation.loadSequence("assets/characters/cowboy/walk/right", 31);
        animation.loadSequence("assets/characters/cowboy/walk/right_down", 31);
        animation.loadSequence("assets/characters/cowboy/walk/down", 31);
        animation.loadSequence("assets/characters/cowboy/walk/left_down", 31);
        animation.loadSequence("assets/characters/cowboy/walk/left", 31);
        animation.loadSequence("assets/characters/cowboy/walk/left_up", 31);
        animation.loadSequence("assets/characters/cowboy/walk/up", 31);
        animation.loadSequence("assets/characters/cowboy/walk/right_up", 31);

        //stand
        animation.loadSequence("assets/characters/cowboy/stand/v2/right", 31);
        animation.loadSequence("assets/characters/cowboy/stand/v2/right_down", 31);
        animation.loadSequence("assets/characters/cowboy/stand/v2/down", 31);
        animation.loadSequence("assets/characters/cowboy/stand/v2/left_down", 31);
        animation.loadSequence("assets/characters/cowboy/stand/v2/left", 31);
        animation.loadSequence("assets/characters/cowboy/stand/v2/left_up", 31);
        animation.loadSequence("assets/characters/cowboy/stand/v2/up", 31);
        animation.loadSequence("assets/characters/cowboy/stand/v2/right_up", 31);
    };
    ~Player() {};

    bool input(sf::Event event)
    {
        return false;
    }

    bool update(unsigned long long elapsedTime)
    {
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            player.move({2.f, 2.f});
        } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            player.move({2.f, -2.f});
        } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            player.move({-2.f, 2.f});
        } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            player.move({-2.f, -2.f});
        } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            player.move({3.f, 0.f});
        } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            player.move({0.f, -3.f});
        } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            player.move({0.f, 3.f});
        } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            player.move({3.f, 0.f});
        }
    }
};

#endif // _PlayerClass_