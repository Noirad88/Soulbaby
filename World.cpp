//
//  World.cpp
//  HardSoda
//
//  Created by Darion McCoy on 6/18/15.
//  Copyright (c) 2015 Darion. All rights reserved.
//

#include "World.h"
#include "Container.h"


class Container;

sf::Clock World::clock2;

void Transition::Update() {

	if (!hasLoadedScene) {
		FadeOut();

	}

	else FadeIn();
	Draw();

}

void Transition::LoadScene() {

	World::GetInstance()->LoadScene(destination);
	hasLoadedScene = true;

}

void Transition::Draw() {

	tranOverlay.setPosition(World::GetInstance()->Screen.getCenter().x, World::GetInstance()->Screen.getCenter().y);
	World::GetInstance()->windowWorld->draw(tranOverlay);

}


Fade::Fade(std::string mapName,bool dir ) : Transition(mapName)
{

    // OUT closes level
    // IN opens level
    
    direction = dir;
    overlay.setPosition(0,0);
    overlay.setSize(sf::Vector2f(1000,1000));
	overlay.setOrigin(500, 500);
    overlay.setFillColor(sf::Color(50,0,0,transparency));
    transparency = (direction == TRANIN) ? 255 : 0;

}

void Fade::FadeIn() {

	transparency -= 5;

	if (transparency <= 0) isDone = true;

	overlay.setPosition(World::GetInstance()->Screen.getCenter().x, World::GetInstance()->Screen.getCenter().y);

}

void Fade::FadeOut() {

	transparency += 5;

	if (transparency >= 255) {
		LoadScene();
		transparency = 255;
	}

	overlay.setPosition(World::GetInstance()->Screen.getCenter().x, World::GetInstance()->Screen.getCenter().y);


}

void Fade::Draw() {

	overlay.setFillColor(sf::Color(0, 0, 0, transparency));
	World::GetInstance()->windowWorld->draw(overlay);
}

