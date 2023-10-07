#include <os/graphics.h>
#include <os/graphics3d.h>
#include <os/ints/interrupts.h>
#include <os/math.h>
#include <os/memory.h>
#include <os/mesh3d.h>


static float angle = 0;


static const float aspect_ratio = (float) SCREEN_HEIGHT/ (float) SCREEN_WIDTH;
static float fov;
static const float z_far = 1000;
static const float z_near = 0.1;

vector3d_t camera = {0, 0, 0};

static float camera_rotation = 0;

static enum MOVEMENT {IDLE, UP, DOWN, LEFT, RIGHT};

static enum MOVEMENT move_x;
static enum MOVEMENT move_y;

//mesh_t cube;

object_t objects[10];

static text_t debug_text;
static text_t fps_text;

static int number_of_objects = 9;

static float deltatime = 0;
static uint16_t fps = 0;

static void draw();
static void cube_drawing();
static void new_cube_drawing();
static vector3d_t project(vector3d_t vector);
static void handle_input();
static void handle_logic();
static void init();

void test3d_start()
{
    init_3d();
    init();
    fov = cos((pi/2)/2)/sin((pi/2)/2);
    while(1)
    {
        timer_ms_passed = 0;
        if(angle > 2*pi) angle = 0;
        angle += 0.02;
        draw();
        handle_input();
        handle_logic();

        deltatime = (float) timer_ms_passed / 1000;
        fps = 1 / deltatime;
    }
}

static void handle_logic()
{
    memset(debug_text.content, ' ', 12);
    int_to_string(graphics3d.camera.x, debug_text.content);
    int_to_string(graphics3d.camera.y, debug_text.content+4);
    int_to_string(graphics3d.camera.z, debug_text.content+8);

    memset(fps_text.content+5, ' ', 4);
    int_to_string(fps, fps_text.content+5);
    if(graphics3d.camera_rotation >= 2*pi || graphics3d.camera_rotation < -pi*2) graphics3d.camera_rotation = 0;
    if(move_y == UP)
    {
        graphics3d.camera = add_vector3d(graphics3d.camera, make_vector3d(-8 * sin(graphics3d.camera_rotation) * deltatime, 0, -8*deltatime * cos(graphics3d.camera_rotation)));
    }
    else if(move_y == DOWN)
    {
        graphics3d.camera = add_vector3d(graphics3d.camera, make_vector3d(8 * sin(graphics3d.camera_rotation) * deltatime, 0, 8*deltatime * cos(graphics3d.camera_rotation)));
    }
    if(move_x == LEFT)
    {
        graphics3d.camera = add_vector3d(graphics3d.camera, make_vector3d(8*deltatime * cos(graphics3d.camera_rotation), 0, -8*deltatime * sin(graphics3d.camera_rotation)));
    }
    else if(move_x == RIGHT)
    {
        graphics3d.camera = add_vector3d(graphics3d.camera, make_vector3d(-8*deltatime * cos(graphics3d.camera_rotation), 0, 8*deltatime * sin(graphics3d.camera_rotation)));
    }

    graphics3d.objects[0].rotation = make_vector3d(angle, angle, angle);
}

static void new_new_cube_drawing();
static void draw()
{
    clear_buffer(0);
    for(int i = 0; i < SCREEN_HEIGHT; i++)
        for(int j = 0; j < SCREEN_WIDTH; j++)
            z_buffer[i][j] = 99999;

    //cube_drawing();
    //new_cube_drawing();
    new_new_cube_drawing();
    draw_text(debug_text);
    draw_text(fps_text);

    display_buffer();
}

static void init()
{
    debug_text = make_text("            ", make_vector2d(10, 10), 15, 1);
    fps_text = make_text("FPS:     ", make_vector2d(10, 30), 15, 1);

    //object_t cube_obj = {cube, make_vector3d(3, 2, 5), make_vector2df(0, 0)};

    //graphics3d.objects[0] = cube_obj;
    //graphics3d.number_of_objects = 4;

    graphics3d.number_of_objects = 3;

    for(int i = 0; i < 1; i++)
    {
        object_t cube_obj = {cube, make_vector3d(i, 0, 0), make_vector3d(pi/6, pi/6, 0), make_vector3d(2, 2, 2)};
        graphics3d.objects[i] = cube_obj;
    }

    object_t cube_obj = {cube, make_vector3d(5, 0, 0), make_vector3d(0, 0, 0), make_vector3d(10, 2, 2)};
    graphics3d.objects[1] = cube_obj;

    object_t cube_obj2 = {cube, make_vector3d(0, 2, 0), make_vector3d(50, 0, 50), make_vector3d(100, 1, 100)};
    //graphics3d.objects[2] = cube_obj2;

    graphics3d.camera.y = 2;
}

