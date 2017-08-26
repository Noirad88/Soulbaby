//
//  Textures.cpp
//  sdev
//
//  Created by Darion McCoy on 1/13/14.
//  Copyright (c) 2014 D man. All rights reserved.
//

#include "Textures.h"
#include "ResourcePath.hpp"
#include <random>


namespace Textures{

	std::map<std::string, sf::Texture*> TextureContainer::textureMap;
	std::map<std::string, sf::MemoryInputStream> TextureContainer::shaderMap;
	sf::View* TextureContainer::gameView;
    
    float TextureContainer::tick = 0;
    
    Shader::Shader(){
        
    }
    
    WaveShader::WaveShader(){
       

		//shader.loadFromStream(TextureContainer::shaderMap.at("sh_sine"), TextureContainer::shaderMap.at("sh_wave"));
	
		//objectTexture = *TextureContainer::textureMap.at("tx_map");

        objectTexture.setRepeated(true);
        object.setTexture(objectTexture);
        object.setTextureRect(sf::IntRect(0,0,100,100));
        object.setScale(10, 10);
        
        // It is important to set repeated to true to enable scrolling upwards
        distortionMap.setRepeated(true);
        
        // Setting smooth to true lets us use small maps even on larger images
        distortionMap.setSmooth(true);
        
		//distortionMap = *TextureContainer::textureMap.at("tx_map");
        
        renderTexture.create(400, 300);
        
        sprite.setTexture(renderTexture.getTexture());
        sprite.setPosition(100, 150);
        sprite.setPosition(0,0);
    }
    
    RedShader::RedShader()
    {
		shader.loadFromStream(TextureContainer::shaderMap.at("sh_redoverlay"), sf::Shader::Fragment);
    }

	WhiteShader::WhiteShader()
	{
		shader.loadFromStream(TextureContainer::shaderMap.at("sh_whiteoverlay"), sf::Shader::Fragment);
	}

	GlitchShader::GlitchShader()
	{
		shader.loadFromStream(TextureContainer::shaderMap.at("sh_glitch"), sf::Shader::Fragment);
	}
    
    DamageShader::DamageShader()
    {
		shader.loadFromStream(TextureContainer::shaderMap.at("sh_damage"), sf::Shader::Fragment);

	}

    
    Shader::~Shader(){
        
    }
    
    RedShader::~RedShader(){
        
    }
    
    WaveShader::~WaveShader(){
        
    }

	GlitchShader::~GlitchShader() {

	}

	WhiteShader::~WhiteShader() {

	}
    
    DamageShader::~DamageShader(){
        
    }

	TextureContainer::~TextureContainer() {

	}
    
    void Shader::Update(){
        
    }
    
    void WaveShader::Update(){
        
        shader.setParameter("currentTexture", sf::Shader::CurrentTexture);
        shader.setParameter("distortionMapTexture", distortionMap);
        shader.setParameter("time", TextureContainer::tick);
        shader.setParameter("distortionFactor",0);
        shader.setParameter("wave_amplitude",20,20);
        shader.setParameter("wave_phase",TextureContainer::tick);
        shader.setParameter("riseFactor",.0f);
        
    }
    
    void RedShader::Update(){
        
        shader.setParameter("currentTexture", sf::Shader::CurrentTexture);
    }

	void WhiteShader::Update() {

		shader.setParameter("currentTexture", sf::Shader::CurrentTexture);
	}

	void GlitchShader::Update() {

		float r;
		float g;
		float b;

		if (mode == 1.0) {

			std::random_device rd;
			std::mt19937 mt(rd());
			std::uniform_int_distribution<int> rand(0, 20);

			r = rand(mt) * 0.05;
			g = rand(mt) * 0.05;
			b = rand(mt) * 0.05;
			sf::Vector2f view = TextureContainer::gameView->getCenter();

			shader.setParameter("r", r);
			shader.setParameter("g", g);
			shader.setParameter("b", b);

		}

		else {

			shader.setParameter("r", opac);
			shader.setParameter("g", opac);
			shader.setParameter("b", opac);
			if(opac > 0.0) opac-=0.005;


		} 

		shader.setParameter("mode", mode);
		shader.setParameter("currentTexture", sf::Shader::CurrentTexture);

	}
    
    void DamageShader::Update(){
        
        shader.setParameter("currentTexture", sf::Shader::CurrentTexture);
    }
    