BlockFade::BlockFade(std::string mapName, bool dir) : Transition(mapName)
{

	tile.setTexture(World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_misc"));
	tile.setTextureRect(sf::IntRect(48, 48, 32, 32));

}

void BlockFade::FadeIn() {
	
	tile.setPosition(World::GetInstance()->Screen.getCenter().x - 240 - 90, World::GetInstance()->Screen.getCenter().y - 135 - 90);
	camPos = tile.getPosition();

	if(fadeProgress == 0) Transition::Draw();

	if (fadeProgress < maxWidth * 2) {

		if (World::GetInstance()->Timer(*this,NORMAL)) {

			fadeProgress += spriteSize;

		}
	} 

	else {

		isDone = true;
	}



}

void BlockFade::FadeOut() {

	tile.setPosition(World::GetInstance()->Screen.getCenter().x - 240 - 90, World::GetInstance()->Screen.getCenter().y - 135 - 90);

	camPos = tile.getPosition();

	if (fadeProgress < maxWidth*2) {

		if (World::GetInstance()->Timer(*this,NORMAL)) {

			fadeProgress += spriteSize;

		}
	}

	else {

		LoadScene();
		Transition::Draw();
		fadeProgress = 0;
	}

	//std::cout << "tile pos(" << tile.getPosition().x << "," << tile.getPosition().y << ")" << std::endl;

}

void BlockFade::Draw() {

	if (!hasLoadedScene) {

		for (int i = 0; i != fadeProgress; i += spriteSize) {

			for (int p = 0; p != maxHeight; p += spriteSize) {

				int frame = fadeProgress - i;
				if (frame > 180) frame = 180;

				tile.setPosition(camPos.x + i, camPos.y + p);
				tile.setTextureRect(sf::IntRect(48 + frame, 48, spriteSize, spriteSize));
				World::GetInstance()->windowWorld->draw(tile);

			}


		}
	}

	else {

		for (int i = 0; i <= maxWidth; i += spriteSize) {

			for (int p = 0; p != maxHeight; p += spriteSize) {

				int frame = fadeProgress - i;
				if (frame > 180) frame = 180;
				if (i >= fadeProgress) frame = 0;

				tile.setPosition(camPos.x + i, camPos.y + p);
				tile.setTextureRect(sf::IntRect(228 - frame, 48, spriteSize, spriteSize));
				World::GetInstance()->windowWorld->draw(tile);

			}


		}




	}

}

void World::UpdateTransition(){
    
    WorldScene.transition->Update();
    
	if (WorldScene.transition->isDone) {

		WorldScene.transition.reset();

	}
    

}

World::World(){
    
    /*
     
    Setting controls
     
    ----
     
    0 = left
    1 = up
    2 = right
    3 = down
    4 = A
    5 = B
    6 = Pause
     
    */
    
    GlobalMembers.keyboardControls[0] = sf::Keyboard::Left;
    GlobalMembers.keyboardControls[1] = sf::Keyboard::Up;
    GlobalMembers.keyboardControls[2] = sf::Keyboard::Right;
    GlobalMembers.keyboardControls[3] = sf::Keyboard::Down;
    GlobalMembers.keyboardControls[4] = sf::Keyboard::Z;
    GlobalMembers.keyboardControls[5] = sf::Keyboard::X;
    GlobalMembers.keyboardControls[6] = sf::Keyboard::C;
	GlobalMembers.keyboardControls[7] = sf::Keyboard::P;
	GlobalMembers.keyboardControls[8] = sf::Keyboard::V;

	GlobalMembers.joystickControls[0] = -100;
	GlobalMembers.joystickControls[1] = -100;
	GlobalMembers.joystickControls[2] = 100;
	GlobalMembers.joystickControls[3] = 100;
	GlobalMembers.joystickControls[4] = 4;
	GlobalMembers.joystickControls[5] = 3;
	GlobalMembers.joystickControls[6] = 0;
	GlobalMembers.joystickControls[7] = 0;
	GlobalMembers.joystickControls[8] = 1;

	dir = "C:/Users/Darion/Documents/Visual Studio 2015/Projects/SoulbabyPC/SoulbabyPC";

	/*
	0 = repeater
	5 = spreader
	*/

	GlobalMembers.weapons.reserve(7);
	GlobalMembers.weapons.push_back(0);
	GlobalMembers.weapons.push_back(0);
	GlobalMembers.weapons.push_back(0);
	GlobalMembers.weapons.push_back(0);
	GlobalMembers.weapons.push_back(0);
	GlobalMembers.weapons.push_back(0);
	GlobalMembers.weapons.push_back(0);

	GlobalMembers.weapons.at(0) = 0;
	GlobalMembers.weapons.at(1) = 0;
	GlobalMembers.weapons.at(2) = 0;
	GlobalMembers.weapons.at(3) = 0;
	GlobalMembers.weapons.at(4) = 0;
	GlobalMembers.weapons.at(5) = 0;
	GlobalMembers.weapons.at(6) = 0;

}

void World::Setup(sf::Clock &clock, sf::RenderWindow &window, sf::Event &events){
    
    GlobalMembers.maxVolume = std::shared_ptr<int> (new int(80));
    GlobalMembers.health = std::shared_ptr<int> (new int(30));
    GlobalMembers.textSpeed = std::shared_ptr<int> (new int(5));
	Screen.setSize(WINDOW_X/3, WINDOW_Y/3);

    
    clockWorld = &clock;
    windowWorld = &window;
    eventWorld = &events;
    CurrentScene = std::shared_ptr<SceneScript>(new SceneScript);
    WorldScene.transition.reset();
    
    CreateCharacterScripts();
    GlobalMembers.levelsCompleted.fill(0);
	GlobalMembers.levelsCompleted.at(0) = 1;

	GlobalMembers.gateKeepersSaved.fill(0);
	GlobalMembers.gateKeepersSaved.at(0) = 1;
	GlobalMembers.currentCharacterScripts.fill(0);


	WorldScene.textureContainer.CreateShaderInstances();
	WorldScene.textureContainer.gameView = &Screen;
    LoadSceneBank();

	/*
	Scenes:
	Nexus "map2_1"
	Gamescene "gameScene0" - "gameScene7"
	Level "battle"
	Menu "menu"
	*/

	LoadScene("menu");
	testShape.setFillColor(sf::Color::Blue);
	testShape.setSize(sf::Vector2f(200,200));

	sf::Joystick::update();
	bool joystick = sf::Joystick::isConnected(0);
	int buttons = sf::Joystick::getButtonCount(0);
	int axisx = sf::Joystick::hasAxis(0,sf::Joystick::Axis::X);
	int axisy = sf::Joystick::hasAxis(0, sf::Joystick::Axis::Y);


	std::cout << "Joystick? : " << joystick << std::endl;
	std::cout << "Button? : " << buttons << std::endl;
	std::cout << "Axis? : " << axisx << ", " << axisy << std::endl;

}

void World::ReadyScene(std::string mapName){
    
   WorldScene.isLoaded = false;
   if(mapName == "menu" || mapName == "gameScene0" ) WorldScene.transition = std::unique_ptr<Transition>(new Fade(mapName, TRANOUT));
   else WorldScene.transition = std::unique_ptr<Transition>(new BlockFade(mapName, TRANOUT));
   //turn music down
   World::GetInstance()->WorldScene.audioContainer.MusicFadeOut();


}

void World::LoadScene(std::string sceneName){
    
        clockWorld->restart();
        ResetScene();
        SceneScript currentScript = SceneScriptBank.at(sceneName);
        *CurrentScene = currentScript;
        std::cout << "Loading " << CurrentScene->name << std::endl;
        WorldScene.levelContainer->CreateScene();
        WorldScene.isLoaded = true;
        std::cout << "Done Loading" << CurrentScene->name << std::endl;
		// turn music back up
		World::GetInstance()->WorldScene.audioContainer.MusicFadeIn();




}

void World::LoadSceneBank(){
    
	SceneScript intro0("intro0", GAMESCENE);
	SceneScript intro1("intro1", GAMESCENE);
    SceneScript Menu("menu",MENU);
    SceneScript Map2("map2_1");
    SceneScript Battle("battle",ENCOUNTER);
	SceneScript gameScene("gamescene0", GAMESCENE);
	SceneScript gameScene1("gamescene1", GAMESCENE);
	SceneScript gameScene2("gamescene2", GAMESCENE);
	SceneScript gameScene3("gamescene3", GAMESCENE);
	SceneScript gameScene4("gamescene4", GAMESCENE);
	SceneScript gameScene5("gamescene5", GAMESCENE);
	SceneScript gameScene6("gamescene6", GAMESCENE);

	SceneScriptBank.insert(std::pair<std::string, SceneScript>("intro0", intro0));
	SceneScriptBank.insert(std::pair<std::string, SceneScript>("intro1", intro1));
    SceneScriptBank.insert(std::pair<std::string,SceneScript>("menu",Menu));
    SceneScriptBank.insert(std::pair<std::string,SceneScript>("map2_1",Map2));
    SceneScriptBank.insert(std::pair<std::string,SceneScript>("battle",Battle));
	SceneScriptBank.insert(std::pair<std::string, SceneScript>("gameScene0",gameScene));
	SceneScriptBank.insert(std::pair<std::string, SceneScript>("gameScene1", gameScene1));
	SceneScriptBank.insert(std::pair<std::string, SceneScript>("gameScene2", gameScene2));
	SceneScriptBank.insert(std::pair<std::string, SceneScript>("gameScene3", gameScene3));
	SceneScriptBank.insert(std::pair<std::string, SceneScript>("gameScene4", gameScene4));
	SceneScriptBank.insert(std::pair<std::string, SceneScript>("gameScene5", gameScene5));
	SceneScriptBank.insert(std::pair<std::string, SceneScript>("gameScene6", gameScene6));
    
}

void World::ResetScene(){
    
    WorldScene.isLoaded = false;
    WorldScene.objectContainer->ObjectContainer.clear();
    WorldScene.levelContainer->levelArray.clear();
    WorldScene.levelContainer->LvlContainer.clear();
    
}

void World::Run(sf::Event& event, float timestamp, sf::Clock& clock) {

	UpdateTime(timestamp);
	eventWorld = &event;

	//sf::Time test = WorldScene.audioContainer.music.getPlayingOffset();

	//if (test.asMilliseconds() >= 6850)  WorldScene.audioContainer.music.play();
	//std::cout << test.asMilliseconds() << std::endl;

	if (Timer(*this, 100.0f)) {


		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num7)) {

			ReadyScene("menu");

		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::T)) {

			ReadyScene("battle");

		}

		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num8)) {

			ReadyScene("map1");

		}

		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num5)) {

			ReadyScene("map2_1");

		}

	}

	WorldScene.objectContainer->AddObjects();
	WorldScene.objectContainer->RemoveObjects();
	WorldScene.objectContainer->UpdateObjects();
	WorldScene.audioContainer.Update();
	WorldScene.levelContainer->Update();
	WorldScene.textureContainer.Update();
	WorldScene.objectContainer->CheckCollisions();
	WorldScene.levelContainer->CheckCollisions();

	UpdateCamera(); 

	WorldScene.levelContainer->DrawLevel();
	WorldScene.objectContainer->DrawObjects(*windowWorld, *this);

	if (CurrentScene->mapType == MAP) WorldScene.levelContainer->DrawBGTop();

	if(WorldScene.UIPtr) WorldScene.UIPtr->Draw(*windowWorld);
    
    if(WorldScene.transition) UpdateTransition();
     
     RemoveTimeObjects();

	 testShape.setPosition(viewPos.x, viewPos.y);
}

