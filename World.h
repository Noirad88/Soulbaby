//
//  World.h
//  HardSoda
//
//  Created by Darion McCoy on 6/18/15.
//  Copyright (c) 2015 Darion. All rights reserved.
//

#ifndef __HardSoda__World__
#define __HardSoda__World__

#include <stdio.h>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <math.h>
#include <functional>
#include <memory>
#include <map>
#include <sstream>

//#include "resource.h"
//#include "winuser.h"

#define DELAY 0
#define NODELAY 1
#define M_PI 3.14159265358979323846

#ifndef WINDOW_X
#define WINDOW_X 480*3
#endif

#ifndef WINDOW_Y
#define WINDOW_Y 270*3
#endif

#define VERY_SLOW 100000.0
#define SLOW 70000.0
#define NORMAL 50000
#define FAST 25000.0
#define VERY_FAST 10000.0



class Audio;
class Container;

namespace Textures{
    
    class TextureContainer;
}

namespace Entity{
    
    class Object;
    class Player;
    class Hud;
	class Guide;
}

namespace Level{
    
    class LevelContainer;
    
}

#include "Audio.h"
#include "Textures.h"
#include "LevelContainer.h"
#include "Container.h"

#define MENU 0
#define MAP 1
#define ENCOUNTER 2
#define GAMESCENE 3
#define controlsLeft 0
#define controlsUp 1
#define controlsRight 2
#define controlsDown 3
#define controlsShootLeft 4
#define controlsShootUp 5
#define controlsShootRight 6
#define controlsShootDown 7
#define controlsDash 8
#define controlsStart 9


#define TRANIN 0
#define TRANOUT 1

struct Attributes{
    
	int currentResolution = 1;
	bool fullscreen = false;
	bool currentCRTMode = false;
    std::shared_ptr<int> musicVolume;
	std::shared_ptr<int> sfxVolume;
	bool firstTimeInNexus = false;
    std::array<int,13> levelsCompleted;
	std::array<int, 7> gateKeepersSaved;
    std::vector<int> weapons;
	int currentWeapon = 0;
	int currentPowerLevel = 1;
    std::array<sf::Keyboard::Key,10> keyboardControls;
	std::array<std::string, 7> WeaponNames{ 

		"PlayerLaser",
		"PlayerRepeater",
		"null",
		"null",
		"null",
		"null",
		"null" 

	};

	std::array<std::pair<int, sf::Joystick::Axis>, 10> joystickControls;
	std::array<int, 9> currentCharacterScripts;
	std::array<std::string, 10> keyNames{
						"Left"
						,"Up"
						,"Right"
						,"Down"
						,"Shoot Left"
						,"Shoot Up"
						,"Shoot Right"
						,"Shoot Down"
						,"Dash"
						,"Start"
	};

