#ifndef _AnimationClass_
#define _AnimationClass_

#include <SFML/Graphics.hpp>
#include <string>
#include <map>

class Animation
{
private:
protected:
    std::string basePath;

    std::map<std::string, std::vector<sf::Texture>> animations;
    std::map<std::string, float> animationsSpeedModifier;
    float currentAnimationIndex = 0;

    std::string currentAnimation;

public:

    void setBasePath(const std::string& basePath)
    {
        this->basePath = basePath;
    }

    void loadSequence(const std::string& path, float speedModifier = 1, unsigned int frameCount = 31)
    {
        std::vector<sf::Texture> animation;

        for(unsigned int i = 0; i < frameCount; i++) {
            sf::Texture texture;
            std::string _path = basePath;
            _path.append("/");
            _path.append(path);
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
        this->animationsSpeedModifier[path] = speedModifier;
    }

    void setAnimation(const std::string& key)
    {
        if(this->currentAnimation != key) {
            this->currentAnimation = key;
            this->currentAnimationIndex = 0;
        }
    }

    void nextFrame()
    {
        this->currentAnimationIndex += this->animationsSpeedModifier[this->currentAnimation];
        if(this->currentAnimationIndex >= this->animations[this->currentAnimation].size()) {
            this->currentAnimationIndex = 0;
        }
    }

    const sf::Texture& getCurrentFrame()
    {
        return this->animations[this->currentAnimation][static_cast<int>(this->currentAnimationIndex)];
    }
};

#endif //_AnimationClass_
