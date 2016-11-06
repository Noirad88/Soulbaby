//
//  Container.cpp
//  sdev
//
//  Created by Darion McCoy on 1/11/14.
//  Copyright (c) 2014 D man. All rights reserved.
//

#include "ResourcePath.hpp"
#include "Container.h"



Container::Container()
{
    
    posX = 400;
    posY = 200;
    SpawnPoint.move(posX,posY);
    std::cout << "Spawn Point created at " << posX << ", " << posY << std::endl;
    
}

bool normalSort (int i,int j) { return (i<j); }

void Container::AddObjects()
{
    
    if(Queue.size() > 0)
    {
        
        for(QueueIterator QI = Queue.begin(); QI != Queue.end(); QI++)
        {
            
            
            // GUI & ETC
            
            if((QI)->properties["itemType"] == "Menu")
            {
                
                std::unique_ptr<Entity::Menu> ptr(new Entity::Menu);
                ObjectContainer.push_back(std::move(ptr));
                
            }
            
            else if((QI)->properties["itemType"] == "Hud")
            {
                
                std::unique_ptr<Entity::Hud> ptr(new Entity::Hud);
                ObjectContainer.push_back(std::move(ptr));
                
            }
            
            else if((QI)->properties["itemType"] == "Textbox")
            {
                
                std::unique_ptr<Entity::Textbox> ptr(new Entity::Textbox(stoi((QI)->properties["ActorName"])));
                ObjectContainer.push_back(std::move(ptr));
                
            }
            
            else if((QI)->properties["itemType"] == "PlayerMenu")
            {
                
                std::unique_ptr<Entity::PlayerMenu> ptr(new Entity::PlayerMenu);
                ObjectContainer.push_back(std::move(ptr));
                
            }
            
            else if((QI)->properties["itemType"] == "BattleBackground")
            {
                
                std::unique_ptr<Entity::BattleBackground> ptr(new Entity::BattleBackground);
                ObjectContainer.push_back(std::move(ptr));
                
            }
            
            else if((QI)->properties["itemType"] == "LevelManager")
            {
                
                std::unique_ptr<Entity::LevelManager> ptr(new Entity::LevelManager);
                ObjectContainer.push_back(std::move(ptr));
                
            }
            
            // Effects
        
            
            
            else if((QI)->properties["itemType"] == "DamageSpark")
            {
                
                std::unique_ptr<Entity::DamageSpark> ptr(new Entity::DamageSpark);
                ptr->objectSprite.setPosition(stoi((QI)->properties["PosX"]),stoi((QI)->properties["PosY"]));
                ObjectContainer.push_back(std::move(ptr));
                
            }

			else if ((QI)->properties["itemType"] == "DashEffect")
			{

				std::unique_ptr<Entity::DashEffect> ptr(new Entity::DashEffect);
				ptr->objectSprite.setPosition(stoi((QI)->properties["PosX"]), stoi((QI)->properties["PosY"]));
				ObjectContainer.push_back(std::move(ptr));

			}
            
            else if((QI)->properties["itemType"] == "DeathBoom")
            {
                
                std::unique_ptr<Entity::DeathBoom> ptr(new Entity::DeathBoom);
                ptr->objectSprite.setPosition(stoi((QI)->properties["PosX"]),stoi((QI)->properties["PosY"]));
                ObjectContainer.push_back(std::move(ptr));
                
            }
            
            else if((QI)->properties["itemType"] == "PlayerBomb")
            {
                
                std::unique_ptr<Entity::PlayerBomb> ptr(new Entity::PlayerBomb);
                ptr->objectSprite.setPosition(stoi((QI)->properties["PosX"]),stoi((QI)->properties["PosY"]));
                RotateVector(ptr->vel,45*(stoi((QI)->properties["Direction"])));
                ObjectContainer.push_back(std::move(ptr));
                
            }
            
            else if((QI)->properties["itemType"] == "Spark")
            {
                
                std::unique_ptr<Entity::Spark> ptr(new Entity::Spark);
                ptr->objectSprite.setPosition(stoi((QI)->properties["PosX"]),stoi((QI)->properties["PosY"]));
                ObjectContainer.push_back(std::move(ptr));
                
            }
            
            else if((QI)->properties["itemType"] == "DeathPoof")
            {
                
                std::unique_ptr<Entity::DeathPoof> ptr(new Entity::DeathPoof);
                ptr->objectSprite.setPosition(stoi((QI)->properties["PosX"]),stoi((QI)->properties["PosY"]));
                ObjectContainer.push_back(std::move(ptr));
                
            }

			else if ((QI)->properties["itemType"] == "EnemySpark")
			{

				std::unique_ptr<Entity::EnemySpark> ptr(new Entity::EnemySpark);
				ptr->objectSprite.setPosition(stoi((QI)->properties["PosX"]), stoi((QI)->properties["PosY"]));
				ObjectContainer.push_back(std::move(ptr));

			}

			else if ((QI)->properties["itemType"] == "HauzerCharge")
			{

				std::unique_ptr<Entity::HauzerCharge> ptr(new Entity::HauzerCharge);
				ptr->objectSprite.setPosition(stoi((QI)->properties["PosX"]), stoi((QI)->properties["PosY"]));
				ObjectContainer.push_back(std::move(ptr));

			}
            
            else if((QI)->properties["itemType"] == "DoorDestroy")
            {
                
                std::unique_ptr<Entity::DoorDestroy> ptr(new Entity::DoorDestroy);
                ptr->objectSprite.setPosition(stoi((QI)->properties["PosX"]),stoi((QI)->properties["PosY"]));
                ObjectContainer.push_back(std::move(ptr));
                
            }
            
            else if((QI)->properties["itemType"] == "BigExp")
            {
                
                std::unique_ptr<Entity::BigExp> ptr(new Entity::BigExp);
                ptr->objectSprite.setPosition(stoi((QI)->properties["PosX"]),stoi((QI)->properties["PosY"]));
                ObjectContainer.push_back(std::move(ptr));
                
            }
            
            else if((QI)->properties["itemType"] == "Dirt"){
                
                std::unique_ptr<Entity::Dirt> ptr(new Entity::Dirt);
                ptr->objectSprite.setPosition(stoi((QI)->properties["PosX"]) + (rand() % 4 + -2),stoi((QI)->properties["PosY"]));
                ptr->vel.x = rand() % 6 + -3;
                ptr->vel.y = rand() % 6 + -12;
                ObjectContainer.push_back(std::move(ptr));
                
                
            }
            
            
            else if((QI)->properties["itemType"] == "EnemyBlip")
            {
                
                std::unique_ptr<Entity::EnemyBlip> ptr(new Entity::EnemyBlip);
                ptr->objectSprite.setPosition(stoi((QI)->properties["PosX"]),stoi((QI)->properties["PosY"]));
                if((QI)->properties.count("Speed")>0) ptr->vel.y = stoi((QI)->properties["Speed"]);
                
                // multiplying direction by 45 degrees gets us the direction player is shooting
                
                ptr->vel.y = -ptr->vel.y;
                if((QI)->properties.count("Direction"))RotateVector(ptr->vel,(-stoi((QI)->properties["Direction"])));
                ObjectContainer.push_back(std::move(ptr));

            }

			else if ((QI)->properties["itemType"] == "HauzerSmog")
			{

				std::unique_ptr<Entity::HauzerSmog> ptr(new Entity::HauzerSmog);
				ptr->objectSprite.setPosition(stoi((QI)->properties["PosX"]), stoi((QI)->properties["PosY"]));
				if ((QI)->properties.count("Speed")>0) ptr->vel.y = stoi((QI)->properties["Speed"]);

				// multiplying direction by 45 degrees gets us the direction player is shooting

				ptr->vel.y = -ptr->vel.y;
				if ((QI)->properties.count("Direction"))RotateVector(ptr->vel, (-stoi((QI)->properties["Direction"])));
				ObjectContainer.push_back(std::move(ptr));

			}

			else if ((QI)->properties["itemType"] == "HauzerSpear")
			{

				std::unique_ptr<Entity::HauzerSpear> ptr(new Entity::HauzerSpear);
				ptr->objectSprite.setPosition(stoi((QI)->properties["PosX"]), stoi((QI)->properties["PosY"]));
				if ((QI)->properties.count("Speed")>0) ptr->vel.y = stoi((QI)->properties["Speed"]);

				// multiplying direction by 45 degrees gets us the direction player is shooting

				ptr->vel = -ptr->vel;

				if ((QI)->properties.count("Direction")) RotateVector(ptr->vel, (-stoi((QI)->properties["Direction"])));
				ptr->objectSprite.setRotation((-stoi((QI)->properties["Direction"]))+180);
				ObjectContainer.push_back(std::move(ptr));

			}
            
            else if((QI)->properties["itemType"] == "PlayerLaser")
            {

                    std::unique_ptr<Entity::PlayerLaser> ptr(new Entity::PlayerLaser);
                    ptr->objectSprite.setPosition(stoi((QI)->properties["PosX"]),stoi((QI)->properties["PosY"]));
                    
                    // multiplying direction by 45 degrees gets us the direction player is shooting
                    
                    RotateVector(ptr->vel,45*(stoi((QI)->properties["Direction"])));
                    ptr->objectSprite.setRotation(45*(stoi((QI)->properties["Direction"])));
                    ObjectContainer.push_back(std::move(ptr));
                
            }
            
            else if((QI)->properties["itemType"] == "PlayerBoomerang")
            {
                
                std::unique_ptr<Entity::PlayerBoomerang> ptr(new Entity::PlayerBoomerang);
                ptr->objectSprite.setPosition(stoi((QI)->properties["PosX"]),stoi((QI)->properties["PosY"]));
                
                // multiplying direction by 45 degrees gets us the direction player is shooting
                
                RotateVector(ptr->vel,45*(stoi((QI)->properties["Direction"])));
                ptr->objectSprite.setRotation(45*(stoi((QI)->properties["Direction"])));
                ObjectContainer.push_back(std::move(ptr));
                
            }
            
            else if((QI)->properties["itemType"] == "PlayerBeam")
            {
                
                std::unique_ptr<Entity::PlayerBeam> ptr(new Entity::PlayerBeam);
                ptr->shaft.setPosition(stoi((QI)->properties["PosX"]),stoi((QI)->properties["PosY"]));
                
                // multiplying direction by 45 degrees gets us the direction player is shooting
                
                RotateVector(ptr->vel,45*(stoi((QI)->properties["Direction"])));
                ptr->shaft.setRotation(45*(stoi((QI)->properties["Direction"])));
                ObjectContainer.push_back(std::move(ptr));
                
            }
            
            else if((QI)->properties["itemType"] == "SpriteClone")
            {
                
                std::unique_ptr<Entity::SpriteClone> ptr(new Entity::SpriteClone);
                ptr->objectSprite.setPosition(stoi((QI)->properties["PosX"]),stoi((QI)->properties["PosY"]));
                ptr->objectSprite.setTexture(World::GetInstance()->WorldScene.textureContainer.SetTexture((QI)->properties["Sprite"]));
                ptr->objectSprite.setTextureRect(sf::IntRect(stoi((QI)->properties["RectX"]),stoi((QI)->properties["RectY"]),stoi((QI)->properties["RectWidth"]),stoi((QI)->properties["RectHeight"])));
                ptr->objectSprite.setOrigin(stoi((QI)->properties["SpriteOriginX"]),stoi((QI)->properties["SpriteOriginY"]));
				if (((QI)->properties["Color"]) == "Red") ptr->objectSprite.setColor(sf::Color::Red);
                ObjectContainer.push_back(std::move(ptr));
                
            }
            
            else if((QI)->properties["itemType"] == "PlayerRepeater")
            {
                
                std::unique_ptr<Entity::PlayerRepeater> ptr(new Entity::PlayerRepeater);
                ptr->objectSprite.setPosition(stoi((QI)->properties["PosX"]),stoi((QI)->properties["PosY"]));
                RotateVector(ptr->vel,(stoi((QI)->properties["Direction"])));
                ObjectContainer.push_back(std::move(ptr));
                
            }
            
            else if((QI)->properties["itemType"] == "PlayerBombExp")
            {
                
                std::unique_ptr<Entity::PlayerBombExp> ptr(new Entity::PlayerBombExp);
                ptr->objectSprite.setPosition(stoi((QI)->properties["PosX"]),stoi((QI)->properties["PosY"]));
                ObjectContainer.push_back(std::move(ptr));
                
            }
            
            else if((QI)->properties["itemType"] == "Hit")
            {
                
                std::unique_ptr<Entity::Hit> ptr(new Entity::Hit);
                ptr->objectSprite.setPosition(stoi((QI)->properties["PosX"]),stoi((QI)->properties["PosY"]));
                ObjectContainer.push_back(std::move(ptr));
                
            }

            else if((QI)->properties["itemType"] == "Explosion")
            {
                
                std::unique_ptr<Entity::Explosion> ptr(new Entity::Explosion);
                ptr->objectSprite.setPosition(stoi((QI)->properties["PosX"]),stoi((QI)->properties["PosY"]));
                ObjectContainer.push_back(std::move(ptr));
                
            }
            
            else if((QI)->properties["itemType"] == "Cluster")
            {
                
                std::unique_ptr<Entity::Cluster> ptr(new Entity::Cluster);
                ptr->objectSprite.setPosition(stoi((QI)->properties["PosX"]) + (rand() % 16 + -8),stoi((QI)->properties["PosY"]) + (rand() % 16 + -8));
                ObjectContainer.push_back(std::move(ptr));
                
            }
            
            //Weapons
            
            
            // Actors
            
            else if((QI)->properties["itemType"] == "EnemyNode")
            {
                
                std::unique_ptr<Entity::EnemyNode> ptr(new Entity::EnemyNode);
                ptr->objectSprite.setPosition(stoi((QI)->properties["PosX"]),stoi((QI)->properties["PosY"]));
                ObjectContainer.push_back(std::move(ptr));
                
            }
            
            else if((QI)->properties["itemType"] == "Slime")
            {
                
                std::unique_ptr<Entity::Slime> ptr(new Entity::Slime);
                ptr->objectSprite.setPosition(stoi((QI)->properties["PosX"]),stoi((QI)->properties["PosY"]));
                ObjectContainer.push_back(std::move(ptr));
                
            }

			else if ((QI)->properties["itemType"] == "Roach")
			{

				std::unique_ptr<Entity::Roach> ptr(new Entity::Roach);
				ptr->objectSprite.setPosition(stoi((QI)->properties["PosX"]), stoi((QI)->properties["PosY"]));
				ObjectContainer.push_back(std::move(ptr));

			}

			else if ((QI)->properties["itemType"] == "Mask")
			{

				std::unique_ptr<Entity::Mask> ptr(new Entity::Mask);
				ptr->objectSprite.setPosition(stoi((QI)->properties["PosX"]), stoi((QI)->properties["PosY"]));
				ObjectContainer.push_back(std::move(ptr));

			}

			else if ((QI)->properties["itemType"] == "HauzerSpire")
			{

				std::unique_ptr<Entity::HauzerSpire> ptr(new Entity::HauzerSpire);
				ptr->objectSprite.setPosition(stoi((QI)->properties["PosX"]), stoi((QI)->properties["PosY"]));
				ObjectContainer.push_back(std::move(ptr));

			}
            
            else if((QI)->properties["itemType"] == "Star")
            {
                
                std::unique_ptr<Entity::Star> ptr(new Entity::Star);
                ptr->objectSprite.setPosition(stoi((QI)->properties["PosX"]),stoi((QI)->properties["PosY"]));
                ObjectContainer.push_back(std::move(ptr));
                
            }
            
            else if((QI)->properties["itemType"] == "Mozza")
            {
                
                std::unique_ptr<Entity::Mozza> ptr(new Entity::Mozza);
                ptr->objectSprite.setPosition(stoi((QI)->properties["PosX"]),stoi((QI)->properties["PosY"]));
                ObjectContainer.push_back(std::move(ptr));
                
            }
            
            else if((QI)->properties["itemType"] == "Squid")
            {
                
                std::unique_ptr<Entity::Squid> ptr(new Entity::Squid);
                ptr->objectSprite.setPosition(stoi((QI)->properties["PosX"]),stoi((QI)->properties["PosY"]));
                ObjectContainer.push_back(std::move(ptr));
                
            }
            
            else if((QI)->properties["itemType"] == "Boss")
            {
                
                std::unique_ptr<Entity::Boss> ptr(new Entity::Boss);
                ptr->objectSprite.setPosition(stoi((QI)->properties["PosX"]),stoi((QI)->properties["PosY"]));
                ObjectContainer.push_back(std::move(ptr));
                
            }
            
            else if((QI)->properties["itemType"] == "Player")
            {
                
                std::unique_ptr<Entity::Player> ptr(new Entity::Player);
                ptr->objectSprite.setPosition(stoi((QI)->properties["PosX"]),stoi((QI)->properties["PosY"]));
                ObjectContainer.push_back(std::move(ptr));

            }
            
            else if((QI)->properties["itemType"] == "Actor")
            {
                
                std::unique_ptr<Entity::Actor> ptr(new Entity::Actor(stoi((QI)->properties["Name"])));
                ptr->objectSprite.setPosition(stoi((QI)->properties["PosX"]),stoi((QI)->properties["PosY"]));
                ObjectContainer.push_back(std::move(ptr));
                
            }
            
            else if((QI)->properties["itemType"] == "BattlePlayer")
            {
                
                std::unique_ptr<Entity::BattlePlayer> ptr(new Entity::BattlePlayer);
                ptr->objectSprite.setPosition(stoi((QI)->properties["PosX"]),stoi((QI)->properties["PosY"]));
                ObjectContainer.push_back(std::move(ptr));
                 
            }
            
            // Misc
            
            else if((QI)->properties["itemType"] == "PropFlame")
            {
                std::unique_ptr<Entity::PropFlame> ptr(new Entity::PropFlame);
                ptr->objectSprite.setPosition(stoi((QI)->properties["PosX"]),stoi((QI)->properties["PosY"]));
                ObjectContainer.push_back(std::move(ptr));
                
            }
            
            else if((QI)->properties["itemType"] == "HitNum")
            {
                
                std::unique_ptr<Entity::HitNum> ptr(new Entity::HitNum);
                ptr->num.setPosition(stoi((QI)->properties["PosX"]),stoi((QI)->properties["PosY"]));
                ptr->num.setString(((QI)->properties["Damage"]));
                ptr->numbg.setSize(sf::Vector2f(ptr->num.getString().getSize()*3,9));
                ObjectContainer.push_back(std::move(ptr));
                
            }
            
            else if((QI)->properties["itemType"] == "Door")
            {
                std::unique_ptr<Entity::Door> ptr(new Entity::Door((QI)->properties["Path"]));
                ptr->objectSprite.setPosition(stoi((QI)->properties["PosX"]),stoi((QI)->properties["PosY"]));
                ObjectContainer.push_back(std::move(ptr));
                
            }

        }
        
        Queue.clear();
        
    }
    
}


