//
// Created by marcel on 12.11.23.
//

#include "breakout.h"
#include "io.h"
#include "fb.h"
#include "itoa.h"
#include "terminal.h"

// The screen
#define WIDTH         1920
#define HEIGHT        1080
#define MARGIN        30
#define VIRTWIDTH     (WIDTH-(2*MARGIN))
#define FONT_BPG      8

// For the bricks
#define ROWS          5
#define COLS          10
unsigned int bricks = ROWS * COLS;

// Gameplay
#define NUM_LIVES     3

struct GameObject {
    unsigned int type;
    int x;
    int y;
    int width;
    int height;
    unsigned char alive;
    unsigned char color;
};

enum {
    BRICK = 1,
    PADDLE = 2,
    BALL = 3
};

unsigned int number_of_objects = 0;

struct GameObject *objects = (struct GameObject *) SAFE_ADDRESS;
struct GameObject *ball;
struct GameObject *paddle;

void removeObject(struct GameObject *object) {
    drawRect(object->x, object->y, object->x + object->width, object->y + object->height, 0, 1);
    object->alive = 0;
}

void moveObject(struct GameObject *object, int x_offset, int y_offset) {
    moveRect(object->x, object->y, object->width, object->height, x_offset, y_offset, 0x00);
    object->x = object->x + x_offset;
    object->y = object->y + y_offset;
}

struct GameObject *detectCollision(struct GameObject *with, int x_offset, int y_offset) {
    for (int i = 0; i < number_of_objects; i++) {
        if (&objects[i] != with && objects[i].alive == 1) {
            if (with->x + x_offset > objects[i].x + objects[i].width ||
                objects[i].x > with->x + x_offset + with->width) {
                // with is too far left or right to collide
            } else if (with->y + y_offset > objects[i].y + objects[i].height ||
                       objects[i].y > with->y + y_offset + with->height) {
                // with is too far up or down to collide
            } else {
                // Collision!
                return &objects[i];
            }
        }
    }
    return 0;
}
// Initialize Objects

void initBricks() {
    int brick_width = 32;
    int brick_height = 8;
    int brick_spacer = 20;
    static int brick_colors[] = {0x11, 0x22, 0xEE, 0x44, 0x66};

    int y_brick = MARGIN + brick_height;

    for (int i = 0; i < ROWS; i++) {
        int x_brick = MARGIN + (VIRTWIDTH / COLS / 2) - (brick_width / 2);

        for (int j = 0; j < COLS; j++) {
            drawRect(x_brick, y_brick, x_brick + brick_width, y_brick + brick_height, brick_colors[i], 1);

            objects[number_of_objects].type = BRICK;
            objects[number_of_objects].x = x_brick;
            objects[number_of_objects].y = y_brick;
            objects[number_of_objects].width = brick_width;
            objects[number_of_objects].height = brick_height;
            objects[number_of_objects].alive = 1;
            objects[number_of_objects].color = brick_colors[i];

            number_of_objects++;

            x_brick += (VIRTWIDTH / COLS);
        }
        y_brick = y_brick + brick_spacer;
    }
}

void initBall() {
    drawCircle(WIDTH / 2, HEIGHT / 2, 15, 0x55, 1);

    objects[number_of_objects].type = BALL;
    objects[number_of_objects].x = (WIDTH / 2) - 15;
    objects[number_of_objects].y = (HEIGHT / 2) - 15;
    objects[number_of_objects].width = 15 * 2;
    objects[number_of_objects].height = 15 * 2;
    objects[number_of_objects].alive = 1;
    objects[number_of_objects].color = 0x55;
    ball = &objects[number_of_objects];
    number_of_objects++;
}

void initPaddle() {
    int paddle_width = 80;
    int paddle_height = 20;

    drawRect((WIDTH - paddle_width) / 2, (HEIGHT - MARGIN - paddle_height), (WIDTH - paddle_width) / 2 + paddle_width,
             (HEIGHT - MARGIN), 0x11, 1);

    objects[number_of_objects].type = PADDLE;
    objects[number_of_objects].x = (WIDTH - paddle_width) / 2;
    objects[number_of_objects].y = (HEIGHT - MARGIN - paddle_height);
    objects[number_of_objects].width = paddle_width;
    objects[number_of_objects].height = paddle_height;
    objects[number_of_objects].alive = 1;
    objects[number_of_objects].color = 0x11;
    paddle = &objects[number_of_objects];
    number_of_objects++;
}

void drawScoreboard(int score, int lives) {
    char tens = score / 10;
    score -= (10 * tens);
    char ones = score;

    drawString((WIDTH / 2) - 252, MARGIN - 25, "Score: 0     Lives:  ", 0x0f, 3);
    drawChar(tens + 0x30, (WIDTH / 2) - 252 + (8 * 8 * 3), MARGIN - 25, 0x0f, 3);
    drawChar(ones + 0x30, (WIDTH / 2) - 252 + (8 * 9 * 3), MARGIN - 25, 0x0f, 3);
    drawChar((char) lives + 0x30, (WIDTH / 2) - 252 + (8 * 20 * 3), MARGIN - 25, 0x0f, 3);
}

void startBreakout() {
    struct GameObject *foundObject;

    int lives = NUM_LIVES;
    int points = 0;

    int velocity_x = 1;
    int velocity_y = 3;

    initBricks();
    initBall();
    initPaddle();
    drawScoreboard(points, lives);

    while (lives > 0 && bricks > 0) {
        //TODO: Input controls

        foundObject = detectCollision(ball, velocity_x, velocity_y);

        if (foundObject) {
            if (foundObject == paddle) {
                velocity_y = -velocity_y;

                // Side of the paddle?
                if (ball->x + ball->width + velocity_x == paddle->x ||
                    ball->x + velocity_x == paddle->x + paddle->width) {
                    velocity_x = -velocity_x;
                }
            } else if (foundObject->type == BRICK) {
                removeObject(foundObject);
                velocity_y = -velocity_y;
                bricks--;
                points++;
                drawScoreboard(points, lives);
            }
        }

        wait_msec(4000);
        moveObject(ball, velocity_x, velocity_y);


        // Check we're in the game arena still
        if (ball->x + ball->width >= WIDTH - MARGIN) {
            velocity_x = -velocity_x;
        } else if (ball->x <= MARGIN) {
            velocity_x = -velocity_x;
        } else if (ball->y + ball->height >= HEIGHT - MARGIN) {
            lives--;

            removeObject(ball);
            removeObject(paddle);

            initBall();
            initPaddle();
            drawScoreboard(points, lives);
        } else if (ball->y <= MARGIN) {
            velocity_y = -velocity_y;
        }
    }

    int zoom = WIDTH / 192;
    int str_width = 10 * FONT_BPG * zoom;
    int str_height = FONT_BPG * zoom;

    if (bricks == 0) {
        drawString((WIDTH / 2) - (str_width / 2), (HEIGHT / 2) - (str_height / 2), "Well done!", 0x02, zoom);
    } else {
        drawString((WIDTH / 2) - (str_width / 2), (HEIGHT / 2) - (str_height / 2), "Game over!", 0x04, zoom);
    }
}
