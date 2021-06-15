#pragma once
#include<SDL.h>

struct Vector2 {
	float x;
	float y;
};

class Game {
public:
	Game();

	// initialize game
	bool initialize();

	// run loop until game is over
	void RunLoop();

	// shutdown the game
	void Shutdown();

private:

	// helper variables for game loop
	void ProcessInput();

	void UpdateGame();

	void GenerateOutput();

	// keyboard
	const Uint8* state;

	// window for game
	SDL_Window* mWindow;

	// game should continue to run
	bool mIsRunning;

	// graphics renderer (draws graphics)
	SDL_Renderer* renderer;

	Vector2 L_PaddlePos;
	Vector2 R_PaddlePos;
	Vector2 BallPos;
	Vector2 BallVel;

	// holds the amount of elapsed time since the last frame
	Uint32 TicksCount;

	// left paddle direction
	int l_paddleDir;
	// right paddle direction
	int r_paddleDir;
};