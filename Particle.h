//
//  Particle.h
//  sdev
//
//  Created by Darion McCoy on 1/10/14.
//  Copyright (c) 2014 D man. All rights reserved.
//

#ifndef __sdev__Particle__
#define __sdev__Particle__

#include <iostream>
#include <array>
#include <SFML/Graphics.hpp>

//class Container;
#include "World.h"
class World;


#ifndef __CONTAINER_H_INCLUDED__
#define __CONTAINER_H_INCLUDED__
#include "Container.h"
#endif

#include <chrono>
#include <thread>
#include <functional>
#include <list>
#include <cmath>
#include <boost/function.hpp>

#define FRAME 24
#define IDLE (FRAME * 0)
#define WALKING (FRAME * 1)
#define NORMAL 0
#define JUMPER 1
#define NODE 2
#define CLOSE 100
#define MID 150
#define LONG 400
#define LINELENGTH 60
#define START 'a'
#define DIVE 'b'
#define SWITCH 'c'
#define EXIT 'd'
#define NOTCOMPLETED 0
#define JUSTCOMPLETED 1
#define COMPLETED 2
#ifndef WINDOW_X
#define WINDOW_X 480*3
#endif

#ifndef WINDOW_Y
#define WINDOW_Y 270*3
#endif

namespace Entity
{
    
    struct itemQueue
    {
        itemQueue()
        {
            
            
        }
        
        ~itemQueue()
        {
        }
        
        std::map<std::string,std::string> properties;
        
    };
    
    //Base class for all entities

    class Object
    {
        friend class LevelContainer;
        
    public:
        static int totalObjects;
        sf::Vector2f vel;
        float posZ = 0;
        float velZ = 0;
        sf::Sprite objectSprite;
        sf::RectangleShape objectHitBox;
        sf::Sprite objectShadow;
        
        Object();
        virtual ~Object();
        virtual void Update();
        virtual void isCollided(int var = 0);
        virtual void Draw(sf::RenderTarget& window);
        sf::IntRect GetObjectBounds();
        float GetYPosition();
        bool PlayerDistance(int distance);
        void SetShadow();
        virtual void CheckLvlCollision();
        void UpdateShadow();
        void GetZone(int vecPos);
        void DrawShadow(sf::RenderTarget& window);
        bool isDestroyed();
        int damage;
        bool misDestroyed = false;
        bool active = false;
        std::string type = "Object";
        std::string zone;
        void SetCharacterOrigin(sf::Sprite** object = NULL, bool center = false);
        void SetEffectOrigin();
        void SetHitBox(sf::Vector2f size,int originPos = 0);
        void UpdateHitBox();
        
    private: 
        static int center;
        static int bottomCenter;
        static int topLeft;
    };
    
    // Misc classes (hud, menu, etc)
    
    class GUI : public Object
    {
        
    public:GUI();
        ~GUI();
        void Update();
        static int guiCount;
        
    };
    
    class MenuItem : public GUI
    {
        friend class MenuItemHolder;
        
    public:
        MenuItem(std::string lableName, char typeName);
        ~MenuItem();
        void Update();
        void Draw(sf::RenderTarget& window);
        void ToggleSelection();
        sf::Text menuLable;
        static sf::Font menuFnt;
        char type;
        std::weak_ptr<int> linkedAttribute;
        static int count;
        
    };
    
    
    class MenuItemHolder : public GUI
    {
        
    public:
        MenuItemHolder();
        ~MenuItemHolder();
        void Update();
        void Draw(sf::RenderTarget& window);
        int currentPos = 0;
        std::vector<MenuItem> menuList;
        
        
    };
    
    class Menu : public GUI
    {
        
    public:
        Menu();
        ~Menu();
        void Update();
        void DoMenuAction(MenuItem& item);
        bool HasSwitch(MenuItem& item);
        void Draw(sf::RenderTarget& window);
        std::vector<MenuItemHolder> menuContainer;
        
    };
    
    
    
    class Hud : public GUI
    {
        
    public:
        Hud();
        void Update();
        void Draw(sf::RenderTarget& window);
        
        ~Hud();
        
