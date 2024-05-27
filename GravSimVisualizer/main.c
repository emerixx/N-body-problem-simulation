#include "raylib.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <time.h>



const int maxFilenameLenght = 40;
const int maxFiles = 20;



const double width = 1800;
const double height = 900;
const int drawScale = 100;

const int offset[2] = {0,0};


int speed;

int chunkSize;

bool drawTrajectories = false;


int bodies;

int waitBetweenChunks = 10;



void openFiles(FILE *file[]) {
  char filename[maxFilenameLenght];

  for (int i = 0; i < maxFiles; i++) {
    snprintf(filename, maxFilenameLenght, "../outputTxtFiles/%d.txt", i);
        
    file[i] = fopen(filename, "r");
        
    if (file[i] == NULL) {
      printf("No more files to open. Stopped at %d.txt\n", i);
      bodies = i;
      break;
        }

        printf("Opened file: %s\n", filename);

        
    }
}

void loadNextChunk(FILE *file[], double position[chunkSize][bodies][2]){
  


  for (int i = 0; i < bodies; i++) {
    double x,y;
    for (int j =0; j < chunkSize; j++) {
      char line[256] = "";
      fgets(line, sizeof(line), file[i]);
      sscanf(line, "%lf,%lf", &x, &y);
      position[j][i][0] = x;
      position[j][i][1] = y;
      
    }

  }

  printf("Next chunk loaded\n");
}

void askIfWantTrajectories(){

  char inputBool;
  printf("Do you want to draw the trajectories? (Y/n)\n");
  inputBool = fgetc(stdin);
  inputBool = tolower(inputBool);
  if (inputBool == 'n') {
    drawTrajectories = false;
    } else if (inputBool == 'y'){
    
    drawTrajectories = true;
    
  
    printf("user-inputed value used (%d)\n", drawTrajectories);
  }else {

    printf("default value used (%d)\n", drawTrajectories);

    drawTrajectories = false;
  }
}


void askForTimeDelay(){
 
  char inputInt[10];
  printf("What time do you want to wait between each position shown? (in ns) (default = 10)\n");
  fgets(inputInt, sizeof(inputInt), stdin);
  if (inputInt[0] == '\n') {
    waitBetweenChunks = 10;
    printf("default value used (%d)\n", waitBetweenChunks);
  } else {
    waitBetweenChunks = atoi(inputInt);
    printf("user-inputed value used (%d)\n", waitBetweenChunks);
  }

  
   
}

void emptyInputBuffer(){

  char randomCharDontUse;
  while(randomCharDontUse != '\n'){
      randomCharDontUse = getchar();
  }
}




int main(void)
{ 

 
  
 
  

  FILE *file[maxFiles];
   
  openFiles(file);
  
 
  printf("Bodies: %d\n", bodies);
  
  speed = pow(10, 0);

  chunkSize = pow(10, 2);
  
  double position[chunkSize][bodies][2];
  
  Color clrs[] = {RAYWHITE, MAGENTA, RED, LIME, ORANGE};
  
  loadNextChunk(file, position);

 
  
  askIfWantTrajectories();
  
  emptyInputBuffer();
  
  if (drawTrajectories){

    askForTimeDelay();
  
  }
 
  



  InitWindow(width, height, "GP Graphics Imager");
  
  
  
 
  BeginDrawing();
  ClearBackground(BLACK);
  EndDrawing();

  if(drawTrajectories == false){
    while (1){

      for (int j=0; j < chunkSize -speed; j+= speed){
     
        BeginDrawing();

        ClearBackground(BLACK);
      
        for (int i=0; i < bodies; i++){

          DrawCircle((int)(width/2+position[j][i][0]*drawScale) + offset[0],(int)(height/2-position[j][i][1]*drawScale) + offset[1], 2 ,clrs[i% (sizeof(clrs) / sizeof(clrs[0]))]);   
        
        }

        EndDrawing();
      }

      loadNextChunk(file, position);
   
    }
  }
  else if (drawTrajectories == true){
    Image imageBuffer = GenImageColor(GetScreenWidth(), GetScreenHeight(), BLACK);
    Texture displayTexture = LoadTextureFromImage(imageBuffer);
    int x = 0;
    int y = 0;
    printf("heree");
    struct timespec ts;
      
    ts.tv_sec =  0;
    ts.tv_nsec = waitBetweenChunks;

    while(1){
     
      
      for (int j=0; j < chunkSize; j++){
     
   

      
      
        for (int i=0; i < bodies; i++){

          ImageDrawPixel(&imageBuffer, (int)(width/2+position[j][i][0]*drawScale) + offset[0],(int)(height/2-position[j][i][1]*drawScale) + offset[1] ,clrs[i]);   
        
        }
        nanosleep(&ts, NULL);

      
      }

      loadNextChunk(file, position);
   
	  	

		  UpdateTexture(displayTexture, imageBuffer.data);
		
		  BeginDrawing();
		  ClearBackground(BLACK);

		  DrawTexture(displayTexture, 0, 0, WHITE);

		  EndDrawing();

      
     
     
    }
  }
  sleep(10);


 
  CloseWindow();        // Close window and OpenGL context

  return 0;
}
