#include "SDL/SDL.h"
#include <fstream>


class brick
{
    public:
    int x;
    int y;

    void show(SDL_Surface*, SDL_Surface*);
};

class goal
{
    public:
    int x;
    int y;

    void show(SDL_Surface*, SDL_Surface*);
};

class player
{
    public:
    int x;
    int y;
    int xvel;
    int yvel;
    
    player();
    void bounce();   
    void update(Uint8*);
    void show(SDL_Surface*, SDL_Surface*);
};

class zombie
{
    public:
    int x;
    int y;
    int xvel;
    int yvel;
    Uint8 keys[4];
    bool bounced;

    zombie();
    void bounce();
    Uint8* think(SDL_Rect);
    void update(Uint8*);
    void show(SDL_Surface*, SDL_Surface*);
};


class map
{
   public:
   goal house;
   player you;
   zombie them[320];
   brick obst[320];
   int zombiecount;
   int brickcount;
   map();
   Uint8 update(Uint8*);
   void read(std::ifstream&);
   void show(SDL_Surface*, SDL_Surface*, SDL_Surface*, SDL_Surface*, SDL_Surface*, SDL_Surface*);
};

