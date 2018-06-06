//
//  Particle.cpp
//  sdev
//
//  Created by Darion McCoy on 1/10/14.
//  Copyright (c) 2014 D man. All rights reserved.
//

#include "Particle.h"
#include "ResourcePath.hpp"
#include <math.h>
#include <memory>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/ref.hpp>

#define VERY_SLOW 200000.0
#define SLOW 100000.0
#define NORMAL 50000
#define FAST 25000.0
#define VERY_FAST 10000.0

namespace Entity
{

	//////////////// GUI & RELATED ////////////////////////////////////////////////////////////////////////////////////
	bool Player::dead = false;
	int MenuItem::count = 0;
	std::vector<MenuItemHolder> Menu::menuContainer;

	sf::Font HitNum::hitFnt;
	int Object::center = 0;
	int Object::bottomCenter = 1;
	int Object::topLeft = 2;
	int PlayerBomb::totalBombs = 0;
	int PlayerBoomerang::totalBoomerangs = 0;
	int PlayerHoming1::count = 0;
	Object* Enemy::closestEnemy = nullptr;

	// array used for wall shooters to call the correct projectile and its behaviors
	// first = speed
	// second = time on screen

	std::array<std::pair<int,float>, 7> BounceCrawler::BehaviorsForWeapons = std::array<std::pair<int,float>, 7> {
		std::make_pair(3,SLOW),
		std::make_pair(1, VERY_SLOW),
		std::make_pair(0, SLOW),
		std::make_pair(0, SLOW),
		std::make_pair(0, SLOW),
		std::make_pair(0, SLOW),
		std::make_pair(0, SLOW),
	};

	int GUI::guiCount = 0;
	int Textbox::lineLength = 55;
	int Textbox::maxLines = 3;
	int Textbox::scriptLength = 0;
	int Textbox::progressSpeed = 20;
	int Textbox::textboxCount = 0;
	std::string Textbox::characters[11] = { "HAWZER","JIRA","DORAN","GURIAN","LIMA", "MOTHER", "NOTE", };

	bool PlayerMenu::menuUp = false;

