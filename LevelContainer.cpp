//
//  LevelContainer.cpp
//  sdev
//
//  Created by Darion McCoy on 2/6/14.
//  Copyright (c) 2014 D man. All rights reserved.
//

#include "LevelContainer.h"
#include "TMXLoader/TMXLoader.h"

#define STATIC 0
#define FIXED 1
#define PARALLAX 2
#define REPEAT 3

namespace Level

{
    
    LevelContainer::LevelContainer()
    {
        
		//letterbox for fullscreen assets only

		letterBoxTop.setFillColor(sf::Color::Black);
		letterBoxBottom.setFillColor(sf::Color::Black);
		letterBoxTop.setSize(sf::Vector2f(580,100));
		letterBoxBottom.setSize(sf::Vector2f(580, 100));
		letterBoxTop.setOrigin(letterBoxTop.getSize().x / 2, letterBoxTop.getSize().y / 2);
		letterBoxBottom.setOrigin(letterBoxTop.getSize().x / 2, letterBoxTop.getSize().y / 2);

		//Sets the border for battle levels

		battleBorder.left = 16;
		battleBorder.top = 16;
		battleBorder.width = 448;
		battleBorder.height = 238;

    }
    
    void LevelContainer::CreateScene(){
        
        World::GetInstance()->CameraTarget = nullptr;
        int sceneType = World::GetInstance()->CurrentScene->mapType;
        std::string sceneName = World::GetInstance()->CurrentScene->name;
  
        
        if(sceneType == MENU){
            
            CreateMenu();
        }
        
        if(sceneType == MAP){
        
            ///// Level creation
            
            // Pass reference to tile pointer
            
            Tile::view = &view;
            
            // Loading the map reference image
            
            TMXLoader* loader = new TMXLoader();
            //loader->loadMap(sceneName, "C:/Users/Darion/Documents/Visual Studio 2015/Projects/SoulbabyPC/SoulbabyPC/levels/" + sceneName + ".tmx");
			loader->loadMapData(sceneName, levelMap.at("lvl_" + sceneName));

            sf::Vector2u mapvec = sf::Vector2u(loader->getMap(sceneName)->getWidth(),loader->getMap(sceneName)->getHeight());
            
            for(int i = 0; i < mapvec.y*16; i++){
                
                TileItem tempitem;
                tempitem.isPlatform = false;
                
                std::unique_ptr<std::vector<TileItem>> tempVec(new std::vector<TileItem>(mapvec.x*16,tempitem));
                
                levelArray.push_back(std::move(tempVec));
                
            }
            
            sf::Color pixelColor;
            std::vector<vector<unsigned int>> map = loader->getMap(sceneName)->getTileLayer("Walls")->getTileVector();
            
            // Create collison map
            
            for(int col = 0; col < mapvec.y; col++){
                
                for(int row = 0; row < mapvec.x; row++){
                    
                    if (map[col][row] != 0) AddTile(pixelColor,row*16,col*16);
                    
                    if (map[col][row] == 57){
                        
                        Entity::itemQueue flame;
                        flame.properties["PosX"] = to_string(((row*16)+3));
                        flame.properties["PosY"] = to_string(((col*16)-27));
                        flame.properties["itemType"] = "PropFlame";
                        World::GetInstance()->WorldScene.objectContainer->Queue.push_back(flame);
                        
                        
                    }
                    
                    
                }
                
            }
            
            std::vector<TMXObject> objectmap = loader->getMap(sceneName)->getObjectLayer("objects")->getObjectVector();
        
            for(std::vector<TMXObject>::iterator it = objectmap.begin(); it != objectmap.end();it++){
                
                Entity::itemQueue item;
                
                if(it->getType() == "Door"){
                    
                    
                    int doorPath = stoi(it->getProperty("Path").erase(0,it->getProperty("Path").find("_")+1));
                    //std::cout << "door path(" << doorPath << "): " << World::GetInstance()->GlobalMembers.levelsCompleted.at(doorPath) << " = " << NOTCOMPLETED << std::endl;
                    item.properties["PosX"] = to_string(it->getXposition());
                    item.properties["PosY"] = to_string(it->getYposition());
                    item.properties["Path"] = it->getProperty("Path");
                    
                    if(World::GetInstance()->GlobalMembers.levelsCompleted.at(doorPath) == NOTCOMPLETED){
                        
                        item.properties["itemType"] = it->getType();
                    }
                    
                    else if(World::GetInstance()->GlobalMembers.levelsCompleted.at(doorPath) == JUSTCOMPLETED){
                        
                        item.properties["itemType"] = "DoorDestroy";
                        World::GetInstance()->GlobalMembers.levelsCompleted.at(doorPath) = COMPLETED;
                        
                    }
                }

                
                
                else if(it->getType() == "Actor" && World::GetInstance()->GlobalMembers.gateKeepersSaved.at(stoi(it->getProperty("Name"))) >= 1){
                    
                    item.properties["PosX"] = to_string(it->getXposition());
                    item.properties["PosY"] = to_string(it->getYposition());
                    item.properties["Name"] = it->getProperty("Name");
                    item.properties["itemType"] = it->getType();
                    
                }
                
                else if (it->getType() == "Player"){

				
					if (World::GetInstance()->GlobalMembers.firstTimeInNexus == true) {

						Entity::itemQueue birth;
						birth.properties["itemType"] = "PlayerBirth";
						birth.properties["PosX"] = std::to_string(it->getXposition()+8);
						birth.properties["PosY"] = std::to_string(it->getYposition()+8);
						World::GetInstance()->WorldScene.objectContainer->Queue.push_back(birth);

					}

					else CreatePlayer(MAP,it->getXposition(),it->getYposition());
					Entity::itemQueue hud;
					hud.properties["itemType"] = "Hud";
					World::GetInstance()->WorldScene.objectContainer->Queue.push_back(hud);
					
                }
                
                World::GetInstance()->WorldScene.objectContainer->Queue.push_back(item);
            }
            
            // Create doors, actors, enemies, etc
            
            //store level limits for camera stop points
            
            lvlSize.x = mapvec.x*16;
            lvlSize.y = mapvec.y*16;
            

            // Create the guide (for interactions)

            Entity::itemQueue guide;
			guide.properties["itemType"] = "Guide";
			guide.properties["PosX"] = std::to_string(1);
			guide.properties["PosY"] = std::to_string(1);
			World::GetInstance()->WorldScene.objectContainer->Queue.push_back(guide);

			guide.properties["itemType"] = "QuakeManager";
			World::GetInstance()->WorldScene.objectContainer->Queue.push_back(guide);


           
          
            delete loader;
            
        }
        
        if(sceneType == ENCOUNTER){
             
            Entity::itemQueue enemyManager;
            enemyManager.properties["itemType"] = "LevelManager";
            World::GetInstance()->WorldScene.objectContainer->Queue.push_back(enemyManager);
            
            lvlSize.x = 480;
            lvlSize.y = 270;
            
            Entity::itemQueue player;
            player.properties["itemType"] = "BattlePlayer";
            player.properties["PosX"] = std::to_string(lvlSize.x/2);
            player.properties["PosY"] = std::to_string(lvlSize.y/2);
            World::GetInstance()->WorldScene.objectContainer->Queue.push_back(player);
            LevelBG[0].setTextureRect(sf::IntRect(0,0,0,0));
			CreateHud();


			Entity::itemQueue guide;
			guide.properties["itemType"] = "Guide";
			guide.properties["PosX"] = std::to_string(1);
			guide.properties["PosY"] = std::to_string(1);
			World::GetInstance()->WorldScene.objectContainer->Queue.push_back(guide);

        }

		if (sceneType == GAMESCENE) {


			Entity::itemQueue scene;
			scene.properties["itemType"] = "GameScene";
			World::GetInstance()->WorldScene.objectContainer->Queue.push_back(scene);

		}
        
        CreateBG();
        if(sceneName == "battle") World::GetInstance()->WorldScene.audioContainer.PlayMusic("mus_" + sceneName + std::to_string(World::GetInstance()->GlobalMembers.currentLevel));
		
		else if (sceneName == "map2_1") {

			if (World::GetInstance()->GlobalMembers.firstTimeInNexus == false) World::GetInstance()->WorldScene.audioContainer.PlayMusic("mus_" + sceneName);
			World::GetInstance()->GlobalMembers.firstTimeInNexus = false;
		}

		//else if (sceneName == "menu") World::GetInstance()->WorldScene.audioContainer.PlayMusic("mus_" + sceneName);

        else World::GetInstance()->WorldScene.audioContainer.music.stop();
        
    }
    
