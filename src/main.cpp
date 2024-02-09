// Support Code written by Michael D. Shah
//
// You can modify this could however you like in order to implement your 'pong'
//
// Last Updated: 01/17/23
// Please do not redistribute without asking permission.

// ==================== Libraries ==================
// Depending on the operating system we use
// The paths to SDL are actually different.
// The #define statement should be passed in
// when compiling using the -D argument.
// This gives an example of how a programmer
// may support multiple platforms with different
// dependencies.
#if defined(LINUX) || defined(MINGW)
#include <SDL2/SDL.h>
#else
// Windows and Mac use a different path
// If you have compilation errors, change this as needed.
#include <SDL2/SDL.h>
#endif

#ifndef MAIN_H
#define MAIN_H

#include <SDL2/SDL_ttf.h>
#include "Ball.cpp"
#include "Paddle.cpp"
#include "PlayerScore.cpp"
#include <iostream>
#include <chrono>
#include <string>

const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;

enum Buttons
{
	PaddleRightUp = 0,
	PaddleRightDown,
	PaddleLeftUp,
	PaddleLeftDown,
};

Contact CheckPaddleCollision(Ball const &ball, Paddle const &paddle)
{
	float ballLeft = ball.position.x;
	float ballRight = ball.position.x + BALL_WIDTH;
	float ballTop = ball.position.y;
	float ballBottom = ball.position.y + BALL_HEIGHT;

	float paddleLeft = paddle.position.x;
	float paddleRight = paddle.position.x + PADDLE_WIDTH;
	float paddleTop = paddle.position.y;
	float paddleBottom = paddle.position.y + PADDLE_HEIGHT;

	Contact contact{};

	if (ballLeft >= paddleRight)
	{
		return contact;
	}

	if (ballRight <= paddleLeft)
	{
		return contact;
	}

	if (ballTop >= paddleBottom)
	{
		return contact;
	}

	if (ballBottom <= paddleTop)
	{
		return contact;
	}

	float paddleRangeUpper = paddleBottom - (2.0f * PADDLE_HEIGHT / 3.0f);
	float paddleRangeMiddle = paddleBottom - (PADDLE_HEIGHT / 3.0f);

	if (ball.velocity.x < 0)
	{
		// Left paddle
		contact.penetration = paddleRight - ballLeft;
	}

	if (ball.velocity.x > 0)
	{
		// Right paddle
		contact.penetration = paddleLeft - ballRight;
	}

	if ((ballBottom > paddleTop) && (ballBottom < paddleRangeUpper))
	{
		contact.type = CollisionType::Top;
	}
	else if ((ballBottom > paddleRangeUpper) && (ballBottom < paddleRangeMiddle))
	{
		contact.type = CollisionType::Middle;
	}
	else
	{
		contact.type = CollisionType::Bottom;
	}

	return contact;
}

Contact CheckWallCollision(Ball const &ball)
{
	float ballLeft = ball.position.x;
	float ballRight = ball.position.x + BALL_WIDTH;
	float ballTop = ball.position.y;
	float ballBottom = ball.position.y + BALL_HEIGHT;

	Contact contact{};

	if (ballLeft < 0.0f)
	{
		contact.type = CollisionType::Left;
	}
	else if (ballRight > WINDOW_WIDTH)
	{
		contact.type = CollisionType::Right;
	}
	else if (ballTop < 0.0f)
	{
		contact.type = CollisionType::Top;
		contact.penetration = -ballTop;
	}
	else if (ballBottom > WINDOW_HEIGHT)
	{
		contact.type = CollisionType::Bottom;
		contact.penetration = WINDOW_HEIGHT - ballBottom;
	}

	return contact;
}

const float PADDLE_SPEED = 1.0f;