    TextureContainer::TextureContainer(){
      

		/*
		dir = "C:/Users/Darion/Documents/Visual Studio 2015/Projects/SoulbabyPC/SoulbabyPC/";
        //Uses dirent.h to open up the textures directory, get all texture file names, create a map of file names/textures
        
        DIR *dip;
        dip = opendir("C:/Users/Darion/Documents/Visual Studio 2015/Projects/SoulbabyPC/SoulbabyPC/textures");
        struct dirent *dit;
        
        while((dit = readdir(dip)) != NULL){
            
            std::string name = dit->d_name;
            sf::Texture* newTexture;
            newTexture = new sf::Texture;
            newTexture->setSmooth(false);
            std::size_t found = name.find("tx_");
            
            if(found!=std::string::npos){
                
            
                if(newTexture->loadFromFile(dir + "textures/" + name)){
    
                    if(name.find("battle")) newTexture->setRepeated(true);
                    textureMap.insert(std::pair<std::string,sf::Texture*>(name,newTexture));
                    std::cout << name << " texture loaded ... " << std::endl;
                
                }
            }
    
        
            
        }
        
        closedir(dip);
        
        DIR *dip2;
        dip2 = opendir("C:/Users/Darion/Documents/Visual Studio 2015/Projects/SoulbabyPC/SoulbabyPC/levels");
        struct dirent *dit2;
        
        while((dit2 = readdir(dip2)) != NULL){
            
            std::string name = dit2->d_name;
            sf::Texture* newTexture;
            newTexture = new sf::Texture;
            newTexture->setSmooth(false);
            std::size_t found = name.find("tx_");
            
            if(found!=std::string::npos){
                
                
                if(newTexture->loadFromFile(dir + "levels/" + name)){
                    
                    if(name.find("bg")) newTexture->setRepeated(true);
                    textureMap.insert(std::pair<std::string,sf::Texture*>(name,newTexture));
                    std::cout << name << " texture loaded ... " << std::endl;
                    
                }
            }
            
            
            
        }
        
        closedir(dip2);
        
        GameFont.loadFromFile(dir + "fonts/basis33.ttf");
		GameFont2.loadFromFile(dir + "fonts/apfont.ttf");
        const_cast<sf::Texture&>(GameFont.getTexture(16)).setSmooth(false);
		const_cast<sf::Texture&>(GameFont2.getTexture(16)).setSmooth(false);
        
        shaders.insert(std::pair<std::string,sf::Shader&>("redShader",redShader.shader));
        shaders.insert(std::pair<std::string,sf::Shader&>("waveShader",waveShader.shader));
		shaders.insert(std::pair<std::string, sf::Shader&>("whiteShader", whiteShader.shader));
		*/
    
    }

	void TextureContainer::CreateShaderInstances() {

		waveShader = std::shared_ptr<WaveShader>(new WaveShader);
		redShader = std::shared_ptr<RedShader>(new RedShader);
		whiteShader = std::shared_ptr<WhiteShader>(new WhiteShader);
		dmgShader = std::shared_ptr<DamageShader>(new DamageShader);
		glitchShader = std::shared_ptr<GlitchShader>(new GlitchShader);


		shaders.insert(std::pair<std::string, sf::Shader&>("redShader", redShader.get()->shader));
		shaders.insert(std::pair<std::string, sf::Shader&>("waveShader", waveShader.get()->shader));
		shaders.insert(std::pair<std::string, sf::Shader&>("whiteShader", whiteShader.get()->shader));
		shaders.insert(std::pair<std::string, sf::Shader&>("damageShader", dmgShader.get()->shader));
		shaders.insert(std::pair<std::string, sf::Shader&>("glitchShader", glitchShader.get()->shader));


		GameFont.loadFromStream(Fontdata);
		const_cast<sf::Texture&>(GameFont.getTexture(16)).setSmooth(false);

	}
    
    
    sf::Texture& TextureContainer::SetTexture(std::string temp_name){
        
        //std::cout << temp_name << std::endl;
        return *textureMap.at(temp_name);
    
    }
    
    sf::Font& TextureContainer::GetFont(std::string font){
        
        //std::cout << temp_name << std::endl;
        if(font == "") return GameFont;

		else return GameFont2;
        
    }
    
    sf::Texture* TextureContainer::SetTextureForShapes(std::string temp_name){
        
        return textureMap.at(temp_name);
        
    }
    
    void TextureContainer::Update(){
        
        waveShader->Update();
		glitchShader->Update();
        tick += 0.005;
        
    }
        
    
}