void Container::UpdateObjects()
{
    // create partiions for enemys and projectiles to compare in checkcollisions()
    
    Enemies = std::partition(ObjectContainer.begin(),ObjectContainer.end(),Entity::enemyPred);
    Projectiles = std::partition(Enemies,ObjectContainer.end(),Entity::projectilePred);
    EnemyProjectiles = std::partition(Projectiles,ObjectContainer.end(),Entity::enemyProjectilePred);
    Props = std::partition(EnemyProjectiles,ObjectContainer.end(),Entity::propPred);
    
    if(ObjectContainer.size() > 0)
    {
        int i = 0;
        
        for (ObjectIterator it = ObjectContainer.begin(); it !=  ObjectContainer.end(); it++){ 
            
            (*it)->Update();
            (*it)->UpdateHitBox();
            
            // this should set the objects zone for its new vector (position)
            
            (*it)->GetZone(i);
            i++;
        }
    }
    
}


void Container::RemoveObjects()
{
    
    if(ObjectContainer.size() > 0)
    {
        
        auto todestroyBegin = std::remove_if (ObjectContainer.begin(), ObjectContainer.end(), std::mem_fn(&Entity::Object::isDestroyed));
        ObjectContainer.erase(todestroyBegin, ObjectContainer.end());
        
    }
    
}


