#ifndef _vec2fStruct_
#define _vec2fStruct_

struct vec2f
{
    float x = 0;
    float y = 0;

    vec2f(float x = 0, float y = 0)
    {
        this->x = x;
        this->y = y;
    }

    vec2f& operator+(const vec2f& other)
    {
        this->x += other.x;
        this->y += other.y;

        return *this;
    }

    vec2f& operator-(const vec2f& other)
    {
        this->x -= other.x;
        this->y -= other.y;

        return *this;
    }

    vec2f& operator*(const vec2f& other)
    {
        this->x *= other.x;
        this->y *= other.y;

        return *this;
    }

    vec2f& operator/(const vec2f& other)
    {
        this->x /= other.x;
        this->y /= other.y;

        return *this;
    }

    vec2f& operator*(float other)
    {
        this->x *= other;
        this->y *= other;

        return *this;
    }
};
#endif //_vec2fStruct_