    void LevelContainer::CreateBoss(){
        
        //std::cout << "Creating Boss ..." << std::endl;
        Entity::itemQueue boss;
        boss.properties["itemType"] = "Boss";
        boss.properties["PosX"] = to_string(World::GetInstance()->WorldScene.levelContainer->lvlSize.x/2);
        boss.properties["PosY"] = to_string(World::GetInstance()->WorldScene.levelContainer->lvlSize.y/2);
        World::GetInstance()->WorldScene.objectContainer->Queue.push_back(boss);
        
    }
    
    void LevelContainer::CreatePlayer(int type,int posX, int posY){
        
        std::cout << "Creating Player ..." << std::endl;
        
        if(type == ENCOUNTER){
            
            Entity::itemQueue player;
            player.properties["itemType"] = "BattlePlayer";
            player.properties["PosX"] = to_string(lvlSize.x/2);
            player.properties["PosY"] = to_string(lvlSize.y/2);
            World::GetInstance()->WorldScene.objectContainer->Queue.push_back(player);
            
        }
        
        else{
            
            Entity::itemQueue player;
            player.properties["itemType"] = "Player";
            player.properties["PosX"] = to_string(posX);
            player.properties["PosY"] = to_string(posY);
            World::GetInstance()->WorldScene.objectContainer->Queue.push_back(player);       
            
        }
       
        
    }
    
