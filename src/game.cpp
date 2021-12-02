#include "game.h"
#include "graphics.h"
#include "wad.h"
#include <cstdio>

void gameloop()
{
    while(!(glfwWindowShouldClose(window))) // Game loop
    {
        process_input();
        display();
    }
}