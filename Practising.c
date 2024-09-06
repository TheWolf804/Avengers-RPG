#include <raylib.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

typedef enum State {
    Game,
    Dead,
    Menu,
    Settings,
    Submenu,
    Back,
 };

 typedef enum Difficulty {
    Practise,
    Easy,
    Normal,
    Hard,
    Insane,
    Nightmare
} Difficulty;

enum Difficulty diff;

typedef struct Game {
    int health;
    float playerSpeed, enemySpeed ,enemyVelocity;
    float bombInterval, bombTimer,appleTotal;
};

#define MAX_BOMBS 0xFF
#define MAX_APPLES 0XFF

typedef struct Bomb {
    float x[MAX_BOMBS], y[MAX_BOMBS];
    int number;
    bool used[MAX_BOMBS];
} Bomb;

typedef struct Apple{
   float x[MAX_APPLES],y[MAX_APPLES]; 
   int number;
   bool used[MAX_APPLES];   
    
}Apple;

struct Game game;
Apple apples;
Bomb bombs;

enum State state = Menu;
float windowX = 800, windowY = 600;

int numberOfCircles = 0;
char *title = "Avengers RPG";
Sound click, eat, hurt, explosion;
Texture2D plank,brick,grass,apple,bomb;
Texture2D bg;
Texture2D fruit;
Music music;
// sounds: apple eat, the rest is to do
void settings(){
    Color color =WHITE;
    int fontsize=20;
    DrawRectangle(0,0,windowX,windowY,BLACK);
    DrawText("Settings",0.5*windowX-MeasureText("Settings",fontsize)/2,0.2f*windowY-fontsize/2,fontsize,color);
    Rectangle BackButton={
      0.45f*windowX,
       0.5f*windowY,
       0.1f*windowX,
       0.05f*windowY,           
    };
    
    Rectangle difficultyButton = {
        0.45f*windowX,
        0.40f*windowY,
        0.1f*windowX,
        0.05f*windowY,
    };
    
    
    char difficultyLabel[32];
    
    switch (diff) {
        case Practise:
            strcpy (difficultyLabel, "Practise");
            break;
        case Easy:
            strcpy (difficultyLabel, "Easy");
            break;
        case Normal:
            strcpy (difficultyLabel, "Normal");
            break;
        case Hard:
            strcpy (difficultyLabel, "Hard");
            break;
        case Insane:
            strcpy (difficultyLabel, "Insane");
            break;
        case Nightmare:
            strcpy (difficultyLabel, "Nightmare");
            break;
        default:
            printf ("Difficulty is invalid");
            
    }
    
        
    // Vector2 = {x, y};
    Color bg;
    Vector2 mouse = GetMousePosition();
    // if its selected
    
    if (CheckCollisionPointRec(mouse, BackButton)) {
        if (IsMouseButtonPressed (MOUSE_BUTTON_LEFT))
            state = Menu;
        bg = LIME;
    }
    else {
        bg = WHITE;
    }
    if (CheckCollisionPointRec(mouse, difficultyButton)) {
        if (IsMouseButtonPressed (MOUSE_BUTTON_LEFT)) {
            diff++;
            if (diff > Nightmare)
                diff = Practise;
            difficultySelector(diff);
        }
        bg = LIME;
    }
    else {
        bg = WHITE;
    }       
      DrawRectangleRec(BackButton,bg);
      DrawRectangleRec(difficultyButton,bg);
      DrawText("Back",BackButton.x+BackButton.width/2-MeasureText("Back",fontsize)/2,BackButton.y+fontsize/2-0.01f*windowY,fontsize,BLACK);
      
      DrawText(difficultyLabel, difficultyButton.x+difficultyButton.width/2-MeasureText(difficultyLabel,fontsize)/2,difficultyButton.y+fontsize/2-0.01f*windowY,fontsize,BLACK);
    
}