	MouseTest::MouseTest() : Object() {

		objectSprite.setTexture((World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_misc")));
		objectSprite.setTextureRect(sf::IntRect(154, 15, 13, 8));
	}

	void MouseTest::Update() {

		int mousePosX = sf::Mouse::getPosition().x;
		int mousePosY = sf::Mouse::getPosition().y;

		objectSprite.setPosition(100, 100);
		std::cout << "mouse update" << std::endl;

	}

	void MouseTest::Draw(sf::RenderTarget& window) {

		World::GetInstance()->DrawObject(objectSprite);

	}

	MouseTest::~MouseTest() {
		std::cout << "mouse dead" << std::endl;
	}


	GUI::GUI() : Object() {

		type = "GUI";
		guiCount++;

	}

	void GUI::Update() {

	}

	GUI::~GUI() {

		guiCount--;
		if (guiCount < 0) guiCount = 0;
	}


	//MenuItem should be derived, so each menu item class can have its own draw position, ui, etc
	//Menu updates itself ... only needs to check going back here. menu update then calls the menuItem update thats currently selected


	MenuItem::MenuItem() : GUI() {


		hand.setTexture((World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_misc")));
		hand.setTextureRect(sf::IntRect(154, 15, 13, 8));
		menuLable.setFont(World::GetInstance()->WorldScene.textureContainer.GetFont());
		menuLable.setCharacterSize(16);
		menuLable.setColor(sf::Color(150, 150, 150));
		count++;

	}

	//Menu Item for Start

	MenuItemStart::MenuItemStart() : MenuItem() {

		menuLable.setString("Start");
		menuLable.setPosition(212, 150 + (count * 15));


	}

	void MenuItemStart::Update() {


	}

	void MenuItemStart::Action() {

		World::GetInstance()->ReadyScene("map2_1");
		//World::GetInstance()->ReadyScene("intro0");


	}

	MenuItemSetKey::MenuItemSetKey(int key) {

		assignedKey = key;
		joyAssignment.setCharacterSize(16);
		joyAssignment.setFont(World::GetInstance()->WorldScene.textureContainer.GetFont());
		ResetLables();	
		menuLable.setPosition(160, 20 + (count * 15));
		joyAssignment.setPosition(menuLable.getPosition().x + 100, menuLable.getPosition().y);

	}

	void MenuItemSetKey::Update() {

		if (isAssigningNewKey == true) {

			if (waitASec < 10) waitASec++;

			menuLable.setString(World::GetInstance()->GlobalMembers.keyNames.at(assignedKey) + ": Waiting for input...");
			menuLable.setColor(sf::Color::Green);

			if (waitASec >= 10) {

				World::GetInstance()->windowWorld->pollEvent(*World::GetInstance()->eventWorld);

				std::cout << World::GetInstance()->eventWorld->joystickMove.axis << std::endl;

					if (World::GetInstance()->eventWorld->type == sf::Event::JoystickButtonPressed) {

						std::cout << "joy pressed: " << World::GetInstance()->GlobalMembers.keycodes.at(World::GetInstance()->eventWorld->joystickButton.button) << std::endl;
						int newButton = World::GetInstance()->eventWorld->joystickButton.button;
						sf::Joystick::Axis newAxis = static_cast<sf::Joystick::Axis>(-1);
						
						World::GetInstance()->GlobalMembers.joystickControls.at(assignedKey).first = newButton;
						World::GetInstance()->GlobalMembers.joystickControls.at(assignedKey).second = newAxis;

						isAssigningNewKey = false;
						waitASec = 0;
						menuLable.setColor(sf::Color::White);
						locked = false;
						ResetLables();

					}

					else if (World::GetInstance()->eventWorld->type == sf::Event::JoystickMoved) {

						std::cout << "joymoved pressed: " << World::GetInstance()->GlobalMembers.keycodes.at(World::GetInstance()->eventWorld->joystickMove.axis) << std::endl;
						int newPosition = 0;
						sf::Joystick::Axis newAxis = World::GetInstance()->eventWorld->joystickMove.axis;

						if (World::GetInstance()->eventWorld->joystickMove.position > 0) newPosition = 100;
						else newPosition = -100;
						World::GetInstance()->GlobalMembers.joystickControls.at(assignedKey).first = newPosition;
						World::GetInstance()->GlobalMembers.joystickControls.at(assignedKey).second = newAxis;
						isAssigningNewKey = false;
						waitASec = 0;
						menuLable.setColor(sf::Color::White);
						locked = false;
						ResetLables();

					}

					else if (World::GetInstance()->eventWorld->type == sf::Event::KeyPressed) {

						int joyKey = World::GetInstance()->eventWorld->joystickMove.axis;
						std::cout << joyKey << std::endl;
						sf::Keyboard::Key newKey = World::GetInstance()->eventWorld->key.code;
						World::GetInstance()->GlobalMembers.keyboardControls.at(assignedKey) = newKey;
						isAssigningNewKey = false;
						waitASec = 0;
						menuLable.setColor(sf::Color::White);
						locked = false;
						ResetLables();

					}

				

			}

		}


	}

	void MenuItemSetKey::Action() {

		if (isAssigningNewKey == false) {

			isAssigningNewKey = true;
			locked = true;

		}

	}

	void MenuItemSetKey::ResetLables() {

		menuLable.setString(World::GetInstance()->GlobalMembers.keyNames.at(assignedKey) + ": " + World::GetInstance()->GlobalMembers.keycodes.at(World::GetInstance()->GlobalMembers.keyboardControls.at(assignedKey)));

		if (abs(World::GetInstance()->GlobalMembers.joystickControls.at(assignedKey).first) == 100) {

			joyAssignment.setString("Axis " + std::to_string(World::GetInstance()->GlobalMembers.joystickControls.at(assignedKey).second) + "(" + std::to_string(World::GetInstance()->GlobalMembers.joystickControls.at(assignedKey).first) + ")");

		}

		else joyAssignment.setString("Button " + std::to_string(World::GetInstance()->GlobalMembers.joystickControls.at(assignedKey).first));


	}

	MenuItemControls::MenuItemControls() : MenuItem() {

		menuLable.setString("Control Settings");
		menuLable.setPosition(212, 150 + (count * 15));


	}

	void MenuItemControls::Update() {

	}

	void MenuItemControls::Action() {


		MenuItemHolder menuHolder;
		MenuItemSetKey* item1 = new MenuItemSetKey(0);
		MenuItemSetKey* item2 = new MenuItemSetKey(1);
		MenuItemSetKey* item3 = new MenuItemSetKey(2);
		MenuItemSetKey* item4 = new MenuItemSetKey(3);
		MenuItemSetKey* item5 = new MenuItemSetKey(4);
		MenuItemSetKey* item6 = new MenuItemSetKey(5);
		MenuItemSetKey* item7 = new MenuItemSetKey(6);
		MenuItemSetKey* item8 = new MenuItemSetKey(7);
		MenuItemSetKey* item9 = new MenuItemSetKey(8);
		MenuItemSetKey* item10 = new MenuItemSetKey(9);

		menuHolder.menuList.push_back(item1);
		menuHolder.menuList.push_back(item2);
		menuHolder.menuList.push_back(item3);
		menuHolder.menuList.push_back(item4);
		menuHolder.menuList.push_back(item5);
		menuHolder.menuList.push_back(item6);
		menuHolder.menuList.push_back(item7);
		menuHolder.menuList.push_back(item8);
		menuHolder.menuList.push_back(item9);
		menuHolder.menuList.push_back(item10);

		menuHolder.menuList.at(0)->ToggleSelection();
		Menu::menuContainer.push_back(menuHolder);

	}

	MenuItemCRTMode::MenuItemCRTMode() : MenuItem() {
		std::string crtModeStatus = "Off";
		if(World::GetInstance()->GlobalMembers.currentCRTMode == 1) crtModeStatus = "On";
		menuLable.setString("CRT Mode: " + crtModeStatus);
		menuLable.setPosition(212, 150 + (count * 15));


	}

	void MenuItemCRTMode::Update() {

		if ((World::GetInstance()->PlayerPressedButton(controlsLeft, true))) {

			World::GetInstance()->GlobalMembers.currentCRTMode = !World::GetInstance()->GlobalMembers.currentCRTMode;
				std::string crtModeStatus = "Off";
				if (World::GetInstance()->GlobalMembers.currentCRTMode == 1) crtModeStatus = "On";
				menuLable.setString("CRT Mode: " + crtModeStatus);


		}

		else if ((World::GetInstance()->PlayerPressedButton(controlsRight, true))) {

			World::GetInstance()->GlobalMembers.currentCRTMode = !World::GetInstance()->GlobalMembers.currentCRTMode;
				std::string crtModeStatus = "Off";
				if (World::GetInstance()->GlobalMembers.currentCRTMode == 1) crtModeStatus = "On";
				menuLable.setString("CRT Mode: " + crtModeStatus);


		}

	}

	void MenuItemCRTMode::Action() {


	}

	//Menu item for Options

	MenuItemOptions::MenuItemOptions() : MenuItem() {

		menuLable.setString("Options");
		menuLable.setPosition(212, 150 + (count * 15));


	}

	void MenuItemOptions::Update() {



	}

	void MenuItemOptions::Action() {

		MenuItemHolder menuHolder;
		MenuItemMusicVolume* item1 = new MenuItemMusicVolume;
		MenuItemSFXVolume* item2 = new MenuItemSFXVolume;
		MenuItemFullscreen* item3 = new MenuItemFullscreen;
		MenuItemResolution* item4 = new MenuItemResolution;
		MenuItemCRTMode* item5 = new MenuItemCRTMode;
		MenuItemControls* item6 = new MenuItemControls;

		menuHolder.menuList.push_back(item1);
		menuHolder.menuList.push_back(item2);
		menuHolder.menuList.push_back(item3);
		menuHolder.menuList.push_back(item4);
		menuHolder.menuList.push_back(item5);
		menuHolder.menuList.push_back(item6);

		menuHolder.menuList.at(0)->ToggleSelection();
		Menu::menuContainer.push_back(menuHolder);

	}

	MenuItemQuit::MenuItemQuit() : MenuItem() {

		menuLable.setString("Quit");
		menuLable.setPosition(212, 150 + (count * 15));

	}

	void MenuItemQuit::Update() {

	}

	void MenuItemQuit::Action() {

		World::GetInstance()->windowWorld->close();

	}

	MenuItemMusicVolume::MenuItemMusicVolume() : MenuItem() {
		
		menuLable.setString("Music Volume: " + std::to_string(World::GetInstance()->WorldScene.audioContainer.musicVolume));
		menuLable.setPosition(212, 150 + (count * 15));

	}

	void MenuItemMusicVolume::Update() {

		if ((World::GetInstance()->PlayerPressedButton(controlsLeft, true))) {

			if (World::GetInstance()->WorldScene.audioContainer.musicVolume > 0) {

				--World::GetInstance()->WorldScene.audioContainer.musicVolume;

			}

		}

		else if ((World::GetInstance()->PlayerPressedButton(controlsRight, true))) {

			if (World::GetInstance()->WorldScene.audioContainer.musicVolume < 100) {

				++World::GetInstance()->WorldScene.audioContainer.musicVolume;

			}
			

		}

		menuLable.setString("Music Volume: " + std::to_string(World::GetInstance()->WorldScene.audioContainer.musicVolume));

	}

	void MenuItemMusicVolume::Action() {

	}

	MenuItemSFXVolume::MenuItemSFXVolume() : MenuItem() {

		menuLable.setString("SFX Volume: " + std::to_string(World::GetInstance()->WorldScene.audioContainer.sfxVolume));
		menuLable.setPosition(212, 150 + (count * 15));

	}

	void MenuItemSFXVolume::Update() {

		if ((World::GetInstance()->PlayerPressedButton(controlsLeft, true))) {

			if (World::GetInstance()->WorldScene.audioContainer.sfxVolume > 0) {

				--World::GetInstance()->WorldScene.audioContainer.sfxVolume;

			}

		}

		else if ((World::GetInstance()->PlayerPressedButton(controlsRight, true))) {

			if (World::GetInstance()->WorldScene.audioContainer.sfxVolume < 100) {

				++World::GetInstance()->WorldScene.audioContainer.sfxVolume;

			}


		}

		menuLable.setString("SFX Volume: " + std::to_string(World::GetInstance()->WorldScene.audioContainer.sfxVolume));

	}

	void MenuItemSFXVolume::Action() {


	}

	MenuItemFullscreen::MenuItemFullscreen() : MenuItem() {

		menuLable.setString("Fullscreen: " + std::to_string(World::GetInstance()->GlobalMembers.fullscreen));
		menuLable.setPosition(212, 150 + (count * 15));
	}

	void MenuItemFullscreen::Update() {

		if ((World::GetInstance()->PlayerPressedButton(controlsLeft, true))) {

			World::GetInstance()->GlobalMembers.fullscreen = 0;
			menuLable.setString("Fullscreen: " + std::to_string(World::GetInstance()->GlobalMembers.fullscreen));

		}

		else if ((World::GetInstance()->PlayerPressedButton(controlsRight, true))) {

			World::GetInstance()->GlobalMembers.fullscreen = 1;
			menuLable.setString("Fullscreen: " + std::to_string(World::GetInstance()->GlobalMembers.fullscreen));

		}

	}

	void MenuItemFullscreen::Action() {

		if (World::GetInstance()->GlobalMembers.fullscreen == true) {


			int desktopWidth = sf::VideoMode::getDesktopMode().width;
			int desktopHeight = sf::VideoMode::getDesktopMode().height;

			//2560 x 1600

			int xdiff = ceil(desktopWidth / 480);
			int ydiff = ceil(desktopHeight / 270);

			int gameWidth = 480;
			int gameHeight = 270 + (ydiff * 6);
			World::GetInstance()->windowWorld->create(sf::VideoMode(desktopWidth, desktopHeight), "Soulbaby", sf::Style::Fullscreen);
			World::GetInstance()->Screen.setSize(gameWidth, gameHeight);
			World::GetInstance()->windowWorld->setFramerateLimit(60);
			World::GetInstance()->windowWorld->setVerticalSyncEnabled(true);

		}

		if (World::GetInstance()->GlobalMembers.fullscreen == false) {

			int resolution = World::GetInstance()->GlobalMembers.currentResolution;
			World::GetInstance()->windowWorld->create(sf::VideoMode(480, 270), "Soulbaby", sf::Style::Titlebar);
			World::GetInstance()->Screen.setSize(480*resolution, 270*resolution);
			World::GetInstance()->windowWorld->setFramerateLimit(60);
			World::GetInstance()->windowWorld->setVerticalSyncEnabled(true);
		
		}

	}


	MenuItemResolution::MenuItemResolution() : MenuItem() {

		menuLable.setString("Resolution: " + std::to_string(World::GetInstance()->GlobalMembers.currentResolution) + "X");
		menuLable.setPosition(212, 150 + (count * 15));

	}

	void MenuItemResolution::Update() {

		int resolution;

		if (World::GetInstance()->GlobalMembers.fullscreen == 0) {

			if ((World::GetInstance()->PlayerPressedButton(controlsLeft, true))) {

				if (World::GetInstance()->GlobalMembers.currentResolution > 1) {
					--World::GetInstance()->GlobalMembers.currentResolution;
					resolution = World::GetInstance()->GlobalMembers.currentResolution;
					World::GetInstance()->windowWorld->setSize(sf::Vector2u(480 * resolution, 270 * resolution));
					sf::VideoMode userMode = sf::VideoMode::getDesktopMode();
					World::GetInstance()->windowWorld->setPosition(sf::Vector2i(userMode.width / 2 - ((480 * resolution) / 2), userMode.height / 2 - ((270 * resolution) / 2)));


				}


			}

			else if ((World::GetInstance()->PlayerPressedButton(controlsRight, true))) {

				if (World::GetInstance()->GlobalMembers.currentResolution < 6) {

					++World::GetInstance()->GlobalMembers.currentResolution;
					resolution = World::GetInstance()->GlobalMembers.currentResolution;
					World::GetInstance()->windowWorld->setSize(sf::Vector2u(480 * resolution, 270 * resolution));
					sf::VideoMode userMode = sf::VideoMode::getDesktopMode();
					World::GetInstance()->windowWorld->setPosition(sf::Vector2i(userMode.width / 2 - ((480 * resolution) / 2), userMode.height / 2 - ((270 * resolution) / 2)));

				}


			}

			menuLable.setString("Resolution: " + std::to_string(World::GetInstance()->GlobalMembers.currentResolution) + "X");

		}
	}

	void MenuItemResolution::Action() {

	}

	void MenuItem::Update() {

	};

	void MenuItem::Action() {

	}

	void MenuItem::ToggleSelection() {

		if (menuLable.getColor() == sf::Color::White) {

			menuLable.setColor(sf::Color(150, 150, 150));
			menuLable.move(2, 0);

		}

		else {

			menuLable.setColor(sf::Color::White);
			menuLable.move(-2, 0);
			hand.setPosition(menuLable.getPosition().x-17, menuLable.getPosition().y + 8);

		}
	}

	void MenuItem::Draw(sf::RenderTarget& window) {

		World::GetInstance()->DrawObject(objectSprite);
		World::GetInstance()->DrawObject(menuLable);

		if (menuLable.getColor() == sf::Color::White) {

			World::GetInstance()->DrawObject(hand);

		}

	}

	void MenuItemSetKey::Draw(sf::RenderTarget& window) {

		World::GetInstance()->DrawObject(objectSprite);
		World::GetInstance()->DrawObject(menuLable);
		World::GetInstance()->DrawObject(joyAssignment);


		if (menuLable.getColor() == sf::Color::White) {

			World::GetInstance()->DrawObject(hand);

		}

	}

	MenuItemHolder::MenuItemHolder() : GUI() {

		MenuItem::count = 0;

	}

	void MenuItemHolder::Update() {
	}

	void MenuItemHolder::Draw(sf::RenderTarget &window) {

	}

	MenuItemHolder::~MenuItemHolder() {
	}


	Menu::Menu() : GUI() {

		MenuItemHolder menuHolder;
		MenuItemStart* item1 = new MenuItemStart;
		MenuItemOptions* item2 = new MenuItemOptions;
		MenuItemQuit* item3 = new MenuItemQuit;

		menuHolder.menuList.push_back(item1);
		menuHolder.menuList.push_back(item2);
		menuHolder.menuList.push_back(item3);
		menuHolder.menuList.at(0)->ToggleSelection();
		menuContainer.push_back(menuHolder);

	}

	void Menu::Update() {

		//curent menu is always beginning of vector and what should be drawn

		int currentScreen = menuContainer.size() - 1;

		if (menuContainer.at(currentScreen).menuList.at(menuContainer.at(currentScreen).currentPos)->locked == false) {

			//Up/Down Selection
			if (World::GetInstance()->Timer(*this, SLOW)) {

				if (World::GetInstance()->PlayerPressedButton(controlsUp, true)) {

					World::GetInstance()->WorldScene.audioContainer.PlaySFX("sfx_jump2");
					if (menuContainer.at(currentScreen).currentPos != 0) {

						menuContainer.at(currentScreen).menuList.at(menuContainer.at(currentScreen).currentPos)->ToggleSelection();
						--menuContainer.at(currentScreen).currentPos;
						menuContainer.at(currentScreen).menuList.at(menuContainer.at(currentScreen).currentPos)->ToggleSelection();

					}

					else {

						menuContainer.at(currentScreen).menuList.at(menuContainer.at(currentScreen).currentPos)->ToggleSelection();
						menuContainer.at(currentScreen).currentPos = menuContainer.at(currentScreen).menuList.size() - 1;
						menuContainer.at(currentScreen).menuList.at(menuContainer.at(currentScreen).currentPos)->ToggleSelection();

					}

				}

			}

			if (World::GetInstance()->Timer(*this, SLOW)) {

				if (World::GetInstance()->PlayerPressedButton(controlsDown, true) && menuContainer.at(currentScreen).menuList.at(menuContainer.at(currentScreen).currentPos)->locked == false) {

					World::GetInstance()->WorldScene.audioContainer.PlaySFX("sfx_jump2");
					if (menuContainer.at(currentScreen).currentPos != menuContainer.at(currentScreen).menuList.size() - 1) {

						menuContainer.at(currentScreen).menuList.at(menuContainer.at(currentScreen).currentPos)->ToggleSelection();
						++menuContainer.at(currentScreen).currentPos;
						menuContainer.at(currentScreen).menuList.at(menuContainer.at(currentScreen).currentPos)->ToggleSelection();

					}

					else {

						menuContainer.at(currentScreen).menuList.at(menuContainer.at(currentScreen).currentPos)->ToggleSelection();
						menuContainer.at(currentScreen).currentPos = 0;
						menuContainer.at(currentScreen).menuList.at(menuContainer.at(currentScreen).currentPos)->ToggleSelection();
					}

				}

			}

		}

		//Detect possible inputs only for menu item that is selected

		menuContainer.at(currentScreen).menuList.at(menuContainer.at(currentScreen).currentPos)->Update();

		// Call Action() on selected menu item if press action/fire button
		if (menuContainer.at(currentScreen).menuList.at(menuContainer.at(currentScreen).currentPos)->locked == false) {

			if ((World::GetInstance()->PlayerPressedButton(controlsShootLeft, true))) {

				menuContainer.at(currentScreen).menuList.at(menuContainer.at(currentScreen).currentPos)->Action();
				World::GetInstance()->WorldScene.audioContainer.PlaySFX("sfx_jump4");

			}

			// Go back a menu state if pressed cancel/dash button

			else if ((World::GetInstance()->PlayerPressedButton(controlsShootRight, true) && menuContainer.size() != 1)) menuContainer.pop_back();

			}
		}
	


	void Menu::Draw(sf::RenderTarget& window) {

		for (auto i : menuContainer.at(menuContainer.size() - 1).menuList) i->Draw(window);
		World::GetInstance()->DrawObject(objectSprite);

	}

	Menu::~Menu() {

	}

	MenuItem::~MenuItem() {

	}

	MenuItemOptions::~MenuItemOptions() {

	}

	MenuItemMusicVolume::~MenuItemMusicVolume() {

	}

	MenuItemSFXVolume::~MenuItemSFXVolume() {

	}

	MenuItemFullscreen::~MenuItemFullscreen() {

	}

	MenuItemResolution::~MenuItemResolution() {

	}

	MenuItemControls::~MenuItemControls() {

	}

	MenuItemSetKey::~MenuItemSetKey() {

	}

	MenuItemStart::~MenuItemStart() {

	}

	MenuItemQuit::~MenuItemQuit() {

	}

	MenuItemCRTMode::~MenuItemCRTMode() {

	}

	QuakeManager::QuakeManager(){

		//std::cout << "quake manager created" << std::endl;
    }

	Guide::Guide() {

		objectSprite.setTexture(World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_misc"));
		objectSprite.setTextureRect(sf::IntRect(68, 218, 24, 18));
		SetCharacterOrigin();
		Entity::GUI::guiCount--;
		World::GetInstance()->WorldScene.guidePtr = this;
	}
    
	QuakeManager::~QuakeManager() {

	}

	void QuakeManager::Update() {

		if (World::GetInstance()->Timer(*this, VERY_SLOW*2, NODELAY)) {

			int roll = RandomNumber(10);

			if (roll == 1) {

				World::GetInstance()->ScreenShake(5);
				World::GetInstance()->WorldScene.audioContainer.PlaySFX("sfx_quake");
				World::GetInstance()->viewPos.x;


			}

		}


	}
    
    PlayerMenu::PlayerMenu(){
        
        objectSprite.setTexture(World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_portraits"));
        objectSprite.setTextureRect(sf::IntRect(0,0,50,100));
        
        itemName.setFont(World::GetInstance()->WorldScene.textureContainer.GetFont());
        itemDescriptionText.setFont(World::GetInstance()->WorldScene.textureContainer.GetFont());
        itemDescriptionLable.setFont(World::GetInstance()->WorldScene.textureContainer.GetFont());
        itemLvl.setFont(World::GetInstance()->WorldScene.textureContainer.GetFont());
        itemsLable.setFont(World::GetInstance()->WorldScene.textureContainer.GetFont());
        
        itemDescriptionLable.setString("description");
        itemsLable.setString("items");
        
        vel.y = 500;
        objectSprite.setPosition(World::GetInstance()->viewPos.x-((WINDOW_X/3)/2),vel.y);
        
    }
    
    void PlayerMenu::Update(){
        
        objectSprite.setPosition(World::GetInstance()->viewPos.x-((WINDOW_X/3)/2)+16,(World::GetInstance()->viewPos.y-((WINDOW_Y/3)/2))+16);

    }
    
    void PlayerMenu::Draw(sf::RenderTarget& window){
        
        World::GetInstance()->DrawObject(itemName);
        World::GetInstance()->DrawObject(itemDescriptionText);
        World::GetInstance()->DrawObject(itemDescriptionLable);
        World::GetInstance()->DrawObject(itemsLable);
        World::GetInstance()->DrawObject(itemLvl);
        World::GetInstance()->DrawObject(objectSprite);
        
    }
    
    Textbox::Textbox(std::string nameAndScript) : GUI()
    {
		World::GetInstance()->WorldScene.UIPtr = this;

		scriptNumber = nameAndScript[1] - '0';

        textboxCount = 1;

		characterNamePos = nameAndScript[0] - '0';

		//std::cout << scriptNumber << " \ " << characterNamePos;

		characterName = characters[characterNamePos];
        script = World::GetInstance()->CharacterScripts.at(characterName + std::to_string(scriptNumber));
        
        int lineCount = 0;
        

        //iterate through string to insert breaks and catch user inputted pauses

		for (int i = 0; i < script.length(); i++) {

			//checks if user has inserted a full break here

			if (!strncmp(&script.at(i), ">", 1))
			{
				lineCount = 0;

			}

			//if not, check if there should be a line-break here or full break

			else if (lineCount == 55 || lineCount == 110) {

				//if string is a letter

				if (strncmp(&script.at(i), " ", 1))
				{
					//check previous letters for the first space to break there instead of breaking mid-word

					for (int n = i; n != 0; n--) {

						if (!strncmp(&script.at(n), " ", 1)) {

							script.replace(n, 1, "\n");
							break;

						}

					}

				}

				//if not, treat this letter as a space
				else {


					script.replace(i, 1, "\n");

				}
			}

			else if (lineCount == 165) {

				if (strncmp(&script.at(i), " ", 1))
				{
					//check previous letters for the first space to break there instead of breaking mid-word

					for (int n = i; n != 0; n--) {

						if (!strncmp(&script.at(n), " ", 1)) {
 
							script.replace(n, 1, ">");
							break;

						}

					}

				}

				//if not, treat this letter as a space
				else {


					script.replace(i, 1, ">");

				}

				lineCount = 0;

			}

			lineCount++;


		}


        scriptLength = script.length();
        
        backDrop.setTexture(World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_misc"));
        backDrop.setTextureRect(sf::IntRect(0,138,448,80));
        
        // objectsprite is portrait sprite
        
        if(characterNamePos <= 5) objectSprite.setTexture(World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_portraits"));
        
        portraitSize = sf::Vector2i(80,100);
        
        objectSprite.setTextureRect(sf::IntRect(0,portraitSize.y * characterNamePos,portraitSize.x,portraitSize.y));
        
        boxText.setFont(World::GetInstance()->WorldScene.textureContainer.GetFont());
        boxText.setCharacterSize(16);
        
        boxName.setFont(World::GetInstance()->WorldScene.textureContainer.GetFont());
        boxName.setString(characterName);
        boxNameBG.setSize(sf::Vector2f(8 * characterName.length()+2,16));
        boxNameBG.setFillColor((sf::Color::Black));
        boxName.setCharacterSize(16);

		textChoice2.setFont(World::GetInstance()->WorldScene.textureContainer.GetFont());
		textChoice2.setString("Absorb");
		if (characterNamePos == 6 && scriptNumber == 7) textChoice2.setString("Leave");
		textChoice2.setCharacterSize(16);
		textChoice2.setColor(sf::Color(28, 203, 158));

		textChoice1.setFont(World::GetInstance()->WorldScene.textureContainer.GetFont());
		textChoice1.setString("Revive");
		if (characterNamePos == 6 && scriptNumber == 7) textChoice1.setString("Restart");

		textChoice1.setCharacterSize(16);
		textChoice1.setColor(sf::Color(28, 203, 158));

		hand.setTexture((World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_misc")));
		hand.setTextureRect(sf::IntRect(154, 15, 13, 8));
        
        boxArrow.setTexture(World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_misc"));
        boxArrow.setTextureRect(sf::IntRect(15,0,5,5));

		choice1bg.setTexture((World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_misc")));
		choice1bg.setTextureRect(sf::IntRect(144, 218, 66, 30));

		choice2bg.setTexture((World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_misc")));
		choice2bg.setTextureRect(sf::IntRect(144, 218, 66, 30));
        
    }
    
    void Textbox::Draw(sf::RenderTarget &window){
        
        World::GetInstance()->DrawObject(backDrop, "bypass");
        World::GetInstance()->DrawObject(boxText, "bypass");

		if (characterName != "NOTE") {

			World::GetInstance()->DrawObject(boxNameBG, "bypass");
			World::GetInstance()->DrawObject(boxName, "bypass");

		}

        World::GetInstance()->DrawObject(objectSprite);
        if(script.length() != 0 && !strncmp(&script.at(0),">",1))World::GetInstance()->DrawObject(boxArrow, "bypass");
        
		if (((characterNamePos == 6 && scriptNumber == 6) || (characterNamePos == 6 && scriptNumber == 7)) && isDone == true) {

			World::GetInstance()->DrawObject(choice1bg,"bypass");
			World::GetInstance()->DrawObject(choice2bg,"bypass");
			World::GetInstance()->DrawObject(textChoice2,"bypass");
			World::GetInstance()->DrawObject(textChoice1,"bypass");
			World::GetInstance()->DrawObject(hand,"bypass");

		}
    }
    
    Textbox::~Textbox(){
        
		World::GetInstance()->WorldScene.UIPtr = nullptr;
        scriptLength = 0;
        textboxCount = 0;
        World::GetInstance()->ResetCamera();
    }
    
    void Textbox::Update(){
        
        backDrop.setPosition(World::GetInstance()->viewPos.x-((WINDOW_X/3)/2)+16,World::GetInstance()->viewPos.y + 40);
        objectSprite.setPosition(backDrop.getPosition().x + 18,backDrop.getPosition().y -100);
        boxName.setPosition(backDrop.getPosition().x + 18,backDrop.getPosition().y-6);
        boxNameBG.setPosition(boxName.getPosition().x-3,boxName.getPosition().y+6);
        boxText.setPosition(backDrop.getPosition().x + 18,backDrop.getPosition().y + 11);
        boxArrow.setPosition(backDrop.getPosition().x + backDrop.getTextureRect().width - 25, backDrop.getPosition().y + backDrop.getTextureRect().height -26);
		
		choice1bg.setPosition(backDrop.getPosition().x + 16, backDrop.getPosition().y + 34);
		choice2bg.setPosition(choice1bg.getPosition().x + 71, choice1bg.getPosition().y);

		textChoice1.setPosition(choice1bg.getPosition().x + 12, choice1bg.getPosition().y + 2);
		textChoice2.setPosition(choice2bg.getPosition().x + 12, textChoice1.getPosition().y);

		hand.setPosition(choice1bg.getPosition().x -4 + (selectState * 71) ,choice1bg.getPosition().y + 12);

        if(newScript.length() != scriptLength && script.length() != 0){
            
            if(World::GetInstance()->Timer(*this,FAST, NODELAY)){
                
                // if end of text has been reach, stop progress until player presses action
                
                for(int p = 0; p != progressSpeed; p++){
                    
                    //if our progression goes beyond the limit, break and set to limit
                    //std::cout << newScript.length() + 1 << " | " <<  script.length() << std::endl;

                    if(script.length() == 0){
                        
                        break;
                        
                    }
                    
                    // in constructor, trigger characters are placed to let the text box know when to stop progressing and wait for the player to continue
                    // if a trigger is reached - text has reached end of text limit for text box) - stop progress and move to next set of lines when player presses "Z"
                    
                    else if(!strncmp(&script.at(0),">",1)){
                    
                        
                        if(World::GetInstance()->Timer(*this,25100.0)){
                        
                            if(World::GetInstance()->PlayerPressedButton(controlsShootLeft,true)){
                            
                                newScript.erase(0);
                                script.erase(0,1);
                            
                            }
                            
                        }
                        
                        if(World::GetInstance()->Timer(*this,SLOW)){
                            
                            int left = boxArrow.getTextureRect().left == 15 ? 20 : 15;
                            boxArrow.setTextureRect(sf::IntRect(left,0,5 ,5));

						}

					}

					// if none of the conditions above are not true, proceed to progress text

					else {

						newScript.append(script, 0, 1);
						script.erase(0, 1);
						World::GetInstance()->WorldScene.audioContainer.PlaySFX("sfx_txt");
						boxText.setString(newScript);

					}

				}

			}

		}

		// Kill the text box if the script is empty and the last line has been written

		else {

			isDone = true;

			if (World::GetInstance()->Timer(*this, 25200.0)) {

				//Also, if this is the Got Seed script, then show the two choices and enable hand/selecting


				if ((characterNamePos == 6 && scriptNumber == 6) || (characterNamePos == 6 && scriptNumber == 7)) {

					if (World::GetInstance()->Timer(*this, 25400.0)) {

						if (World::GetInstance()->PlayerPressedButton(controlsLeft,true) && selectState == 1) {

							selectState = 0;
							World::GetInstance()->WorldScene.audioContainer.PlaySFX("sfx_jump2");


						}

						if (World::GetInstance()->PlayerPressedButton(controlsRight,true) && selectState == 0) {

							selectState = 1;
							World::GetInstance()->WorldScene.audioContainer.PlaySFX("sfx_jump2");


						}
					}

				}


				if (World::GetInstance()->PlayerPressedButton(controlsShootLeft,true)) {

					if (World::GetInstance()->WorldScene.guidePtr) World::GetInstance()->WorldScene.guidePtr->ready = false;

					//If this is the first time the player has talking to this NPC for this NPC's level, give upgrade

					if (World::GetInstance()->GlobalMembers.levelsCompleted[characterNamePos] != 0 && World::GetInstance()->GlobalMembers.weapons[characterNamePos+1] == 0) {
						
						//std::cout << "ABILLTY LEARNED" << std::endl;
						itemQueue learn;
						learn.properties["PosX"] = std::to_string(World::GetInstance()->CameraTarget->objectSprite.getPosition().x);
						learn.properties["PosY"] = std::to_string(World::GetInstance()->CameraTarget->objectSprite.getPosition().y - (World::GetInstance()->CameraTarget->objectSprite.getTextureRect().height / 2));
						learn.properties["itemType"] = "LearnedAbility";
						learn.properties["AbilityLearned"] = std::to_string(characterNamePos);
						World::GetInstance()->WorldScene.objectContainer->Queue.push_back(learn);
						World::GetInstance()->WorldScene.audioContainer.MusicFadeOut();

					}

					//If this is the Got Seed script ...

					else if (characterNamePos == 6 && scriptNumber == 6) {

						// If player chose to revive gate keeper, mark that gate keeper as revived to be created in nexus

						if (selectState == 0) {

							int thisGateKeeper = World::GetInstance()->GlobalMembers.currentLevel;
							//std::cout << "npc " << thisGateKeeper << std::endl;
							World::GetInstance()->GlobalMembers.gateKeepersSaved[thisGateKeeper] = 1;
							World::GetInstance()->ReadyScene("map2_1");

						}

						// If player chose to absorb, increase level of current ability


						if (selectState == 1) {

							World::GetInstance()->GlobalMembers.currentPowerLevel++;
							//std::cout << "weapon is not " << World::GetInstance()->GlobalMembers.weapons.at(World::GetInstance()->GlobalMembers.currentWeapon);
							World::GetInstance()->ReadyScene("map2_1");

						}



					}

					//If this is the Death script ...

					else if (characterNamePos == 6 && scriptNumber == 7) {

						
						// If player chose to revive gate keeper, mark that gate keeper as revived to be created in nexus

						if (selectState == 0) {

							int thisGateKeeper = World::GetInstance()->GlobalMembers.currentLevel;
							World::GetInstance()->ReadyScene("battle");

						}

						// If player chose to absorb, increase level of current ability


						if (selectState == 1) {

							World::GetInstance()->ReadyScene("map2_1");

						}



					}


					misDestroyed = true;

				}

				if (World::GetInstance()->WorldScene.guidePtr) World::GetInstance()->WorldScene.guidePtr->ready = false;


			}

		}

	}

	BattleBackground::BattleBackground() {

		objectSprite.setTexture(World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_battle_bg_" + std::to_string(World::GetInstance()->GlobalMembers.currentLevel)));
		objectSprite.setTextureRect(sf::IntRect(0, 0, 2000, 2000));
		objectSprite.setOrigin(2000 / 2, 2000 / 2);
		type = "BG";

	}

	Door::Door(std::string pathway) : Object() {

		levelPath = stoi(pathway.erase(0, pathway.find("_") + 1));

		objectSprite.setTexture(World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_misc"));
		objectSprite.setTextureRect(sf::IntRect(0, 42 + (32 * 0), 16, 32));
		SetHitBox(sf::Vector2f(objectSprite.getTextureRect().width, objectSprite.getTextureRect().height+2), 2);
		Object::type = "Prop";

	}

	int Door::getPath() {

		return levelPath;

	}

	void Door::Update() {

		if (World::GetInstance()->Timer(*this, FAST)) {

			objectSprite.setTextureRect(sf::IntRect(16 * framePos, 42 + (32 * 0), 16, 32));
			framePos = (framePos > 1) ? 0 : ++framePos;

		}

	}

	void Door::Draw(sf::RenderTarget &window) {

		World::GetInstance()->DrawObject(objectSprite);
	}

	void Door::isCollided(int var) {

		World::GetInstance()->GlobalMembers.currentLevel = levelPath;
		World::GetInstance()->ReadyScene("battle");
	}


	///////////////////////

	Hud::Hud() : GUI() {

		World::GetInstance()->WorldScene.hudPtr = this;

		// Background hud

		objectSprite.setTexture(World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_misc"));
		selectedWeapon.setTexture(World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_misc"));
		selectionItems.setTexture(World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_misc"));

		pow1Lable.setFont(World::GetInstance()->WorldScene.textureContainer.GetFont("small"));
		pow2Lable.setFont(World::GetInstance()->WorldScene.textureContainer.GetFont("small"));

		pow1Lable.setString("Lv." + std::to_string(World::GetInstance()->GlobalMembers.currentPowerLevel));
		
		
		pow2Lable.setString("MP:");
		pow1Lable.setCharacterSize(16);
		pow2Lable.setCharacterSize(16);
		hand.setTexture((World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_misc")));
		hand.setTextureRect(sf::IntRect(154, 15, 13, 8));

		objectSprite.setTextureRect(sf::IntRect(0,218, 34, 33));
		selectedWeapon.setTextureRect(sf::IntRect(224, 240 + (World::GetInstance()->GlobalMembers.currentWeapon * 16), 16, 16));

		manaBar.setFillColor(sf::Color(28, 203, 158));
		manaBar.setSize(sf::Vector2f(100, 8));

		newAbilityOverlay.setTexture((World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_misc")));
		newAbilityOverlay.setTextureRect(sf::IntRect(34, 218, 34, 33));

    }

	ManaMeter::ManaMeter() {

		World::GetInstance()->WorldScene.playerPtr->PlayerManaMeter = this;
	    objectSprite.setTexture(World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_particles"));
		objectSprite.setTextureRect(sf::IntRect(67, 166, 2, 2));
	}

  
	void Hud::Update() {

		if (isExpanded == true) {


			expandTween += (40 - expandTween) / 5;
			//Selection

			if (World::GetInstance()->PlayerPressedButton(controlsLeft,true) && (World::GetInstance()->Timer(*this, NORMAL, NODELAY) && selectState != 0)) {

				selectState--;
				World::GetInstance()->WorldScene.audioContainer.PlaySFX("sfx_jump2");

			}

			else if (World::GetInstance()->PlayerPressedButton(controlsRight,true) && (World::GetInstance()->Timer(*this, NORMAL, NODELAY) && selectState != 6)) {

				selectState++;
				World::GetInstance()->WorldScene.audioContainer.PlaySFX("sfx_jump2");


			}

			if (World::GetInstance()->PlayerPressedActionButton() && World::GetInstance()->Timer(*this, SLOW, NODELAY)) {
				
				Close();
			}
		}


		if (World::GetInstance()->IsPlayerActive()) {

			if (World::GetInstance()->CurrentScene->mapType == 1 && World::GetInstance()->PlayerPressedButton(controlsDash,true) && (World::GetInstance()->Timer(*this, SLOW, NODELAY))) {

				Expand();

			}

	
		}

		//If a new ability has been got ...

		if (newAbility != 0) {

			//Begin to remove overlay after x seconds
			if (World::GetInstance()->Timer(*this, VERY_SLOW * 2) && newAbilityOverlay.getColor().a == 255) {

				newAbilityOverlay.setColor(sf::Color(255, 255, 255, 254));
			}

			if (newAbilityOverlay.getColor().a != 255 && newAbilityOverlay.getColor().a != 0) {

				newAbilityOverlay.setColor(sf::Color(255, 255, 255, newAbilityOverlay.getColor().a - 1));

			}

			if (World::GetInstance()->Timer(*this, VERY_SLOW * 8) && newAbilityOverlay.getColor().a == 0) {

				Close();
				newAbility = 0;
				itemQueue textbox;
				textbox.properties["itemType"] = "Textbox";
				textbox.properties["ActorName&Script"] = std::to_string(60);
				World::GetInstance()->WorldScene.objectContainer->Queue.push_back(textbox);
				World::GetInstance()->SetCameraTarget(*World::GetInstance()->WorldScene.playerPtr);
				World::GetInstance()->WorldScene.audioContainer.MusicFadeIn();
			}

		}



		
		if (World::GetInstance()->WorldScene.playerPtr) {

		pow1Lable.setPosition((World::GetInstance()->viewPos.x) - 220, World::GetInstance()->viewPos.y - 134);
		pow2Lable.setPosition(World::GetInstance()->WorldScene.playerPtr->objectSprite.getPosition().x, World::GetInstance()->WorldScene.playerPtr->objectSprite.getPosition().y + 20);
		pow2Lable.setString("MP:" + std::to_string(World::GetInstance()->WorldScene.playerPtr->mana) + "/ " + std::to_string(World::GetInstance()->GlobalMembers.maxMana));
		objectSprite.setPosition(World::GetInstance()->viewPos.x - 230, World::GetInstance()->viewPos.y - 125);

		selectedWeapon.setPosition((World::GetInstance()->viewPos.x) - 221, objectSprite.getPosition().y + 8);


		manaBar.setSize(sf::Vector2f(50, 2));
		manaBar.setPosition(pow2Lable.getPosition().x, pow2Lable.getPosition().y + 18);

		}


    }

	void Hud::Close() {


		if (itemList.at(selectState) != 7) {

			World::GetInstance()->GlobalMembers.currentWeapon = itemList.at(selectState);
			selectedWeapon.setTextureRect(sf::IntRect(224, 240 + (World::GetInstance()->GlobalMembers.currentWeapon * 16), 16, 16));
			pow1Lable.setString("Lv." + std::to_string(World::GetInstance()->GlobalMembers.currentPowerLevel));
			World::GetInstance()->WorldScene.audioContainer.PlaySFX("sfx_jump4");

		}


		isExpanded = false;
		World::GetInstance()->WorldScene.UIPtr = NULL;
		World::GetInstance()->WorldScene.textureContainer.dimWorld = false;
		expandTween = 0;


	}

	void Hud::Expand() {

		isExpanded = true;
		selectState = 0;
		itemList = { 0,1,2,3,4,5,6 };
		int slots = 0;
		for (int i = 0; i != itemList.size(); i++) {

			if (World::GetInstance()->GlobalMembers.weapons.at(i) == 0) {

				itemList.at(i) = 7;
				slots++;

			}

		}

		itemList.erase(std::remove(itemList.begin(), itemList.end(), 7), itemList.end());
		itemList.shrink_to_fit();

		for (int i = 0; i != slots; i++) {

			itemList.push_back(7);
		}

		//std::cout << "list size: " << itemList.size() << " " << std::endl;

		for (int i = 0; i != itemList.size(); i++) {

			//std::cout << itemList.at(i);




			auto it = itemList.begin();
			itemList.erase(std::remove(itemList.begin(), itemList.end(), World::GetInstance()->GlobalMembers.currentWeapon), itemList.end());
			itemList.insert(it, World::GetInstance()->GlobalMembers.currentWeapon);

		}

			World::GetInstance()->WorldScene.UIPtr = this;
			World::GetInstance()->WorldScene.audioContainer.PlaySFX("sfx_open");
			World::GetInstance()->WorldScene.textureContainer.dimWorld = true;

	}

	void Guide::Update() {

		sf::Vector2f newPos(objectSprite.getPosition().x, objectSprite.getPosition().y);

		newPos.y += ((targetPosition.y - 5) - objectSprite.getPosition().y) / 25;

		objectSprite.setPosition(targetPosition.x, newPos.y-10);

		if (ready == false) {

			if (World::GetInstance()->Timer(*this, VERY_SLOW*2)) {

				ready = true;

			}
		}

		if (World::GetInstance()->Timer(*this, VERY_SLOW)) {

			if (objectSprite.getTextureRect().left != 116) {

				objectSprite.setTextureRect(sf::IntRect(objectSprite.getTextureRect().left + 24, 218, 24, 18));

			}

			else objectSprite.setTextureRect(sf::IntRect(68, 218, 24, 18));

		}


	}

	void Guide::SetTarget(sf::Vector2f newPosition) {

		if (hidden == true) {

			hidden = false;
			targetPosition = newPosition;
			objectSprite.setPosition(targetPosition.x, targetPosition.y-5);
		}

	}

    
    void BattleBackground::Update(){
        
         objectSprite.rotate(0.05);
        
    }
    
	void Guide::Draw(sf::RenderTarget& window) {


		if (hidden == false && World::GetInstance()->IsPlayerCameraTarget() && (World::GetInstance()->WorldScene.hudPtr->isExpanded == false)) {

			World::GetInstance()->DrawObject(objectSprite);

		}

		hidden = true;

	}

	GameScene::GameScene() {

	}


	void GameScene::Update() {

		if (textboxCreated == false && World::GetInstance()->WorldScene.UIPtr == nullptr) {

			itemQueue textbox;
			textbox.properties["itemType"] = "Textbox";
			textbox.properties["ActorName&Script"] = std::to_string(50);
			World::GetInstance()->WorldScene.objectContainer->Queue.push_back(textbox);
			textboxCreated = true;

		}

		else if (textboxCreated == true && World::GetInstance()->WorldScene.UIPtr == nullptr && sceneDone == false) {

			World::GetInstance()->ReadyScene("map2_1");
			sceneDone = true;

		}
	}

    void Hud::Draw(sf::RenderTarget& window){


		if (isExpanded == true) {

			sf::Vector2f objectSpritePos = objectSprite.getPosition();

			objectSprite.setPosition(objectSpritePos);


			for (int i = 0; i != itemList.size()-1; i++) {
				
				objectSprite.setPosition(objectSpritePos.x + (expandTween + (i * expandTween)), objectSpritePos.y);
				if (itemList.at(i+1) == 7) objectSprite.setTextureRect(sf::IntRect(34, 218, 34, 33));
				World::GetInstance()->DrawObject(objectSprite, "bypass");
				objectSprite.setTextureRect(sf::IntRect(0, 218, 34, 33));

				selectionItems.setPosition(objectSpritePos.x + 9 + (expandTween + (i * expandTween)), objectSpritePos.y+8);
				if (itemList.at(i+1) == 7) selectionItems.setTextureRect(sf::IntRect(224, 224, 16, 16));
				else selectionItems.setTextureRect(sf::IntRect(224, 240 + ((itemList.at(i+1)) * 16), 16, 16));
				World::GetInstance()->DrawObject(selectionItems,"bypass");

				if (newAbility != 0 && itemList.at(i) == newAbility) {

					newAbilityOverlay.setPosition(objectSpritePos.x + (expandTween + ((i-1) * expandTween)), objectSpritePos.y);
					World::GetInstance()->DrawObject(newAbilityOverlay, "bypass");

				}
				
			}

			objectSprite.setPosition(objectSpritePos);
			hand.setPosition((objectSprite.getPosition().x-3) + (selectState * 40), objectSprite.getPosition().y+12);

		}

		if (World::GetInstance()->CurrentScene->mapType != ENCOUNTER) {

			World::GetInstance()->DrawObject(objectSprite, "bypass");
			World::GetInstance()->DrawObject(selectedWeapon, "bypass");
			World::GetInstance()->DrawObject(pow1Lable, "bypass");
		}


		if (isExpanded == true && newAbility == 0) World::GetInstance()->DrawObject(hand, "bypass");

		
    }

	void ManaMeter::Update() {


	}

	void ManaMeter::Draw(sf::RenderTarget& window) {

		if (World::GetInstance()->WorldScene.playerPtr) {

			int currentMana = World::GetInstance()->WorldScene.playerPtr->mana;


			objectSprite.setTextureRect(sf::IntRect(73, 166, 2, 2));
			for (int i = -30; i != World::GetInstance()->GlobalMembers.maxMana - 30; i++) {

				objectSprite.setPosition(int(World::GetInstance()->WorldScene.playerPtr->objectSprite.getPosition().x + (cos((i)*0.05) * 20)),
					int((World::GetInstance()->WorldScene.playerPtr->objectSprite.getPosition().y - 12) + (sin((i)*0.05) * 20)));

				if (got == true) World::GetInstance()->DrawObject(objectSprite, "whiteShader");
				else World::GetInstance()->DrawObject(objectSprite);

			}

			objectSprite.setTextureRect(sf::IntRect(67, 166, 2, 2));

			for (int i = -30; i != currentMana - 30; i++) {

				objectSprite.setPosition(int(World::GetInstance()->WorldScene.playerPtr->objectSprite.getPosition().x + (cos((i)*0.05) * 20)),
					int((World::GetInstance()->WorldScene.playerPtr->objectSprite.getPosition().y - 12) + (sin((i)*0.05) * 20)));

				if (got == true) World::GetInstance()->DrawObject(objectSprite, "whiteShader");
				else if (World::GetInstance()->WorldScene.playerPtr->chargeFlag > 0) World::GetInstance()->DrawObject(objectSprite,"chargeShader");
				else World::GetInstance()->DrawObject(objectSprite);

			}

			if (got == true) got = false;

		}

	}
    
    void BattleBackground::Draw(sf::RenderTarget& window){
        
        World::GetInstance()->DrawObject(objectSprite,"waveShader");
        
    }


    
    Hud::~Hud(){
        
		World::GetInstance()->WorldScene.hudPtr = NULL;

    }

	ManaMeter::~ManaMeter(){

	}

	Guide::~Guide() {

		Entity::GUI::guiCount++;
	}
    
    PlayerMenu::~PlayerMenu(){
        
    }
    
    Door::~Door(){
        
    }
    
    BattleBackground::~BattleBackground(){
        
    }

	GameScene::~GameScene() {

	}

    
    //////////////// PLAYER DEFINITIONS //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
        
    Player::Player(sf::Vector2i pos) : Object ()
    {
        
        //std::cout << "Player created" << std::endl;
        objectSprite.setTexture(World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_player"));
        objectSprite.setTextureRect(sf::IntRect(0, south, 13, 24));
        SetCharacterOrigin();
        spriteHeight = objectSprite.getTextureRect().height;
        spriteWidth = objectSprite.getTextureRect().width;
        World::GetInstance()->WorldScene.playerPtr = this;
        SetShadow();
        type = "Player";
        scaleTemp = sf::Vector2f(0.12,0);
        SetHitBox(sf::Vector2f(6,6),0);
        dead = false;
        if(!World::GetInstance()->CameraTarget) World::GetInstance()->SetCameraTarget(*this);
		mana = World::GetInstance()->GlobalMembers.maxMana - (World::GetInstance()->GlobalMembers.maxMana*0.75);

    }
    
    Player::~Player(){
        
        posXtemp1 = NULL;
        posYtemp2 = NULL;
        delete posXtemp1;
        delete posYtemp2;
        dead = true;
		World::GetInstance()->WorldScene.playerPtr = NULL;

    }
    
    void Player::Update()
    {


		// Walking
        
        if(World::GetInstance()->IsPlayerActive()){
            
        
        if((World::GetInstance()->PlayerPressedButton(controlsDown)
			&& World::GetInstance()->PlayerPressedButton(controlsLeft))) movement = swest;

		else if ((World::GetInstance()->PlayerPressedButton(controlsLeft)
			&& World::GetInstance()->PlayerPressedButton(controlsUp))) movement = nwest;

		else if ((World::GetInstance()->PlayerPressedButton(controlsUp)
			&& World::GetInstance()->PlayerPressedButton(controlsRight))) movement = neast;

		else if ((World::GetInstance()->PlayerPressedButton(controlsRight)
			&& World::GetInstance()->PlayerPressedButton(controlsDown))) movement = seast;

		else if (World::GetInstance()->PlayerPressedButton(controlsDown)) movement = south;

		else if (World::GetInstance()->PlayerPressedButton(controlsLeft)) movement = west;

		else if (World::GetInstance()->PlayerPressedButton(controlsUp)) movement = north;

		else if (World::GetInstance()->PlayerPressedButton(controlsRight)) movement = east;

        }
        
		if (!World::GetInstance()->PlayerPressedButton(controlsRight)
			&& !World::GetInstance()->PlayerPressedButton(controlsUp)
			&& !World::GetInstance()->PlayerPressedButton(controlsDown)
			&& !World::GetInstance()->PlayerPressedButton(controlsLeft))
			movement = idle;
        
        
        // Animation
        
        if(movement != idle){
            
            vel.y = 1;
            vel.x = 0;
			RotateVector(vel, 45 * movement);

            if(World::GetInstance()->Timer(*this,NORMAL, NODELAY)){
				 
                frame_pos = (frame_pos >= 5) ? 0 : frame_pos+1;
                objectSprite.setTextureRect(sf::IntRect(frame_pos * spriteWidth, movement * FRAME, spriteWidth, spriteHeight));
            
            }
            
        }
        
        else{
              
              vel.x = 0;
              vel.y = 0;
              objectSprite.setTextureRect(sf::IntRect(0, objectSprite.getTextureRect().top, spriteWidth, spriteHeight));
          
          }
    
        
        vel.x = round(vel.x);
        vel.y = round(vel.y);
        objectSprite.move(vel.x,vel.y);
        UpdateShadow();
        UpdateHitBox();
        *posXtemp1 = objectSprite.getPosition().x;
        *posYtemp2 = objectSprite.getPosition().y;
        
    }

	void Player::Bounce() {

		int borderLeft = World::GetInstance()->WorldScene.levelContainer->battleBorder.left;
		int borderTop = World::GetInstance()->WorldScene.levelContainer->battleBorder.top;
		int borderRight = borderLeft + World::GetInstance()->WorldScene.levelContainer->battleBorder.width;
		int borderBottom = borderTop + World::GetInstance()->WorldScene.levelContainer->battleBorder.height;
		int waveDirOne = 0;
		int waveDirTwo = 0;


		// Need to bounce the player back inside of arena; could be far outside and will get stuck

		if (objectHitBox.getPosition().x <= borderLeft) {

			std::cout << "borderLeft" << std::endl;

			objectSprite.setPosition(borderLeft + 1, objectSprite.getPosition().y);
			waveDirOne = north;
			waveDirTwo = south;

		}

		else if (objectHitBox.getPosition().y >= borderBottom) {

			std::cout << "borderBottom" << std::endl;

			objectSprite.setPosition(objectSprite.getPosition().x, borderBottom - 1);
			waveDirOne = west;
			waveDirTwo = east;
		}

		else if (objectHitBox.getPosition().x >= borderRight) {


			std::cout << "borderRight" << std::endl;
			objectSprite.setPosition(borderRight - 1, objectSprite.getPosition().y);
			waveDirOne = north;
			waveDirTwo = south;

		}

		else if (objectHitBox.getPosition().y <= borderTop) {

			std::cout << "borderTop" << std::endl;

			objectSprite.setPosition(objectSprite.getPosition().x, borderTop + 1);
			waveDirOne = west;
			waveDirTwo = east;

		}
		

		sf::Vector2f tempPos = objectSprite.getPosition();

		//bounce dash in correct direction

		if (movement == north) movement = south;
		else if (movement == neast) {

			if (objectHitBox.getPosition().x > borderRight) movement = nwest;
			else movement = seast;
		}
		else if (movement == nwest) {

			if (objectHitBox.getPosition().x < borderLeft) movement = neast;
			else movement = swest;
		}
		else if (movement == east) movement = west;
		else if (movement == south) movement = north;
		else if (movement == swest) {

			if (objectHitBox.getPosition().x < borderLeft) movement = seast;
			else movement = nwest;
		}
		else if (movement == seast) {

			if (objectHitBox.getPosition().x > borderRight) movement = swest;
			else movement = neast;
		}
		else if (movement == west) movement = east;

		//accelerate and rotate angle to correct direction

		vel.y = 350;
		vel.x = 0;
		RotateVector(vel, 45 * movement);
		tempPos += vel;
		vel = tempPos;

		//if current weapon level is 2 or greater,  shoot projectile object

		if (World::GetInstance()->GlobalMembers.currentPowerLevel > 1) {

			itemQueue wave;
			wave.properties["itemType"] = "BounceCrawler";
			wave.properties["PosX"] = std::to_string(objectSprite.getPosition().x);
			wave.properties["PosY"] = std::to_string(objectSprite.getPosition().y);
			wave.properties["Direction"] = std::to_string(waveDirOne);
			World::GetInstance()->WorldScene.objectContainer->Queue.push_back(wave);
			wave.properties["Direction"] = std::to_string(waveDirTwo);
			World::GetInstance()->WorldScene.objectContainer->Queue.push_back(wave);

		}

		//effects and sxf

		itemQueue dash;
		dash.properties["PosX"] = std::to_string(objectSprite.getPosition().x);
		dash.properties["PosY"] = std::to_string(objectSprite.getPosition().y - 6);
		dash.properties["itemType"] = "ActionSpark";
		World::GetInstance()->WorldScene.objectContainer->Queue.push_back(dash);
		World::GetInstance()->WorldScene.audioContainer.PlaySFX("sfx_bouncyhit");
		World::GetInstance()->WorldScene.levelContainer->levelManagerPtr->bgwallHit = true;

		std::this_thread::sleep_for(std::chrono::milliseconds(75));
		World::GetInstance()->ScreenShake(20);



	}
    
    BattlePlayer::BattlePlayer(){
        
        hotSpot.x = objectSprite.getPosition().x + 6;
        hotSpot.y = objectSprite.getPosition().y + 12;
		sshield.setTexture(World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_misc"));
		sshield.setTextureRect(sf::IntRect(103, 0, 17, 41));
		sshield.setColor(sf::Color::Color(255, 255, 255, 50));

		itemQueue meter;
		meter.properties["itemType"] = "ManaMeter";
		World::GetInstance()->WorldScene.objectContainer->Queue.push_back(meter);

    }

	void BattlePlayer::DoChargeAttack() {

		//std::cout << "charge commenced" << std::endl;
		jumpFlag = false;

		sf::Vector2f tempPos = objectSprite.getPosition();


		if (chargeFlag == 0 && mana >= 10) {

			if ((World::GetInstance()->PlayerPressedButton(controlsDown)
				&& World::GetInstance()->PlayerPressedButton(controlsLeft))) movement = swest;

			else if ((World::GetInstance()->PlayerPressedButton(controlsLeft)
				&& World::GetInstance()->PlayerPressedButton(controlsUp))) movement = nwest;

			else if ((World::GetInstance()->PlayerPressedButton(controlsUp)
				&& World::GetInstance()->PlayerPressedButton(controlsRight))) movement = neast;

			else if ((World::GetInstance()->PlayerPressedButton(controlsRight)
				&& World::GetInstance()->PlayerPressedButton(controlsDown))) movement = seast;

			else if (World::GetInstance()->PlayerPressedButton(controlsDown)) movement = south;

			else if (World::GetInstance()->PlayerPressedButton(controlsLeft)) movement = west;

			else if (World::GetInstance()->PlayerPressedButton(controlsUp)) movement = north;

			else if (World::GetInstance()->PlayerPressedButton(controlsRight)) movement = east;


			vel.y = 100;
			vel.x = 0;
			RotateVector(vel, 45 * movement);
			tempPos += vel;
			vel = tempPos;
			dashing = true;
			itemQueue dash;
			dash.properties["PosX"] = std::to_string(objectSprite.getPosition().x);
			dash.properties["PosY"] = std::to_string(objectSprite.getPosition().y - 6);
			dash.properties["itemType"] = "ActionSpark";
			World::GetInstance()->WorldScene.objectContainer->Queue.push_back(dash);
			World::GetInstance()->WorldScene.audioContainer.PlaySFX("sfx_lightdash");
			hyperDash = false;
			//mana -= (10 * chargeFlag) + 10;

		}

		else if (chargeFlag == 1 && mana >= 10 * chargeFlag) {

			if ((World::GetInstance()->PlayerPressedButton(controlsDown)
				&& World::GetInstance()->PlayerPressedButton(controlsLeft))) movement = swest;


			else if ((World::GetInstance()->PlayerPressedButton(controlsLeft)
				&& World::GetInstance()->PlayerPressedButton(controlsUp))) movement = nwest;

			else if ((World::GetInstance()->PlayerPressedButton(controlsUp)
				&& World::GetInstance()->PlayerPressedButton(controlsRight))) movement = neast;

			else if ((World::GetInstance()->PlayerPressedButton(controlsRight)
				&& World::GetInstance()->PlayerPressedButton(controlsDown))) movement = seast;

			else if (World::GetInstance()->PlayerPressedButton(controlsDown)) movement = south;

			else if (World::GetInstance()->PlayerPressedButton(controlsLeft)) movement = west;

			else if (World::GetInstance()->PlayerPressedButton(controlsUp)) movement = north;

			else if (World::GetInstance()->PlayerPressedButton(controlsRight)) movement = east;

			vel.y = 350;
			vel.x = 0;
			RotateVector(vel, 45 * movement);
			tempPos += vel;
			vel = tempPos;
			dashing = true;
			hyperDash = true;
			itemQueue dash;
			dash.properties["PosX"] = std::to_string(objectSprite.getPosition().x);
			dash.properties["PosY"] = std::to_string(objectSprite.getPosition().y - 6);
			dash.properties["itemType"] = "ActionSpark";
			World::GetInstance()->WorldScene.objectContainer->Queue.push_back(dash);
			dash.properties["PosY"] = std::to_string(objectSprite.getPosition().y);
			dash.properties["itemType"] = "DashEffect";
			World::GetInstance()->WorldScene.objectContainer->Queue.push_back(dash);
			dash.properties["itemType"] = "PlayerDashBall";
			World::GetInstance()->WorldScene.objectContainer->Queue.push_back(dash);
			World::GetInstance()->WorldScene.audioContainer.PlaySFX("sfx_bigbang");
			World::GetInstance()->ScreenShake(10);
			//mana -= (10 * chargeFlag) + 10;

		}

		else if (chargeFlag == 2 && mana >= 10 * chargeFlag) {

			itemQueue wave;
			wave.properties["PosX"] = std::to_string(objectSprite.getPosition().x);
			wave.properties["PosY"] = std::to_string(objectSprite.getPosition().y - 6);
			wave.properties["itemType"] = "ChargeWaveAttack";
			World::GetInstance()->WorldScene.objectContainer->Queue.push_back(wave);
			World::GetInstance()->WorldScene.audioContainer.PlaySFX("sfx_bigbang");

			//mana -= (10 * chargeFlag) + 10;

		}

		chargeFlag = 0;
		chargeCounter = 0;

	}

    
	void BattlePlayer::Update() {


     //charging/dashing logic

		//if player has let go of dash key, dash

		if (jumpFlag == true && !World::GetInstance()->PlayerPressedButton(controlsDash)) {

			//std::cout << "DASH START" << std::endl;
			//do the appropraite charge move here
			DoChargeAttack();
			dashSave = true;

		}

		//if player has pressed dash key, initiate charge sequence

		else if (jumpFlag == false) {

			if (mana >= 10 && World::GetInstance()->PlayerPressedButton(controlsDash)) jumpFlag = true;

		}

		if (jumpFlag == true){

			if (World::GetInstance()->Timer(*this, VERY_FAST, NODELAY)) {

				chargeCounter++;

			}

			if (chargeCounter == 25) {

				chargeFlag++;

				World::GetInstance()->WorldScene.audioContainer.PlaySFX("sfx_pcharge" + std::to_string(chargeFlag));

				itemQueue charge;
				charge.properties["PosX"] = std::to_string(objectHitBox.getPosition().x);
				charge.properties["PosY"] = std::to_string(objectHitBox.getPosition().y - 8);
				charge.properties["itemType"] = "BlockedWave";
				World::GetInstance()->WorldScene.objectContainer->Queue.push_back(charge);

				chargeCounter = 0;

			}
			
			if (chargeFlag > 0) {

				if (World::GetInstance()->Timer(*this, NORMAL)) {

					itemQueue charge;
					charge.properties["PosX"] = std::to_string(objectHitBox.getPosition().x);
					charge.properties["PosY"] = std::to_string(objectHitBox.getPosition().y - 6);
					charge.properties["itemType"] = "PlayerChargeParticleSmall";
					World::GetInstance()->WorldScene.objectContainer->Queue.push_back(charge);
					World::GetInstance()->WorldScene.objectContainer->Queue.push_back(charge);
					itemQueue clone;
					clone.properties["PosX"] = std::to_string(objectSprite.getPosition().x);
					clone.properties["PosY"] = std::to_string(objectSprite.getPosition().y);
					clone.properties["itemType"] = "SpriteClone";
					clone.parent = this;
					World::GetInstance()->WorldScene.objectContainer->Queue.push_back(clone);

				}
				

			}

			
		}



		// Walking

		if (dashing == false) {

			if (!World::GetInstance()->WorldScene.transition) {


				//set movement direction

				if ((World::GetInstance()->PlayerPressedButton(controlsDown)
					&& World::GetInstance()->PlayerPressedButton(controlsLeft))) movement = swest;

				else if ((World::GetInstance()->PlayerPressedButton(controlsLeft)
					&& World::GetInstance()->PlayerPressedButton(controlsUp))) movement = nwest;

				else if ((World::GetInstance()->PlayerPressedButton(controlsUp)
					&& World::GetInstance()->PlayerPressedButton(controlsRight))) movement = neast;

				else if ((World::GetInstance()->PlayerPressedButton(controlsRight)
					&& World::GetInstance()->PlayerPressedButton(controlsDown))) movement = seast;

				else if (World::GetInstance()->PlayerPressedButton(controlsDown)) movement = south;

				else if (World::GetInstance()->PlayerPressedButton(controlsLeft)) movement = west;

				else if (World::GetInstance()->PlayerPressedButton(controlsUp)) movement = north;

				else if (World::GetInstance()->PlayerPressedButton(controlsRight)) movement = east;

				//set fire direction

				if ((World::GetInstance()->PlayerPressedButton(controlsShootDown)
					&& World::GetInstance()->PlayerPressedButton(controlsShootLeft))) fireDir = swest;

				else if ((World::GetInstance()->PlayerPressedButton(controlsShootLeft)
					&& World::GetInstance()->PlayerPressedButton(controlsShootUp))) fireDir = nwest;

				else if ((World::GetInstance()->PlayerPressedButton(controlsShootUp)
					&& World::GetInstance()->PlayerPressedButton(controlsShootRight))) fireDir = neast;

				else if ((World::GetInstance()->PlayerPressedButton(controlsShootRight)
					&& World::GetInstance()->PlayerPressedButton(controlsShootDown))) fireDir = seast;

				else if (World::GetInstance()->PlayerPressedButton(controlsShootDown)) fireDir = south;

				else if (World::GetInstance()->PlayerPressedButton(controlsShootLeft)) fireDir = west;

				else if (World::GetInstance()->PlayerPressedButton(controlsShootUp)) fireDir = north;

				else if (World::GetInstance()->PlayerPressedButton(controlsShootRight)) fireDir = east;



			}

			if (!World::GetInstance()->PlayerPressedButton(controlsRight)
				&& !World::GetInstance()->PlayerPressedButton(controlsUp)
				&& !World::GetInstance()->PlayerPressedButton(controlsDown)
				&& !World::GetInstance()->PlayerPressedButton(controlsLeft))
				movement = idle;

			if (!World::GetInstance()->PlayerPressedButton(controlsShootRight)
				&& !World::GetInstance()->PlayerPressedButton(controlsShootUp)
				&& !World::GetInstance()->PlayerPressedButton(controlsShootDown)
				&& !World::GetInstance()->PlayerPressedButton(controlsShootLeft))
				fireDir = movement;

		     

		}
		


		// Dashing/Hyper dashing logic

		if (World::GetInstance()->PlayerPressedButton(controlsDash) && dashing == true && dashSave == true) {

			dashing = false;
			dashSave = false;
			//std::cout << "DASH BREAK" << std::endl;
			World::GetInstance()->WorldScene.audioContainer.PlaySFX("sfx_knockwave");
			vel.x = 0;
			vel.y = 0;

		}

			else if (dashing == true) {

				if (World::GetInstance()->Timer(*this, NORMAL, NODELAY)) {

					if (hyperDash == true) {

						itemQueue dash;
						dash.properties["itemType"] = "Electricity";
						dash.properties["PosX"] = std::to_string(objectSprite.getPosition().x - 30 + RandomNumber(60, 0));
						dash.properties["PosY"] = std::to_string(objectSprite.getPosition().y - 30 + RandomNumber(60, 0));
						World::GetInstance()->WorldScene.objectContainer->Queue.push_back(dash);
						dash.properties["PosX"] = std::to_string(objectSprite.getPosition().x - 30 + RandomNumber(60, 0));
						dash.properties["PosY"] = std::to_string(objectSprite.getPosition().y - 30 + RandomNumber(60, 0));
						World::GetInstance()->WorldScene.objectContainer->Queue.push_back(dash);

					}

					else {

						itemQueue dash;
						dash.properties["PosX"] = std::to_string(objectSprite.getPosition().x);
						dash.properties["PosY"] = std::to_string(objectSprite.getPosition().y);
						dash.properties["itemType"] = "SlideEffect";
						World::GetInstance()->WorldScene.objectContainer->Queue.push_back(dash);
					}
				}

				
					//decrease speed of amount dependent on if hyper or regular dash

					if (hyperDash == true) {

						sf::Vector2f tempPos = objectSprite.getPosition();
						tempPos.x += (vel.x - objectSprite.getPosition().x) / 20;
						tempPos.y += (vel.y - objectSprite.getPosition().y) / 20;
						objectSprite.setPosition(double(tempPos.x), double(tempPos.y));

						if (abs((vel.x - objectSprite.getPosition().x) / 5) <= 0.5 && abs((vel.y - objectSprite.getPosition().y) / 5) <= 0.5) {

							//std::cout << "DASH ENDS" << std::endl;
							dashing = false;
							hyperDash = false;
							jumpFlag = false;
							vel.x = 0;
							vel.y = 0;

						}

				    }

					else if(hyperDash == false){

						sf::Vector2f tempPos = objectSprite.getPosition();
						tempPos.x += (vel.x - objectSprite.getPosition().x) / 10;
						tempPos.y += (vel.y - objectSprite.getPosition().y) / 10;
						objectSprite.setPosition(double(tempPos.x),double(tempPos.y));

						if ( abs((vel.x - objectSprite.getPosition().x) / 5) <= 0.5 && abs((vel.y - objectSprite.getPosition().y) / 5) <= 0.5) {

							//std::cout << "DASH ENDS" << std::endl;
							dashing = false;
							hyperDash = false;
							jumpFlag = false;
							vel.x = 0;
							vel.y = 0;

						}

					}

			
				objectSprite.setTextureRect(sf::IntRect(0, 192 + (movement * FRAME), 16, spriteHeight));
				


			}
		


			else if (movement != idle && dashing == false) {

				//weapon 3 slows down the player when firing, so we need to check this

				if (World::GetInstance()->PlayerPressedActionButton() && World::GetInstance()->GlobalMembers.currentWeapon == 3) {

					vel.y = 1;
					vel.x = 0;

				}

				else {

				vel.y = 2;
				vel.x = 0;

				}

			if (World::GetInstance()->Timer(*this, FAST, NODELAY)) {

				frame_pos = (frame_pos >= 5) ? 0 : frame_pos + 1;

				if (!World::GetInstance()->PlayerPressedButton(controlsShootLeft)&&
					!World::GetInstance()->PlayerPressedButton(controlsShootUp)&&
					!World::GetInstance()->PlayerPressedButton(controlsShootRight)&&
					!World::GetInstance()->PlayerPressedButton(controlsShootDown)) {

					objectSprite.setTextureRect(sf::IntRect(frame_pos * spriteWidth, movement * FRAME, spriteWidth, spriteHeight));
					fireDir = movement;

				}

				else objectSprite.setTextureRect(sf::IntRect(frame_pos * spriteWidth, fireDir * FRAME, spriteWidth, spriteHeight));

			}


		}

		//

		else if (movement == idle) {

			objectSprite.setTextureRect(sf::IntRect(0, objectSprite.getTextureRect().top, spriteWidth, spriteHeight));

		}

		if (World::GetInstance()->Timer(*this, 140000.0, NODELAY) && dashing == false && hyperDash == false) {

			vel.x *= 0.25;
			vel.y *= 0.25;

		}

		// rotates speed in the correct direction

		if (dashing == false) {

			if (movement == idle) {

				vel.x = 0;
				vel.y = 0;

			}

			RotateVector(vel, 45 * movement);

		}



		/*

		Firing:
		Here we check the currentweapon and fire the correct projectile 

		weapons[0] = pulse (default)

		Boss upgrades:
		weapons[1] = spreader
		weapons[2] = lance
		weapons[3] = laser
		weapons[4] = buster
		weapons[5] = homing lightning
		weapons[6] = bomb

		every weapon has 3 or 6 levels

		*/

		//targeter is always active; doesn't require player to shoot

		if (World::GetInstance()->GlobalMembers.currentWeapon == 5 && PlayerHoming1::count == 0) {

			itemQueue proj;
			proj.properties["itemType"] = "PlayerHoming1";
			proj.properties["PosX"] = std::to_string(hotSpot.x);
			proj.properties["PosY"] = std::to_string(hotSpot.y);
			proj.properties["Direction"] = std::to_string(fireDir);
			proj.properties["VelX"] = std::to_string(vel.x);
			proj.properties["VelY"] = std::to_string(vel.y);
			World::GetInstance()->WorldScene.objectContainer->Queue.push_back(proj);

		}


		if ((World::GetInstance()->PlayerPressedButton(controlsShootLeft)||
			World::GetInstance()->PlayerPressedButton(controlsShootRight)||
			World::GetInstance()->PlayerPressedButton(controlsShootUp)||
			World::GetInstance()->PlayerPressedButton(controlsShootDown)) && dashing == false) {
			
			itemQueue proj;
			proj.properties["PosX"] = std::to_string(hotSpot.x);
			proj.properties["PosY"] = std::to_string(hotSpot.y);
			proj.properties["Direction"] = std::to_string(fireDir);
			proj.properties["VelX"] = std::to_string(vel.x);
			proj.properties["VelY"] = std::to_string(vel.y);

			// repeater


			if (World::GetInstance()->GlobalMembers.currentWeapon == 0 && World::GetInstance()->Timer(*this, SLOW, NODELAY)) {

				proj.properties["itemType"] = "PlayerLaser" + std::to_string(World::GetInstance()->GlobalMembers.currentPowerLevel);
				World::GetInstance()->WorldScene.audioContainer.PlaySFX("sfx_laser9");
				World::GetInstance()->WorldScene.objectContainer->Queue.push_back(proj);

			}

			// spreader

			else if (World::GetInstance()->GlobalMembers.currentWeapon == 1 && World::GetInstance()->Timer(*this, SLOW, NODELAY)) {

				proj.properties["itemType"] = "PlayerRepeater" + std::to_string(World::GetInstance()->GlobalMembers.currentPowerLevel);
				World::GetInstance()->WorldScene.audioContainer.PlaySFX("sfx_laser9");
				proj.properties["Direction"] = std::to_string((fireDir * 45));
				World::GetInstance()->WorldScene.objectContainer->Queue.push_back(proj);
				proj.properties["Direction"] = std::to_string((fireDir * 45) + 15);
				World::GetInstance()->WorldScene.objectContainer->Queue.push_back(proj);
				proj.properties["Direction"] = std::to_string((fireDir * 45) - 15);
				World::GetInstance()->WorldScene.objectContainer->Queue.push_back(proj);

			}

			// Laser 

			else if (World::GetInstance()->GlobalMembers.currentWeapon == 2 && World::GetInstance()->Timer(*this, VERY_SLOW, NODELAY)) {

				proj.properties["itemType"] = "PlayerBeam1";
				World::GetInstance()->WorldScene.audioContainer.PlaySFX("sfx_laser9");
				World::GetInstance()->WorldScene.objectContainer->Queue.push_back(proj);

			}


			// shotgun

			else if (World::GetInstance()->GlobalMembers.currentWeapon == 3 && World::GetInstance()->Timer(*this, 45210.0, NODELAY)) {

				proj.properties["itemType"] = "PlayerShotgun1";
				World::GetInstance()->WorldScene.audioContainer.PlaySFX("sfx_rapid4");
				proj.properties["Direction"] = std::to_string((fireDir * 45));
				World::GetInstance()->WorldScene.objectContainer->Queue.push_back(proj);
				proj.properties["Direction"] = std::to_string((fireDir * 45) + (RandomNumber(10)));
				World::GetInstance()->WorldScene.objectContainer->Queue.push_back(proj);
				proj.properties["Direction"] = std::to_string((fireDir * 45) - (RandomNumber(10)));
				World::GetInstance()->WorldScene.objectContainer->Queue.push_back(proj);


			}


			// cannon

			else if (World::GetInstance()->GlobalMembers.currentWeapon == 4 && World::GetInstance()->Timer(*this, VERY_SLOW, NODELAY)) {

				proj.properties["itemType"] = "PlayerBomb";
				World::GetInstance()->WorldScene.objectContainer->Queue.push_back(proj);

			}


		}

		//Shield logic
		//shield == 0; set when player is just damaged; cannot be damaged in this state
		//shield > 0 && != 41; can be damaged in this state
		//shield == 41; shield is on/recovered

		if (shield == 0) {

			objectSprite.setColor(sf::Color::Color(255, 255, 255, 50 + cos(World::GetInstance()->clock2.getElapsedTime().asMilliseconds()*0.5) * 100));
			if (World::GetInstance()->Timer(*this, VERY_SLOW * 8)) {
				shield = 1;
				objectSprite.setColor(sf::Color::Color(255, 255, 255,255));

			}

		}

		if (shield != 0 && shield < 41) {

			if (World::GetInstance()->Timer(*this, VERY_SLOW)) shield += 1;
			sshield.setColor(sf::Color::Color(255, 255, 255, 50 + cos(World::GetInstance()->clock2.getElapsedTime().asMilliseconds()*0.5) * 100));
			sshield.setTextureRect(sf::IntRect(86, 41 - shield, 17, shield));
			
			if (shield >= 41) {

				shield = 41;
				itemQueue particles;
				particles.properties["PosX"] = std::to_string(objectSprite.getPosition().x);
				particles.properties["PosY"] = std::to_string(objectSprite.getPosition().y - 10);
				particles.properties["itemType"] = "ShieldEffect";
				World::GetInstance()->WorldScene.objectContainer->Queue.push_back(particles);
				World::GetInstance()->WorldScene.audioContainer.PlaySFX("sfx_twinkle");

					
			}

		}

		else if (shield == 41) {

			float transparency = (40 + cos(World::GetInstance()->clock2.getElapsedTime().asMilliseconds()*0.0015) * 20) + 10;
			sshield.setColor(sf::Color::Color(255, 255, 255, transparency));
			sshield.setTextureRect(sf::IntRect(103, 41 - shield, 17, shield));

		}
				
		if (dashing == true || hyperDash == true) {

		}
		else objectSprite.move(double(vel.x), double(vel.y));

		hotSpot.x = objectSprite.getPosition().x;
		hotSpot.y = objectSprite.getPosition().y - objectSprite.getTextureRect().height / 2;
		UpdateShadow();

		*posXtemp1 = objectSprite.getPosition().x;
		*posYtemp2 = objectSprite.getPosition().y;

		sshield.setPosition(objectSprite.getPosition().x - 8, (objectSprite.getPosition().y - 32)+41-shield);


	}

	void Player::Draw(sf::RenderTarget& window) {
		
		    if (chargeFlag > 0) {

			   World::GetInstance()->DrawObject(objectSprite,"chargeShader");

			}
		
			else if (hyperDash == false ){

				World::GetInstance()->DrawObject(objectSprite);

			}

			if(shield != 0 && shield != 41) World::GetInstance()->DrawObject(sshield);



	}

	void DeathPoof::Draw(sf::RenderTarget& window) {

		World::GetInstance()->DrawObject(objectSprite, "chargeShader");

	}

	void EnemyPart::Draw(sf::RenderTarget& window) {

		World::GetInstance()->DrawObject(objectSprite, "chargeShader");

	}
    
    BattlePlayer::~BattlePlayer(){
        
        itemQueue particles;

        particles.properties["PosX"] = std::to_string(objectSprite.getPosition().x);
        particles.properties["PosY"] = std::to_string(objectSprite.getPosition().y);
        particles.properties["itemType"] = "DeathPoof";
        
        
        World::GetInstance()->WorldScene.objectContainer->Queue.push_back(particles);
        World::GetInstance()->WorldScene.objectContainer->Queue.push_back(particles);
        World::GetInstance()->WorldScene.objectContainer->Queue.push_back(particles);
        
        particles.properties["itemType"] = "DeathBoom";
        World::GetInstance()->WorldScene.objectContainer->Queue.push_back(particles);
        
        
        World::GetInstance()->WorldScene.audioContainer.PlaySFX("sfx_death");
        
        World::GetInstance()->ScreenShake(20);


    }
    
    //////////// END PLAYER DEFINITIONS ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    ////////// MISC
    
    
    // Constructors
    
    int Object::totalObjects = 0;
    int Fixed::spriteRotation = 0;
    int Fixed::assetHeight = 0;
    
    Object::Object()
    {
        
        totalObjects++;
        
    }
    
    Fixed::Fixed() : Particle(){
        
        
        objectSprite.setTexture((World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_particles")));
        
        if(assetHeight == 0){
                
            sf::Texture temp = *objectSprite.getTexture();
            sf::Vector2u tempSize = temp.getSize();
            assetHeight = tempSize.y/10;
            
        }
        
        if(spriteRotation != 2) spriteRotation++;
        else spriteRotation = 0;
        
    }
    
    SpriteClone::SpriteClone() : Fixed(){
        
		objectSprite.setColor(sf::Color(28,203,158,255));
        velZ = -99;
    }
    
    PlayerBeam::PlayerBeam() : Fixed()
    {
        shaft.setSize(sf::Vector2f(20,480));
        shaft.setFillColor(sf::Color::Yellow);
        shaft.setOrigin(10,0);
        SetEffectOrigin();
        deacceleration = 0.2;
        vel.y = 1;
    
    }
    
    Spark::Spark() : Fixed(){
        
        int randomSpark = RandomNumber(2);
        if (randomSpark == 0) objectSprite.setTextureRect(sf::IntRect(0, 128, 5, 5));
        else if (randomSpark ==  1) objectSprite.setTextureRect(sf::IntRect(0, 133, 5, 5));
        else if (randomSpark == 2) objectSprite.setTextureRect(sf::IntRect(0, 138, 7, 7));
        
        vel.y = int(RandomNumber(7));
        RotateVector(vel,RandomNumber(180));
        SetEffectOrigin();
        maxFrame = 4;
        animSpeed = NORMAL;
        deacceleration = 0.5;
        
    }

	Electricity::Electricity() : Fixed() {

		objectSprite.setTextureRect(sf::IntRect(96+(RandomNumber(2) *32), 112, 32, 32));
		SetEffectOrigin();
		maxFrame = 7;
		animSpeed = VERY_FAST;

	}

	EnemySpark::EnemySpark() : Fixed() {

		int randomSpark = RandomNumber(1);
		if (randomSpark == 0) objectSprite.setTextureRect(sf::IntRect(0, 292, 7, 7));
		else if (randomSpark == 1) objectSprite.setTextureRect(sf::IntRect(0, 299, 10, 10));

		vel.y = 0;
		SetEffectOrigin();
		maxFrame = 3;
		animSpeed = FAST;

	}

	PlayerSpark::PlayerSpark() : Fixed() {

		int randomSpark = RandomNumber(1);
		if (randomSpark == 0) objectSprite.setTextureRect(sf::IntRect(27, 292, 7, 7));
		else if (randomSpark == 1) objectSprite.setTextureRect(sf::IntRect(27, 299, 10, 10));

		vel.y = 0;
		SetEffectOrigin();
		maxFrame = 3;
		animSpeed = FAST;

	}

	EnemyPart::EnemyPart() : Fixed() {

		objectSprite.setTextureRect(sf::IntRect(10, 292, 17, 17));
		vel.y = 0.5;
		RotateVector(vel, RandomNumber(180));
		SetEffectOrigin();
		maxFrame = 8;
		animSpeed = VERY_SLOW;
		deacceleration = 0.5;

	}
    
    Spark::~Spark(){
        
    }

	EnemySpark::~EnemySpark() {

	}

	PlayerSpark::~PlayerSpark() {

	}


	EnemyPart::~EnemyPart() {

	}

	LearnedAbility::LearnedAbility() : Fixed() {

		//std::cout << "created ability" << std::endl;
		objectSprite.setTexture(World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_particles"));
		objectSprite.setTextureRect(sf::IntRect(64,288, 11, 11));
		maxFrame = 3;
		animSpeed = VERY_FAST;
		maxTime = VERY_SLOW*16;
		World::GetInstance()->SetCameraTarget(*this);
		World::GetInstance()->WorldScene.textureContainer.glitchShader->mode = 1.0;
		World::GetInstance()->worldShader = 1;



	}
    
    DoorDestroy::DoorDestroy() : Fixed(){
        
        objectSprite.setTexture(World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_misc"));
        objectSprite.setTextureRect(sf::IntRect(0,42+(32*0),16,32));
        maxFrame = 3;
        animSpeed = 1000000.0;
        World::GetInstance()->SetCameraTarget(*this);
    }
    
    DeathPoof::DeathPoof() : Fixed(){
        
        objectSprite.setTextureRect(sf::IntRect(0, 106, 17, 17));
        vel.y = 1;
        RotateVector(vel,RandomNumber(180));
        SetEffectOrigin();
        maxFrame = 4;
        animSpeed = VERY_SLOW;
        deacceleration = 0.5;
        
    }

	EnemyChargeParticle::EnemyChargeParticle() : Fixed() {

		objectSprite.setTextureRect(sf::IntRect(32, 123, 15, 15));
		vel.y = RandomNumber(40, 0);
		SetEffectOrigin();
		RotateVector(vel, RandomNumber(360));

		maxFrame = 8;
		animSpeed = VERY_FAST;
		deacceleration = 0.5;

	}

	PlayerChargeParticle::PlayerChargeParticle() : Fixed() {

		objectSprite.setTextureRect(sf::IntRect(47, 123, 15, 15));
		vel.y = RandomNumber(40, 0);
		SetEffectOrigin();
		RotateVector(vel, RandomNumber(360));
		if(World::GetInstance()->WorldScene.playerPtr) parent = World::GetInstance()->WorldScene.playerPtr;
		maxFrame = 8;
		animSpeed = VERY_FAST;
		deacceleration = 0.5;

	}

	PlayerChargeParticleSmall::PlayerChargeParticleSmall() : Fixed() {

		objectSprite.setTextureRect(sf::IntRect(64, 128, 9, 9));
		vel.y = RandomNumber(16,0);
		SetEffectOrigin();
		RotateVector(vel, RandomNumber(360));
		if (World::GetInstance()->WorldScene.playerPtr) parent = World::GetInstance()->WorldScene.playerPtr;
		maxFrame = 6;
		animSpeed = VERY_FAST;
		deacceleration = 0.5;

	}

	ChargeParticleTiny::ChargeParticleTiny() : Fixed() {

		objectSprite.setTextureRect(sf::IntRect(64, 137, 1, 1));
		vel.y = -RandomNumber(2, 1);
		SetEffectOrigin();
		maxFrame = 6;
		animSpeed = VERY_FAST;

	}

	ChargeWaveAttack::ChargeWaveAttack() : Fixed() {

		objectSprite.setTextureRect(sf::IntRect(47, 123, 15, 15));
		SetEffectOrigin();
		if (World::GetInstance()->WorldScene.playerPtr) parent = World::GetInstance()->WorldScene.playerPtr;
		maxTime = VERY_SLOW * 10;

		itemQueue dash;
		dash.properties["itemType"] = "ElectricNode";
		dash.properties["PosX"] = std::to_string(objectSprite.getPosition().x);
		dash.properties["PosY"] = std::to_string(objectSprite.getPosition().y);
		dash.ptrparent = &projectileNode;
		World::GetInstance()->WorldScene.objectContainer->Queue.push_back(dash);
		dash.ptrparent = &projectileNode2;
		World::GetInstance()->WorldScene.objectContainer->Queue.push_back(dash);
		dash.ptrparent = &projectileNode3;
		World::GetInstance()->WorldScene.objectContainer->Queue.push_back(dash);

	}


	HauzerCharge::HauzerCharge() : Fixed() {

		objectSprite.setTextureRect(sf::IntRect(0, 241, 50, 50));
		SetEffectOrigin();
		animSpeed = VERY_FAST;
		maxTime = 6000000.0;
		maxFrame = 8;

	}

	LargeCharge::LargeCharge() : Fixed() {

		objectSprite.setTextureRect(sf::IntRect(64, 0, 96, 112));
		SetEffectOrigin();
		animSpeed = VERY_FAST;
		maxTime = 10000000.0;
		maxFrame = 10;

	}

	PlayerSpawn::PlayerSpawn() : Fixed() {

		objectSprite.setTextureRect(sf::IntRect(160, 0, 41, 41));
		SetEffectOrigin();
		animSpeed = VERY_FAST;
		maxTime = VERY_SLOW*20;
		maxFrame = 4;

	}

	ActionSpark::ActionSpark() : Fixed() {

		objectSprite.setTextureRect(sf::IntRect(96, 144, 67, 64));
		SetEffectOrigin();
		animSpeed = VERY_FAST;
		maxFrame = 4;

	}

	PlayerEgg::PlayerEgg() : Fixed() {

		objectSprite.setTextureRect(sf::IntRect(160, 41, 41, 41));
		SetEffectOrigin();
		animSpeed = VERY_FAST;
		maxTime = VERY_SLOW * 34;
		maxFrame = 1;

	}

	PlayerPoint::PlayerPoint() : Fixed() {

		objectSprite.setTextureRect(sf::IntRect(64, 163, 6, 6));
		SetEffectOrigin();
		SetShadow();
		animSpeed = VERY_FAST;
		maxTime = VERY_SLOW * 34;
		maxFrame = 4;

	}
    
    DamageSpark::DamageSpark() : Fixed(){
        

        int x = std::rand() % 4 + -8;
        int y = std::rand() % 4 + -8;
        
        objectSprite.setTextureRect(sf::IntRect(0, spriteRotation*35, 44, 35));
        objectSprite.move(x,y);
        SetEffectOrigin();
        maxFrame = 4;
        animSpeed = VERY_FAST;

    }
    
    DeathBoom::DeathBoom() : Fixed(){
        
        objectSprite.setTextureRect(sf::IntRect(0, 145, 25, 25));
        SetEffectOrigin();
        maxFrame = 2;
        animSpeed = FAST;
        
    }
    
    BigExp::BigExp() : Fixed(){
        
        objectSprite.setTextureRect(sf::IntRect(0, 191, 50, 50));
        SetEffectOrigin();
        maxFrame = 8;
        animSpeed = NORMAL;
        deacceleration = 0.15;
        spdReduceRate = 0.25f;
		damage = 0;
        
    }

	ShieldEffect::ShieldEffect() : Fixed() {

		objectSprite.setTextureRect(sf::IntRect(25, 143, 17, 41));
		SetEffectOrigin();
		maxFrame = 7;
		if (World::GetInstance()->WorldScene.playerPtr) parent = World::GetInstance()->WorldScene.playerPtr;
		animSpeed = VERY_FAST;

	}

	// When projectile is countered

	BlockedEffect::BlockedEffect() : Fixed() {

		objectSprite.setTextureRect(sf::IntRect(42, 143, 19, 19));
		objectSprite.setTextureRect(sf::IntRect(64, 143, 20, 20));
		SetEffectOrigin();
		maxFrame = 6;
		animSpeed = VERY_FAST;

	}

	// When player dashes

	BlockedWave::BlockedWave() : Fixed() {

		objectSprite.setTextureRect(sf::IntRect(54, 195, 42, 42));
		SetEffectOrigin();
		maxFrame = 4;
		animSpeed = FAST;

	}

	DashEffect::DashEffect() : Fixed() {

		objectSprite.setTextureRect(sf::IntRect(0, 310, 63, 13));
		SetCharacterOrigin();
		maxFrame = 8;
		animSpeed = SLOW;
		deacceleration = 5;

	}

	SlideEffect::SlideEffect() : Fixed() {

		objectSprite.setTextureRect(sf::IntRect(64, 308, 8, 15));
		SetCharacterOrigin();
		maxFrame = 5;
		animSpeed = FAST;

	}
    
    PlayerBomb::PlayerBomb() : Fixed(){
        
        totalBombs++;
        objectSprite.setTextureRect(sf::IntRect(0, 170, 20, 21));
        vel.y = 12;
        SetEffectOrigin();
        maxFrame = 4;
        maxTime = 600000.0;
		animSpeed = VERY_FAST;
        deacceleration = 0.25;
		damage = 1;
        
    }

	QuakeDirt::QuakeDirt() : Fixed() {

		objectSprite.setTextureRect(sf::IntRect(16,128, 2, 2));
		vel.y = 1;
		maxTime = VERY_SLOW*5;
		acceleration = 2;
		SetEffectOrigin();

	}

	EnemyCharge::EnemyCharge() : Fixed() {

		objectSprite.setTextureRect(sf::IntRect(44,168, 15, 15));
		SetEffectOrigin();
		maxFrame = 4;
		maxTime = 1000000.0;
		animSpeed = VERY_FAST;
	}

	BounceCrawler::BounceCrawler() : Fixed() {

		objectSprite.setTextureRect(sf::IntRect(80, 306, 17, 17));
		SetEffectOrigin();

		//Bounce crawler is slower depending on your weapon, so set the velocity if it is a specific weapon
		vel.y = BounceCrawler::BehaviorsForWeapons.at(World::GetInstance()->GlobalMembers.currentWeapon).first;
		
		maxFrame = 2;

		//Bounce crawler lasts longer the higher your level
		maxTime = BounceCrawler::BehaviorsForWeapons.at(World::GetInstance()->GlobalMembers.currentWeapon).second + (SLOW * World::GetInstance()->GlobalMembers.currentPowerLevel);
		animSpeed = VERY_FAST;
	}


	StarSpawn::StarSpawn() : Fixed() {

		objectSprite.setTextureRect(sf::IntRect(44, 168, 15, 15));
		SetEffectOrigin();
		maxFrame = 4;
		maxTime = 1000000.0;
		animSpeed = VERY_FAST;
	}
    
    
    Hit::Hit(){
        
        objectSprite.setTexture((World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_hit")));
        objectSprite.setTextureRect(sf::IntRect(0, 0, 30, 30));
        objectSprite.setOrigin(15,15);
    
    }
    
    
    Projectile::Projectile() : Object ()
    {
        
        objectSprite.setTexture((World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_projectiles")));
        type = "Projectile";
        
    }
    
    EnemyProjectile::EnemyProjectile() : Projectile(){
        
        type = "EnemyProjectile";
        
    }
    
    EnemyBlip::EnemyBlip() : EnemyProjectile(){
        
        objectSprite.setTextureRect(sf::IntRect(0, 19,7, 7));
		speed = 1.5;
        vel.y = speed;
        SetEffectOrigin();
        SetHitBox(sf::Vector2f(3,3));
        active = true;

    }

	EnemyMine::EnemyMine() : EnemyProjectile() {

		objectSprite.setTextureRect(sf::IntRect(0, 19, 7, 7));
		speed = 0;
		maxFrame = 1;
		vel.y = speed;
		SetEffectOrigin();
		SetHitBox(sf::Vector2f(3, 3));
		active = true;
		maxTime = VERY_SLOW * 16;

	}

	EnemyHomingBlip::EnemyHomingBlip() : EnemyProjectile() {

		objectSprite.setTextureRect(sf::IntRect(14, 170, 7, 15));
		speed = 1;
		vel.y = speed;
		SetEffectOrigin();
		SetHitBox(sf::Vector2f(3, 3));
		active = true;

	}

	EnemyLaser::EnemyLaser() : EnemyProjectile() {

		objectSprite.setTextureRect(sf::IntRect(0, 55, 16, 13));
		laserBody.setTexture((World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_projectiles")));
		laserHead.setTexture((World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_projectiles")));
		laserBody.setTextureRect(sf::IntRect(32, 55, 16, 2));
		laserHead.setTextureRect(sf::IntRect(32, 58, 15, 15));
		speed = 2;
		maxFrame = 1;
		vel.y = speed;
		SetEffectOrigin();
		SetHitBox(sf::Vector2f(6, 6));
		laserHead.setOrigin(7.5, 7.5);
		laserBody.setOrigin(0, 0.5);

		active = true;
		followsPlayer = true;
		emitter = "DeathPoof";
		emitTime = SLOW;

	}

	BigEnemyLaser::BigEnemyLaser() : EnemyLaser() {

		objectSprite.setTextureRect(sf::IntRect(0, 152, 41, 26));
		laserBody.setTexture((World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_projectiles")));
		laserHead.setTexture((World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_projectiles")));
		laserBody.setTextureRect(sf::IntRect(32, 79, 16, 14));
		laserHead.setTextureRect(sf::IntRect(96, 53, 41, 41));
		speed = 2.5;
		maxFrame = 1;
		vel.y = speed;
		SetEffectOrigin();
		SetHitBox(sf::Vector2f(12, 12));
		laserHead.setOrigin(20.5, 20.5);
		laserBody.setOrigin(0, 7);

		active = true;
		followsPlayer = true;
		emitter = "DeathPoof";
		emitTime = SLOW;

	}

	HauzerSmog::HauzerSmog() : EnemyProjectile() {

		sprite = 96 + (17 *RandomNumber(2));
		objectSprite.setTextureRect(sf::IntRect(0, 96, 17, 17));
		speed = RandomNumber(6, 4);
		vel.y = speed;
		SetEffectOrigin();
		SetHitBox(sf::Vector2f(6, 6));
		active = true;

	}

	HauzerSpear::HauzerSpear() : EnemyProjectile() {

		objectSprite.setTextureRect(sf::IntRect(0, 69, 7, 27));
		vel.y = speed;
		SetEffectOrigin();
		SetHitBox(sf::Vector2f(3, 6));
		active = true;
		acceleration = 0.05;
		emitter = "EnemySpark";
		emitTime = SLOW;
		emitScatter = true;
		emitCount = 3;
		maxFrame = 1;
		rotation = true;

	}
    
    Particle::Particle() : Object ()
    {
        
        objectSprite.setTexture(World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_particles"));
		type = "Particle";

        
    }
    
    Passive::Passive() : Particle(){
        
        
        
    }
    
    Prop::Prop() : Object(){
        
        
        
    }
    
    PropFlame::PropFlame() : Prop(){
        

        objectSprite.setTexture((World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_misc")));
        objectSprite.setTextureRect(sf::IntRect(0, 5, 11, 30));
        maxFrame = 5;
        animSpd = SLOW;
        frame = RandomNumber(5);
        
    }

	PlayerBirth::PlayerBirth() : Prop() {

		objectSprite.setTexture(World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_misc"));
		objectSprite.setTextureRect(sf::IntRect(0, 251, 105, 114));
		objectSprite.setOrigin(105 / 2, 114 / 2);
		animSpd = VERY_FAST;
		maxFrame = 1;
		World::GetInstance()->SetCameraTarget(*this);

	}
    
    float Object::GetYPosition(){
        
        return objectSprite.getPosition().y;
        
    }
    
    sf::IntRect Object::GetObjectBounds(){
        
		// Gets the local bounds of the objects sprite, minus its origin 
        return sf::IntRect(objectHitBox.getPosition().x - objectHitBox.getOrigin().x,objectHitBox.getPosition().y - objectHitBox.getOrigin().y,objectHitBox.getSize().x,objectHitBox.getSize().y);
    }
    
    Dirt::Dirt() : Particle (){
        
        isAnimated = false;
        objectSprite.setTextureRect(sf::IntRect(0, 6, 2, 2));
        gravity = 0.2f;
        
        int q = rand() % 3;
        
        if(q == 0 || q == 1)
        {
            
            objectSprite.setScale(0.5,0.5);
            
        }
        
        
    }
    
    Gib::Gib() : Particle(){
        
        objectSprite.setTexture(gibTexture);
        isAnimated = false;
        gravity = 0.3f;
        
    }
    
    
    Bullet::Bullet() : Projectile ()
    {
        
        objectSprite.setTexture((World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_projectiles")));
        objectSprite.setTextureRect(sf::IntRect(0, 0, 25, 4));
        damage = 6;
        
    }
    
    PlayerLaser1::PlayerLaser1() : Projectile()
    {
        
        objectSprite.setTexture((World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_projectiles")));
        objectSprite.setTextureRect(sf::IntRect(0, 178, 5, 7));
        objectSprite.setOrigin(2,7/2);
        SetEffectOrigin();
        vel.y = 5;
		damage = 9;
		maxFrame = 2;
        SetHitBox(sf::Vector2f(16,16));
        
    }

	PlayerShotgun1::PlayerShotgun1() : Projectile()
	{

		objectSprite.setTexture((World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_projectiles")));
		objectSprite.setTextureRect(sf::IntRect(0, 417, 9, 18));
		objectSprite.setOrigin(9/2, 18 / 2);

		SetEffectOrigin();
		vel.y = 10;
		damage = 4;
		maxFrame = 2;
		SetHitBox(sf::Vector2f(16, 16));
		maxTime = SLOW*2;

	}

	PlayerHoming1::PlayerHoming1() : Projectile()
	{

		objectSprite.setTexture((World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_projectiles")));
		objectSprite.setTextureRect(sf::IntRect(0, 435, 20, 20));
		objectSprite.setOrigin(20/2, 20/2);
		linkSprite.setTexture((World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_projectiles")));
		linkSprite.setTextureRect(sf::IntRect((RandomNumber(2) * 10), 455, 10, 16));
		linkSprite.setOrigin(10/2, 16/2);
		SetEffectOrigin();
		vel.y = 0;
		damage = 2;
		maxFrame = 0;
		SetHitBox(sf::Vector2f(16, 16));
		health = 10;
		destroyOnImpact = false;
		count = 1;

	}

	ElectricNode::ElectricNode() : Projectile()
	{
		objectSprite.setTexture((World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_projectiles")));
		objectSprite.setTextureRect(sf::IntRect(0, 32, 20, 20));
		objectSprite.setOrigin(5, 5);
		SetEffectOrigin();
		vel.y = 0;
		damage = 14;
		maxFrame = 4;
		SetHitBox(sf::Vector2f(16, 16));
		destroyOnImpact = false;

	}


	PlayerDashBall::PlayerDashBall() : Projectile()
	{

		objectSprite.setTexture((World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_projectiles")));
		objectSprite.setTextureRect(sf::IntRect(0, 320, 36, 57));
		objectSprite.setOrigin(20.5, 20.5);
		SetEffectOrigin();
		vel.y = 0;
		damage = 32;
		maxFrame = 4;
		SetHitBox(sf::Vector2f(32, 32));
		destroyOnImpact = false;

	}

	PlayerLaser2::PlayerLaser2() : PlayerLaser1()
	{
		objectSprite.setTexture((World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_projectiles")));
		objectSprite.setTextureRect(sf::IntRect(0, 185, 5, 14));
		objectSprite.setOrigin(2, 7);
		SetEffectOrigin();
		vel.y = 5;
		damage = 7;
		SetHitBox(sf::Vector2f(6,6));
		health = 1;

	}

	PlayerLaser3::PlayerLaser3() : PlayerLaser1()
	{
		objectSprite.setTexture((World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_projectiles")));
		objectSprite.setTextureRect(sf::IntRect(0, 199, 5, 13));
		objectSprite.setOrigin(2.5, 7);
		SetEffectOrigin();
		vel.y = 5;
		damage = 7;
		SetHitBox(sf::Vector2f(6, 6));
		health = 1;

	}

	PlayerLaser4::PlayerLaser4() : PlayerLaser1()
	{
		objectSprite.setTexture((World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_projectiles")));
		objectSprite.setTextureRect(sf::IntRect(0, 212, 8, 18));
		objectSprite.setOrigin(4, 9);
		SetEffectOrigin();
		vel.y = 5;
		damage = 7;
		SetHitBox(sf::Vector2f(6, 6));
		health = 1;

	}

	PlayerLaser5::PlayerLaser5() : PlayerLaser1()
	{
		objectSprite.setTexture((World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_projectiles")));
		objectSprite.setTextureRect(sf::IntRect(0, 230, 9, 21));
		objectSprite.setOrigin(4.5, 10);
		SetEffectOrigin();
		vel.y = 5;
		damage = 7;
		SetHitBox(sf::Vector2f(6, 6));
		health = 1;
	}

	PlayerBeam1::PlayerBeam1() : Projectile()
	{
		objectSprite.setTexture((World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_projectiles")));
		objectSprite.setTextureRect(sf::IntRect(0, 395, 7, 14));
		objectSprite.setScale(sf::Vector2f(1, 50));
		SetEffectOrigin();
		objectSprite.setOrigin(7 / 2, -0.25);
		vel.y = 5;
		damage = 16;
		maxFrame = 2;
		maxTime = VERY_SLOW * 5;
		SetHitBox(sf::Vector2f(16, 16));
		parent = World::GetInstance()->WorldScene.playerPtr;
	}

	PlayerBeamNode::PlayerBeamNode() : Projectile() 
	{

		objectSprite.setTexture((World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_projectiles")));
		objectSprite.setTextureRect(sf::IntRect(32, 58, 16, 16));
		objectSprite.setOrigin(8, 8);
		SetEffectOrigin();
		vel.y = 16;
		damage = 2;
		SetHitBox(sf::Vector2f(16, 16));
		destroyOnImpact = false;
		deleteOnOutOfBounds = false;

	}
    
    PlayerBoomerang::PlayerBoomerang() : Projectile(){
        
        objectSprite.setTexture((World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_projectiles")));
        objectSprite.setTextureRect(sf::IntRect(0, 32, 20, 20));
        objectSprite.setOrigin(10,10);
        SetEffectOrigin();
        vel.y = 3;
        damage = 4;
        SetHitBox(sf::Vector2f(12,12));
		destroyOnImpact = false;
        totalBoomerangs++;
        
        
    }
    
    PlayerRepeater1::PlayerRepeater1() : Projectile()
    {
        objectSprite.setTexture((World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_projectiles")));
        objectSprite.setTextureRect(sf::IntRect(0, 303, 5, 5));
        objectSprite.setOrigin(2,2);
        SetEffectOrigin();
        vel.y = 4;
        damage = 4;
        maxFrame = 2;
        SetHitBox(sf::Vector2f(16, 16));

    
    }

	PlayerRepeater2::PlayerRepeater2() : Projectile()
	{

		objectSprite.setTexture((World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_projectiles")));
		objectSprite.setTextureRect(sf::IntRect(0, 308, 5, 9));
		objectSprite.setOrigin(2.5, 5);
		SetEffectOrigin();
		vel.y = 3;
		damage = 3;
		maxFrame = 2;
		SetHitBox(sf::Vector2f(3, 3));
		health = 2;

	}

	PlayerRepeater3::PlayerRepeater3() : Projectile()
	{

		objectSprite.setTexture((World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_projectiles")));
		objectSprite.setTextureRect(sf::IntRect(0, 317, 5, 14));
		objectSprite.setOrigin(2.5, 7);
		SetEffectOrigin();
		vel.y = 3;
		damage = 3;
		maxFrame = 2;
		SetHitBox(sf::Vector2f(3, 3));
		health = 2;

	}

	PlayerRepeater6::PlayerRepeater6() : Projectile()
	{

		objectSprite.setTexture((World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_projectiles")));
		objectSprite.setTextureRect(sf::IntRect(0, 284, 9, 25));
		objectSprite.setOrigin(2.5, 2);
		SetEffectOrigin();
		vel.y = 3;
		damage = 3;
		maxFrame = 2;
		SetHitBox(sf::Vector2f(3, 3));
		health = 10;
		destroyOnImpact = false;
	}
    
    PlayerBombExp::PlayerBombExp() : Projectile()
    {
        
        objectSprite.setTexture((World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_projectiles")));
        objectSprite.setTextureRect(sf::IntRect(0, 0, 60, 60));
        SetEffectOrigin();
        vel.y = 0;
        damage = 20;
        SetHitBox(sf::Vector2f(70,70));
		destroyOnImpact = false;
        
    }
    
	void PlayerBombExp::Draw(sf::RenderTarget& window) {

	}

	void PlayerBeamNode::Draw(sf::RenderTarget& window) {

	}

	void PlayerHoming1::Draw(sf::RenderTarget& window) {

		if (isAttacking == true) {

			linkSprite.setRotation(RandomNumber(3) * 90);
			World::GetInstance()->DrawObject(linkSprite);
		}

		World::GetInstance()->DrawObject(objectSprite);

	}
    
    Explosion::Explosion() : Object ()
    {
        
        objectSprite.setTexture((World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_polygon")));
        objectSprite.setOrigin(40,40);
        objectSprite.setTextureRect(sf::IntRect(0, 0, 80, 80));
        
    }
    
    Cluster::Cluster() : Object ()
    {
        
        
        
    }
    
    
    Laser::Laser() : Projectile ()
    {
        
        objectSprite.setTexture((World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_blip_3")));
        objectSprite.setOrigin(3,3);
        objectSprite.setTextureRect(sf::IntRect(0, 0, 6, 6));
        Projectile::damage = 10;
        
    }
    
    HitNum::HitNum() : Object()
    {
        
        if(hitFnt.getInfo().family == ""){
            
            hitFnt.loadFromFile("/Users/darionmccoy/Dropbox/Game Design Stuff/Projects/Backup/HardSoda/HardSoda/fonts/apfont.ttf");
            const_cast<sf::Texture&>(hitFnt.getTexture(16)).setSmooth(false);
            
        }
        
        num.setFont(hitFnt);
        num.setCharacterSize(16);
        num.setColor(sf::Color::White);
        
        numbg.setFillColor(sf::Color::Black);
        velZ = 99;
        
        
        
    }
    
    
    // Update Functions
    
	int Object::Return(int p) {

		return 0;
	}

    void Object::Update()
    {
        
        
        
    }

	void PlayerBirth::Update() {

		Prop::Update();

		if (phase == 0 && World::GetInstance()->Timer(*this, VERY_SLOW * 20)) {

			World::GetInstance()->WorldScene.audioContainer.PlaySFX("sfx_frission");
			World::GetInstance()->WorldScene.textureContainer.glitchShader->mode = 0.0;
			World::GetInstance()->WorldScene.textureContainer.glitchShader->opac = 1.0;
			World::GetInstance()->worldShader = 1;


			phase = 1;


		}

		else if (phase == 1 && World::GetInstance()->Timer(*this, VERY_SLOW * 10)) {

			itemQueue charge;
			charge.properties["PosX"] = std::to_string(objectSprite.getPosition().x+4);
			charge.properties["PosY"] = std::to_string(objectSprite.getPosition().y-25);
			charge.properties["itemType"] = "LargeCharge";
			World::GetInstance()->WorldScene.audioContainer.PlaySFX("sfx_wigglyscreech");
			World::GetInstance()->WorldScene.objectContainer->Queue.push_back(charge);
			World::GetInstance()->WorldScene.audioContainer.PlaySFX("sfx_static");


			charge.properties["PosX"] = std::to_string(objectSprite.getPosition().x - 1);
			charge.properties["PosY"] = std::to_string(objectSprite.getPosition().y);
			charge.properties["itemType"] = "PlayerSpawn";
			World::GetInstance()->WorldScene.objectContainer->Queue.push_back(charge);

			World::GetInstance()->WorldScene.textureContainer.glitchShader->mode = 1.0;


			phase = 2;

		}

		else if (phase == 2 && World::GetInstance()->Timer(*this, VERY_SLOW*20)) {

			World::GetInstance()->WorldScene.audioContainer.PlaySFX("sfx_static");


			phase = 3;

		}


		else if (phase == 3 && World::GetInstance()->Timer(*this, VERY_SLOW * 40)) {

			misDestroyed = true;

		}

		if (phase == 2 && World::GetInstance()->Timer(*this, SLOW)) { 

			itemQueue charge2;
			charge2.properties["PosX"] = std::to_string(objectSprite.getPosition().x-1);
			charge2.properties["PosY"] = std::to_string(objectSprite.getPosition().y);
			charge2.properties["itemType"] = "PlayerChargeParticle";
			World::GetInstance()->WorldScene.objectContainer->Queue.push_back(charge2);
			World::GetInstance()->ScreenShake(5);

			charge2.properties["PosX"] = std::to_string((objectSprite.getPosition().x - 40) + RandomNumber(80));
			charge2.properties["PosY"] = std::to_string((objectSprite.getPosition().y - 40) + RandomNumber(80));
			charge2.properties["itemType"] = "Electricity";
			World::GetInstance()->WorldScene.objectContainer->Queue.push_back(charge2);

		}

		if (phase >= 3 && World::GetInstance()->Timer(*this, SLOW)) {

			itemQueue charge2;
			charge2.properties["PosX"] = std::to_string((objectSprite.getPosition().x - 40) + RandomNumber(80));
			charge2.properties["PosY"] = std::to_string((objectSprite.getPosition().y - 40) + RandomNumber(80));
			charge2.properties["itemType"] = "Electricity";
			World::GetInstance()->WorldScene.objectContainer->Queue.push_back(charge2);


		}



	}

	void Buddy::Update() 
	{
		if (World::GetInstance()->WorldScene.playerPtr) {

			float oldx = objectSprite.getPosition().x;
			float oldy = objectSprite.getPosition().y;

			float pdir = 0.785 * (World::GetInstance()->WorldScene.playerPtr->fireDir + buddyPos + 3);

			//pdir uses 45 degree angles
			//cos uses 0.0 to 0.9
			//45 / 360 = 0.125


			float plx = World::GetInstance()->WorldScene.playerPtr->objectSprite.getPosition().x;
			float ply = World::GetInstance()->WorldScene.playerPtr->objectSprite.getPosition().y+5;

			float nx = plx + (cos(pdir) * 20.0);
			float ny = ply + (sin(pdir) * 20.0);

			oldx += (nx - oldx) / 2;
			oldy += (ny - oldy) / 2;

			objectSprite.setPosition(oldx,oldy-posZ);
			posZ = (5 + cos(World::GetInstance()->clock2.getElapsedTime().asMilliseconds()*0.0025));

			UpdateShadow();

		}

	}
    

    void Fixed::Update(){
        
        
        if(top == -1)top = objectSprite.getTextureRect().top;
        
        if(World::GetInstance()->Timer(*this,animSpeed))
        {
            
                objectSprite.setTextureRect(sf::IntRect(objectSprite.getTextureRect().left,(frame * assetHeight) + top,objectSprite.getTextureRect().width,objectSprite.getTextureRect().height));
                
                if(frame < maxFrame) {
                    
                    frame++;
                    animSpeed -= spdReduceRate;
                    vel.x -= (vel.x * deacceleration);
                    vel.y -= (vel.y * deacceleration);

					if ((acceleration != 0) && (World::GetInstance()->Timer(*this,NORMAL))) {

						animSpeed -= acceleration;
					}

                    if(rotation) RotateVector(vel,animSpeed);
                
                }
                
				else if (maxTime != 0) {

					if (dontLoop == true) frame = maxFrame;
					else frame = 0;

				}
    
               else misDestroyed = true;
            
                if((maxTime != 0) && (World::GetInstance()->Timer(*this,maxTime))) misDestroyed = true;
        
            
        }
        
		if(parent) objectSprite.move(vel.x + parent->vel.x, vel.y + parent->vel.y);
		else objectSprite.move(vel.x,vel.y);

    }
    
    void SpriteClone::Update(){
        
        if(World::GetInstance()->Timer(*this,FAST,NODELAY)){
            
            if(objectSprite.getColor().a - 30 <= 0) misDestroyed = true;
            
            else objectSprite.setColor(sf::Color::Color(objectSprite.getColor().r, objectSprite.getColor().g, objectSprite.getColor().b,objectSprite.getColor().a - 35));
            
        }
    
    }

	void PlayerPoint::Update() {

		Fixed::Update();
	
		if (World::GetInstance()->WorldScene.playerPtr && World::GetInstance()->WorldScene.playerPtr->mana < 100) {

			if (PlayerDistance(60)) {

				float px = objectSprite.getPosition().x;
				float py = objectSprite.getPosition().y;

				int playerPosX = World::GetInstance()->WorldScene.playerPtr->objectSprite.getPosition().x;
				int playerPosY = World::GetInstance()->WorldScene.playerPtr->objectSprite.getPosition().y;

				px += (playerPosX - objectSprite.getPosition().x) / 5;
				py += (playerPosY - objectSprite.getPosition().y) / 5;

				objectSprite.setPosition(sf::Vector2f(px, py));

			}

			if (PlayerDistance(15) && World::GetInstance()->WorldScene.playerPtr && World::GetInstance()->WorldScene.playerPtr->PlayerManaMeter) {

				World::GetInstance()->WorldScene.audioContainer.PlaySFX("sfx_lswallow", false);
				World::GetInstance()->WorldScene.playerPtr->mana++;
				misDestroyed = true;
				World::GetInstance()->WorldScene.playerPtr->PlayerManaMeter->got = true;


			}

		}


		UpdateShadow();
	
	}

	void EnemyLaser::Update() {

		Projectile::Update();
		laserBodyvel.x = 0;
		laserBodyvel.y = -16;
		laserBody.setRotation(-GetAngle(laserBody.getPosition(),objectSprite.getPosition())-90);
		RotateVector(laserBodyvel, -GetAngle(laserBody.getPosition(), objectSprite.getPosition()));
		if (objReference->type == "") misDestroyed = true;
		else if (World::GetInstance()->Timer(*this, VERY_SLOW*10)) misDestroyed = true;
	}

	void PlayerBomb::Update() {


		if (top == -1)top = objectSprite.getTextureRect().top;

		if (World::GetInstance()->Timer(*this, animSpeed))
		{

			objectSprite.setTextureRect(sf::IntRect(0, (frame * assetHeight) + top, objectSprite.getTextureRect().width, objectSprite.getTextureRect().height));

			if (frame < maxFrame) {

				frame++;
				animSpeed -= spdReduceRate;
				vel.x -= (vel.x * deacceleration);
				vel.y -= (vel.y * deacceleration);

				if ((acceleration != 0) && (World::GetInstance()->Timer(*this, NORMAL))) {

					animSpeed -= acceleration;
				}

				if (rotation) RotateVector(vel, animSpeed);

			}

			else if (maxTime != 0) frame = 0;

			else misDestroyed = true;

			if ((maxTime != 0) && (World::GetInstance()->Timer(*this, maxTime))) misDestroyed = true;


		}

		objectSprite.move(vel.x, vel.y);
		if (PlayerBomb::totalBombs > 1) {

			misDestroyed = true; 
			PlayerBomb::totalBombs = 0;
		}
	}
    
    void DoorDestroy::Update(){
        
        if(top == -1)top = objectSprite.getTextureRect().top;
        
        if(World::GetInstance()->Timer(*this,animSpeed))
        {
            
            objectSprite.setTextureRect(sf::IntRect(0,(frame * assetHeight) + top,objectSprite.getTextureRect().width,objectSprite.getTextureRect().height));
            
            if(frame < maxFrame) {
                
                frame++;
                animSpeed -= spdReduceRate;
                vel.x -= (vel.x * deacceleration);
                vel.y -= (vel.y * deacceleration);
                if(rotation) RotateVector(vel,animSpeed);
                
            }
            
            else if(maxTime != 0) frame = 0;
            
            else misDestroyed = true;
            
            if((maxTime != 0) && (World::GetInstance()->Timer(*this,maxTime))) misDestroyed = true;
            
            
        }
        
        if(World::GetInstance()->Timer(*this,NORMAL)){
            
            World::GetInstance()->ScreenShake(5);
            World::GetInstance()->WorldScene.audioContainer.PlaySFX("sfx_quake");
            
        }
        
    }

	void LearnedAbility::Update() {

		Fixed::Update();

		sf::Vector2f tempPos = objectSprite.getPosition();
		tempPos.x += double(World::GetInstance()->WorldScene.playerPtr->objectSprite.getPosition().x - tempPos.x) / 100;
		tempPos.y += double((World::GetInstance()->WorldScene.playerPtr->objectSprite.getPosition().y-12) - tempPos.y) / 100;
		objectSprite.setPosition(tempPos);

		if (World::GetInstance()->Timer(*this, FAST, NODELAY)) {

			World::GetInstance()->WorldScene.audioContainer.PlaySFX("sfx_longring", false);

		}


		if (World::GetInstance()->Timer(*this, NORMAL)) {

			itemQueue effect;
			effect.properties["PosX"] = std::to_string(objectHitBox.getPosition().x);
			effect.properties["PosY"] = std::to_string(objectHitBox.getPosition().y);
			effect.properties["itemType"] = "ChargeParticleTiny";
			World::GetInstance()->WorldScene.objectContainer->Queue.push_back(effect);

		}

	}

	void ChargeWaveAttack::Update() {

		count+=0.1;

		Fixed::Update();

		if (projectileNode) {

			//std::cout << time << std::endl;

			float nPosX = objectSprite.getPosition().x + (cos(count) * (count*10));
			float nPosY = objectSprite.getPosition().y + (sin(count) * (count*10));
			float nPosX2 = objectSprite.getPosition().x + (cos(count+8) * (count*10));
			float nPosY2 = objectSprite.getPosition().y + (sin(count+8) * (count*10));
			float nPosX3 = objectSprite.getPosition().x + (cos(count +17) * (count * 10));
			float nPosY3 = objectSprite.getPosition().y + (sin(count +17) * (count * 10));
			
			projectileNode->objectSprite.setPosition(nPosX, nPosY);
			projectileNode2->objectSprite.setPosition(nPosX2, nPosY2);
			projectileNode3->objectSprite.setPosition(nPosX3, nPosY3);

		
		
		}

	}

	void BounceCrawler::Update() {

		Fixed::Update();

		// Here we need to check where the position of the crawler is

		//depending on where it is, we just need to continue pushing it around the wall

		//before we create the projectile, get its direction based off the crawlers direction

		int centerX = World::GetInstance()->WorldScene.levelContainer->lvlSize.x / 2;
		int centerY = World::GetInstance()->WorldScene.levelContainer->lvlSize.y / 2;
		int shootDirection;

		if (direction == 0) {

			if(objectHitBox.getPosition().x < centerX) shootDirection = 6;
			else shootDirection = 2;

		}

		else if (direction == 2) {

			if (objectHitBox.getPosition().y < centerY) shootDirection = 0;
			else shootDirection = 4;
		}

		else if (direction == 4) {

			if (objectHitBox.getPosition().x < centerX) shootDirection = 6;
			else shootDirection = 2;
		}

		else if (direction == 6) {

			if (objectHitBox.getPosition().y < centerY) shootDirection = 0;
			else shootDirection = 4;
		}

		if (World::GetInstance()->Timer(*this, NORMAL, NODELAY)) {

			itemQueue proj;
			proj.properties["PosX"] = std::to_string(objectHitBox.getPosition().x);
			proj.properties["PosY"] = std::to_string(objectHitBox.getPosition().y);
			proj.properties["Direction"] = std::to_string(shootDirection);
			proj.properties["itemType"] = World::GetInstance()->GlobalMembers.WeaponNames.at(World::GetInstance()->GlobalMembers.currentWeapon) + std::to_string(World::GetInstance()->GlobalMembers.currentPowerLevel +1);
			
			if (World::GetInstance()->GlobalMembers.currentWeapon == 1) {

				proj.properties["Direction"] = std::to_string((shootDirection * 45));
				World::GetInstance()->WorldScene.objectContainer->Queue.push_back(proj);
				proj.properties["Direction"] = std::to_string((shootDirection * 45) + 15);
				World::GetInstance()->WorldScene.objectContainer->Queue.push_back(proj);
				proj.properties["Direction"] = std::to_string((shootDirection * 45) - 15);
				World::GetInstance()->WorldScene.objectContainer->Queue.push_back(proj);
			}
			else World::GetInstance()->WorldScene.objectContainer->Queue.push_back(proj);

			World::GetInstance()->WorldScene.audioContainer.PlaySFX("sfx_laser9");

		}
	}
    
    void PlayerBeam::Update(){
        
        if(shaft.getSize().x == 20){
            
            sf::Vector2f nodePosition = shaft.getPosition();
            
            for(int i = 0; i != 10; i++){
                
                itemQueue particles;

				/*
                particles.properties["itemType"] = "Electricity";
                
                particles.properties["PosX"] = std::to_string(nodePosition.x + RandomNumber(10));
                particles.properties["PosY"] = std::to_string(nodePosition.y + RandomNumber(10));
                World::GetInstance()->WorldScene.objectContainer->Queue.push_back(particles);
                
                
                particles.properties["PosX"] = std::to_string(nodePosition.x - RandomNumber(10));
                particles.properties["PosY"] = std::to_string(nodePosition.y - RandomNumber(10));
                World::GetInstance()->WorldScene.objectContainer->Queue.push_back(particles);
                
                
                particles.properties["PosX"] = std::to_string(nodePosition.x - RandomNumber(10));
                particles.properties["PosY"] = std::to_string(nodePosition.y + RandomNumber(10));
                World::GetInstance()->WorldScene.objectContainer->Queue.push_back(particles);
                
                
                particles.properties["PosX"] = std::to_string(nodePosition.x + RandomNumber(10));
                particles.properties["PosY"] = std::to_string(nodePosition.y - RandomNumber(10));
                World::GetInstance()->WorldScene.objectContainer->Queue.push_back(particles);
                */
              

                particles.properties["PosX"] = std::to_string(nodePosition.x);
                particles.properties["PosY"] = std::to_string(nodePosition.y);
                
                
                particles.properties["itemType"] = "PlayerBombExp";
                World::GetInstance()->WorldScene.objectContainer->Queue.push_back(particles);
                
                /*
                particles.properties["itemType"] = "BigExp";
                World::GetInstance()->WorldScene.objectContainer->Queue.push_back(particles);
                */
                 
                nodePosition.x += vel.x * 50;
                nodePosition.y += vel.y * 50;
                
            }
        }
        
        int r = RandomNumber(255);
        int g = RandomNumber(255);
        int b = RandomNumber(255);
        shaft.setFillColor(sf::Color::Color(r,g,b));
        
        
        if(World::GetInstance()->Timer(*this,animSpeed))
        {
            shaft.setSize(sf::Vector2f(shaft.getSize().x - (shaft.getSize().x*deacceleration),shaft.getSize().y));
            
        }
        
        shaft.setOrigin(shaft.getSize().x/2,0);
        
        if(shaft.getSize().x < 1) misDestroyed = true;
    
        
    }
    
    void Hit::Update(){
        
    }
    
    void Particle::Update()
    {
        sf::Vector2f prevPos = objectSprite.getPosition();
        
        if(World::GetInstance()->Timer(*this,VERY_FAST))
        {
            
            if(isAnimated){
                
                if(frame == 3)
                {
                    
                    frame = 0;
                    
                }
                
                objectSprite.setTextureRect(sf::IntRect(frame * objectSprite.getTextureRect().width,
                                                                objectSprite.getTextureRect().top,
                                                                objectSprite.getTextureRect().width,
                                                                objectSprite.getTextureRect().height));
                
                frame++;
                    
                }
            
            vel.y++;
            
        }
            
    
        if (gravity <= 0.1)
        {
            
            misDestroyed = true;
            
        }

        objectSprite.move(vel.x * (0.3f),vel.y * (gravity) * gravity);
        World::GetInstance()->WorldScene.levelContainer->CheckCollison(prevPos, objectSprite.getPosition(), *this);
        
    
    }
       
    void Projectile::Update()
    {

		if (followsPlayer == true && World::GetInstance()->WorldScene.playerPtr) {
			
			vel.x = 0;
			vel.y = -speed;

			RotateVector(vel, -GetAngle(objectSprite.getPosition(), World::GetInstance()->WorldScene.playerPtr->objectSprite.getPosition()));
		}
        
        if(World::GetInstance()->Timer(*this,VERY_FAST))
        {
            
            objectSprite.setTextureRect(sf::IntRect(frame * objectSprite.getTextureRect().width,
															objectSprite.getTextureRect().top,
                                                            objectSprite.getTextureRect().width,
                                                            objectSprite.getTextureRect().height));
            frame++;
            
            if(frame > maxFrame)
            {
				frame = 0;
                
            }
            
        }
        
        
		if (deacceleration != 0) {

			if (World::GetInstance()->Timer(*this, NORMAL)) {

				vel.x -= (vel.x * deacceleration);
				vel.y -= (vel.y * deacceleration);
				speed -= deacceleration;
			}
		}

		if (acceleration != 0) {

			if (World::GetInstance()->Timer(*this, NORMAL)) {

				vel.x += (vel.x * acceleration);
				vel.y += (vel.y * acceleration);
				speed += acceleration;


			}
		}

		if (emitter != "" && misDestroyed == false) {

			if (World::GetInstance()->Timer(*this, emitTime)) {

				itemQueue particles;

				for (int i = 0; i < emitCount; i++) {

					if (emitScatter == true) {

						particles.properties["PosX"] = std::to_string(double(objectSprite.getPosition().x - 8 + RandomNumber(16)));
						particles.properties["PosY"] = std::to_string(double(objectSprite.getPosition().y - 8 + RandomNumber(16)));

					}

					else {

						particles.properties["PosX"] = std::to_string(double(objectSprite.getPosition().x));
						particles.properties["PosY"] = std::to_string(double(objectSprite.getPosition().y));

					}

					particles.properties["itemType"] = emitter;
					World::GetInstance()->WorldScene.objectContainer->Queue.push_back(particles);

				}

			}

		}


		if (parent) {

			objectSprite.setPosition(parent->objectSprite.getPosition());

		}

		else  objectSprite.move(vel.x,vel.y);
        
        
        if(objectSprite.getPosition().x >= World::GetInstance()->WorldScene.levelContainer->lvlSize.x || objectSprite.getPosition().x <= 0)
        {
            
            isCollided = true;
            
        }

		if (maxTime != 0) {

			if (World::GetInstance()->Timer(*this, maxTime)) {

				misDestroyed = true;
			}

		}
    
        
    }
    
    void EnemyBlip::Update(){
        
        if(World::GetInstance()->Timer(*this,VERY_FAST))
        {
            
            
            objectSprite.setTextureRect(sf::IntRect((frame * 7),19,7,7));
            
            frame++;
            
            if(frame >= 2)
            {
                
                frame = 0;
                
            }
            
        }
        
        
        objectSprite.move(vel.x,vel.y);
        
    }

	void EnemyHomingBlip::Update() {

		if (World::GetInstance()->Timer(*this, VERY_FAST))
		{


			objectSprite.setTextureRect(sf::IntRect((frame * 7), 70, 7, 15));

			frame++;

			if (frame >= 2)
			{

				frame = 0;

			}

		}
		
		vel.x = 0;
		vel.y = -speed;
		float angle = 0;
		if (World::GetInstance()->WorldScene.playerPtr) {
			angle = GetAngle(objectSprite.getPosition(), World::GetInstance()->WorldScene.playerPtr->objectSprite.getPosition());
		}
		RotateVector(vel, -angle);
		objectSprite.setRotation(-angle);
		objectSprite.move(vel.x,vel.y);

	}



	void HauzerSmog::Update() {

		if (World::GetInstance()->Timer(*this, VERY_SLOW))
		{

			objectSprite.setTextureRect(sf::IntRect((frame * 17), sprite, 17, 17));

			frame++;

			if (frame >= 4)
			{

				misDestroyed = true;

			}

		}

		if (World::GetInstance()->Timer(*this,VERY_SLOW)) {

			vel.x -= (vel.x * 0.25);
			vel.y -= (vel.y * 0.25);
		}

		objectSprite.move(vel.x, vel.y);

	}
    
	void PlayerBoomerang::Update() {

		if (World::GetInstance()->Timer(*this, VERY_FAST))
		{

			objectSprite.setTextureRect(sf::IntRect((frame * 20), 32, 20, 20));
			objectSprite.setOrigin(10, 10);

			frame++;

			if (frame >= 2)
			{

				frame = 0;

			}

		}

		objectSprite.setRotation(objectSprite.getRotation() + 20);
		sf::Vector2f newPos(objectSprite.getPosition().x, objectSprite.getPosition().y);

		/*
		if (World::GetInstance()->WorldScene.playerPtr) {

		newPos.x += (World::GetInstance()->WorldScene.playerPtr->objectSprite.getPosition().x - objectSprite.getPosition().x) / timeCurve;
		newPos.y += (World::GetInstance()->WorldScene.playerPtr->objectSprite.getPosition().y - objectSprite.getPosition().y) / timeCurve;

		}
		*/

		/*
        if(World::GetInstance()->Timer(*this,FAST)) if(timeCurve > 10) timeCurve -= 4;
    
        objectSprite.setPosition(newPos.x + vel.x,newPos.y + vel.y);
        

        if(World::GetInstance()->Timer(*this,NORMAL))
        {
            vel.x *= 0.5;
            vel.y *= 0.5;
        }
		*/
		
		objectSprite.move(vel.x, vel.y);
        
        if(World::GetInstance()->Timer(*this,VERY_SLOW*10)) misDestroyed = true;
        
        //if(World::GetInstance()->Timer(*this,FAST)) CreateClone(objectSprite);
        
    }

	void PlayerBoomerang::HasCollided(const std::unique_ptr<Entity::Object>& a) {

		objectSprite.move(-vel.x,-vel.y);

		Projectile::HasCollided(a);

	}
   

    
    void PlayerBombExp::Update(){

        misDestroyed = true;
    }


	void PlayerBeam1::Update() {

		//reset our beam length every update
		nodeCollided = length;


		if (parent) {

			objectSprite.setPosition(parent->objectSprite.getPosition());

		}

		//We can't create our nodes in the constructor because that changes the size of the ItemQueue iterator (bad access)

		if (nodesCreated == false) {

			itemQueue node;
			node.properties["PosX"] = std::to_string(objectHitBox.getPosition().x);
			node.properties["PosY"] = std::to_string(objectHitBox.getPosition().y);
			node.properties["itemType"] = "PlayerBeamNode";
			node.parent = this;
			//type is not nil here
			
			for (int i = 0; i != length; i++){
			
				// just pass the nod Slot here, otherwise will have to use a dynamic cast in Container::AddObject() and I hate that
				node.properties["NodeSlot"] = std::to_string(i);
				World::GetInstance()->WorldScene.objectContainer->Queue.push_back(node);

			}

			nodesCreated = true;

		}

		if (World::GetInstance()->Timer(*this, 20302.0)) {
			objectSprite.setScale(sf::Vector2f(objectSprite.getScale().x*0.7, objectSprite.getScale().y));
		}

		if (World::GetInstance()->Timer(*this, VERY_FAST))
		{

			objectSprite.setTextureRect(sf::IntRect(frame * objectSprite.getTextureRect().width,
				objectSprite.getTextureRect().top,
				objectSprite.getTextureRect().width,
				objectSprite.getTextureRect().height));
			frame++;

			if (frame > maxFrame)
			{
				frame = 0;

			}

		}

		if (objectSprite.getScale().x < 0.02 || !parent || parent->type == "") misDestroyed = true;

	}

	void PlayerBeamNode::Update() {

		vel.y = 16;
		vel.x = 0;
		RotateVector(vel, 45 * World::GetInstance()->WorldScene.playerPtr->fireDir);
		objectSprite.setPosition(beamParent->objectHitBox.getPosition().x + (vel.x * nodeSlot), beamParent->objectHitBox.getPosition().y + (vel.y * nodeSlot));

		Projectile::Update();

		if (!beamParent || beamParent->type == "") misDestroyed = true;

	}

	void PlayerHoming1::Update() {

		Projectile::Update();
			//cursor position

			if (Enemy::closestEnemy && World::GetInstance()->WorldScene.playerPtr) {

				if (GetDistance(World::GetInstance()->WorldScene.playerPtr->objectHitBox.getPosition(), Enemy::closestEnemy->objectHitBox.getPosition()) <= 150) {

					isAttacking = true;
					cursorSpeed = 5;

				}

				else {

					isAttacking = false;
					cursorSpeed = 20;
				}

			}

			else {

				misDestroyed = true;
				
			}

			//if the closest enemy is within range, then attack it

			if (isAttacking == true) {

				objectSprite.setTextureRect(sf::IntRect(0, 435, 20, 20));
				targetPosition = Enemy::closestEnemy->objectHitBox.getPosition();
				float oldx = objectSprite.getPosition().x;
				float oldy = objectSprite.getPosition().y;
				oldx += ((targetPosition.x - oldx) / cursorSpeed);
				oldy += ((targetPosition.y - oldy) / cursorSpeed);
				objectSprite.setPosition(oldx, oldy);

				//particle position

				midPosition.x = RandomNumber(20);
				midPosition.y = RandomNumber(20);

				float xa = GetPoint(World::GetInstance()->WorldScene.playerPtr->objectHitBox.getPosition().x, objectSprite.getPosition().x + midPosition.x, timeline);
				float ya = GetPoint(World::GetInstance()->WorldScene.playerPtr->objectHitBox.getPosition().y, objectSprite.getPosition().y + midPosition.y, timeline);
				float xb = GetPoint(objectSprite.getPosition().x + midPosition.x, objectSprite.getPosition().x, timeline);
				float yb = GetPoint(objectSprite.getPosition().y + midPosition.y, objectSprite.getPosition().y, timeline);

				float newX = GetPoint(xa, xb, timeline);
				float newY = GetPoint(ya, yb, timeline);
				linkSprite.setPosition(newX, newY);
				linkSprite.setTextureRect(sf::IntRect((RandomNumber(2) * 10), 455, 10, 16));
				timeline = RandomNumber(100) * 0.01;

				World::GetInstance()->WorldScene.audioContainer.PlaySFX("sfx_homer5", false);

			}

			//if there are no enemys within range, then don't

			else if (isAttacking == false) {

				objectSprite.setTextureRect(sf::IntRect(21, 435, 20, 20));
				World::GetInstance()->WorldScene.audioContainer.StopSFX("sfx_homer5");

				targetPosition = World::GetInstance()->WorldScene.playerPtr->objectHitBox.getPosition();
				float oldx = objectSprite.getPosition().x;
				float oldy = objectSprite.getPosition().y;
				oldx += ((targetPosition.x - oldx) / cursorSpeed);
				oldy += ((targetPosition.y - oldy) / cursorSpeed);
				objectSprite.setPosition(oldx, oldy);

			}

	
	}

	void PlayerDashBall::Update() {

		Projectile::Update();

		if (World::GetInstance()->WorldScene.playerPtr) {

			objectSprite.setPosition(World::GetInstance()->WorldScene.playerPtr->objectSprite.getPosition().x, World::GetInstance()->WorldScene.playerPtr->objectSprite.getPosition().y-10);
			objectSprite.setRotation(45 * World::GetInstance()->WorldScene.playerPtr->movement);

			if (World::GetInstance()->WorldScene.playerPtr->dashing == false) misDestroyed = true;

		}

		else misDestroyed = true;

		if (World::GetInstance()->Timer(*this, FAST)) {

			itemQueue clone;
			clone.properties["PosX"] = std::to_string((objectHitBox.getPosition().x-20) + RandomNumber(40));
			clone.properties["PosY"] = std::to_string((objectHitBox.getPosition().y-20) + RandomNumber(40));
			clone.properties["itemType"] = "PlayerSpark";
			clone.parent = this;
			World::GetInstance()->WorldScene.objectContainer->Queue.push_back(clone);

		}


	}
    
    void PlayerLaser1::isCollided(int var){
        
        misDestroyed = true;
        
    }

	void PlayerDashBall::isCollided(int var) {


	}
    
    void PlayerRepeater1::isCollided(int var){
        
        misDestroyed = true;
        
    }
    
    
    void Explosion::Update()
    {
        
        if(World::GetInstance()->Timer(*this,VERY_FAST))
        {
            
            objectSprite.setTextureRect(sf::IntRect(anim_frameX * 80,anim_frameY * 80, 80, 80));
            anim_frameY++;
            
            if(anim_frameY >= 3)
            {
                anim_frameX++;
                anim_frameY = 0;
            }
            
        }
        
        if(anim_frameX >= 5)
        {
            
            misDestroyed = true;
            
        }
        
        
    }
    
    
    void Cluster::Update()
    {
        
        if(World::GetInstance()->Timer(*this,10.0f) && maxBomb > 0)
        {

            itemQueue bomb;
            bomb.properties["PosX"] = std::to_string(objectSprite.getPosition().x + (rand() % 32 + -16));
            bomb.properties["PosY"] = std::to_string(objectSprite.getPosition().y + (rand() % 32 + -16));
            bomb.properties["itemType"] = "Explosion";
            World::GetInstance()->WorldScene.objectContainer->Queue.push_back(bomb);
            
            maxBomb--;
            
            if(maxBomb <= 0)
            {
                
                misDestroyed = true;
                
            }
            
            
        }
        
        
        
    }
    
    void Prop::Update(){
        
        objectSprite.setTextureRect(sf::IntRect(frame * objectSprite.getTextureRect().width,objectSprite.getTextureRect().top,objectSprite.getTextureRect().width,objectSprite.getTextureRect().height));
        
        if(maxFrame != 0){
        
			if (World::GetInstance()->Timer(*this, animSpd, NODELAY)) {

				if (frame == maxFrame){ 
					
					objectSprite.setTextureRect(sf::IntRect(0, objectSprite.getTextureRect().top, objectSprite.getTextureRect().width, objectSprite.getTextureRect().height));
					frame = 0;

				}

				else frame++;

			}
        
        }
    }
    
    void Laser::Update()
    {
        
        //float laserStartpnt = *playerPosX + 20 + (3 * length);
        
        // Since Scaling throws off origin, move laser by including sprite size and multiplying length x size of sprite "(6 * length)"
        
        
        if(World::GetInstance()->Timer(*this,30.f))
        {
            
            if(frame == 3) frame = 0;
           
            objectSprite.setTextureRect(sf::IntRect(frame * 6, 0, 6, 6));
            frame++;
            
        }
        
        if((*playerPosX + 20 + (3 * length)) + (3 * length) <= 399)
        {
            
            length = length * 1.3;
            float laserStartpnt = *playerPosX + 20 + (3 * length);
            objectSprite.setPosition(laserStartpnt,*playerPosY + 14);
            objectSprite.setScale((length),1);
            
        }
        
        
        
        if((*playerPosX + 20 + (3 * length)) + (3 * length) >= 400)
        {
        
            
            while((*playerPosX + 20 + (3 * length)) + (3 * length) > 400)
            {
                
                length = round(length);
                length--;
                
            }
            
            /// Creates explosion at end point
            
            if(height == 10)
            {
                
                World::GetInstance()->WorldScene.audioContainer.PlaySFX("sfx_big_exp");
                    

                
                for(int i = 0; i < 30; i++)
                {
                    
                    itemQueue particles;
                    particles.properties["PosX"] = std::to_string(*playerPosX + 20 + (3 * length) + (3 * length));
                    particles.properties["PosY"] = std::to_string(objectSprite.getPosition().y);
                    particles.properties["itemType"] = "Spark";
                    World::GetInstance()->WorldScene.objectContainer->Queue.push_back(particles);
                    
                }
                
                itemQueue bomb;
                bomb.properties["PosX"] = std::to_string(*playerPosX + 20 + (3 * length) + (3 * length));
                bomb.properties["PosY"] = std::to_string(objectSprite.getPosition().y);
                bomb.properties["itemType"] = "Cluster";
                World::GetInstance()->WorldScene.objectContainer->Queue.push_back(bomb);
                std::this_thread::sleep_for(std::chrono::milliseconds(80));
                World::GetInstance()->ScreenShake(20);
                
            }
            
            height = height / 1.1;
            objectSprite.setScale((length),height);
            float laserStartpnt = *playerPosX + 20 + (3 * length);
            objectSprite.setPosition(laserStartpnt,*playerPosY + 14);
            
            if(height <= 0.1)
            {
                
                misDestroyed = true;
                
            }
            
        }
        
    }
    
    // Misc functions
    
    int Projectile::GetDamage(){
        
        return damage;
        
    }
    
    void HitNum::Update(){
        
        if(World::GetInstance()->Timer(*this,60)){num.move(0,-1); numbg.setPosition(num.getPosition().x-2,num.getPosition().y+7);}
        
        if(World::GetInstance()->Timer(*this,1000)) misDestroyed = true;
        
    }

	void Projectile::HasCollided(const std::unique_ptr<Entity::Object>& a) {

		//we call a dynamic cast to be able to call a specific function from the enemy class
		Entity::Object* obj = a.get();
		Entity::Enemy* pobj = dynamic_cast<Entity::Enemy*>(obj);


		//only if the enemy is in an active state (i.e., not currently in the middle of its spawn transition) should we confirm a collision 
		if (pobj->active == true) {

					//Call isCollided() to enemy to remove damage and respond to projectile
					a->isCollided(damage);

					//if this projectile should be destroyed if it collides with an enemy, destroy it
					misDestroyed = destroyOnImpact;

					itemQueue particles;
					particles.properties["PosX"] = std::to_string(objectSprite.getPosition().x);
					particles.properties["PosY"] = std::to_string(objectSprite.getPosition().y);
					particles.properties["itemType"] = "DamageSpark";
					World::GetInstance()->WorldScene.objectContainer->Queue.push_back(particles);


		}

	}

    
    // Draw functions
    
    
    void Object::Draw(sf::RenderTarget& window)
    {
        World::GetInstance()->DrawObject(objectSprite);
        //World::GetInstance()->DrawObject(objectHitBox);
    }
    
    void PlayerBeam::Draw(sf::RenderTarget& window){
        
        World::GetInstance()->DrawObject(shaft);
        
    }

	void PlayerBirth::Draw(sf::RenderTarget& window) {

		if(phase >= 1) World::GetInstance()->DrawObject(objectSprite,"bypass");

	}


	void EnemyLaser::Draw(sf::RenderTarget& window) {

		int lsrBdyFrame = laserBody.getTextureRect().left;
		int lsrHdFrame = laserHead.getTextureRect().left;

		if (World::GetInstance()->Timer(*this, VERY_FAST)) {
			 
			if (lsrBdyFrame != 80) lsrBdyFrame += 16;
			else lsrBdyFrame = 32;

			if (lsrHdFrame != 77) lsrHdFrame += 15;
			else lsrHdFrame = 32;

			laserHead.setTextureRect(sf::IntRect(lsrHdFrame, laserHead.getTextureRect().top, laserHead.getTextureRect().width, laserHead.getTextureRect().height));

		}

		//lsrBdyFrame = 48; 

		sf::Vector2f previousBodyPos = laserBody.getPosition();
		sf::IntRect BodyRect = sf::IntRect(lsrBdyFrame, laserBody.getTextureRect().top, laserBody.getTextureRect().width, laserBody.getTextureRect().height);
		int distance = GetDistance(laserBody.getPosition(), objectSprite.getPosition());
		int laserBodyLength = distance / 16;
		int lengthRemainder = distance % 16;

		World::GetInstance()->DrawObject(laserBody);

		for (int i = 0; i != laserBodyLength; i++) {

			laserBody.move(laserBodyvel);

			if (i == laserBodyLength-1) {

				laserBody.setTextureRect(sf::IntRect(BodyRect.left, BodyRect.top, lengthRemainder, BodyRect.height));
				World::GetInstance()->DrawObject(laserBody);

			}

			else World::GetInstance()->DrawObject(laserBody);



		}

		laserBody.setPosition(previousBodyPos);
		laserBody.setTextureRect(BodyRect);
		World::GetInstance()->DrawObject(laserHead);
		World::GetInstance()->DrawObject(objectSprite);

	}

	void BigEnemyLaser::Draw(sf::RenderTarget& window) {

		//laserHead.setPosition(objReference->objectSprite.getPosition().x, objReference->objectSprite.getPosition().y);
		
		int lsrBdyFrame = laserBody.getTextureRect().left;
		int lsrHdFrame = laserHead.getTextureRect().left;

		if (World::GetInstance()->Timer(*this, VERY_FAST)) {

			//laserBody.getTextureRect().left + laserBody.getTextureRect().width * 3

			if (lsrBdyFrame != 80) lsrBdyFrame += 16;
			else lsrBdyFrame = 32;

			if (lsrHdFrame != 219) lsrHdFrame += 41;
			else lsrHdFrame = 96;

			laserHead.setTextureRect(sf::IntRect(lsrHdFrame, laserHead.getTextureRect().top, laserHead.getTextureRect().width, laserHead.getTextureRect().height));

		}

		//lsrBdyFrame = 48; 

		sf::Vector2f previousBodyPos = laserBody.getPosition();
		sf::IntRect BodyRect = sf::IntRect(lsrBdyFrame, laserBody.getTextureRect().top, laserBody.getTextureRect().width, laserBody.getTextureRect().height);
		int distance = GetDistance(laserBody.getPosition(), objectSprite.getPosition());
		int laserBodyLength = distance / 16;
		int lengthRemainder = distance % 16;

		World::GetInstance()->DrawObject(laserBody);

		for (int i = 0; i != laserBodyLength; i++) {

			laserBody.move(laserBodyvel);

			if (i == laserBodyLength - 1) {

				laserBody.setTextureRect(sf::IntRect(BodyRect.left, BodyRect.top, lengthRemainder, BodyRect.height));
				World::GetInstance()->DrawObject(laserBody);

			}

			else World::GetInstance()->DrawObject(laserBody);



		}

		laserBody.setPosition(previousBodyPos);
		laserBody.setTextureRect(BodyRect);
		World::GetInstance()->DrawObject(laserHead);
		World::GetInstance()->DrawObject(objectSprite);

	}
    
    void Object::DrawShadow(sf::RenderTarget& window)
    {
		//World::GetInstance()->DrawObject(objectHitBox);

		if (type == "Enemy") {

			if(active == true) World::GetInstance()->DrawObject(objectShadow);
		}

		else World::GetInstance()->DrawObject(objectShadow);
        
    }

	void PlayerSpawn::Draw(sf::RenderTarget& window)
	{

		World::GetInstance()->DrawObject(objectSprite,"bypass");

	}

	void PlayerEgg::Draw(sf::RenderTarget& window)
	{

		World::GetInstance()->DrawObject(objectSprite, "bypass");

	}
    
    void HitNum::Draw(sf::RenderTarget& window)
    {
        
        World::GetInstance()->DrawObject(numbg);
        World::GetInstance()->DrawObject(num);
        
        
    }


    
    // Desctructors
    
    Object::~Object()
    {
        
        
    }
    
    Projectile::~Projectile()
    {
        
        
    }
    
    EnemyProjectile::~EnemyProjectile()
    {
        
    }
    
    EnemyBlip::~EnemyBlip(){
        
    }

	EnemyMine::~EnemyMine() {

	}

	QuakeDirt::~QuakeDirt() {

	}

	EnemyLaser::~EnemyLaser() {

	}

	EnemyChargeParticle::~EnemyChargeParticle() {

	}

	PlayerChargeParticle::~PlayerChargeParticle() {

	}

	PlayerChargeParticleSmall::~PlayerChargeParticleSmall() {

	}

	ChargeParticleTiny::~ChargeParticleTiny() {

	}

	ChargeWaveAttack::~ChargeWaveAttack() {

		if(projectileNode) projectileNode->misDestroyed = true;
		if(projectileNode2) projectileNode2->misDestroyed = true;
		if(projectileNode3) projectileNode3->misDestroyed = true;

	}

	BigEnemyLaser::~BigEnemyLaser() {

	}

	EnemyHomingBlip::~EnemyHomingBlip() {

	}

	PlayerBirth::~PlayerBirth() {


	}

	EnemyCharge::~EnemyCharge() {

		itemQueue bullet;
		int dx = World::GetInstance()->WorldScene.playerPtr->objectSprite.getPosition().x - objectSprite.getPosition().x;
		int dy = World::GetInstance()->WorldScene.playerPtr->objectSprite.getPosition().y - objectSprite.getPosition().y;
		int angle = atan2(dx, dy);

		bullet.properties["PosX"] = std::to_string(objectSprite.getPosition().x);
		bullet.properties["PosY"] = std::to_string(objectSprite.getPosition().y - 6);
		bullet.properties["itemType"] = "EnemyBlip";
		bullet.properties["Speed"] = std::to_string(1);


		int fireDir = 0;
		if (World::GetInstance()->WorldScene.playerPtr) fireDir = GetAngle(objectSprite.getPosition(), World::GetInstance()->WorldScene.playerPtr->objectSprite.getPosition());
		bullet.properties["Direction"] = std::to_string(fireDir);
		World::GetInstance()->WorldScene.objectContainer->Queue.push_back(bullet);
		bullet.properties["Direction"] = std::to_string(fireDir - 10);
		World::GetInstance()->WorldScene.objectContainer->Queue.push_back(bullet);
		bullet.properties["Direction"] = std::to_string(fireDir + 10);
		World::GetInstance()->WorldScene.objectContainer->Queue.push_back(bullet);
		bullet.properties["Direction"] = std::to_string(fireDir - 20);
		World::GetInstance()->WorldScene.objectContainer->Queue.push_back(bullet);
		bullet.properties["Direction"] = std::to_string(fireDir + 20);
		World::GetInstance()->WorldScene.objectContainer->Queue.push_back(bullet);

	}

	StarSpawn::~StarSpawn() {

		itemQueue star;
		star.properties["PosX"] = std::to_string(objectSprite.getPosition().x);
		star.properties["PosY"] = std::to_string(objectSprite.getPosition().y - 6);
		star.properties["itemType"] = "Eball";
		World::GetInstance()->WorldScene.objectContainer->Queue.push_back(star);

	}

	BounceCrawler::~BounceCrawler() {

	}

	HauzerSmog::~HauzerSmog() {

	}

	HauzerSpear::~HauzerSpear() {

	}

	DashEffect::~DashEffect() {

	}

	SlideEffect::~SlideEffect() {

	}

	PlayerPoint::~PlayerPoint() {


	}

	ActionSpark::~ActionSpark() {


	}

	PlayerSpawn::~PlayerSpawn() {

		itemQueue egg;
		egg.properties["PosX"] = std::to_string(objectSprite.getPosition().x);
		egg.properties["PosY"] = std::to_string(objectSprite.getPosition().y);
		egg.properties["itemType"] = "PlayerEgg";
		World::GetInstance()->WorldScene.objectContainer->Queue.push_back(egg);
	}

	PlayerEgg::~PlayerEgg() {

		World::GetInstance()->CameraTarget->misDestroyed = true;
		World::GetInstance()->WorldScene.audioContainer.PlaySFX("sfx_twinkle");
		World::GetInstance()->WorldScene.textureContainer.glitchShader->opac = 1.0;
		World::GetInstance()->worldShader = 0;

		itemQueue charge2;
		charge2.properties["PosX"] = std::to_string(519);
		charge2.properties["PosY"] = std::to_string(808);
		charge2.properties["itemType"] = "ShieldEffect";
		World::GetInstance()->WorldScene.objectContainer->Queue.push_back(charge2);
		World::GetInstance()->CameraTarget = World::GetInstance()->WorldScene.playerPtr;
		World::GetInstance()->WorldScene.levelContainer->CreatePlayer(0, 519, 819);

	}

	BlockedEffect::~BlockedEffect() {

	}
    
	ShieldEffect::~ShieldEffect() {

	}
    
    HitNum::~HitNum()
    {
        
        
        
        
    }
    
    SpriteClone::~SpriteClone(){
        
        
    }

	LargeCharge::~LargeCharge() {

	}
    
    PlayerBomb::~PlayerBomb(){
        
        itemQueue particles;
        particles.properties["itemType"] = "DeathPoof";
        
        particles.properties["PosX"] = std::to_string(objectSprite.getPosition().x + RandomNumber(20));
        particles.properties["PosY"] = std::to_string(objectSprite.getPosition().y + RandomNumber(20));
        World::GetInstance()->WorldScene.objectContainer->Queue.push_back(particles);
        
        
        particles.properties["PosX"] = std::to_string(objectSprite.getPosition().x - RandomNumber(20));
        particles.properties["PosY"] = std::to_string(objectSprite.getPosition().y - RandomNumber(20));
        World::GetInstance()->WorldScene.objectContainer->Queue.push_back(particles);
        
        
        particles.properties["PosX"] = std::to_string(objectSprite.getPosition().x - RandomNumber(20));
        particles.properties["PosY"] = std::to_string(objectSprite.getPosition().y + RandomNumber(20));
        World::GetInstance()->WorldScene.objectContainer->Queue.push_back(particles);
        
        
        particles.properties["PosX"] = std::to_string(objectSprite.getPosition().x + RandomNumber(20));
        particles.properties["PosY"] = std::to_string(objectSprite.getPosition().y - RandomNumber(20));
        World::GetInstance()->WorldScene.objectContainer->Queue.push_back(particles);
        
        
        particles.properties["PosX"] = std::to_string(objectSprite.getPosition().x);
        particles.properties["PosY"] = std::to_string(objectSprite.getPosition().y);
        
        
        particles.properties["itemType"] = "PlayerBombExp";
        World::GetInstance()->WorldScene.objectContainer->Queue.push_back(particles);
        
        
        particles.properties["itemType"] = "BigExp";
        World::GetInstance()->WorldScene.objectContainer->Queue.push_back(particles);
        totalBombs = 0;
        
        World::GetInstance()->WorldScene.audioContainer.PlaySFX("sfx_bomb_exp");

        
    }
    
    PlayerBombExp::~PlayerBombExp(){
        
    }
    
    PlayerBeam::~PlayerBeam(){
        
    }
    
    PlayerBoomerang::~PlayerBoomerang(){
        
        itemQueue particles;
        particles.properties["PosX"] = std::to_string(objectSprite.getPosition().x);
        particles.properties["PosY"] = std::to_string(objectSprite.getPosition().y);
        particles.properties["itemType"] = "DeathPoof";
        
        
        World::GetInstance()->WorldScene.objectContainer->Queue.push_back(particles);
        World::GetInstance()->WorldScene.objectContainer->Queue.push_back(particles);
        World::GetInstance()->WorldScene.objectContainer->Queue.push_back(particles);
        totalBoomerangs--;
        
    }
    
    DeathPoof::~DeathPoof(){
        
    }

	BlockedWave::~BlockedWave() {

	}
    
    DeathBoom::~DeathBoom(){
        
    }
    
    BigExp::~BigExp(){
        
    }
    
    Hit::~Hit(){
       
        
    }
    
    Prop::~Prop(){
        
    }
    
    PropFlame::~PropFlame(){
        
    }
    
    Particle::~Particle()
    {
        
        
        
    }
    
    DoorDestroy::~DoorDestroy(){
        
        itemQueue particles;
        particles.properties["PosX"] = std::to_string(objectSprite.getPosition().x + objectSprite.getTextureRect().width/2);
        particles.properties["PosY"] = std::to_string(objectSprite.getPosition().y + objectSprite.getTextureRect().height/2);
        particles.properties["itemType"] = "DeathPoof";
        World::GetInstance()->WorldScene.objectContainer->Queue.push_back(particles);
        World::GetInstance()->WorldScene.objectContainer->Queue.push_back(particles);
        World::GetInstance()->WorldScene.objectContainer->Queue.push_back(particles);
        World::GetInstance()->ResetCamera();
        World::GetInstance()->WorldScene.audioContainer.PlaySFX("sfx_bomb_exp");
        
    }

	LearnedAbility::~LearnedAbility() {

		int thisWeapon = 1;

		World::GetInstance()->CameraTarget = World::GetInstance()->WorldScene.playerPtr;
		World::GetInstance()->GlobalMembers.weapons[thisWeapon] = 1;
		World::GetInstance()->WorldScene.hudPtr->Expand();
		World::GetInstance()->WorldScene.hudPtr->newAbility = thisWeapon;
		World::GetInstance()->worldShader = 0;

	}
    
    Fixed::~Fixed(){
        
        
    }

	HauzerCharge::~HauzerCharge() {

	}
    
    DamageSpark::~DamageSpark(){
        
        
    }
    
    Passive::~Passive(){
        
        
        
    }
    
    Dirt::~Dirt(){
        
    }
    
    Gib::~Gib(){
        
    }
    
    Explosion::~Explosion()
    {
        
        
        
    }
    
    Bullet::~Bullet()
    {
        
        for(int i = 0; i < 6; i++)
        {
            
            itemQueue particles;
            particles.properties["PosX"] = std::to_string(objectSprite.getPosition().x);
            particles.properties["PosY"] = std::to_string(objectSprite.getPosition().y);
            particles.properties["itemType"] = "Spark";
            World::GetInstance()->WorldScene.objectContainer->Queue.push_back(particles);
            
        }
        
        itemQueue bomb;
        bomb.properties["PosX"] = std::to_string(objectSprite.getPosition().x);
        bomb.properties["PosY"] = std::to_string(objectSprite.getPosition().y);
        bomb.properties["itemType"] = "Cluster";
        World::GetInstance()->WorldScene.objectContainer->Queue.push_back(bomb);
        
        World::GetInstance()->WorldScene.audioContainer.PlaySFX("sfx_bullet_exp");

        
        
    }
    
    Cluster::~Cluster()
    {
        
        
        
    }

	Electricity::~Electricity() {

	}
    
    Laser::~Laser()
    {
        
        playerPosX = NULL;
        playerPosY = NULL;
        delete playerPosX;
        delete playerPosY;
        
    }
    
    PlayerLaser1::~PlayerLaser1()
    {
        
    }

	PlayerShotgun1::~PlayerShotgun1() {


	}

	PlayerHoming1::~PlayerHoming1() {

		count = 0;
	}

	ElectricNode::~ElectricNode() {


	}

	PlayerDashBall::~PlayerDashBall()
	{

	}

	PlayerLaser2::~PlayerLaser2()
	{

	}

	PlayerBeam1::~PlayerBeam1()
	{

	}

	PlayerBeamNode::~PlayerBeamNode()
	{

	}

	PlayerLaser3::~PlayerLaser3()
	{

	}

	PlayerLaser4::~PlayerLaser4()
	{

	}

	PlayerLaser5::~PlayerLaser5()
	{

	}
    
    PlayerRepeater1::~PlayerRepeater1()
    {
        
    }

	PlayerRepeater2::~PlayerRepeater2()
	{

	}

	PlayerRepeater3::~PlayerRepeater3()
	{

	}


	PlayerRepeater6::~PlayerRepeater6()
	{

	}
    
    void isDestroyed(Entity::Enemy& object)
    {
        if(object.health <= 0){
            
            itemQueue bomb;
            bomb.properties["PosX"] = std::to_string(object.objectSprite.getPosition().x + 5);
            bomb.properties["PosY"] = std::to_string(object.objectSprite.getPosition().y + 5);
            bomb.properties["itemType"] = "BlockedEffect";
            World::GetInstance()->WorldScene.objectContainer->Queue.push_back(bomb);
            
            object.misDestroyed = true;
            
        }
        
    }
    
    void isDestroyed(Entity::Bullet& bullet){
        
        if(bullet.isCollided == true){
            
            for(int i = 0; i < 4; i++)
            {
                
                itemQueue particles;
                particles.properties["PosX"] = std::to_string(bullet.objectSprite.getPosition().x);
                particles.properties["PosY"] = std::to_string(bullet.objectSprite.getPosition().y);
                particles.properties["itemType"] = "Spark";
                World::GetInstance()->WorldScene.objectContainer->Queue.push_back(particles);
                
            }
            
            itemQueue bomb;
            bomb.properties["PosX"] = std::to_string(bullet.objectSprite.getPosition().x);
            bomb.properties["PosY"] = std::to_string(bullet.objectSprite.getPosition().y);
            bomb.properties["itemType"] = "Cluster";
            World::GetInstance()->WorldScene.objectContainer->Queue.push_back(bomb);
            World::GetInstance()->ScreenShake(10);
            World::GetInstance()->WorldScene.audioContainer.PlaySFX("sfx_bullet_exp");
            bullet.misDestroyed = true;
            
        }
        
    }
    
	bool Object::isDestroyed() {

		int borderLeft = World::GetInstance()->WorldScene.levelContainer->battleBorder.left;
		int borderTop = World::GetInstance()->WorldScene.levelContainer->battleBorder.top;
		int borderRight = borderLeft + World::GetInstance()->WorldScene.levelContainer->battleBorder.width;
		int borderBottom = borderTop + World::GetInstance()->WorldScene.levelContainer->battleBorder.height;

		if (World::GetInstance()->WorldScene.playerPtr && this == World::GetInstance()->WorldScene.playerPtr) {

			if (World::GetInstance()->CurrentScene->mapType == ENCOUNTER) {

				if ((objectHitBox.getPosition().x < borderLeft || objectHitBox.getPosition().x > borderRight || objectHitBox.getPosition().y < borderTop || objectHitBox.getPosition().y > borderBottom)) {

					Entity::Object* obj = this;
					Entity::BattlePlayer* pobj = dynamic_cast<Entity::BattlePlayer*>(obj);
					if (pobj->hyperDash == true)pobj->Bounce();
				}

			}

		}

		else if ((objectHitBox.getPosition().x < 0 || objectHitBox.getPosition().x > World::GetInstance()->WorldScene.levelContainer->lvlSize.x || objectHitBox.getPosition().y < 0 || objectHitBox.getPosition().y > World::GetInstance()->WorldScene.levelContainer->lvlSize.y) && deleteOnOutOfBounds == true && type != "GUI") misDestroyed = true;


		return misDestroyed;
        
    }
    
    /*
     ––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––
     Actor definitions – includes enemies
     ––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––
     */
    
    // Constructors
    
    int Enemy::totalEnemies = 0;
    std::array<std::string,26> LevelManager::enemyList;
	int Enemy::spriteSheetHeight = 424;
    
	LevelManager::LevelManager() : Object() {

		World::GetInstance()->WorldScene.levelContainer->levelManagerPtr = this;

		objectSprite.setTextureRect(sf::IntRect(0, 0, 0, 0));

		std::string level = std::to_string(World::GetInstance()->GlobalMembers.currentLevel);

		bgwall.setTexture(World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_battle_wall"));
		bgwall.setTextureRect(sf::IntRect(0, 0, 480, 270));

		if (World::GetInstance()->GlobalMembers.currentLevel >= 6) {

			bg.setTexture(World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_battle_bg_" + level + ""));
			bg.setTextureRect(sf::IntRect(0, 0, 2000, 2000));
			bg.setOrigin(2000 / 2, 2000 / 2);

		}

		if (enemyList[0] == "") {

			//bosses start at 0

			enemyList[0] = "Mozza";
			enemyList[1] = "Hauzer";
			enemyList[2] = "Lim";
			enemyList[3] = "Boss4";
			enemyList[4] = "Boss5";
			enemyList[5] = "Boss6";
			enemyList[6] = "Boss7";

			//enemies start at 7

			// level1 hauzer(dragon)
			enemyList[7] = "Djinn";
			enemyList[8] = "Squid";
			enemyList[9] = "Slime";

			//level2 gurian(knight)
			enemyList[10] = "Mask";
			enemyList[11] = "Spore";
			enemyList[12] = "Roach";

			//level3: doran(wolf)
			enemyList[13] = "";
			enemyList[14] = "";
			enemyList[15] = "Sprite";

			//level4: jira(devil)
			enemyList[16] = "Tower";
			enemyList[17] = "Family";
			enemyList[18] = "Bricky";

			//level5: lima(doll)
			enemyList[19] = "HorseKnight";
			enemyList[20] = "BabyDemon";
			enemyList[21] = "Tomb";

			//level6: Mozza(cat)
			enemyList[22] = "Mozza";
			enemyList[23] = "Mozza";
			enemyList[24] = "Mozza";

		}
        
        type = "BG";

		lvlEnemyBank[0] = 8;
		lvlEnemyBank[1] = 32;
		lvlEnemyBank[2] = 200;

		//lvlEnemyBank[0] = 0;
	    //lvlEnemyBank[1] = 0;
		//lvlEnemyBank[2] = 0;

    }

	void LevelManager::CreateEnemy(int enemy) {


		int borderLeft = World::GetInstance()->WorldScene.levelContainer->battleBorder.left;
		int borderTop = World::GetInstance()->WorldScene.levelContainer->battleBorder.top;
		int borderRight = borderLeft + World::GetInstance()->WorldScene.levelContainer->battleBorder.width;
		int borderBottom = borderTop + World::GetInstance()->WorldScene.levelContainer->battleBorder.height;

		std::random_device rd;
		std::mt19937 mt(rd());
		std::uniform_int_distribution<int> randomLeftwidth(borderLeft, borderRight);
		std::uniform_int_distribution<int> randomTopheight(borderTop, borderBottom);

		Entity::itemQueue ienemy;
		ienemy.properties["itemType"] = enemyList[7 + (3 * (World::GetInstance()->GlobalMembers.currentLevel - 6)) + enemy];
		ienemy.properties["PosX"] = std::to_string(randomLeftwidth(mt));
		ienemy.properties["PosY"] = std::to_string(randomTopheight(mt));
		World::GetInstance()->WorldScene.objectContainer->Queue.push_back(ienemy);
		lvlEnemyBank[enemy]--;

	}

    void LevelManager::Update(){
       
		//if this is a boss level

		if (World::GetInstance()->GlobalMembers.currentLevel < 6) {

			if (bossIsCreated == false) {

				Entity::itemQueue boss;
				boss.properties["PosX"] = std::to_string(World::GetInstance()->WorldScene.levelContainer->lvlSize.x);
				boss.properties["PosY"] = std::to_string(World::GetInstance()->WorldScene.levelContainer->lvlSize.y);
				boss.properties["itemType"] = enemyList[World::GetInstance()->GlobalMembers.currentLevel];
				World::GetInstance()->WorldScene.objectContainer->Queue.push_back(boss);
				World::GetInstance()->WorldScene.audioContainer.PlayMusic("mus_boss_fm");
				bossIsCreated = true;

			}

		}

		//if it is not
		
		else {

			if (lvlEnemyBank[0] != 0 || lvlEnemyBank[1] != 0 || lvlEnemyBank[2] != 0) {

				// need to figure out the correct intervals to spawn enemies
				// big enemies should take a while to respond
				// mid and low enemies should be intervals of big enemies.
				// we should figure out how to end up with a big enemy spawned before all the smaller enemies 
				// (i.e., smaller enemies should still be spawning after LAST big enemy is spawned

				if (lvlEnemyBank[0] != 0 && World::GetInstance()->Timer(*this, 20000000.0, NODELAY)) CreateEnemy(0);
				if (lvlEnemyBank[1] != 0 && World::GetInstance()->Timer(*this, 5000000.0, NODELAY)) CreateEnemy(1);
				if (lvlEnemyBank[2] != 0 && World::GetInstance()->Timer(*this, 600000.0, NODELAY)) CreateEnemy(2);

			}
		}

		bg.rotate(0.05);


    }
    
    void LevelManager::Draw(sf::RenderTarget &window){
        
	
		if (bossIsCreated == false) { 

			World::GetInstance()->DrawObject(bg, "waveShader");

		}

		float transparency = (50 + cos(World::GetInstance()->clock2.getElapsedTime().asMilliseconds()*0.0015) * 50) + 90;
		bgwall.setColor(sf::Color::Color(255, 255, 255, transparency + RandomNumber(60)));

		if (bgwallHit == true) {

			World::GetInstance()->DrawObject(bgwall, "whiteShader");
			bgwallHit = false;

		}

		else World::GetInstance()->DrawObject(bgwall);
	
		
    }
    
    LevelManager::~LevelManager(){
        
    }
    
    Actor::Actor(int actor,int script) : characterName(actor), Object()
    {
        
		objectSprite.setTexture(World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_actors"));
		objectSprite.setTextureRect(sf::IntRect((1 + actor) * 16, 0 * 24, 16, 24));

		characterName = actor;

		//if we have not forced specific dialogue, choose the script set for this character

		if (script == 99) {  

			scriptNumber = World::GetInstance()->GlobalMembers.currentCharacterScripts[characterName]; 

		}

		else scriptNumber = script;

        SetCharacterOrigin();
        SetShadow();
        type = "Actor";
        SetHitBox(sf::Vector2f(50,50),0);
        
    }

	SoulOrb::SoulOrb(int actor,int script) : Actor(characterName,script)
	{

		objectSprite.setTextureRect(sf::IntRect(16, 24, 96, 0));
		characterName = 5;

	}
    
    void Actor::Update(){
        
        Move();
        UpdateShadow();
    }

	void Actor::Draw(sf::RenderTarget& window) {

		if(characterName != 6) World::GetInstance()->DrawObject(objectSprite);

	}

	void Actor::DrawShadow(sf::RenderTarget& window) {

		if (characterName != 6) World::GetInstance()->DrawObject(objectShadow);

	}


	void SoulOrb::Update() {

		Actor::Update();

		if (World::GetInstance()->Timer(*this, NORMAL)) {

			itemQueue effect;
			effect.properties["PosX"] = std::to_string(objectSprite.getPosition().x);
			effect.properties["PosY"] = std::to_string(objectSprite.getPosition().y-4);
			effect.properties["itemType"] = "ChargeParticleTiny";
			World::GetInstance()->WorldScene.objectContainer->Queue.push_back(effect);

		}

		if (World::GetInstance()->Timer(*this, FAST,NODELAY)) {

			World::GetInstance()->WorldScene.audioContainer.PlaySFX("sfx_longring",false);

		}

	}

	void SoulOrb::Draw(sf::RenderTarget& window) {

		World::GetInstance()->DrawObject(objectSprite,"chargeShader");

	}

    
    void Actor::Move()
    {
        if(PlayerDistance(CLOSE)){
            actorMovement = RoundUp((GetAngle(objectShadow.getPosition(),World::GetInstance()->WorldScene.playerPtr->objectShadow.getPosition())),45);
        objectSprite.setTextureRect(sf::IntRect((1+characterName) * 16, abs(((180 - actorMovement) / 45) * 24),16, 24));
        }
        
        else if(idleBehavior == none) objectSprite.setTextureRect(sf::IntRect((1+ characterName) * 16, south,16, 24));
        
        else if(idleBehavior == looking){
            
            if(World::GetInstance()->Timer(*this,VERY_SLOW*10,DELAY)) objectSprite.setTextureRect(sf::IntRect((1+ characterName) * 16, RandomNumber(7)*24,16, 24));
        
            }
        
    }
    
    void Actor::isCollided(int var)
	{

            if(Textbox::textboxCount == 0){
                
                if(World::GetInstance()->Timer(*this,NORMAL)){

                    itemQueue textbox;
                    textbox.properties["itemType"] = "Textbox";
                    textbox.properties["ActorName&Script"] = std::to_string(characterName) + std::to_string(scriptNumber);
                    World::GetInstance()->WorldScene.objectContainer->Queue.push_back(textbox);
                    World::GetInstance()->SetCameraTarget(*this);
                    
                }
            }
           
    }

	void SoulOrb::isCollided(int var) 
	{

		if (Textbox::textboxCount == 0) {

			if (World::GetInstance()->Timer(*this, NORMAL)) {

				itemQueue textbox;
				textbox.properties["itemType"] = "Textbox";
				textbox.properties["ActorName&Script"] = std::to_string(66);


				World::GetInstance()->WorldScene.objectContainer->Queue.push_back(textbox);
				World::GetInstance()->SetCameraTarget(*this);

			}
		}

	}
    
    Enemy::Enemy() : Object()
    {
        enemyID = totalEnemies;
        ++totalEnemies;
        objectSprite.setTexture(World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_enemies"));
        type = "Enemy";
        scaleTemp = sf::Vector2f(0.12,0);
        objectSprite.setColor(sf::Color(255,255,255,0));
        hurtPos.x = 0;
        hurtPos.y = 0;
        SetHitBox(sf::Vector2f(24,24),1);
		hotSpot.x = 0;
		hotSpot.y = 0;
		AssignedAttack = 0;

    }

	void Enemy::SetSpriteRedForHealth() {

		double percent = 0;

		if(health <= 0) percent = maxHealth / 1;
		else percent = maxHealth / health;
		double red = 255 / percent;
		objectSprite.setColor(sf::Color(255, red, red, 255));

	}
    
    EnemyNode::EnemyNode() : Enemy()
    {
        objectSprite.setTextureRect(sf::IntRect (0,0,16,16));
        SetShadow();
        type = "EnemyNode";
    }
    
    Slime::Slime() : Enemy()
    {
        objectSprite.setTextureRect(sf::IntRect (0,0,24,24));
        SetCharacterOrigin();
        SetShadow();
        health = 5;
		enemyMode = 1;
		AssignedBehavior = &Enemy::FollowPlayer;
		AssignedMovement = &Enemy::MoveJump;
		maxHealth = health;

    }

	Sprite::Sprite() : Enemy() {

		objectSprite.setTextureRect(sf::IntRect(92, 107, 18, 28));
		SetCharacterOrigin();
		SetShadow();
		flatAnimation = true;
		speed = 0.5;
		health = 15;
		enemyMode = 1;
		AssignedBehavior = &Enemy::FollowPlayer;
		AssignedMovement = &Enemy::MoveCircle;
		AssignedAttack = &Enemy::Shoot;
		maxHealth = health;

	}

	Tomb::Tomb() : Enemy()
	{
		objectSprite.setTextureRect(sf::IntRect(93, 147, 29, 39));
		SetCharacterOrigin();
		SetShadow();
		health = 5;
		enemyMode = 1;
		speed = 0.5;
		AssignedBehavior = &Enemy::FollowPlayer;
		AssignedMovement = &Enemy::MoveJump;
		AssignedAttack = &Enemy::Shoot;
		maxHealth = health;

	}

	Bricky::Bricky() : Enemy()
	{
		objectSprite.setTextureRect(sf::IntRect(87, 212, 21, 28));
		SetCharacterOrigin();
		SetShadow();
		health = 10;
		enemyMode = 1;
		AssignedBehavior = &Enemy::FollowPlayer;
		AssignedMovement = &Enemy::MoveWalk;
		maxHealth = health;
		speed = 1;

	}

	Raider::Raider() : Enemy()
	{
		objectSprite.setTextureRect(sf::IntRect(0, 144, 76, 67));
		SetCharacterOrigin();
		SetShadow();
		health = 30;
		enemyMode = 1;
		animationSpeed = VERY_FAST;
		AssignedBehavior = &Enemy::FollowPlayer;
		AssignedMovement = &Enemy::MoveWalk;
		maxHealth = health;
		speed = 2;
		shakey = 1;

	}

	Spore::Spore() : Enemy()
	{
		objectSprite.setTextureRect(sf::IntRect(78, 3, 16, 17));
		SetHitBox(sf::Vector2f(10, 10), 1);
		SetCharacterOrigin();
		SetShadow();
		health = 60;
		AssignedBehavior = &Enemy::Sticky;
		AssignedMovement = &Enemy::MoveWalk;
		AssignedAttack = &Enemy::SporeShoot;
		moveOnAttack = true;
		flatAnimation = true;
		alwaysTargetPlayer = false;
		speed = 1.5;
		maxHealth = health;
		create = "Spore";
		vel.x = 0;
		vel.y = speed;
		int rand = RandomNumber(3);
		RotateVector(vel,rand * 90);
		if (rand == 0) fireDir = 0;
		else if (rand == 1) fireDir = 270;
		else if (rand == 2) fireDir = 180;
		else if (rand == 3) fireDir = 90;
		std::cout << fireDir << std::endl;

	}

	Buddy::Buddy() : Object()
	{
		//std::cout << "buddy created" << std::endl;
		objectSprite.setTexture(World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_misc"));
		objectSprite.setTextureRect(sf::IntRect(204, 0, 4, 8));
		SetHitBox(sf::Vector2f(5, 5));
		SetCharacterOrigin();
		SetShadow();

	}


	Roach::Roach() : Enemy()
	{
		objectSprite.setTextureRect(sf::IntRect(0, 117, 37, 16));
		SetHitBox(sf::Vector2f(38, 17), 1);
		SetCharacterOrigin();
		SetShadow();
		health = 10;
		AssignedBehavior = &Enemy::FollowPlayer;
		AssignedMovement = &Enemy::MoveSlide;
		animationSpeed = VERY_FAST;
		maxHealth = health;


		// For roaches, speed is the DISTANCE the enemy will move

		speed = 15;

	}

	Tower::Tower() : Enemy() {

		objectSprite.setTextureRect(sf::IntRect(211, 144, 48, 122));
		SetCharacterOrigin();
		SetShadow();
		health = 280;
		flatAnimation = true;
		animationSpeed = VERY_SLOW;
		if(RandomNumber(1,0) == 0) AssignedBehavior = &Enemy::RandomPosition;
		else AssignedBehavior = &Enemy::FollowPlayer;
		AssignedMovement = &Enemy::MoveWalk;
		AssignedAttack = &Enemy::CircleShoot;
		SetHitBox(sf::Vector2f(20, 110),0);
		maxHealth = health;
		speed = 0.5;
		bigDaddy = true;

	}

	Djinn::Djinn() : Enemy()
	{

		objectSprite.setTextureRect(sf::IntRect(100, 0, 140, 112));
		SetCharacterOrigin();
		SetShadow();
		health = 180;
		flatAnimation = true;
		animationSpeed = VERY_SLOW;
		AssignedBehavior = &Enemy::RandomPosition;
		AssignedMovement = &Enemy::MoveWalk;
		AssignedAttack = &Enemy::DoubleCast;
		SetHitBox(sf::Vector2f(70, 70), 0);
		maxHealth = health;
		bigDaddy = true;

		//Set child sprite(wings)
		wings.setTexture(World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_enemies"));
		wings.setTextureRect(sf::IntRect(243, 0, 134, 44));

		// For roaches, speed is the DISTANCE the enemy will move

		speed = 0.25;

	}

	HorseKnight::HorseKnight() : Enemy()
	{

		objectSprite.setTextureRect(sf::IntRect(275, 127, 78, 131));
		SetCharacterOrigin();
		SetShadow();
		health = 180;
		flatAnimation = true;
		animationSpeed = VERY_SLOW;
		AssignedBehavior = &Enemy::FollowPlayer;
		AssignedMovement = &Enemy::MoveSlide;
		AssignedAttack = &Enemy::AroundShoot;
		SetHitBox(sf::Vector2f(70, 70), 0);
		maxHealth = health;
		bigDaddy = true;
		alwaysTargetPlayer = false;

		speed = 10;

	}
    
    Star::Star() : Enemy()
    {
        objectSprite.setTextureRect(sf::IntRect (0,24,35,35));
        SetCharacterOrigin();
        SetShadow();
		speed = 1.5; 
        health = 10;
		flatAnimation = true;
		AssignedBehavior = &Enemy::LazyFollowPlayer;
		AssignedMovement = &Enemy::MoveWalk;
		animationSpeed = 1000.0;
		transition = false;
		maxHealth = health;

    }

	Eball::Eball() : Enemy()
	{
		objectSprite.setTextureRect(sf::IntRect(45, 62, 41, 41));
		SetCharacterOrigin();
		SetShadow();
		speed = 0.5;
		health = 10;
		flatAnimation = true;
		AssignedBehavior = &Enemy::LazyFollowPlayer;
		AssignedMovement = &Enemy::MoveWalk;
		animationSpeed = 2000.0;
		transition = false;
		maxHealth = health;


	}

	Family::Family() : Enemy()
	{
		objectSprite.setTextureRect(sf::IntRect(46, 337, 21, 16));
		SetCharacterOrigin();
		SetShadow();
		speed = 1;
		health = 7;
		animationSpeed = FAST;
		AssignedBehavior = &Enemy::FollowPlayer;
		AssignedMovement = &Enemy::MoveWalk;
		AssignedAttack = &Enemy::SelfDestruct;
		SetHitBox(sf::Vector2f(16, 16), 1);
		maxHealth = health;

	}

	Mask::Mask() : Enemy()
	{
		objectSprite.setTextureRect(sf::IntRect(36, 13, 37, 45));
		SetCharacterOrigin();
		SetShadow();
		speed = 0.5;
		health = 60;
		flatAnimation = true;
		animationSpeed = VERY_SLOW*2;
		AssignedBehavior = &Enemy::FollowPlayer;
		AssignedMovement = &Enemy::MoveWalk;
		AssignedAttack = &Enemy::Laser;
		SetHitBox(sf::Vector2f(32, 42), 1);
		maxHealth = health;

	}

	BabyDemon::BabyDemon() : Enemy()
	{
		objectSprite.setTextureRect(sf::IntRect(22, 230, 39, 64));
		SetCharacterOrigin();
		SetShadow();
		speed = 1;
		health = 60;
		moveOnAttack = true;
		animationSpeed = VERY_SLOW;
		AssignedBehavior = &Enemy::Bounce;
		AssignedMovement = &Enemy::MoveWalk;
		AssignedAttack = &Enemy::LayFire;
		SetHitBox(sf::Vector2f(32, 42), 1);
		maxHealth = health;
		vel.x = 0;
		vel.y = speed;
		RotateVector(vel, 45);
		targetPosition = sf::Vector2f(objectSprite.getPosition().x + vel.x * 50, objectSprite.getPosition().y + vel.y * 50);
		currentDirection = 25;

	}
    
    Squid::Squid() : Enemy()
    {
        objectSprite.setTextureRect(sf::IntRect (0,60,43,46));
        SetCharacterOrigin();
        SetShadow();
        speed = 0.5;
        health = 60;
		flatAnimation = false;
		AssignedBehavior = &Enemy::FollowPlayer;
		AssignedMovement = &Enemy::MoveWalk;
		AssignedAttack = &Enemy::CastShoot;
        SetHitBox(sf::Vector2f(24,43),1);
		flatAnimation = false;
		enemyMode = 1;
		hotSpot.x = 20;
		maxHealth = health;

		hotSpot.y = 30;
    
    }

	HauzerSpire::HauzerSpire() : Enemy()
	{
		objectSprite.setTextureRect(sf::IntRect(0, 74, 26, 47));
		SetCharacterOrigin();
		SetShadow();
		moveType = 0;
		speed = 0;
		health = 70;
		SetHitBox(sf::Vector2f(30, 40), 1);
		transition = false;
		maxHealth = health;



	}
    
    Boss::Boss()
    {
        //std::cout << "Boss created" << std::endl;
		objectSprite.setTexture(World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_bosses"));
		bigDaddy = true;

		// Create movesets

		boost::function<void(Boss*)> f;
		boost::function<void(Boss*)> f2;
		boost::function<void(Boss*)> f3;
		boost::function<void(Boss*)> f4;

		f = &Boss::Rest;
		f2 = &Boss::Behavior1;
		f3 = &Boss::Behavior3;
		f4 = &Boss::Behavior4;

		BehaviorList.push_back(f);
		BehaviorList.push_back(f2);
		BehaviorList.push_back(f3);
		BehaviorList.push_back(f4);

    }
    
    Mozza::Mozza(){
        
        objectSprite.setTextureRect(sf::IntRect (0,105,175,175));
		wings.setTexture(World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_bosses"));
		wings.setTextureRect(sf::IntRect(0,0, 175, 44));
        speed = 2;
		health = 2400;
		maxHealth = health;
        moveType = NORMAL;
		sf::Sprite *mysprite = &wings;
        SetCharacterOrigin(&mysprite,true);
        SetHitBox(sf::Vector2f(70,70));
        SetShadow();
        hasAttack = 0;
		speed = 1;
        alwaysTargetPlayer = false;
		moveOnAttack = true;
		defending = true;
    }
    
	void Enemy::MoveElse() {

	}
    
    // Update Functions

	//Behavior functions

	void Enemy::FollowPlayer() {

		vel.x = 0;
		vel.y = -speed;

		if(AssignedMovement != &Enemy::MoveSlide) targetPosition = World::GetInstance()->WorldScene.playerPtr->objectSprite.getPosition();
		RotateVector(vel, -(GetAngle(objectSprite.getPosition(), targetPosition + varyMov)));

		OffsetPosition();

	}

	void Enemy::LazyFollowPlayer() {

		targetPosition = World::GetInstance()->WorldScene.playerPtr->objectSprite.getPosition();

		if (World::GetInstance()->Timer(*this, VERY_FAST, NODELAY)) {

			//set slow move to player

			if (objectSprite.getPosition().x < targetPosition.x + varyMov.x && vel.x < speed) vel.x += 0.15;
			else if (objectSprite.getPosition().x > targetPosition.x + varyMov.x && vel.x > -speed) vel.x -= 0.15;
			if (objectSprite.getPosition().y < targetPosition.y + varyMov.y && vel.y < speed) vel.y += 0.15;
			else if (objectSprite.getPosition().y > targetPosition.y + varyMov.y && vel.y > -speed) vel.y -= 0.15;


		}

		OffsetPosition();

	}

	void Enemy::RandomPosition() {

		vel.x = 0;
		vel.y = -speed;

		if (shakey == 0) {
			if (World::GetInstance()->Timer(*this, 800000.0, NODELAY) && velZ == 0) {

				targetPosition.x = RandomNumber(600);
				targetPosition.y = RandomNumber(600);
			}
		}

		else {

			if (World::GetInstance()->Timer(*this, VERY_SLOW*5, NODELAY) && velZ == 0) {

				targetPosition.x = RandomNumber(600);
				targetPosition.y = RandomNumber(600);
			}

		}

		RotateVector(vel, -(GetAngle(objectSprite.getPosition(), targetPosition)));

	}

	// Move functions

	void Enemy::MoveWalk() {

		if (hasAttack == 0) objectSprite.move(vel.x, vel.y + velZ);

		else if (moveOnAttack == false) {

			if (attacking == false) objectSprite.move(vel.x, vel.y + velZ);

		}

		else objectSprite.move(vel.x, vel.y + velZ);

	}

	void Enemy::Sticky() {

		int borderLeft = World::GetInstance()->WorldScene.levelContainer->battleBorder.left;
		int borderTop = World::GetInstance()->WorldScene.levelContainer->battleBorder.top;
		int borderRight = borderLeft + World::GetInstance()->WorldScene.levelContainer->battleBorder.width;
		int borderBottom = borderTop + World::GetInstance()->WorldScene.levelContainer->battleBorder.height;

		if (objectHitBox.getPosition().x >= borderRight
			|| objectHitBox.getPosition().x <= borderLeft
			|| objectHitBox.getPosition().y <= borderTop
			|| objectHitBox.getPosition().y >= borderBottom) {

			speed = 0;
			attacking = true;

		}
	}

	void Enemy::Bounce() {

		vel.x = 0;
		vel.y = speed;
		RotateVector(vel, currentDirection);

		int borderLeft = World::GetInstance()->WorldScene.levelContainer->battleBorder.left;
		int borderTop = World::GetInstance()->WorldScene.levelContainer->battleBorder.top;
		int borderRight = borderLeft + World::GetInstance()->WorldScene.levelContainer->battleBorder.width;
		int borderBottom = borderTop + World::GetInstance()->WorldScene.levelContainer->battleBorder.height;

		int direction = 0;


		//check where we're hitting the border and set the reflect direction
		
		if (objectHitBox.getPosition().x >= borderRight
			|| objectHitBox.getPosition().x <= borderLeft
			|| objectHitBox.getPosition().y <= borderTop
			|| objectHitBox.getPosition().y >= borderBottom) {


			if (objectHitBox.getPosition().x >= borderRight) {

				if (vel.y < 0) {

					direction = 135;
				}

				if (vel.y > 0) {

					direction = 45;

				}

			}


			else if (objectHitBox.getPosition().x <= borderLeft) {

				if (vel.y < 0) {

					direction = 225;

				}

				if (vel.y > 0) {

					direction = 315;
				}

			}

			else if (objectHitBox.getPosition().y >= borderBottom) {

				if (vel.x < 0) {

					direction = 135;
				}

				if (vel.x > 0) {

					direction = 225;

				}

			}


			else if (objectHitBox.getPosition().y <= borderTop) {

				if (vel.x < 0) {

					direction = 45;

				}

				if (vel.x > 0) {

					direction = 315;

				}

			}

			objectSprite.move(-vel.x, -vel.y);

			vel.x = 0;
			vel.y = speed;

			RotateVector(vel, direction);
			currentDirection = direction;

		}

		OffsetPosition();

	}


	void Enemy::MoveSlide() {

		//for this movement type, speed is the amount of pixels traveled, NOT the speed

		vel.x = 0;
		vel.y = -speed;
	
		if (World::GetInstance()->Timer(*this, 400000.0, NODELAY) && velZ == 0) {

			int rand1 = RandomNumber(45);
			int rand2 = RandomNumber(1);
			if (rand2 == 0) rand1 *= -1;

			RotateVector(vel, -(GetAngle(objectSprite.getPosition(), World::GetInstance()->WorldScene.playerPtr->objectSprite.getPosition())));
			RotateVector(vel, rand1);

			targetPosition.x = objectSprite.getPosition().x + vel.x;
			targetPosition.y = objectSprite.getPosition().y + vel.y;

		}

		else {

			RotateVector(vel, -(GetAngle(objectSprite.getPosition(), targetPosition)));

		}

		sf::Vector2f newPos = sf::Vector2f(objectSprite.getPosition().x, objectSprite.getPosition().y);
		newPos.x += (targetPosition.x - objectSprite.getPosition().x) / 5;
		newPos.y += (targetPosition.y - objectSprite.getPosition().y) / 5;
		objectSprite.setPosition(newPos.x, newPos.y);
		
	
	}

	void Enemy::BehaveNone() {

	}

	void Enemy::MoveCircle() {

		objectSprite.setPosition(prevPos);
		objectSprite.move(vel.x,vel.y);


		float time = World::GetInstance()->clock2.getElapsedTime().asMilliseconds()*0.0025;

		double newPosX = cos((time)) * 40;
		double newPosY = sin((time)) * 40;

		prevPos = objectSprite.getPosition();
		objectSprite.move(newPosX,newPosY);

	}

	void Enemy::MoveJump() {

		if (velZ != 0) objectSprite.move(vel.x, vel.y + velZ);

		posZ -= velZ;

		if (objectSprite.getPosition().y < objectShadow.getPosition().y) {

			if (World::GetInstance()->Timer(*this, VERY_FAST))velZ += 0.2;

		}

		else {
			
			velZ = 0;

		}


	}

	//Attack functions

	void Enemy::Shoot() {

		if (World::GetInstance()->Timer(*this, 2000000.0)) {

			itemQueue bullet;
			bullet.properties["PosX"] = std::to_string(objectSprite.getPosition().x);
			bullet.properties["PosY"] = std::to_string(objectSprite.getPosition().y - 3);
			bullet.properties["itemType"] = "EnemyBlip";
			bullet.properties["Direction"] = std::to_string(fireDir);
			World::GetInstance()->WorldScene.objectContainer->Queue.push_back(bullet);

		}

	}

	void Enemy::SporeShoot() {

		if (attacking == true) {

			if (World::GetInstance()->Timer(*this, VERY_SLOW)) {

				itemQueue bullet;
				bullet.properties["PosX"] = std::to_string(objectSprite.getPosition().x);
				bullet.properties["PosY"] = std::to_string(objectSprite.getPosition().y - 3);
				bullet.properties["itemType"] = "EnemyBlip";
				bullet.properties["Speed"] = std::to_string(2);
				bullet.properties["Direction"] = std::to_string(fireDir);
				World::GetInstance()->WorldScene.objectContainer->Queue.push_back(bullet);

			}

		}

	}

	void Enemy::AroundShoot() {

		if (attacking == false) {

			if (World::GetInstance()->Timer(*this, VERY_SLOW*16)) {

				attacking = true;

			}

		}

		if (attacking == true) {

			if (World::GetInstance()->Timer(*this, NORMAL)) {

				itemQueue bullet;
				bullet.properties["PosX"] = std::to_string(objectSprite.getPosition().x);
				bullet.properties["PosY"] = std::to_string(objectSprite.getPosition().y - 6);
				bullet.properties["itemType"] = "EnemyBlip";
				bullet.properties["Speed"] = std::to_string(2);
				bullet.properties["Direction"] = std::to_string(fireDir);
				World::GetInstance()->WorldScene.objectContainer->Queue.push_back(bullet);


				bullet.properties["Direction"] = std::to_string(fireDir + 90);
				World::GetInstance()->WorldScene.objectContainer->Queue.push_back(bullet);


				bullet.properties["Direction"] = std::to_string(fireDir + 180);
				World::GetInstance()->WorldScene.objectContainer->Queue.push_back(bullet);


				bullet.properties["Direction"] = std::to_string(fireDir + 270);
				World::GetInstance()->WorldScene.objectContainer->Queue.push_back(bullet);


				fireDir+=10;

				if (fireDir >= 100) {

					attacking = false;
					fireDir = 0;

				}

			}

		}

		

	}

	void Enemy::LayFire() {

		if (World::GetInstance()->Timer(*this, VERY_SLOW*2)) {

			itemQueue bullet;
			bullet.properties["PosX"] = std::to_string(objectSprite.getPosition().x);
			bullet.properties["PosY"] = std::to_string(objectSprite.getPosition().y);
			bullet.properties["itemType"] = "EnemyMine";
			World::GetInstance()->WorldScene.objectContainer->Queue.push_back(bullet);
			attacking = false;
		}

	}


	void Enemy::Laser() {

		if (attacking == false) {

			if (PlayerDistance(MID)) {

				if (World::GetInstance()->Timer(*this, VERY_SLOW)) {

					itemQueue bullet;
					bullet.properties["PosX"] = std::to_string(objectSprite.getPosition().x);
					bullet.properties["PosY"] = std::to_string(objectSprite.getPosition().y);
					bullet.properties["itemType"] = "EnemyLaser";
					bullet.properties["Direction"] = std::to_string(fireDir);
					bullet.parent = this;
					World::GetInstance()->WorldScene.objectContainer->Queue.push_back(bullet);
					int rand = RandomNumber(2);
					World::GetInstance()->WorldScene.audioContainer.PlaySFX("sfx_elsr_" + std::to_string(rand));
					attacking = true;

				}
			}

		}

		else if (attacking == true) {

			if (World::GetInstance()->Timer(*this, VERY_SLOW * 10)) attacking = false;

		}

	}

	void Enemy::CircleShoot() {
		

		if (attacking == false) {

				// shoot, but wait for charge particles first
			if (World::GetInstance()->Timer(*this, FAST)) {

				Entity::itemQueue part;
				part.properties["itemType"] = "EnemyChargeParticle";
				part.properties["PosX"] = std::to_string(objectHitBox.getPosition().x);
				part.properties["PosY"] = std::to_string(objectHitBox.getPosition().y);
				World::GetInstance()->WorldScene.objectContainer->Queue.push_back(part);

			}

				if (World::GetInstance()->Timer(*this, VERY_SLOW*6)) {

					itemQueue bullet;
					bullet.properties["PosX"] = std::to_string(objectSprite.getPosition().x);
					bullet.properties["PosY"] = std::to_string(objectSprite.getPosition().y-16);
					bullet.properties["itemType"] = "HauzerSpear";
					bullet.properties["Speed"] = std::to_string(2);
					bullet.properties["Direction"] = std::to_string(0);
					World::GetInstance()->WorldScene.objectContainer->Queue.push_back(bullet);

					bullet.properties["Direction"] = std::to_string(45);
					World::GetInstance()->WorldScene.objectContainer->Queue.push_back(bullet);

					bullet.properties["Direction"] = std::to_string(90);
					World::GetInstance()->WorldScene.objectContainer->Queue.push_back(bullet);

					bullet.properties["Direction"] = std::to_string(135);
					World::GetInstance()->WorldScene.objectContainer->Queue.push_back(bullet);

					bullet.properties["Direction"] = std::to_string(180);
					World::GetInstance()->WorldScene.objectContainer->Queue.push_back(bullet);

					bullet.properties["Direction"] = std::to_string(225);
					World::GetInstance()->WorldScene.objectContainer->Queue.push_back(bullet);

					bullet.properties["Direction"] = std::to_string(270);
					World::GetInstance()->WorldScene.objectContainer->Queue.push_back(bullet);

					bullet.properties["Direction"] = std::to_string(315);
					World::GetInstance()->WorldScene.objectContainer->Queue.push_back(bullet);
					World::GetInstance()->WorldScene.audioContainer.PlaySFX("sfx_enemy_shot");

					attacking = true;

				//if not yet ready to shoot, create charge particles

			
			}

		}

		else if (attacking == true) {

			// pauses enemy a bit to prevent them from immediately attacking again

			if (World::GetInstance()->Timer(*this, VERY_SLOW*5)) {

				attacking = false;

			}


		}

	}

	void Enemy::CastShoot() {

		if (attacking == false) {

			if (PlayerDistance(MID)) {

				// create charge object (charge object casts bullets)

				itemQueue bullet;
				bullet.properties["PosX"] = std::to_string(hotSpot.x);
				bullet.properties["PosY"] = std::to_string(hotSpot.y);
				bullet.properties["itemType"] = "EnemyCharge";
				bullet.parent = this;
				World::GetInstance()->WorldScene.objectContainer->Queue.push_back(bullet);
				attacking = true;

			}

		}

		else if (attacking == true) {

			//ENEMY SHOULDNT MOVE IF ATTACKING

			// this time should take in account the charge asset + bullets; there should be a small break and

			if (World::GetInstance()->Timer(*this, 1000000.0)) attacking = false;

			// also can set "cast" animatino here as well

		}

	}

	void Enemy::SelfDestruct() {

		if (PlayerDistance(CLOSE)) {

			// create charge object (charge object casts bullets)
			Die();
		}

	}

	void Enemy::DoubleCast() {

		if (attacking == false) {

			if (PlayerDistance(MID)) {

				// create charge object (charge object casts bullets)

				itemQueue bullet;
				bullet.properties["PosX"] = std::to_string(objectSprite.getPosition().x + 50);
				bullet.properties["PosY"] = std::to_string(objectSprite.getPosition().y - 35);
				bullet.properties["itemType"] = "StarSpawn";
				bullet.parent = this;
				World::GetInstance()->WorldScene.objectContainer->Queue.push_back(bullet);
				bullet.properties["PosX"] = std::to_string(objectSprite.getPosition().x - 50);
				World::GetInstance()->WorldScene.objectContainer->Queue.push_back(bullet);
				attacking = true;

			}

		}

		else if (attacking == true) {

			//ENEMY SHOULDNT MOVE IF ATTACKING

			// this time should take in account the charge asset + bullets; there should be a small break and

			if (World::GetInstance()->Timer(*this, 1000000.0)) attacking = false;

			// also can set "cast" animatino here as well

		}





	}

	void Enemy::OffsetPosition() {


			if (World::GetInstance()->Timer(*this, 200000.0)) {

				varyMov.x = RandomNumber(35);
				varyMov.y = RandomNumber(35);
				int rand = RandomNumber(1);
				int rand2 = RandomNumber(1);

				if (rand == 0) {

					varyMov.x *= -1;

				}

				if (rand2 == 0) {

					varyMov.y *= -1;

				}

			}

	}


	void Enemy::Animate() {

		if (flatAnimation == true) {

			if (World::GetInstance()->Timer(*this, animationSpeed)) {

				if (frame >= 3) frame = 0;

				else frame++;

			}

			objectSprite.setTextureRect(sf::IntRect(objectSprite.getTextureRect().left, (spriteSheetHeight * frame) + spriteTop, objectSprite.getTextureRect().width, objectSprite.getTextureRect().height));

		}

		else if (flatAnimation == false) {

			/*
			spriteDirection = RoundUp((GetAngle(objectShadow.getPosition(), World::GetInstance()->WorldScene.playerPtr->objectShadow.getPosition())), 45);
			int tempdir = abs((180 - spriteDirection) / 45);
			spriteDirection = tempdir;
			*/

			spriteDirection = objectSprite.getPosition().x < (objectSprite.getPosition().x + vel.x) ? 1 : 0;


			if (World::GetInstance()->Timer(*this, animationSpeed)) {

				if (AssignedMovement != &Enemy::MoveJump) {

					if (frame >= 3) frame = 0;
					else if (AssignedMovement != &Enemy::MoveSlide) frame++;

					else if (AssignedMovement == &Enemy::MoveSlide) {

						if (!IfDistance(objectSprite.getPosition(), targetPosition, 2)) frame++;

					}
				}

				else {

					if (frame < 3) frame++;
					if (frame == 2) velZ = -2;
					if (frame == 3 && velZ == 0) frame = 0;
				}

			}

			objectSprite.setTextureRect(sf::IntRect(objectSprite.getTextureRect().left, (((spriteDirection * 4 + frame) * spriteSheetHeight) + spriteTop), objectSprite.getTextureRect().width, objectSprite.getTextureRect().height));

		}

	}
    
	void Enemy::Move() {

		if (AssignedBehavior != NULL || 0) AssignedBehavior(this);
		if (AssignedMovement != NULL || 0) AssignedMovement(this);
		MoveElse();
		Animate(); 
        if(alwaysTargetPlayer == true) fireDir = GetAngle(objectSprite.getPosition(),World::GetInstance()->WorldScene.playerPtr->objectSprite.getPosition());
    
    }
    
    
    void Enemy::Act(){

		// HotSpot - vector where we create objects in orientation from the enemy. We need 
		// to flip this vector horizontally depending on the direction the sprite is facing

		if (health > 0) {

			sf::Vector2f hs;
			hs = hotSpot;
			if (hotSpot.x != 0 && hotSpot.y != 0) {

				if (objectSprite.getTextureRect().top >= 696) {

					hotSpot.x = objectSprite.getPosition().x + hotSpot.x;
					hotSpot.y = objectSprite.getPosition().y - hotSpot.y;

				}

				else {

					hotSpot.x = objectSprite.getPosition().x - hotSpot.x;
					hotSpot.y = objectSprite.getPosition().y - hotSpot.y;

				}
			}


			if (spriteTop == -1) spriteTop = objectSprite.getTextureRect().top;

			// Creates effects for enemies that lack transition (specifically for projectile enemies)

			if (!transition && !active) {

				objectSprite.setColor(sf::Color(255, 255, 255, 255));
				active = true;
				itemQueue particles;
				particles.properties["PosX"] = std::to_string(objectSprite.getPosition().x);
				particles.properties["PosY"] = std::to_string(objectSprite.getPosition().y);
				particles.properties["itemType"] = "DeathPoof";
				World::GetInstance()->WorldScene.objectContainer->Queue.push_back(particles);
				World::GetInstance()->WorldScene.objectContainer->Queue.push_back(particles);
				World::GetInstance()->WorldScene.objectContainer->Queue.push_back(particles);

			}

			//if transition is complete, run move and attack functions

			if (active) {

				//return to original position from hurtPos

				if (hurt == true) objectSprite.move(-hurtPos);
				if (attacking == false) Move();
				Attack();

				// misc decorations

				if (hotSpot.x != 0 && hotSpot.y != 0) {

					if (World::GetInstance()->Timer(*this, SLOW)) {


						itemQueue particles;
						particles.properties["PosX"] = std::to_string(hotSpot.x - 4 + RandomNumber(8));
						particles.properties["PosY"] = std::to_string(hotSpot.y - 4 + RandomNumber(8));
						particles.properties["itemType"] = "EnemySpark";
						World::GetInstance()->WorldScene.objectContainer->Queue.push_back(particles);

					}

				}


				UpdateShadow();

			}

			// if enemy has not fully transitioned into scene, continue to progress transition

			else {

				if (World::GetInstance()->Timer(*this, NORMAL, NODELAY)) {

					if (int(objectSprite.getColor().a) < 255) objectSprite.setColor(sf::Color(255, 255, 255, int(objectSprite.getColor().a) + 5));
					else active = true;

				}


			}

			isHurt();
			hotSpot = hs;
		}

		else {

			Die();
		}
    }

	void Enemy::isHurt() {

		if (hurt == true) {

			if (World::GetInstance()->Timer(*this,21010.0)) {

				hurt = false;
				hurtPos.y = 0;
				hurtPos.x = 0;

			}

			else {

				hurtPos.x = RandomNumber(4);
				hurtPos.y = RandomNumber(4);
				objectSprite.move(hurtPos);

			}
			 
		}
	}
    
    
    void Enemy::Update()
                 
    {
        
		if (!World::GetInstance()->WorldScene.playerPtr->dead) {

			Act();

			// Check which enemy is closests to the player (for homing weapon)

			if (active && World::GetInstance()->WorldScene.playerPtr) {

				if (closestEnemy){

					if (closestEnemy->misDestroyed == true) closestEnemy = nullptr;
				}

				if (!closestEnemy) closestEnemy = this;

				else {

					if (GetDistance(objectSprite.getPosition(), World::GetInstance()->WorldScene.playerPtr->objectSprite.getPosition()) <
						GetDistance(closestEnemy->objectSprite.getPosition(), World::GetInstance()->WorldScene.playerPtr->objectSprite.getPosition()))
						closestEnemy = this;
				}
			}

		}
    }
    
	void Enemy::Attack() {

		if(AssignedAttack != NULL || 0) AssignedAttack(this);
        
    }
    
	void Enemy::DrawChild() {

		  

	}

	void Djinn::DrawChild() {

		if (World::GetInstance()->Timer(*this, VERY_FAST)) {

			if (wings.getTextureRect().top >= 348) wings.setTextureRect(sf::IntRect(243, 0, 134, 44));

			else wings.setTextureRect(sf::IntRect(243, wings.getTextureRect().top + spriteSheetHeight, 134, 44));

		}

		wings.setPosition(objectSprite.getPosition().x-66, objectSprite.getPosition().y-100);
		World::GetInstance()->DrawObject(wings);

	}

    void Enemy::Draw(sf::RenderTarget& window){
        
		if(active) SetSpriteRedForHealth();

		DrawChild();

		if (showHurt == true) {
			World::GetInstance()->DrawObject(objectSprite, "whiteShader");
			showHurt = false;
		}

		else if(bigDaddy == true && attacking == true) World::GetInstance()->DrawObject(objectSprite,"chargeShader");

		else World::GetInstance()->DrawObject(objectSprite);


    }

	void Enemy::TargetPlayer() {

		//BUG
		if(World::GetInstance()->WorldScene.playerPtr) fireDir = GetAngle(objectSprite.getPosition(), World::GetInstance()->WorldScene.playerPtr->objectSprite.getPosition());
	}
    
    void Boss::Update(){
        
        Act();
		HUDUpdate();
		MoveElse();
        
    }

	void Boss::Move() {

		if (currentMovement > BehaviorList.size() - 1) { 
			
			currentMovement = 0;
			NewMovementSet();
	
		}

		BehaviorList[currentMovement](this);

	}

	void Boss::Attack() {


	}

	void Boss::Behavior1(){

	}

	void Boss::Behavior2(){

	}

	void Boss::Behavior3(){

	}

	void Boss::Behavior4(){

	}

	void Boss::Rest() {

	}


	void Boss::Idle() {

		vel.x = 0;
		vel.y = 0;

	}

	void Boss::Roam() {

	}

	void Boss::RoachMovement() {

	}

	void Boss::NewMovementSet() {

		std::vector<boost::function<void(Boss*)>> tempList;

		boost::function<void(Boss*)> f2;
		boost::function<void(Boss*)> f3;
		boost::function<void(Boss*)> f4;
		boost::function<void(Boss*)> f5;


		f2 = &Boss::Behavior3;
		f3 = &Boss::Behavior4;
		f4 = &Boss::Behavior2;
		f5 = &Boss::Behavior1;

		tempList.push_back(f2);
		tempList.push_back(f3);
		tempList.push_back(f4);
		tempList.push_back(f5);

		for (int i = 0; i < BehaviorList.size(); i++) {

			BehaviorList[i] = tempList[RandomNumber(3,0)];
			//std::cout << i << " for " << BehaviorList.size() << std::endl;
				
		}

		tempList.clear();
	}

	void Boss::MoveToCenter() {

		float oldx = objectHitBox.getPosition().x;
		float oldy = objectHitBox.getPosition().y;

		oldx += ((World::GetInstance()->WorldScene.levelContainer->lvlSize.x/2) - oldx) / 25;
		oldy += ((World::GetInstance()->WorldScene.levelContainer->lvlSize.y/2) - oldy) / 25;

		objectSprite.setPosition(oldx, oldy);
		if (World::GetInstance()->Timer(*this, NORMAL, NODELAY)) {


			itemQueue bullet;
			bullet.properties["PosX"] = std::to_string(objectSprite.getPosition().x);
			bullet.properties["PosY"] = std::to_string(objectSprite.getPosition().y - 6);
			bullet.properties["itemType"] = "SpriteClone";
			bullet.parent = this;
			World::GetInstance()->WorldScene.objectContainer->Queue.push_back(bullet);

		}

	}

	void Boss::FollowPlayer() {

		vel.x = 0;
		vel.y = -speed;

		//set move to player
		if(World::GetInstance()->WorldScene.playerPtr)RotateVector(vel, -(GetAngle(objectSprite.getPosition(), World::GetInstance()->WorldScene.playerPtr->objectSprite.getPosition())));
		else speed = 0;
		objectSprite.move(vel.x, vel.y + velZ);

	}

	void Mozza::Behavior1() {

		// Dash & spreader

		Idle();

		AnimateAttack();

		if (phase == 0) {

			World::GetInstance()->WorldScene.audioContainer.PlaySFX("sfx_enemycharge");
			Entity::itemQueue enemy;
			phase = 1;

		}

		else if (phase == 1) {

			if (World::GetInstance()->Timer(*this, 1000000.0)) phase = 2;

		}

		else if (phase == 2) {

			if (World::GetInstance()->Timer(*this, VERY_SLOW)) {

				TargetPlayer();
				itemQueue bullet;
				bullet.properties["PosX"] = std::to_string(objectSprite.getPosition().x);
				bullet.properties["PosY"] = std::to_string(objectSprite.getPosition().y - 6);
				bullet.properties["itemType"] = "HauzerSpear";
				bullet.properties["Speed"] = std::to_string(2);
				bullet.properties["Direction"] = std::to_string(fireDir);
				World::GetInstance()->WorldScene.objectContainer->Queue.push_back(bullet);

				bullet.properties["Direction"] = std::to_string(fireDir - 5);
				World::GetInstance()->WorldScene.objectContainer->Queue.push_back(bullet);

				bullet.properties["Direction"] = std::to_string(fireDir + 5);
				World::GetInstance()->WorldScene.objectContainer->Queue.push_back(bullet);

				bullet.properties["Direction"] = std::to_string(fireDir + 10);
				World::GetInstance()->WorldScene.objectContainer->Queue.push_back(bullet);

				bullet.properties["Direction"] = std::to_string(fireDir + 10);
				World::GetInstance()->WorldScene.objectContainer->Queue.push_back(bullet);
				World::GetInstance()->WorldScene.audioContainer.PlaySFX("sfx_enemy_shot");

			}

			if (World::GetInstance()->Timer(*this, VERY_SLOW*3)) NextMovement();

		}

	}

	void Mozza::Behavior2() {

		// Spawn Spires

		AnimateIdle();

		/*
		if (phase == 0) {

			World::GetInstance()->WorldScene.audioContainer.PlaySFX("sfx_enemycharge");
			Entity::itemQueue enemy;
			enemy.properties["itemType"] = "HauzerCharge";
			enemy.properties["PosX"] = std::to_string(objectHitBox.getPosition().x - 50);
			enemy.properties["PosY"] = std::to_string(objectHitBox.getPosition().y);
			World::GetInstance()->WorldScene.objectContainer->Queue.push_back(enemy);

			enemy.properties["PosX"] = std::to_string(objectHitBox.getPosition().x + 50);
			World::GetInstance()->WorldScene.objectContainer->Queue.push_back(enemy);

			phase = 1;

		}

		else if (phase == 1) {

			if (World::GetInstance()->Timer(*this, 1600000.0)) { 

				Entity::itemQueue enemy;
				enemy.properties["itemType"] = "HauzerSpire";
				enemy.properties["PosX"] = std::to_string(World::GetInstance()->WorldScene.playerPtr->objectHitBox.getPosition().x);
				enemy.properties["PosY"] = std::to_string(World::GetInstance()->WorldScene.playerPtr->objectHitBox.getPosition().y);
				World::GetInstance()->WorldScene.objectContainer->Queue.push_back(enemy);


				World::GetInstance()->ScreenShake(20);
				World::GetInstance()->WorldScene.audioContainer.PlaySFX("sfx_faintexp");
				World::GetInstance()->WorldScene.audioContainer.PlaySFX("sfx_stomp");

				NextMovement(); 
			}

		}
		*/

		if (phase == 0) {

			World::GetInstance()->WorldScene.audioContainer.PlaySFX("sfx_enemycharge");
			phase = 1;

		}


		else if (phase == 1) {

			if (World::GetInstance()->Timer(*this, SLOW)) {

				Entity::itemQueue part;
				part.properties["itemType"] = "EnemyChargeParticle";
				part.properties["PosX"] = std::to_string(objectHitBox.getPosition().x);
				part.properties["PosY"] = std::to_string(objectHitBox.getPosition().y);
				World::GetInstance()->WorldScene.objectContainer->Queue.push_back(part);
				World::GetInstance()->WorldScene.objectContainer->Queue.push_back(part);
				World::GetInstance()->WorldScene.objectContainer->Queue.push_back(part);

			}

			if (World::GetInstance()->Timer(*this, 1000000.0)) phase = 2;

		}


		else if (phase == 2) {

					itemQueue bullet;
					bullet.properties["PosX"] = std::to_string(objectSprite.getPosition().x-20);
					bullet.properties["PosY"] = std::to_string(objectSprite.getPosition().y);
					bullet.properties["itemType"] = "BigEnemyLaser";
					bullet.properties["Direction"] = std::to_string(fireDir);
					bullet.parent = this;
					World::GetInstance()->WorldScene.objectContainer->Queue.push_back(bullet);
					bullet.properties["PosX"] = std::to_string(objectSprite.getPosition().x + 20);
					World::GetInstance()->WorldScene.objectContainer->Queue.push_back(bullet);
					World::GetInstance()->WorldScene.audioContainer.PlaySFX("sfx_superbeam2");
					phase = 3;

		}

		else if (phase == 3 ) {

			World::GetInstance()->ScreenShake(2);
			if (World::GetInstance()->Timer(*this, VERY_SLOW * 10)) NextMovement();

		}

	}

	void Mozza::Behavior3() {

		// Toxic breath

		FollowPlayer();

		AnimateAttack();

		if (PlayerDistance(FAR)) {

			if (World::GetInstance()->Timer(*this, FAST,NODELAY)) {

				TargetPlayer();
				itemQueue bullet;
				bullet.properties["PosX"] = std::to_string(objectSprite.getPosition().x);
				bullet.properties["PosY"] = std::to_string(objectSprite.getPosition().y - 6);
				bullet.properties["itemType"] = "HauzerSmog";
				int rand = RandomNumber(1);
				if(rand == 0) bullet.properties["Direction"] = std::to_string(fireDir + RandomNumber(2));
				if (rand == 1) bullet.properties["Direction"] = std::to_string(fireDir - RandomNumber(2));

				World::GetInstance()->WorldScene.objectContainer->Queue.push_back(bullet);

			}

			if (World::GetInstance()->Timer(*this, SLOW, NODELAY)) World::GetInstance()->WorldScene.audioContainer.PlaySFX("sfx_hauzerbreath");
		}

		if (World::GetInstance()->Timer(*this, 3200000.0)) NextMovement();

	}

	void Mozza::Behavior4() {

		// 8 direction continous shot


		if (phase == 0) {

			AnimateIdle();
			World::GetInstance()->WorldScene.audioContainer.PlaySFX("sfx_hauzerbark");
			phase = 1;
			
		}

		else if (phase == 1) {

			AnimateIdle();
			if (World::GetInstance()->Timer(*this, 600000.0)) { 
				
				phase = 2;
				World::GetInstance()->WorldScene.audioContainer.PlaySFX("sfx_bossdash"); 
			
			}

		}

		else if (phase == 2) {

			AnimateIdle();
			MoveToCenter();
			if (World::GetInstance()->Timer(*this, 1600000.0)) phase = 3;
		}

		else if (phase == 3) {

			AnimateAttack();

			if (World::GetInstance()->Timer(*this, SLOW)) {

				itemQueue bullet;
				bullet.properties["PosX"] = std::to_string(objectSprite.getPosition().x);
				bullet.properties["PosY"] = std::to_string(objectSprite.getPosition().y - 6);
				bullet.properties["itemType"] = "EnemyBlip";
				bullet.properties["Speed"] = std::to_string(2);
				bullet.properties["Direction"] = std::to_string(fireDir);
				World::GetInstance()->WorldScene.objectContainer->Queue.push_back(bullet);

				bullet.properties["Direction"] = std::to_string(fireDir + 45);
				World::GetInstance()->WorldScene.objectContainer->Queue.push_back(bullet);

				bullet.properties["Direction"] = std::to_string(fireDir + 90);
				World::GetInstance()->WorldScene.objectContainer->Queue.push_back(bullet);


				bullet.properties["Direction"] = std::to_string(fireDir + 135);
				World::GetInstance()->WorldScene.objectContainer->Queue.push_back(bullet);


				bullet.properties["Direction"] = std::to_string(fireDir + 180);
				World::GetInstance()->WorldScene.objectContainer->Queue.push_back(bullet);


				bullet.properties["Direction"] = std::to_string(fireDir + 225);
				World::GetInstance()->WorldScene.objectContainer->Queue.push_back(bullet);


				bullet.properties["Direction"] = std::to_string(fireDir + 270);
				World::GetInstance()->WorldScene.objectContainer->Queue.push_back(bullet);


				bullet.properties["Direction"] = std::to_string(fireDir + 315);
				World::GetInstance()->WorldScene.objectContainer->Queue.push_back(bullet);

				fireDir += 10;
			}

			if (World::GetInstance()->Timer(*this, 6000000.0)) NextMovement();
		}


	}

	void Mozza::Rest() {

		AnimateIdle();

		if (World::GetInstance()->Timer(*this, 1600000.0,NODELAY)) {

			int randx = RandomNumber(100,50);
			int randy = RandomNumber(100,50);

			int randabs = RandomNumber(1);
			int randabs2 = RandomNumber(1);

			if (randabs == 1) randx = -randx; 
			if (randabs2 == 1) randy = -randy;

			targetPosition.x = objectSprite.getPosition().x + randx;
			targetPosition.y = objectSprite.getPosition().y + randy;

			if (targetPosition.x < 100) targetPosition.x = 100;
			if (targetPosition.x >(World::GetInstance()->WorldScene.levelContainer->lvlSize.x / 2)) targetPosition.x = (World::GetInstance()->WorldScene.levelContainer->lvlSize.x / 2);
			if (targetPosition.y < 100) targetPosition.y = 100;
			if (targetPosition.y >(World::GetInstance()->WorldScene.levelContainer->lvlSize.x / 2)) targetPosition.y = (World::GetInstance()->WorldScene.levelContainer->lvlSize.x / 2);

			//std::cout << targetPosition.x << " & " << targetPosition.y << std::endl;

		}

		float oldx = objectSprite.getPosition().x;
		float oldy = objectSprite.getPosition().y;

		oldx += (targetPosition.x - oldx) / 50;
		oldy += (targetPosition.y - oldy) / 50;

		objectSprite.setPosition(oldx, oldy);
		
		if (World::GetInstance()->Timer(*this, 6000000.0)) NextMovement();

	}

	void Boss::NextMovement() {

		phase = 0;
		currentMovement++;
		defending = true;
	}

	void Boss::AnimateIdle() {

		if (World::GetInstance()->Timer(*this, VERY_SLOW)) {

			frame++;
			if (frame >= 4) frame = 0;
			objectSprite.setTextureRect(sf::IntRect(0,(frame * 280) + 105, 175, 175));
			defending = false;

		}
	}

	void Boss::AnimateAttack() {

		if (World::GetInstance()->Timer(*this, VERY_FAST)) {

			frame++;
			if (frame >= 4) frame = 0;
			objectSprite.setTextureRect(sf::IntRect(0, 1120 + frame * 280 + 105, 175, 175));
			defending = false;

		}
	}



	void Boss::HUDUpdate() {

		if (health <= 0) {

			Die();
			int currentLevel = World::GetInstance()->GlobalMembers.currentLevel;
			World::GetInstance()->GlobalMembers.levelsCompleted.at(currentLevel) = 1;

		}

	}

	void Mozza::isHurt() {

		if (hurt) {

			if (World::GetInstance()->Timer(*this,400000.0)) {
				hurt = false;
				hurtPos.y = 0;
				hurtPos.x = 0;
			}

			else {
				hurtPos.x = RandomNumber(4);
				hurtPos.y = RandomNumber(4);
				objectSprite.move(hurtPos);
				wings.move(hurtPos);
			}

		}
	}

	void Mozza::MoveElse() {

		if (World::GetInstance()->Timer(*this, VERY_FAST)) {

			wingFrames++;
			wings.setTextureRect(sf::IntRect(0, wingFrames*280, 175, 44));
			if (wingFrames >= 3) wingFrames = 0;

		}

		wings.setPosition(objectSprite.getPosition().x,objectSprite.getPosition().y+50);

	}
    
	void Mozza::Draw(sf::RenderTarget& window) {

		if (active) SetSpriteRedForHealth();

		if (showHurt == true) {
			World::GetInstance()->DrawObject(objectSprite, "whiteShader");
			showHurt = false;
		}

		else World::GetInstance()->DrawObject(objectSprite);

		World::GetInstance()->DrawObject(wings);


	}
    
    void Boss::Draw(sf::RenderTarget& window){

		if (active) SetSpriteRedForHealth();


		if (showHurt == true) {
			World::GetInstance()->DrawObject(objectSprite, "whiteShader");
			showHurt = false;
		}

		else World::GetInstance()->DrawObject(objectSprite);
        World::GetInstance()->DrawObject(objectHitBox);
        
        
        
    }

    
    // Destructors
    
	Enemy::~Enemy()
	{


    }
    
    Boss::~Boss(){

		World::GetInstance()->WorldScene.audioContainer.music.stop();

		itemQueue orb;
		orb.properties["PosX"] = std::to_string(objectSprite.getPosition().x);
		orb.properties["PosY"] = std::to_string(objectSprite.getPosition().y);
		orb.properties["itemType"] = "SoulOrb";

		World::GetInstance()->WorldScene.objectContainer->Queue.push_back(orb);
    

    }

	void Enemy::Die() {

		
		// if this is a large enemy, do appropriate death animation; otherwise do the small one

		if (bigDaddy == true) {

			if (World::GetInstance()->Timer(*this, NORMAL)) {

				int ex = RandomNumber(64, 0);
				int ey = RandomNumber(64, 0);

				itemQueue particles;
				particles.properties["PosX"] = std::to_string((objectSprite.getPosition().x - 32) + ex);
				particles.properties["PosY"] = std::to_string((objectSprite.getPosition().y - objectSprite.getTextureRect().height/2 + 32) - ey);
				particles.properties["itemType"] = "DeathPoof";

				World::GetInstance()->WorldScene.objectContainer->Queue.push_back(particles);
				World::GetInstance()->WorldScene.objectContainer->Queue.push_back(particles);
				World::GetInstance()->WorldScene.objectContainer->Queue.push_back(particles);

				particles.properties["itemType"] = "DeathBoom";
				World::GetInstance()->WorldScene.objectContainer->Queue.push_back(particles);
				int randnum = RandomNumber(2);
				World::GetInstance()->WorldScene.audioContainer.PlaySFX("sfx_bighit" + std::to_string(randnum));

			}

			if (World::GetInstance()->Timer(*this, VERY_SLOW * 4)) {

				misDestroyed = true;
				World::GetInstance()->ScreenShake(5);
				itemQueue particles;
				for (int i = 0; i != 30; i++) {

					int oh = RandomNumber(85, 0);
					int ow = RandomNumber(85, 0);
					particles.properties["PosX"] = std::to_string((objectSprite.getPosition().x - 85/2) + ow);
					particles.properties["PosY"] = std::to_string((objectSprite.getPosition().y - objectSprite.getTextureRect().height/2) + 85/2 - oh);
					particles.properties["itemType"] = "EnemyPart";
					World::GetInstance()->WorldScene.objectContainer->Queue.push_back(particles);
					World::GetInstance()->WorldScene.audioContainer.PlaySFX("sfx_growl");
					World::GetInstance()->WorldScene.audioContainer.PlaySFX("sfx_stomp");


				}


			}

		}

		else {

			misDestroyed = true;

			itemQueue particles;
			particles.properties["PosX"] = std::to_string(objectSprite.getPosition().x);
			particles.properties["PosY"] = std::to_string(objectSprite.getPosition().y);
			particles.properties["itemType"] = "DeathPoof";

			World::GetInstance()->WorldScene.objectContainer->Queue.push_back(particles);
			World::GetInstance()->WorldScene.objectContainer->Queue.push_back(particles);
			World::GetInstance()->WorldScene.objectContainer->Queue.push_back(particles);

			particles.properties["itemType"] = "DeathBoom";
			World::GetInstance()->WorldScene.objectContainer->Queue.push_back(particles);
			int randnum = RandomNumber(2);
			World::GetInstance()->WorldScene.audioContainer.PlaySFX("sfx_bighit" + std::to_string(randnum));
		}
	}

	HauzerSpire::~HauzerSpire() {

	}
    
    Mozza::~Mozza(){
        
    }

	Mask::~Mask() {

	}

	BabyDemon::~BabyDemon() {

	}

	HorseKnight::~HorseKnight() {

	}

	Family::~Family() {

		itemQueue bullet;
		bullet.properties["PosX"] = std::to_string(objectSprite.getPosition().x);
		bullet.properties["PosY"] = std::to_string(objectSprite.getPosition().y - 16);
		bullet.properties["itemType"] = "EnemyBlip";
		bullet.properties["Speed"] = std::to_string(2);
		bullet.properties["Direction"] = std::to_string(0);
		World::GetInstance()->WorldScene.objectContainer->Queue.push_back(bullet);

		bullet.properties["Direction"] = std::to_string(45);
		World::GetInstance()->WorldScene.objectContainer->Queue.push_back(bullet);

		bullet.properties["Direction"] = std::to_string(90);
		World::GetInstance()->WorldScene.objectContainer->Queue.push_back(bullet);

		bullet.properties["Direction"] = std::to_string(135);
		World::GetInstance()->WorldScene.objectContainer->Queue.push_back(bullet);

		bullet.properties["Direction"] = std::to_string(180);
		World::GetInstance()->WorldScene.objectContainer->Queue.push_back(bullet);

		bullet.properties["Direction"] = std::to_string(225);
		World::GetInstance()->WorldScene.objectContainer->Queue.push_back(bullet);

		bullet.properties["Direction"] = std::to_string(270);
		World::GetInstance()->WorldScene.objectContainer->Queue.push_back(bullet);

		bullet.properties["Direction"] = std::to_string(315);
		World::GetInstance()->WorldScene.objectContainer->Queue.push_back(bullet);


	}

	Buddy::~Buddy() {
		//std::cout << "buddy destroyed" << std::endl;
	}
    
    EnemyNode::~EnemyNode(){
        
    }
    
    Actor::~Actor(){
        
        
    }

	SoulOrb::~SoulOrb() {

	}

	Raider::~Raider() {

	}
    
    Slime::~Slime(){
        
    }

	Tomb::~Tomb() {

	}

	Tower::~Tower() {

	}

	Bricky::~Bricky() {

	}

	Roach::~Roach() {

	}

	Eball::~Eball() {

	}

	Spore::~Spore() {

	}
    
    Star::~Star(){
        
        
    }

	Sprite::~Sprite() {

	}

	Djinn::~Djinn() {

	}
    
    Squid::~Squid(){
        
    }
    
    ////// MISC FUNCTIONS
    
    
    void Object::GetZone(int vecPos){
        
        zone.clear();
        
		int zoneSize = World::GetInstance()->WorldScene.objectContainer->zoneSize;
		int zonesPerScreen = World::GetInstance()->WorldScene.objectContainer->zonesPerScreen;

		// Gets the local bounds of the objects sprite, minus its origin 
        sf::IntRect bounds = GetObjectBounds();


		//Gets the current zones that the sprite occupies
		int leftTop = RoundDown(bounds.left, zoneSize) * 2 / 100 + ((RoundDown(bounds.top, zoneSize) * 2 / 100)*zonesPerScreen);
		int rightTop = RoundDown(bounds.left + bounds.width, zoneSize) * 2 / 100 + ((RoundDown(bounds.top, zoneSize) * 2 / 100)*zonesPerScreen);
		int leftBttm = RoundDown(bounds.left, zoneSize) * 2 / 100 + ((RoundDown(bounds.top + bounds.height, zoneSize) * 2 / 100)*zonesPerScreen);
		int rightBttm = RoundDown(bounds.left + bounds.width, zoneSize) * 2 / 100 + ((RoundDown(bounds.top + bounds.height, zoneSize) * 2 / 100)*zonesPerScreen);

		/*
        int leftTop = (RoundDown(bounds.left,zoneSize) + RoundDown(bounds.top,zoneSize) + (RoundDown(bounds.top,zoneSize)*2))/100 ;
        int rightTop = (RoundDown(bounds.width,zoneSize) + RoundDown(bounds.top,zoneSize) + (RoundDown(bounds.top,zoneSize)*2))/100;
        int leftBttm = (RoundDown(bounds.left,zoneSize) + RoundDown(bounds.height,zoneSize) + (RoundDown(bounds.height,zoneSize)*2))/100;
        int rightBttm = (RoundDown(bounds.width,zoneSize) + RoundDown(bounds.height,zoneSize) + (RoundDown(bounds.height,zoneSize)*2))/100;
        */

        std::vector<int> vec = {leftTop,rightTop,leftBttm,rightBttm};
        std::sort(vec.begin(),vec.end());
        vec.erase( unique( vec.begin(), vec.end() ), vec.end() );

        for(int i : vec){
			
            World::GetInstance()->WorldScene.objectContainer->zoneMatrix.insert(std::make_pair(i,vecPos));
            zone.append(std::to_string(i) + ",");
        }
        

    }
    
    
    // This function is called to set the origin to the bottom and center of the sprite for accurate draw order
    
    void Object::CheckLvlCollision(){
        
    }
    
    void Player::CheckLvlCollision(){
        
        sf::Vector2f currPos = objectShadow.getPosition();
        sf::Vector2f prevPos = objectShadow.getPosition() - vel;
        prevPos.x = round(prevPos.x);
        prevPos.y = round(prevPos.y);
        
        World::GetInstance()->WorldScene.levelContainer->GetSurroundingTiles(prevPos,currPos,objectShadow.getTextureRect());
        
        int spriteWidth = abs(objectShadow.getTextureRect().width);
        int spriteHeight = objectShadow.getTextureRect().height;
        int tileWidth = 16;
        
        if(World::GetInstance()->WorldScene.levelContainer->collisionArray.size() > 0){
            
            for(Level::collArrayIter it = World::GetInstance()->WorldScene.levelContainer->collisionArray.begin(); it != World::GetInstance()->WorldScene.levelContainer->collisionArray.end(); it++){
                
                if(IsOverlapping(currPos.x - spriteWidth/2, currPos.x + spriteWidth/2, it->x,it->x + tileWidth) && IsOverlapping(currPos.y - spriteHeight/2, currPos.y + spriteHeight/2, it->y,it->y + tileWidth) ){
                    
                    // Check previous interesection of X axis only, if there is no interesct, we know the X axis needs to be cleared by this block on this axis
                    
                    if(!IsOverlapping(prevPos.x - spriteWidth/2,prevPos.x + spriteWidth/2, it->x, it->x + tileWidth)){
                        
                        // If collision on left ...
                        
                        if(prevPos.x - spriteWidth/2 > currPos.x - spriteWidth/2){
                            
                            currPos.x = it->x + tileWidth + spriteWidth/2;
                            
                        }
                        
                        // If collision on right ...
                        
                        else if(prevPos.x - spriteWidth/2 < currPos.x - spriteWidth/2){
                            
                            currPos.x = it->x - spriteWidth/2;
                            
                        }
                        
                        objectSprite.setPosition(currPos.x,currPos.y);
                        objectShadow.setPosition(currPos.x,currPos.y);
                        
                    }
                    
                    // Check previous interesection of Y axis only, if there is no interesct, we know the Y axis needs to be cleared by this block on this axis
                    
                    if(!IsOverlapping(prevPos.y - spriteHeight/2,prevPos.y + spriteHeight/2, it->y, it->y + tileWidth)){
                        
                        // If collision on top ...
                        
                        if(prevPos.y - spriteHeight/2 > currPos.y - spriteHeight/2){
                            
                            currPos.y = it->y + tileWidth + spriteHeight/2;
                            
                        }
                        
                        // If collision on bottom ...
                        
                        else if(prevPos.y - spriteHeight/2 < currPos.y - spriteHeight/2){
                            
                            currPos.y = it->y - spriteHeight/2;
                            
                        }
                        
                        objectSprite.setPosition(currPos.x,currPos.y);
                        objectShadow.setPosition(currPos.x,currPos.y);
                        
                    }
                    
                    frame_pos = 0;
                    vel.x = 0;
                    vel.y = 0;
                    objectSprite.setTextureRect(sf::IntRect(0,objectSprite.getTextureRect().top,objectSprite.getTextureRect().width,objectSprite.getTextureRect().height));
                    
                }
                
                
            }
            
        }
    

    }
    
    void BattlePlayer::CheckLvlCollision(){
        
        
    }
    
    void Object::SetCharacterOrigin(sf::Sprite** object,bool center){
        
		int origin = 1;
		origin += center;
        objectSprite.setOrigin(objectSprite.getTextureRect().width/2,objectSprite.getTextureRect().height/origin);
		if(object != NULL) (*object)->setOrigin(objectSprite.getTextureRect().width / 2, objectSprite.getTextureRect().height/origin);
        
    }
    
    void Object::SetEffectOrigin(){
        
        objectSprite.setOrigin(objectSprite.getTextureRect().width/2,objectSprite.getTextureRect().height/2);
        
    } 
    
    void Object::SetShadow()
    {
		// Set the shadow size based on the actor size

        objectShadow.setTexture((World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_misc")));
        if(objectSprite.getTextureRect().width >= 90) objectShadow.setTextureRect(sf::IntRect(154, 0, 50, 15));
		
		else if (objectSprite.getTextureRect().width >= 14) objectShadow.setTextureRect(sf::IntRect(0, 0, 15, 5));
		
		else objectShadow.setTextureRect(sf::IntRect(210, 0, 6, 3));
        objectShadow.setOrigin(objectShadow.getTextureRect().width / 2,objectShadow.getTextureRect().height/2);
        objectShadow.setPosition(objectSprite.getPosition());
        
    }
    
    void Object::UpdateShadow(){
        
        objectShadow.setPosition(objectSprite.getPosition().x,objectSprite.getPosition().y + posZ);
    }
    
    void Enemy::isDamaged(int damage){
     
				if (moveType == NORMAL) vel.y = 0;
				health -= damage;
				hurt = true;
				showHurt = true;
    }

	bool Enemy::isActive() {

		return hurt;
	}
    
	void Enemy::isCollided(int var) {

		int damage = var;
		if (moveType == NORMAL) vel.y = 0;
		health -= damage;
		hurt = true;
		showHurt = true;

		if (enemyMode == 3) {

			vel.x = -vel.x;
			vel.y = -vel.y;
		}

		else {

			vel.x *= 0.25;
			vel.y *= 0.25;

		}
            
    }
    
    void Object::SetHitBox(sf::Vector2f size, int originPos){
        
        objectHitBox.setSize(size);
        objectHitBox.setFillColor(sf::Color::Yellow);
        
        // originpos specifys the origin of the hitbox to the center(0 = default), the bottom center (1)
        
        if(originPos == center) objectHitBox.setOrigin(size.x/2,size.y/2);
        else if (originPos == bottomCenter) objectHitBox.setOrigin(size.x/2,size.y);
        else if (originPos == topLeft) objectHitBox.setOrigin(0,0);

		hitBoxType = originPos;
        
    }
    
    void Object::UpdateHitBox(){
        
		//character origin - hitbox lives at bottom of sprite
        if (hitBoxType == 1) objectHitBox.setPosition(objectSprite.getPosition());

		//particle origin - hitbox lives at center of sprite
		else if (hitBoxType == 0) objectHitBox.setPosition((objectSprite.getPosition().x - objectSprite.getOrigin().x) + (objectSprite.getTextureRect().width/2), (objectSprite.getPosition().y - objectSprite.getOrigin().y) + (objectSprite.getTextureRect().height / 2));
		
		//default - hitbox lives in left/top corner of sprit
		else if (hitBoxType == 2) objectHitBox.setPosition(objectSprite.getPosition().x-objectSprite.getOrigin().x, objectSprite.getPosition().y - objectSprite.getOrigin().y);

	
	}
    
    void Object::isCollided(int var){
        
    
        
    }

	bool Object::isActive() {
	
		return active;
	}
    
    void CreateClone(sf::Sprite& sprite,std::string type, bool color){
        
        itemQueue clone;
        clone.properties["itemType"] = "SpriteClone";
        clone.properties["PosX"] = std::to_string(sprite.getPosition().x);
        clone.properties["PosY"] = std::to_string(sprite.getPosition().y);
		if (color == false) clone.properties["Color"] = "Red";
        if (type == "") clone.properties["Sprite"] = "tx_projectiles";
        else clone.properties["Sprite"] = type;
        
        clone.properties["RectX"] = std::to_string(sprite.getTextureRect().left);
        clone.properties["RectY"] = std::to_string(sprite.getTextureRect().top);
        clone.properties["RectWidth"] = std::to_string(sprite.getTextureRect().width);
        clone.properties["RectHeight"] = std::to_string(sprite.getTextureRect().height);
        clone.properties["SpriteOriginX"] = std::to_string(sprite.getOrigin().x);
        clone.properties["SpriteOriginY"] = std::to_string(sprite.getOrigin().y);
        World::GetInstance()->WorldScene.objectContainer->Queue.push_back(clone);
        
    }
    
    int GetRandDmg(int damage){
        
        std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_int_distribution<int> random(damage-2,damage+1);
        return random(mt);
        
    }
    
    bool enemyPred (const std::unique_ptr<Entity::Object>& a)
    {
        int aType = (a->type == "Enemy") ? 1 : 0;
        return aType;
    }
    
    bool enemyProjectilePred (const std::unique_ptr<Entity::Object>& a)
    {
        int aType = (a->type == "EnemyProjectile") ? 1 : 0;
        return aType;
    }
    
    bool projectilePred (const std::unique_ptr<Entity::Object>& a)
    {
        int aType = (a->type == "Projectile") ? 1 : 0;
        return aType;
    }
    
    bool propPred (const std::unique_ptr<Entity::Object>& a)
    {
        int aType = (a->type == "Prop") || (a->type == "Actor")  ? 1 : 0;
        return aType;
    }
    
    
    bool Object::PlayerDistance(int distance){
        
        //BUG?
		if (World::GetInstance()->WorldScene.playerPtr)
			return sqrt((objectShadow.getPosition().x - World::GetInstance()->WorldScene.playerPtr->objectShadow.getPosition().x)*(objectShadow.getPosition().x - World::GetInstance()->WorldScene.playerPtr->objectShadow.getPosition().x) + (objectShadow.getPosition().y - World::GetInstance()->WorldScene.playerPtr->objectShadow.getPosition().y)*(objectShadow.getPosition().y - World::GetInstance()->WorldScene.playerPtr->objectShadow.getPosition().y)) <= distance ? true : false;
		else
			return false;
        
    }


	void ObjectCall(const std::unique_ptr<Entity::Object>& a) {

	}

    
    
    
}


