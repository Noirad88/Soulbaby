#include <SFML/Graphics.hpp>
#include <dirent.h>
#include <iostream>
#include <array>
#include <vector>
#include <functional>
#include <random>
#include <stddef.h>

#define WINDOW_X 480*2

#define WINDOW_Y 270*2
#define SCREEN_X sf::VideoMode::getDesktopMode().width
#define SCREEN_Y sf::VideoMode::getDesktopMode().height
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
#include "ResourcePath.hpp"

World* World::Worldptr = nullptr;

int main()
{

	// Getting resource list
	HRSRC str2 = FindResource(GetModuleHandle(NULL), MAKEINTRESOURCE(r_resourceList), "TXT");
	HGLOBAL data2 = LoadResource(NULL, str2);
	LPVOID ptr = GlobalLock(data2);
	char *charData = (char*)ptr;
	int s = 4354*3;
	char charst[4354 * 3];
	std::string list;
	for (int i = 0; i < s; i += 2) list.push_back(charData[i]);	
	//std::cout << list << std::endl;

	int firstID = 142;
	size_t endi = list.find("Next", 4);

	//get the first ID and setup iterating

	for (int i = firstID; i < endi; i+= 45) {
		std::cout << i << " of " << endi << std::endl;
		int resourceID = stoi(list.substr(i,3));
		std::string resourceName = list.substr(i-32, list.find_first_of(' ',i-32) - (i-32));
		std::string resourceType = list.substr(i - 32, list.find_first_of('_', i - 32) - (i - 32));
		std::cout << resourceID << " | " << resourceName << " | " << resourceType << std::endl;


		//Check type and create to insert into correct container

		if (resourceType == "tx") {

			sf::Texture* texture;
			texture = new sf::Texture;
			texture->setSmooth(false);
			HRSRC source = FindResource(GetModuleHandle(NULL), MAKEINTRESOURCE(resourceID), "PNG");
			if (source) std::cout << source << std::endl;
			HGLOBAL data = LoadResource(NULL, source);
			if (data) std::cout << data << std::endl;
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
			if (wav) std::cout << wav << std::endl;
			HGLOBAL data = LoadResource(NULL, wav);
			if (data) std::cout << data2 << std::endl;
			sf::MemoryInputStream str2;
			str2.open(data, SizeofResource(GetModuleHandle(NULL), wav));
			item.buffer.loadFromStream(str2);
			World::GetInstance()->WorldScene.audioContainer.AudioContainer.insert(std::pair<std::string, AudioItem>(resourceName, item));


		}

		else if (resourceType == "mus") {

			sf::Music sound;
			HRSRC wav = FindResource(GetModuleHandle(NULL), MAKEINTRESOURCE(resourceID), "OGG");
			if (wav) std::cout << wav << std::endl;
			HGLOBAL data2 = LoadResource(NULL, wav);
			if (data2) std::cout << data2 << std::endl;
			sf::MemoryInputStream str2;
			str2.open(data2, SizeofResource(GetModuleHandle(NULL), wav));
			World::GetInstance()->WorldScene.audioContainer.MusicContainer.insert(std::pair<std::string,sf::MemoryInputStream>(resourceName, str2));

		}


		else if (resourceType == "font") {

		}

		else if (resourceType == "txt") {

			HRSRC str2 = FindResource(GetModuleHandle(NULL), MAKEINTRESOURCE(resourceID), "TXT");
			HGLOBAL data2 = LoadResource(NULL, str2);
			sf::MemoryInputStream str;
			str.open(data2, SizeofResource(GetModuleHandle(NULL), str2));
			//need to figure out where to keep txt files (no container)
			World::GetInstance()->WorldScene.textureContainer.shaderMap.insert(std::pair<std::string, sf::MemoryInputStream>(resourceName, str));

		}

	}

    // Create the main window
    sf::RenderWindow window(sf::VideoMode(WINDOW_X,WINDOW_Y), "SFML window", sf::Style::Titlebar);
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
        World::GetInstance()->Run(event,timestamp,clock);
        window.setView(*World::GetInstance()->GetView());
        window.display();
        
        timestamp = clock.restart().asMicroseconds();
        
    }
    
    return EXIT_SUCCESS;
}
