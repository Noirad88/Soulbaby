//
//  Container.h
//  sdev
//
//  Created by Darion McCoy on 1/11/14.
//  Copyright (c) 2014 D man. All rights reserved.
//

#ifndef __sdev__Container__
#define __sdev__Container__

#include <iostream>
#include <SFML/Graphics.hpp>
#include <functional>
#include <string>


namespace Entity
{
    
    class Player;
    class Object;
    class Enemy;
    class Bullet;
    class Projectile;
    class PlayerLaser;
    struct itemQueue;
    
}

class World;

#include <Particle.h>

class Container
{

    
public:
    
    Container();
    ~Container();
    void RemoveObjects();
    void UpdateObjects();
    void AddObjects();
    void CheckCollisions();
	void PlayerDamaged();
    void GetCollisionArea(sf::Vector2f prevPos, sf::Vector2f currPos, sf::IntRect spriteRect);
    std::vector<int> GetProjectileObjects(std::vector<int>& vec);
	std::vector<int> GetEnemyObjects(std::vector<int>& vec);
    std::vector<int> GetEnemyProjectileObjects(std::vector<int>& vec);
    std::vector<int> GetPropObjects(std::vector<int>& vec);

    std::vector<int> GetObjectsInZone(std::string zone);
    std::vector<sf::Vector2i> objectCollisionArray;
    std::multimap<int,int> zoneMatrix;
    sf::FloatRect objectBoundingRect;


    void DrawObjects(sf::RenderTarget& window, class World& world);
    sf::Clock clock;
    sf::Time AccumuTime;
    std::vector<Entity::itemQueue> Queue;
    std::vector<std::unique_ptr<Entity::Object>> ObjectContainer;
    std::vector<std::unique_ptr<Entity::Object>>::iterator Enemies;
    std::vector<std::unique_ptr<Entity::Object>>::iterator Projectiles;
    std::vector<std::unique_ptr<Entity::Object>>::iterator EnemyProjectiles;
    std::vector<std::unique_ptr<Entity::Object>>::iterator Props;
    std::vector<int> objectsInZone;
    std::vector<int> typeInZone;
    
private:
    
    int posX = 0;
    int posY = 0;
    int totalP = 0;
    sf::Sprite SpawnPoint;
    
};


typedef std::vector<Entity::itemQueue>::iterator QueueIterator;

typedef std::vector<std::unique_ptr<Entity::Object>>::iterator ObjectIterator;

struct TypeInZone
{
    TypeInZone(int mbegin, int mend) : begin(mbegin), end(mend-1) {}
    int begin = 0;
    int end = 0;
    bool operator()(const int a)
    {
        return a < begin || a > end;
    }
};

bool normalSort (int i,int j);






#endif /* defined(__sdev__Container__) */



