#ifndef _PositionableClass_
#define _PositionableClass_

#include "vec2f.h"

class Positionable
{
private:

protected:
    vec2f position;
    vec2f lastMove;

    Positionable() {};
    ~Positionable() {}

public:

    void setPosition(const vec2f& position)
    {
        this->position = position;
    }

    void move(const vec2f& positionDelta)
    {
        this->lastMove = positionDelta;

        this->position.x += positionDelta.x;
        this->position.y += positionDelta.y;
    }

    const vec2f& getPosition()
    {
        return this->position;
    }

    const vec2f& getLastMove()
    {
        return this->lastMove;
    }

};
#endif // _PositionableClass_
