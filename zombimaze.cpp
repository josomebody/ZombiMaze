#include "SDL/SDL.h" 
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include "zombimaze.h"

bool collide(SDL_Rect a, SDL_Rect b)
{
    int lefta, leftb;
    int righta, rightb;
    int topa, topb;
    int bota, botb;

    lefta = a.x;
    leftb = b.x;
    righta = a.x + a.w;
    rightb = b.x + b.w;
    topa = a.y;
    topb = b.y;
    bota = a.y + a.h;
    botb = b.y + b.h;

    if( lefta >= rightb ) { return false; }
    if( righta <= leftb ) { return false; }
    if( topa >= botb ) { return false; }
    if( bota <= topb ) { return false; }
    return true;
}

void goal::show(SDL_Surface* src, SDL_Surface* dest)
{
    SDL_Rect rect;
    rect.x=x;
    rect.y=y;
    SDL_BlitSurface(src, NULL, dest, &rect);
}

void brick::show(SDL_Surface* src, SDL_Surface* dest)
{
    SDL_Rect rect;
    rect.x=x;
    rect.y=y;
    SDL_BlitSurface(src, NULL, dest, &rect);
}

player::player()
{
    xvel=0;
    yvel=0;
}

void player::update(Uint8* keystates)
{
    if( keystates[ SDLK_UP ] )
    {
        if( yvel > -2 ) { yvel--; }
    }
    else if( keystates[ SDLK_DOWN ] )
    {
        if( yvel < 2 ) { yvel++; }
    }
    else
    {
        if( yvel < 0 ) { yvel++; }
        if( yvel > 0 ) { yvel--; }
    }

    if( keystates[ SDLK_LEFT ] )
    {
        if( xvel > -2 ) { xvel--; }
    }
    else if( keystates[ SDLK_RIGHT ] )
    {
        if( xvel < 2 ) { xvel++; }
    }
    else
    {
        if( xvel < 0 ) { xvel++; }
        if( xvel > 0 ) { xvel--; }
    }
    x+=xvel;
    y+=yvel;  
    if( ( x < 0 ) || ( x > 608 ) ) { x-=xvel; }
    if( ( y < 0 ) || ( y > 448 ) ) { y-=yvel; }
}

void player::bounce()
{
    x-=xvel;y-=yvel;
    if( ( x < 0 ) || ( x > 608 ) ) { x+=xvel; }
    if( ( y < 0 ) || ( y > 440 ) ) { y+=yvel; }
}

void player::show(SDL_Surface* src, SDL_Surface* dest)
{
    SDL_Rect rect;
    rect.x=x;
    rect.y=y;
    SDL_BlitSurface(src, NULL, dest, &rect);
}

zombie:: zombie()
{
    xvel=0;
    yvel=0;
    for( int i=0;i<4;i++)
    {
        keys[i]=0;
    }
    bounced=false;
}

Uint8* zombie::think(SDL_Rect target)
{
    int distx, disty;
    distx = x - target.x;
    disty = y - target.y;
    
    if( bounced )
    {
        if( keys[ 0 ] )
        {
            keys[ 0 ] = false;
            keys[ 2 ] = true;
        }
        else if( keys[ 1 ] )
        {
            keys[ 1 ] = false;
            keys[ 3 ] = true;
        }
        else if( keys[ 2 ] )
        {
            keys[ 2 ] = false;
            keys[ 0 ] = true;
        }
        else if( keys[ 3 ] )
        {
            keys[ 3 ] = false;
            keys[ 1 ] = true;
        } 
        else
        {
            keys[ 1 ] = true;
        }
        bounced=false;
        Uint8* k=keys;
        return k;
    } 
    for( int i=0; i<4; i++ )
    {
        keys[ i ] = false;
    }

//    if( abs(distx) < abs(disty) )
//    {
        if( distx < 0 )
        {
            keys[ 3 ] = true;
        }
        else
        {
            keys[ 2 ] = true;
        }
//    }
//    else
//    {
        if( disty < 0 )
        {
            keys[ 1 ] = true;
        }
        else
        {
            keys[ 0 ] = true;
        }
//    }
    if( !( keys[ 0 ] || keys[ 1 ] || keys[ 2 ] || keys[ 3 ] ) ) { keys[ 1 ]=true;}
    Uint8* k=keys;
    return k;
}

