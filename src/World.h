#ifndef _WorldClass_
#define _WorldClass_

#include <vector>
#include <string>

#include <iostream>
#include <fstream>
#include <sstream>

#include <iterator>

#include "vec2f.h"

template<typename Out>
void split(const std::string &s, char delim, Out result) {
    std::stringstream ss;
    ss.str(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        *(result++) = item;
    }
}


std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, std::back_inserter(elems));
    return elems;
}

class World
{
public:
        using Map = std::map<unsigned int, std::map<unsigned int, int>>;

private:
protected:
    Map map;

    sf::Texture tileTexture;
    sf::Sprite tile;
    vec2f tileSize;

public:
    World(const std::string& mapPath)
    {
        this->loadMap(mapPath);

        tileSize.x = 256.f;
        tileSize.y = 150.f;

        tileTexture.loadFromFile("assets/tile.png");

        tile.setTexture(tileTexture);
    }

    ~World()
    {

    }

    void loadMap(const std::string& mapPath)
    {
        std::vector<std::string> lines;
        std::ifstream file;
        file.open(mapPath);

        char line[512];
        unsigned int lineCount = 0;
        while(file.getline(line, 512)) {
            auto tiles = split(line, ',');

            unsigned int columnCount = 0;
            for(auto& tile : tiles) {
                this->map[lineCount][columnCount] = std::stoi(tile);
                columnCount++;
            }
            lineCount++;
        }
    }

    void draw(sf::RenderWindow* window)
    {
        float mapOffset = static_cast<float>((this->map.size()-1)*this->tileSize.x) / 2.f;

        for(auto& line : this->map) {
            for(auto& column : line.second) {
                if(column.second == 0) {
                    vec2f iso;
                    iso.x = (static_cast<float>(line.first) - static_cast<float>(column.first)) * this->tileSize.x/2.f;
                    iso.y = (static_cast<float>(line.first) + static_cast<float>(column.first)) * this->tileSize.y/2.f;
                    tile.setPosition(mapOffset + iso.x, iso.y);
                    window->draw(tile);
                }
            }
        }
    }
};

#endif //_WorldClass_
