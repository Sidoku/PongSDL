# sdl2-pong
An SDL2 implementation of the classic Pong game.

Each paddle is controlled by two players to make it a PvP game.

## Features
The game contains the following features:
* Each game lasts until one of the players reaches 10 points.
* Each paddle is controlled thorugh the keyboard
* Ball velocity is increased on each hit with a paddle.
* Ball velocity does not exceed the pre-defined maximum velocity.
* Ball movement stops after a point is scored and is positioned at the side that lost the point.
* Ball direction is randomized from four different directions after each reset.
* Paddles are returned to their default position after each reset.

## External Dependencies
This implementation has external dependencies on the following libraries:
* SDL2
* SDL2_ttf

## Compilation
Easiest way to compile the code is to use the [CMake](https://cmake.org/) utility.

Required tools and libraries for the compilation:
1. [CMake](https://cmake.org/) (ver. >=3.7.2)
2. [SDL2](https://www.libsdl.org/download-2.0.php) (ver. >=2.0.5)
3. [SDL2_ttf](https://www.libsdl.org/projects/SDL_ttf/) (ver. >=2.0.14)
4. A compilation toolkit supported by the CMake utility (e.g. Visual Studio).

CMake compilation uses Find*-modules from the /cmake folder which specify where CMake looks for the required libraries.

Without any modifications, CMake will make use the following environment variable paths.
* SDL2DIR, which should point to the root directory of the SDL2 library.
* SDL2TTFDIR, which should point to the root directory of the SDL2_ttf library.

## Screenshots
![alt text](https://github.com/toivjon/sdl2-pong/blob/master/screenshots/welcome-scene.png "WelcomeScene")
![alt text](https://github.com/toivjon/sdl2-pong/blob/master/screenshots/court-scene.png "CourtScene")
![alt text](https://github.com/toivjon/sdl2-pong/blob/master/screenshots/endgame-scene.png "EndGameScene")
