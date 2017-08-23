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

    std::unordered_map<uint8_t, sf::Music*> musics;
    uint8_t musicId = 1;

protected:

public:
    SoundManager () {}

    ~SoundManager()
    {
        for (auto it = sounds.begin(); it != sounds.end(); sounds.erase(it++)) {
            delete it->second;
        }

        for (auto it = musics.begin(); it != musics.end(); musics.erase(it++)) {
            delete it->second;
        }
    }

    uint8_t playSound(const std::string& path, bool loop = false)
    {
        bool loaded = soundBuffers.count(path) > 0;

        if (!loaded)
        {
            sf::SoundBuffer* buffer = new sf::SoundBuffer();
            if (!buffer->loadFromFile(path))
            {
                return 0;
            }

            soundBuffers.insert({path, *buffer});
        }

        sf::Sound* sound = new sf::Sound();
        sound->setBuffer(soundBuffers[path]);
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

    bool releaseSound(const uint8_t& id)
    {
        if (sounds.count(id) > 0)
        {
            sounds.at(id)->play();
            return true;
        }

        return false;
    }

    uint8_t playMusic(const std::string& path, bool loop = false)
    {
        sf::Music* music = new sf::Music();

        if (!music->openFromFile(path))
        {
            return 0;
        }

        music->setLoop(loop);
        music->play();

        uint8_t id = musicId++;
        musics.insert({id, music});

        return id;
    }

    bool stopMusic(const uint8_t& id)
    {
        if (musics.count(id) > 0)
        {
            musics.at(id)->stop();
            return true;
        }

        return false;
    }

    bool releaseMusic(const uint8_t& id)
    {
        if (musics.count(id) > 0)
        {
            musics.at(id)->play();
            return true;
        }

        return false;
    }
};

#endif //_SoundManagerClass_
