#ifndef _AnimationClass_
#define _AnimationClass_

#include <SFML/Graphics.hpp>
#include <string>
#include <map>

class Animation
{
private:
protected:
    std::map<std::string, std::vector<sf::Texture>> animations;
    uint8_t currentAnimationIndex = 0;

public:

    void loadSequence(std::string path, unsigned int frameCount)
    {
        std::vector<sf::Texture> animation;

        for(unsigned int i = 0; i < frameCount; i++) {
            sf::Texture texture;
            std::string _path = path;
            _path.append("/");
            if(i < 100) {
                _path.append("0");
            }
            if(i < 10) {
                _path.append("0");
            }
            _path.append(std::to_string(i));
            _path.append(".png");
            texture.loadFromFile(_path);
            texture.setSmooth(1);
            animation.push_back(texture);
        }

        this->animations[path] = animation;
    }
};

#endif //_AnimationClass_