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
	sf::Font MenuItem::menuFnt;
	sf::Font HitNum::hitFnt;
	int Object::center = 0;
	int Object::bottomCenter = 1;
	int Object::topLeft = 2;
	int PlayerBomb::totalBombs = 0;
	int PlayerBoomerang::totalBoomerangs = 0;

	int GUI::guiCount = 0;
	int Textbox::lineLength = 55;
	int Textbox::maxLines = 3;
	int Textbox::scriptLength = 0;
	int Textbox::progressSpeed = 0;
	int Textbox::textboxCount = 0;
	std::string Textbox::characters[11] = { "HAWZER","JIRA","DORAN","GURIAN","LIMA", "MOTHER" };

	bool PlayerMenu::menuUp = false;

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

	MenuItem::MenuItem(std::string lableName, char typeName) : GUI() {
		// hand icon
		hand.setTexture((World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_misc")));
		hand.setTextureRect(sf::IntRect(154, 15, 13, 8));
		menuLable.setFont(World::GetInstance()->WorldScene.textureContainer.GetFont());
		menuLable.setString(lableName);
		menuLable.setPosition(212, 150 + (count * 15));
		menuLable.setCharacterSize(16);
		menuLable.setColor(sf::Color(150, 150, 150));
		type = typeName;

		if (type == SWITCH) {

			objectSprite.setTexture(World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_menu_bar"));
			objectSprite.setPosition(menuLable.getPosition().x + (35), menuLable.getPosition().y + 7);
			objectSprite.setTextureRect(sf::IntRect(0, 0, 1, 10));

			if (lableName == "Volume") {

				linkedAttribute = std::weak_ptr<int>(World::GetInstance()->GlobalMembers.maxVolume);
				objectSprite.setTextureRect(sf::IntRect(0, 0, *World::GetInstance()->GlobalMembers.maxVolume, 10));
			}

			else if (lableName == "Health") {

				linkedAttribute = std::weak_ptr<int>(World::GetInstance()->GlobalMembers.health);
				objectSprite.setTextureRect(sf::IntRect(0, 0, *World::GetInstance()->GlobalMembers.health, 10));

			}

		}

		count++;

	}



	void MenuItem::Update() {



	};

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
		MenuItem item1("Start", START);
		MenuItem item2("Options", DIVE);
		MenuItem item3("Quit", EXIT);

		menuHolder.menuList.push_back(item1);
		menuHolder.menuList.push_back(item2);
		menuHolder.menuList.push_back(item3);
		menuHolder.menuList.at(0).ToggleSelection();
		menuContainer.push_back(menuHolder);



	}

	void Menu::Update() {

		//curent menu is always beginning of vector and what should be drawn

		int currentScreen = menuContainer.size() - 1;

		if (World::GetInstance()->Timer(*this, FAST)) {

			if (sf::Keyboard::isKeyPressed(World::GetInstance()->GlobalMembers.keyboardControls[controlsUp])) {

				World::GetInstance()->WorldScene.audioContainer.PlaySFX("sfx_jump2");
				if (menuContainer.at(currentScreen).currentPos != 0) {

					menuContainer.at(currentScreen).menuList.at(menuContainer.at(currentScreen).currentPos).ToggleSelection();
					--menuContainer.at(currentScreen).currentPos;
					menuContainer.at(currentScreen).menuList.at(menuContainer.at(currentScreen).currentPos).ToggleSelection();

				}

				else {

					menuContainer.at(currentScreen).menuList.at(menuContainer.at(currentScreen).currentPos).ToggleSelection();
					menuContainer.at(currentScreen).currentPos = menuContainer.at(currentScreen).menuList.size() - 1;
					menuContainer.at(currentScreen).menuList.at(menuContainer.at(currentScreen).currentPos).ToggleSelection();

				}

			}

			if (sf::Keyboard::isKeyPressed(World::GetInstance()->GlobalMembers.keyboardControls[controlsDown])) {

				World::GetInstance()->WorldScene.audioContainer.PlaySFX("sfx_jump2");
				if (menuContainer.at(currentScreen).currentPos != menuContainer.at(currentScreen).menuList.size() - 1) {

					menuContainer.at(currentScreen).menuList.at(menuContainer.at(currentScreen).currentPos).ToggleSelection();
					++menuContainer.at(currentScreen).currentPos;
					menuContainer.at(currentScreen).menuList.at(menuContainer.at(currentScreen).currentPos).ToggleSelection();

				}

				else {

					menuContainer.at(currentScreen).menuList.at(menuContainer.at(currentScreen).currentPos).ToggleSelection();
					menuContainer.at(currentScreen).currentPos = 0;
					menuContainer.at(currentScreen).menuList.at(menuContainer.at(currentScreen).currentPos).ToggleSelection();
				}

			}


			else if (sf::Keyboard::isKeyPressed(World::GetInstance()->GlobalMembers.keyboardControls[controlsLeft]) && HasSwitch(menuContainer.at(currentScreen).menuList.at(menuContainer.at(currentScreen).currentPos))) {

				--*menuContainer.at(currentScreen).menuList.at(menuContainer.at(currentScreen).currentPos).linkedAttribute.lock();
				// switch visul changes need to go here
				std::cout << *World::GetInstance()->GlobalMembers.maxVolume << std::endl;
				menuContainer.at(currentScreen).menuList.at(menuContainer.at(currentScreen).currentPos).objectSprite.setTextureRect(sf::IntRect(0, 0, *menuContainer.at(currentScreen).menuList.at(menuContainer.at(currentScreen).currentPos).linkedAttribute.lock(), 10));

			}

			else if (sf::Keyboard::isKeyPressed(World::GetInstance()->GlobalMembers.keyboardControls[controlsRight]) && HasSwitch(menuContainer.at(currentScreen).menuList.at(menuContainer.at(currentScreen).currentPos))) {

				++*menuContainer.at(currentScreen).menuList.at(menuContainer.at(currentScreen).currentPos).linkedAttribute.lock();
				// switch visul changes need to go here
				std::cout << *World::GetInstance()->GlobalMembers.maxVolume << std::endl;
				//menuContainer.at(currentScreen).menuList.at(menuContainer.at(currentScreen).currentPos).objectSprite.setTextureRect(sf::IntRect(0, 0, *menuContainer.at(currentScreen).menuList.at(menuContainer.at(currentScreen).currentPos).linkedAttribute.lock(), 10));

			}

			else if (sf::Keyboard::isKeyPressed(World::GetInstance()->GlobalMembers.keyboardControls[controlsA])) {

				DoMenuAction(menuContainer.at(currentScreen).menuList.at(menuContainer.at(currentScreen).currentPos));
				World::GetInstance()->WorldScene.audioContainer.PlaySFX("sfx_jump4");

			}

			else if ((sf::Keyboard::isKeyPressed(World::GetInstance()->GlobalMembers.keyboardControls[controlsB]) && menuContainer.size() != 1)) menuContainer.pop_back();

		}
	}

	void Menu::DoMenuAction(MenuItem& item) {

		if (item.type == START) World::GetInstance()->ReadyScene("gameScene0");


		else if (item.type == DIVE) {

			MenuItemHolder menuHolder;
			MenuItem item1("Volume", SWITCH);
			MenuItem item2("Health", SWITCH);

			menuHolder.menuList.push_back(item1);
			menuHolder.menuList.push_back(item2);
			menuHolder.menuList.at(0).ToggleSelection();
			menuContainer.push_back(menuHolder);

		}

		else if (item.type == EXIT) World::GetInstance()->windowWorld->close();
	}

	bool Menu::HasSwitch(MenuItem& item) {

		return item.type == SWITCH;

	}

	void Menu::Draw(sf::RenderTarget& window) {

		for (auto i : menuContainer.at(menuContainer.size() - 1).menuList) i.Draw(window);
		World::GetInstance()->DrawObject(objectSprite);

	}

	Menu::~Menu() {


	}

	QuakeManager::QuakeManager(){

		std::cout << "quake manager created" << std::endl;
    }

	Guide::Guide() {

		objectSprite.setTexture(World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_misc"));
		objectSprite.setTextureRect(sf::IntRect(60, 218, 24, 18));
		SetCharacterOrigin();
		Entity::GUI::guiCount--;
		World::GetInstance()->WorldScene.guidePtr = this;
	}
    
    MenuItem::~MenuItem(){
    
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

				itemQueue dirt;
				dirt.properties["PosX"] = std::to_string(World::GetInstance()->CameraTarget->objectSprite.getPosition().x + RandomNumber(100));
				dirt.properties["PosY"] = std::to_string(World::GetInstance()->CameraTarget->objectSprite.getPosition().y + RandomNumber (100));
				dirt.properties["itemType"] = "QuakeDirt";
				World::GetInstance()->WorldScene.objectContainer->Queue.push_back(dirt);


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
    
    Textbox::Textbox(int name) : GUI()
    {
		World::GetInstance()->WorldScene.UIPtr = this;

        textboxCount = 1;
            
        characterName = characters[name];
        
        script = World::GetInstance()->CharacterScripts.at(characterName + std::to_string(0));
        
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
        
        objectSprite.setTexture(World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_portraits"));
        
        portraitSize = sf::Vector2i(80,100);
        
        objectSprite.setTextureRect(sf::IntRect(0,portraitSize.y * name,portraitSize.x,portraitSize.y));
        
        boxText.setFont(World::GetInstance()->WorldScene.textureContainer.GetFont());
        boxText.setCharacterSize(16);
        
        boxName.setFont(World::GetInstance()->WorldScene.textureContainer.GetFont());
        boxName.setString(characterName);
        boxNameBG.setSize(sf::Vector2f(8 * characterName.length()+2,16));
        boxNameBG.setFillColor((sf::Color::Black));
        boxName.setCharacterSize(16);
        
        boxArrow.setTexture(World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_misc"));
        boxArrow.setTextureRect(sf::IntRect(15,0,5,5));
        
        progressSpeed = *World::GetInstance()->GlobalMembers.textSpeed.get();
        
        
    }
    
    void Textbox::Draw(sf::RenderTarget &window){
        
        World::GetInstance()->DrawObject(backDrop);
        World::GetInstance()->DrawObject(boxText);
        World::GetInstance()->DrawObject(boxNameBG);
        World::GetInstance()->DrawObject(boxName);
        World::GetInstance()->DrawObject(objectSprite);
        if(script.length() != 0 && !strncmp(&script.at(0),">",1))World::GetInstance()->DrawObject(boxArrow);
        
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
                        
                            if(World::GetInstance()->PlayerPressedButton(controlsB)){
                            
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
                    
                    else{
                                 
                        newScript.append(script,0,1);
                        script.erase(0,1);
                        World::GetInstance()->WorldScene.audioContainer.PlaySFX("sfx_txt"); 
                        boxText.setString(newScript);
                                            
                    }
                    
                }           
            
            } 
            
        }
        
        else{
            
            if(World::GetInstance()->Timer(*this,25200.0)){
                
                if(World::GetInstance()->PlayerPressedButton(controlsB)) misDestroyed = true;
				if(World::GetInstance()->WorldScene.guidePtr) World::GetInstance()->WorldScene.guidePtr->ready = false;

            }
            
        }
        
    }
    
    BattleBackground::BattleBackground(){
                
        objectSprite.setTexture(World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_battle_bg_" + std::to_string(World::GetInstance()->GlobalMembers.currentLevel)));
        objectSprite.setTextureRect(sf::IntRect (0,0,2000,2000));
        objectSprite.setOrigin(2000/2,2000/2);
        type = "BG";
        
    }
    
    Door::Door(std::string pathway) : Object() {
        
        levelPath = stoi(pathway.erase(0,pathway.find("_")+1));
        
        objectSprite.setTexture(World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_misc"));
        objectSprite.setTextureRect(sf::IntRect(0,42+(32*0),16,32));
        SetHitBox(sf::Vector2f(objectSprite.getTextureRect().width,objectSprite.getTextureRect().height),2);
        Object::type = "Prop";
        
    }

    int Door::getPath(){
        
        return levelPath;
    
    }
    
    void Door::Update(){
        
        if (World::GetInstance()->Timer(*this,FAST)){ 
            
            objectSprite.setTextureRect(sf::IntRect(16*framePos,42+(32*0),16,32));
            framePos = (framePos > 1) ? 0 : ++framePos;
            
        }
        
        
    }
    
    void Door::Draw(sf::RenderTarget &window){
        
        World::GetInstance()->DrawObject(objectSprite);
    }
    
    void Door::isCollided(int var){
        
        World::GetInstance()->GlobalMembers.currentLevel = levelPath;
        World::GetInstance()->ReadyScene("battle");
    }
    
    
    ///////////////////////
    
    Hud::Hud() : GUI (){
        
        World::GetInstance()->WorldScene.hudPtr = this;
        
		// Background hud

		objectSprite.setTexture(World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_misc"));
        weapon1.setTexture(World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_misc"));
		weapon2.setTexture(World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_misc"));

		pow1Lable.setFont(World::GetInstance()->WorldScene.textureContainer.GetFont("small"));
		pow2Lable.setFont(World::GetInstance()->WorldScene.textureContainer.GetFont("small"));
		pow1Lable.setString("pow1");
		pow2Lable.setString("pow2");
		pow1Lable.setCharacterSize(16);
		pow2Lable.setCharacterSize(16);

		objectSprite.setTextureRect(sf::IntRect(0,218, 61, 34));
		//weapon1.setTextureRect(sf::IntRect(0, 271 + (15 * World::GetInstance()->GlobalMembers.playerWeapon), 21, 15));
		//weapon2.setTextureRect(sf::IntRect(0, 271 + (15 * World::GetInstance()->GlobalMembers.playerWeapon2), 21, 15));
		//std::cout << "Hud created. pw1 = " << World::GetInstance()->GlobalMembers.playerWeapon << ", pw2 = " << World::GetInstance()->GlobalMembers.playerWeapon2 << std::endl;

		weapon1slot.at(0) = 0;
		weapon1slot.at(1) = 23;
		weapon2slot.at(0) = 23;
		weapon2slot.at(1) = 0;

		
		if (switching > 1) switching = 1;
		objectSprite.setPosition(World::GetInstance()->viewPos.x - 230, World::GetInstance()->viewPos.y - 125);
		weapon1.setPosition(World::GetInstance()->viewPos.x - 224 + weapon1slot.at(switching), World::GetInstance()->viewPos.y - 111);
		weapon2.setPosition(World::GetInstance()->viewPos.x - 224 + weapon2slot.at(switching), World::GetInstance()->viewPos.y - 111);

    }

    
    void Hud::Update(){

		pow1Lable.setPosition((World::GetInstance()->viewPos.x) - 222, World::GetInstance()->viewPos.y - 134);
		pow2Lable.setPosition((World::GetInstance()->viewPos.x) - 222 + 23, World::GetInstance()->viewPos.y - 134);


		objectSprite.setPosition(World::GetInstance()->viewPos.x - 230, World::GetInstance()->viewPos.y - 125);


		newPos.x += ((weapon1slot.at(switching)) - (newPos.x)) / 5;
		newPos2.x += ((weapon2slot.at(switching)) - (newPos2.x)) / 5;

		weapon1.setPosition(((World::GetInstance()->viewPos.x) - 222) + newPos.x, objectSprite.getPosition().y + 10);
		weapon2.setPosition(((World::GetInstance()->viewPos.x) - 222) + newPos2.x, objectSprite.getPosition().y + 10);

    }

	void Guide::Update() {

		//sf::Vector2f newPos(objectSprite.getPosition().x, objectSprite.getPosition().y);

		//newPos.y += ((targetPosition.y - 5) - objectSprite.getPosition().y) / 25;

		//objectSprite.setPosition(targetPosition.x, newPos.y-10);

		if (ready == false) {

			if (World::GetInstance()->Timer(*this, VERY_SLOW*2)) {

				ready = true;

			}
		}

		if (World::GetInstance()->Timer(*this, VERY_SLOW)) {

			if (objectSprite.getTextureRect().left != 108) {

				objectSprite.setTextureRect(sf::IntRect(objectSprite.getTextureRect().left + 24, 218, 24, 18));

			}

			else objectSprite.setTextureRect(sf::IntRect(60, 218, 24, 18));

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

		if (hidden == false && World::GetInstance()->IsPlayerCameraTarget()) {

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
			textbox.properties["ActorName"] = std::to_string(5);
			World::GetInstance()->WorldScene.objectContainer->Queue.push_back(textbox);
			textboxCreated = true;

		}

		else if (textboxCreated == true && World::GetInstance()->WorldScene.UIPtr == nullptr && sceneDone == false) {

			World::GetInstance()->ReadyScene("map2_1");
			sceneDone = true;

		}
	}

    void Hud::Draw(sf::RenderTarget& window){

        /*
		World::GetInstance()->DrawObject(objectSprite);


		if (switching == true) {

			World::GetInstance()->DrawObject(weapon2);
			World::GetInstance()->DrawObject(weapon1);


		}

		if (switching == false) {

			World::GetInstance()->DrawObject(weapon1);
			World::GetInstance()->DrawObject(weapon2);


		}

		World::GetInstance()->DrawObject(pow1Lable);
		World::GetInstance()->DrawObject(pow2Lable);
		*/
        
    }


    
    void BattleBackground::Draw(sf::RenderTarget& window){
        
        World::GetInstance()->DrawObject(objectSprite,"waveShader");
        
    }


    
    Hud::~Hud(){
        
		World::GetInstance()->WorldScene.hudPtr = NULL;

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
        
        std::cout << "Player created" << std::endl;
        objectSprite.setTexture(World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_player"));
        objectSprite.setTextureRect(sf::IntRect(0, south, 13, 24));
        SetCharacterOrigin();
        spriteHeight = objectSprite.getTextureRect().height;
        spriteWidth = objectSprite.getTextureRect().width;
        playerHealth = World::GetInstance()->GlobalMembers.playerMaxHP;
        playerPow = World::GetInstance()->GlobalMembers.playerMaxMP;
        World::GetInstance()->WorldScene.playerPtr = this;
        SetShadow();
        type = "Player";
        scaleTemp = sf::Vector2f(0.12,0);
        SetHitBox(sf::Vector2f(6,6),0);
        dead = false;
        if(!World::GetInstance()->CameraTarget) World::GetInstance()->SetCameraTarget(*this);

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
    
    BattlePlayer::BattlePlayer(){
        
        hotSpot.x = objectSprite.getPosition().x + 6;
        hotSpot.y = objectSprite.getPosition().y + 12;
		sshield.setTexture(World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_misc"));
		sshield.setTextureRect(sf::IntRect(103, 0, 17, 41));
		sshield.setColor(sf::Color::Color(255, 255, 255, 50));

		itemQueue bud;
		bud.properties["PosX"] = std::to_string(objectSprite.getPosition().x);
		bud.properties["PosY"] = std::to_string(objectSprite.getPosition().y - 10);
		bud.properties["itemType"] = "Buddy";


		if (World::GetInstance()->GlobalMembers.weapons[5] > 0) {

			bud.properties["BuddyPos"] = std::to_string(0);
			World::GetInstance()->WorldScene.objectContainer->Queue.push_back(bud);

		}

		if (World::GetInstance()->GlobalMembers.weapons[3] > 0) {

			bud.properties["BuddyPos"] = std::to_string(1);
			World::GetInstance()->WorldScene.objectContainer->Queue.push_back(bud);
		}

		if (World::GetInstance()->GlobalMembers.weapons[6] > 0) {

			bud.properties["BuddyPos"] = std::to_string(2);
			World::GetInstance()->WorldScene.objectContainer->Queue.push_back(bud);

		}

		if (World::GetInstance()->GlobalMembers.weapons[2] > 0) {

			bud.properties["BuddyPos"] = std::to_string(3);
			World::GetInstance()->WorldScene.objectContainer->Queue.push_back(bud);

		}

		if (World::GetInstance()->GlobalMembers.weapons[6] > 0) {

			bud.properties["BuddyPos"] = std::to_string(4);
			World::GetInstance()->WorldScene.objectContainer->Queue.push_back(bud);

		}

		if (World::GetInstance()->GlobalMembers.weapons[3] > 0) {

			bud.properties["BuddyPos"] = std::to_string(5);
			World::GetInstance()->WorldScene.objectContainer->Queue.push_back(bud);

		}

		if (World::GetInstance()->GlobalMembers.weapons[5] > 0) {

			bud.properties["BuddyPos"] = std::to_string(6);
			World::GetInstance()->WorldScene.objectContainer->Queue.push_back(bud);

		}
    }
    
	void BattlePlayer::Update() {

		if (dashing == false) {

			if (!World::GetInstance()->WorldScene.transition) {
			

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

			}

			if (!World::GetInstance()->PlayerPressedButton(controlsRight)
				&& !World::GetInstance()->PlayerPressedButton(controlsUp)
				&& !World::GetInstance()->PlayerPressedButton(controlsDown)
				&& !World::GetInstance()->PlayerPressedButton(controlsLeft))
				movement = idle;

		}

		// Animation

		if (World::GetInstance()->IsPlayerActive() && World::GetInstance()->PlayerPressedButton(controlsA)) {

			if (World::GetInstance()->Timer(*this, 160000.0)) {

				World::GetInstance()->WorldScene.hudPtr->switching = !World::GetInstance()->WorldScene.hudPtr->switching;
				World::GetInstance()->WorldScene.audioContainer.PlaySFX("sfx_jump4");

				//int tempWeap = World::GetInstance()->GlobalMembers.playerWeapon;
				//World::GetInstance()->GlobalMembers.playerWeapon = World::GetInstance()->GlobalMembers.playerWeapon2;
				//World::GetInstance()->GlobalMembers.playerWeapon2 = tempWeap;

			}

		}


		if (World::GetInstance()->IsPlayerActive() && dashing == false && movement != idle &&  World::GetInstance()->PlayerPressedButton(controlsC)) {

			vel.y = 6;
			vel.x = 0;
			dashing = true;
			RotateVector(vel, 45 * movement);
			itemQueue dash;
			dash.properties["PosX"] = std::to_string(objectSprite.getPosition().x);
			dash.properties["PosY"] = std::to_string(objectSprite.getPosition().y);
			dash.properties["itemType"] = "DashEffect";
			World::GetInstance()->WorldScene.objectContainer->Queue.push_back(dash);

			if (shield == 2) {

				itemQueue particles;
				particles.properties["PosX"] = std::to_string(objectSprite.getPosition().x);
				particles.properties["PosY"] = std::to_string(objectSprite.getPosition().y - 10);
				particles.properties["itemType"] = "BlockedWave";
				World::GetInstance()->WorldScene.objectContainer->Queue.push_back(particles);

			}

		}

		if (dashing == true) {

			if (World::GetInstance()->Timer(*this, NORMAL, NODELAY)) {

				itemQueue dash;
				dash.properties["PosX"] = std::to_string(objectSprite.getPosition().x);
				dash.properties["PosY"] = std::to_string(objectSprite.getPosition().y);
				dash.properties["itemType"] = "SlideEffect";
				World::GetInstance()->WorldScene.objectContainer->Queue.push_back(dash);
			}

			if (World::GetInstance()->Timer(*this, 140000.0)) {

				vel.x *= 0.3;
				vel.y *= 0.3;

			}

			if (World::GetInstance()->Timer(*this, 510000.0)) {

				dashing = false;

			}

			//std::cout << vel.x << ", " << vel.y << std::endl;

		}


		else if (movement != idle) {

			vel.y = 2;
			vel.x = 0;

			if (World::GetInstance()->Timer(*this, FAST, NODELAY)) {

				frame_pos = (frame_pos >= 5) ? 0 : frame_pos + 1;

				if (!World::GetInstance()->PlayerPressedButton(controlsB)) {

					objectSprite.setTextureRect(sf::IntRect(frame_pos * spriteWidth, movement * FRAME, spriteWidth, spriteHeight));
					fireDir = movement;

				}

				else objectSprite.setTextureRect(sf::IntRect(frame_pos * spriteWidth, objectSprite.getTextureRect().top, spriteWidth, spriteHeight));

			}


		}

		else if (movement == idle) {

			objectSprite.setTextureRect(sf::IntRect(0, objectSprite.getTextureRect().top, spriteWidth, spriteHeight));

		}

		if (World::GetInstance()->Timer(*this, 140000.0, NODELAY) && dashing == false) {

			vel.x *= 0.25;
			vel.y *= 0.25;

		}

		// rotates speed in the correct direction

		if (dashing == false) RotateVector(vel, 45 * movement);

		/*

		Firing:
		Here we check which abilities we have at World::Attributes::weapons, what level they are and fire the apprproiate projectiles


		weapons[0] = pulse (default)

		Boss upgrades:
		weapons[1] = spreader
		weapons[2] = lance
		weapons[3] = rail
		weapons[4] = buster
		weapons[5] = homing lightning
		weapons[6] = bomb

		every weapon has 6 levels, except pulse, having 7

		*/

		if (World::GetInstance()->IsPlayerActive() && World::GetInstance()->PlayerPressedButton(controlsB)) {

			itemQueue proj;
			proj.properties["PosX"] = std::to_string(hotSpot.x);
			proj.properties["PosY"] = std::to_string(hotSpot.y);
			proj.properties["Direction"] = std::to_string(fireDir);
			proj.properties["VelX"] = std::to_string(vel.x);
			proj.properties["VelY"] = std::to_string(vel.y);

			// repeater

			if (World::GetInstance()->GlobalMembers.weapons[0] >= 1 && World::GetInstance()->Timer(*this, SLOW, NODELAY)) {

				if (World::GetInstance()->GlobalMembers.weapons[0] > 1) proj.properties["itemType"] = "PlayerLaser" + std::to_string(World::GetInstance()->GlobalMembers.weapons[0]);
				else proj.properties["itemType"] = "PlayerLaser";
				World::GetInstance()->WorldScene.audioContainer.PlaySFX("sfx_laser9");
				World::GetInstance()->WorldScene.objectContainer->Queue.push_back(proj);


			}

			// cannon

			else if (World::GetInstance()->GlobalMembers.weapons[6] >= 1) {


				if (PlayerBomb::totalBombs == 0) {

					if (World::GetInstance()->Timer(*this, VERY_SLOW)) {

						itemQueue bullet;
						bullet.properties["PosX"] = std::to_string(hotSpot.x);
						bullet.properties["PosY"] = std::to_string(hotSpot.y);
						bullet.properties["Direction"] = std::to_string(fireDir);
						bullet.properties["VelX"] = std::to_string(vel.x);
						bullet.properties["VelY"] = std::to_string(vel.y);
						bullet.properties["itemType"] = "PlayerBomb";
						World::GetInstance()->WorldScene.audioContainer.PlaySFX("sfx_bomb_shoot");
						World::GetInstance()->WorldScene.objectContainer->Queue.push_back(bullet);

					}

				}

				else if (PlayerBomb::totalBombs == 1) {

					if (World::GetInstance()->Timer(*this, NORMAL)) PlayerBomb::totalBombs = 2;

				}

			}


			// rail

			else if (World::GetInstance()->GlobalMembers.weapons[3] >= 1 && World::GetInstance()->Timer(*this, VERY_SLOW*16, NODELAY)) {

				if (World::GetInstance()->GlobalMembers.weapons[0] > 1) proj.properties["itemType"] = "PlayerBeam" + std::to_string(World::GetInstance()->GlobalMembers.weapons[5]);
				else proj.properties["itemType"] = "PlayerBeam";

				proj.properties["PosX"] = std::to_string(BuddyAB->objectSprite.getPosition().x);
				proj.properties["PosY"] = std::to_string(BuddyAB->objectSprite.getPosition().y);
				World::GetInstance()->WorldScene.objectContainer->Queue.push_back(proj);

				proj.properties["PosX"] = std::to_string(BuddyCB->objectSprite.getPosition().x);
				proj.properties["PosY"] = std::to_string(BuddyCB->objectSprite.getPosition().y);
				World::GetInstance()->WorldScene.objectContainer->Queue.push_back(proj);

			}

			// spreader

			else if (World::GetInstance()->GlobalMembers.weapons[5] >= 1 && World::GetInstance()->Timer(*this, VERY_SLOW, NODELAY)) {

				if (World::GetInstance()->GlobalMembers.weapons[0] > 1) proj.properties["itemType"] = "PlayerRepeater" + std::to_string(World::GetInstance()->GlobalMembers.weapons[5]);
				else proj.properties["itemType"] = "PlayerRepeater";
				proj.properties["PosX"] = std::to_string(BuddyA->objectSprite.getPosition().x);
				proj.properties["PosY"] = std::to_string(BuddyA->objectSprite.getPosition().y);
				proj.properties["Direction"] = std::to_string(fireDir + 1);
				World::GetInstance()->WorldScene.objectContainer->Queue.push_back(proj);

				proj.properties["PosX"] = std::to_string(BuddyD->objectSprite.getPosition().x);
				proj.properties["PosY"] = std::to_string(BuddyD->objectSprite.getPosition().y);
				proj.properties["Direction"] = std::to_string(fireDir - 1);
				World::GetInstance()->WorldScene.objectContainer->Queue.push_back(proj);


			}

			// boomerang


			else if (World::GetInstance()->GlobalMembers.weapons[5] == 5 && World::GetInstance()->Timer(*this, VERY_SLOW * 2, NODELAY)) {

				itemQueue bullet;
				bullet.properties["PosX"] = std::to_string(hotSpot.x);
				bullet.properties["PosY"] = std::to_string(hotSpot.y);
				bullet.properties["Direction"] = std::to_string(fireDir);
				bullet.properties["VelX"] = std::to_string(vel.x);
				bullet.properties["VelY"] = std::to_string(vel.y);
				bullet.properties["itemType"] = "PlayerBoomerang";
				World::GetInstance()->WorldScene.objectContainer->Queue.push_back(bullet);

				World::GetInstance()->WorldScene.audioContainer.PlaySFX("sfx_bomb_shoot");

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

			if (World::GetInstance()->Timer(*this, VERY_SLOW*2)) shield += 1;
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
				
		objectSprite.move(vel.x, vel.y);
		hotSpot.x = objectSprite.getPosition().x;
		hotSpot.y = objectSprite.getPosition().y - objectSprite.getTextureRect().height / 3;
		UpdateShadow();

		*posXtemp1 = objectSprite.getPosition().x;
		*posYtemp2 = objectSprite.getPosition().y;

		sshield.setPosition(objectSprite.getPosition().x - 8, (objectSprite.getPosition().y - 32)+41-shield);

	}

	void Player::Draw(sf::RenderTarget& window) {
		
			World::GetInstance()->DrawObject(objectSprite);
			if(shield != 0 ) World::GetInstance()->DrawObject(sshield);


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
		//World::GetInstance()->WorldScene.playerPtr = nullptr;

		BuddyA = NULL;
		BuddyAB = NULL;
		BuddyB = NULL;
		BuddyBB = NULL;
		BuddyC = NULL;
		BuddyCB = NULL;
		BuddyD = NULL;

		delete BuddyA;
		delete BuddyAB;
		delete BuddyB;
		delete BuddyBB;
		delete BuddyC;
		delete BuddyCB;
		delete BuddyD;

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
            assetHeight = tempSize.y/8;
            
        }
        
        if(spriteRotation != 2) spriteRotation++;
        else spriteRotation = 0;
        
    }
    
    SpriteClone::SpriteClone() : Fixed(){
        
		objectSprite.setColor(sf::Color::Blue);
        velZ = -99;
        SetEffectOrigin();
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

		objectSprite.setTextureRect(sf::IntRect(55, 105, 18, 29));
		RotateVector(vel, RandomNumber(180));
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

	EnemyPart::~EnemyPart() {

	}

	PlayerBirth::PlayerBirth() : Fixed() {



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
		int r = RandomNumber(1, 0);
		if(r==0) RotateVector(vel, RandomNumber(180));
		if (r == 1) RotateVector(vel, RandomNumber(-180));

		maxFrame = 8;
		animSpeed = VERY_FAST;
		deacceleration = 0.5;

	}

	HauzerCharge::HauzerCharge() : Fixed() {

		objectSprite.setTextureRect(sf::IntRect(0, 241, 50, 50));
		SetEffectOrigin();
		animSpeed = VERY_FAST;
		maxTime = 6000000.0;
		maxFrame = 8;

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
        
    }

	ShieldEffect::ShieldEffect() : Fixed() {

		objectSprite.setTextureRect(sf::IntRect(25, 143, 17, 41));
		SetEffectOrigin();
		maxFrame = 7;
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
		maxFrame = 6;
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
		acceleration = 0.15;
		emitter = "EnemySpark";
		emitTime = SLOW;
		emitScatter = true;
		emitCount = 3;
		maxFrame = 1;

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
    
    float Object::GetYPosition(){
        
        return objectSprite.getPosition().y;
        
    }
    
    sf::IntRect Object::GetObjectBounds(){
        
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
    
    PlayerLaser::PlayerLaser() : Projectile()
    {
        
        objectSprite.setTexture((World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_projectiles")));
        objectSprite.setTextureRect(sf::IntRect(0, 178, 5, 19));
        objectSprite.setOrigin(5,5);
        SetEffectOrigin();
        vel.y = 5;
		damage = 7;
		maxFrame = 2;
        SetHitBox(sf::Vector2f(4,4));
        
    }

	PlayerLaser2::PlayerLaser2() : PlayerLaser()
	{
		objectSprite.setTexture((World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_projectiles")));
		objectSprite.setTextureRect(sf::IntRect(0, 197, 6, 27));
		objectSprite.setOrigin(7, 10);
		SetEffectOrigin();
		vel.y = 5;
		damage = 7;
		SetHitBox(sf::Vector2f(6,6));
		health = 1;

	}
    
    PlayerBoomerang::PlayerBoomerang() : Projectile(){
        
        objectSprite.setTexture((World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_projectiles")));
        objectSprite.setTextureRect(sf::IntRect(0, 32, 20, 20));
        objectSprite.setOrigin(10,10);
        SetEffectOrigin();
        vel.y = 4;
        damage = 4;
        SetHitBox(sf::Vector2f(12,12));
		destroyOnImpact = false;
        totalBoomerangs++;
        
        
    }
    
    PlayerRepeater::PlayerRepeater() : Projectile()
    {
        
        objectSprite.setTexture((World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_projectiles")));
        objectSprite.setTextureRect(sf::IntRect(0, 224, 5, 4));
        objectSprite.setOrigin(2.5,2);
        SetEffectOrigin();
        vel.y = 3;
        damage = 3;
        maxFrame = 2;
        SetHitBox(sf::Vector2f(3,3));
    
    }

	PlayerRepeater2::PlayerRepeater2() : Projectile()
	{

		objectSprite.setTexture((World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_projectiles")));
		objectSprite.setTextureRect(sf::IntRect(0, 228, 5, 7));
		objectSprite.setOrigin(2.5, 2);
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
        objectSprite.setTextureRect(sf::IntRect(0, 0, 80, 80));
        SetEffectOrigin();
        vel.y = 0;
        damage = 20;
        SetHitBox(sf::Vector2f(70,70));
		destroyOnImpact = false;
        
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
    
    
    void Object::Update()
    {
        
        
        
    }

	void PlayerBirth::Update() {


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
                
               else if(maxTime != 0) frame = 0;
    
               else misDestroyed = true;
            
                if((maxTime != 0) && (World::GetInstance()->Timer(*this,maxTime))) misDestroyed = true;
        
            
        }
        
        objectSprite.move(vel.x,vel.y);

    }
    
    void SpriteClone::Update(){
        
        if(World::GetInstance()->Timer(*this,FAST,NODELAY)){
            
            if(objectSprite.getColor().a - 30 <= 0) misDestroyed = true;
            
            else objectSprite.setColor(sf::Color::Color(objectSprite.getColor().r, objectSprite.getColor().g, objectSprite.getColor().b,objectSprite.getColor().a - 75));
            
        }
    
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

		if (emitter != "") {

			if (World::GetInstance()->Timer(*this, emitTime)) {

				itemQueue particles;

				for (int i = 0; i < emitCount; i++) {

					if (emitScatter == true) {

						particles.properties["PosX"] = std::to_string(objectSprite.getPosition().x - 8 + RandomNumber(16));
						particles.properties["PosY"] = std::to_string(objectSprite.getPosition().y - 8 + RandomNumber(16));

					}

					else {

						particles.properties["PosX"] = std::to_string(objectSprite.getPosition().x);
						particles.properties["PosY"] = std::to_string(objectSprite.getPosition().y);

					}

					particles.properties["itemType"] = emitter;
					World::GetInstance()->WorldScene.objectContainer->Queue.push_back(particles);

				}

			}

		}


        objectSprite.move(vel.x,vel.y);
        
        
        if(objectSprite.getPosition().x >= World::GetInstance()->WorldScene.levelContainer->lvlSize.x || objectSprite.getPosition().x <= 0)
        {
            
            isCollided = true;
            
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

		float angle = GetAngle(objectSprite.getPosition(), World::GetInstance()->WorldScene.playerPtr->objectSprite.getPosition());
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
        
        if(World::GetInstance()->Timer(*this,1000000.0)) misDestroyed = true;
        
        if(World::GetInstance()->Timer(*this,FAST)) CreateClone(objectSprite);
        
    }

	void PlayerBoomerang::HasCollided(const std::unique_ptr<Entity::Object>& a) {

		objectSprite.move(-vel.x,-vel.y);

		Entity::Object* obj = a.get();
		Entity::Enemy* pobj = dynamic_cast<Entity::Enemy*>(obj);

		if (pobj->active == true) {

			if (a->velZ >= -3) {

				if (World::GetInstance()->Timer(this, NORMAL, NODELAY)) {

					a->isCollided(damage);
					misDestroyed = destroyOnImpact;

					if (pobj->defending == false) {

						itemQueue particles;
						particles.properties["PosX"] = std::to_string(objectSprite.getPosition().x);
						particles.properties["PosY"] = std::to_string(objectSprite.getPosition().y);
						particles.properties["itemType"] = "DamageSpark";
						World::GetInstance()->WorldScene.objectContainer->Queue.push_back(particles);

						particles.properties["itemType"] = "Spark";
						World::GetInstance()->WorldScene.objectContainer->Queue.push_back(particles);
						World::GetInstance()->WorldScene.objectContainer->Queue.push_back(particles);
						World::GetInstance()->WorldScene.objectContainer->Queue.push_back(particles);

						World::GetInstance()->WorldScene.audioContainer.PlaySFX("sfx_badhit2");

					}

					if (pobj->defending == true) {

						itemQueue particles;
						particles.properties["PosX"] = std::to_string(objectSprite.getPosition().x);
						particles.properties["PosY"] = std::to_string(objectSprite.getPosition().y);
						particles.properties["itemType"] = "ShieldEffect";
						World::GetInstance()->WorldScene.objectContainer->Queue.push_back(particles);

						World::GetInstance()->WorldScene.audioContainer.PlaySFX("sfx_block");


					}


				}

			}
		}

	}
   

    
    void PlayerBombExp::Update(){

        misDestroyed = true;
    }
    
    void PlayerLaser::isCollided(int var){
        
        misDestroyed = true;
        
    }
    
    void PlayerRepeater::isCollided(int var){
        
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
        
            if(World::GetInstance()->Timer(*this,animSpd,NODELAY)) frame = (frame == maxFrame) ? 0 : frame+1;
        
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

		Entity::Object* obj = a.get();
		Entity::Enemy* pobj = dynamic_cast<Entity::Enemy*>(obj);

		if (pobj->active == true) {

			if (a->velZ >= -3) {

				if (World::GetInstance()->Timer(this, NORMAL, NODELAY)) {

					a->isCollided(damage);

					if (health > 0) {
						if (health <= 0) misDestroyed = true;

						health--;
					}

					else misDestroyed = destroyOnImpact;

					if (pobj->defending == false) {

						itemQueue particles;
						particles.properties["PosX"] = std::to_string(objectSprite.getPosition().x);
						particles.properties["PosY"] = std::to_string(objectSprite.getPosition().y);
						particles.properties["itemType"] = "DamageSpark";
						World::GetInstance()->WorldScene.objectContainer->Queue.push_back(particles);

						particles.properties["itemType"] = "Spark";
						World::GetInstance()->WorldScene.objectContainer->Queue.push_back(particles);
						World::GetInstance()->WorldScene.objectContainer->Queue.push_back(particles);
						World::GetInstance()->WorldScene.objectContainer->Queue.push_back(particles);

						World::GetInstance()->WorldScene.audioContainer.PlaySFX("sfx_badhit2");

					}

					if (pobj->defending == true) {

						itemQueue particles;
						particles.properties["PosX"] = std::to_string(objectSprite.getPosition().x);
						particles.properties["PosY"] = std::to_string(objectSprite.getPosition().y);
						particles.properties["itemType"] = "ShieldEffect";
						World::GetInstance()->WorldScene.objectContainer->Queue.push_back(particles);

						World::GetInstance()->WorldScene.audioContainer.PlaySFX("sfx_block");


					}


				}

			}
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
        
        World::GetInstance()->DrawObject(objectShadow);
        
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

	QuakeDirt::~QuakeDirt() {

	}

	EnemyLaser::~EnemyLaser() {

	}

	EnemyChargeParticle::~EnemyChargeParticle() {

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

	HauzerSmog::~HauzerSmog() {

	}

	HauzerSpear::~HauzerSpear() {

	}

	DashEffect::~DashEffect() {

	}

	SlideEffect::~SlideEffect() {

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
    
    PlayerLaser::~PlayerLaser()
    {
        
    }

	PlayerLaser2::~PlayerLaser2()
	{

	}
    
    PlayerRepeater::~PlayerRepeater()
    {
        
    }

	PlayerRepeater2::~PlayerRepeater2()
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
            bomb.properties["itemType"] = "Explosion";
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
    
    bool Object::isDestroyed(){
        
        if((objectSprite.getPosition().x < 0 || objectSprite.getPosition().x > World::GetInstance()->WorldScene.levelContainer->lvlSize.x || objectSprite.getPosition().y < 0 || objectSprite.getPosition().y > World::GetInstance()->WorldScene.levelContainer->lvlSize.y) && type != "GUI") misDestroyed = true;
        
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
    
    LevelManager::LevelManager() : Object(){
        
        objectSprite.setTextureRect(sf::IntRect(0,0,0,0));

		std::string level = std::to_string(World::GetInstance()->GlobalMembers.currentLevel);
    
        bg.setTexture(World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_battle_bg_" + level + ""));
        bg.setTextureRect(sf::IntRect (0,0,2000,2000));
        bg.setOrigin(2000/2,2000/2);

        if(enemyList[0] == ""){
            
            enemyList[0] = "Djinn";
            enemyList[1] = "Squid";
            enemyList[2] = "Slime";
            enemyList[3] = "Mask";
            enemyList[4] = "Spore";
            enemyList[5] = "Roach";
            enemyList[6] = "";
            enemyList[7] = "";
            enemyList[8] = "";
            enemyList[9] = "Mozza";
            enemyList[10] = "Mozza";
            enemyList[11] = "Mozza";
            enemyList[12] = "Mozza";
            enemyList[13] = "Mozza";
            enemyList[14] = "Mozza";
            enemyList[15] = "Mozza";
            enemyList[16] = "Mozza";
            enemyList[17] = "Mozza";
            enemyList[18] = "Mozza";
            
            //bosses start at 19
            
            enemyList[19] = "Mozza";
            enemyList[20] = "Hauzer";
            enemyList[21] = "Lim";
            enemyList[22] = "Boss4";
            enemyList[23] = "Boss5";
            enemyList[24] = "Boss6";
            enemyList[25] = "Boss7";
            
        }
        
        type = "BG";

		//lvlEnemyBank[0] = 4; 
		//lvlEnemyBank[1] = 4;
		//lvlEnemyBank[2] = 32;

		lvlEnemyBank[0] = 4;
		lvlEnemyBank[1] = 16;
		lvlEnemyBank[2] = 100;

    }

	void LevelManager::CreateEnemy(int enemy) {

		int lvlwidth = World::GetInstance()->WorldScene.levelContainer->lvlSize.x;
		int lvlheight = World::GetInstance()->WorldScene.levelContainer->lvlSize.y;

		std::random_device rd;
		std::mt19937 mt(rd());
		std::uniform_int_distribution<int> randomLeftwidth(0, lvlwidth);
		std::uniform_int_distribution<int> randomTopheight(0, lvlheight);

		Entity::itemQueue ienemy;
		ienemy.properties["itemType"] = enemyList[3 * World::GetInstance()->GlobalMembers.currentLevel + enemy];
		ienemy.properties["PosX"] = std::to_string(randomLeftwidth(mt));
		ienemy.properties["PosY"] = std::to_string(randomTopheight(mt));
		World::GetInstance()->WorldScene.objectContainer->Queue.push_back(ienemy);
		lvlEnemyBank[enemy]--;

	}

    void LevelManager::Update(){
       
		if (World::GetInstance()->Timer(*this, 5000000.0)) {


			std::cout << lvlEnemyBank[0] << ".. " << lvlEnemyBank[1] << ".. " << lvlEnemyBank[2] << std::endl;


		}
			

        if(lvlEnemyBank[0] != 0 || lvlEnemyBank[1] != 0 || lvlEnemyBank[2] != 0){
        
			// need to figure out the correct intervals to spawn enemies
			// big enemies should take a while to respond
			// mid and low enemies should be intervals of big enemies.
			// we should figure out how to end up with a big enemy spawned before all the smaller enemies 
			// (i.e., smaller enemies should still be spawning after LAST big enemy is spawned

			if (lvlEnemyBank[0] != 0 && World::GetInstance()->Timer(*this, 20000000.0,NODELAY)) CreateEnemy(0);
			if (lvlEnemyBank[1] != 0 && World::GetInstance()->Timer(*this, 5000000.0, NODELAY)) CreateEnemy(1);
			if (lvlEnemyBank[2] != 0 && World::GetInstance()->Timer(*this, 600000.0, NODELAY)) CreateEnemy(2);
           
        }
        
        else if( int(bg.getColor().a) != 0 ){
            
            if(World::GetInstance()->Timer(*this,FAST,NODELAY)){
                
                if(int(bg.getColor().a)-2 < 0) bg.setColor(sf::Color(255,255,255,0));     
                else bg.setColor(sf::Color(255,255,255,int(bg.getColor().a)-2));
                World::GetInstance()->WorldScene.audioContainer.music.setVolume(World::GetInstance()->WorldScene.audioContainer.music.getVolume()-0.5);
                
            }
            
        }
        
        else{
            
            misDestroyed = true;
            Entity::itemQueue boss;
            boss.properties["PosX"] = std::to_string(600/2);
            boss.properties["PosY"] = std::to_string(600/2);
            boss.properties["itemType"] = enemyList[19 + World::GetInstance()->GlobalMembers.currentLevel];;
            //std::cout << "enemys left: " << maxEnemies << std:: endl;
            World::GetInstance()->WorldScene.objectContainer->Queue.push_back(boss);
            World::GetInstance()->WorldScene.audioContainer.PlayMusic("boss");
                
        }
        
        bg.rotate(0.05);

    }
    
    void LevelManager::Draw(sf::RenderTarget &window){
        
        World::GetInstance()->DrawObject(bg,"waveShader");
    }
    
    LevelManager::~LevelManager(){
        
    }
    
    Actor::Actor(int actor) : character(actor), Object()
    {
        
        objectSprite.setTexture(World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_actors"));
        objectSprite.setTextureRect(sf::IntRect ((1 + actor) * 16,0 * 24,16,24));
        name = actor;
        SetCharacterOrigin();
        SetShadow();
        type = "Actor";
        SetHitBox(sf::Vector2f(100,100),0);
        
    }
    
    void Actor::Update(){
        
        Move();
        UpdateShadow();
    }
    
    void Actor::Move()
    {
        if(PlayerDistance(CLOSE)){
            actorMovement = RoundUp((GetAngle(objectShadow.getPosition(),World::GetInstance()->WorldScene.playerPtr->objectShadow.getPosition())),45);
        objectSprite.setTextureRect(sf::IntRect((1+name) * 16, abs(((180 - actorMovement) / 45) * 24),16, 24));
        }
        
        else if(idleBehavior == none) objectSprite.setTextureRect(sf::IntRect((1+name) * 16, south,16, 24));
        
        else if(idleBehavior == looking){
            
            if(World::GetInstance()->Timer(*this,VERY_SLOW*10,DELAY)) objectSprite.setTextureRect(sf::IntRect((1+name) * 16, RandomNumber(7)*24,16, 24));
        
            }
        
    }
    
    void Actor::isCollided(int var){

            if(Textbox::textboxCount == 0){
                
                if(World::GetInstance()->Timer(*this,NORMAL)){

                    itemQueue textbox;
                    textbox.properties["itemType"] = "Textbox";
                    textbox.properties["ActorName"] = std::to_string(character);
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
        SetHitBox(sf::Vector2f(16,16),1);
		hotSpot.x = 0;
		hotSpot.y = 0;
		AssignedAttack = 0;


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
    }

	Spore::Spore() : Enemy()
	{
		objectSprite.setTextureRect(sf::IntRect(78, 3, 16, 17));
		SetHitBox(sf::Vector2f(10, 10), 1);
		SetCharacterOrigin();
		SetShadow();
		health = 5;
		AssignedBehavior = &Enemy::RandomPosition;
		AssignedMovement = &Enemy::MoveWalk;
		flatAnimation = true;
		speed = 1.5;
	}

	Buddy::Buddy() : Object()
	{
		std::cout << "buddy created" << std::endl;
		objectSprite.setTexture(World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_misc"));
		objectSprite.setTextureRect(sf::IntRect(204, 0, 4, 8));
		SetHitBox(sf::Vector2f(5, 5));
		SetCharacterOrigin();
		SetShadow();

	}


	Roach::Roach() : Enemy()
	{
		objectSprite.setTextureRect(sf::IntRect(0, 117, 37, 16));
		SetCharacterOrigin();
		SetShadow();
		health = 10;
		AssignedBehavior = &Enemy::FollowPlayer;
		AssignedMovement = &Enemy::MoveSlide;
		animationSpeed = VERY_FAST;

		// For roaches, speed is the DISTANCE the enemy will move

		speed = 30;

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
		SetHitBox(sf::Vector2f(45, 70), 1);

		//Set child sprite(wings)
		wings.setTexture(World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_enemies"));
		wings.setTextureRect(sf::IntRect(243, 0, 134, 44));

		// For roaches, speed is the DISTANCE the enemy will move

		speed = 0.25;

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

	}

	Mask::Mask() : Enemy()
	{
		objectSprite.setTextureRect(sf::IntRect(36, 13, 37, 45));
		SetCharacterOrigin();
		SetShadow();
		speed = 0.5;
		health = 30;
		flatAnimation = true;
		animationSpeed = VERY_SLOW*2;
		AssignedBehavior = &Enemy::FollowPlayer;
		AssignedMovement = &Enemy::MoveWalk;
		AssignedAttack = &Enemy::Laser;
		SetHitBox(sf::Vector2f(25, 35), 1);


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


	}
    
    Boss::Boss()
    {
        std::cout << "Boss created" << std::endl;
		objectSprite.setTexture(World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_bosses"));
        healthBar.setSize(sf::Vector2f(0,8));
        healthBar.setFillColor(sf::Color::Yellow);
        bossName.setFont(World::GetInstance()->WorldScene.textureContainer.GetFont());
        bossName.setCharacterSize(16);

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
        maxhealth = 1200;
        health = maxhealth;
        moveType = NORMAL;
		sf::Sprite *mysprite = &wings;
        SetCharacterOrigin(&mysprite,true);
        SetHitBox(sf::Vector2f(70,70));
        SetShadow();
        hasAttack = 0;
		speed = 1;
        bossName.setString("Mozza");
        targetPlayer = false;
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

		if (World::GetInstance()->Timer(*this, 800000.0, NODELAY) && velZ == 0) {

			targetPosition.x = RandomNumber(600);
			targetPosition.y = RandomNumber(600);
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

	void Enemy::MoveSlide() {

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

			objectSprite.setTextureRect(sf::IntRect(objectSprite.getTextureRect().left, (174 * frame) + spriteTop, objectSprite.getTextureRect().width, objectSprite.getTextureRect().height));

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

			objectSprite.setTextureRect(sf::IntRect(objectSprite.getTextureRect().left, (((spriteDirection * 4 + frame) * 174) + spriteTop), objectSprite.getTextureRect().width, objectSprite.getTextureRect().height));

		}

	}
    
	void Enemy::Move() {

		if (AssignedBehavior != NULL || 0) AssignedBehavior(this);
		if (AssignedMovement != NULL || 0) AssignedMovement(this);
		MoveElse();
		Animate(); 
        if(targetPlayer) fireDir = GetAngle(objectSprite.getPosition(),World::GetInstance()->WorldScene.playerPtr->objectSprite.getPosition());
       
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

			if (World::GetInstance()->Timer(*this,400000.0)) {
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
        
        if(!World::GetInstance()->WorldScene.playerPtr->dead)Act();
        
    }
    
	void Enemy::Attack() {

		if(AssignedAttack != NULL || 0) AssignedAttack(this);
        
    }
    
	void Enemy::DrawChild() {



	}

	void Djinn::DrawChild() {

		if (World::GetInstance()->Timer(*this, VERY_FAST)) {

			if (wings.getTextureRect().top >= 348) wings.setTextureRect(sf::IntRect(243, 0, 134, 44));

			else wings.setTextureRect(sf::IntRect(243, wings.getTextureRect().top + 174, 134, 44));

		}

		wings.setPosition(objectSprite.getPosition().x-66, objectSprite.getPosition().y-100);
		World::GetInstance()->DrawObject(wings);

	}

    void Enemy::Draw(sf::RenderTarget& window){
        
		DrawChild();

		if (showHurt == true) {
			World::GetInstance()->DrawObject(objectSprite, "whiteShader");
			showHurt = false;
		}

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
			std::cout << i << " for " << BehaviorList.size() << std::endl;
				
		}

		tempList.clear();
	}

	void Boss::MoveToCenter() {

		float oldx = objectHitBox.getPosition().x;
		float oldy = objectHitBox.getPosition().y;

		oldx += (300 - oldx) / 25;
		oldy += (300 - oldy) / 25;

		objectSprite.setPosition(oldx, oldy);
		if (World::GetInstance()->Timer(*this, NORMAL, NODELAY)) CreateClone(objectSprite, "tx_bosses",false);

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
			if (targetPosition.x > 500) targetPosition.x = 500;
			if (targetPosition.y < 100) targetPosition.y = 100;
			if (targetPosition.y > 500) targetPosition.y = 500;

			std::cout << targetPosition.x << " & " << targetPosition.y << std::endl;

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

		healthBar.setPosition(World::GetInstance()->viewPos.x - ((WINDOW_X / 3) / 2), World::GetInstance()->viewPos.y + ((WINDOW_Y / 3) / 2 - 8));
		bossName.setPosition(healthBar.getPosition().x, healthBar.getPosition().y - 22);
		currhealth += -(healthBar.getSize().x - (float(WINDOW_X) / 3) / (maxhealth / health)) / 20;
		healthBar.setSize(sf::Vector2f(currhealth, 8));

		if (health <= 0) {

			misDestroyed = true;
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

		if (showHurt == true) {
			World::GetInstance()->DrawObject(objectSprite, "whiteShader");
			showHurt = false;
		}

		else World::GetInstance()->DrawObject(objectSprite);

		World::GetInstance()->DrawObject(healthBar);
		World::GetInstance()->DrawObject(bossName);
		World::GetInstance()->DrawObject(wings);


	}
    
    void Boss::Draw(sf::RenderTarget& window){
        

		if (showHurt == true) {
			World::GetInstance()->DrawObject(objectSprite, "whiteShader");
			showHurt = false;
		}

		else World::GetInstance()->DrawObject(objectSprite);
        World::GetInstance()->DrawObject(objectHitBox);
        World::GetInstance()->DrawObject(healthBar);
        World::GetInstance()->DrawObject(bossName);
        
        
        
    }

    
    // Destructors
    
    Enemy::~Enemy()
    {
	
    }
    
    Boss::~Boss(){

		//MovementList.clear();
		//AttackList.clear();
		World::GetInstance()->WorldScene.audioContainer.music.stop();
    
    }

	void Enemy::Die() {

		
		// if this is a large enemy, do appropriate death animation; otherwise do the small one

		if (objectShadow.getTextureRect().left != 0) {

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

	Buddy::~Buddy() {
		std::cout << "buddy destroyed" << std::endl;
	}
    
    EnemyNode::~EnemyNode(){
        
    }
    
    Actor::~Actor(){
        
        
    }
    
    Slime::~Slime(){
        
    }

	Roach::~Roach() {

	}

	Eball::~Eball() {

	}

	Spore::~Spore() {

	}
    
    Star::~Star(){
        
        
    }

	Djinn::~Djinn() {

	}
    
    Squid::~Squid(){
        
    }
    
    ////// MISC FUNCTIONS
    
    
    void Object::GetZone(int vecPos){
        
        zone.clear();
        
        int zoneSize = 70;
        
        sf::RectangleShape tempshape (sf::Vector2f (zoneSize,zoneSize));
        
        tempshape.setFillColor(sf::Color::Color(255,255,255,30));
        
        sf::IntRect bounds = GetObjectBounds();
        
        int leftTop = (RoundDown(bounds.left,zoneSize) + RoundDown(bounds.top,zoneSize) + (RoundDown(bounds.top,zoneSize)*2))/100 ;
        int rightTop = (RoundDown(bounds.width,zoneSize) + RoundDown(bounds.top,zoneSize) + (RoundDown(bounds.top,zoneSize)*2))/100 ;
        int leftBttm = (RoundDown(bounds.left,zoneSize) + RoundDown(bounds.height,zoneSize) + (RoundDown(bounds.height,zoneSize)*2))/100 ;
        int rightBttm = (RoundDown(bounds.width,zoneSize) + RoundDown(bounds.height,zoneSize) + (RoundDown(bounds.height,zoneSize)*2))/100 ;
        
        //std::cout << leftTop << std::endl;
        
        std::vector<int> vec = {leftTop,rightTop,leftBttm,rightBttm};
        std::sort(vec.begin(),vec.end());
        vec.erase( unique( vec.begin(), vec.end() ), vec.end() );

        for(int i : vec){
            
            World::GetInstance()->WorldScene.objectContainer->zoneMatrix.insert(std::make_pair(i,vecPos));
            zone.append(std::to_string(i) + ",");
            
        }
        
        //std::cout << "zone list: " << zone << std::endl;
               
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
     
			if (!defending) {

					if (moveType == NORMAL) vel.y = 0;

					int newDamage = GetRandDmg(damage);
					health -= newDamage;
					hurt = true;
					showHurt = true;

				}

    }
    
	void Enemy::isCollided(int var) {

		isDamaged(var);

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
        
    }
    
    void Object::UpdateHitBox(){
        
        objectHitBox.setPosition(objectSprite.getPosition());
    }
    
    void Object::isCollided(int var){
        
    
        
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