// Our entry point into our program.
// Remember, C++ programs have exactly one entry point
// where the program starts.
int main(int argc, char *args[])
{
	SDL_Init(SDL_INIT_VIDEO);
	TTF_Init();

	SDL_Window *window = SDL_CreateWindow("Pong", 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

	TTF_Font *scoreFont = TTF_OpenFont("../include/arial.ttf", 40);
	if (scoreFont == NULL)
	{
		std::cout << ("%s\n", TTF_GetError());
		exit(EXIT_FAILURE);
	}

	Ball ball(WINDOW_HEIGHT, WINDOW_WIDTH, Vec2((WINDOW_WIDTH / 2.0f) - (BALL_WIDTH / 2.0f), (WINDOW_HEIGHT / 2.0f) - (BALL_HEIGHT / 2.0f)), Vec2(BALL_SPEED, 0.0f));

	Paddle paddle_right(WINDOW_HEIGHT, WINDOW_WIDTH, Vec2(WINDOW_WIDTH - 50.f, (WINDOW_HEIGHT / 2.0f) - (PADDLE_HEIGHT / 2.0f)), Vec2(0.0f, 0.0f));
	Paddle paddle_left(WINDOW_HEIGHT, WINDOW_WIDTH, Vec2(50.0f, (WINDOW_HEIGHT / 2.0f) - (PADDLE_HEIGHT / 2.0f)), Vec2(0.0f, 0.0f));

	PlayerScore playerRightScoreText(Vec2(3 * WINDOW_WIDTH / 4.0f, 20.0f), renderer, scoreFont);
	PlayerScore playerLeftScoreText(Vec2(WINDOW_WIDTH / 4.0f, 20.0f), renderer, scoreFont);

	// Game Logic

	{
		bool running = true;
		bool buttons[4] = {};

		float dt = 0.0f;

		int PlayerRightScore = 0;
		int PlayerLeftScore = 0;

		while (running)
		{
			auto startTime = std::chrono::high_resolution_clock::now();

			SDL_Event event;
			while (SDL_PollEvent(&event))
			{
				if (event.type == SDL_QUIT)
				{
					running = false;
				}
				else if (event.type == SDL_KEYDOWN)
				{
					if (event.key.keysym.sym == SDLK_ESCAPE)
					{
						running = false;
					}
					else if (event.key.keysym.sym == SDLK_w)
					{
						buttons[Buttons::PaddleLeftUp] = true;
					}
					else if (event.key.keysym.sym == SDLK_s)
					{
						buttons[Buttons::PaddleLeftDown] = true;
					}
					else if (event.key.keysym.sym == SDLK_UP)
					{
						buttons[Buttons::PaddleRightUp] = true;
					}
					else if (event.key.keysym.sym == SDLK_DOWN)
					{
						buttons[Buttons::PaddleRightDown] = true;
					}
				}
				else if (event.type == SDL_KEYUP)
				{
					if (event.key.keysym.sym == SDLK_w)
					{
						buttons[Buttons::PaddleLeftUp] = false;
					}
					else if (event.key.keysym.sym == SDLK_s)
					{
						buttons[Buttons::PaddleLeftDown] = false;
					}
					else if (event.key.keysym.sym == SDLK_UP)
					{
						buttons[Buttons::PaddleRightUp] = false;
					}
					else if (event.key.keysym.sym == SDLK_DOWN)
					{
						buttons[Buttons::PaddleRightDown] = false;
					}
				}
			}

			if (buttons[Buttons::PaddleLeftUp])
			{
				paddle_left.velocity.y = -PADDLE_SPEED;
			}
			else if (buttons[Buttons::PaddleLeftDown])
			{
				paddle_left.velocity.y = PADDLE_SPEED;
			}
			else
			{
				paddle_left.velocity.y = 0.0f;
			}

			if (buttons[Buttons::PaddleRightUp])
			{
				paddle_right.velocity.y = -PADDLE_SPEED;
			}
			else if (buttons[Buttons::PaddleRightDown])
			{
				paddle_right.velocity.y = PADDLE_SPEED;
			}
			else
			{
				paddle_right.velocity.y = 0.0f;
			}

			// Update the paddle positions
			paddle_left.Update(dt);
			paddle_right.Update(dt);

			// Update ball position
			ball.Update(dt);

			if (Contact contact = CheckPaddleCollision(ball, paddle_left);
				contact.type != CollisionType::None)
			{
				ball.CollideWithPaddle(contact);
			}
			else if (contact = CheckPaddleCollision(ball, paddle_right);
					 contact.type != CollisionType::None)
			{
				ball.CollideWithPaddle(contact);
			}
			else if (contact = CheckWallCollision(ball);
					 contact.type != CollisionType::None)
			{
				ball.CollideWithWall(contact);

				if(contact.type == CollisionType::Left)
				{
					++PlayerRightScore;
					playerRightScoreText.SetScore(PlayerRightScore);
				}
				else if(contact.type == CollisionType::Right)
				{
					++PlayerLeftScore;
					playerLeftScoreText.SetScore(PlayerLeftScore);
				}
			}

			// Clear the window to black
			SDL_SetRenderDrawColor(renderer, 0x0, 0x0, 0x0, 0xFF);
			SDL_RenderClear(renderer);

			// Rendering happens here

			// Set the draw color to white
			SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

			// Draw the net whenever y is not a multiple of 5
			for (int y = 0; y < WINDOW_HEIGHT; ++y)
			{
				if (y % 5)
				{
					SDL_RenderDrawPoint(renderer, WINDOW_WIDTH / 2, y);
				}
			}

			// Draw the ball
			ball.Draw(renderer);

			// Draw the paddles
			paddle_right.Draw(renderer);
			paddle_left.Draw(renderer);

			// Display the scores
			playerLeftScoreText.Draw();
			playerRightScoreText.Draw();

			// Present the backbuffer
			SDL_RenderPresent(renderer);

			// Calculate frame time
			auto stopTime = std::chrono::high_resolution_clock::now();
			dt = std::chrono::duration<float, std::chrono::milliseconds::period>(stopTime - startTime).count();
		}
	}

	// Clean up

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	TTF_CloseFont(scoreFont);
	TTF_Quit();
	SDL_Quit();

	return 0;
}
#endif // MAIN_H