        sf::Sprite hbar;
        sf::Sprite hitem;
        int hmaxhealth = 0;
        int hmaxpow = 0;
        bool hdamaged = false;
        sf::Sprite hhealth;
        sf::Sprite hpower;
        sf::Text hhealthNum;
        sf::Text hpowerNum;
        sf::Font hnumFont;
        
    };
    
    class Textbox : public GUI
    {
        
    public:
        Textbox(int name);
        ~Textbox();
        void Update();
        void Draw(sf::RenderTarget& window);
        sf::Sprite backDrop;
        std::string script = "";
        std::string newScript = "";
        static int lineLength;
        static int maxLines;
        static int scriptLength;
        static int progressSpeed;
        static int textboxCount;
        sf::Vector2i portraitSize;
        sf::Text boxText;
        sf::Text boxName;
        sf::RectangleShape boxNameBG;
        sf::Sprite boxArrow;
        static std::string characters[11];
        std::string characterName = "";
        
    };
    
    class PlayerMenu : public GUI
    {
        
    public:
        PlayerMenu();
        ~PlayerMenu();
        void Update();
        void Draw(sf::RenderTarget& window);
        sf::Text itemsLable;
        sf::Text itemName;
        sf::Text itemDescriptionText;
        sf::Text itemDescriptionLable;
        sf::Text itemLvl;
        sf::Sprite itemsBg;
        static bool menuUp;
        
    };
    
    class Clone : public Object
    {
        
    public:
        Clone();
        ~Clone();
        void Update();
        sf::Texture* tempTexture;
        
    };
    
    class BattleBackground : public Object{
        
    public:
        BattleBackground();
        ~BattleBackground();
        void Update();
        void Draw(sf::RenderTarget& window);
        bool isLoaded = false;
        float size = 1;
        float rate = 1000;
        float plrDir = 0;
        sf::Vector2f playerPos;
        
        
    };
    
    // Player Classes
    
    class Player : public Object
    {
        
        friend class LevelContainer;
        
    public:
        void Update();
        void CheckLvlCollision();
        Player(sf::Vector2i pos = sf::Vector2i(200,50));
        ~Player();
        float* posXtemp1 = new float;
        float* posYtemp2 = new float;
        int spriteWidth = 0;
        int spriteHeight = 0;
        int playerHealth = 0;
        int playerPow = 0;
        enum movement {south,swest, west, nwest, north, neast, east, seast, idle};
        int movement = south;
        int fireDir = south;
        int frame_pos = 0;
        sf::Vector2f scaleTemp;
        bool isCollided = false;
        static bool dead;
        bool menuUp = false;
        
    };
    
    class BattlePlayer : public Player
    {
    
    public:
        BattlePlayer();
        void CheckLvlCollision();
        sf::Vector2f hotSpot;
        void Update();
        ~BattlePlayer();
        
    };

    
    //Bullets
    
    //Base class for bullets
    
    class Projectile : public Object
    {
        
    public:
        Projectile();
        virtual ~Projectile();
        virtual int GetDamage();
        void Update();
		void HasCollided(const std::unique_ptr<Entity::Object>& a);
        bool isCollided = false;
		bool destroyOnImpact = true;
        int frame = 0;
        int maxFrame = 0;
        float time;
        
    };
    
    class EnemyProjectile : public Projectile{
        
    public:
        EnemyProjectile();
        ~EnemyProjectile();
        
    };
    
    class EnemyBlip : public EnemyProjectile{
        
    public:
        EnemyBlip();
        ~EnemyBlip();
        void Update();
        
    };
    
    class Bullet : public Projectile{
        
    public:
        Bullet();
        ~Bullet();
    
    };
    
    class PlayerBombExp : public Projectile{
        
    public:
        PlayerBombExp();
        void Update();
        ~PlayerBombExp();
        static int Bombcount;
    };
    
    class PlayerLaser : public Projectile{
        
    public:
        PlayerLaser();
        void Update();
        void isCollided(int var = 0);
        ~PlayerLaser();
        
    };
    
    class PlayerBoomerang : public Projectile{
    public:
        PlayerBoomerang();
        void Update();
        int timeCurve = 100;
        ~PlayerBoomerang();
        static int totalBoomerangs;
        
    };
    
    class PlayerRepeater : public Projectile{
        
    public:
        PlayerRepeater();
        void Update();
        void isCollided(int var = 0);
        ~PlayerRepeater();
        
    };
    
