#include <stdio.h>
#include <math.h>
#include <sys/stat.h>
#include <time.h>

//program setup
char outputFolderName[] = "../outputTxtFiles";

double positions; // number of lines in an object's file. The lenght of the simulation
double timeDelta; // time step every loop, accuracy of the simulation. lower number -> higher accuracy
    
int logsPerComputation; // how many lines are written to the console during the computation (info about progress)
int compress; // write only every {compress} positions to reduce file size. when decreaseing timeDelta, its recommended to increase this
    
//constants constants
double gravityConstant = 39.4947001202; // AU^3 MO^-1 Year^-2

int NofBodies;

clock_t start, curTime, end;

  
  


  


 


typedef struct body{
  double mass;
  double position[2];
  double velocity[2];
  double velocityOld[2];
  double acceleration[2];
  double accelerationOld[2];
  FILE *bodyFile;
} body ;



void createOutputFolder(){
  
  mkdir(outputFolderName, 0777);
 
  
}


void saveInitialConditions(body bodies[]){
  FILE *initCon;
  char path[30]; 
  sprintf(path,"%s/initialConditions.txt", outputFolderName);
  initCon = fopen(path, "w");
  
  //save initial masses
  fprintf(initCon, "----------Masses---------\n");
  for (int i = 0; i < NofBodies; i++){
    char str[30];
    sprintf(str, "mass (body %d): %f;\n", i, bodies[i].mass );
    fprintf(initCon, "%s", str);
  }

  //save initial positions
  fprintf(initCon, "\n----------Positions---------\n");
  for (int i = 0; i < NofBodies; i++){
    char str[45];
    sprintf(str, "position (body %d): {%f, %f};\n", i, bodies[i].position[0], bodies[i].position[1]);
    fprintf(initCon, "%s", str);
  }

  //save initial velocities
  fprintf(initCon, "\n----------Velocities---------\n");
  for (int i = 0; i < NofBodies; i++){
    char str[45];
    sprintf(str, "velocity (body %d): {%f, %f};\n", i, bodies[i].velocity[0], bodies[i].velocity[1]);
    fprintf(initCon, "%s", str);
  }
  fclose(initCon);

}

void createBodyFiles(body bodies[]) {

  char filename[20]; // Increased size to accommodate folder name
  

  for (int i = 0; i < NofBodies; i++) {
    sprintf(filename, "%s/%d.txt", outputFolderName, i);
    bodies[i].bodyFile = fopen(filename, "w");
    
  
  }
}



void computeAndWrite(int loop, body bodies[]){
  for (int i = 0; i < NofBodies; i++){
    double force[] = {0, 0};
    
    for (int j = 0; j < NofBodies; j++){
      
      if(j == i){
        continue;
      }
      double distanceAsVector[] = {0, 0};
      double forceThis[] = {0, 0};
      double distance = 0;

      distanceAsVector[0] = bodies[i].position[0] - bodies[j].position[0];
      distanceAsVector[1] = bodies[i].position[1] - bodies[j].position[1];
      
      distance = sqrt( pow(distanceAsVector[0], 2) + pow(distanceAsVector[1], 2) );

      forceThis[0] = -(gravityConstant * bodies[i].mass * bodies[j].mass * distanceAsVector[0]) / pow(distance, 3);
      forceThis[1] = -(gravityConstant * bodies[i].mass * bodies[j].mass * distanceAsVector[1]) / pow(distance, 3);
      

      
   
      
      force[0] += forceThis[0];
      force[1] += forceThis[1];


    }

       
    bodies[i].accelerationOld[0] = bodies[i].acceleration[0];
    bodies[i].accelerationOld[1] = bodies[i].acceleration[1];

    bodies[i].acceleration[0] = force[0] / bodies[i].mass;
    bodies[i].acceleration[1] = force[1] / bodies[i].mass;

    double k1[2];
    double k2[2];
    double k4[2];
    for (int j = 0; j < 2; j++){

      
      k1[j] = bodies[i].accelerationOld[j];
    
      k2[j] = (bodies[i].accelerationOld[j] + bodies[i].acceleration[j])/2;
    
      k4[j] = bodies[i].acceleration[j];


      bodies[i].velocityOld[j] = bodies[i].velocity[j];
      bodies[i].velocity[j] += timeDelta*(k1[j] + 4*k2[j] + k4[j])/6;
      
      k1[j] = bodies[i].velocityOld[j];
    
      k2[j] = (bodies[i].velocityOld[j] + bodies[i].velocity[j])/2;
    
      k4[j] = bodies[i].velocity[j];

      bodies[i].position[j] += timeDelta*(k1[j] + 4*k2[j] + k4[j])/6;



  
 


    }

    if (loop % compress == 0){
      char out[50] = {0};
      sprintf(out, "%f,%f\n", bodies[i].position[0], bodies[i].position[1]);
      
      
      fprintf(bodies[i].bodyFile, "%s", out);
    }

    

    
  }
  if (loop % (int)(positions / logsPerComputation) == 0){
    double done = loop / positions;
    curTime = clock();
    double timeElapsed =(double)(curTime - start) / (double)(CLOCKS_PER_SEC);
    double approxTimeLeft = timeElapsed / done - timeElapsed;
    printf("Done : %f %%; Time Elapsed : %f s; approx time left : %f s; \n", done*100, timeElapsed, approxTimeLeft);
  }
}

int main(){
  

 
  start = clock();
  
  //specify initial conditions
  double massInit[] = { 1, 1, 1, 1, 1 };
  double posInit[][2] =
  {
    {2, -2},
    {0.74, 2},
    {-1, -1},
    {1, -0.75},
    {1, 0.88}
  };
  double velInit[][2] =
  {
    {-2.7, 0.7},
    {2.25, 2},
    {3.75, 0.25},
    {1.7, 0},
    {-1.83, 1.25}


  };
  NofBodies = sizeof(massInit) / sizeof(massInit[0]);

  body bodies[NofBodies]; 
  
  for(int i = 0; i < NofBodies; i++){
    bodies[i].mass = massInit[i];
    for(int j = 0; j < 2; j++){
      
      bodies[i].position[j] = posInit[i][j];
      bodies[i].velocity[j] = velInit[i][j];
    }
  }


  positions = pow(10, 8);
  timeDelta = pow(10, -8);
  logsPerComputation = pow(10, 2);
  compress = pow(10, 4);

  createOutputFolder();
 
  saveInitialConditions(bodies);
  
  printf("saved initial conditions\n");

  createBodyFiles(bodies);
  
  for (int i = 0; i < positions; i++){
    
    computeAndWrite(i, bodies);
  }

  end = clock();

  double time_taken = (double)(end - start) / (double)(CLOCKS_PER_SEC);
  
  printf("Time taken: %f s\n", time_taken);

  return 0;


}