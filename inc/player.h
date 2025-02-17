#ifndef PLAYER_H
#define PLAYER_H

struct Player
{
    int walkDirection; /* 1 for forward, -1 for backward, 0 for none */
    int turnDirection; /* 1 for right, -1 for left, 0 for none */
    float x; /* Player's X position */
    float y; /* Player's Y position */
    float width;
    float height;
    float rotation_angle;
    float moveSpeed;
    float rotationSpeed;
    float turnSpeed;
};

extern struct Player p;

#endif
