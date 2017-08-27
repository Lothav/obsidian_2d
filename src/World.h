#ifndef _WorldClass_
#define _WorldClass_

#include <cmath>

#include <vector>
#include <string>

#include <iostream>
#include <fstream>
#include <sstream>

#include <iterator>

#include <nlohmann/json.hpp>

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
        using Map = std::map<unsigned int, std::map<unsigned int, std::map<unsigned int, int>>>;

private:
protected:
    Map map;
    vec2f mapOffset;

    std::map<unsigned int, sf::Texture> tiles;

    sf::Sprite tile;

    vec2f tileSize;
    vec2f tileOffset;

    vec2f mouseOver;
    float colorModifier = 1.f;
public:
    World(const std::string& mapPath)
    {

        sf::Texture* tileTexture = new sf::Texture();
        tileTexture->loadFromFile("assets/tiles/prototype/floor_E.png");

        this->tileSize.x = 256.f*0.75f;
        this->tileSize.y = 149.f*0.75f;

        this->tileOffset.x = tileTexture->getSize().x*0.75f - tileSize.x;
        this->tileOffset.y = tileTexture->getSize().y*0.75f - tileSize.y;

        this->loadMap(mapPath);

        this->tile.setTexture(*tileTexture);
        tile.setScale({0.75f, 0.75f});
    }

    ~World()
    {

    }

    void update(unsigned long long time, vec2f mouse)
    {
        mouse.x -= this->mapOffset.x;
        mouse.x -= this->tileSize.x/2.f;

        mouse.x -= this->tileOffset.x;

        mouse.y -= this->mapOffset.y;
        mouse.y += this->tileSize.y/2.f;

        mouse.y -= this->tileOffset.y;

        this->mouseOver.x = mouse.x / this->tileSize.x + mouse.y / this->tileSize.y;
        this->mouseOver.y = mouse.y / this->tileSize.y - mouse.x / this->tileSize.x;

        this->colorModifier = 1-(static_cast<float>(std::sin(time*0.005)) + 1.f)/8.f;
    }

    void loadMap(const std::string& mapPath)
    {
        std::ifstream file(mapPath);
        nlohmann::json json;
        file >> json;

        unsigned int layerCount = 0;
        for(auto& layer : json.at("layers"))
        {
            unsigned int mapWidth = layer.at("width");
            unsigned int mapHeight = layer.at("height");
            for(unsigned int i = 0; i < mapWidth; i++) {
                for(unsigned int j = 0; j < mapHeight; j++) {
                    int tileId = layer.at("data").at(i*mapWidth + j);

                    this->map[layerCount][j][i] = tileId;
                    if(tileId > 0 && !this->tiles.count(tileId)) {
                        if(json.at("tilesets").at(0).at("tiles").count(std::to_string(tileId))) {
                            std::string imagePath = json.at("tilesets").at(0).at("tiles").at(std::to_string(tileId)).at("image");
                            this->tiles[tileId].loadFromFile("assets/maps/" + imagePath);
                        }
                    }
                }
            }
            layerCount++;
        }


        this->mapOffset.x = (static_cast<float>((this->map[0].size())*this->tileSize.x) / 2.f) - (this->tileSize.x/2.f);
        this->mapOffset.y = 0;
    }

    void draw(sf::RenderWindow* window)
    {

        for(auto& layer : this->map)
        {
            for(auto& line : layer.second)
            {
                for(auto& column : line.second)
                {
                    if(column.second > 0) {
                        vec2f iso;
                        iso.x = (static_cast<float>(line.first) - static_cast<float>(column.first)) * this->tileSize.x/2.f;
                        iso.y = (static_cast<float>(line.first) + static_cast<float>(column.first)) * this->tileSize.y/2.f;

                        tile.setPosition(this->mapOffset.x + iso.x, this->mapOffset.y + iso.y);
                        tile.setTexture(this->tiles[column.second]);

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
    }
};

#endif //_WorldClass_