void Container::DrawObjects(sf::RenderTarget& window, class World& world)
{
    
    std::sort (ObjectContainer.begin(),ObjectContainer.end(),Entity::sortByYPos());
    std::sort (ObjectContainer.begin(),ObjectContainer.end(),Entity::isGUI());
    
    if(ObjectContainer.size() > 0)
    {
        
        for (ObjectIterator it = ObjectContainer.begin(); it !=  ObjectContainer.end(); it++)
        {
            if (!(*it)->Object::isDestroyed())
            {
                (*it)->DrawShadow(*World::GetInstance()->windowWorld);
                (*it)->Draw(*World::GetInstance()->windowWorld);
            }
            
            
        }
        
        
    }
    
}

std::vector<int> Container::GetObjectsInZone(std::string zone){
    
    std::vector<int> objectsInZone;
    std::vector<int> zoneList;
    
    //convert string to vector to iterate zone numbers
    
    std::stringstream ss(zone);
    
    int i;
    
    while (ss >> i)
    {
        zoneList.push_back(i);
        
        if (ss.peek() == ',')
            ss.ignore();
    }
    
    // change this pair to be compatible with strings
    
    std::pair <std::multimap<int,int>::iterator, std::multimap<int,int>::iterator> pair;
    
    // change zone int to string; wrap here with an iterator for each zone
    
    for (int zoneNode : zoneList){
    
        pair = zoneMatrix.equal_range(zoneNode);
        
        //for enemies, if vec position is within vec.begin() or enemies, add to objectsInZone
    
        for (std::multimap<int,int>::iterator it = pair.first; it != pair.second; ++it){
            
            objectsInZone.push_back(it->second);
            
        }
    
    }
    
    return objectsInZone;
    
}


