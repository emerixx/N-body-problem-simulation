#include "raylib.h"
#include "rlgl.h"
#include <math.h>
#include <stdio.h>

int const circleResolution = 10;
Image img;
Texture2D texture;
Camera camera = { 0 };


float width = 100;
float height = 100;

float depth = 0; 

void drawCircle(int radius, Vector2 center, Color color);

int main(void)
{
  // set up the window
  InitWindow(1280, 800, "Hello Raylib");

  img = GenImageColor(32, 32, WHITE);
  texture = LoadTextureFromImage(img);
  UnloadImage(img);

  
  camera.position = (Vector3){ 0, 0, 100 };
  camera.target = (Vector3){ 0, 0, 0 };
  camera.up = (Vector3){ 0, 1, 0 };
  camera.fovy = 45;
  camera.projection = CAMERA_PERSPECTIVE;
// you must say WHERE in 3d space the triangle is when you are in 3d mode
  //drawCircle(0, 0);

// game loop
  while (!WindowShouldClose())
  {
 
    // drawing
    BeginDrawing();
    ClearBackground(BLACK);

    BeginMode3D(camera);

    // draw an XY grid just so we know we are sane
    rlPushMatrix();
    rlRotatef(90, 1, 0, 0);
    DrawGrid(100, 10);
    rlPopMatrix();

    // draw a quad
    drawCircle(50, (Vector2){0,0}, WHITE);   
    

    EndMode3D();

    EndDrawing();
  }
  // cleanup
  UnloadTexture(texture);
  CloseWindow();
  return 0;
}

void drawCircle(int radius, Vector2 center, Color color){
  double angle;
  rlSetTexture(texture.id);

  rlBegin(RL_TRIANGLES);
  for (int i = 0; i < circleResolution; i++)
  {
    
  
    rlColor4ub(255, color.g, color.b, 255);
  
    rlVertex2f(center.x, center.y);
    rlVertex2f(center.x + cosf(DEG2RAD*(angle + 1.0f/circleResolution))*radius, center.y + sinf(DEG2RAD*(angle + 1.0f/circleResolution))*radius);
    
    rlVertex2f(center.x + cosf(DEG2RAD*angle)*radius, center.y + sinf(DEG2RAD*angle)*radius);

    angle += 1.0f/circleResolution;
  }
  rlEnd();
}
