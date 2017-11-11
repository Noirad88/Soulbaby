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
#define SLIDER 2
#define LAZYFOLLOW 3
#define VERYCLOSE 25
#define CLOSE 100
#define MID 150
#define FAR 250
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

		std::map<std::string, std::string> properties;
		Entity::Object* parent = nullptr;
		Entity::Object** ptrparent = nullptr;


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
		virtual bool isActive();
		virtual int Return(int p);
		virtual void Draw(sf::RenderTarget& window);
		sf::IntRect GetObjectBounds();
		float GetYPosition();
		bool PlayerDistance(int distance);
		void SetShadow();
		virtual void CheckLvlCollision();
		void UpdateShadow();
		void GetZone(int vecPos);
		virtual void DrawShadow(sf::RenderTarget& window);
		bool isDestroyed();
		int damage;
		bool misDestroyed = false;
		bool active = false;
		std::string type = "Object";
		std::string zone;
		void SetCharacterOrigin(sf::Sprite** object = NULL, bool center = false);
		void SetEffectOrigin();
		void SetHitBox(sf::Vector2f size, int originPos = 0);
		int hitBoxType = 0;
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

	class QuakeManager : public Object
	{
	public: QuakeManager();
			void Update();
			~QuakeManager();

	};

	class MouseTest : public Object 
	{
	public: MouseTest();
			void Update();
			void Draw(sf::RenderTarget& window);
			~MouseTest();
	};

	class MenuItem : public GUI
	{
		friend class MenuItemHolder;

	public:
		MenuItem();
		virtual ~MenuItem();
		virtual void Update();
		void Draw(sf::RenderTarget& window);
		virtual void Action();
		void ToggleSelection();
		static int count;
		sf::Text menuLable;
		sf::Text joyAssignment;
		sf::Sprite hand;
		bool locked = false;
		bool disabled = false;

	};

	class MenuItemStart : public MenuItem
	{

	public:
		MenuItemStart();
		~MenuItemStart();
		void Update();
		void Action();

	};

	class MenuItemOptions: public MenuItem
	{

	public:
		MenuItemOptions();
		~MenuItemOptions();
		void Update();
		void Action();

	};

	class MenuItemMusicVolume : public MenuItem {

	public:
		MenuItemMusicVolume();
		~MenuItemMusicVolume();
		void Update();
		void Action();
	};

	class MenuItemSFXVolume : public MenuItem {

	public:
		MenuItemSFXVolume();
		~MenuItemSFXVolume();
		void Update();
		void Action();
	};

	class MenuItemFullscreen : public MenuItem {

	public:
		MenuItemFullscreen();
		~MenuItemFullscreen();
		void Update();
		void Action();
	};

	class MenuItemResolution : public MenuItem {

	public:
		MenuItemResolution();
		~MenuItemResolution();
		void Update();
		void Action();
	};

	class MenuItemControls : public MenuItem {

	public:
		MenuItemControls();
		~MenuItemControls();
		void Update();
		void Action();
	};

	class MenuItemSetKey : public MenuItem {

	public:
		MenuItemSetKey(int key);
		~MenuItemSetKey();
		void Update();
		void ResetLables();
		void Draw(sf::RenderTarget& window);
		void Action();
		int assignedKey = 0;
		bool isAssigningNewKey = false;
		int waitASec = 0;
	};

	class MenuItemCRTMode : public MenuItem {

	public:
		MenuItemCRTMode();
		~MenuItemCRTMode();
		void Update();
		void Action();
	};

	class MenuItemQuit : public MenuItem
	{
	public:
		MenuItemQuit();
		~MenuItemQuit();
		void Update();
		void Action();
	};


	class MenuItemHolder : public GUI
	{

	public:
		MenuItemHolder();
		~MenuItemHolder();
		void Update();
		void Draw(sf::RenderTarget& window);
		int currentPos = 0;
		std::vector<MenuItem*> menuList;


	};

	class Menu : public GUI
	{

	public:
		Menu();
		~Menu();
		void Update();
		void Draw(sf::RenderTarget& window);
		static std::vector<MenuItemHolder> menuContainer;

	};



	class Hud : public GUI
	{

	public:
		Hud();
		void Update();
		void Expand();
		void Close();
		void Draw(sf::RenderTarget& window);
		bool isExpanded = false;


		~Hud();

		sf::Sprite selectedWeapon;
		sf::Text pow1Lable;
		sf::Text pow2Lable;
		sf::Sprite hand;
		sf::Sprite selectionItems;
		sf::Sprite newAbilityOverlay;
		std::vector<int> itemList;
		int selectState = 0;
		float expandTween = 0;
		sf::RectangleShape manaBar;
		float manaFillBar = 0;
		int newAbility = 0;

	};

	class ManaMeter : public GUI
	{
	public:
		ManaMeter();
		void Update();
		void Draw(sf::RenderTarget& window);
		~ManaMeter();
		std::vector<sf::Vector2f> pointPositions;
		bool got;

	};


	class Guide : public GUI
	{

	public:
		Guide();
		void Update();
		void Draw(sf::RenderTarget& window);
		void SetTarget(sf::Vector2f newPosition);
		void Hide();
		~Guide();

		bool hidden = true;
		sf::Sprite Icon;
		sf::Vector2f targetPosition;
		bool ready = true;

	};

	class Textbox : public GUI
	{

	public:
		Textbox(std::string nameAndScript);
		~Textbox();
		void Update();
		void Draw(sf::RenderTarget& window);
		sf::Sprite backDrop;
		int scriptNumber = 0;
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
		sf::Text textChoice1;
		sf::Text textChoice2;
		sf::Sprite hand;
		sf::RectangleShape boxNameBG;
		sf::Sprite boxArrow;
		sf::Sprite choice1bg;
		sf::Sprite choice2bg;
		static std::string characters[11];
		std::string characterName = "";
		int characterNamePos = 0;
		int selectState = 0;
		bool isDone = false;

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

	class BattleBackground : public Object {

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
		Player(sf::Vector2i pos = sf::Vector2i(200, 50));
		~Player();
		void Draw(sf::RenderTarget& window);
		void Bounce();
		float* posXtemp1 = new float;
		float* posYtemp2 = new float;
		//bool IsShooting();
		//void SetShootDirection();
		int spriteWidth = 0;
		int spriteHeight = 0;
		int playerHealth = 0;
		int playerPow = 0;
		enum movement { south, swest, west, nwest, north, neast, east, seast, idle };
		int movement = south;
		int fireDir = south;
		bool dashing = false;
		int frame_pos = 0;
		sf::Vector2f scaleTemp;
		bool isCollided = false;
		static bool dead;
		bool menuUp = false;
		int chargeFlag = 0;
		int chargeCounter = 0;
		int dashCount = 0;
		bool jumpFlag = false;
		bool dashSave = true;
		int shield = 41;
		sf::Sprite sshield;
		int mana = 0;
		bool hyperDash = false;


		ManaMeter* PlayerManaMeter;

	};

	class BattlePlayer : public Player
	{

	public:
		BattlePlayer();
		void DoChargeAttack();
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

		//only used for beam projectiles; beams are made of multple hitboxes; getcollidednode() returns the node collided closest to its parent
		int GetCollidedNode();
		int nodeCollided = 0;

		virtual void HasCollided(const std::unique_ptr<Entity::Object>& a);
		bool isCollided = false;
		bool destroyOnImpact = true;
		float deacceleration = 0;
		float acceleration = 0;
		int speed = 1;
		int frame = 0;
		int maxFrame = 0;
		int maxTime = 0;
		float time;
		bool rotation = false;
		bool followsPlayer = false;
		int health = 0;

		std::string emitter = "";
		float emitTime = 0;
		bool emitScatter = false;
		int emitCount = 1;
		Object* parent = nullptr;


	};

	class GameScene : public Object {

	public:
		GameScene();
		void Update();
		bool textboxCreated = false;
		bool sceneDone = false;
		~GameScene();

	};

	class Buddy : public Object {

	public:
		Buddy();
		int buddyPos = 0;
		void Update();
		~Buddy();

	};

	class EnemyProjectile : public Projectile {

	public:
		EnemyProjectile();
		~EnemyProjectile();

	};

	class HauzerSmog : public EnemyProjectile {

	public:
		HauzerSmog();
		~HauzerSmog();
		void Update();
		int sprite;

	};

	class HauzerSpear : public  EnemyProjectile {

	public:
		HauzerSpear();
		~HauzerSpear();

	};

	class EnemyBlip : public EnemyProjectile {

	public:
		EnemyBlip();
		~EnemyBlip();
		void Update();

	};

	class EnemyMine : public EnemyProjectile {

	public:
		EnemyMine();
		~EnemyMine();

	};

	class EnemyHomingBlip : public EnemyProjectile {

	public:
		EnemyHomingBlip();
		~EnemyHomingBlip();
		void Update();
	};

	class EnemyLaser : public EnemyProjectile {

	public:
		EnemyLaser();
		~EnemyLaser();
		void Update();
		virtual void Draw(sf::RenderTarget& window);

		sf::Sprite laserBody;
		sf::Sprite laserHead;
		sf::Vector2f laserBodyvel;
		Entity::Object* objReference = nullptr;


	};

	class BigEnemyLaser : public EnemyLaser {

	public:
		BigEnemyLaser();
		~BigEnemyLaser();
		virtual void Draw(sf::RenderTarget& window);

	};

	class Bullet : public Projectile {

	public:
		Bullet();
		~Bullet();

	};

	class PlayerBombExp : public Projectile {

	public:
		PlayerBombExp();
		void Update();
		~PlayerBombExp();
	    void Draw(sf::RenderTarget& window);

		static int Bombcount;
	};

	class PlayerLaser1 : public Projectile {

	public:
		PlayerLaser1();
		void isCollided(int var = 0);
		~PlayerLaser1();

	};

	class PlayerLaser2 : public PlayerLaser1 {

	public:
		PlayerLaser2();
		~PlayerLaser2();
	};

	class PlayerLaser3 : public PlayerLaser1 {

	public:
		PlayerLaser3();
		~PlayerLaser3();

	};

	class PlayerLaser4 : public PlayerLaser1 {

	public:
		PlayerLaser4();
		~PlayerLaser4();

	};

	class PlayerLaser5 : public PlayerLaser1 {

	public:
		PlayerLaser5();
		~PlayerLaser5();

	};

	class PlayerBeam1 : public Projectile {

	public:
		PlayerBeam1();
		void Update();
		~PlayerBeam1();
		bool isFiring = false;
		int length = 30;
		bool nodesCreated = false;
		int direction = 0;

	};

	class PlayerBeamNode : public Projectile {
	
	public:
		PlayerBeamNode();
		void Update();
		void HasCollided(const std::unique_ptr<Entity::Object>& a);
		~PlayerBeamNode();
		int nodeSlot = 0;

	};


	class ElectricNode : public Projectile {

	public:
		ElectricNode();
		~ElectricNode();
	};

	class PlayerDashBall : public Projectile {

	public:
		PlayerDashBall();
		void Update();
		void isCollided(int var = 0);
		~PlayerDashBall();

	};



	class PlayerBoomerang : public Projectile {
	public:
		PlayerBoomerang();
		void Update();
		void HasCollided(const std::unique_ptr<Entity::Object>& a);
		int timeCurve = 100;
		~PlayerBoomerang();
		static int totalBoomerangs;

	};

	class PlayerRepeater1 : public Projectile {

	public:
		PlayerRepeater1();
		void isCollided(int var = 0);
		~PlayerRepeater1();

	};

	class PlayerRepeater2 : public Projectile {

	public:
		PlayerRepeater2();
		~PlayerRepeater2();

	};

	class PlayerRepeater3 : public Projectile {

	public:
		PlayerRepeater3();
		~PlayerRepeater3();

	};

	class PlayerRepeater6 : public Projectile {

	public:
		PlayerRepeater6();
		~PlayerRepeater6();

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
		void Update();
		int frame = 0;
		float gravity = 0.4f;
		bool isAnimated = true;
		Object* parent;

	};

	class Hit : public Object {

	public:
		Hit();
		~Hit();
		void Update();
		int damage = 0;
	};

	class Dirt : public Particle {

	public:Dirt();
		   ~Dirt();
	};


	class Gib : public Particle {

	public:Gib();
		   ~Gib();
		   sf::Texture gibTexture;
		   int textRectLeft = 0;
		   int textRectTop = 0;

	};

	class Fixed : public Particle {

	public:
		Fixed();
		~Fixed();
		void Update();
		static int assetHeight;
		int maxFrame = 0;
		float maxTime = 0;
		float animSpeed = 5.0f;
		float spdReduceRate = 0;
		bool dontLoop = false;
		int top = -1;
		static int spriteRotation;
		float deacceleration = 0;
		float acceleration = 0;
		bool rotation = false;


	};

	class BounceCrawler : public Fixed {
	public:
		BounceCrawler();
		void Update();
		~BounceCrawler();
		int direction = 0;
		static std::array<std::pair<int,float>,7> BehaviorsForWeapons;

	};

	class QuakeDirt : public Fixed {

	public: QuakeDirt();
			~QuakeDirt();

	};


	class PlayerBeam : public Fixed {
	public:
		PlayerBeam();
		sf::RectangleShape shaft;
		void Update();
		void Draw(sf::RenderTarget& window);
		~PlayerBeam();
	};

	class EnemyCharge : public Fixed {
	public:
		EnemyCharge();
		~EnemyCharge();
	};

	class StarSpawn: public Fixed {
	public:
		StarSpawn();
		~StarSpawn();
	};

	class PlayerPoint : public Fixed {
	public:
		PlayerPoint();
		void Update();
		~PlayerPoint();
		int meterPos = -1;

	};
    
	class LearnedAbility : public Fixed {
	public:
		LearnedAbility();
		void Update();
		~LearnedAbility();
		int ability = 0;

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

	class DashEffect : public Fixed {
	public:
		DashEffect();
		~DashEffect();
	};

	class SlideEffect : public Fixed {
	public:
		SlideEffect();
		~SlideEffect();
	};
    
    class Spark: public Fixed{
    public:
        Spark();
        ~Spark();
    };

	class Electricity : public Fixed {
	public:
		Electricity();
		~Electricity();
	};

	class EnemySpark: public Fixed {

	public:
		EnemySpark();
		~EnemySpark();
	};

	class PlayerSpark : public Fixed {

	public:
		PlayerSpark();
		~PlayerSpark();
	};

	class EnemyPart : public Fixed {

	public:
		EnemyPart();
		void Draw(sf::RenderTarget& window);
		~EnemyPart();
	};
    
    class DeathPoof: public Fixed{
    public:
        DeathPoof();
        ~DeathPoof();
		void Draw(sf::RenderTarget& window);
        
    };

	class EnemyChargeParticle : public Fixed {
	public:
		EnemyChargeParticle();
		~EnemyChargeParticle();

	};

	class PlayerChargeParticle : public Fixed {
	public:
		PlayerChargeParticle();
		~PlayerChargeParticle();

	};

	class PlayerChargeParticleSmall : public Fixed {
	public:
		PlayerChargeParticleSmall();
		~PlayerChargeParticleSmall();

	};

	class ChargeParticleTiny : public Fixed {
	public:
		ChargeParticleTiny();
		~ChargeParticleTiny();

	};

	class ChargeWaveAttack : public Fixed {
	public:
		ChargeWaveAttack();
		void Update();
		~ChargeWaveAttack();
		Object* projectileNode;
		Object* projectileNode2;
		Object* projectileNode3;
		double count = 0;


	};

	class HauzerCharge : public Fixed {

	public:
		HauzerCharge();
		~HauzerCharge();
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

	class ShieldEffect : public Fixed {
	public:
		ShieldEffect();
		~ShieldEffect();

	};

	class BlockedEffect : public Fixed {
	public:
		BlockedEffect();
		~BlockedEffect();

	};

	class BlockedWave : public Fixed {
	public:
		BlockedWave();
		~BlockedWave();

	};
    
    class DamageSpark: public Fixed{
        
    public:
        DamageSpark();
        ~DamageSpark();
    };
    
    class PlayerBomb : public Fixed{
        
    public:
        PlayerBomb();
		void Update();
        ~PlayerBomb();
        static int totalBombs;
        
    };

	class LargeCharge : public Fixed {

	public:
		LargeCharge();
		~LargeCharge();

	};

	class ActionSpark : public Fixed {

	public:
		ActionSpark();
		~ActionSpark();
	};

	class PlayerSpawn : public Fixed {

	public:
		PlayerSpawn();
		void Draw(sf::RenderTarget& window);
		~PlayerSpawn();

	};

	class PlayerEgg : public Fixed {

	public:
		PlayerEgg();
		void Draw(sf::RenderTarget& window);
		Object* parent;
		~PlayerEgg();

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

	class PlayerBirth : public Prop {
	public:
		PlayerBirth();
		void Update();
		void Draw(sf::RenderTarget& window);
		~PlayerBirth();
		int phase = 0;
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
        Actor(int actor,int script = 99);
        virtual ~Actor();
        void Update();
		void Draw(sf::RenderTarget& window);
		void DrawShadow(sf::RenderTarget& window);
        void Move();
        int framePos = 0;
        void isCollided(int var = 0);
        int characterName = 0;
		int scriptNumber = 0;
        enum movement {south,swest, west, nwest, north, neast, east, seast, idle};
        int actorMovement = south;
        enum behavior {none,looking};
        int idleBehavior = looking;
        
    };

	class SoulOrb : public Actor
	{

	public:
		SoulOrb(int actor,int script = 99);
		void Update();
		void Draw(sf::RenderTarget& window);
		void isCollided(int var = 0);
		~SoulOrb();

	};
    
    class LevelManager : public Object
    {
        
    public:
        LevelManager();
        ~LevelManager();
		void CreateEnemy(int enemy);
        void Draw(sf::RenderTarget& window);
        void Update();
        int fieldSize = 600;
        int maxEnemies = 100;
        // default int maxEnemies = 150;
        static std::array<std::string,26> enemyList;
        sf::Sprite bg;
		sf::Sprite bgwall;
		bool bgwallHit = false;
		bool bossIsCreated = false;
		std::array<int, 3> lvlEnemyBank;

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
		bool isActive();
        void Act();
		virtual void isHurt();
		virtual void MoveElse();
		void Animate();
		void Die();
        void Draw(sf::RenderTarget& window);
		virtual void DrawChild();
        virtual void Attack();
		void TargetPlayer();
		void SetSpriteRedForHealth();
        void Damaged();
		static int spriteSheetHeight;
        int hasAttack = 0;
        float health = 3;
        float maxHealth = 3;
		bool defending = false;
        int frame = 0;
        bool enemyDir = true;
        int enemyMode = 0;
		sf::Vector2f varyMov;
        bool push = true;
        float fireDir;
        float animate = 0;
		float animationSpeed = VERY_SLOW;
        float jump = 0;
        float speed = 1;
		int spriteTop = -1;
        sf::Vector2f scaleTemp;
        int moveType = NORMAL;
        int enemyID = 0;
		int shakey = 0;
		sf::Vector2f prevPos;
		bool attacking = false;
		int flatAnimation = false;
		int spriteDirection = 0;
		int currentDirection = 0;
		int bigDaddy = false;
        bool hurt = false;
		bool showHurt = false;
		bool moveOnAttack = false;
        bool alwaysTargetPlayer = true;
		void OffsetPosition();
		std::string create = "";

        sf::Vector2f hurtPos;
		sf::Vector2f hotSpot;
		bool transition = true;
		sf::Vector2f targetPosition;

		//boost functions are used for each enemy movement and attack; functions are set during each enemy constructor

		boost::function<void(Enemy*)> AssignedBehavior;
		boost::function<void(Enemy*)> AssignedMovement;
		boost::function<void(Enemy*)> AssignedAttack;

		//List of functions

		//Behaviors
		void BehaveNone();
		void FollowPlayer();
		void LazyFollowPlayer();
		void RandomPosition();
		void Bounce();
		void Sticky();

		//Movements
		void MoveJump();
		void MoveWalk();
		void MoveSlide();
		void MoveCircle();

		//Attacks
		void Shoot();
		void SporeShoot();
		void CastShoot();
		void SelfDestruct();
		void Laser();
		void LayFire();
		void AroundShoot();
		void DoubleCast();
		void CircleShoot();


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

	class Eball : public Enemy
	{
	public:
		Eball();
		~Eball();

	};

	class Family : public Enemy
	{
	public:
		Family();
		~Family();

	};

	class BabyDemon : public Enemy
	{
	public:
		BabyDemon();
		~BabyDemon();

	};

	class HorseKnight : public Enemy
	{
	public:
		HorseKnight();
		~HorseKnight();

	};

	class Mask : public Enemy
	{
	public:
		Mask();
		~Mask();

	};

	class Sprite : public Enemy
	{
	public:
		Sprite();
		~Sprite();

	};
    
    class Slime : public Enemy
    {
    public:
        Slime();
        ~Slime();
        
    };

	class Tomb : public Enemy
	{
	public:
		Tomb();
		~Tomb();

	};

	class Bricky : public Enemy
	{
	public:
		Bricky();
		~Bricky();

	};


	class Spore : public Enemy
	{
	public:
		Spore();
		~Spore();

	};

	class Roach : public Enemy
	{
	public:
		Roach();
		~Roach();

	};

	class Raider : public Enemy
	{
	public:
		Raider();
		~Raider();

	};

	class Djinn : public Enemy
	{
	public:
		Djinn();
		void DrawChild();
		sf::Sprite wings;
		~Djinn();
	};

	class Tower : public Enemy
	{
	public:
		Tower();
		~Tower();
	};
    
    class Squid : public Enemy
    {
    public:
        Squid();
        ~Squid();
        
    };

	class HauzerSpire : public Enemy {

	public:
		HauzerSpire();
		~HauzerSpire();
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
		void NextMovement();
		void NewMovementSet();
        void Draw(sf::RenderTarget& window);
		int phase = 0;

		std::vector<boost::function<void(Boss*)>> BehaviorList;

		// functions to create different behaviors for boss

		virtual void Behavior1();
		virtual void Behavior2();
		virtual void Behavior3();
		virtual void Behavior4();
		virtual void Rest();

		// stock movements

		void Idle();
		void FollowPlayer();
		void Roam();
		void MoveToCenter();
		void RoachMovement();

		void AnimateIdle();
		void AnimateAttack();

		int currentMovement = 0;
		int currentAttack = 0;
        
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

		void Behavior1();
		void Behavior2();
		void Behavior3();
		void Behavior4();
		void Rest();

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
			if (a->type == "GUI") aType = 3;
			if (a->type == "Particle" || a->type == "Projectile" || a->type ==  "EnemyProjectile") aType = 2;
            if(a->type == "BG") aType = 0;
            if(b->type == "GUI") bType = 3;
			if (b->type == "Particle" || b->type == "Projectile" || b->type ==  "EnemyProjectile") bType = 2;
            if(b->type == "BG") bType = 0;
            return aType < bType;
            
        }
        
    };

    
    
    
//// MISC
    
    sf::Vector2f GetCharacterOrigin(Entity::Object& obj);
    void isDestroyed(Entity::Enemy& object);
    void CreateClone(sf::Sprite& sprite,std::string type = "", bool color = true);
    int GetRandDmg(int damage);
    bool enemyPred (const std::unique_ptr<Entity::Object>& a);
    bool enemyProjectilePred (const std::unique_ptr<Entity::Object>& a);
    bool projectilePred (const std::unique_ptr<Entity::Object>& a);
    bool propPred (const std::unique_ptr<Entity::Object>& a);
    
}



#endif /* defined(__sdev__Particle__) */

