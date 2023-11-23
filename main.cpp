#include<iostream>
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<SDL2/SDL_mixer.h>
#include<math.h>
using namespace std;

int InnerSeconds;


//when timer is over do some actions
void Timer(int nowT, int wait, void (*funcName))
{
    if(InnerSeconds-nowT >= wait)
    {
        funcName();
    }
}

int PlayerAllWAndh = 22;

struct Vector2
{
    public:
    float x;
    float y;
};

Vector2 cPlPos;
Vector2 PLVel;
int PLSpeed = 400;
int health = 100;

bool IsAlreadyDamaged;
float DamageInterval;
float Interval;

SDL_Rect Player;
int frameHeight, frameWidth;
int textureWidth, textutrHeight;

void ShowFreeBox(SDL_Renderer* mRender, Vector2 Cpos, Vector2 Size)
{
    SDL_SetRenderDrawColor(
        mRender,
        255,
        100,
        100,
        255
    );

    SDL_Rect Srect;
    Srect.x = Cpos.x -(Size.x/2);
    Srect.y = Cpos.y -(Size.y/2);
    Srect.h = Size.y;
    Srect.w = Size.x;

    SDL_RenderFillRect(mRender, &Srect);
}

void TakeDamage()
{
    if(IsAlreadyDamaged == false)
    {
        Player.x += frameWidth;
        health -= 25;
        IsAlreadyDamaged = true;
    }
}

void FreeCollideBox(Vector2 Cpos, Vector2 Size)
{
    SDL_Rect Srect;
    Srect.x = Cpos.x -(Size.x/2);
    Srect.y = Cpos.y -(Size.y/2);
    Srect.h = Size.y;
    Srect.w = Size.x;

    if(cPlPos.y+PlayerAllWAndh/2 >= Srect.y && cPlPos.y-PlayerAllWAndh/2 <= Cpos.y+(Size.y/2))
    {
        if(cPlPos.x+PlayerAllWAndh/2 >= Srect.x && cPlPos.x-PlayerAllWAndh/2 <= Cpos.x+(Size.x/2))
        {
            TakeDamage();
            //PLVel.x = -PLVel.x;
        }
    }

    if(cPlPos.x+PlayerAllWAndh/2 >= Srect.x && cPlPos.x-PlayerAllWAndh/2 <= Cpos.x+(Size.x/2))
    {
        if(cPlPos.y+PlayerAllWAndh/2 >= Srect.y && cPlPos.y-PlayerAllWAndh/2 <= Cpos.y+(Size.y/2))
        {
            TakeDamage();
            //PLVel.y = -PLVel.y;
        }
    }
}

void ScrapCollideBox(Vector2 pos, bool IsInX)
{
    Vector2 dir;
    SDL_Rect Srect;

    if(IsInX == true)
    {
        Srect.w = 100;
        Srect.h = 780;
    }
    else if(IsInX == false)
    {
        Srect.w = 1024;
        Srect.h = 100;
    }

    Srect.x = pos.x - (Srect.w/2);
    Srect.y = pos.y - (Srect.h/2);

    if(Srect.x-512 < 0)
        dir.x = 1;
    else if(Srect.x-512 > 0)
        dir.x = -1;

    if(Srect.y-390 < 0)
        dir.y = 1;
    else if(Srect.y-390 > 0)
        dir.y = -1;

    if(IsInX == true)
    {
        if(cPlPos.x+PlayerAllWAndh/2 >= Srect.x && cPlPos.x-PlayerAllWAndh/2 <= Srect.w+ Srect.x)
        {
            TakeDamage();
            //cPlPos.x = (Srect.w/2+Srect.x)+64*dir.x;
        }
    }
    else if(IsInX == false)
    {
        if(cPlPos.y+PlayerAllWAndh/2 >= Srect.y && cPlPos.y-PlayerAllWAndh/2 <= Srect.h+ Srect.y)
        {
            TakeDamage();
            //cPlPos.y = (Srect.h/2+Srect.y)+64*dir.y;
        }
    }
}

void Ticker(float Inter)
{
    Interval++;
    if(60.0f/Interval <= Inter)
    {
        InnerSeconds++;
        Interval = 0;
    }
}

void WorldCollideCapsule()
{

}

void WorldGraphicCapsule(SDL_Renderer* mRender)
{

}

