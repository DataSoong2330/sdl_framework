#ifndef __MENUSTATE__
#define __MENUSTATE__

#include "../header/GameState.hpp"

class MenuState : public GameState
{
    public:
        // update input and so on
        void update();
        // render all necessary stuff of a certain state
        void render();

        // loads all images, sounds and stuff
        bool onEnter(std::string fileName);
        // frees the memory allocated on entering a state
        bool onExit();

        // gets the state ID like Menu, Play, GameOver and so on
        inline const std::string getStateID() const { return menuID; }

    protected:
    private:
        // state ID
        static const std::string menuID;

        // the transition methods of the state
        static void menuToPlay();
        static void exitFromMenu();
};

#endif // __MENUSTATE__
