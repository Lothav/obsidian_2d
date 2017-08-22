#ifndef _SoundManagerClass_
#define _SoundManagerClass_

#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/Music.hpp>

#include <unordered_map>

class SoundManager
{
private:
    std::unordered_map<std::string, sf::SoundBuffer> soundBuffers;
    std::unordered_map<uint8_t, sf::Sound*> sounds;
    uint8_t soundId = 1;

protected:

public:
    SoundManager () {}

    ~SoundManager()
    {
        for (auto it = soundBuffers.begin(); it != soundBuffers.end(); soundBuffers.erase(it++)) {}
    }

    uint8_t playSound(const std::string& path, bool loop = false)
    {
        bool loaded = soundBuffers.count(path) > 0;
        sf::SoundBuffer& buffer = soundBuffers[path];

        if (!loaded)
        {
            if (!buffer.loadFromFile(path))
            {
                soundBuffers.erase(path);
                return 0;
            }
        }

        sf::Sound* sound = new sf::Sound();
        sound->setBuffer(buffer);
        sound->setLoop(loop);
        sound->play();

        uint8_t id = soundId++;
        sounds.insert({id, sound});

        return id;
    }

    bool stopSound(const uint8_t& id)
    {
        if (sounds.count(id) > 0)
        {
            sounds.at(id)->stop();
            return true;
        }

        return false;
    }

    void playMusic(const std::string& path, bool loop = false)
    {
        sf::Music* music = new sf::Music();

        if (!music->openFromFile(path))
            return;

        music->setLoop(loop);
        music->play();
    }
};

#endif //_SoundManagerClass_
