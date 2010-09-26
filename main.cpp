#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include <fstream>
#include <iostream>
#include "zombimaze.h"


int main()
{
    SDL_Surface* g = NULL;
    SDL_Surface* b = NULL;
    SDL_Surface* z = NULL;
    SDL_Surface* p = NULL;
    SDL_Surface* h = NULL;
    SDL_Surface* screen = NULL;
    SDL_Event event;
    int mapmax=4;
    map m[ mapmax ];
    int mapnum=0;
    std::ifstream fin;
    bool quit;

    SDL_Init( SDL_INIT_EVERYTHING );
    screen = SDL_SetVideoMode( 640, 480, 32, SDL_SWSURFACE );
    g = IMG_Load( "ground.png" );
    b = IMG_Load( "brick.png" );
    z = IMG_Load( "zombie.png");
    p = IMG_Load( "guy.png" );
    h = IMG_Load( "goal.png" );
    z = SDL_DisplayFormat(z);
    p = SDL_DisplayFormat(p);
    h = SDL_DisplayFormat(h);

    Uint32 colorkey = 0x00ffff;
    
    SDL_SetColorKey( z, SDL_SRCCOLORKEY, colorkey);
    SDL_SetColorKey( p, SDL_SRCCOLORKEY, colorkey);
    SDL_SetColorKey( h, SDL_SRCCOLORKEY, colorkey);
    fin.open( "map1.map" );
    for( int i=0; i<mapmax; i++ )
    {
        m[i].read( fin );
    }
    bool fs = true;
    SDL_WM_ToggleFullScreen(screen);
    SDL_ShowCursor(SDL_DISABLE);
    while( !quit )
    {
        SDL_PollEvent( &event );
        if( event.type == SDL_QUIT )
        {
            quit = true;
        }
        Uint8 *keystates = SDL_GetKeyState( NULL );
        if( keystates[ SDLK_f ] ) { fs = !fs; SDL_WM_ToggleFullScreen(screen); }
        if( keystates[ SDLK_ESCAPE ] ) { quit = true; }
        Uint8 outcome = m[mapnum].update(keystates);
        if( outcome == 1 ) 
        {  
            if( mapnum < mapmax - 1 )
            {
                mapnum++;
            }
            else
            {
                quit=true;
            }
        }
        else if( outcome == 2 ) { quit=true; }
          
        m[mapnum].show(g,b,z,p,h,screen);
        SDL_Flip(screen);
    }
    if( fs ) { SDL_WM_ToggleFullScreen(screen); }
    return 0;
}

