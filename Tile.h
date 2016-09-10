//
//  Tile.h
//  sdev
//
//  Created by Darion McCoy on 2/6/14.
//  Copyright (c) 2014 D man. All rights reserved.
//

#ifndef __sdev__Tile__
#define __sdev__Tile__

#include <iostream>
#include <SFML/Graphics.hpp>
#include "Textures.h"

class World;

#ifndef __WORLD_H_INCLUDED__
#define __WORLD_H_INCLUDED__
#include "World.h"
#endif

namespace Level
{
    
    class Tile
    {
        
    public:
        
        Tile(sf::Color pixelcolor, int posX, int posY);
        virtual~Tile();
        virtual void Update();
        virtual void Draw(sf::RenderTarget& window);
        sf::Sprite tSprite;
        sf::Texture tTexture;
        int top = 0,bottom = 0,left = 0,right = 0;
        bool isDestroyed();
        bool inView();
        bool misDestroyed = false;
        sf::Clock clock;
        bool foreground = false;
        static sf::Vector2f* view;
        
    };
    
    class Background : public Tile{
        
        Background(sf::Color pixelcolor, int posX, int posY);
        ~Background();
        void Update();
        int* ptrx;
        int* ptry;
        void Draw(sf::RenderTarget& window);

        
        
    };
    
    class Platform : public Tile{
        
    public:
        Platform(sf::Color pixelcolor, int posX, int posY);
        ~Platform();
        void Update();
        void Draw(sf::RenderTarget& window);

        
    };
    
    class Mover : public Platform{
        
    public:
        Mover(sf::Color pixelcolor, int posX, int posY);
        ~Mover();
        void Update();
        void Draw(sf::RenderTarget& window);
        bool dir = true;
        
        
    };
}

#endif /* defined(__sdev__Tile__) */
