//
//  Audio.cpp
//  HardSoda
//
//  Created by Darion McCoy on 4/11/15.
//  Copyright (c) 2015 Darion. All rights reserved.
//

#include "Audio.h"
#ifndef PROJ_DIR
#define PROJ_DIR "C:/Users/Darion/Documents/Visual Studio 2015/Projects/SoulbabyPC/SoulbabyPC/"
#endif

Audio::Audio(){
    
    //Uses dirent.h to open up the sfx folder directory, get all the sound file names, and create a map to add sound buffers and retrive with file name/key
    
	std::string dir = "C:/Users/Darion/Documents/Visual Studio 2015/Projects/SoulbabyPC/SoulbabyPC/";

    DIR *dip;
    dip = opendir("C:/Users/Darion/Documents/Visual Studio 2015/Projects/SoulbabyPC/SoulbabyPC/sfx");
    struct dirent *dit;
    
    while((dit = readdir(dip)) != NULL){
        
        std::string name = dit->d_name;
        sf::SoundBuffer buffer;
        std::size_t found = name.find("sfx_");
        AudioItem item;
        
        if(found!=std::string::npos){
        
            if(item.buffer.loadFromFile(dir + "sfx/" + name)){

                AudioContainer.insert(std::pair<std::string,AudioItem>(name,item));
                std::cout << name << " SFX loaded ... " << std::endl;
                
            }
        
        }

    }
    
    closedir(dip);
    
}

void Audio::PlaySFX(std::string soundName){
    
    audioIter iter = AudioContainer.find(soundName + ".wav");
    iter->second.sample.setBuffer(iter->second.buffer);
    //iter->second.sample.stop();
    iter->second.sample.setVolume(80);
    iter->second.sample.play();

}

void Audio::PlayMusic(std::string musicName){
    
	std::string dir = "C:/Users/Darion/Documents/Visual Studio 2015/Projects/SoulbabyPC/SoulbabyPC/";

    music.stop();
    music.openFromFile(dir + "music/" + musicName + ".ogg");
    music.setLoop(true);
    music.play();
    music.setVolume(50);
    
}

Audio::~Audio(){
    
    for(audioIter it = AudioContainer.begin(); it != AudioContainer.end(); it++){
        
        it->second.sample.resetBuffer();
        
    }
    
    
    AudioContainer.clear();
    music.stop();
    
    
}


