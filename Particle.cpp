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
    std::string Textbox::characters[11] = {"LIM","MOZZA","HAUZER","FEET","NOOK"};
    
    bool PlayerMenu::menuUp = false;
    
    GUI::GUI() : Object(){
        
        type = "GUI";
        guiCount++;
        
    }
    
    void GUI::Update(){
        
    }
    
    GUI::~GUI(){
        
        guiCount--;
    }
    
    MenuItem::MenuItem(std::string lableName, char typeName) : GUI(){
        
        if(menuFnt.getInfo().family == ""){
        
        menuFnt.loadFromFile("/Users/darionmccoy/Dropbox/Game Design Stuff/Projects/Backup/HardSoda/HardSoda/fonts/apfont.ttf");
         const_cast<sf::Texture&>(menuFnt.getTexture(16)).setSmooth(false);
    
        }
        
        menuLable.setFont(menuFnt);
        menuLable.setString(lableName);
        menuLable.setPosition(70,135+(count * 15));
        menuLable.setCharacterSize(16);
        menuLable.setColor(sf::Color::White);
        
        type = typeName;
        
        if(type == SWITCH){
            
            objectSprite.setTexture(World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_menu_bar.png"));
            objectSprite.setPosition(menuLable.getPosition().x + (35),menuLable.getPosition().y +7);
            objectSprite.setTextureRect(sf::IntRect(0,0,1,10));
            
            if(lableName == "Volume"){
                
                linkedAttribute = std::weak_ptr<int>(World::GetInstance()->GlobalMembers.maxVolume);
                objectSprite.setTextureRect(sf::IntRect(0,0,*World::GetInstance()->GlobalMembers.maxVolume,10));
            }
            
            else if(lableName == "Health"){
                
                linkedAttribute = std::weak_ptr<int>(World::GetInstance()->GlobalMembers.health);
                objectSprite.setTextureRect(sf::IntRect(0,0,*World::GetInstance()->GlobalMembers.health,10));
                
            }
        
        }
        
        count++;
        
    }
    
    
    
    void MenuItem::Update(){
        
        
        
    };
    
    void MenuItem::ToggleSelection(){
        
        if(menuLable.getColor() == sf::Color::White){
            
            menuLable.setColor(sf::Color::Green);
            menuLable.move(2,0);
            
        }
        
        else{
            
            menuLable.setColor(sf::Color::White);
            menuLable.move(-2,0);
            
        }
    }
    
    void MenuItem::Draw(sf::RenderTarget& window){
        
        World::GetInstance()->DrawObject(objectSprite);
        World::GetInstance()->DrawObject(menuLable);
        
    }
    
    MenuItemHolder::MenuItemHolder() : GUI(){
        
        MenuItem::count = 0;
        
    }
    
    void MenuItemHolder::Update(){
    }
    
    void MenuItemHolder::Draw(sf::RenderTarget &window){
        
    }
    
    MenuItemHolder::~MenuItemHolder(){
    }
    
    
    Menu::Menu() : GUI(){
        
        MenuItemHolder menuHolder;
        MenuItem item1 ("Start",START);
        MenuItem item2("Options",DIVE);
        MenuItem item3("Quit",EXIT);
    
        menuHolder.menuList.push_back(item1);
        menuHolder.menuList.push_back(item2);
        menuHolder.menuList.push_back(item3);
        menuHolder.menuList.at(0).ToggleSelection();
        menuContainer.push_back(menuHolder);
        
    }
    
    void Menu::Update(){
        
        //curent menu is always beginning of vector and what should be drawn
        
        int currentScreen = menuContainer.size()-1;
        
        if(World::GetInstance()->Timer(*this,FAST)){
        
            if(sf::Keyboard::isKeyPressed(World::GetInstance()->GlobalMembers.keyboardControls[controlsUp])){
                
                World::GetInstance()->WorldScene.audioContainer.PlaySFX("sfx_jump2");
                if(menuContainer.at(currentScreen).currentPos != 0){
                    
                    menuContainer.at(currentScreen).menuList.at(menuContainer.at(currentScreen).currentPos).ToggleSelection();
                    --menuContainer.at(currentScreen).currentPos;
                    menuContainer.at(currentScreen).menuList.at(menuContainer.at(currentScreen).currentPos).ToggleSelection();
                    
                }
                
                else{
                    
                    menuContainer.at(currentScreen).menuList.at(menuContainer.at(currentScreen).currentPos).ToggleSelection();
                    menuContainer.at(currentScreen).currentPos = menuContainer.at(currentScreen).menuList.size()-1;
                    menuContainer.at(currentScreen).menuList.at(menuContainer.at(currentScreen).currentPos).ToggleSelection();
                    
                }
            
            }
            
            if(sf::Keyboard::isKeyPressed(World::GetInstance()->GlobalMembers.keyboardControls[controlsDown])){
                
                World::GetInstance()->WorldScene.audioContainer.PlaySFX("sfx_jump2");
                if(menuContainer.at(currentScreen).currentPos != menuContainer.at(currentScreen).menuList.size()-1){
                    
                    menuContainer.at(currentScreen).menuList.at(menuContainer.at(currentScreen).currentPos).ToggleSelection();
                    ++menuContainer.at(currentScreen).currentPos;
                    menuContainer.at(currentScreen).menuList.at(menuContainer.at(currentScreen).currentPos).ToggleSelection();
                    
                }
                
                else{
                    
                    menuContainer.at(currentScreen).menuList.at(menuContainer.at(currentScreen).currentPos).ToggleSelection();
                    menuContainer.at(currentScreen).currentPos = 0;
                    menuContainer.at(currentScreen).menuList.at(menuContainer.at(currentScreen).currentPos).ToggleSelection();
                }
                
            }
            
            
            else if(sf::Keyboard::isKeyPressed(World::GetInstance()->GlobalMembers.keyboardControls[controlsLeft]) && HasSwitch(menuContainer.at(currentScreen).menuList.at(menuContainer.at(currentScreen).currentPos))){
                
                --*menuContainer.at(currentScreen).menuList.at(menuContainer.at(currentScreen).currentPos).linkedAttribute.lock();
                // switch visul changes need to go here
                std::cout << *World::GetInstance()->GlobalMembers.maxVolume << std::endl;
                menuContainer.at(currentScreen).menuList.at(menuContainer.at(currentScreen).currentPos).objectSprite.setTextureRect(sf::IntRect(0,0,*menuContainer.at(currentScreen).menuList.at(menuContainer.at(currentScreen).currentPos).linkedAttribute.lock(),10));
            
            }
        
            else if(sf::Keyboard::isKeyPressed(World::GetInstance()->GlobalMembers.keyboardControls[controlsRight]) && HasSwitch(menuContainer.at(currentScreen).menuList.at(menuContainer.at(currentScreen).currentPos))){
                
                ++*menuContainer.at(currentScreen).menuList.at(menuContainer.at(currentScreen).currentPos).linkedAttribute.lock();
                // switch visul changes need to go here
                std::cout << *World::GetInstance()->GlobalMembers.maxVolume << std::endl;
                menuContainer.at(currentScreen).menuList.at(menuContainer.at(currentScreen).currentPos).objectSprite.setTextureRect(sf::IntRect(0,0,*menuContainer.at(currentScreen).menuList.at(menuContainer.at(currentScreen).currentPos).linkedAttribute.lock(),10));
            
            }
        
            else if(sf::Keyboard::isKeyPressed(World::GetInstance()->GlobalMembers.keyboardControls[controlsA])) DoMenuAction(menuContainer.at(currentScreen).menuList.at(menuContainer.at(currentScreen).currentPos));
            
            else if((sf::Keyboard::isKeyPressed(World::GetInstance()->GlobalMembers.keyboardControls[controlsB]) && menuContainer.size() != 1)) menuContainer.pop_back();
                
                

        }
    }
    
    void Menu::DoMenuAction(MenuItem& item){
        
        if(item.type == START) World::GetInstance()->ReadyScene("map1");
        
        
        else if(item.type == DIVE){
            
            MenuItemHolder menuHolder;
            MenuItem item1 ("Volume",SWITCH);
            MenuItem item2("Health",SWITCH);
            
            menuHolder.menuList.push_back(item1);
            menuHolder.menuList.push_back(item2);
            menuHolder.menuList.at(0).ToggleSelection();
            menuContainer.push_back(menuHolder);
            
        }
        
        else if(item.type == EXIT) World::GetInstance()->windowWorld->close();
    }
    
    bool Menu::HasSwitch(MenuItem& item){
        
        return item.type == SWITCH;
        
    }
    
    void Menu::Draw(sf::RenderTarget& window){
        
        for (auto i : menuContainer.at(menuContainer.size()-1).menuList) i.Draw(window);
        
    }
    
    Menu::~Menu(){
        
    
    }
    
    MenuItem::~MenuItem(){
    
    }
    
    PlayerMenu::PlayerMenu(){
        
        objectSprite.setTexture(World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_portraits.png"));
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
        std::cout << objectSprite.getPosition().x << " | " << objectSprite.getPosition().y << std::endl;

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
        
        textboxCount = 1;
        
        std::cout << "creating texbox" << std::endl;
        
        
        characterName = characters[name];
        
        //std::cout << characterName + std::to_string(World::GetInstance()->GlobalMembers.levelsFinished) << std::endl;
        
        script = World::GetInstance()->CharacterScripts.at(characterName + std::to_string(0));
        
        int lineCount = 0;
        
        //iterate through string to insert breaks
        
        if(script.length() >= lineLength){
        
            for(int i = lineLength; i < script.length(); i+= lineLength){
            
                if(i <= script.length()){
                    
                    //check if the current string position is a letter (this logic has to be flipped for some reason)
                    
                    if(strncmp(&script.at(i)," ",1))
                    {
                        
                        //check previous letters for the first space to break there instead of breaking mid-word
                        
                        for(int n = i; n != 0; n--){
                            
                            if(!strncmp(&script.at(n)," ",1)){
                                
                                script.replace(n,1,"\n");
                                i -= i-n;
                                break;
                                
                            }
                            
                        }
                    
                    }
                    
                    //if not, treat this letter as a space
                    
                    else{
                    
                        script.replace(i,1,"\n");
                        i++;
                                   
                    }
                    
                    lineCount++;
                    
                    //if the max lines for the text box has been reached, insert the trigger to erase the current text to begin progressing the next lines
                    
                    if(lineCount == maxLines){
                        
                        script.replace(i,1,">");
                        lineCount = 0;
                    
                    }
                    
                }
                
                else break;
            }
            
        }
        
        scriptLength = script.length();
        
        backDrop.setTexture(World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_misc.png"));
        backDrop.setTextureRect(sf::IntRect(0,138,448,80));
        
        // objectsprite is portrait sprite
        
        objectSprite.setTexture(World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_portraits.png"));
        
        portraitSize = sf::Vector2i(80,100);
        
        objectSprite.setTextureRect(sf::IntRect(0,portraitSize.y * name,portraitSize.x,portraitSize.y));
        
        boxText.setFont(World::GetInstance()->WorldScene.textureContainer.GetFont());
        boxText.setCharacterSize(16);
        
        boxName.setFont(World::GetInstance()->WorldScene.textureContainer.GetFont());
        boxName.setString(characterName);
        boxNameBG.setSize(sf::Vector2f(8 * characterName.length()+2,16));
        boxNameBG.setFillColor((sf::Color::Black));
        boxName.setCharacterSize(16);
        
        boxArrow.setTexture(World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_misc.png"));
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
                    
                        
                        if(World::GetInstance()->Timer(*this,FAST)){
                        
                            if(sf::Keyboard::isKeyPressed(World::GetInstance()->GlobalMembers.keyboardControls[controlsB])){
                            
                                newScript.erase(0);
                                script.erase(0,1);
                            
                            }
                            
                        }
                        
                        if(World::GetInstance()->Timer(*this,SLOW)){
                            
                            int left = boxArrow.getTextureRect().left == 15 ? 20 : 15;
                            std::cout << left << std::endl;
                            
                            boxArrow.setTextureRect(sf::IntRect(left,0,5 ,5));
                            
                        }
                        
                    }
                    
                    // if none of the conditions above are not true, proceed to progress text
                    
                    else{
                        
                        //if player presses "Z" while progressing, this takes the text the trigger character
                        
                        if(World::GetInstance()->Timer(*this,FAST, NODELAY)){
                            
                            /*
                            if(sf::Keyboard::isKeyPressed(World::GetInstance()->GlobalMembers.keyboardControls[buttonA])){
                                
                                unsigned long int lastLinePos = script.find(">");
                                std::cout << "trigger at " << lastLinePos << std::endl;
                                newScript.append(script,0,lastLinePos);
                                script.erase(0,lastLinePos);
                                boxText.setString(newScript);
                                
                            }
                             */
                            
                        }
                        
                        else{
                        
                            newScript.append(script,0,1);
                            script.erase(0,1);
                            World::GetInstance()->WorldScene.audioContainer.PlaySFX("sfx_txt"); 
                            
                        }
                        
                        boxText.setString(newScript);
                                            
                    }
                    
                }
                
                              
            
            } 
            
        }
        
        else{
            
            if(World::GetInstance()->Timer(*this,FAST)){
                
                if(sf::Keyboard::isKeyPressed(World::GetInstance()->GlobalMembers.keyboardControls[controlsB])) misDestroyed = true;
                
            }
            
        }
        
    }
    
    BattleBackground::BattleBackground(){
                
        objectSprite.setTexture(World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_battle_bg_" + std::to_string(World::GetInstance()->GlobalMembers.currentLevel) + ".png"));
        objectSprite.setTextureRect(sf::IntRect (0,0,2000,2000));
        objectSprite.setOrigin(2000/2,2000/2);
        type = "BG";
        
    }
    
    Door::Door(std::string pathway) : Object() {
        
        levelPath = stoi(pathway.erase(0,pathway.find("_")+1));
        
        objectSprite.setTexture(World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_misc.png"));
        objectSprite.setTextureRect(sf::IntRect(0,42+(32*0),16,32));
        SetHitBox(sf::Vector2f(objectSprite.getTextureRect().width,objectSprite.getTextureRect().height),2);
        Object::type = "Prop";
        std::cout << "Door created" << std::endl;
        
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
        
        std::cout << "Calling ReadyScene() ..." << std::endl;
        World::GetInstance()->GlobalMembers.currentLevel = levelPath;
        World::GetInstance()->ReadyScene("battle");
    }
    
    
    ///////////////////////
    
    Hud::Hud() : GUI (){
        
        World::GetInstance()->WorldScene.hudPtr = this;
        hnumFont.loadFromFile("/Users/darionmccoy/Dropbox/Game Design Stuff/Projects/Backup/HardSoda/HardSoda/fonts/pxsmll.ttf");
        hhealthNum.setFont(hnumFont);
        hpowerNum.setFont(hnumFont);
        
        hbar.setTextureRect(sf::IntRect(0,0,62,39));
        hbar.setTexture(World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_menu.png"));
        
        hitem.setTextureRect(sf::IntRect(0,0,16,16));
        hitem.setTexture(World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_menu_item.png"));
        
        hhealth.setTextureRect(sf::IntRect(-20,0,24,4));
        hhealth.setTexture(World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_menu_health_pow.png"));
        
        hpower.setTextureRect(sf::IntRect(-20,4,24,4));
        hpower.setTexture(World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_menu_health_pow.png"));
        
    }

    
    void Hud::Update(){
        
        
        if(hmaxhealth == 0 || hmaxpow == 0){
            
            hmaxhealth = World::GetInstance()->WorldScene.playerPtr->playerHealth;
            hmaxpow = World::GetInstance()->WorldScene.playerPtr->playerPow;
            
        }
        
        if(World::GetInstance()->WorldScene.playerPtr->playerHealth < 0) World::GetInstance()->WorldScene.playerPtr->playerHealth = 0;
        
        
        if(World::GetInstance()->WorldScene.playerPtr->playerPow < 0) World::GetInstance()->WorldScene.playerPtr->playerPow = 0;
        
        hbar.setPosition(World::GetInstance()->viewPos.x - 225,40);
        hitem.setPosition(hbar.getPosition().x + 11,hbar.getPosition().y + 11);
        
        hhealth.setTextureRect(sf::IntRect(-World::GetInstance()->WorldScene.playerPtr->playerHealth,0,World::GetInstance()->WorldScene.playerPtr->playerHealth + 4,4));
        hhealth.setPosition(hbar.getPosition().x + 53,hbar.getPosition().y + 4);
        
        hpower.setTextureRect(sf::IntRect(-World::GetInstance()->WorldScene.playerPtr->playerPow,4,World::GetInstance()->WorldScene.playerPtr->playerPow + 4,4));
        hpower.setPosition(hbar.getPosition().x + 50,hbar.getPosition().y + 10);
        
        std::string temphealth (std::to_string(World::GetInstance()->WorldScene.playerPtr->playerHealth) + "/" + std::to_string(hmaxhealth));
        std::string temppow (std::to_string(World::GetInstance()->WorldScene.playerPtr->playerPow) + "/" + std::to_string(hmaxpow));
        
        hhealthNum.setString(temphealth);
        hpowerNum.setString(temppow);
        hhealthNum.setCharacterSize(16);
        hpowerNum.setCharacterSize(16);
        
        hhealthNum.setPosition(hhealth.getPosition().x + World::GetInstance()->WorldScene.playerPtr->playerHealth + 6,hhealth.getPosition().y - 12);
        hpowerNum.setPosition(hhealth.getPosition().x + World::GetInstance()->WorldScene.playerPtr->playerPow + 3,hpower.getPosition().y - 12);
        
        if(hdamaged){
            
            hhealth.setTextureRect(sf::IntRect(-World::GetInstance()->WorldScene.playerPtr->playerHealth,8,World::GetInstance()->WorldScene.playerPtr->playerHealth + 4,4));
            
            if(World::GetInstance()->Timer(*this,200.0f)) hdamaged = false;
            
        }
        
        //hitem.setTextureRect(sf::IntRect(World::GetInstance()->WorldScene.playerPtr->selectedWeapon * 16,0,16,16));
        
    }
    
    void BattleBackground::Update(){
        
         objectSprite.rotate(0.05);
        
    }
    
    
    void Hud::Draw(sf::RenderTarget& window){
        
        World::GetInstance()->DrawObject(hbar);
        World::GetInstance()->DrawObject(hitem);
        World::GetInstance()->DrawObject(hhealth);
        World::GetInstance()->DrawObject(hpower);
        World::GetInstance()->DrawObject(hhealthNum);
        World::GetInstance()->DrawObject(hpowerNum);
        
    }
    
    void BattleBackground::Draw(sf::RenderTarget& window){
        
        World::GetInstance()->DrawObject(objectSprite,"waveShader");
        
    }
    
    Hud::~Hud(){
        
        
    }
    
    PlayerMenu::~PlayerMenu(){
        
    }
    
    Door::~Door(){
        
    }
    
    BattleBackground::~BattleBackground(){
        
    }

    
    //////////////// PLAYER DEFINITIONS //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
        
    Player::Player(sf::Vector2i pos) : Object ()
    {
        
        std::cout << "Player created" << std::endl;
        objectSprite.setTexture(World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_player.png"));
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
        SetHitBox(sf::Vector2f(6,6),1);
        dead = false;
        if(!World::GetInstance()->CameraTarget) World::GetInstance()->SetCameraTarget(*this);

        
        
    }
    
    Player::~Player(){
        
        posXtemp1 = NULL;
        posYtemp2 = NULL;
        delete posXtemp1;
        delete posYtemp2;
        dead = true;
        
        
    }
    
    void Player::Update()
    {
        
        // Walking
        
        if(sf::Keyboard::isKeyPressed(World::GetInstance()->GlobalMembers.keyboardControls[controlsPause])){
            
            if(!PlayerMenu::menuUp){
                
                itemQueue menu;
                menu.properties["itemType"] = "PlayerMenu";
                World::GetInstance()->WorldScene.objectContainer->Queue.push_back(menu);
                PlayerMenu::menuUp = true;
                
            }
            
            else PlayerMenu::menuUp = false;
            
        }
        
        if(World::GetInstance()->IsPlayerActive()){
            
        
        if(sf::Keyboard::isKeyPressed(World::GetInstance()->GlobalMembers.keyboardControls[controlsDown]) && sf::Keyboard::isKeyPressed(World::GetInstance()->GlobalMembers.keyboardControls[controlsLeft])) movement = swest;
        
        else if(sf::Keyboard::isKeyPressed(World::GetInstance()->GlobalMembers.keyboardControls[controlsLeft]) && sf::Keyboard::isKeyPressed(World::GetInstance()->GlobalMembers.keyboardControls[controlsUp])) movement = nwest;
        
        else if(sf::Keyboard::isKeyPressed(World::GetInstance()->GlobalMembers.keyboardControls[controlsUp]) && sf::Keyboard::isKeyPressed(World::GetInstance()->GlobalMembers.keyboardControls[controlsRight])) movement = neast;
        
        else if(sf::Keyboard::isKeyPressed(World::GetInstance()->GlobalMembers.keyboardControls[controlsRight]) && sf::Keyboard::isKeyPressed(World::GetInstance()->GlobalMembers.keyboardControls[controlsDown])) movement = seast;
        
        else if(sf::Keyboard::isKeyPressed(World::GetInstance()->GlobalMembers.keyboardControls[controlsDown])) movement = south;
        
        else if(sf::Keyboard::isKeyPressed(World::GetInstance()->GlobalMembers.keyboardControls[controlsLeft])) movement = west;
        
        else if(sf::Keyboard::isKeyPressed(World::GetInstance()->GlobalMembers.keyboardControls[controlsUp])) movement = north;
        
        else if(sf::Keyboard::isKeyPressed(World::GetInstance()->GlobalMembers.keyboardControls[controlsRight])) movement = east;

        }
        
        if (!sf::Keyboard::isKeyPressed(World::GetInstance()->GlobalMembers.keyboardControls[controlsRight]) && !sf::Keyboard::isKeyPressed(World::GetInstance()->GlobalMembers.keyboardControls[controlsUp]) && !sf::Keyboard::isKeyPressed(World::GetInstance()->GlobalMembers.keyboardControls[controlsDown]) && !sf::Keyboard::isKeyPressed(World::GetInstance()->GlobalMembers.keyboardControls[controlsLeft])) movement = idle;
        
        
        
        // Animation
        
        if(movement != idle){
            
            vel.y = 1;
            vel.x = 0;
			RotateVector(vel, 45 * movement);

            if(World::GetInstance()->Timer(*this,NORMAL, NODELAY)){
				 
				std::cout << NORMAL << std::endl;
                frame_pos = (frame_pos >= 5) ? 0 : frame_pos+1;
                if(frame_pos == 1 || frame_pos == 4) World::GetInstance()->WorldScene.audioContainer.PlaySFX("sfx_step"); 
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
        
        //std::cout << vel.x << "|" << vel.y << std::endl;
        
        // breathing effect
        
        objectSprite.setScale(Sine(),Sine());
        
    }
    
    BattlePlayer::BattlePlayer(){
        
        hotSpot.x = objectSprite.getPosition().x + 6;
        hotSpot.y = objectSprite.getPosition().y + 12;
        std::cout << objectSprite.getPosition().x << "," << objectSprite.getPosition().y << std::endl;
        
        
    }
    
    void BattlePlayer::Update(){
        
        if(!World::GetInstance()->WorldScene.transition){
            
        
        if(sf::Keyboard::isKeyPressed(World::GetInstance()->GlobalMembers.keyboardControls[controlsDown]) && sf::Keyboard::isKeyPressed(World::GetInstance()->GlobalMembers.keyboardControls[controlsLeft])) movement = swest;
        
        else if(sf::Keyboard::isKeyPressed(World::GetInstance()->GlobalMembers.keyboardControls[controlsLeft]) && sf::Keyboard::isKeyPressed(World::GetInstance()->GlobalMembers.keyboardControls[controlsUp])) movement = nwest;
        
        else if(sf::Keyboard::isKeyPressed(World::GetInstance()->GlobalMembers.keyboardControls[controlsUp]) && sf::Keyboard::isKeyPressed(World::GetInstance()->GlobalMembers.keyboardControls[controlsRight])) movement = neast;
        
        else if(sf::Keyboard::isKeyPressed(World::GetInstance()->GlobalMembers.keyboardControls[controlsRight]) && sf::Keyboard::isKeyPressed(World::GetInstance()->GlobalMembers.keyboardControls[controlsDown])) movement = seast;
        
        else if(sf::Keyboard::isKeyPressed(World::GetInstance()->GlobalMembers.keyboardControls[controlsDown])) movement = south;
        
        else if(sf::Keyboard::isKeyPressed(World::GetInstance()->GlobalMembers.keyboardControls[controlsLeft])) movement = west;
        
        else if(sf::Keyboard::isKeyPressed(World::GetInstance()->GlobalMembers.keyboardControls[controlsUp])) movement = north;
        
        else if(sf::Keyboard::isKeyPressed(World::GetInstance()->GlobalMembers.keyboardControls[controlsRight])) movement = east;
            
        
        }
        
        if (!sf::Keyboard::isKeyPressed(World::GetInstance()->GlobalMembers.keyboardControls[controlsRight]) && !sf::Keyboard::isKeyPressed(World::GetInstance()->GlobalMembers.keyboardControls[controlsUp]) && !sf::Keyboard::isKeyPressed(World::GetInstance()->GlobalMembers.keyboardControls[controlsDown]) && !sf::Keyboard::isKeyPressed(World::GetInstance()->GlobalMembers.keyboardControls[controlsLeft])) movement = idle;
        
        // Animation
        
        if(movement != idle){
            
            vel.y = 2.25;
            vel.x = 0;
            RotateVector(vel,45*movement);
            
            if(World::GetInstance()->Timer(*this,FAST,NODELAY)){
                
                frame_pos = (frame_pos >= 5) ? 0 : frame_pos+1;
    
                if(!sf::Keyboard::isKeyPressed(World::GetInstance()->GlobalMembers.keyboardControls[controlsA])){
                    
                    objectSprite.setTextureRect(sf::IntRect(frame_pos * spriteWidth, movement * FRAME, spriteWidth, spriteHeight));
                    fireDir = movement;
                    
                }
                
                else objectSprite.setTextureRect(sf::IntRect(frame_pos * spriteWidth, objectSprite.getTextureRect().top, spriteWidth, spriteHeight));
                
            }
            
            
        }
        
        else{
            
            vel.x = 0;
            vel.y = 0;
            objectSprite.setTextureRect(sf::IntRect(0, objectSprite.getTextureRect().top, spriteWidth, spriteHeight));
            
        }
        
        if(sf::Keyboard::isKeyPressed(World::GetInstance()->GlobalMembers.keyboardControls[controlsB])){
            
            // repeater
        
            if(World::GetInstance()->GlobalMembers.playerWeapon == 0 && World::GetInstance()->Timer(*this,SLOW,NODELAY)){
                
                itemQueue bullet;
                bullet.properties["PosX"] = std::to_string(hotSpot.x);
                bullet.properties["PosY"] = std::to_string(hotSpot.y);
                bullet.properties["Direction"] = std::to_string(fireDir);
                bullet.properties["VelX"] = std::to_string(vel.x);
                bullet.properties["VelY"] = std::to_string(vel.y);
                bullet.properties["itemType"] = "PlayerLaser";
                World::GetInstance()->WorldScene.audioContainer.PlaySFX("sfx_laser9");
                World::GetInstance()->WorldScene.objectContainer->Queue.push_back(bullet);
                    
            }
            
            // cannon
            
            else if(World::GetInstance()->GlobalMembers.playerWeapon == 1 && PlayerBomb::totalBombs < 4 && World::GetInstance()->Timer(*this,VERY_SLOW,NODELAY)){
                
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
            
            // rail
            
            else if(World::GetInstance()->GlobalMembers.playerWeapon == 2 && PlayerBomb::totalBombs < 2 && World::GetInstance()->Timer(*this,VERY_SLOW,NODELAY)){
                
                itemQueue bullet;
                bullet.properties["PosX"] = std::to_string(hotSpot.x);
                bullet.properties["PosY"] = std::to_string(hotSpot.y);
                bullet.properties["Direction"] = std::to_string(fireDir);
                bullet.properties["VelX"] = std::to_string(vel.x);
                bullet.properties["VelY"] = std::to_string(vel.y);
                bullet.properties["itemType"] = "PlayerBeam";
                World::GetInstance()->WorldScene.audioContainer.PlaySFX("sfx_bomb_shoot");
                World::GetInstance()->WorldScene.objectContainer->Queue.push_back(bullet);
                
            }
            
            // spreader
            
            else if(World::GetInstance()->GlobalMembers.playerWeapon == 3 && World::GetInstance()->Timer(*this,VERY_SLOW,NODELAY)){
                
                itemQueue bullet;
                bullet.properties["PosX"] = std::to_string(hotSpot.x);
                bullet.properties["PosY"] = std::to_string(hotSpot.y);
                bullet.properties["VelX"] = std::to_string(vel.x);
                bullet.properties["VelY"] = std::to_string(vel.y);
                bullet.properties["itemType"] = "PlayerRepeater";
                bullet.properties["Direction"] = std::to_string(45*fireDir);
                std::cout << fireDir << std::endl;
                World::GetInstance()->WorldScene.objectContainer->Queue.push_back(bullet);
                
                bullet.properties["Direction"] = std::to_string(45*fireDir -20);
                World::GetInstance()->WorldScene.objectContainer->Queue.push_back(bullet);
                
                bullet.properties["Direction"] = std::to_string(45*fireDir +20);
                World::GetInstance()->WorldScene.objectContainer->Queue.push_back(bullet);
                
                
                World::GetInstance()->WorldScene.audioContainer.PlaySFX("sfx_laser9");
                
            }
            
            // boomerang
            
            else if(World::GetInstance()->GlobalMembers.playerWeapon == 4 && PlayerBoomerang::totalBoomerangs < 6 && World::GetInstance()->Timer(*this,SLOW,NODELAY)){
                
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
        
        vel.x = round(vel.x);
        vel.y = round(vel.y);
        objectSprite.move(vel.x,vel.y);
        //std::cout << vel.x << "|" << vel.y << std::endl;
        hotSpot.x = objectSprite.getPosition().x;
        hotSpot.y = objectSprite.getPosition().y - objectSprite.getTextureRect().height/3;
        UpdateShadow();
        *posXtemp1 = objectSprite.getPosition().x;
        *posYtemp2 = objectSprite.getPosition().y;
        if(World::GetInstance()->Timer(*this,60.0f)) CreateClone(objectSprite,"tx_player.png");
        
        objectSprite.setScale(Sine(),Sine());
        

                
        
        
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
        
        
        World::GetInstance()->WorldScene.audioContainer.PlaySFX("sfx_playerDeath");
        
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
        
        
        objectSprite.setTexture((World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_particles.png")));
        
        if(assetHeight == 0){
                
            sf::Texture temp = *objectSprite.getTexture();
            sf::Vector2u tempSize = temp.getSize();
            assetHeight = tempSize.y/8;
            
        }
        
        if(spriteRotation != 2) spriteRotation++;
        else spriteRotation = 0;
        
    }
    
    SpriteClone::SpriteClone() : Fixed(){
        
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
    
    Spark::~Spark(){
        
    }
    
    DoorDestroy::DoorDestroy() : Fixed(){
        
        objectSprite.setTexture(World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_misc.png"));
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
        animSpeed = FAST;

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
    
    PlayerBomb::PlayerBomb() : Fixed(){
        
        totalBombs++;
        objectSprite.setTextureRect(sf::IntRect(0, 170, 20, 21));
        vel.y = 8;
        SetEffectOrigin();
        maxFrame = 4;
        maxTime = 600000.0;
		animSpeed = VERY_FAST;
        deacceleration = 0.25;
        
    }
    
    
    Hit::Hit(){
        
        objectSprite.setTexture((World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_hit.png")));
        objectSprite.setTextureRect(sf::IntRect(0, 0, 30, 30));
        objectSprite.setOrigin(15,15);
    
    }
    
    
    Projectile::Projectile() : Object ()
    {
        
        objectSprite.setTexture((World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_projectiles.png")));
        type = "Projectile";
		velZ = -99;
        
    }
    
    EnemyProjectile::EnemyProjectile() : Projectile(){
        
        type = "EnemyProjectile";
        
    }
    
    EnemyBlip::EnemyBlip() : EnemyProjectile(){
        
        objectSprite.setTextureRect(sf::IntRect(0, 19,7, 7));
        vel.y = 1;
        SetEffectOrigin();
        SetHitBox(sf::Vector2f(3,3));
        active = true;

    }

	HauzerSmog::HauzerSmog() : EnemyProjectile() {

		sprite = 96 + (17 *RandomNumber(2));
		objectSprite.setTextureRect(sf::IntRect(0, 96, 17, 17));
		vel.y = RandomNumber(6,4);
		SetEffectOrigin();
		SetHitBox(sf::Vector2f(6, 6));
		active = true;

	}
    
    Particle::Particle() : Object ()
    {
        
        objectSprite.setTexture(World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_particles.png"));
        
        
    }
    
    Passive::Passive() : Particle(){
        
        
        
    }
    
    Prop::Prop() : Object(){
        
        
        
    }
    
    PropFlame::PropFlame() : Prop(){
        
        objectSprite.setTexture((World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_misc.png")));
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
        
        objectSprite.setTexture((World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_projectiles.png")));
        objectSprite.setTextureRect(sf::IntRect(0, 0, 25, 4));
        damage = 6;
        
    }
    
    PlayerLaser::PlayerLaser() : Projectile()
    {
        
        objectSprite.setTexture((World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_projectiles.png")));
        objectSprite.setTextureRect(sf::IntRect(0, 0, 10, 10));
        objectSprite.setOrigin(5,5);
        SetEffectOrigin();
        vel.y = 4;
        damage = 7;
        SetHitBox(sf::Vector2f(4,4));
        
    }
    
    PlayerBoomerang::PlayerBoomerang() : Projectile(){
        
        objectSprite.setTexture((World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_projectiles.png")));
        objectSprite.setTextureRect(sf::IntRect(0, 32, 20, 20));
        objectSprite.setOrigin(10,10);
        SetEffectOrigin();
        vel.y = 12;
        damage = 6;
        SetHitBox(sf::Vector2f(12,12));
		destroyOnImpact = false;
        totalBoomerangs++;
        
        
    }
    
    PlayerRepeater::PlayerRepeater() : Projectile()
    {
        
        objectSprite.setTexture((World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_projectiles.png")));
        objectSprite.setTextureRect(sf::IntRect(0, 25, 7, 7));
        objectSprite.setOrigin(3,3);
        SetEffectOrigin();
        vel.y = 3;
        damage = 3;
        maxFrame = 3;
        SetHitBox(sf::Vector2f(3,3));
    
    }
    
    PlayerBombExp::PlayerBombExp() : Projectile()
    {
        
        objectSprite.setTexture((World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_projectiles.png")));
        objectSprite.setTextureRect(sf::IntRect(0, 0, 80, 80));
        SetEffectOrigin();
        vel.y = 0;
        damage = 20;
        SetHitBox(sf::Vector2f(70,70));
		destroyOnImpact = false;
        
    }
    
    

    Explosion::Explosion() : Object ()
    {
        
        objectSprite.setTexture((World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_polygon.png")));
        objectSprite.setOrigin(40,40);
        objectSprite.setTextureRect(sf::IntRect(0, 0, 80, 80));
        
    }
    
    Cluster::Cluster() : Object ()
    {
        
        
        
    }
    
    
    Laser::Laser() : Projectile ()
    {
        
        objectSprite.setTexture((World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_blip_3.png")));
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
    

    void Fixed::Update(){
        
        
        if(top == -1)top = objectSprite.getTextureRect().top;
        
        if(World::GetInstance()->Timer(*this,animSpeed))
        {
            
                objectSprite.setTextureRect(sf::IntRect(0,(frame * assetHeight) + top,objectSprite.getTextureRect().width,objectSprite.getTextureRect().height));
                
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
            
            else objectSprite.setColor(sf::Color::Color(0,0,255,objectSprite.getColor().a - 75));
            
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
                particles.properties["itemType"] = "DeathPoof";
                
                particles.properties["PosX"] = std::to_string(nodePosition.x + RandomNumber(20));
                particles.properties["PosY"] = std::to_string(nodePosition.y + RandomNumber(20));
                World::GetInstance()->WorldScene.objectContainer->Queue.push_back(particles);
                
                
                particles.properties["PosX"] = std::to_string(nodePosition.x - RandomNumber(20));
                particles.properties["PosY"] = std::to_string(nodePosition.y - RandomNumber(20));
                World::GetInstance()->WorldScene.objectContainer->Queue.push_back(particles);
                
                
                particles.properties["PosX"] = std::to_string(nodePosition.x - RandomNumber(20));
                particles.properties["PosY"] = std::to_string(nodePosition.y + RandomNumber(20));
                World::GetInstance()->WorldScene.objectContainer->Queue.push_back(particles);
                
                
                particles.properties["PosX"] = std::to_string(nodePosition.x + RandomNumber(20));
                particles.properties["PosY"] = std::to_string(nodePosition.y - RandomNumber(20));
                World::GetInstance()->WorldScene.objectContainer->Queue.push_back(particles);
                
                
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
        
        if(objectSprite.getGlobalBounds().intersects(World::GetInstance()->WorldScene.playerPtr->objectSprite.getGlobalBounds())){
            
        World::GetInstance()->WorldScene.playerPtr->playerHealth -= 15;
        World::GetInstance()->WorldScene.hudPtr->hdamaged = true;
        
        }
        
        sf::Vector2f newOrigin(objectSprite.getPosition().x-15,objectSprite.getPosition().y-15);
        
        World::GetInstance()->WorldScene.levelContainer->CheckCollison(newOrigin,newOrigin, *this);
    
        misDestroyed = true;
        
        
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
        
        if(World::GetInstance()->Timer(*this,VERY_FAST))
        {
            
            objectSprite.setTextureRect(sf::IntRect(frame * objectSprite.getTextureRect().width,
                                                            0,
                                                            objectSprite.getTextureRect().width,
                                                            objectSprite.getTextureRect().height));
            frame++;
            
            if(frame > maxFrame)
            {
                
                frame = 0;
                
            }
            
        }
        
       
        if(vel.x < 0)
        {
            
            objectSprite.setTextureRect(sf::IntRect(objectSprite.getTextureRect().width,
                                                    0,
                                                    -objectSprite.getTextureRect().width,
                                                    objectSprite.getTextureRect().height));
            
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
    
    void PlayerBoomerang::Update(){
        
        if(World::GetInstance()->Timer(*this,VERY_FAST))
        {
        
            objectSprite.setTextureRect(sf::IntRect((frame * 20),32,20,20));
            objectSprite.setOrigin(10,10);
            
            frame++;
            
            if(frame >= 2)
            {
                
                frame = 0;
                
            }
            
        }
        
        objectSprite.setRotation(objectSprite.getRotation() +10);
        sf::Vector2f newPos(objectSprite.getPosition().x, objectSprite.getPosition().y);
        
        newPos.x += (World::GetInstance()->WorldScene.playerPtr->objectSprite.getPosition().x - objectSprite.getPosition().x) / timeCurve;
        newPos.y += (World::GetInstance()->WorldScene.playerPtr->objectSprite.getPosition().y - objectSprite.getPosition().y) / timeCurve;
        if(World::GetInstance()->Timer(*this,FAST)) if(timeCurve > 10) timeCurve -= 4;
    
        objectSprite.setPosition(newPos.x + vel.x,newPos.y + vel.y);
        
        if(World::GetInstance()->Timer(*this,NORMAL))
        {
            vel.x *= 0.5;
            vel.y *= 0.5;
        }
        
        if(World::GetInstance()->Timer(*this,2000000.0)) misDestroyed = true;
        
        if(World::GetInstance()->Timer(*this,FAST)) CreateClone(objectSprite);
        
    }
    
    void PlayerLaser::Update(){
        
        
        if(World::GetInstance()->Timer(*this,VERY_FAST))
        {
            
            
            objectSprite.setTextureRect(sf::IntRect((10+(frame) * 5),0,5,19));
            objectSprite.setOrigin(2,0);
            
            frame++;
            
            if(frame >= 2)
            {
                
                frame = 0;
                
            }
            
        }
        
        
        objectSprite.move(vel.x,vel.y);


    }
    
    void PlayerRepeater::Update(){
        
        
        if(World::GetInstance()->Timer(*this,VERY_FAST))
        {
            
            
            objectSprite.setTextureRect(sf::IntRect(((frame+1) * 7),25,7,7));
            objectSprite.setOrigin(2,0);
            
            frame++;
            
            if(frame > maxFrame)
            {
                
                frame = 0;
                
            }
            
        }
        
        
        if(World::GetInstance()->Timer(*this,1000000.0))
        {
            misDestroyed = true;
            
        }
        
        
        
        objectSprite.move(vel.x,vel.y);
        
        
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

		if (World::GetInstance()->Timer(this,NORMAL, NODELAY)) {

			a->isCollided(damage);
			misDestroyed = destroyOnImpact;

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

	HauzerSmog::~HauzerSmog() {

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
        totalBombs--;
        
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
    
    PlayerRepeater::~PlayerRepeater()
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
    
    
    // Function for collision detection

    
    // Constructors
    
    int Enemy::totalEnemies = 0;
    std::array<std::string,26> LevelManager::enemyList;
    
    LevelManager::LevelManager() : Object(){
        
        objectSprite.setTextureRect(sf::IntRect(0,0,0,0));
    
        bg.setTexture(World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_battle_bg_1.png"));
        bg.setTextureRect(sf::IntRect (0,0,2000,2000));
        bg.setOrigin(2000/2,2000/2);

        if(enemyList[0] == ""){
            
            enemyList[0] = "Star";
            enemyList[1] = "Squid";
            enemyList[2] = "Slime";
            enemyList[3] = "Star2";
            enemyList[4] = "Squid2";
            enemyList[5] = "Slime2";
            enemyList[6] = "Star3";
            enemyList[7] = "Squid3";
            enemyList[8] = "Slime3";
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

    }
    
    void LevelManager::Update(){
        
        if(maxEnemies > 0){
        
            if(World::GetInstance()->Timer(*this, 1500000.0,NODELAY)){
                
                int lvlwidth = fieldSize;
                int lvlheight = fieldSize;;
                
                std::random_device rd;
                std::mt19937 mt(rd());
                std::uniform_int_distribution<int> randomLeftwidth(0,lvlwidth);
                std::uniform_int_distribution<int> randomTopheight(0,lvlheight);
                std::uniform_int_distribution<int> randomEnemy(0,5);
                
                int enemyr = RandomNumber(5);
                
                for(int i = 0; i != 2; i++){
                    
                    
                    Entity::itemQueue enemy;
                    
                    if(enemyr > 2) enemyr = 2;
                    enemy.properties["itemType"] = enemyList[3 * World::GetInstance()->GlobalMembers.currentLevel + enemyr];
                    std::cout << "calling(" << 3 * World::GetInstance()->GlobalMembers.currentLevel + enemyr << "): " << enemyList[3 * World::GetInstance()->GlobalMembers.currentLevel + enemyr] << std::endl;
                    enemy.properties["PosX"] = std::to_string(randomLeftwidth(mt));
                    enemy.properties["PosY"] = std::to_string(randomTopheight(mt));
                    std::cout << "enemys left: " << maxEnemies << std:: endl;
                    World::GetInstance()->WorldScene.objectContainer->Queue.push_back(enemy);
                    maxEnemies--;
                }
                
            }
            
        }
        
        else if( int(bg.getColor().a) != 0 ){
            
            if(World::GetInstance()->Timer(*this,FAST,NODELAY)){
                
                if(int(bg.getColor().a)-2 < 0) bg.setColor(sf::Color(255,255,255,0));     
                else bg.setColor(sf::Color(255,255,255,int(bg.getColor().a)-2));
                World::GetInstance()->WorldScene.audioContainer.music.setVolume(World::GetInstance()->WorldScene.audioContainer.music.getVolume()-0.5);
                std::cout <<  World::GetInstance()->WorldScene.audioContainer.music.getVolume() << std::endl;   
                
            }
            
        }
        
        else{
            
            misDestroyed = true;
            Entity::itemQueue boss;
            boss.properties["PosX"] = std::to_string(600/2);
            boss.properties["PosY"] = std::to_string(600/2);
            boss.properties["itemType"] = enemyList[19 + World::GetInstance()->GlobalMembers.currentLevel];;
            std::cout << "enemys left: " << maxEnemies << std:: endl;
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
        
        objectSprite.setTexture(World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_actors.png"));
        objectSprite.setTextureRect(sf::IntRect (0,actor * 191,16,24));
        name = Textbox::characters[character];
        SetCharacterOrigin();
        SetShadow();
        type = "Actor";
        SetHitBox(sf::Vector2f(40,40),0);
        
    }
    
    void Actor::Update(){
        
        Move();
        UpdateShadow();
        objectSprite.setScale(Sine(),Sine());
    }
    
    void Actor::Move()
    {
        if(PlayerDistance(CLOSE)){
            actorMovement = RoundUp((GetAngle(objectShadow.getPosition(),World::GetInstance()->WorldScene.playerPtr->objectShadow.getPosition())),45);
        objectSprite.setTextureRect(sf::IntRect(framePos * 16, abs(((180 - actorMovement) / 45) * 24),16, 24));
        }
        
        else if(idleBehavior == none) objectSprite.setTextureRect(sf::IntRect(framePos * 16, south,16, 24));
        
        else if(idleBehavior == looking){
            
            if(World::GetInstance()->Timer(*this,VERY_SLOW,DELAY)) objectSprite.setTextureRect(sf::IntRect(framePos * 16, RandomNumber(7)*24,16, 24));
        
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
        objectSprite.setTexture(World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_enemies.png"));
        type = "Enemy";
        scaleTemp = sf::Vector2f(0.12,0);
        objectSprite.setColor(sf::Color(255,255,255,0));
        hurtPos.x = 0;
        hurtPos.y = 0;
        SetHitBox(sf::Vector2f(16,16),1);


    }
    
    EnemyNode::EnemyNode() : Enemy()
    {
        objectSprite.setTextureRect(sf::IntRect (0,0,16,16));
        SetShadow();
        moveType = NODE;
        type = "EnemyNode";
    }
    
    Slime::Slime() : Enemy()
    {
        objectSprite.setTextureRect(sf::IntRect (0,0,24,21));
        SetCharacterOrigin();
        SetShadow();
        moveType = JUMPER;
        health = 20;
    }
    
    Star::Star() : Enemy()
    {
        objectSprite.setTextureRect(sf::IntRect (0,21,28,26));
        SetCharacterOrigin();
        SetShadow();
        moveType = NORMAL;
        speed = 2;
        health = 10;
        vib = 5;
        
    }
    
    Squid::Squid() : Enemy()
    {
        objectSprite.setTextureRect(sf::IntRect (0,47,33,27));
        SetCharacterOrigin();
        SetShadow();
        moveType = NORMAL;
        speed = 0.5;
        health = 30;
        hasAttack = true;
        SetHitBox(sf::Vector2f(20,20),1);
    
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
		objectSprite.setTexture(World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_bosses.png"));
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
		f2 = &Boss::Behavior2;
		f3 = &Boss::Behavior3;
		f4 = &Boss::Behavior4;

		BehaviorList.push_back(f);
		BehaviorList.push_back(f2);
		BehaviorList.push_back(f3);
		BehaviorList.push_back(f4);

    }
    
    Mozza::Mozza(){
        
        objectSprite.setTextureRect(sf::IntRect (0,140,140,140));
		wings.setTexture(World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_bosses.png"));
		wings.setTextureRect(sf::IntRect(0,0, 140, 140));
        speed = 0.5;
        maxhealth = 1200;
        health = maxhealth;
        moveType = NORMAL;
		sf::Sprite *mysprite = &wings;
        SetCharacterOrigin(&mysprite,true);
        SetHitBox(sf::Vector2f(70,70));
        SetShadow();
        hasAttack = true;
		speed = 1;
        bossName.setString("Mozza");
        targetPlayer = false;
		moveOnAttack = true;

    }
    
	void Enemy::MoveElse() {

	}
    
    // Update Functions
    
    void Enemy::Move(){
        
        if(moveType == NORMAL){
            
            
            vel.x = 0;
            vel.y = -speed;
            
            //set move to player
            RotateVector(vel,-(GetAngle(objectSprite.getPosition(),World::GetInstance()->WorldScene.playerPtr->objectSprite.getPosition())));
            
            if(!hasAttack)objectSprite.move(vel.x,vel.y + velZ);
            
			else if (moveOnAttack == false) {

				if (!PlayerDistance(MID)) objectSprite.move(vel.x, vel.y + velZ);

			}

			else objectSprite.move(vel.x, vel.y + velZ);
            
        }
        
        if(moveType == JUMPER){
            
            vel.x = 0;
            vel.y = -speed;
            RotateVector(vel,-(GetAngle(objectSprite.getPosition(),World::GetInstance()->WorldScene.playerPtr->objectSprite.getPosition())));
            
            
            if(World::GetInstance()->Timer(*this, 400000.0,NODELAY)) velZ = -1;
            
            if(velZ != 0) objectSprite.move(vel.x,vel.y + velZ);
            posZ -= velZ;
            
            if(objectSprite.getPosition().y < objectShadow.getPosition().y){
                
                velZ += 0.2;
                
            }
            
            else{
                
                velZ = 0;
                
            }
            
        }

		MoveElse();
        
        //remove this below to activate angle rotation (this makes the enemy always look to player)
        
        if(targetPlayer) fireDir = GetAngle(objectSprite.getPosition(),World::GetInstance()->WorldScene.playerPtr->objectSprite.getPosition());

		if (hasAttack) Attack();
        
    }
    
    
    void Enemy::Act(){
        
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
        
        if(active){
            
            if(hurt) objectSprite.move(-hurtPos);
            Move();
            UpdateShadow();
            
        }
        
        else{
            
            if(World::GetInstance()->Timer(*this,NORMAL,NODELAY)){
                
                if( int(objectSprite.getColor().a) < 255 ) objectSprite.setColor(sf::Color(255,255,255,int(objectSprite.getColor().a)+5));
                else active = true;
                
            }
            
            
        }
        
		isHurt();

    }

	void Enemy::isHurt() {

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
			}
			 
		}
	}
    
    
    void Enemy::Update()
                 
    {
        
        if(!World::GetInstance()->WorldScene.playerPtr->dead)Act();
        if( health <= 0) misDestroyed = true;
        
    }
    
    void Enemy::Attack(){
        
        if(PlayerDistance(MID)){
            
            if(World::GetInstance()->Timer(*this, 1000000.0)){
                
                itemQueue bullet;
                bullet.properties["PosX"] = std::to_string(objectSprite.getPosition().x);
                bullet.properties["PosY"] = std::to_string(objectSprite.getPosition().y - 6);
                bullet.properties["itemType"] = "EnemyBlip";
                bullet.properties["Direction"] = std::to_string(fireDir);
                World::GetInstance()->WorldScene.objectContainer->Queue.push_back(bullet);
                
                bullet.properties["Direction"] = std::to_string(fireDir -10);
                World::GetInstance()->WorldScene.objectContainer->Queue.push_back(bullet);
                
                bullet.properties["Direction"] = std::to_string(fireDir +10);
                World::GetInstance()->WorldScene.objectContainer->Queue.push_back(bullet);
                    
            }
            
        }
        
    }
    
    void Enemy::Draw(sf::RenderTarget& window){
        
        World::GetInstance()->DrawObject(objectSprite);
    }

	void Enemy::TargetPlayer() {

		fireDir = GetAngle(objectSprite.getPosition(), World::GetInstance()->WorldScene.playerPtr->objectSprite.getPosition());
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


		f2 = &Boss::Behavior3;
		f3 = &Boss::Behavior4;
		f4 = &Boss::Behavior2;

		tempList.push_back(f2);
		tempList.push_back(f3);
		tempList.push_back(f4);

		for (int i = 1; i < BehaviorList.size(); i++) {

			BehaviorList[i] = tempList[RandomNumber(2,0)];
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
		if (World::GetInstance()->Timer(*this, NORMAL, NODELAY)) CreateClone(objectSprite, "tx_bosses.png");

	}

	void Boss::FollowPlayer() {

		vel.x = 0;
		vel.y = -speed;

		//set move to player
		RotateVector(vel, -(GetAngle(objectSprite.getPosition(), World::GetInstance()->WorldScene.playerPtr->objectSprite.getPosition())));
		objectSprite.move(vel.x, vel.y + velZ);

	}

	void Mozza::Behavior1() {

		//8 direction continous shot

		// Dash & spreader

		Idle();

	}

	void Mozza::Behavior2() {

		// Spawn Spires

		Idle();

		if (phase == 0) {

			World::GetInstance()->WorldScene.audioContainer.PlaySFX("sfx_hauzercharge1");
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
				enemy.properties["PosX"] = std::to_string(World::GetInstance()->WorldScene.playerPtr->objectHitBox.getPosition().x - 50);
				enemy.properties["PosY"] = std::to_string(World::GetInstance()->WorldScene.playerPtr->objectHitBox.getPosition().y);
				World::GetInstance()->WorldScene.objectContainer->Queue.push_back(enemy);


				enemy.properties["PosX"] = std::to_string(World::GetInstance()->WorldScene.playerPtr->objectHitBox.getPosition().x + 50);
				enemy.properties["PosY"] = std::to_string(World::GetInstance()->WorldScene.playerPtr->objectHitBox.getPosition().y);
				World::GetInstance()->WorldScene.objectContainer->Queue.push_back(enemy);


				enemy.properties["PosX"] = std::to_string(World::GetInstance()->WorldScene.playerPtr->objectHitBox.getPosition().x);
				enemy.properties["PosY"] = std::to_string(World::GetInstance()->WorldScene.playerPtr->objectHitBox.getPosition().y - 50);
				World::GetInstance()->WorldScene.objectContainer->Queue.push_back(enemy);


				enemy.properties["PosX"] = std::to_string(World::GetInstance()->WorldScene.playerPtr->objectHitBox.getPosition().x);
				enemy.properties["PosY"] = std::to_string(World::GetInstance()->WorldScene.playerPtr->objectHitBox.getPosition().y + 50);
				World::GetInstance()->WorldScene.objectContainer->Queue.push_back(enemy);

				World::GetInstance()->ScreenShake(20);
				World::GetInstance()->WorldScene.audioContainer.PlaySFX("sfx_faintexp");
				World::GetInstance()->WorldScene.audioContainer.PlaySFX("sfx_stomp");

				NextMovement(); 
			}

		}
	}

	void Mozza::Behavior3() {

		// Toxic breath

		FollowPlayer();

		if (World::GetInstance()->Timer(*this, FAST)) {

			frame++;
			if (frame >= 3) frame = 1;
			objectSprite.setTextureRect(sf::IntRect(0, frame * 280 + 140, 140, 140));

		}

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

		if (World::GetInstance()->Timer(*this, FAST)) {

			frame++;
			if (frame >= 3) frame = 1;
			objectSprite.setTextureRect(sf::IntRect(0, frame * 280 + 140, 140, 140));

		}

		if (phase == 0) {

			World::GetInstance()->WorldScene.audioContainer.PlaySFX("sfx_hauzerbark");
			phase = 1;
			
		}

		else if (phase == 1) {

			if (World::GetInstance()->Timer(*this, 600000.0)) { 
				
				phase = 2;
				World::GetInstance()->WorldScene.audioContainer.PlaySFX("sfx_bossdash"); 
			
			}

		}

		else if (phase == 2) {

			MoveToCenter();
			if (World::GetInstance()->Timer(*this, 1600000.0)) phase = 3;
		}

		else if (phase == 3) {

			if (World::GetInstance()->Timer(*this, SLOW)) {

				itemQueue bullet;
				bullet.properties["PosX"] = std::to_string(objectSprite.getPosition().x);
				bullet.properties["PosY"] = std::to_string(objectSprite.getPosition().y - 6);
				bullet.properties["itemType"] = "EnemyBlip";
				bullet.properties["Speed"] = std::to_string(3);
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

		objectSprite.setTextureRect(sf::IntRect(0, 140, 140, 140));

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
			wings.setTextureRect(sf::IntRect(0, wingFrames*280, 140, 140));
			if (wingFrames >= 3) wingFrames = 0;

		}

		wings.setPosition(objectSprite.getPosition());

	}
    
	void Mozza::Draw(sf::RenderTarget& window) {

		World::GetInstance()->DrawObject(objectSprite);
		World::GetInstance()->DrawObject(healthBar);
		World::GetInstance()->DrawObject(bossName);
		World::GetInstance()->DrawObject(wings);
	}
    
    void Boss::Draw(sf::RenderTarget& window){
        
        World::GetInstance()->DrawObject(objectSprite);
        World::GetInstance()->DrawObject(objectHitBox);
        World::GetInstance()->DrawObject(healthBar);
        World::GetInstance()->DrawObject(bossName);
        
        
        
    }

    
    // Destructors
    
    Enemy::~Enemy()
    {
        --totalEnemies;
        
        std::cout << "Enemy destroyed" << std::endl;
        
        itemQueue particles;
        particles.properties["PosX"] = std::to_string(objectSprite.getPosition().x);
        particles.properties["PosY"] = std::to_string(objectSprite.getPosition().y);
        particles.properties["itemType"] = "DeathPoof";
        

        World::GetInstance()->WorldScene.objectContainer->Queue.push_back(particles);
        World::GetInstance()->WorldScene.objectContainer->Queue.push_back(particles);
        World::GetInstance()->WorldScene.objectContainer->Queue.push_back(particles);
        
        particles.properties["itemType"] = "DeathBoom";
        World::GetInstance()->WorldScene.objectContainer->Queue.push_back(particles);
        
    }
    
    Boss::~Boss(){

		//MovementList.clear();
		//AttackList.clear();
    
    }

	HauzerSpire::~HauzerSpire() {

	}
    
    Mozza::~Mozza(){
        
    }
    
    EnemyNode::~EnemyNode(){
        
    }
    
    Actor::~Actor(){
        
        
    }
    
    Slime::~Slime(){
        
    }
    
    Star::~Star(){
        
        
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
        objectShadow.setTexture((World::GetInstance()->WorldScene.textureContainer.SetTexture("tx_misc.png")));
        objectShadow.setTextureRect(sf::IntRect(0, 0, 15, 5));
        objectShadow.setOrigin(7,2);
        objectShadow.setPosition(objectSprite.getPosition());
        
    }
    
    void Object::UpdateShadow(){
        
        objectShadow.setPosition(objectSprite.getPosition().x,objectSprite.getPosition().y + posZ);
    }
    
    void Enemy::isDamaged(int damage){
        
        if(active){
            
            if(moveType == NORMAL) vel.y = 0;
            
            int newDamage = GetRandDmg(damage);
            health -= newDamage;
            hurt = true;
        }
        
        itemQueue particles;
        particles.properties["PosX"] = std::to_string(objectSprite.getPosition().x);
        particles.properties["PosY"] = std::to_string(objectSprite.getPosition().y);
        particles.properties["itemType"] = "DamageSpark";
        World::GetInstance()->WorldScene.objectContainer->Queue.push_back(particles);
        
        particles.properties["itemType"] = "Spark";
        World::GetInstance()->WorldScene.objectContainer->Queue.push_back(particles);
        World::GetInstance()->WorldScene.objectContainer->Queue.push_back(particles);
        World::GetInstance()->WorldScene.objectContainer->Queue.push_back(particles);
        
        World::GetInstance()->WorldScene.audioContainer.PlaySFX("sfx_fall");
        
    }
    
    void Enemy::isCollided(int var) {
        
        isDamaged(var);
            
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
    
    void CreateClone(sf::Sprite& sprite,std::string type){
        
        itemQueue clone;
        clone.properties["itemType"] = "SpriteClone";
        clone.properties["PosX"] = std::to_string(sprite.getPosition().x);
        clone.properties["PosY"] = std::to_string(sprite.getPosition().y);
        
        if (type == "") clone.properties["Sprite"] = "tx_projectiles.png";
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
        
        
        return sqrt((objectShadow.getPosition().x - World::GetInstance()->WorldScene.playerPtr->objectShadow.getPosition().x)*(objectShadow.getPosition().x - World::GetInstance()->WorldScene.playerPtr->objectShadow.getPosition().x) + (objectShadow.getPosition().y - World::GetInstance()->WorldScene.playerPtr->objectShadow.getPosition().y)*(objectShadow.getPosition().y - World::GetInstance()->WorldScene.playerPtr->objectShadow.getPosition().y)) <= distance ? true : false;
        
        
    }


	void ObjectCall(const std::unique_ptr<Entity::Object>& a) {

		std::cout << "got object ..." << a->vel.y << std::endl;

	}

    
    
    
}