	std::array<std::string, 101> keycodes
	{
		"A",        ///< The A key
		"B",            ///< The B key
		"C",            ///< The C key
		"D",            ///< The D key
		"E",            ///< The E key
		"F",            ///< The F key
		"G",            ///< The G key
		"H",            ///< The H key
		"I",            ///< The I key
		"J",            ///< The J key
		"K",            ///< The K key
		"L",            ///< The L key
		"M",            ///< The M key
		"N",            ///< The N key
		"O",            ///< The O key
		"P",            ///< The P key
		"Q",            ///< The Q key
		"R",            ///< The R key
		"S",            ///< The S key
		"T",            ///< The T key
		"U",            ///< The U key
		"V",            ///< The V key
		"W",            ///< The W key
		"X",            ///< The X key
		"Y",            ///< The Y key
		"Z",            ///< The Z key
		"Num0",         ///< The 0 key
		"Num1",         ///< The 1 key
		"Num2",         ///< The 2 key
		"Num3",         ///< The 3 key
		"Num4",         ///< The 4 key
		"Num5",         ///< The 5 key
		"Num6",         ///< The 6 key
		"Num7",         ///< The 7 key
		"Num8",         ///< The 8 key
		"Num9",         ///< The 9 key
		"Escape",       ///< The Escape key
		"LControl",     ///< The left Control key
		"LShift",       ///< The left Shift key
		"LAlt",         ///< The left Alt key
		"LSystem",      ///< The left OS specific key: window (Windows and Linux), apple (MacOS X), ...
		"RControl",     ///< The right Control key
		"RShift",       ///< The right Shift key
		"RAlt",         ///< The right Alt key
		"RSystem",      ///< The right OS specific key: window (Windows and Linux), apple (MacOS X), ...
		"Menu",         ///< The Menu key
		"LBracket",     ///< The [ key
		"RBracket",     ///< The ] key
		"SemiColon",    ///< The ; key
		"Comma",        ///< The , key
		"Period",       ///< The . key
		"Quote",        ///< The ' key
		"Slash",        ///< The / key
		"BackSlash",    ///< The \ key
		"Tilde",        ///< The ~ key
		"Equal",        ///< The = key
		"Dash",         ///< The - key
		"Space",        ///< The Space key
		"Return",       ///< The Return key
		"BackSpace",    ///< The Backspace key
		"Tab",          ///< The Tabulation key
		"PageUp",       ///< The Page up key
		"PageDown",     ///< The Page down key
		"End",          ///< The End key
		"Home",         ///< The Home key
		"Insert",       ///< The Insert key
		"Delete",       ///< The Delete key
		"Add",          ///< The + key
		"Subtract",     ///< The - key
		"Multiply",     ///< The * key
		"Divide",       ///< The / key
		"Left",         ///< Left arrow
		"Right",        ///< Right arrow
		"Up",           ///< Up arrow
		"Down",         ///< Down arrow
		"Numpad0",      ///< The numpad 0 key
		"Numpad1",      ///< The numpad 1 key
		"Numpad2",      ///< The numpad 2 key
		"Numpad3",      ///< The numpad 3 key
		"Numpad4",      ///< The numpad 4 key
		"Numpad5",      ///< The numpad 5 key
		"Numpad6",      ///< The numpad 6 key
		"Numpad7",      ///< The numpad 7 key
		"Numpad8",      ///< The numpad 8 key
		"Numpad9",      ///< The numpad 9 key
		"F1",           ///< The F1 key
		"F2",           ///< The F2 key
		"F3",           ///< The F3 key
		"F4",           ///< The F4 key
		"F5",           ///< The F5 key
		"F6",           ///< The F6 key
		"F7",           ///< The F7 key
		"F8",           ///< The F8 key
		"F9",           ///< The F9 key
		"F10",          ///< The F10 key
		"F11",          ///< The F11 key
		"F12",          ///< The F12 key
		"F13",          ///< The F13 key
		"F14",          ///< The F14 key
		"F15",          ///< The F15 key
		"Pause",        ///< The Pause key
	};


	bool joystick = true;
    int currentLevel = 0;
	int maxMana = 100;

    
};

struct Transition{
    
    Transition(std::string mapName = "")
    {
        if(mapName != "") destination = mapName;
		tranOverlay.setSize(sf::Vector2f(1000, 1000));
		tranOverlay.setOrigin(500, 500);
		tranOverlay.setFillColor(sf::Color::Black);
    }

    std::string destination = "";
	void Update();
	void LoadScene();
	virtual void FadeIn(){}
	virtual void FadeOut(){}
	virtual void Draw();
    
    bool isDone = false;
	bool hasLoadedScene = false;
	sf::RectangleShape tranOverlay;


    
};

struct Fade : public Transition {

	Fade(std::string mapName = "", bool dir = TRANIN);
	float spriteFrameCount = 0;
	float transparency = 0;
	void FadeIn();
	void FadeOut();
	void Draw();
	bool direction = TRANOUT;
	sf::RectangleShape overlay;


};

struct BlockFade : public Transition{
    
    BlockFade(std::string mapName = "", bool dir = TRANIN);
	void FadeIn();
	void FadeOut();
	void Draw();
    bool direction = TRANOUT;
	sf::Sprite tile;
	int fadeProgress = 0;
	sf::Vector2f camPos;
	int spriteSize = 30;
	int maxWidth = 600; //should be divisible by spriteSize
	int maxHeight = 450; //should be divisible by spriteSize
    
    
};



