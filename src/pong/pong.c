#include <os/output.h>
#include <os/utils.h>
#include <os/graphics.h>
#include <os/globals.h>
#include <os/ints/interrupts.h>
#include <os/math.h>

static enum MOVEMENT {IDLE, UP, DOWN, LEFT, RIGHT};

static struct {
    vector2df_t position;
    enum MOVEMENT movement;
    uint8_t score;
} player_left, player_right;

static struct {
    vector2df_t position;
    uint16_t speed;
    enum MOVEMENT movement_x;
    enum MOVEMENT movement_y;
} ball;


static rectangle_t wall_up;
static rectangle_t wall_down;
static rectangle_t player_left_obj;
static rectangle_t player_right_obj;
static rectangle_t ball_obj;

static text_t pong_text;
static text_t score_text;
static text_t left_player_won_text;
static text_t right_player_won_text;
static text_t play_again_text1;
static text_t play_again_text2;


static float deltatime = 0;

static uint32_t player_speed = 150;

static uint8_t game_over = 0;
static uint8_t winner = 0;

static const vector2d_t player_size = {6, 60};

static uint8_t score[2] = {0, 0};

static uint8_t running = 0;

static void init_pong();
static void draw();
static void handle_logic();
static void handle_input();
static void collision_check();
static void move_players();
static void start_new_game();

static uint8_t is_colliding(rectangle_t obj1, rectangle_t obj2);
static enum MOVEMENT get_collision_movement(rectangle_t obj1, rectangle_t obj2);


void pong_start()
{
    init_pong();

    running = 1;

    while(running)
    {
        timer_ms_passed = 0;
        handle_input();
        if(!game_over) handle_logic();
        draw();
        deltatime = (float) timer_ms_passed / 1000;
    }
}

static void init_pong()
{
    start_new_game();

    score[0] = 0;
    score[1] = 0;

    wall_up = make_rectangle(make_vector2d(0, 0), make_vector2d(SCREEN_WIDTH, 10), 15);

    wall_down = make_rectangle(make_vector2d(0, SCREEN_HEIGHT - 10), make_vector2d(SCREEN_WIDTH, 10), 15);

    player_left_obj = make_rectangle(make_vector2d(20, player_left.position.y), player_size, 15);

    player_right_obj = make_rectangle(make_vector2d(SCREEN_WIDTH - 20 - 6, player_right.position.y), player_size, 15);

    ball_obj = make_rectangle(make_vector2d(ball.position.x, ball.position.y), make_vector2d(4, 4), 15);

    pong_text = make_text("PING PONG", make_vector2d(center_text_x(9), 30), 15);
    score_text = make_text("  0 : 0  ", make_vector2d(center_text_x(9), 50), 15);
    left_player_won_text = make_text("LIJEVI IGRAC POBJEDIO!", make_vector2d(center_text_x(22), 80), 15);
    right_player_won_text = make_text("DESNI IGRAC POBJEDIO!", make_vector2d(center_text_x(21), 80), 15);
    play_again_text1 = make_text("PRITISNITE SPACE DA", make_vector2d(center_text_x(19), 100), 15);
    play_again_text2 = make_text("IGRATE PONOVO", make_vector2d(center_text_x(13), 120), 15);
}

static void start_new_game()
{
    player_left.position = make_vector2df(0, center_y(player_size.y));
    player_right.position = make_vector2df(0, center_y(player_size.y));
    player_left.score = player_right.score = 0;
    player_left.movement = player_right.movement = IDLE;

    ball.movement_x = (winner == 0) ? RIGHT : LEFT;
    ball.movement_y = UP;
    ball.speed = 100;
    ball.position = make_vector2df(SCREEN_WIDTH/2, SCREEN_HEIGHT/2);

    game_over = 0;
}

static void update_score_text()
{
    char* text_start = score_text.content+2;

    for(int i = 0; i < 3; i++)
    {
        text_start[i] = (score[0] / pow(10, 2-i)) % 10 + '0';
    }

    //score_text.content[2] = score[0] + '0';
    score_text.content[6] = score[1] + '0';
}

static void draw()
{
    clear_buffer(0);
    background_color = 0;
    draw_rectangle(wall_up);
    draw_rectangle(wall_down);
    draw_rectangle(player_left_obj);
    draw_rectangle(player_right_obj);
    draw_rectangle(ball_obj);
    draw_text(pong_text);
    draw_text(score_text);
    if(game_over)
    {
        draw_text(winner ? right_player_won_text : left_player_won_text);
        draw_text(play_again_text1);
        draw_text(play_again_text2);
    }
    display_buffer();
}

static void handle_logic()
{
    move_players();
    
    collision_check();

    ball.position.x += (ball.movement_x == RIGHT ? ball.speed : -ball.speed) * deltatime;
    ball.position.y += (ball.movement_y == DOWN ? ball.speed : -ball.speed) * deltatime;

    if(ball.position.x < 0)
    {
        game_over = 1;
        score[1]++;
        winner = 1;
        update_score_text();
    }
    else if(ball.position.x > SCREEN_WIDTH - ball_obj.size.x)
    {
        game_over = 1;
        score[0]++;
        winner = 0;
        update_score_text();
    }

    ball_obj.position.x = ball.position.x;
    ball_obj.position.y = ball.position.y;
    player_left_obj.position.y = player_left.position.y;
    player_right_obj.position.y = player_right.position.y;
}

