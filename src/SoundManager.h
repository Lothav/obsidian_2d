#ifndef _SoundManagerClass_
#define _SoundManagerClass_

#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/Music.hpp>

class SoundManager
{
private:
protected:

public:
    void playSound(const std::string &path, bool loop = false)
    {
        sf::SoundBuffer* buffer = new sf::SoundBuffer();

        if (!buffer->loadFromFile(path))
            return;

        sf::Sound* sound = new sf::Sound();
        sound->setBuffer(*buffer);
        sound->setLoop(loop);
        sound->play();
    }

    void playMusic(const std::string &path, bool loop = false)
    {
        sf::Music* music = new sf::Music();

        if (!music->openFromFile(path))
            return;

        music->setLoop(loop);
        music->play();
    }
};

#endif //_SoundManagerClass_