struct Scene{
    
    Scene();
    ~Scene();
    Audio audioContainer;
    Textures::TextureContainer textureContainer;
    std::unique_ptr<Container> objectContainer;
    std::unique_ptr<Level::LevelContainer> levelContainer;
    std::unique_ptr<Transition> transition;
    Entity::Player* playerPtr;
    Entity::Hud* hudPtr;
	Entity::Object* UIPtr;
	Entity::Guide* guidePtr;

    bool isLoaded = false;

};

struct SceneScript{
    
    SceneScript(std::string tempname = "map1", int tempmapType = 1, float tempduration = 0);
    std::string name;
    int mapType;
    float duration = 0;
};

struct TimeObject{
    
    TimeObject(){
        
    }
    
    ~TimeObject(){
        
    }
    
    float timer = 0;
    float maxTime = 0;
    bool done = false;
    bool isActive = true;
    int type = DELAY;
    
};

int RoundUp(int numToRound, int multiple = 16);
int RoundDown(int numToRound, int multiple = 100);
void RotateVector(sf::Vector2f& coords,int degree = 45);
bool IsOverlapping(int minA, int maxA, int minB, int maxB);
float GetAngle(sf::Vector2f a, sf::Vector2f b);
int RandomNumber(int max, int min = 0);
bool IfDistance(sf::Vector2f a, sf::Vector2f b, int distance = 15);
float GetDistance(sf::Vector2f a, sf::Vector2f b);

float Sine();

class World{
    
    public:
    
    friend struct Transition;
    
    World();
    static World *Worldptr;
    void Setup(sf::Clock& clock,sf::RenderWindow& window, sf::Event& events);
    void Run(sf::Event& event, float timestamp, sf::Clock& clock);
    void ScreenShake(int strength);
    void UpdateCamera();
    void SetCameraTarget(Entity::Object& target);
    bool IsPlayerActive();
	bool IsPlayerCameraTarget();
    void ResetCamera();
    void ReadyScene(std::string mapName);
    void UpdateTransition();
    void UpdateTime(float timestamp);
    void RemoveTimeObjects();
    void ResetScene();
    void LoadSceneBank();
    void CreateCharacterScripts();
    void DrawObject(sf::Drawable& sprite, std::string shader = "");
	bool PlayerPressedButton(int button,bool menuAction = false);
	bool PlayerPressedActionButton();
	void MapController(int id, int vendor);

	// For contoller setup
	// Controller presets


	/*

	Setting controls

	----

	0 = left
	1 = up
	2 = right
	3 = down
	4 = shoot left
	5 = shoot up
	6 = shoot right
	7 = shoot down
	8 = dash
	9 = pause;

	*/

	std::array<sf::Keyboard::Key, 10> DefaultKeyboardMaps = { 
		sf::Keyboard::A,
		sf::Keyboard::W,
		sf::Keyboard::D,
		sf::Keyboard::S,
		sf::Keyboard::J,
		sf::Keyboard::I,
		sf::Keyboard::L,
		sf::Keyboard::K,
		sf::Keyboard::RShift,
		sf::Keyboard::P
	};
	
	std::array<std::pair<int,sf::Joystick::Axis>, 10> DefaultControllerMaps = {

		std::make_pair(-100,sf::Joystick::Axis::X),
		std::make_pair(-100,sf::Joystick::Axis::Y),
		std::make_pair(100,sf::Joystick::Axis::X),
		std::make_pair(100,sf::Joystick::Axis::Y),
		std::make_pair(3,static_cast<sf::Joystick::Axis>(-1)),
		std::make_pair(0,static_cast<sf::Joystick::Axis>(-1)),
		std::make_pair(1,static_cast<sf::Joystick::Axis>(-1)),
		std::make_pair(2,static_cast<sf::Joystick::Axis>(-1)),
		std::make_pair(5,static_cast<sf::Joystick::Axis>(-1)),
		std::make_pair(9,static_cast<sf::Joystick::Axis>(-1)),
	};