static void move_players()
{
    if(player_left.movement == UP)
    { 
        player_left.position.y -= player_speed * deltatime;
    }
    else if (player_left.movement == DOWN)
    {
        player_left.position.y += player_speed * deltatime;
    }

    if(player_right.movement == UP)
    {
        player_right.position.y -= player_speed * deltatime;
    }
    else if (player_right.movement == DOWN)
    {
        player_right.position.y += player_speed * deltatime;
    }

    if(player_left.position.y < wall_up.size.y) player_left.position.y = wall_up.size.y;
    if(player_left.position.y > SCREEN_HEIGHT - wall_down.size.y - player_left_obj.size.y) player_left.position.y = SCREEN_HEIGHT - wall_down.size.y - player_left_obj.size.y;

    if(player_right.position.y < wall_up.size.y) player_right.position.y = wall_up.size.y;
    if(player_right.position.y > SCREEN_HEIGHT - wall_down.size.y - player_right_obj.size.y) player_right.position.y = SCREEN_HEIGHT - wall_down.size.y - player_right_obj.size.y;
}

static void collision_check()
{
    enum MOVEMENT collision_direction;
    uint8_t ball_wall_collision = 0, ball_player_collision = 0;

    if(is_colliding(ball_obj, wall_up))
    {
        collision_direction = get_collision_movement(ball_obj, wall_up);
        ball_wall_collision = 1;
    }
    else if(is_colliding(ball_obj, wall_down))
    {
        collision_direction = get_collision_movement(ball_obj, wall_down);
        ball_wall_collision = 1;
    }

    if(is_colliding(ball_obj, player_left_obj))
    {
        collision_direction = get_collision_movement(ball_obj, player_left_obj);
        ball_player_collision = 1;
    }
    else if(is_colliding(ball_obj, player_right_obj))
    {
        collision_direction = get_collision_movement(ball_obj, player_right_obj);
        ball_player_collision = 1;
    }

    if(!ball_wall_collision && !ball_player_collision) return;

    if(ball_player_collision) 
    {
        ball.speed += 20;
        player_speed += 20;
    }
    switch(collision_direction)
    {
        case UP:
            ball.movement_y = DOWN;
            ball.position.y += 1;
            break;
        case DOWN:
            ball.movement_y = UP;
            ball.position.y -= 1;
            break;
         case LEFT:
            ball.movement_x = RIGHT;
            ball.position.x += 1;
            break;
        case RIGHT:
            ball.movement_x = LEFT;
            ball.position.x -= 1;
            break;
    }
}

static void handle_input()
{
   if(input_pressed)
    {
        input_pressed = 0;
        if(last_key_pressed == 'W')
        {
            player_left.movement = UP;
        }
        else if (last_key_pressed == 'S')
        {
            player_left.movement = DOWN;
        }
        else if (last_key_pressed == 'I')
        {
            player_right.movement = UP;
        }
        else if (last_key_pressed == 'K')
        {
            player_right.movement = DOWN;
        }
        else if (last_key_pressed == ' ' && game_over)
        {
            start_new_game();
        }
        else if(last_key_pressed == 'Q')
        {
            running = 0;
        }
    }

    if(!input_released) return;

    input_released = 0;

    if((last_key_released == 'W' && player_left.movement == UP) || (last_key_released == 'S' && player_left.movement == DOWN))
    {
        player_left.movement = IDLE;
    }
    else if ((last_key_released == 'I' && player_right.movement == UP) || (last_key_released == 'K' && player_right.movement == DOWN))
    {
        player_right.movement = IDLE;
    }
    
    return;
}


static uint8_t is_colliding(rectangle_t obj1, rectangle_t obj2)
{
    return (obj1.position.x + obj1.size.x >= obj2.position.x &&
      obj1.position.x <= obj2.position.x + obj2.size.x &&
      obj1.position.y + obj1.size.y >= obj2.position.y &&
      obj1.position.y <= obj2.position.y + obj2.size.y);
}

static enum MOVEMENT get_collision_movement(rectangle_t obj1, rectangle_t obj2)
{
    vector2d_t topleft1 = obj1.position;
    vector2d_t topright1 = make_vector2d(obj1.position.x + obj1.size.x, obj1.position.y);
    vector2d_t botleft1 = make_vector2d(obj1.position.x, obj1.position.y + obj1.size.y);

    if(topright1.x <= obj2.position.x) return RIGHT;
    if(botleft1.y - 2 <= obj2.position.y) return DOWN;
    if(topleft1.y >= obj2.position.y + obj2.size.y) return UP;
    if(topleft1.x >= obj2.position.x + obj2.size.x) return LEFT;

    return IDLE;

}