static vector3d_t project(vector3d_t vector)
{
    matrix4d_t projection_matrix = make_matrix4d(make_vector4d(aspect_ratio*fov, 0, 0, 0),
                                                 make_vector4d(0, fov, 0, 0),
                                                 make_vector4d(0, 0, z_far / (z_far - z_near), 1),
                                                 make_vector4d(0, 0, (-z_far * z_near) / (z_far - z_near), 0));

    vector4d_t scaled_vector = apply_matrix4d(projection_matrix, make_vector4d(vector.x, vector.y, vector.z, 1));
    if(scaled_vector.w != 0)
    {
        scaled_vector.x /= scaled_vector.w;
        scaled_vector.y /= scaled_vector.w;
    }

    vector3d_t projected_vector = make_vector3d(scaled_vector.x, scaled_vector.y, scaled_vector.z);
    return projected_vector;
}

static triangle3d_t project_triangle(triangle3d_t t)
{
    triangle3d_t res = {project(t.points[0]), project(t.points[1]), project(t.points[2])};
    return res;
}


static void new_new_cube_drawing()
{
    draw_3d();
}

/*static void new_cube_drawing()
{    

    matrix3d_t rotation_x = make_matrix3d(make_vector3d(1, 0, 0),
                                                 make_vector3d(0, cos(camera_rotation), -sin(camera_rotation)),
                                                 make_vector3d(0, sin(camera_rotation), cos(camera_rotation)));
    matrix3d_t rotation_y = make_matrix3d(make_vector3d(cos(camera_rotation), 0, sin(camera_rotation)),
                                                 make_vector3d(0, 1, 0),
                                                 make_vector3d(-sin(camera_rotation), 0, cos(camera_rotation)));

    matrix3d_t rotation_z = make_matrix3d(make_vector3d(cos(camera_rotation), -sin(camera_rotation), 0),
                                                 make_vector3d(sin(camera_rotation), cos(camera_rotation), 0),
                                                 make_vector3d(0, 0, 1));
    for(int i = 0; i < number_of_objects; i++)
    {
        for(int j = 0; j < objects[i].mesh.size; j++)
        {

            triangle3d_t distanced = objects[i].mesh.triangles[j];

            // -5 0 -5
            distanced.points[0] = add_vector3d(distanced.points[0], camera);
            distanced.points[1] = add_vector3d(distanced.points[1], camera);
            distanced.points[2] = add_vector3d(distanced.points[2], camera);

            distanced.points[0] = add_vector3d(distanced.points[0], objects[i].position);
            distanced.points[1] = add_vector3d(distanced.points[1], objects[i].position);
            distanced.points[2] = add_vector3d(distanced.points[2], objects[i].position);

            distanced = transform_triangle(distanced, rotation_y);

            if(distanced.points[0].z < 0 || distanced.points[1].z < 0 || distanced.points[2].z < 0) continue;


            triangle3d_t rotated = distanced;

            

            //rotated = transform_triangle(rotated, rotation_x);
            //rotated = transform_triangle(rotated, rotation_y);
            //rotated = transform_triangle(rotated, rotation_z);

            //rotated.points[0].z += 5;
            //rotated.points[1].z += 5;
            //rotated.points[2].z += 5;

            vector3d_t normal, line1, line2;
            line1 = sub_vector3d(rotated.points[1], rotated.points[0]);
            line2 = sub_vector3d(rotated.points[2], rotated.points[0]);

            normal = cross_vector3d(line1, line2);

            normal = unit_vector3d(normal);

            if(dot_vector3d(normal, sub_vector3d(rotated.points[0], make_vector3d(0,0,0))) > 0) continue; //camera)) > 0) continue;  //normal.x * (rotated.points[0].x - camera.x) + normal.y * (rotated.points[0].y - camera.y) + normal.z * (rotated.points[0].z - camera.z) > 0) continue;


            triangle3d_t projected = project_triangle(rotated);

            vector3d_t p1 = projected.points[0];
            vector3d_t p2 = projected.points[1];
            vector3d_t p3 = projected.points[2];

            p1.x += 1; p1.y += 1;
            p2.x += 1; p2.y += 1;
            p3.x += 1; p3.y += 1;

            p1.x *= 0.5 * SCREEN_WIDTH; p1.y *= 0.5 * SCREEN_HEIGHT;
            p2.x *= 0.5 * SCREEN_WIDTH; p2.y *= 0.5 * SCREEN_HEIGHT;
            p3.x *= 0.5 * SCREEN_WIDTH; p3.y *= 0.5 * SCREEN_HEIGHT;

            draw_linef(make_vector2df(p1.x, p1.y), make_vector2df(p2.x, p2.y), 15);
            draw_linef(make_vector2df(p2.x, p2.y), make_vector2df(p3.x, p3.y), 15);
            draw_linef(make_vector2df(p1.x, p1.y), make_vector2df(p3.x, p3.y), 15);
        }
    }

    
}*/


