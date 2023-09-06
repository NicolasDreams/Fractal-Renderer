# Fractal Renderer
A simple fractal renderer written in c++ using SDL2. It currently has Mandelbrot and Burning Ship fractals.
In the future, I plan to add multithreading, anti-aliasing and more fractals.

Download executable on my itch.io page: https://nicolasdreams.itch.io/fractal-renderer
## How to Compile and Run the Project
### Linux
You will need g++ and SDL2. If you are on a deb based distro you can install them by running these commands:
```
sudo apt-get install g++
sudo apt-get install libsdl2-2.0
sudo apt-get install libsdl2-dev
```
Then to compile and run the project, run these commands in the root directory of the project:\
Debug:
```
g++ -c src/*.cpp -std=c++14 -m64 -g -Wall
g++ *.o -o bin/debug/main -lSDL2main -lSDL2
./bin/debug/main
```
Release:
```
g++ -c src/*.cpp -std=c++14 -m64 -O3 -Wall
g++ *.o -o bin/release/main -s -lSDL2main -lSDL2
./bin/release/main
```
### Windows
WIP
## Controls
- Left Mouse - Zoom in
- Right Mouse - Zoom out
- F11 - Toggle fullscreen
- Escape - Either switches out to window mode or quits the application
- M - Increase iterations
- N - Decrease iterations
- X - Increase color rotation speed
- Z - Decrease color rotation speed
- V - Increase color offset
- C - Decrease color offset
- R - Return to initial position, iterations (128) and colors
- 1 - Mandelbrot Set
- 2 - Burning Ship
- F5 - Manually refresh window
