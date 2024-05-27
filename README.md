This is a Project ive been working on for a while now, its a program that simulates gravitational interaction between N bodies.
Whole project is written in C and uses raylib library for visualization ( https://github.com/raysan5/raylib ).
The project is split into 2 parts:

1. Computation Module (The brain):
    This part calculates the positions of the bodies during the simulation.
    It currently uses RKF45 for solving ODEs.
    (The code isnt optimized very well, so there may be few bugs)

2. Graphical Visualizer (The thing that converts long .txt files into pretty animations and/or images):
    This part reads the .txt files with the positions and converts them into (pretty) animations and/or images.
    It utilizes the raylib library ( https://github.com/raysan5/raylib ) to visualize the data.
    (Also isnt very optimized)

Any suggestions on how to make the code faster and/or less buggy (no bugs present to my knowledge) are welcome.
The folder "outputTxtFiles" contains output of a short 5 body problem (it took abt 15 mins to compute positions).


To compile GravSimComputationProgram with gcc, run "gcc main.c -lm" in GravSimComputationProgram directory.

To compile GravSimVisualizer with gcc, you first need to install raylib (refer to https://github.com/raysan5/raylib for instructions) and then run "gcc main.c -lraylib".