static void cube_drawing()
{
    matrix3d_t projection_matrix = make_matrix3d(make_vector3d(1, 0, 0),
                                                 make_vector3d(0, 1, 0),
                                                 make_vector3d(0, 0, 0));

    matrix3d_t rotation_x = make_matrix3d(make_vector3d(1, 0, 0),
                                                 make_vector3d(0, cos(angle), -sin(angle)),
                                                 make_vector3d(0, sin(angle), cos(angle)));
    
    matrix3d_t rotation_y = make_matrix3d(make_vector3d(cos(angle), 0, sin(angle)),
                                                 make_vector3d(0, 1, 0),
                                                 make_vector3d(-sin(angle), 0, cos(angle)));

    matrix3d_t rotation_z = make_matrix3d(make_vector3d(cos(angle), -sin(angle), 0),
                                                 make_vector3d(sin(angle), cos(angle), 0),
                                                 make_vector3d(0, 0, 1));

    vector3d_t points[] = {make_vector3d(-1, -1, 1),
                           make_vector3d(1, -1, 1),
                           make_vector3d(1, 1, 1),
                           make_vector3d(-1, 1, 1),
                           make_vector3d(-1, -1, -1),
                           make_vector3d(1, -1, -1),
                           make_vector3d(1, 1, -1),
                           make_vector3d(-1, 1, -1)};

    float scale = 40;
    vector2df_t center = {SCREEN_WIDTH/2, SCREEN_HEIGHT/2};

    vector2d_t projected_points[8];

    for(int i = 0; i < 8; i++)
    {
        vector3d_t rotated = apply_matrix3d(rotation_x, points[i]);
        rotated = apply_matrix3d(rotation_y, rotated);
        rotated = apply_matrix3d(rotation_z, rotated);
        vector3d_t projected = apply_matrix3d(projection_matrix, rotated);

        float x = projected.x*scale+center.x;
        float y = projected.y*scale+center.y;

        draw_pixel(make_vector2d(x, y), 15);

        projected_points[i] = make_vector2d(x, y);
    }

    draw_line(projected_points[0], projected_points[1], 15);
    draw_line(projected_points[1], projected_points[2], 15);
    draw_line(projected_points[2], projected_points[3], 15);
    draw_line(projected_points[3], projected_points[0], 15);

    draw_line(projected_points[4], projected_points[5], 15);
    draw_line(projected_points[5], projected_points[6], 15);
    draw_line(projected_points[6], projected_points[7], 15);
    draw_line(projected_points[7], projected_points[4], 15);

    draw_line(projected_points[0], projected_points[4], 15);
    draw_line(projected_points[1], projected_points[5], 15);
    draw_line(projected_points[2], projected_points[6], 15);
    draw_line(projected_points[3], projected_points[7], 15);
}


static void handle_input()
{
   if(input_pressed)
    {
        input_pressed = 0;
        if(last_key_pressed == 'W')
        {
            move_y = UP;
        }
        else if (last_key_pressed == 'S')
        {
            move_y = DOWN;
        }
        else if (last_key_pressed == 'A')
        {
            move_x = LEFT;
        }
        else if (last_key_pressed == 'D')
        {
            move_x = RIGHT;
        }
        else if(last_key_pressed == 'Q')
        {
            graphics3d.camera_rotation-= 0.1;
        }
        else if(last_key_pressed == 'E')
        {
            graphics3d.camera_rotation += 0.1;
        }
    }

    if(!input_released) return;

    input_released = 0;

    if((last_key_released == 'W' && move_y == UP) || (last_key_released == 'S' && move_y == DOWN))
    {
        move_y = IDLE;
    }
    else if ((last_key_released == 'A' && move_x == LEFT) || (last_key_released == 'D' && move_x == RIGHT))
    {
        move_x = IDLE;
    }
    
    return;
}