void Container::CheckCollisions(){

    
	for (ObjectIterator enemy = ObjectContainer.begin(); enemy != Enemies; ++enemy)
	{
		// get all projectiles for enemy to check collisions to

		objectsInZone = GetObjectsInZone((*enemy)->zone);
		typeInZone = GetProjectileObjects(objectsInZone);

		// iterate against all projectiles

		for (int i = 0; i != typeInZone.size(); i++)
		{

			if ((*enemy)->objectHitBox.getGlobalBounds().intersects(ObjectContainer.at(typeInZone.at(i))->objectHitBox.getGlobalBounds()) && (*enemy)->active)
			{

				Entity::Object* obj = ObjectContainer.at(typeInZone.at(i)).get();
				Entity::Projectile* pobj = dynamic_cast<Entity::Projectile*>(obj);
				pobj->HasCollided(*enemy);

			}

		}

		// checking the player for enemy and enemy projectile collisions

		if ((*enemy)->objectHitBox.getGlobalBounds().intersects(World::GetInstance()->WorldScene.playerPtr->objectHitBox.getGlobalBounds()) && (*enemy)->active && World::GetInstance()->WorldScene.playerPtr->dashing == false) {

			World::GetInstance()->WorldScene.playerPtr->misDestroyed = true;
			World::GetInstance()->WorldScene.audioContainer.music.stop();
			World::GetInstance()->ReadyScene("map2_1");

		}

		/*// check enemies in zone for collision

		typeInZone = GetEnemyObjects(objectsInZone);

		if (typeInZone.size() != 0) 
		{

			for (int i = 0; i != typeInZone.size(); i++)
			{
				if (*enemy != ObjectContainer.at(typeInZone.at(i))) {

					if (GetDistance((*enemy)->objectHitBox.getPosition(), ObjectContainer.at(typeInZone.at(i))->objectHitBox.getPosition()) && (*enemy)->active && ObjectContainer.at(typeInZone.at(i))->active)
					{

						(*enemy)->objectSprite.move((*enemy)->vel);
						if (GetDistance((*enemy)->objectHitBox.getPosition(), ObjectContainer.at(typeInZone.at(i))->objectHitBox.getPosition())) 
						{
							(*enemy)->objectSprite.move(-(*enemy)->vel);
							(*enemy)->objectSprite.move(-(*enemy)->vel);

						}

					}

				}

			}

		}
        */
        
    }
    
    //get all enemy projectiles to compare to player
        
   objectsInZone = GetObjectsInZone(World::GetInstance()->WorldScene.playerPtr->zone);
    typeInZone = GetEnemyProjectileObjects(objectsInZone);
        
        for(int i = 0; i != typeInZone.size(); i++)
        {
            
            if(ObjectContainer.at(typeInZone.at(i))->objectHitBox.getGlobalBounds().intersects(World::GetInstance()->WorldScene.playerPtr->objectHitBox.getGlobalBounds()) && World::GetInstance()->WorldScene.playerPtr->dashing == false) 
		
			
            
{
                
                World::GetInstance()->WorldScene.playerPtr->misDestroyed = true;
                World::GetInstance()->WorldScene.audioContainer.music.stop();
                World::GetInstance()->ReadyScene("map2_1");
                
            }
            
        }
    
    
    //check prop/actor collison
    
    typeInZone = GetPropObjects(objectsInZone);
    
        for(int i = 0; i != typeInZone.size(); i++)
        {
            //std::cout << "in prop zone: " << ObjectContainer.at(typeInZone.at(i))->type << std::endl;
            if(ObjectContainer.at(typeInZone.at(i))->objectHitBox.getGlobalBounds().intersects(World::GetInstance()->WorldScene.playerPtr->objectHitBox.getGlobalBounds())){
                
                
                //launch is collided for interaction manager; pass variable
                // show interaction manager if not already up
                // set string to relative (i.e., "Talk to Mozza" "Purge gate")
                
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::X))ObjectContainer.at(typeInZone.at(i))->isCollided();
                
            }
            
        }
   
    
    zoneMatrix.clear();
}