void World::UpdateTime(float timestamp){
    
    if(TimeArray.size() > 0){
        
        for(timeIt it = TimeArray.begin(); it != TimeArray.end(); ++it){
            
            if(it->second.isActive == false) it->second.done = true;
            else {it->second.timer += timestamp; it->second.isActive = false;}
        
            
        }
        
    }
}

void World::RemoveTimeObjects(){
    
    if(TimeArray.size() > 0){
        
        for(timeIt it = TimeArray.begin(); it != TimeArray.end();){
            
            if(it->second.done == true){
                
                TimeArray.erase(it++);
                
            }
            
            else{
                
                it++;
            }
            
        }

    }
    
}

void World::ScreenShake(int strength){
    
    quakeStrength = strength;
    isShaking = true;
 
}

void World::UpdateCamera(){
    
	if (WorldScene.isLoaded) {

		if (CurrentScene->mapType == MAP || ENCOUNTER) {

			//center the camera on player

			camOrient = 0;
			vel = 20;

			//

			float playerPosX = 0;
			float playerPosY = 0;

			if (CameraTarget) {

				playerPosX = CameraTarget->objectSprite.getPosition().x + camOrient;
				playerPosY = CameraTarget->objectSprite.getPosition().y + camOrient;

			}

			else {

				playerPosX = Screen.getCenter().x;
				playerPosY = Screen.getCenter().y;

			}


			//Update the camera position
			if (CameraTarget) {

				viewPos.x += (playerPosX - Screen.getCenter().x) / vel;
				viewPos.y += (playerPosY - Screen.getCenter().y) / vel;
				Screen.setCenter(double(viewPos.x), double(viewPos.y));

			}

			sf::Vector2f view = Screen.getCenter();
			sf::Vector2f size = Screen.getSize();

			if (CurrentScene->mapType == ENCOUNTER) {

				if (!WorldScene.transition) {

					// If camera is at the end of the level on the left, stop the camera

					if (view.x < size.x / 2) {

						viewPos.x = size.x / 2;
						Screen.setCenter(size.x / 2, viewPos.y);
						camOrient = 0;
						vel = 0;

					}

					// If right


					else if (view.x > WorldScene.levelContainer->lvlSize.x - size.x / 2) {

						viewPos.x = WorldScene.levelContainer->lvlSize.x - size.x / 2;
						Screen.setCenter(WorldScene.levelContainer->lvlSize.x - size.x / 2, viewPos.y);
						camOrient = 0;
						vel = 0;

					}


					if (view.y > WorldScene.levelContainer->lvlSize.y - size.y / 2) {

						viewPos.y = WorldScene.levelContainer->lvlSize.y - size.y / 2;
						Screen.setCenter(viewPos.x, WorldScene.levelContainer->lvlSize.y - size.y / 2);
						camOrient = 0;
						vel = 0;

					}

					else if (view.y < size.y / 2) {

						viewPos.y = size.y / 2;
						Screen.setCenter(viewPos.x, size.y / 2);
						camOrient = 0;
						vel = 0;

					}
				}
				

			}

			//Pass viewport data to level

			WorldScene.levelContainer->view = view;

			// If called to shake the camera ...

			if (isShaking) {

				//Lower music volume on screenshake


				//if ScreenShake() has been called, shake the screen until back to original position

				int x = (rand() % quakeStrength) + viewPos.x;
				int y = rand() % quakeStrength + viewPos.y;

				Screen.setCenter(x, y);
				quakeStrength -= 0.5;

				if (quakeStrength <= 0) {

					isShaking = false;

				}

			}

		}

		if (CurrentScene->mapType == MENU || CurrentScene->mapType == GAMESCENE) {

			viewPos.x = 240;
			viewPos.y = 135;
			Screen.setCenter(240, 135);
			camOrient = 0;
			vel = 0;


		}
	}

    
}

