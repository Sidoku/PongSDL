#ifndef PADDLE_H
#define PADDLE_H

#include "Vec2.cpp"
#include <SDL2/SDL.h>

const int PADDLE_WIDTH = 15;
const int PADDLE_HEIGHT = 100;

class Paddle
{
public:

    Paddle(const int WINDOW_HEIGHT, const int WINDOW_WIDTH, Vec2 position, Vec2 velocity) :WINDOW_HEIGHT(WINDOW_HEIGHT), WINDOW_WIDTH(WINDOW_WIDTH), position(position), velocity(velocity)
    {
        rect.x = static_cast<int>(position.x);
        rect.y = static_cast<int>(position.y);
        rect.w = PADDLE_WIDTH;
        rect.h = PADDLE_HEIGHT;
    }

    void Update(float dt)
    {
        position += velocity * dt;

        if (position.y < 0)
        {
            // Restrict to top of the screen
            position.y = 0;
        }
        else if (position.y > WINDOW_HEIGHT - PADDLE_HEIGHT)
        {
            // Restrict to bottom of the screen
            position.y = WINDOW_HEIGHT - PADDLE_HEIGHT;
        }
    }

    void Draw(SDL_Renderer *renderer)
    {
        rect.y = static_cast<int>(position.y);

        SDL_RenderFillRect(renderer, &rect);
    }

    Vec2 position;
    Vec2 velocity;
    SDL_Rect rect;
    const int WINDOW_HEIGHT;
    const int WINDOW_WIDTH;
};
#endif // PADDLE_H