void Container::GetCollisionArea(sf::Vector2f prevPos, sf::Vector2f currPos, sf::IntRect spriteRect){
    
    //objectCollisionArray.clear();
    
    // We need to get the absolute value of the sprite width since we are flipping the sprite for directions (i.e., avoid negative value)
    
    int spriteWidth =  spriteRect.width;
    int spriteHeight = spriteRect.height;
    
    int minposX = fmin(prevPos.x,currPos.x);
    int maxposX = fmax(prevPos.x,currPos.x);
    int minposY = fmin(prevPos.y,currPos.y);
    int maxposY = fmax(prevPos.y,currPos.y);
    
    // Defines bounding rectangle size and position of object position, before and after. Rounding to tile size
    
    objectBoundingRect.width = (minposX - maxposX) + ((spriteWidth/2) * 2);
    objectBoundingRect.height =  (minposY - maxposY) + (spriteHeight * 2);
    objectBoundingRect.left = minposX - spriteWidth/2;
    objectBoundingRect.top = minposY  - spriteHeight;
    
    // use objectBoundingRect to detect which objects are in range and add them to the objectCollsionArray (or just get those numbers and return them to CheckObjectCollision)
    
}

std::vector<int> Container::GetProjectileObjects(std::vector<int>& vec){
    
    
    
    int projectilesBegin = int(std::distance(ObjectContainer.begin(),Enemies));
    int projectilesSize = int(std::distance(Enemies,Projectiles));
    
    std::vector<int> projectilesInZone = vec;
    auto toRemove = std::remove_if (projectilesInZone.begin(), projectilesInZone.end(),TypeInZone(projectilesBegin,projectilesBegin+projectilesSize));
    projectilesInZone.erase(toRemove, projectilesInZone.end());
    std::sort(projectilesInZone.begin(),projectilesInZone.end());
    
    std::vector<int>::iterator it = std::unique (projectilesInZone.begin(),projectilesInZone.end());
    projectilesInZone.resize( std::distance(projectilesInZone.begin(),it));
    
    return projectilesInZone;
    
}

