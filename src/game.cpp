#include "game.h"
#include <cstdio>

void gameloop()
{
    while(true)
    {
        if (!display())
        {
            break;
        }
    }
}