int main(int argc, char** argv)
{
    int a = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    IMG_Init(IMG_INIT_PNG);
    SDL_Window *window;
    window = SDL_CreateWindow(
            "Just shapes and beats: my version",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            1024,
            780,
            SDL_WINDOW_OPENGL
    );

    SDL_Renderer* mRender;
    mRender = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );

    if(a != 0)
    {
        SDL_Log("dfkjsd", SDL_GetError());
        return 0;
    }
    if(window == NULL)
    {
        SDL_Log("dfkjsdfdfdfd", SDL_GetError());
        return 0;
    }
    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048)<0)
    {
        SDL_Log("dfkjsdfdfdfd", SDL_GetError());
        return 0;
    }

    Mix_Music *bgm = Mix_LoadMUS("It Lives in the Woods - Fight or Flight.mp3");
    const Uint8 *keyST = SDL_GetKeyboardState(NULL);
    Uint32 mTickCount;

    bool isRunning = true;
    SDL_Event event;
    Mix_PlayMusic(bgm, -1);

    Vector2 leftWall;
    leftWall.x = 50;
    leftWall.y = 390;

    Vector2 RightWall;
    RightWall.x = 974;
    RightWall.y = 390;

    Vector2 TopWall;
    TopWall.x = 510;
    TopWall.y = 50;

    Vector2 BottomWall;
    BottomWall.x = 510;
    BottomWall.y = 730;

    //PLAYER
    cPlPos.x = 500;
    cPlPos.y = 350;

    SDL_Rect PLrect;
    PLrect.x = cPlPos.x - (PlayerAllWAndh/2);
    PLrect.y = cPlPos.y - (PlayerAllWAndh/2);
    PLrect.w = PlayerAllWAndh;
    PLrect.h = PlayerAllWAndh;

    SDL_Surface* surf = IMG_Load("cubeChrac.PNG");
    SDL_Texture* texture = SDL_CreateTextureFromSurface(mRender, surf);
    float angle = 0;
    SDL_Point center = {PlayerAllWAndh/2, PlayerAllWAndh/2};
    SDL_RendererFlip flip = SDL_FLIP_NONE;

    PLVel.y = 0;
    PLVel.x = 0;

    textureWidth = 1500;
    textutrHeight = 250;
    SDL_QueryTexture(texture, NULL, NULL, &textureWidth, &textutrHeight);
    frameWidth = textureWidth/6;
    frameHeight = textutrHeight;

    Player.x = 0;
    Player.y = 0;
    Player.w = frameWidth;
    Player.h = frameHeight;

    //Others
    while(isRunning)
    {
        //cout << health << endl;
        cout << cPlPos.x << " " << cPlPos.y << endl;
        if(IsAlreadyDamaged)
        {
            DamageInterval++;
            if(60.0f/DamageInterval <= 0.5f)
            {
                DamageInterval = 0;
                IsAlreadyDamaged = false;
            }
        }

        mTickCount = SDL_GetTicks();
        while(!SDL_TICKS_PASSED(SDL_GetTicks(), mTickCount+16))
            ;

        float deltaTime = (SDL_GetTicks()-mTickCount)/1000.0f;
        if(deltaTime > 0.05f)
        {
            deltaTime = 0.05f;
        }

        Ticker(2);

        while(SDL_PollEvent(&event))
        {
            switch(event.type)
            {
            case SDL_QUIT:
                isRunning = false;
                break;
            }
        }
        if(keyST[SDL_SCANCODE_W])
        {
            PLVel.y = -PLSpeed*deltaTime;
        }
        if(keyST[SDL_SCANCODE_A])
        {
            PLVel.x = -PLSpeed*deltaTime;
        }
        if(keyST[SDL_SCANCODE_S])
        {
            PLVel.y = PLSpeed*deltaTime;
        }
        if(keyST[SDL_SCANCODE_D])
        {
            PLVel.x = PLSpeed*deltaTime;
        }

        if(cPlPos.y-PlayerAllWAndh/2 < 0)
        {
            PLVel.y = 0;
            cPlPos.y = 14;
        }
        else if(cPlPos.y+PlayerAllWAndh/2 > 780)
        {
            PLVel.y = 0;
            cPlPos.y = 766;
        }

        if(cPlPos.x-PlayerAllWAndh/2 < 0)
        {
            PLVel.x = 0;
            cPlPos.x = 14;
        }
        else if(cPlPos.x+PlayerAllWAndh/2 > 1024)
        {
            PLVel.x = 0;
            cPlPos.x = 1010;
        }

        Vector2 cpos;
        cpos.x = 300;
        cpos.y = 500;
        Vector2 sizee;
        sizee.x = 50;
        sizee.y = 50;
        FreeCollideBox(cpos, sizee);

        WorldCollideCapsule();

        cPlPos.x += PLVel.x;
        cPlPos.y += PLVel.y;
        PLrect.x = cPlPos.x - (PlayerAllWAndh/2);
        PLrect.y = cPlPos.y - (PlayerAllWAndh/2);
        PLrect.w = PlayerAllWAndh;
        PLrect.h = PlayerAllWAndh;

        angle = atan2(PLVel.y, PLVel.x)*180/3.14f;

        PLVel.x = 0;
        PLVel.y = 0;

        SDL_SetRenderDrawColor(
            mRender,
            0,
            0,
            0,
            255
        );
        SDL_RenderClear(mRender);

        SDL_RenderCopyEx(mRender, texture, &Player , &PLrect, angle, &center, flip);

        WorldGraphicCapsule(mRender);

        ShowFreeBox(mRender, cpos, sizee);
        ScrapCollideBox(leftWall, true);
        ScrapCollideBox(RightWall, true);
        ScrapCollideBox(TopWall, false);
        ScrapCollideBox(BottomWall, false);

        SDL_RenderPresent(mRender);
    }

    SDL_DestroyWindow(window);
    Mix_FreeMusic(bgm);
    bgm = NULL;
    SDL_Quit();
    IMG_Quit();
    Mix_Quit();
    return 0;
}
