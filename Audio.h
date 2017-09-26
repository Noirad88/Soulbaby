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
        void PlaySFX(std::string soundName, bool multipleInstances = 1);
        void PlayMusic(std::string musicName = "map1");
		void ToggleMusic();
		void playLoopedMusic(float length, std::string musicName = "map1");
        void Update();
        sf::Music music;
        std::map<std::string,AudioItem> AudioContainer;
		std::map<std::string, sf::MemoryInputStream> MusicContainer;

        bool limiter;
		bool mute = false;
    
};


class LoopingMusic : public sf::SoundStream
{
public:

	////////////////////////////////////////////////////////////
	/// \brief Default constructor
	///
	////////////////////////////////////////////////////////////
	LoopingMusic(sf::InputSoundFile& file) :
		m_file(file),
		m_loopBegin(0),
		m_loopEnd(0),
		m_loopCurrent(0)
	{
		// Resize the internal buffer so that it can contain 1 second of audio samples
		m_samples.resize(m_file.getSampleRate() * m_file.getChannelCount());

		// Initialize the stream
		SoundStream::initialize(m_file.getChannelCount(), m_file.getSampleRate());
	}

	////////////////////////////////////////////////////////////
	/// \brief Destructor
	///
	////////////////////////////////////////////////////////////
	~LoopingMusic()
	{
		// We must stop before destroying the file
		stop();
	}

	////////////////////////////////////////////////////////////
	/// \brief Get the position of the beginning of the sound's looping sequence
	///
	/// \return Loop begin position
	///
	/// \see getLoopEnd, setLoopPoints
	///
	////////////////////////////////////////////////////////////
	sf::Time getLoopBegin() const;

	////////////////////////////////////////////////////////////
	/// \brief Get the position of the end of the sound's looping sequence
	///
	/// \return Loop end position
	///
	/// \see getLoopBegin, setLoopPoints
	///
	////////////////////////////////////////////////////////////
	sf::Time getLoopEnd() const;

	////////////////////////////////////////////////////////////
	/// \brief Sets the beginning and end of the sound's looping sequence using sf::Time
	///
	/// Loop points allow one to specify a pair of positions such that, when the music
	/// is enabled for looping, it will seamlessly seek to begin whenever it encounters end.
	/// The input values are clamped to the duration of the sound. If they are the same,
	/// then a closed loop cannot be formed, and this function will "reset" the loop to the
	/// full length of the sound. Note that the implicit "loop points" from the end to the
	/// beginning of the stream are still honored. Because of this, "reverse" loop ranges,
	/// where end comes before begin, are allowed, and will cause the sound to loop everything
	/// "outside" of the specified range. This function can be safely called at any point
	/// after a stream is opened, and will be applied to a playing sound without affecting
	/// the current playing offset.
	///
	/// \param begin    The offset of the beginning of the loop. Can be any time point within the sound's length
	/// \param end      The offset of the end of the loop. If Time::Zero is passed, it defaults to the end of the sound
	///
	/// \see getLoopBegin, getLoopEnd
	///
	////////////////////////////////////////////////////////////
	void setLoopPoints(sf::Time begin, sf::Time end);

protected:

	////////////////////////////////////////////////////////////
	/// \brief Request a new chunk of audio samples from the stream source
	///
	/// This function fills the chunk from the next samples
	/// to read from the audio file.
	///
	/// \param data Chunk of data to fill
	///
	/// \return True to continue playback, false to stop
	///
	////////////////////////////////////////////////////////////
	virtual bool onGetData(Chunk& data);

	////////////////////////////////////////////////////////////
	/// \brief Change the current playing position in the stream source
	///
	/// \param timeOffset New playing position, from the beginning of the music
	///
	////////////////////////////////////////////////////////////
	virtual void onSeek(sf::Time timeOffset);

private:

	////////////////////////////////////////////////////////////
	// Member data
	////////////////////////////////////////////////////////////
	sf::InputSoundFile&    m_file;        ///< The streamed music file
	sf::Time               m_duration;    ///< Music duration
	std::vector<sf::Int16> m_samples;     ///< Temporary buffer of samples
	sf::Mutex              m_mutex;       ///< Mutex protecting the data
	sf::Uint64             m_loopBegin;   ///< First sample position
	sf::Uint64             m_loopEnd;     ///< Last sample position
	sf::Uint64             m_loopCurrent; ///< Current sample position
};


typedef std::map<std::string,AudioItem>::iterator audioIter;




#endif /* defined(__HardSoda__Audio__) */