void World::SetCameraTarget(Entity::Object& target){
    
    CameraTarget = &target;
    
}

bool World::IsPlayerActive(){
    
    return (CameraTarget == WorldScene.playerPtr) && (!World::GetInstance()->WorldScene.UIPtr
	) && (!WorldScene.transition);
    
}

bool World::IsPlayerCameraTarget() {

	return (CameraTarget == WorldScene.playerPtr);

}

void World::ResetCamera(){
    
    CameraTarget = WorldScene.playerPtr;
    
}


sf::View* World::GetView(){
    
    return &Screen;
    
}

void World::CreateCharacterScripts(){
    
	std::string hawzer0 = "We've been able to talk for a while now.>but I just never really know what to say? God dang bro, why do you do this to me! OK dawg, I guess I do have something to talk about: I'm starting a restaurant down the hall, it's a pizza joint - AUTHENTIC BABY!!>OK, so here is the kicker: I need a name for the mascot; what do you think about ...>... Charles Entertainment Cheese?";
	std::string jira0 = "Well ...>I don't really have much to say, tbh.>The card, I open it and I now know what it means to hear terrified numbness being slid from out of a pouch which contains little nuggets of something and, still, all I see is you search it all, like you’re looking for the rotten bit. I know what’s coming, it’s the same thing as always : tears and yours, not mine. Never mine.";
	std::string doran0 = "Woo!>Do you know how hard it is to do this junk?>... Without thumbs?>Lord, you humans are so privledged. The destruction of the galaxy doesn't even phase your arrogance.";
	std::string gurian0 = "Hmmm. Who do I have to fuck for that job?";
    std::string lima0 = "FUCK ... OFF";
	std::string mother0 = "...>Ahh!>So, you're here ... I'm sure you have questions. There is a lot going on.>My journey has come to an unexpected end. I will tell you my story.";
	std::string note0 = "Learned Weapon2!";
	std::string note1 = "Learned Weapon3!";
	std::string note2 = "Learned Weapon4!";
	std::string note3 = "Learned Weapon5!";
	std::string note4 = "Learned Weapon6!";
	std::string note5 = "Learned Weapon7!";
	std::string note6 = "A soul seed was dropped ...>You can use this to increse your strength or revive the gate keeper to their true form.>What do you want to do?";
	std::string note7 = "Restart or return to the Nexus?";




    CharacterScripts.insert(std::pair<std::string,std::string>("HAWZER0",hawzer0));
    CharacterScripts.insert(std::pair<std::string,std::string>("JIRA0",jira0));
	CharacterScripts.insert(std::pair<std::string, std::string>("DORAN0", doran0));
    CharacterScripts.insert(std::pair<std::string,std::string>("GURIAN0",gurian0));
    CharacterScripts.insert(std::pair<std::string,std::string>("LIMA0",lima0));
	CharacterScripts.insert(std::pair<std::string, std::string>("MOTHER0", mother0));
	CharacterScripts.insert(std::pair<std::string, std::string>("NOTE0", note0));
	CharacterScripts.insert(std::pair<std::string, std::string>("NOTE1", note1));
	CharacterScripts.insert(std::pair<std::string, std::string>("NOTE2", note2));
	CharacterScripts.insert(std::pair<std::string, std::string>("NOTE3", note3));
	CharacterScripts.insert(std::pair<std::string, std::string>("NOTE4", note4));
	CharacterScripts.insert(std::pair<std::string, std::string>("NOTE5", note5));
	CharacterScripts.insert(std::pair<std::string, std::string>("NOTE6", note6));
	CharacterScripts.insert(std::pair<std::string, std::string>("NOTE7", note7));



    
}

