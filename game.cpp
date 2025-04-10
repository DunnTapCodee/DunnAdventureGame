#include "GameLoop.h"


// Main function
int main(int argc, char* argv[]) {
    Menu* menu = nullptr;
    
    InitGame(menu);

    LoadMainCharacters(menu);

    Monster* bat = nullptr;
    SetupMonster(bat);

    if (!menu->inMenu) {
        RunGameLoop(menu, bat);
    }

    cleanUp(dunn, ronaldo, ball, bat);
    CleanupScoreTextures();
    graphics.quit();
    return 0;
}