void zombie::bounce()
{
    bounced=true;
    x-=xvel;y-=yvel;
    if( ( x < 0 ) || ( x > 608 ) ) { x+=xvel; }
    if( ( y < 0 ) || ( y > 448 ) ) { y+=yvel; }
}

void zombie::update(Uint8* keystates)
{
    if( keystates[ 0 ] )
    {
        yvel=-1;
    }
    else if( keystates[ 1 ] )
    {
        yvel=1;
    }
    else
    {
        yvel=0;
    }

    if( keystates[ 2 ] )
    {
        xvel=-1;
    }
    else if(keystates[ 3 ] )
    {
        xvel=1;
    }
    else
    {
        xvel=0;
    }
    x+=xvel;y+=yvel;
    if( ( x < 0 ) || ( x > 608 ) )
    {
        x-=xvel;
    }
    if( ( y < 0 ) || ( y > 448 ) )
    {
        y-=yvel;
    }
}

void zombie::show(SDL_Surface* src, SDL_Surface* dest)
{
    SDL_Rect rect;
    rect.x=x;
    rect.y=y;
    SDL_BlitSurface(src, NULL, dest, &rect);
}

map::map()
{
    zombiecount=0;
    brickcount=0;
}

Uint8 map::update(Uint8* keystates)
{
    SDL_Rect p, q;
    p.x=you.x;
    p.y=you.y;
    p.h=16;
    p.w=32;
    q.x=house.x;
    q.y=house.y;
    q.h=32;
    q.w=32;
    if( collide( p, q ) ) { return 1; }
    you.update(keystates);
    for( int i=0; i<brickcount; i++ )
    {
        SDL_Rect a, b;
        a.x=you.x;
        a.y=you.y+16;
        a.h=16;
        a.w=32;
        b.x=obst[i].x;
        b.y=obst[i].y;
        b.h=32;
        b.w=32;
        if( collide( a, b ) ) { you.bounce(); }
    }
    for( int i=0; i<zombiecount; i++ )
    {
        SDL_Rect target;
        target.x=you.x;
        target.y=you.y;
        target.h=16;
        target.w=32;
        SDL_Rect a;
        a.x = them[i].x;
        a.y = them[i].y;
        a.h=16;
        a.w=32;
        if( collide( target, a ) ) { return 2; }
        Uint8* zkeys=them[i].think(target);
        them[i].update(zkeys);
        SDL_Rect b, c;
        b.x=them[i].x;
        b.y=them[i].y+32;
        b.h=16;
        b.w=32;
        for( int j=0; j<zombiecount; j++ )
        {
            if( j!=i )
            {
                c.x=them[j].x;
                c.y=them[j].y;
                c.h=16;
                c.h=32;
                if( collide( b, c ) ) { them[i].bounce(); }
            }
        }

        for( int j=0; j<brickcount; j++ )
        {
            c.x=obst[j].x;
            c.y=obst[j].y;
            c.h=32;
            c.w=32;
            if( collide( b, c ) ) { them[i].bounce(); }
        }
    }
    return 0;
}

void map::read(std::ifstream& mappy)
{
    char m;
    for(int y=0;y<16;y++)
    {
        for(int x=0;x<20;x++)
        {
            mappy>>m;
            while( m=='\n' )
            {
                mappy>>m;
            }
            switch (m)
            {
                case '0':
                break;

                case '1':
                obst[brickcount].x=x*32;
                obst[brickcount].y=y*32;
                brickcount++;
                break;

                case '2':
                them[zombiecount].x=x*32;
                them[zombiecount].y=y*32;
                zombiecount++;
                break;

                case '3':
                you.x=x*32;
                you.y=y*32;
                break;
      
                case '4':
                house.x=x*32;
                house.y=y*32;
                break;
            }
        }
    }
}

void map::show(SDL_Surface* g, SDL_Surface* b, SDL_Surface* z, SDL_Surface* p, SDL_Surface* h, SDL_Surface* screen)
{
    for(int y=0;y<16;y++)
    {
        for(int x=0;x<20;x++)
        {
            SDL_Rect rect;
            rect.x=x*32;
            rect.y=y*32;
            SDL_BlitSurface(g,NULL,screen,&rect);
        }
    }

    house.show(h, screen);

    for(int i=0;i<brickcount;i++)
    {
        obst[i].show(b, screen);
    }
    for(int i=0;i<zombiecount;i++)
    {
        them[i].show(z, screen);
    }
    you.show(p, screen);
}


