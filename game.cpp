#include"game.h"
#include<iostream>


const int thickness = 15;
const int paddleH = 100.0f;

Game::Game()
	:mWindow(nullptr),mIsRunning(true), TicksCount(0),
	l_paddleDir(0), r_paddleDir(0)
{

}

bool Game::initialize() 
{
	mWindow = nullptr;
	mIsRunning = true;

	int sdlResult = SDL_Init(SDL_INIT_VIDEO);

	if (sdlResult != 0)
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError);
		return false;
	}

	mWindow = SDL_CreateWindow("Pong", 100, 100, 1024, 768, 0);

	if (!mWindow)
	{
		SDL_Log("Failed to create window: %s", SDL_GetError);
		return false;
	}

	renderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	if (!renderer)
	{
		SDL_Log("Failed to create renderer: %s", SDL_GetError);
		return false;
	}

	L_PaddlePos.x = 10.0f;
	L_PaddlePos.y = 768.0f / 2.0f;
	R_PaddlePos.x = 10.0f;
	R_PaddlePos.y = 768.0f / 2.0f;
	BallPos.x = 1024.0f / 2.0f;
	BallPos.y = 768.0f / 2.0f;
	BallVel.x = -200.0f;
	BallVel.y = 235.0f;

	return true;

}

void Game::Shutdown() 
{
	SDL_DestroyWindow(mWindow);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}

void Game::RunLoop()
{
	while (mIsRunning)
	{
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::ProcessInput()
{
	SDL_Event event;

	// while there are still events in the queue, process
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			mIsRunning = false;
			break;
		}
	}

	// get keyboard
	state = SDL_GetKeyboardState(NULL);

	if (state[SDL_SCANCODE_ESCAPE])
	{
		mIsRunning = false;
	}
}

void Game::UpdateGame()
{
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), TicksCount + 16))
		;

	// getticks() calculates the amount of ms elapsed since SDL_init
	float deltaTime = (SDL_GetTicks() - TicksCount) / 1000.0f;

	// watching out for delta time that is too high
	if (deltaTime > 0.05f)
		deltaTime = 0.05f;

	// update ball's position
	BallPos.x += BallVel.x * deltaTime;
	BallPos.y += BallVel.y * deltaTime;

	// check if ball collides with top wall AND check to see if the velocity is negative (moving towards top wall)
	// the additional velocity check is to make sure that the ball will not get stuck on the top wall if 
	// the ball doesn't move away from the wall in time (after colliding with it)
	if (BallPos.y <= thickness && BallVel.y < 0.0f)
	{
		BallVel.y = (BallVel.y * -1) + 100;
	}

	// check if ball collides with bottom wall
	if (BallPos.y >= 768 - thickness && BallVel.y > 0.0f)
	{
		BallVel.y = (BallVel.y * -1) - 100;
	}

	// check if ball collides with left paddle
	float L_diff = L_PaddlePos.y - BallPos.y;
	L_diff =  (L_diff > 0.0f) ? L_diff : -L_diff;

	if ((L_diff <= paddleH / 2.0f) && BallPos.x <= 25.0f && BallVel.x < 0.0f)
	{
		BallVel.x = (BallVel.x * -1);
	}

	else if (BallPos.x <= 0.0f)
	{
		std::cout << "Game over!\n";
		mIsRunning = false;
	}

	// check if ball collides with right paddle
	
	float R_diff = R_PaddlePos.y - BallPos.y;
	R_diff = (R_diff > 0.0) ? R_diff : -R_diff;

	if ((R_diff <= paddleH / 2.0f) && BallPos.x >= (1024 - 10 - thickness) && BallVel.x > 0.0f)
	{
		BallVel.x = (BallVel.x * -1);
	}

	else if (BallPos.x >= 1024)
	{
		std::cout << "Game over!\n";
		mIsRunning = false;
	}
	
	l_paddleDir = 0;
	r_paddleDir = 0;
	// left paddle moves up if W is pressed
	if (state[SDL_SCANCODE_W])
	{
		l_paddleDir -= 1;
	}
	// left paddle moves down is S is pressed
	if (state[SDL_SCANCODE_S])
	{
		l_paddleDir += 1;
	}

	// same as above but for right paddle
	if (state[SDL_SCANCODE_UP])
	{
		r_paddleDir -= 1;
	}

	if (state[SDL_SCANCODE_DOWN])
	{
		r_paddleDir += 1;
	}

	// update left paddle in delta time
	if (l_paddleDir != 0)
	{
		L_PaddlePos.y += l_paddleDir * 500.0f * deltaTime;

		// guards against going off screen
		if (L_PaddlePos.y < paddleH / 2.0f + thickness)
		{
			L_PaddlePos.y = paddleH / 2.0f + thickness;
		}
		
		else if (L_PaddlePos.y > 768.0f - paddleH / 2.0f + thickness)
		{
			L_PaddlePos.y = 768.0f - paddleH / 2.0f + thickness;
		}
	}

	// update right paddle in delta time
	if (r_paddleDir != 0)
	{
		R_PaddlePos.y += r_paddleDir * 500.0f * deltaTime;

		// guards against going off screen
		if (R_PaddlePos.y < paddleH / 2.0f + thickness)
		{
			R_PaddlePos.y = paddleH / 2.0f + thickness;
		}

		else if (R_PaddlePos.y > 768.0f - paddleH / 2.0f + thickness)
		{
			R_PaddlePos.y = 768.0f - paddleH / 2.0f + thickness;
		}
	}
	
	// update tick counts for next frame
	TicksCount = SDL_GetTicks();
}

void Game::GenerateOutput()
{
	// clear back buffer

	SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
	SDL_RenderClear(renderer);

	// draw to the screen
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

	// draw top wall
	SDL_Rect wall{
		0,
		0,
		1024,
		thickness
	};
	SDL_RenderFillRect(renderer, &wall);

	// draw bottom wall
	wall.y = 768 - thickness;
	SDL_RenderFillRect(renderer, &wall);

	// draw left paddle
	SDL_Rect l_paddle{
		static_cast<int>(L_PaddlePos.x),
		static_cast<int>(L_PaddlePos.y - paddleH / 2),
		thickness,
		static_cast<int>(paddleH)
	};
	SDL_RenderFillRect(renderer, &l_paddle);

	// draw right paddle
	SDL_Rect r_paddle{
		static_cast<int>(R_PaddlePos.x + 1014 - thickness - 10), // 10 + 1014 = 1024 - thickness - 10 margin
		static_cast<int>(R_PaddlePos.y - paddleH / 2),
		thickness,
		static_cast<int>(paddleH)
	};
	SDL_RenderFillRect(renderer, &r_paddle);

	// draw ball
	SDL_Rect ball{
		static_cast<int>(BallPos.x - thickness / 2),
		static_cast<int>(BallPos.y - thickness / 2),
		thickness,
		thickness
	};
	SDL_RenderFillRect(renderer, &ball);

	SDL_RenderPresent(renderer);
}