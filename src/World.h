#ifndef _WorldClass_
#define _WorldClass_

#include <cmath>

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
    vec2f mapOffset;

    sf::Texture tileTexture;
    sf::Sprite tile;

    vec2f tileSize;
    vec2f tileOffset;

    vec2f mouseOver;
    float colorModifier = 1.f;
public:
    World(const std::string& mapPath)
    {
        tileTexture.loadFromFile("assets/tiles/prototype/floor_E.png");

        tileSize.x = 256.f;
        tileSize.y = 149.f;

        tileOffset.x = tileTexture.getSize().x - tileSize.x;
        tileOffset.y = tileTexture.getSize().y - tileSize.y;

        this->loadMap(mapPath);

        tile.setTexture(tileTexture);
        //tile.setScale({0.75f, 0.75f});
        tileTexture.generateMipmap();
    }

    ~World()
    {

    }

    void update(unsigned long long time, vec2f mouse)
    {
        std::cout << mouse.x << ", " << mouse.y << std::endl;

        //this->mouseOver.x = (((mouse.x - this->mapOffset.x) / (this->tileSize.x/2.f) + (mouse.y) / (this->tileSize.y/2.f)) / 2.f);
        //this->mouseOver.y = (((mouse.y) / (this->tileSize.y/2.f) - (mouse.x - this->mapOffset.x) / (this->tileSize.x/2.f)) / 2.f);

        mouse.x -= this->mapOffset.x;
        mouse.x -= this->tileSize.x/2.f;

        mouse.x -= this->tileOffset.x;

        mouse.y -= this->mapOffset.y;
        mouse.y += this->tileSize.y/2.f;

        mouse.y -= this->tileOffset.y;

        this->mouseOver.x = mouse.x / this->tileSize.x + mouse.y / this->tileSize.y;
        this->mouseOver.y = mouse.y / this->tileSize.y - mouse.x / this->tileSize.x;

        std::cout << this->mouseOver.x << ", " << this->mouseOver.y << std::endl;

        this->colorModifier = 1-(static_cast<float>(std::sin(time*0.005)) + 1.f)/8.f;
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

        this->mapOffset.x = (static_cast<float>((this->map.size()-1)*this->tileSize.x) / 2.f) - (this->tileSize.x/2.f);
        this->mapOffset.y = 0;
    }

    void draw(sf::RenderWindow* window)
    {


        for(auto& line : this->map) {
            for(auto& column : line.second) {
                if(column.second == 0) {
                    vec2f iso;
                    iso.x = (static_cast<float>(line.first) - static_cast<float>(column.first)) * this->tileSize.x/2.f;
                    iso.y = (static_cast<float>(line.first) + static_cast<float>(column.first)) * this->tileSize.y/2.f;
                    tile.setPosition(this->mapOffset.x + iso.x, this->mapOffset.y + iso.y);

                    if(std::floor(this->mouseOver.x) == line.first && std::floor(this->mouseOver.y) == column.first) {
                        tile.setColor(sf::Color::Color(200*(this->colorModifier*0.5f), 200*(this->colorModifier*0.5f), 255*this->colorModifier));
                    } else {
                        tile.setColor(sf::Color::White);
                    }

                    window->draw(tile);
                }
            }
        }

    }
};

#endif //_WorldClass_
