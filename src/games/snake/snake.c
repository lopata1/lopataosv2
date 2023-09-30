#include <os/games/snake/snake.h>
#include <os/output.h>
#include <os/ints/interrupts.h>
#include <os/globals.h>
#include <os/graphics.h>
#include <os/memory.h>
#include <stdint.h>

#define GAME_WIDTH 25
#define GAME_HEIGHT 15

static const uint16_t field_width = (SCREEN_WIDTH - 2*10 - (GAME_WIDTH - 1) * 1) / GAME_WIDTH;
static const uint16_t field_height = (SCREEN_HEIGHT - 2*10 - (GAME_HEIGHT - 1) * 1) / GAME_HEIGHT;

static uint8_t running = 0;
static float deltatime = 0;
static uint8_t game_over = 0;

static rectangle_t wall_left;
static rectangle_t wall_right;
static rectangle_t wall_top;
static rectangle_t wall_down;

static enum FIELD_VALUES {FIELD_EMPTY, FIELD_SNAKE, FIELD_POINT};
static enum DIRECTION {UP, DOWN, LEFT, RIGHT};

static uint8_t game[GAME_HEIGHT][GAME_WIDTH];

static struct snake_body {
    vector2d_t position;
    enum DIRECTION direction;
};

static struct {
    struct snake_body body[GAME_HEIGHT*GAME_WIDTH];
    uint16_t size;
} snake;

static vector2d_t point;

static void handle_input();
static void handle_logic();
static void draw();
static void init_snake();
static void start_new_game();
static void update_game();
static void increase_snake();
static void move_snake();

void snake_start()
{
    init_snake();

    running = 1;

    while(running)
    {
        timer_ms_passed = 0;
        handle_input();
        if (!game_over) handle_logic();
        draw();
        deltatime = (float) timer_ms_passed / 1000;
        sleep(400);
    }
}

static void init_snake()
{
    start_new_game();

    wall_top = make_rectangle(make_vector2d(0, 0), make_vector2d(SCREEN_WIDTH, 10), 15);
    wall_down = make_rectangle(make_vector2d(0, SCREEN_HEIGHT - 10), make_vector2d(SCREEN_WIDTH, 10), 15);
    wall_left = make_rectangle(make_vector2d(0, 0), make_vector2d(10, SCREEN_HEIGHT), 15);
    wall_right = make_rectangle(make_vector2d(SCREEN_WIDTH-10, 0), make_vector2d(10, SCREEN_HEIGHT), 15);
}

static void start_new_game()
{
    snake.size = 1;
    struct snake_body snake_head = {make_vector2d(GAME_WIDTH/2, GAME_HEIGHT/2), RIGHT};
    snake.body[0] = snake_head;
    increase_snake();
    increase_snake();
    point = make_vector2d(3, 3);
}

static void handle_input()
{
    if(input_pressed)
    {
        input_pressed = 0;
        switch(last_key_pressed)
        {
            case 'W':
                snake.body[0].direction = UP;
                break;
            case 'S':
                snake.body[0].direction = DOWN;
                break;
            case 'A':
                snake.body[0].direction = LEFT;
                break;
            case 'D':
                snake.body[0].direction = RIGHT;
                break;
            case 'Q':
                running = 0;
                break;
        }
    }
}

static void handle_logic()
{
    move_snake();

    if(snake.body[0].position.x == point.x && snake.body[0].position.y == point.y)
    {
        increase_snake();
        point = make_vector2d(0, 0);
    }

    update_game();
}

static void move_snake()
{
    struct snake_body last_body = snake.body[0];
    switch(snake.body[0].direction)
    {
        case UP:
            snake.body[0].position.y -= 1;
            break;
        case DOWN:
            snake.body[0].position.y += 1;
            break;
        case LEFT:
            snake.body[0].position.x -= 1;
            break;
        case RIGHT:
            snake.body[0].position.x += 1;
            break;
    }
    for(int i = 1; i < snake.size; i++)
    {
        struct snake_body temp = last_body;
        last_body = snake.body[i];
        snake.body[i] = temp;
    }
}

static void increase_snake()
{
    vector2d_t position = snake.body[snake.size - 1].position;
    enum DIRECTION direction = snake.body[snake.size - 1].direction;

    switch(direction)
    {
        case UP:
            position.y -= 1;
            break;
        case DOWN:
            position.y += 1;
            break;
        case LEFT:
            position.x += 1;
            break;
        case RIGHT:
            position.x -= 1;
            break;
    }

    struct snake_body snake_part = {position, direction};
    snake.body[snake.size++] = snake_part;
}

static void update_game()
{
    memset((char*)game, FIELD_EMPTY, GAME_HEIGHT*GAME_WIDTH);

    for(int i = 0; i < snake.size; i++)
        game[snake.body[i].position.y][snake.body[i].position.x] = FIELD_SNAKE;

    game[point.y][point.x] = FIELD_POINT;
}

static void draw()
{
    clear_buffer(0);
    draw_rectangle(wall_top);
    draw_rectangle(wall_down);
    draw_rectangle(wall_left);
    draw_rectangle(wall_right);

    rectangle_t field = make_rectangle(make_vector2d(10, 10), make_vector2d(field_width, field_height), 1);
    for(int i = 0; i < GAME_HEIGHT; i++)
    {
        field.position.x = 10;
        for(int j = 0; j < GAME_WIDTH; j++)
        {
            switch(game[i][j])
            {
                case FIELD_EMPTY:
                    field.color = 1;
                    break;
                case FIELD_SNAKE:
                    field.color = 2;
                    break;
                case FIELD_POINT:
                    field.color = 3;
                    break;
            }
            draw_rectangle(field);
            field.position.x += field_width + 1;
        }
        field.position.y += field_height + 1;
    }

    display_buffer();
}