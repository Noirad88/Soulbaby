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
    /*
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
	*/
    
}

void Audio::PlaySFX(std::string soundName, bool multipleInstances){
    
    audioIter iter = AudioContainer.find(soundName);

	if (multipleInstances == false) {

		if (AudioContainer.at(soundName).sample.getStatus() == sf::Sound::Status::Stopped) {

			iter->second.sample.setBuffer(iter->second.buffer);
			iter->second.sample.setVolume(80);
			iter->second.sample.play();
		}

	}

	else {

		iter->second.sample.setBuffer(iter->second.buffer);
		iter->second.sample.setVolume(80);
		iter->second.sample.play();

	}

}

void Audio::PlayMusic(std::string musicName){
    
    music.stop();
	music.openFromStream(MusicContainer.at(musicName));
    music.setLoop(true);
    music.play();
    music.setVolume(100);
    
}

Audio::~Audio(){
    
    for(audioIter it = AudioContainer.begin(); it != AudioContainer.end(); it++){
        
        it->second.sample.resetBuffer();
        
    }
    
    
    AudioContainer.clear();
    music.stop();
    
    
}


sf::Time LoopingMusic::getLoopBegin() const
{
	unsigned int sampleRate = getSampleRate();
	unsigned int channelCount = getChannelCount();

	if (sampleRate && channelCount)
		return sf::seconds(static_cast<float>(m_loopBegin) / sampleRate / channelCount);

	return sf::Time::Zero;
}

sf::Time LoopingMusic::getLoopEnd() const
{
	unsigned int sampleRate = getSampleRate();
	unsigned int channelCount = getChannelCount();

	if (sampleRate && channelCount)
		return sf::seconds(static_cast<float>(m_loopEnd) / sampleRate / channelCount);

	return sf::Time::Zero;
}

void LoopingMusic::setLoopPoints(sf::Time begin, sf::Time end)
{
	std::size_t sampleCount = m_file.getSampleCount();

	// Reset to playing whole stream
	if (begin == end)
	{
		m_loopBegin = 0;
		m_loopEnd = sampleCount;
		return;
	}

	unsigned int sampleRate = getSampleRate();
	unsigned int channelCount = getChannelCount();

	begin = sf::microseconds(std::max(begin.asMicroseconds(), 0ll));
	end = sf::microseconds(std::max(end.asMicroseconds(), 0ll));

	sf::Uint64 loopBegin = static_cast<sf::Uint64>(begin.asSeconds() * sampleRate * channelCount);
	sf::Uint64 loopEnd = static_cast<sf::Uint64>(end.asSeconds() * sampleRate * channelCount);

	loopBegin = std::min(loopBegin, static_cast<sf::Uint64>(sampleCount));
	loopEnd = std::min(loopEnd, static_cast<sf::Uint64>(sampleCount));

	if (!loopEnd)
		loopEnd = sampleCount;

	loopBegin -= loopBegin % channelCount;
	loopEnd -= loopEnd % channelCount;

	m_loopBegin = loopBegin;
	m_loopEnd = loopEnd;
}

bool LoopingMusic::onGetData(Chunk& data)
{
	sf::Lock lock(m_mutex);

	// Fill the chunk parameters
	data.samples = &m_samples[0];
	data.sampleCount = static_cast<std::size_t>(m_file.read(&m_samples[0], m_samples.size()));

	if (!data.sampleCount)
	{
		// Check if we have reached the end of the audio file
		return data.sampleCount == m_samples.size();
	}

	// Standard non-looping case
	if (m_loopBegin == m_loopEnd || !getLoop())
	{
		m_loopCurrent += data.sampleCount;

		// Check if we have reached the end of the audio file
		return data.sampleCount == m_samples.size();
	}

	// We are looping

	// Check for a loop transition
	if (m_loopCurrent + data.sampleCount > m_loopEnd)
	{
		std::size_t endSampleCount = std::min(static_cast<std::size_t>(m_loopEnd - m_loopCurrent), data.sampleCount);
		std::size_t beginSampleCount = m_samples.size() - endSampleCount;

		// Jump back to the beginning of the sequence
		m_file.seek(m_loopBegin);

		// Fill the rest of the buffer with the data at the beginning
		beginSampleCount = static_cast<std::size_t>(m_file.read(&m_samples[endSampleCount], beginSampleCount));

		data.sampleCount = endSampleCount + beginSampleCount;
		m_loopCurrent = m_loopBegin + beginSampleCount;
	}
	else
	{
		m_loopCurrent += data.sampleCount;
	}

	return data.sampleCount == m_samples.size();
}

void LoopingMusic::onSeek(sf::Time timeOffset)
{
	sf::Lock lock(m_mutex);

	sf::Uint64 sampleOffset = static_cast<sf::Uint64>(timeOffset.asSeconds() * getSampleRate() * getChannelCount());
	sampleOffset -= sampleOffset % getChannelCount();

	m_loopCurrent = sampleOffset;
	m_file.seek(sampleOffset);
}

void Audio::playLoopedMusic(float length, std::string musicName)
{
	std::string dir = "C:/Users/Darion/Documents/Visual Studio 2015/Projects/SoulbabyPC/SoulbabyPC/music/";

	// Load an ogg music file
	sf::InputSoundFile musicFile;
	musicFile.openFromFile(dir + musicName + ".ogg");
		

	// Create our looping music using the music file
	LoopingMusic music(musicFile);

	// Setup the loop points
	music.setLoopPoints(sf::seconds(0.0f), sf::seconds(2.0f));
	float duration = 13.496f - 5.579f;

	// Display sound informations
	std::cout << "orchestral.ogg (Music, Loop):" << std::endl;
	std::cout << " " << musicFile.getDuration().asSeconds() << " seconds" << std::endl;
	std::cout << " " << musicFile.getSampleRate() << " samples / sec" << std::endl;
	std::cout << " " << musicFile.getChannelCount() << " channels" << std::endl;
	std::cout << " Looping from " << music.getLoopBegin().asSeconds() <<
		" sec to " << music.getLoopEnd().asSeconds() << " sec" << std::endl;

	music.setLoop(true);
	music.play();

	// Loop while the sound is playing. Make sure it stops after a few loops.
	int loops = 3;
	while (music.getStatus() == sf::Sound::Playing)
	{
		// Leave some CPU time for other processes
		sf::sleep(sf::milliseconds(100));
		float time = music.getPlayingOffset().asSeconds();
		if (time >= 13.496f + 3.f * duration)
		{
			loops = 0;
			music.stop();
		}
		else if (time >= 13.496f + 2.f * duration)
		{
			loops = 1;
		}
		else if (time >= 13.496f + 1.f * duration)
		{
			loops = 2;
		}

		// Display the playing position
		//std::cout << "\rPlaying... " << std::fixed << std::setprecision(2) << time << " sec with " << loops << " loops to go        ";
		//std::cout << std::flush;
	}
}