void World::DrawObject(sf::Drawable& sprite, std::string shader){
    
	if(shader == "bypass") windowWorld->draw(sprite);
	else if (WorldScene.textureContainer.dimWorld == true) windowWorld->draw(sprite, &WorldScene.textureContainer.shaders.at("dimShader"));
    else if(WorldScene.playerPtr->dead) windowWorld->draw(sprite,&WorldScene.textureContainer.shaders.at("redShader"));
	else if(worldShader == 1) windowWorld->draw(sprite, &WorldScene.textureContainer.shaders.at("glitchShader"));
    else if (shader != "") windowWorld->draw(sprite,&WorldScene.textureContainer.shaders.at(shader));
    else windowWorld->draw(sprite);

}

////////////// SCENES /////

Scene::Scene(){
    
    levelContainer = std::unique_ptr<Level::LevelContainer>(new Level::LevelContainer);
    objectContainer = std::unique_ptr<Container>(new Container);
    //transition = std::unique_ptr<Transition>(new Transition);
    
}

Scene::~Scene(){
    
}

SceneScript::SceneScript(std::string tempname, int tempmapType, float tempduration){
    
    name = tempname;
    mapType = tempmapType;
    duration = tempduration;
    
}

bool World::PlayerPressedButton(int button) {

	bool buttonPressed = false;
	sf::Joystick::update();

	if ((WorldScene.playerPtr != nullptr && IsPlayerActive())) {

		if (sf::Joystick::isConnected(0)) {

			if (button == controlsUp
				|| button == controlsDown) {

				if (sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::Y) == World::GetInstance()->GlobalMembers.joystickControls[button]) buttonPressed = true;

			}

			else if (button == controlsLeft
				|| button == controlsRight) {

				if (sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::X) == World::GetInstance()->GlobalMembers.joystickControls[button]) buttonPressed = true;


			}

			else buttonPressed = sf::Joystick::isButtonPressed(0, World::GetInstance()->GlobalMembers.joystickControls[button]);

		}

		else buttonPressed = sf::Keyboard::isKeyPressed(World::GetInstance()->GlobalMembers.keyboardControls[button]);

	}
	
	return buttonPressed;

}

