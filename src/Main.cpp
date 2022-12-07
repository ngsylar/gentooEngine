#include "GentooEngine.h"
#include "EngineScreen.h"

// include the game states here
#include "TestScene.h"

#define WINDOW_TITLE    "Game Title"
#define WINDOW_SIZE     1072, 603

int main (int argc, char* argv[]) {
    Game::GetInstance(WINDOW_TITLE, WINDOW_SIZE);
    // Game::GetInstance().AddState(new EngineScreen());
    // Game::GetInstance().Run();

    // add and run the game states here
    Game::GetInstance().AddState(new TestScene5());
    Game::GetInstance().Run();

    Game::DeleteInstance();
    return 0;
}