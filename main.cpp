#include <SFML/Graphics.hpp>
#include <dirent.h>
#include <iostream>
#include <array>
#include <vector>
#include <functional>
#include <random>
#include <stddef.h>
#include <memory.h>
#include <math.h>

#define SCREEN_X sf::VideoMode::getDesktopMode().width
#define SCREEN_Y sf::VideoMode::getDesktopMode().height
#define WINDOW_X int((SCREEN_X/480)*480)
#define WINDOW_Y int((SCREEN_Y/270)*270)

// Use below for gifs
//#define WINDOW_X 1012
//#define WINDOW_Y 569.25
#define PROJ_DIR "C:/Users/Darion/Documents/Visual Studio 2015/Projects/SoulbabyPC/SoulbabyPC"

#include "resource.h"
#include "winuser.h"
#include "winbase.h"
#include "strsafe.h"
#include "wingdi.h"
#include "Container.h"
#include "Audio.h"
#include <fstream>
#include <iostream>

#include <sstream>
#include "ResourcePath.hpp"

World* World::Worldptr = nullptr;

std::string StreamToString(char* charData) {

	std::ostringstream stream;
	int i = 0;
	while (charData != NULL && charData[i] != '\0') {

		stream << charData[i];
		i += 2;

	}

	return 	stream.str();

}

void CreateResourceMap() {

	HRSRC str2 = FindResource(GetModuleHandle(NULL), MAKEINTRESOURCE(r_resourceList), "TXT");
	HGLOBAL data2 = LoadResource(NULL, str2);
	LPVOID ptr = GlobalLock(data2);
	char* charData = (char*)ptr;
	std::string list = StreamToString(charData);
	//std::cout << list << std::endl;
	int firstID = 144;
	size_t endi = list.find("Next", 4);


	//get the first ID and setup iterating

	for (int i = firstID; i < endi; i += 45) {


		//std::cout << i << " of " << endi << std::endl;
		int resourceID = stoi(list.substr(i, 3));
		std::string resourceName = list.substr(i - 32, list.find_first_of(' ', i - 32) - (i - 32));
		std::string resourceType = list.substr(i - 32, list.find_first_of('_', i - 32) - (i - 32));
		std::cout << resourceID << " | " << resourceName << " | " << resourceType << std::endl;


		//Check type and create to insert into correct container

		if (resourceType == "tx") {

			sf::Texture* texture;
			texture = new sf::Texture;
			texture->setSmooth(false);
			HRSRC source = FindResource(GetModuleHandle(NULL), MAKEINTRESOURCE(resourceID), "PNG");
			HGLOBAL data = LoadResource(NULL, source);
			sf::MemoryInputStream str;
			str.open(data, SizeofResource(GetModuleHandle(NULL), source));
			texture->loadFromStream(str);
			if (resourceName.find("battle") || resourceName.find("bg")) texture->setRepeated(true);
			World::GetInstance()->WorldScene.textureContainer.textureMap.insert(std::pair<std::string, sf::Texture*>(resourceName, texture));

		}

		else if (resourceType == "sfx") {

			sf::Sound sound;
			sf::SoundBuffer buff;
			AudioItem item;

			HRSRC wav = FindResource(GetModuleHandle(NULL), MAKEINTRESOURCE(resourceID), "WAVE");
			HGLOBAL data = LoadResource(NULL, wav);
			sf::MemoryInputStream str2;
			str2.open(data, SizeofResource(GetModuleHandle(NULL), wav));
			item.buffer.loadFromStream(str2);
			World::GetInstance()->WorldScene.audioContainer.AudioContainer.insert(std::pair<std::string, AudioItem>(resourceName, item));


		}

		else if (resourceType == "mus") {

			sf::Music sound;
			HRSRC wav = FindResource(GetModuleHandle(NULL), MAKEINTRESOURCE(resourceID), "OGG");
			HGLOBAL data2 = LoadResource(NULL, wav);
			sf::MemoryInputStream str2;
			str2.open(data2, SizeofResource(GetModuleHandle(NULL), wav));
			World::GetInstance()->WorldScene.audioContainer.MusicContainer.insert(std::pair<std::string, sf::MemoryInputStream>(resourceName, str2));

		}

		else if (resourceType == "ft") {

			HRSRC str2 = FindResource(GetModuleHandle(NULL), MAKEINTRESOURCE(resourceID), "TXT");
			HGLOBAL data2 = LoadResource(NULL, str2);
			LPVOID ptr = GlobalLock(data2);
			char* charData = (char*)ptr;
			sf::MemoryInputStream str3;
			str3.open(data2, SizeofResource(GetModuleHandle(NULL), str2));
			if(resourceName == "ft_apfont") World::GetInstance()->WorldScene.textureContainer.FontdataSmall = str3;
			else World::GetInstance()->WorldScene.textureContainer.Fontdata = str3;


		}

		else if (resourceType == "sh") {

			HRSRC str2 = FindResource(GetModuleHandle(NULL), MAKEINTRESOURCE(resourceID), "TXT");
			HGLOBAL data2 = LoadResource(NULL, str2);
			sf::MemoryInputStream str;
			str.open(data2, SizeofResource(GetModuleHandle(NULL), str2));
			//need to figure out where to keep txt files (no container)
			World::GetInstance()->WorldScene.textureContainer.shaderMap.insert(std::pair<std::string, sf::MemoryInputStream>(resourceName, str));
		}

		else if (resourceType == "lvl") {

			HRSRC str2 = FindResource(GetModuleHandle(NULL), MAKEINTRESOURCE(resourceID), "TXT");
			HGLOBAL data2 = LoadResource(NULL, str2);
			LPVOID ptr = GlobalLock(data2);
			char* charData = (char*)ptr;
			std::ostringstream stream;
			int i = 0;
			while (charData != NULL && charData[i] != '\0') {

				stream << charData[i];
				i++;

			}
			std::string lvldata = stream.str();
			World::GetInstance()->WorldScene.levelContainer->levelMap.insert(std::pair<std::string, std::string>(resourceName,lvldata));


		}

	}

}

int main()
{
	CreateResourceMap();

    // Create the main window
    sf::RenderWindow window(sf::VideoMode(WINDOW_X,WINDOW_Y), "SFML window", sf::Style::None);
	sf::VideoMode userMode = sf::VideoMode::getDesktopMode();
	std::cout << userMode.width << " x " << userMode.height << std::endl;

    window.setFramerateLimit(60);
    window.setVerticalSyncEnabled(true);
    window.setKeyRepeatEnabled(true);

    sf::Event event;
        
    sf::Clock clock;
    float timestamp = 0;
	
    World::GetInstance()->Setup(clock,window,event);

    // Start the game loop
    while (window.isOpen())
    {
        
        timestamp += clock.getElapsedTime().asMilliseconds();
        clock.restart();
        
        // Process events
        
        while (window.pollEvent(event))
        {
            
            // Close window : exit
            if (event.type == sf::Event::Closed)
            {
                
                window.close();
                
            }
            
            // Espace pressed : exit
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
            {
                
                window.close();
                
            }
            
        }
        
        window.clear(sf::Color::Color(0,23,0));
        World::GetInstance()->Run(event,double(timestamp),clock);
        window.setView(*World::GetInstance()->GetView());
        window.display();
        
        timestamp = clock.restart().asMicroseconds();
        
    }
    
    return EXIT_SUCCESS;
}
