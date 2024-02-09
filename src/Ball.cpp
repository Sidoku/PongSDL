#ifndef BALL_H
#define BALL_H

#include "Vec2.cpp"
#include <SDL2/SDL.h>
#include "CollisionType.cpp"
#include "Contact.cpp"

const int BALL_WIDTH = 15;
const int BALL_HEIGHT = 15;
const float BALL_SPEED = 0.7f;

class Ball
{

public:
    Ball(const int WINDOW_HEIGHT, const int WINDOW_WIDTH, Vec2 position, Vec2 velocity)
        :WINDOW_HEIGHT(WINDOW_HEIGHT), WINDOW_WIDTH(WINDOW_WIDTH), position(position), velocity(velocity)
    {
        rect.x = static_cast<int>(position.x);
        rect.y = static_cast<int>(position.y);
        rect.w = BALL_WIDTH;
        rect.h = BALL_HEIGHT;
    }

    void Update(float dt)
    {
        position += velocity * dt;
    }

    void Draw(SDL_Renderer* renderer)
    {
        rect.x = static_cast<int>(position.x);
        rect.y = static_cast<int>(position.y);

        SDL_RenderFillRect(renderer, &rect);
    }

    void CollideWithPaddle(Contact const& contact)
    {
        position.x += contact.penetration;
        velocity.x = -velocity.x;

        if (contact.type == CollisionType::Top)
        {
            velocity.y = -0.75f * BALL_SPEED;
        }
        else if (contact.type == CollisionType::Bottom)
        {
            velocity.y = 0.75f * BALL_SPEED;
        }
    }

    void CollideWithWall(Contact const& contact)
    {
        if ((contact.type == CollisionType::Top) || (contact.type == CollisionType::Bottom))
        {
            position.y += contact.penetration;
            velocity.y = -velocity.y;
        }
        else if (contact.type == CollisionType::Left)
        {
            position.x = WINDOW_WIDTH / 2.0f;
            position.y = WINDOW_HEIGHT / 2.0f;
            velocity.x = BALL_SPEED;
            velocity.y = 0.75f * BALL_SPEED;
        }
        else if (contact.type == CollisionType::Right)
        {
            position.x = WINDOW_WIDTH / 2.0f;
            position.y = WINDOW_HEIGHT / 2.0f;
            velocity.x = -BALL_SPEED;
            velocity.y = 0.75f * BALL_SPEED;
        }
    }

    Vec2 position;
    Vec2 velocity;
    SDL_Rect rect;
    const int WINDOW_HEIGHT;
    const int WINDOW_WIDTH;
};

#endif // BALL_H