void difficultySelector (Difficulty diff) {
    if(diff==Practise){
        game.health=100;
        game.enemySpeed=50;
        game.enemyVelocity=40; 
        game.playerSpeed=100;
        game.bombInterval = 0xFFFFFF;
        game.appleTotal=10;
    }
    if (diff == Easy) {
        game.health = 10;
        game.enemySpeed = 100;
        game.enemyVelocity=100;
        game.playerSpeed=70;
        game.bombInterval = 0xFFFFFF; // 0x [0 -> 9 -> A=10 -> B -> F] // 2^20
        game.appleTotal=8;
    }
    if(diff==Normal){
        game.health=8;
        game.enemySpeed=150;
        game.enemyVelocity=150;
        game.playerSpeed=40;
        game.bombInterval = 0xFFFFFF;
        game.appleTotal=4;
    }
    if (diff == Hard) {
        game.health=5;
        game.enemySpeed =200;
        game.enemyVelocity=200;
        game.playerSpeed=30;
        game.bombInterval = 0xFFFFFF;
        game.appleTotal=2;
    }
    if(diff==Insane){ // bomb interval
        game.health=3;
        game.enemySpeed=500;
        game.enemyVelocity=400;  
        game.playerSpeed=10;
        game.bombInterval = 10; // measured in seconds
        game.appleTotal=0;
    }
    if(diff==Nightmare){ // bomb interval
        game.health=1;
        game.enemySpeed=1000;
        game.enemyVelocity=600;
        game.playerSpeed=5;
        game.bombInterval = 5; // measured in seconds
        game.appleTotal=0;
    }
    game.bombTimer = game.bombInterval;
}

void menu () {
    Color color = SKYBLUE;
    int fontsize = 50;
    SetShapesTexture(plank,(Rectangle){0,0,720,720});
    DrawRectangle(0, 0, windowX, windowY, RED);
    DrawText(title, 0.5f*windowX-MeasureText(title, fontsize)/2, 0.2f*windowY-fontsize/2, fontsize, color);
    
    SetShapesTexture(brick,(Rectangle){0,0,1500,1500});
    Rectangle playButton = {
        0.45f*windowX,
        0.5f*windowY,
        0.1f*windowX,
        0.05f*windowY,
    };
    
    // Vector2 = {x, y};
    Color bg;
    Vector2 mouse = GetMousePosition();
    // if its selected
    
    if (CheckCollisionPointRec(mouse, playButton)) {
        if (IsMouseButtonPressed (MOUSE_BUTTON_LEFT))
            state = Game;
        bg = LIME;
    }
    else {
        bg = WHITE;
    }
    DrawRectangleRec(playButton, bg);
    fontsize = 20;
    DrawText ("Play", playButton.x+playButton.width/2-MeasureText ("Play", fontsize)/2, playButton.y+fontsize/2, fontsize, BLACK);
    Rectangle settingsButton = {
        playButton.x,
        playButton.y+playButton.height+0.01f*windowY,
        playButton.width,
        playButton.height
    };
    if (CheckCollisionPointRec(mouse, settingsButton)) {
        if (IsMouseButtonPressed (MOUSE_BUTTON_LEFT))
            state = Settings;
        bg = LIME;
    }
    else {
        bg = WHITE;
    }
    DrawRectangleRec(settingsButton, bg);
    DrawText ("Settings", settingsButton.x+settingsButton.width/2-MeasureText("Settings", fontsize)/2, settingsButton.y+fontsize/2-0.01f*windowY, fontsize, BLACK);
    Rectangle quitButton = {
        settingsButton.x,
        settingsButton.y+settingsButton.height+0.01f*windowY,
        settingsButton.width,
        settingsButton.height
    };
    if (CheckCollisionPointRec(mouse, quitButton)) {
        if (IsMouseButtonPressed (MOUSE_BUTTON_LEFT))
            exit(EXIT_SUCCESS);
        bg = LIME;
    }
    else {
        bg = WHITE;
    }
    DrawRectangleRec(quitButton, bg);
    DrawText ("Quit", quitButton.x+quitButton.width/2-MeasureText("Quit", fontsize)/2, quitButton.y-fontsize/2+0.02f*windowY, fontsize, BLACK);
    
    if (state != Menu)
        PlaySound(click);
}

