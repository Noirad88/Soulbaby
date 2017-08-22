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
#include <memory>



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

	class WhiteShader : public Shader
	{

	public:
		WhiteShader();
		~WhiteShader();
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
		~TextureContainer();
        void Update();
		void CreateShaderInstances();
        sf::Texture& SetTexture(std::string temp_name);
        sf::Font& GetFont(std::string font = "");
        sf::Font GameFont;
		sf::Font GameFont2;
        sf::Texture* SetTextureForShapes(std::string temp_name);
        static std::map<std::string,sf::Texture*> textureMap;
		static std::map<std::string, sf::MemoryInputStream> shaderMap;

        bool set = false;
        static float tick;

		std::shared_ptr<WaveShader> waveShader;
		std::shared_ptr<RedShader> redShader;
		std::shared_ptr<WhiteShader> whiteShader;
		std::shared_ptr<DamageShader> dmgShader;



        std::map<std::string,sf::Shader&> shaders;

		std::string dir;
        
        
        
    };
    
    typedef std::map<std::string,sf::Texture*> ::iterator textureIter;
    

}


#endif /* defined(__sdev__Textures__) */