    class Laser : public Projectile
    {
        
    public:
        Laser();
        ~Laser();
        void Update();
        float* playerPosX;
        float* playerPosY;
        float length = 1;
        float height = 10;
        float frame = 0;
        int span;
        
        
    };
    
    //Particles
    
    class Particle : public Object
    {
        
    public:
        Particle();
        ~Particle();
        void Update ();
        int frame = 0;
        float gravity = 0.4f;
        bool isAnimated = true;
    
    };
    
    class Hit : public Object{
        
    public:
        Hit();
        ~Hit();
        void Update ();
        int damage = 0;
    };
    
    class Dirt : public Particle{
        
    public:Dirt();
        ~Dirt();
    };
    
    
    class Gib : public Particle{
        
    public:Gib();
        ~Gib();
        sf::Texture gibTexture;
        int textRectLeft = 0;
        int textRectTop = 0;

    };
    
    class Fixed: public Particle{
        
    public:
        Fixed();
        ~Fixed();
        void Update();
        static int assetHeight;
        int maxFrame = 0;
        int maxTime = 0;
        float animSpeed = 5.0f;
        float spdReduceRate = 0;
        int top = -1;
        static int spriteRotation;
        float deacceleration = 0;
        bool rotation = false;
        
    };
    
    class PlayerBeam: public Fixed{
    public:
        PlayerBeam();
        sf::RectangleShape shaft;
        void Update();
        void Draw(sf::RenderTarget& window);
        ~PlayerBeam();
    };
    
    class DoorDestroy: public Fixed{
    public:
        DoorDestroy();
        void Update();
        ~DoorDestroy();
    };
    
    class SpriteClone: public Fixed{
    public:
        SpriteClone();
        void Update();
        ~SpriteClone();
    };
    
    class Spark: public Fixed{
    public:
        Spark();
        ~Spark();
    };
    
    class DeathPoof: public Fixed{
    public:
        DeathPoof();
        ~DeathPoof();
        
    };
    
    class DeathBoom: public Fixed{
    public:
        DeathBoom();
        ~DeathBoom();
    };
    
    class BigExp: public Fixed{
    public:
        BigExp();
        ~BigExp();
    };
    
    class DamageSpark: public Fixed{
        
    public:
        DamageSpark();
        ~DamageSpark();
    };
    
    class PlayerBomb : public Fixed{
        
    public:
        PlayerBomb();
        ~PlayerBomb();
        static int totalBombs;
        
    };
    
    class Explosion : public Object
    {
        
    public:
        Explosion();
        ~Explosion();
        void Update();
        int anim_frameX = 0;
        int anim_frameY = 0;
        float anim_speed = 1.0f;
        
    };
    
    
    class Cluster: public Object
    {
        
    public:
        Cluster();
        ~Cluster();
        void Update();
        int maxBomb = 6;
        
    };
    
    //Stationary items/objects
    
    class Passive: public Particle{
        
    public:Passive();
        ~Passive();
    };
    
    class Prop: public Object{
        
    public:
        Prop();
        ~Prop();
        void Update();
        int maxFrame = 0;
        int frame = 0;
        float animSpd = 0.0f;
        
    };
    
    class Door: public Object{
    public:
        Door(std::string pathway);
        void Update();
        int getPath();
        ~Door();
        void isCollided(int var = 0);
        void Draw(sf::RenderTarget& window);
    private:
        int levelPath = 0;
        int framePos = 0;
    };
    
    class PropFlame: public Prop{
        
    public:
        PropFlame();
        ~PropFlame();
    
    };
    
    class HitNum: public Object{
       
    public:
        HitNum();
        ~HitNum();
        void Update();
        void Draw(sf::RenderTarget& window);
        static sf::Font hitFnt;
        sf::Text num;
        sf::RectangleShape numbg;
        
    };
    
    
///////////// Enemies & Props
    
    class Actor : public Object
    {
        
    public:
        Actor(int actor);
        ~Actor();
        void Update();
        void Move();
        int framePos = 0;
        void isCollided(int var = 0);
        std::string name = "";
        int character = 0;
        int moveType = IDLE;
        enum movement {south,swest, west, nwest, north, neast, east, seast, idle};
        int actorMovement = south;
        enum behavior {none,looking};
        int idleBehavior = looking;
        
    };
    
