//
//  Tile.cpp
//  sdev
//
//  Created by Darion McCoy on 2/6/14.
//  Copyright (c) 2014 D man. All rights reserved.
//

#include "Tile.h"

namespace Level
{
    
    sf::Vector2f *Tile::view;
    
    /// Constructors
    
    Tile::Tile(sf::Color pixelcolr, int posX, int posY)
    {
        
        
    }
    
    Platform::Platform(sf::Color pixelcolor, int posX, int posY) : Tile (pixelcolor, posX,posY){
        
        tSprite.setTexture((World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_tile")));
        tSprite.setPosition(posX,posY);
        
        top = tSprite.getPosition().y;
        bottom = tSprite.getPosition().y + tSprite.getScale().y;
        left = tSprite.getPosition().x;
        right = tSprite.getPosition().x + tSprite.getScale().x;
        
    }
    
    Mover::Mover(sf::Color pixelcolor, int posX, int posY) : Platform (pixelcolor, posX,posY){
        
        tSprite.setTexture((World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_tile0")));
        tSprite.setColor(sf::Color::Magenta);
        tSprite.setPosition(posX,posY);
        
    }
    
    /// Destructors
    
    Tile::~Tile(){
       
        
        
    }
    
    Platform::~Platform(){
        
        
        
    }
    
    Mover::~Mover(){
        
        
        
    }
    
    /// Update
    
    void Tile::Update(){
        
    }
    
    void Platform::Update(){
        
        
    }
    
    void Mover::Update(){
        
        
        if(dir == true){
            
            tSprite.move(+1,0);
        }
        
        else{
        
            tSprite.move(-1,0);
        }
        
        top = tSprite.getPosition().y;
        bottom = tSprite.getPosition().y + tSprite.getScale().y;
        left = tSprite.getPosition().x;
        right = tSprite.getPosition().x + tSprite.getScale().x;
        
        
    }
    
    //// Draw
    
    void Tile::Draw(sf::RenderTarget &window){
        
        //window.draw(tSprite);
        
    }
    
    void Platform::Draw(sf::RenderTarget &window){
        
        //window.draw(tSprite);
        
    }
    
    void Mover::Draw(sf::RenderTarget &window){
        
        window.draw(tSprite);
        
    }
    
    // Misc
    
    bool Tile::inView(){
        
        int viewLeft = view->x - 260;
        int viewWidth = view->x + 260;
        
        return ((!misDestroyed) && (tSprite.getPosition().x < viewWidth) && (tSprite.getPosition().x > viewLeft)  ? true : false);
        
    }
    

    
    
}