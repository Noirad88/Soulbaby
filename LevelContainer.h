//
//  LevelContainer.h
//  sdev
//
//  Created by Darion McCoy on 2/6/14.
//  Copyright (c) 2014 D man. All rights reserved.
//

#ifndef __sdev__LevelContainer__
#define __sdev__LevelContainer__


#include <iostream>
#include <SFML/Graphics.hpp>

#ifndef __CONTAINER_H_INCLUDED__
#define __CONTAINER_H_INCLUDED__
#include "Container.h"
#endif

#include <fstream>
#include <math.h>
#include <cstdlib>
#include <random>

#include <chrono>
#include <thread>
#include "Tile.h"

class World;



class Container;

namespace Entity{
    
    class Object;
    class Hud;
    class Particle;
    class Projectile;
    class Passive;
    class Hit;
    class Enemy;
    class Player;
}

class Hud;

static void* loadXMLFile( const char * filename, bool appendNull );

namespace Level
{
    
    class Tile;

    struct TileItem
    {
        TileItem()
        {
            
            
        }
        
        ~TileItem()
        {
            
            
        }
        
        bool IsNull();
        
        std::shared_ptr<Tile> tilePtr;
        bool isPlatform = false;
        
    };
    
    class LevelContainer
    {
        
        
    public:

        LevelContainer();
        ~LevelContainer();
        void CreateScene();
        void AddTile(sf::Color pixelcolor, int posX, int posY);
        void DrawBG();
        void DrawLevel();
        void DrawBGTop();
		void DrawLetterBox();
        void Update();
        void UpdateBackground();
        void CreateBoss();
        void CreatePlayer(int type = 0, int posX = 0, int posY = 0);
        void CreateEnemies();
        void CreateHud();
        void CreateBG();
        void CreateMenu();
        void SetMenu();
        void CheckCollisions();
        void CheckCollison(sf::Vector2f prevPos, sf::Vector2f currPos, Entity::Particle& particle);
        void CheckCollison(sf::Vector2f prevPos, sf::Vector2f currPos, Entity::Projectile& projectile);
        void CheckCollison(sf::Vector2f prevPos, sf::Vector2f currPos, Entity::Passive& object);
        void CheckCollison(sf::Vector2f prevPos, sf::Vector2f currPos, Entity::Hit& hit);
        void CheckCollison(sf::Vector2f prevPos, sf::Vector2f currPos, Entity::Enemy& enemy);
        void CheckCollison(const std::unique_ptr<Entity::Object>& object);
        void GetSurroundingTiles(sf::Vector2f prevPos, sf::Vector2f currPos, sf::IntRect spriteRect);
        std::vector<sf::Sprite> LevelBG;
		sf::RectangleShape letterBoxTop;
		sf::RectangleShape letterBoxBottom;
        sf::Texture prevImg;
        sf::Vector2f lvlSize;
        std::vector<std::shared_ptr<Tile>> LvlContainer;
        sf::Vector2f view;
        std::vector<std::shared_ptr< std::vector<TileItem>> > levelArray;
		std::map<std::string, std::string> levelMap;

        
        sf::IntRect boundingRect;
        sf::RectangleShape recShape;
        std::vector<sf::Vector2i> collisionArray;
        std::vector<sf::RectangleShape> shapes;

        sf::Sprite menu;
        
        void DivideObject(LevelContainer& level, sf::Vector2i tileRect, int tileWidth = 16);
    
        
    };


typedef std::vector<std::shared_ptr<Level::Tile>>::iterator LvlIterator;
    typedef std::vector<sf::Vector2i>::iterator collArrayIter;
    typedef std::vector<sf::RectangleShape>::iterator shapeIter;

}

#endif /* defined(__sdev__LevelContainer__) */