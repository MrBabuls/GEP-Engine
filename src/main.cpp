#include "SDL3/SDL.h"
#include "SDL3/SDL_main.h"
#include "Engine.h"

int main(int argc, char** argv) 
{
    Engine engine;

    if (!engine.initialize("Engine", 720, 480)) 
    {
        return 1;
    }

    engine.run();
    engine.clean();

    return 0;
}