    void LevelContainer::CreateEnemies(){
        
        
    }
    
    void LevelContainer::CreateHud(){
        
        //std::cout << "Creating Hud ..." << std::endl;
        Entity::itemQueue hud;
        hud.properties["itemType"] = "Hud";
        World::GetInstance()->WorldScene.objectContainer->Queue.push_back(hud);
        
    }
    
    void LevelContainer::CreateMenu(){
        
        //std::cout << "Creating Menu..." << std::endl;
        Entity::itemQueue menu;
        menu.properties["itemType"] = "Menu";
        World::GetInstance()->WorldScene.objectContainer->Queue.push_back(menu);
        
    }
    
    void LevelContainer::CreateBG(){

        LevelBG.clear();
        LevelBG.resize(5);
		if(World::GetInstance()->WorldScene.textureContainer.textureMap.count("tx_" + World::GetInstance()->CurrentScene->name + "_bg_" + std::to_string(0) + ""))  LevelBG[0].setTexture(World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_" + World::GetInstance()->CurrentScene->name + "_bg_" + std::to_string(0)));
		if (World::GetInstance()->WorldScene.textureContainer.textureMap.count("tx_" + World::GetInstance()->CurrentScene->name + "_bg_" + "top")) LevelBG[1].setTexture(World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_" + World::GetInstance()->CurrentScene->name + "_bg_" + "top"));
  
    }
    
    void LevelContainer::AddTile(sf::Color pixelcolor, int posX, int posY)
    {
        
        LvlIterator it = LvlContainer.begin();
        
        std::shared_ptr<Level::Tile> ptr(new Platform(pixelcolor,posX,posY));
        ptr->tSprite.setTextureRect(sf::IntRect(0, 0, 16, 16));
        LvlContainer.insert(it,std::move(ptr));
        
        std::shared_ptr<Level::Tile> ptr2(LvlContainer[0]);
        
        levelArray.operator [](posY/16)->operator [](posX/16).tilePtr = std::move(ptr2);
        
        levelArray.operator [](posY/16)->operator [](posX/16).isPlatform = true;
        
        if(levelArray.operator [](posY/16)->operator [](posX/16).tilePtr->view == NULL){
            
            levelArray.operator [](posY/16)->operator [](posX/16).tilePtr->view = &view;
            
        }
        
        
    }
    
    void LevelContainer::UpdateBackground(){
        
        // Iterates through the background vector, checking to see if a texture for the current level has been set.
        //if so, update the background with the correct code recognized by the position in the vector
        
        
        for(int layer = 0; layer != 5;layer++){
            
            if(LevelBG[layer].getTexture()){
                
                
                LevelBG[layer].rotate(0.1);
                
                float newScale = 0.06 * sin((clock() *0.000008) * 0.5 * M_PI) + 1;
                LevelBG[layer].setScale(newScale,newScale);
                 
                
                int viewPos = World::GetInstance()->viewPos.x;
                int roundview = round(viewPos); //round camera position (of player)
                
                if(layer == FIXED){
                    
                    //LevelBG[layer].setPosition((viewPos - 240),+25); // set position to exception of player (set bg to far left of screen)
                    
                }
                
                else if(layer == PARALLAX){
                    
                    int lvlBgpos = round(((roundview / 640) * 640));
                    LevelBG[layer].setPosition((lvlBgpos - 240),+25);
                    LevelBG[layer].move((roundview - lvlBgpos) * 0.25,0);
                    
                }
                
                else if(layer == REPEAT){

                    int lvlBgpos = round(((roundview / 480) * 480));  //round to the nearest size per screen
                    LevelBG[layer].setPosition((lvlBgpos - 240),+25); // set position to exception of player (set bg to far left of screen)
                    
                }
                
                
            }
            
        }
        
    }
    
    void LevelContainer::Update()
    {
        
        //UpdateBackground();
        
        if(World::GetInstance()->CurrentScene->mapType != MENU && LvlContainer.size() > 0){
            
                for (LvlIterator it = LvlContainer.begin(); it !=  LvlContainer.end(); it++)
                {
                    
                    if((*it)->inView()){
                        
                        (*it)->Update();
                        
                    }
                    
                }
            
        }
        
    }
    
    void LevelContainer::DrawBG(){
        
        if(World::GetInstance()->CurrentScene->mapType == MAP || World::GetInstance()->CurrentScene->mapType == GAMESCENE || World::GetInstance()->CurrentScene->mapType == MENU)  World::GetInstance()->DrawObject(LevelBG[0]);
        
    }
    
    void LevelContainer::DrawBGTop(){
        
        World::GetInstance()->DrawObject(LevelBG[1]);
        
    }

	void LevelContainer::DrawLetterBox() {

		if (World::GetInstance()->GlobalMembers.fullscreen == true) {

			letterBoxTop.setPosition(World::GetInstance()->Screen.getCenter().x,
									World::GetInstance()->Screen.getCenter().y - 50 - 135);

			letterBoxBottom.setPosition(World::GetInstance()->Screen.getCenter().x,
				World::GetInstance()->Screen.getCenter().y + 50 + 135);

			World::GetInstance()->DrawObject(letterBoxTop,"bypass");
			World::GetInstance()->DrawObject(letterBoxBottom,"bypass");

		}

	}

    
    void LevelContainer::DrawLevel()
    {
    
        DrawBG();
        
        if(LvlContainer.size() > 0)
        {
        
            for (LvlIterator it = LvlContainer.begin(); it !=  LvlContainer.end(); it++)
            {
                
                if((*it)->inView()){
                    
                    (*it)->Draw(*World::GetInstance()->windowWorld);
                    
                }
                
            }
            
        }

    }
    
    void LevelContainer::CheckCollisions(){
        
        if(World::GetInstance()->WorldScene.objectContainer->ObjectContainer.size() > 0)
        {
            
            for (ObjectIterator it = World::GetInstance()->WorldScene.objectContainer->ObjectContainer.begin(); it !=  World::GetInstance()->WorldScene.objectContainer->ObjectContainer.end(); it++)
            {
                
                (*it)->CheckLvlCollision();
                
            }
            
        }

        
        
        
        
    }
    
    
    void LevelContainer::CheckCollison(sf::Vector2f prevPos, sf::Vector2f currPos, Entity::Hit& hit){
        
        GetSurroundingTiles(prevPos,currPos,hit.objectSprite.getTextureRect());
        
        int spriteWidth = abs(hit.objectSprite.getTextureRect().width);
        int spriteHeight = hit.objectSprite.getTextureRect().height;
        int tileWidth = 16;
        
        if(collisionArray.size() > 0){
            
            for(collArrayIter it = collisionArray.begin(); it != collisionArray.end(); it++){
                
                if(IsOverlapping(currPos.x, currPos.x + spriteWidth, it->x,it->x + tileWidth) && IsOverlapping(currPos.y, currPos.y + spriteHeight, it->y,it->y + tileWidth) ){
    
                    DivideObject(*this,*it,16);
                    
                }
                
            }
            
        }
    
        
    }
    
    
    void LevelContainer::CheckCollison(sf::Vector2f prevPos, sf::Vector2f currPos, Entity::Particle& particle){
        
        if(!levelArray.empty()){
        
        GetSurroundingTiles(prevPos,currPos,particle.objectSprite.getTextureRect());
        
        int spriteWidth = abs(particle.objectSprite.getTextureRect().width);
        int spriteHeight = particle.objectSprite.getTextureRect().height;
        int tileWidth = 16;
        
        if(collisionArray.size() > 0){
            
            for(collArrayIter it = collisionArray.begin(); it != collisionArray.end(); it++){
                
                if(IsOverlapping(currPos.x, currPos.x + spriteWidth, it->x,it->x + tileWidth) && IsOverlapping(currPos.y, currPos.y + spriteHeight, it->y,it->y + tileWidth) ){
                    
                    // Check previous interesection of X axis only, if there is no interesct, we know the X axis needs to be cleared by this block on this axis
                    
                    if(!IsOverlapping(prevPos.x,prevPos.x + spriteWidth, it->x, it->x + tileWidth)){
                        
                        // If collision on left ...
                        
                        if(prevPos.x > currPos.x){
                            
                            currPos.x = it->x + tileWidth;
                            
                        }
                        
                        // If collision on right ...
                        
                        else if(prevPos.x < currPos.x){
                            
                            currPos.x = it->x - spriteWidth;
                            
                        }
                        
                        particle.vel.x = -particle.vel.x;
                        particle.objectSprite.setPosition(currPos.x,currPos.y);
                        
                    }
                    
                    // Check previous interesection of Y axis only, if there is no interesct, we know the Y axis needs to be cleared by this block on this axis
                    
                    if(!IsOverlapping(prevPos.y,prevPos.y + spriteHeight, it->y, it->y + tileWidth)){
                        
                        // If collision on top ...
                        
                        if(prevPos.y > currPos.y){
                            
                            currPos.y = it->y + tileWidth;
                            
                        }
                        
                        // If collision on bottom ...
                        
                        else if(prevPos.y < currPos.y){
                            
                            currPos.y = it->y - spriteHeight;
                            
                        }
                        
                        particle.vel.y = -particle.vel.y / 1.3;
                        particle.objectSprite.setPosition(currPos.x,currPos.y);
                        
                    }
                    
                    particle.vel.x = particle.vel.x * 0.5;
                    particle.gravity -= 0.1f;
                    
                    
                }
                
            }
            
        }
        }
        
    }
    
    void LevelContainer::CheckCollison(sf::Vector2f prevPos, sf::Vector2f currPos, Entity::Projectile& projectile){
       
        if(!levelArray.empty()){
        
            GetSurroundingTiles(prevPos,currPos,projectile.objectSprite.getTextureRect());
            
            int spriteWidth = abs(projectile.objectSprite.getTextureRect().width);
            int spriteHeight = projectile.objectSprite.getTextureRect().height;
            int tileWidth = 16;
            
            if(collisionArray.size() > 0){
                
                for(collArrayIter it = collisionArray.begin(); it != collisionArray.end(); it++){
                    
                    if(IsOverlapping(currPos.x, currPos.x + spriteWidth, it->x,it->x + tileWidth) && IsOverlapping(currPos.y, currPos.y + spriteHeight, it->y,it->y + tileWidth) ){
                       
                        projectile.misDestroyed = true;
                        
                    }
                    
                }
                
            }
            
        }
        
    }
    
    void LevelContainer::CheckCollison(sf::Vector2f prevPos, sf::Vector2f currPos, Entity::Passive& object){
        
        
        GetSurroundingTiles(prevPos,currPos,object.objectSprite.getTextureRect());
        
        int spriteWidth = abs(object.objectSprite.getTextureRect().width);
        int spriteHeight = object.objectSprite.getTextureRect().height;
        int tileWidth = 16;
        
        if(collisionArray.size() > 0){
            
            for(collArrayIter it = collisionArray.begin(); it != collisionArray.end(); it++){
                
                if(IsOverlapping(currPos.x, currPos.x + spriteWidth, it->x,it->x + tileWidth) && IsOverlapping(currPos.y, currPos.y + spriteHeight, it->y,it->y + tileWidth) ){
                    
                    
                    if(!IsOverlapping(prevPos.y,prevPos.y + spriteHeight, it->y, it->y + tileWidth)){
                        
                        // If collision on top ...
                        
                        if(prevPos.y > currPos.y){
                            
                            currPos.y = it->y + tileWidth;
                            
                        }
                        
                        // If collision on bottom ...
                        
                        else if(prevPos.y < currPos.y){
                            
                            currPos.y = it->y - spriteHeight;
                            
                        }
                        
                        object.vel.y = -object.vel.y / 1.3;
                        object.objectSprite.setPosition(currPos.x,currPos.y);
                        
                    }
                    
                    object.gravity -= 0.1f;
                    
                    
                }
                
            }
            
        }
        
    }
    
    void LevelContainer::CheckCollison(sf::Vector2f prevPos, sf::Vector2f currPos, Entity::Enemy& enemy){
        
        GetSurroundingTiles(prevPos,currPos,enemy.objectSprite.getTextureRect());
        
        int spriteWidth = abs(enemy.objectSprite.getTextureRect().width);
        int spriteHeight = enemy.objectSprite.getTextureRect().height;
        int tileWidth = 16;
        
        if(collisionArray.size() > 0){
            
            for(collArrayIter it = collisionArray.begin(); it != collisionArray.end(); it++){
                
                if(IsOverlapping(currPos.x, currPos.x + spriteWidth, it->x,it->x + tileWidth) && IsOverlapping(currPos.y, currPos.y + spriteHeight, it->y,it->y + tileWidth) ){
                    
                    // Check previous interesection of X axis only, if there is no interesct, we know the X axis needs to be cleared by this block on this axis
                    
                    if(!IsOverlapping(prevPos.x,prevPos.x + spriteWidth, it->x, it->x + tileWidth)){
                        
                        // If collision on left ...
                        
                        if(prevPos.x > currPos.x){
                            
                            //std::cout << "left collison" << std::endl;
                            currPos.x = it->x + tileWidth;
                            
                        }
                        
                        // If collision on right ...
                        
                        else if(prevPos.x < currPos.x){
                            
                            //std::cout << "right collison" << std::endl;
                            currPos.x = it->x - spriteWidth;
                            
                        }
                        
                        if(enemy.push == false){
                            
                            DivideObject(*this,*it,16);
                            
                        }
                        
                        
                    }
                    
                    // Check previous interesection of Y axis only, if there is no interesct, we know the Y axis needs to be cleared by this block on this axis
                    
                    if(!IsOverlapping(prevPos.y,prevPos.y + spriteHeight, it->y, it->y + tileWidth)){
                        
                        // If collision on top ...
                        
                        if(prevPos.y > currPos.y){
                            
                            currPos.y = it->y + tileWidth;
                            
                            }
                        
                        // If collision on bottom ...
                        
                        else if(prevPos.y < currPos.y){
                            
                            //std::cout << "bottom collision" << std::endl;
                            
                            currPos.y = it->y - spriteHeight;
                            
                            }
                            
                            //enemy.bttmcollision = true;
                        
                        }
                        
                        
                        enemy.vel.y = 0;
                        enemy.objectSprite.setPosition(currPos.x,currPos.y);
                    
                    }
                    
                    
                }
                
                
            }
        
        else{
            
            //enemy.bttmcollision = false;
        
        }
        
    }
    
    void LevelContainer::GetSurroundingTiles(sf::Vector2f prevPos, sf::Vector2f currPos, sf::IntRect spriteRect){
        
        collisionArray.clear();
        
        int tileSize = 16;
        
        // We need to get the absolute value of the sprite width since we are flipping the sprite for directions (i.e., avoid negative value)
        
        int spriteWidth = abs(spriteRect.width);
        int spriteHeight = spriteRect.height;
        
        int minposX = fmin(prevPos.x - spriteWidth/2,currPos.x - spriteWidth/2);
        int maxposX = fmax(prevPos.x + spriteWidth/2,currPos.x + spriteWidth/2);
        int minposY = fmin(prevPos.y - spriteHeight/2,currPos.y - spriteHeight/2);
        int maxposY = fmax(prevPos.y + spriteHeight/2,currPos.y + spriteHeight/2);
        
        // Defines bounding rectangle size and position of object position, before and after. Rounding to tile size
        
        boundingRect.width =  RoundUp(minposX - maxposX, tileSize) + (RoundUp(spriteWidth,tileSize) * 2);
        boundingRect.height =  RoundUp(minposY - maxposY, tileSize) + (RoundUp(spriteHeight,tileSize) * 2);
        boundingRect.left = RoundUp(minposX,tileSize) - (RoundUp(spriteWidth,tileSize));
        boundingRect.top = RoundUp(minposY,tileSize)  - (RoundUp(spriteHeight,tileSize));
        
        /* TO DRAW SURROUNDING TILES
        sf::RectangleShape tempshape(sf::Vector2f (boundingRect.width,boundingRect.height));
        tempshape.setPosition(boundingRect.left, boundingRect.top );
        tempshape.setFillColor(sf::Color::Color(255,255,255,30));
        shapes.push_back(tempshape);
        */
        
        
        // If selected tiles are outside of tile map vector, constrain rect inside canvas
        
        if(boundingRect.left < 0){
            
            boundingRect.left = 0;
            
        }
        
        else if(boundingRect.left > lvlSize.x){
            
            boundingRect.left = lvlSize.x;
            
        }
        
        if(boundingRect.width < 0){
            
            boundingRect.width = 0;
            
        }
        
        else if(boundingRect.width > lvlSize.x){
            
            boundingRect.width = lvlSize.x;
            
        }
        
        
        if(boundingRect.top > lvlSize.y){
            
            boundingRect.top = lvlSize.y;
            
        }
        
        if(boundingRect.top < 0){
            
            boundingRect.top = 0;
            
        }
        
        if(boundingRect.height > lvlSize.y){
            
            boundingRect.height = lvlSize.y;
            
        }
        
        if(boundingRect.height < 0){
            
            boundingRect.height = 0;
            
        }
        
        
        // Create vector of possible tiles from bounding rectangle
        
        
        for(int i = 0; i < boundingRect.width; i += tileSize){
            
            for(int p = 0; p < boundingRect.height; p += tileSize ){
                
                if((levelArray.operator []((boundingRect.top+(p))/tileSize)->operator []((boundingRect.left+(i))/tileSize).isPlatform == true)
                   && levelArray.operator []((boundingRect.top+(p))/tileSize)->operator []((boundingRect.left+(i))/tileSize).tilePtr != nullptr){
                
                collisionArray.push_back(sf::Vector2i(boundingRect.left + i, boundingRect.top + p));
                
                }
                
            }
            
        }
        
        
    }
    
    
    void LevelContainer::DivideObject(LevelContainer& level, sf::Vector2<int> tileRect, int tileWidth){
        

        level.levelArray.operator [](tileRect.y/tileWidth)->operator [](tileRect.x/tileWidth).isPlatform = false;
        level.levelArray.operator [](tileRect.y/tileWidth)->operator [](tileRect.x/tileWidth).tilePtr->misDestroyed = true;
        
        sf::IntRect tempRect = level.levelArray.operator [](tileRect.y/tileWidth)->operator [](tileRect.x/tileWidth).tilePtr->tSprite.getTextureRect();
        
        for(int i = 0; i < 2; i++){
            
            for(int p = 0; p < 2;p++){
                
                Entity::itemQueue item;
                item.properties["PosX"] = to_string(tileRect.x + (8*p));
                item.properties["PosY"] = to_string(tileRect.y + (8*i));
                //item.texturePtr = *level.levelArray.operator [](tileRect.y/tileWidth)->operator [](tileRect.x/tileWidth).tilePtr->tSprite.getTexture();
                //item.textureRect = sf::IntRect(tempRect.left + (8*p),tempRect.top + (8*i),8,8);;
                item.properties["itemType"] = "Gib";
                World::GetInstance()->WorldScene.objectContainer->Queue.push_back(std::move(item));
                
            }
            
        }
        
        World::GetInstance()->WorldScene.audioContainer.PlaySFX("sfx_break_2");
        World::GetInstance()->ScreenShake(20);
       
    }
    
    
    LevelContainer::~LevelContainer(){
        
        
        
    }
    
}