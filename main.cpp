#include <SFML/Graphics.hpp>
#include <dirent.h>
#include <iostream>
#include <array>
#include <vector>
#include <functional>
#include <random>

#define WINDOW_X 480*3
#define WINDOW_Y 270*3
#define PROJ_DIR "C:/Users/Darion/Documents/Visual Studio 2015/Projects/SoulbabyPC/SoulbabyPC"

#include "Container.h"
#include "Audio.h"

// Here is a small helper for you ! Have a look.
#include "ResourcePath.hpp"

World* World::Worldptr = nullptr;

int main()
{
    
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
        
        window.clear(sf::Color::Color(0,0,0));
        World::GetInstance()->Run(event,timestamp,clock);
        window.setView(*World::GetInstance()->GetView());
        window.display();
        
        timestamp = clock.restart().asMicroseconds();
        
    }
    
    return EXIT_SUCCESS;
}