bool World::PressedButtonforUI(int button) {

	bool buttonPressed = false;
	sf::Joystick::update();


		if (sf::Joystick::isConnected(0)) {

			if (button == controlsUp
				|| button == controlsDown) {

				if (sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::Y) == World::GetInstance()->GlobalMembers.joystickControls[button]) buttonPressed = true;

			}

			else if (button == controlsLeft
				|| button == controlsRight) {

				if (sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::X) == World::GetInstance()->GlobalMembers.joystickControls[button]) buttonPressed = true;


			}

			else buttonPressed = sf::Joystick::isButtonPressed(0, World::GetInstance()->GlobalMembers.joystickControls[button]);

		}

		else buttonPressed = sf::Keyboard::isKeyPressed(World::GetInstance()->GlobalMembers.keyboardControls[button]);


	return buttonPressed;

}

int RoundUp(int numToRound, int multiple)
{
    if (multiple == 0) return numToRound;
    
    int remainder = abs(numToRound) % multiple;
    
    if (remainder == 0) return numToRound;
    
    if (numToRound < 0) return -(abs(numToRound) - remainder);
    
    return numToRound + multiple - remainder;
}

int RoundDown(int numToRound, int multiple)
{
    if (multiple == 0) return numToRound;
    
    int remainder = abs(numToRound) % multiple;
    
    if (remainder == 0) return numToRound;
    
    if (numToRound < 0) return -(abs(numToRound) - remainder);
    
    return (abs(numToRound) - remainder);
}

void RotateVector(sf::Vector2f& coords, int degree){
    
    double angle = degree * M_PI / 180.0;
    double cs = cos(angle);
    double sn = sin(angle);
    
    double newX = coords.x * cs - coords.y * sn;
    double newY = coords.x * sn + coords.y * cs;
    
    coords.x = newX;
    coords.y = newY;

    
}

bool IsOverlapping(int minA, int maxA, int minB, int maxB){
    
    return minB < maxA && minA < maxB;
    
}

float GetAngle(sf::Vector2f a, sf::Vector2f b){
    
    float temp3 = a.x - b.x;
    float temp4 = a.y - b.y;
    return atan2(temp3, temp4) * 180 / M_PI;
    
}

std::string GetAddress(Entity::Object& obj){
    
    std::stringstream ss;
    ss << "your id is " << std::hex << &obj;
    const std::string s = ss.str();
    return s;
    
}

int RandomNumber(int max, int min){
    
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> rand(min,max);
    return rand(mt);
    
}

bool IfDistance(sf::Vector2f a, sf::Vector2f b, int distance) {

	return sqrt((a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y)) <= distance ? true : false;


}

float GetDistance(sf::Vector2f a, sf::Vector2f b) {

	return sqrt((a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y));


}

float Sine(){
    
    return 0.06 * sin((World::clock2.getElapsedTime().asSeconds()) * 0.5 * M_PI) + 1;
    
}