	std::array<std::pair<int, sf::Joystick::Axis>, 10> XboxKeyMaps = {

		std::make_pair(-100,sf::Joystick::Axis::PovX),
		std::make_pair(100,sf::Joystick::Axis::PovY),
		std::make_pair(100,sf::Joystick::Axis::PovX),
		std::make_pair(-100,sf::Joystick::Axis::PovY),
		std::make_pair(2,static_cast<sf::Joystick::Axis>(-1)),
		std::make_pair(3,static_cast<sf::Joystick::Axis>(-1)),
		std::make_pair(1,static_cast<sf::Joystick::Axis>(-1)),
		std::make_pair(0,static_cast<sf::Joystick::Axis>(-1)),
		std::make_pair(-100,sf::Joystick::Axis::Z),
		std::make_pair(7,static_cast<sf::Joystick::Axis>(-1)),
	};


	bool usingController = false;
	void DetectController();

    static World *GetInstance(){
        
        if(!Worldptr) Worldptr = new World;
        return Worldptr;
        
    }
    
    SceneScript GetScene(std::string sceneName);
    std::map<std::string,SceneScript> SceneScriptBank;
    std::shared_ptr<SceneScript> CurrentScene;
    
    template<class T, typename I = int, typename O = int>
    bool Timer(T& obj, I time = NORMAL, O type = DELAY)
    {
        std::stringstream ss;
        ss << std::hex << &obj;
        const std::string s = ss.str();
        
        bool timesUp = false;
        bool thistime = false;
        
        //std::cout << "Timer called" << std::endl;
        
        if((TimeArray.find(s) != TimeArray.end())){
            
            timeFindit it = TimeArray.equal_range(s);
            
            for(timeIt iter = it.first; iter != it.second; iter++){
                
                //std::cout << "Timer(" << iter->second.timer << "/" << iter->second.maxTime << ")" << std::endl;
                
                if(iter->second.maxTime == time){
                    
                    if(iter->second.timer >= time){
                        
                        //std::cout << "Timer(" << iter->second.timer << "/" << iter->second.maxTime << ") is done" << std::endl;
                        if(iter->second.type == DELAY){
                            
                            timesUp = true;
                            iter->second.done = true;
                                
                        }
                        
                        if(iter->second.type == NODELAY){
                            
                            iter->second.done = true;
                            
                        }
                        
                    }
                    
                    thistime = true;
                    
                }
                
                iter->second.isActive = true;
                
                
            }
            
            
        }
        
        
        if(TimeArray.find(s) == TimeArray.end() || thistime == false ){
            
            //std::cout << "Added Timer(" << time << ")" << std::endl;
            
            TimeObject temp;
            temp.maxTime = time;
            temp.type = type;
            
            if(type == NODELAY){
                
                timesUp = true;
                //temp.done = true;
                
            }
            
            TimeArray.insert ( std::pair<std::string,TimeObject>(s,temp) );
            
            
        }
        
        
        
        return timesUp;
    }
    
    Scene WorldScene;
    void LoadScene(std::string sceneName = "map1");
    
    bool created = false;
    
    //Camera  & members
    sf::View* GetView();
    sf::FloatRect screenPos;
    sf::View Screen;
    sf::Vector2f viewPos;
    float camOrient= 0;
    float vel = 0;
    bool isShaking = false;
    int quakeStrength = 20;
    Entity::Object *CameraTarget;
	int worldShader = 0;
    
    Attributes GlobalMembers;
    std::map<std::string,std::string> CharacterScripts;
    
    sf::Event *eventWorld;
    sf::RenderWindow *windowWorld;
    sf::Clock *clockWorld;
    static sf::Clock clock2;
	std::string dir;
	std::string resourceFile;
    
    std::unique_ptr<Transition> TransitionOverlay;
    
    
    std::multimap<std::string,TimeObject> TimeArray;
    typedef std::multimap<std::string,TimeObject>::iterator timeIt;
    
    typedef std::pair <std::multimap<std::string,TimeObject>::iterator, std::multimap<std::string,TimeObject>::iterator> timeFindit;
    
};

extern World gameWorld;


#endif /* defined(__HardSoda__World__) */