#include <os/graphics.h>
#include <os/graphics3d.h>
#include <os/ints/interrupts.h>
#include <os/math.h>


static float angle = 0;


static void draw();

void test3d_start()
{
    while(1)
    {
        if(angle > 2*pi) angle = 0;
        angle += 0.02;
        
        draw();
        sleep(17);
    }
}


static void draw()
{
    clear_buffer(0);

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

    display_buffer();
}