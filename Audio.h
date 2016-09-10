//
//  Audio.h
//  HardSoda
//
//  Created by Darion McCoy on 4/11/15.
//  Copyright (c) 2015 Darion. All rights reserved.
//

#ifndef __HardSoda__Audio__
#define __HardSoda__Audio__

#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <dirent.h>

struct AudioItem{
    
    sf::Sound sample;
    sf::SoundBuffer buffer;
    
};

class Audio
{
    
    public:
        Audio();
        ~Audio();
        void PlaySFX(std::string soundName);
        void PlayMusic(std::string musicName = "map1");
        void Update();
        sf::Music music;
        std::map<std::string,AudioItem> AudioContainer;
        bool limiter;
    
};

typedef std::map<std::string,AudioItem>::iterator audioIter;




#endif /* defined(__HardSoda__Audio__) */
