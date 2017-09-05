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

	};

	class DimShader : public Shader
	{

	public:
		DimShader();
		~DimShader();
	};

	class GlitchShader : public Shader
	{

	public:
		GlitchShader();
		~GlitchShader();
		void Update();
		float mode = 1.0;
		float opac;

	};

	class ChargeShader : public Shader
	{

	public:
		ChargeShader();
		~ChargeShader();
		void Update();

	};
    
    class RedShader : public Shader
    {
        
    public:
        RedShader();
        ~RedShader();
        
    };
    
    class DamageShader : public Shader
    {
        
    public:
        DamageShader();
        ~DamageShader();
        void Update();
        
    };

	class TestShader : public Shader
	{

	public:
		TestShader();
		void Update();
		~TestShader();
		float tick = 0;

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
		sf::MemoryInputStream Fontdata;
		sf::MemoryInputStream FontdataSmall;
		static sf::View* gameView;
		sf::Font GameFont;
		sf::Font GameFontSmall;
		bool dimWorld = false;

        sf::Texture* SetTextureForShapes(std::string temp_name);
        static std::map<std::string,sf::Texture*> textureMap;
		static std::map<std::string, sf::MemoryInputStream> shaderMap;

        bool set = false;
        static float tick;

		std::shared_ptr<WaveShader> waveShader;
		std::shared_ptr<RedShader> redShader;
		std::shared_ptr<WhiteShader> whiteShader;
		std::shared_ptr<DamageShader> dmgShader;
		std::shared_ptr<GlitchShader> glitchShader;
		std::shared_ptr<DimShader> dimShader;
		std::shared_ptr<ChargeShader> chargeShader;
		std::shared_ptr<TestShader> testShader;

        std::map<std::string,sf::Shader&> shaders;
		std::string dir;
        
        
        
    };
    
    typedef std::map<std::string,sf::Texture*> ::iterator textureIter;
    

}


#endif /* defined(__sdev__Textures__) */