    class LevelManager : public Object
    {
        
    public:
        LevelManager();
        ~LevelManager();
        void Draw(sf::RenderTarget& window);
        void Update();
        int fieldSize = 600;
        int maxEnemies = 10;
        // default int maxEnemies = 150;
        static std::array<std::string,26> enemyList;
        sf::Sprite bg;
    };
    
    class Enemy : public Object
    {
        
    public:
        static int totalEnemies;
        
        Enemy();
        virtual~Enemy();
        void Update();
        virtual void Move();
        void isDamaged(int damage = 0);
        void isCollided(int var);
        void Act();
		virtual void isHurt();
		virtual void MoveElse();
        void Draw(sf::RenderTarget& window);
        virtual void Attack();
        void Damaged();
        bool hasAttack = false;
        float health = 3;
        float maxhealth = 3;
        bool bttmcollision = false;
        int frame = 0;
        bool enemyDir = true;
        int enemyMode = 1;
        bool push = true;
        float fireDir;
        float time = 0;
        float animate = 0;
        float jump = 0;
        float speed = 1;
        int vib = 1;
        sf::Vector2f scaleTemp;
        int moveType = NORMAL;
        int enemyID = 0;
        bool hurt = false;
		bool moveOnAttack = false;
        bool targetPlayer = true;
        sf::Vector2f hurtPos;
        
    };
    
    class EnemyNode : public Enemy
    {
    public:
        EnemyNode();
        ~EnemyNode();
    };
    
    class Star : public Enemy
    {
    public:
        Star();
        ~Star();
        
    };
    
    class Slime : public Enemy
    {
    public:
        Slime();
        ~Slime();
        
    };
    
    class Squid : public Enemy
    {
    public:
        Squid();
        ~Squid();
        
    };
    
    class Boss : public Enemy
    {
        
    public:
        Boss();
        ~Boss();
        void Update();
		void HUDUpdate();
		void Move();
		void Attack();
        void Draw(sf::RenderTarget& window);
        sf::RectangleShape healthBar;
        sf::Text bossName;
        float maxhealth = 200;
        float currhealth = 0;

		static std::vector<boost::function<void(Entity::Boss*)>> MovementList;
		static std::vector<boost::function<void(Entity::Boss*)>> AttackList;

		// functions to create different behaviors for boss

		//movement types

		void Idle();
		void FollowPlayer();

		//attack types

		void Shoot8dir();
		void Spawn();

		int currentBehavior = 0;
        
    };
    
    class Mozza : public Boss{
        
    public:
        Mozza();
        ~Mozza();
		sf::Sprite wings;
		void isHurt();
		void MoveElse();
		int wingFrames = 0;
		void Draw(sf::RenderTarget& window);
    };
    
    struct sortByYPos
    {
        inline bool operator()(const std::unique_ptr<Entity::Object>& a, const std::unique_ptr<Entity::Object>& b)
        {
            return a->objectShadow.getPosition().y + a->velZ < b->objectShadow.getPosition().y + b->velZ;
        }
        
    };
    
    struct isGUI
    {
        inline bool operator()(const std::unique_ptr<Entity::Object>& a, const std::unique_ptr<Entity::Object>& b)
        {
        
            int aType = 1;
            int bType = 1;
            if(a->type == "GUI") aType = 2;
            if(a->type == "BG") aType = 0;
            if(b->type == "GUI") bType = 2;
            if(b->type == "BG") bType = 0;
            return aType < bType;
            
        }
        
    };
    
    
    
    
//// MISC
    
    sf::Vector2f GetCharacterOrigin(Entity::Object& obj);
    void isDestroyed(Entity::Enemy& object);
    void CreateClone(sf::Sprite& sprite,std::string type = "");
    int GetRandDmg(int damage);
    bool enemyPred (const std::unique_ptr<Entity::Object>& a);
    bool enemyProjectilePred (const std::unique_ptr<Entity::Object>& a);
    bool projectilePred (const std::unique_ptr<Entity::Object>& a);
    bool propPred (const std::unique_ptr<Entity::Object>& a);
    
}



#endif /* defined(__sdev__Particle__) */

