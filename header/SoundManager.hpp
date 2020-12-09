#ifndef __SOUNDMANAGER__
#define __SOUNDMANAGER__

#include <iostream>
#include <map>
#include <SDL2/SDL_mixer.h>
#include <string>

// is it music or sound
enum SoundType { SOUND_MUSIC, SOUND_SFX };

class SoundManager
{
    public:
        // singleton instance
        static SoundManager* Instance()
        {
            if(soundManager == nullptr)
                soundManager = new SoundManager();

            return soundManager;
        }

        bool init(int flags, unsigned freq, Uint16 format, unsigned channels, unsigned chunksize);

        // load a sound file and add it
        bool load(std::string fileName, std::string soundID, SoundType soundType);

        // play a sound
        // string key and how many times it's repeated
        void playSound(std::string soundID, int loop);
        // same for music
        void playMusic(std::string musicID, int loop);
        // pause
        void pauseMusic();
        // resume
        void resumeMusic();
        // stop
        void haltMusic();

        // is music playing or paused
        bool isMusicPlaying() { return (Mix_PlayingMusic() ? true : false); }
        bool isMusicPaused() { return (Mix_PausedMusic() ? true : false); }

        // removes a sound from the map
        void removeSFX(std::string sfxID);
        // removes a music from the map
        void removeMusic(std::string musicID);

        // frees the memory used by sound and music
        void freeMemory() { delete soundManager; soundManager = nullptr; }

    protected:
    private:
        // ctor and dtor for singleton
        SoundManager();
        ~SoundManager();

        // the sound manager singleton
        static SoundManager* soundManager;

        // maps for sfx and music
        // string keys, Mix_Chunk* for sfx and Mix_Music* for music
        std::map<std::string, Mix_Chunk*> sfxs;
        std::map<std::string, Mix_Music*> musics;
};

#endif // __SOUNDMANAGER__
