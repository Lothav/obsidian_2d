#ifndef _PositionableClass_
#define _PositionableClass_

#include "vec2f.h"

class Positionable
{
private:
    vec2f position;

protected:
    Positionable() {};
    ~Positionable() {}

public:

    void setPosition(const vec2f& position)
    {
        this->position = position;
    }

    void move(const vec2f& positionDelta)
    {
        this->position.x += positionDelta.x;
        this->position.y += positionDelta.y;
    }

    vec2f getPosition()
    {
        return this->position;
    }

};
#endif // _PositionableClass_