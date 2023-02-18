#include "GentooEngine.h"
#include "EngineScreen.h"

// include the game states here
// #include "TestScene.h"
#include "GameScene.h"

int main (int argc, char* argv[]) {
    Game::GetInstance(WINDOW_TITLE, WINDOW_SIZE, GAME_RESOLUTION);
    // Game::GetInstance().AddState(new EngineScreen());
    // Game::GetInstance().Run();

    // add and run the game states here
    // Game::GetInstance().AddState(new TestScene5());
    Game::GetInstance().AddState(new Menu());//U3()use S1 and change spawn location on ZoneManager to start from the initial area

    Game::GetInstance().Run();

    Game::DeleteInstance();
    return 0;
}