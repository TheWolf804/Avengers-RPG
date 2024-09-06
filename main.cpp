#include <raylib.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

typedef enum State {
    Menu,
    Game,
    Settings,
    Submenu,
    Back
};

enum State state = Menu;
float windowX = 800, windowY = 600;
int numberOfCircles = 0;
char *title = "Avengers RPG";

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
      DrawRectangleRec(BackButton,bg);
      DrawText("Back",BackButton.x+BackButton.width/2-MeasureText("Back",fontsize)/2,BackButton.y+fontsize/2-0.01f*windowY,fontsize,BLACK);
    
}
void menu () {
    Color color = SKYBLUE;
    int fontsize = 50;
    DrawRectangle(0, 0, windowX, windowY, RED);
    DrawText(title, 0.5f*windowX-MeasureText(title, fontsize)/2, 0.2f*windowY-fontsize/2, fontsize, color);
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
}

void bonds(int *px,int *py){
    if (*px <= 0)
            *px = windowX-32;
        if (*px >= windowX)
            *px = 32;
        if (*py <= 0)
            *py = windowY-32;
        if (*py >= windowY)
            *py = 32;      
}
void keys (int *px, int *py, float *speed, int *size, int *circleX, int *circleY) {
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
            if (*size == 0) *size = 1;
        }
        if (*px-*circleX < 20 && *py-*circleY < 20) {
            if (IsKeyPressed (KEY_F)) {
                numberOfCircles++;
                *circleX = GetRandomValue (*size, windowX-*size);
                *circleY = GetRandomValue (*size, windowY-*size);
            }
        }
}

int factorial (int number) {
    if (number == 0) return 1;
    else return number*factorial(number-1);
}

int main () {
    int px = windowX/2, py = windowY/2;
    int circleX = 24, circleY = 36;
    float range = 50;
    float speed = 20.5;
    float seed = clock();
    printf ("%f", seed);
    SetRandomSeed(seed);
    int size = 32;
    bool disappear = false;
    SetConfigFlags (FLAG_WINDOW_RESIZABLE);
    InitWindow (windowX, windowY, "First window");
    SetTargetFPS(60);
    px = GetRandomValue(0, windowX);
    py = GetRandomValue(0, windowY);
    while (!WindowShouldClose()) { // while the window should NOT close
    // ! means NOT
    // 60fps
    // 1 frame -> 2nd frame
    // clear the background -> draw -> go to the second frame -> clear the background -> draw
        if (state == Game) {
            keys(&px, &py, &speed, &size, &circleX, &circleY);
            bonds(&px,&py);
        }
        
        
        BeginDrawing(); //drawing block
            windowX = GetRenderWidth();
            windowY = GetRenderHeight();
            ClearBackground(BLACK);
            if (state == Game) {
                if (disappear == false)
                    DrawCircle (circleX, circleY, range, RED);
                DrawText (TextFormat("Score: %i", numberOfCircles), 0, 0, 100, WHITE);
                DrawRectangle(px, py, size, size, BLUE); //draw the player
            }
            if (state == Menu) {
                menu();
            }
            if(state==Settings){
            settings();
            }
        EndDrawing();
        
        
        //if ()
    }
    //CloseWindow();
    printf ("Apple eaten");
}

