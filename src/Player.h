#ifndef _PlayerClass_
#define _PlayerClass_

#include "Position.h"
#include "Animation.h"

class Player
{
private:
protected:
    sf::Sprite sprite;

public:
    Animation animation;
    Position position;

    Player()
    {
        animation.setBasePath("assets/characters/cowboy");

        //Walk
        animation.loadSequence("walk/right");
        animation.loadSequence("walk/right_down");
        animation.loadSequence("walk/down");
        animation.loadSequence("walk/left_down");
        animation.loadSequence("walk/left");
        animation.loadSequence("walk/left_up");
        animation.loadSequence("walk/up");
        animation.loadSequence("walk/right_up");

        //stand
        animation.loadSequence("stand/v2/right", 0.3f);
        animation.loadSequence("stand/v2/right_down", 0.3f);
        animation.loadSequence("stand/v2/down", 0.3f);
        animation.loadSequence("stand/v2/left_down", 0.3f);
        animation.loadSequence("stand/v2/left", 0.3f);
        animation.loadSequence("stand/v2/left_up", 0.3f);
        animation.loadSequence("stand/v2/up", 0.3f);
        animation.loadSequence("stand/v2/right_up", 0.3f);

        animation.setAnimation("stand/v2/right_down");
    };

    ~Player() {};

    bool input(sf::Event event)
    {
        return false;
    }

    bool update(unsigned long long elapsedTime)
    {
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            this->position.move({2.f, 2.f});
            animation.setAnimation("walk/right_down");
        } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            this->position.move({2.f, -2.f});
            animation.setAnimation("walk/right_up");
        } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            this->position.move({-2.f, 2.f});
            animation.setAnimation("walk/left_down");
        } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            this->position.move({-2.f, -2.f});
            animation.setAnimation("walk/left_up");
        } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            this->position.move({-3.f, 0.f});
            animation.setAnimation("walk/left");
        } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            this->position.move({0.f, -3.f});
            animation.setAnimation("walk/up");
        } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            this->position.move({0.f, 3.f});
            animation.setAnimation("walk/down");
        } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            this->position.move({3.f, 0.f});
            animation.setAnimation("walk/right");
        } else {
            vec2f lastMove = this->position.getLastMove();
            if(lastMove.x > 0 && lastMove.y > 0) {
                animation.setAnimation("stand/v2/right_down");
            } else if(lastMove.x > 0 && lastMove.y < 0) {
                animation.setAnimation("stand/v2/right_up");
            } else if(lastMove.x < 0 && lastMove.y > 0) {
                animation.setAnimation("stand/v2/left_down");
            } else if(lastMove.x < 0 && lastMove.y < 0) {
                animation.setAnimation("stand/v2/left_up");
            } else if(lastMove.x < 0 ) {
                animation.setAnimation("stand/v2/left");
            } else if(lastMove.y < 0) {
                animation.setAnimation("stand/v2/up");
            } else if(lastMove.y > 0) {
                animation.setAnimation("stand/v2/down");
            } else if(lastMove.x > 0) {
                animation.setAnimation("stand/v2/right");
            }
        }

        animation.nextFrame();
        return false;
    }

    const sf::Sprite& getSprite()
    {
        this->sprite.setTexture(this->animation.getCurrentFrame());
        this->sprite.setPosition({this->position.getPosition().x, this->position.getPosition().y});
        return this->sprite;
    }
};

#endif // _PlayerClass_