void bonds(float *px, float *py, float *ex, float *ey){
    if (*px <= 0)
        *px = windowX-32;
    if (*px >= windowX)
        *px = 32;
    if (*py <= 0)
        *py = windowY-32;
    if (*py >= windowY)
        *py = 32;
    if (*ex < 64)
        *ex = 64;
    if (*ex > windowX-64)
        *ex = windowX-64;
    if (*ey < 64)
        *ey = 64;
    if (*ey > windowY-64)
        *ey = windowY-64;
}
void keys (float *px, float *py, float *speed, int *size, int *circleX, int *circleY) {
        float dt = GetFrameTime();
        if (IsKeyDown(KEY_W)) {
            *py -= *speed; // py = py - speed;
        }
        if (IsKeyDown (KEY_A)) {
            *px -= *speed;
        }
        if (IsKeyDown (KEY_S)) {
            *py += *speed;
        }
        if (IsKeyDown (KEY_D)) {
            *(px) += *speed;
        }
        if (IsKeyDown(KEY_UP)) {
            *(size) += sqrt(*size);
        }
        if (IsKeyDown(KEY_DOWN)) {
            *(size) -= sqrt(*size);
             if(*size<64) *size=64;
        }
         if(IsKeyDown(KEY_Q))
         {
           state=Menu;  
         }
        if (*px-*circleX< 100 && *py-*circleY < 100) {
            if (IsKeyPressed (KEY_F)) {
                numberOfCircles++;
                PlaySound (eat);
                game.health++;
                for(int i=0;i<=apples.number;i++){
                 apples.x[i] = GetRandomValue (*size, windowX-*size);
                 apples.y[i] = GetRandomValue (*size, windowY-*size);
                 apples.number++;
            }
        }
}

int factorial (int number) {
    if (number == 0) return 1;
    else return number*factorial(number-1);
}

float sigmoid (float x) {
    return 1/(1+exp(-x));
}

int main () {
    float px = windowX/2, py = windowY/2, ex, ey;
    int circleX = 24, circleY = 36;
    float range = 100;
    float seed = 1000;
    float delay = 1;
    InitAudioDevice();
    music=LoadMusicStream("guns.ogg");
    click = LoadSound("click.ogg");
    eat = LoadSound ("eat.ogg");
    hurt = LoadSound ("hurt.ogg");
    explosion = LoadSound("bomb.wav");
    SetMusicVolume(music, 0.3);
    PlayMusicStream(music);
    printf ("%f", seed);
    SetRandomSeed(seed);
    int size = 64;
    bool disappear = false;
    SetConfigFlags (FLAG_WINDOW_RESIZABLE);
    InitWindow (windowX, windowY, "First window");
    plank=LoadTexture("plank.png");
    brick=LoadTexture("brick.png");
    grass=LoadTexture("grass.png");
    apple=LoadTexture("apple.png");
    bomb = LoadTexture ("bomb.png");
    Texture2D player=LoadTexture("smile.png");
    Texture2D enemy = LoadTexture ("enemy.png");
    SetTargetFPS(60);
    px =(float)GetRandomValue(0, windowX);
    py =(float)GetRandomValue(0, windowY);
    ex =(float)GetRandomValue(0, windowX);
    ey =(float)GetRandomValue(0, windowY);
    while (px == ex && py == ey) {
        ex =(float)GetRandomValue(0, windowX);
        ey =(float)GetRandomValue(0, windowY);      
    }
    diff = Practise;
    difficultySelector(diff);
    SetSoundVolume (eat, 1.0); // 1.0 = 100%
    SetSoundVolume (hurt, 1.0);
    if(diff>=Practise){
    for(int i=0;i<=apples.number;i++){ 
         float collision=sqrt(pow(px-apples.x[i],2)+pow(py-apples.y[i],2));
         if(collision<=32&&apples.used[i]==false){
           apples.used[i]=true;
           game.health++;
        if(diff>Hard)
        game.appleTotal=0;
        for(int i=0;i<=apples.number;i++){
           apples.x[i]=GetRandomValue(0,windowX);
           apples.y[i]=GetRandomValue(0,windowY);

        }            
           
              }  
            }
          }
    while (!WindowShouldClose()) { // while the window should NOT close
    // ! means NOT
    // 60fps
    // 1 frame -> 2nd frame
    // clear the background -> draw -> go to the second frame -> clear the background -> draw
        float dt = GetFrameTime();
        Vector2 func = {
            sigmoid (ex/windowX-1),
            sigmoid (ey/windowY-1),
        };
        if (state == Game) {
            keys(&px, &py, &game.playerSpeed, &size, &circleX, &circleY);
            bonds(&px,&py, &ex, &ey);
            
            
            
            
            
                //ex += dt*velocity*GetRandomValue (-ex, windowX-ex); // mean value: windowX/2
                //ey += dt*velocity*GetRandomValue (-ey, windowY-ey); // mean value: windowY/2
                if (ex < px)
                    ex += game.enemySpeed*dt*func.x;
                if (ex > px)
                    ex -= game.enemySpeed*dt*func.x;
                if (ey < py)
                    ey += game.enemySpeed*dt*func.y;
                if (ey > py)
                    ey -= game.enemySpeed*dt*func.y;
                //printf ("%f %f\n", ex, ey);
                //dist = sqrt(pow(px-ex, 2) + pow (py-ey, 2));
                //enemyMovement(velocity, &ex, &ey, px, py, dt);
                
            float dist = sqrt(pow(px-ex, 2) + pow (py-ey, 2));
            /*
            if (dist <= 64 && delay <= 0.1) {
                   printf ("You have taken damage\n");
                   health--;
                   delay = 1;
            } else if (dist <= 64 && delay > 1) {
                delay -= dt;
            }
            */
            if (dist <= 64) {
                if (delay <= 0.01) {
                    game.health -= 1;
                    delay = 1;
                    // hurt sound
                    PlaySound(hurt);
                } else {
                    delay -= dt;
                }
            }
            if(game.health<0) game.health=0;
            if (game.health == 0) state = Dead;
            if (diff >= Insane) {
                for (int i = 0; i <= bombs.number; i++) {
                    float collision = sqrt(pow(px-bombs.x[i], 2)+pow(py-bombs.y[i], 2));
                    if (collision <= 32 && bombs.used[i] == false) {
                        PlaySound(explosion);
                        bombs.used[i] = true;
                        game.health--;
                    }
             
            }
        }
        
        
        
        BeginDrawing(); //drawing block
            windowX = GetRenderWidth();
            windowY = GetRenderHeight();
            ClearBackground(BLACK);
            UpdateMusicStream(music);
            
            if (state == Menu) {
                menu();
            }
            if (state <= Dead) {
                SetShapesTexture(grass,(Rectangle){0,0,1500,1500}); //+
                Rectangle bg={  //+
                     0,0,
                     windowX,windowY
                };
                DrawRectangleRec(bg,WHITE); //+
                if (disappear == false) { //red circle block //+
                    SetShapesTexture(apple,(Rectangle){0,0,744,744});
                    DrawRectangle (circleX, circleY, range, range, RED);
                }
                DrawText (TextFormat("Score: %i", numberOfCircles), 0, 0, 50, WHITE);
                DrawText (TextFormat ("Difficuly:%i", diff) ,0,200,50,WHITE);
                DrawText (TextFormat ("Health:%i", game.health) ,0,100,50,WHITE);
                
                
                if (diff >= Insane) {
                    game.bombTimer -= dt;
                    char timerString[64];
                    strcpy (timerString, TextFormat("Timer: %.2f", game.bombTimer));
                    int timerFontSize = windowX/20;
                    DrawText (timerString, windowX/2-MeasureText(timerString, timerFontSize)/2, windowY/4-timerFontSize/2, timerFontSize, RED);
                    if (game.bombTimer <= 0.1) {
                        game.bombInterval--;
                        game.bombTimer = GetRandomValue(0, game.bombInterval);
                        bombs.x[bombs.number] = GetRandomValue(0, windowX);
                        bombs.y[bombs.number] = GetRandomValue(0, windowY);
                        bombs.number++;
                    }
                    SetShapesTexture (bomb, (Rectangle){0, 0, 2000, 2744});
                    for (int i = 0; i <= bombs.number; i++) {
                        if (bombs.used[i] == false)
                            DrawRectangle (bombs.x[i], bombs.y[i], range, range, WHITE);
                    }
                    
                }
                 //if(apples.number<=0.1){
                   //apples.number=GetRandomValue(0,windowX);
                   //apples.x[apples.number]=GetRandomValue(0,windowX);
                   //apples.y[apples.number]=GetRandomValue(0,windowY);  
                   
           
                   //apples.number++;
                   //if(apples.used[i]==false)
                     for(int i=0;i<=apples.number;i++)
                     if(apples.used[i]==false)                         
                     DrawRectangle(apples.x[i],apples.y[i],range,range,WHITE);                                           
                 }
                Rectangle playerbox={
                    px,
                    py,
                    size,
                    size,
                };
                SetShapesTexture(player,(Rectangle){0,0,64,64});
                DrawRectangleRec(playerbox,WHITE);
                Rectangle enemybox = {
                    ex,
                    ey,
                    size,
                    size,
                };
                SetShapesTexture(enemy,(Rectangle){0,0, 1024, 1024});
                DrawRectangleRec(enemybox,WHITE);
                if (game.health == 0) {
                    DrawText ("You Died!", 0, 0, windowX/5, RED);
                }
            }
            if(state==Settings){
                settings();
            }
        EndDrawing();
        
        
        //if ()
    }
    //CloseWindow();
    UnloadTexture(player);
}
  
  
  
  
  
 