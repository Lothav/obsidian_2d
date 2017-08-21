#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

namespace Animation
{
    struct AnimationFrame
    {
        sf::Texture texture;
    };

    std::vector<AnimationFrame> getSequence(std::string path, unsigned int frameCount)
    {
        std::vector<AnimationFrame> animation;

        for(unsigned int i = 0; i < frameCount; i++) {
            AnimationFrame frame;
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
            frame.texture.loadFromFile(_path);
            frame.texture.setSmooth(1);
            animation.push_back(frame);
        }

        return animation;
    }
};