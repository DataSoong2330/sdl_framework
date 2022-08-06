#include "../header/Logfile.hpp"
#include "../header/SoundManager.hpp"

// initializes the sound manager singleton
SoundManager* SoundManager::soundManager = 0;

// lazy ctor does nothing
SoundManager::SoundManager()
{

}

// dtor calls the quit method
SoundManager::~SoundManager()
{
    // frees all the musics
    for(auto music : this->musics)
        Mix_FreeMusic(music.second);

    // clears the map
    this->musics.clear();

    // frees all the sfxs
    for(auto sfx : this->sfxs)
        Mix_FreeChunk(sfx.second);

    // clears the map
    this->sfxs.clear();

    // quits the audio mixer
    Mix_Quit();
}

// initializes the sound with frequency, format, channels and chunksize (in bytes)
bool SoundManager::init(int flags, unsigned freq, Uint16 format, unsigned channels, unsigned chunksize)
{
    bool success = false;

    if(Mix_Init(flags))
    {
        Logfile::Instance()->Textout(FONTCOLORS::GREEN, "Sound Manager successfully initialized\n");

        Mix_OpenAudio(freq, format, channels, chunksize);
        success = true;
    }
    else
    {
        Logfile::Instance()->Textout(FONTCOLORS::RED, "Could not init the Sound Manager!\n");
        Logfile::Instance()->Textout(FONTCOLORS::RED, Mix_GetError());
    }

    return success;
}

// loads a sound or music file
bool SoundManager::load(std::string fileName, std::string soundID, SoundType soundType)
{
    std::string tempString = "";
    bool success = false;

    if(soundType == SoundType::SOUND_MUSIC)
    {
        tempString = "Music from " + fileName + " could ";

        Mix_Music* music = Mix_LoadMUS(fileName.c_str());

        if(music != NULL)
        {
            tempString += "be loaded\n";
            Logfile::Instance()->Textout(FONTCOLORS::GREEN, tempString.c_str());

            this->musics[soundID] = music;
            success = true;
        }
        else
        {
            tempString += "not be loaded\n";
            Logfile::Instance()->Textout(FONTCOLORS::RED, tempString.c_str());
            Logfile::Instance()->Textout(FONTCOLORS::RED, Mix_GetError());
        }
    }
    else if(soundType == SoundType::SOUND_SFX)
    {
        tempString = "SFX from " + fileName + " could ";

        Mix_Chunk* chunk = Mix_LoadWAV(fileName.c_str());

        if(chunk != NULL)
        {
            tempString += "be loaded\n";
            Logfile::Instance()->Textout(FONTCOLORS::GREEN, tempString.c_str());

            //this->sfxs[soundID] = chunk;
            success = true;
        }
        else
        {
            tempString += "not be loaded\n";
            Logfile::Instance()->Textout(FONTCOLORS::RED, tempString.c_str());
            Logfile::Instance()->Textout(FONTCOLORS::RED, Mix_GetError());
        }
    }

    return success;
}

// plays a sound loop-times
void SoundManager::playSound(std::string soundID, int loop)
{
    Mix_PlayChannel(-1, this->sfxs[soundID], loop);
}

// plays a music loop-times
void SoundManager::playMusic(std::string musicID, int loop)
{
    if(Mix_PlayingMusic() == 0)
        Mix_PlayMusic(this->musics[musicID], loop);
}

// pauses a music file if started
void SoundManager::pauseMusic()
{
    if(Mix_PlayingMusic() == 1)
        Mix_PauseMusic();
}

// resumes a music file if paused
void SoundManager::resumeMusic()
{
    if(Mix_PausedMusic() == 1)
        Mix_ResumeMusic();
}

// stops a music file if playing
void SoundManager::haltMusic()
{
    if(Mix_PlayingMusic() == 1)
        Mix_HaltMusic();
}

// remove a sound from the map
void SoundManager::removeSFX(std::string sfxID)
{
    Mix_FreeChunk(this->sfxs[sfxID]);
    this->sfxs.erase(sfxID);
}

// remove a music from the map
void SoundManager::removeMusic(std::string musicID)
{
    Mix_FreeMusic(this->musics[musicID]);
    this->musics.erase(musicID);
}
