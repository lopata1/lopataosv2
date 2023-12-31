#include <os/games/snake/snake.h>
#include <os/output.h>
#include <os/ints/interrupts.h>
#include <os/globals.h>
#include <os/graphics.h>
#include <os/memory.h>
#include <os/math.h>
#include <stdint.h>


//#define QEMU
#ifdef QEMU

#define GAME_SLEEP 160

#else

#define GAME_SLEEP 160*1.6

#endif


#define GAME_WIDTH 15//25
#define GAME_HEIGHT 9//15

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
enum DIRECTION tail_direction = RIGHT;

static text_t game_over_text1;
static text_t game_over_text2;
static text_t game_over_text3;

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
    srand(timer_ms_since_boot);

    init_snake();

    running = 1;

    while(running)
    {
        timer_ms_passed = 0;
        handle_input();
        if (!game_over) handle_logic();
        draw();
        deltatime = (float) timer_ms_passed / 1000;
        sleep(GAME_SLEEP);
    }
}

static void init_snake()
{
    start_new_game();

    wall_top = make_rectangle(make_vector2d(0, 0), make_vector2d(SCREEN_WIDTH, 10), 0x68);
    wall_down = make_rectangle(make_vector2d(0, SCREEN_HEIGHT - 10), make_vector2d(SCREEN_WIDTH, 10), 0x68);
    wall_left = make_rectangle(make_vector2d(0, 0), make_vector2d(10, SCREEN_HEIGHT), 0x68);
    wall_right = make_rectangle(make_vector2d(SCREEN_WIDTH-10, 0), make_vector2d(10, SCREEN_HEIGHT), 0x68);

    game_over_text1 = make_text("KRAJ IGRE!", make_vector2d(center_text_x(10), 30), 15, 0);
    game_over_text2 = make_text("BODOVI: 0  ", make_vector2d(center_text_x(11), 50), 15, 0);
    game_over_text3 = make_text("PRITISNITE SPACE DA IGRATE PONOVO", make_vector2d(center_text_x(33), 70), 15, 0);
}

static void start_new_game()
{
    tail_direction = RIGHT;
    game_over = 0;
    snake.size = 1;
    struct snake_body snake_head = {make_vector2d(GAME_WIDTH/2, GAME_HEIGHT/2), RIGHT};
    snake.body[0] = snake_head;
    increase_snake();
    increase_snake();
    point = make_vector2d(rand(GAME_WIDTH - 1), rand(GAME_HEIGHT - 1));
}

static void handle_input()
{
    if(input_pressed)
    {
        input_pressed = 0;
        struct snake_body* head = &snake.body[0];
        switch(last_key_pressed)
        {
            case 'W':
                if (head->direction != DOWN) head->direction = UP;
                break;
            case 'S':
                if (head->direction != UP) head->direction = DOWN;
                break;
            case 'A':
                if (head->direction != RIGHT) head->direction = LEFT;
                break;
            case 'D':
                if (head->direction != LEFT) head->direction = RIGHT;
                break;
            case ' ':
                if(game_over) start_new_game();
                break;
            case 'Q':
                running = 0;
                break;
        }
    }
}

static void set_game_over()
{
    game_over = 1;
    game_over_text2.content[8] = ' ';
    game_over_text2.content[9] = ' ';
    game_over_text2.content[10] = ' ';
    int_to_string(snake.size, game_over_text2.content + 8);
}

static void handle_logic()
{
    move_snake();

    if(snake.body[0].position.x >= GAME_WIDTH || snake.body[0].position.y >= GAME_HEIGHT ||
        snake.body[0].position.x < 0 || snake.body[0].position.y < 0) set_game_over();

    if(snake.body[0].position.x == point.x && snake.body[0].position.y == point.y)
    {
        increase_snake();
        while(1)
        {
            point = make_vector2d(rand(GAME_WIDTH - 1), rand(GAME_HEIGHT - 1));
            uint8_t generate_new = 0;
            for(int i = 0; i < snake.size; i++)
            {
                if(snake.body[i].position.x == point.x && snake.body[i].position.y == point.y) 
                {
                    generate_new = 1;
                    break;
                }
            }
            if(!generate_new) break;
        }
    }

    if(!game_over)update_game();
}

static void move_snake()
{
    tail_direction = snake.body[snake.size - 1].direction;
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
    vector2d_t head_position = snake.body[0].position;
    for(int i = 1; i < snake.size; i++)
    {
        if(snake.body[i].position.x == head_position.x && snake.body[i].position.y == head_position.y)
        {
            set_game_over();
            break;
        }
        struct snake_body temp = last_body;
        last_body = snake.body[i];
        snake.body[i] = temp;
    }
}

static void increase_snake()
{
    vector2d_t position = snake.body[snake.size - 1].position;
    //enum DIRECTION direction = snake.body[snake.size - 1].direction;

    switch(tail_direction)
    {
        case UP:
            position.y += 1;
            break;
        case DOWN:
            position.y -= 1;
            break;
        case LEFT:
            position.x += 1;
            break;
        case RIGHT:
            position.x -= 1;
            break;
    }

    struct snake_body snake_part = {position, tail_direction};
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
                    field.color = 0xBF;
                    break;
                case FIELD_SNAKE:
                    field.color = 2;
                    break;
                case FIELD_POINT:
                    field.color = 4;
                    break;
            }
            draw_rectangle(field);
            field.position.x += field_width + 1;
        }
        field.position.y += field_height + 1;
    }

    if(game_over)
    {
        draw_text(game_over_text1);
        draw_text(game_over_text2);
        draw_text(game_over_text3);
    }

    display_buffer();
}