std::vector<int> Container::GetEnemyObjects(std::vector<int>& vec) {

	int projectilesBegin = 0;
	int projectilesSize = int(std::distance(ObjectContainer.begin(), Enemies));

	std::vector<int> projectilesInZone = vec;
	auto toRemove = std::remove_if(projectilesInZone.begin(), projectilesInZone.end(), TypeInZone(projectilesBegin, projectilesBegin + projectilesSize));
	projectilesInZone.erase(toRemove, projectilesInZone.end());
	std::sort(projectilesInZone.begin(), projectilesInZone.end());

	std::vector<int>::iterator it = std::unique(projectilesInZone.begin(), projectilesInZone.end());
	projectilesInZone.resize(std::distance(projectilesInZone.begin(), it));

	return projectilesInZone;

}

std::vector<int> Container::GetEnemyProjectileObjects(std::vector<int>& vec){
    
    int projectilesBegin = int(std::distance(ObjectContainer.begin(),Enemies));
    int enemyProjectilesBegin = int(std::distance(Enemies,Projectiles));
    enemyProjectilesBegin += projectilesBegin;
    int enemyProjectilesSize = int(std::distance(Projectiles,EnemyProjectiles));
    
    //std::cout << "enemy projectiles at: " << enemyProjectilesBegin << "-" << enemyProjectilesSize << std::endl;
    
    std::vector<int> enemyProjectilesInZone = vec;
    auto toRemove = std::remove_if (enemyProjectilesInZone.begin(), enemyProjectilesInZone.end(),TypeInZone(enemyProjectilesBegin,enemyProjectilesBegin+enemyProjectilesSize));
    enemyProjectilesInZone.erase(toRemove, enemyProjectilesInZone.end());
    std::sort(enemyProjectilesInZone.begin(),enemyProjectilesInZone.end());
    
    std::vector<int>::iterator it = std::unique (enemyProjectilesInZone.begin(),enemyProjectilesInZone.end());
    enemyProjectilesInZone.resize( std::distance(enemyProjectilesInZone.begin(),it));
    
    return enemyProjectilesInZone;
    
}

std::vector<int> Container::GetPropObjects(std::vector<int>& vec){
    
    int projectilesBegin = int(std::distance(ObjectContainer.begin(),Enemies));
    int enemyProjectilesBegin = int(std::distance(Enemies,Projectiles));
    int PropsBegin = int(std::distance(Projectiles,EnemyProjectiles));
    PropsBegin += projectilesBegin + enemyProjectilesBegin;
    int PropsSize = int(std::distance(EnemyProjectiles,Props));
    
    //std::cout << "prop projectiles at: " << PropsBegin << "-" << PropsSize << std::endl;
    
    std::vector<int> PropsInZone = vec;
    auto toRemove = std::remove_if (PropsInZone.begin(), PropsInZone.end(),TypeInZone(PropsBegin,PropsBegin+PropsSize));
    PropsInZone.erase(toRemove, PropsInZone.end());
    std::sort(PropsInZone.begin(),PropsInZone.end());
    
    std::vector<int>::iterator it = std::unique (PropsInZone.begin(),PropsInZone.end());
    PropsInZone.resize( std::distance(PropsInZone.begin(),it));
    
    return PropsInZone;
    
}



Container::~Container(){
    
    Queue.clear();
    ObjectContainer.clear();
    
}
