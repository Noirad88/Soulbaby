//
//  Textures.h
//  sdev
//
//  Created by Darion McCoy on 1/13/14.
//  Copyright (c) 2014 D man. All rights reserved.
//

#ifndef __sdev__Textures__
#define __sdev__Textures__

#include <iostream>
#include <SFML/Graphics.hpp>
#include <dirent.h>
#include <sstream>
#include <string>


namespace Textures
{
    class Shader
    {
        
    public:
        Shader();
        ~Shader();
        sf::Clock clock;
        sf::Shader shader;
        sf::Sprite mSprite;
        sf::Sprite object;
        sf::Sprite sprite;
        mutable sf::RenderTexture renderTexture;
        sf::Texture objectTexture;
        sf::Texture distortionMap;
        float transparency = 10.0;
        virtual void Update();

    };
    
    class WaveShader : public Shader
    {
        
    public:
        WaveShader();
        ~WaveShader();
        void Update();
        
    };
    
    class RedShader : public Shader
    {
        
    public:
        RedShader();
        ~RedShader();
        void Update();
        
    };
    
    class DamageShader : public Shader
    {
        
    public:
        DamageShader();
        ~DamageShader();
        void Update();
        
    };
    
    class TextureContainer
    {
        
    public:
        TextureContainer();
        void Update();
        sf::Texture& SetTexture(std::string temp_name);
        sf::Font& GetFont(std::string font = "");
        sf::Font GameFont;
		sf::Font GameFont2;
        sf::Texture* SetTextureForShapes(std::string temp_name);
        std::map<std::string,sf::Texture*> textureMap;
        bool set = false;
        static float tick;
        WaveShader waveShader;
        RedShader redShader;
        std::map<std::string,sf::Shader&> shaders;

        DamageShader dmgShader;
		std::string dir;
        
        
        
    };
    
    typedef std::map<std::string,sf::Texture*> ::iterator textureIter;
    

}


#endif /* defined(__sdev__Textures__) */