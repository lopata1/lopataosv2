#include <os/graphics.h>
#include <os/graphics3d.h>
#include <os/ints/interrupts.h>
#include <os/math.h>


static float angle = 0;


static const float aspect_ratio = (float) SCREEN_HEIGHT/ (float) SCREEN_WIDTH;
static float fov;
static const float z_far = 1000;
static const float z_near = 0.1;

vector3d_t camera = {0, 0, 0};


static void draw();
static void cube_drawing();
static void new_cube_drawing();
static vector3d_t project(vector3d_t vector);

void test3d_start()
{
    fov = cos((pi/2)/2)/sin((pi/2)/2);
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

    cube_drawing();
    //new_cube_drawing();

    display_buffer();
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

static void new_cube_drawing()
{
    mesh_t cube;
    cube.size = 12;

    cube.triangles[0] = make_triangle3d(make_vector3d(0, 0, 0), make_vector3d(0, 1, 0), make_vector3d(1, 1, 0));
    cube.triangles[1] = make_triangle3d(make_vector3d(0, 0, 0), make_vector3d(1, 1, 0), make_vector3d(1, 0, 0));

    cube.triangles[2] = make_triangle3d(make_vector3d(1, 0, 0), make_vector3d(1, 1, 0), make_vector3d(1, 1, 1));
    cube.triangles[3] = make_triangle3d(make_vector3d(1, 0, 0), make_vector3d(1, 1, 1), make_vector3d(1, 0, 1));

    cube.triangles[4] = make_triangle3d(make_vector3d(1, 0, 1), make_vector3d(1, 1, 1), make_vector3d(0, 1, 1));
    cube.triangles[5] = make_triangle3d(make_vector3d(1, 0, 1), make_vector3d(0, 1, 1), make_vector3d(0, 0, 1));

    cube.triangles[6] = make_triangle3d(make_vector3d(0, 0, 1), make_vector3d(0, 1, 1), make_vector3d(0, 1, 0));
    cube.triangles[7] = make_triangle3d(make_vector3d(0, 0, 1), make_vector3d(0, 1, 0), make_vector3d(0, 0, 0));

    cube.triangles[8] = make_triangle3d(make_vector3d(0, 1, 0), make_vector3d(0, 1, 1), make_vector3d(1, 1, 1));
    cube.triangles[9] = make_triangle3d(make_vector3d(0, 1, 0), make_vector3d(1, 1, 1), make_vector3d(1, 1, 0));

    cube.triangles[10] = make_triangle3d(make_vector3d(1, 0, 1), make_vector3d(0, 0, 1), make_vector3d(0, 0, 0));
    cube.triangles[11] = make_triangle3d(make_vector3d(1, 0, 1), make_vector3d(0, 0, 0), make_vector3d(1, 0, 0));

    matrix3d_t rotation_x = make_matrix3d(make_vector3d(1, 0, 0),
                                                 make_vector3d(0, cos(angle), -sin(angle)),
                                                 make_vector3d(0, sin(angle), cos(angle)));
    matrix3d_t rotation_y = make_matrix3d(make_vector3d(cos(angle), 0, sin(angle)),
                                                 make_vector3d(0, 1, 0),
                                                 make_vector3d(-sin(angle), 0, cos(angle)));

    matrix3d_t rotation_z = make_matrix3d(make_vector3d(cos(angle), -sin(angle), 0),
                                                 make_vector3d(sin(angle), cos(angle), 0),
                                                 make_vector3d(0, 0, 1));

    for(int i = 0; i < cube.size; i++)
    {
        triangle3d_t rotated = cube.triangles[i];

        rotated = transform_triangle(rotated, rotation_x);
        rotated = transform_triangle(rotated, rotation_y);
        rotated = transform_triangle(rotated, rotation_z);

        rotated.points[0].z += 3;
        rotated.points[1].z += 3;
        rotated.points[2].z += 3;

        vector3d_t normal, line1, line2;
        line1 = sub_vector3d(rotated.points[1], rotated.points[0]);
        line2 = sub_vector3d(rotated.points[2], rotated.points[0]);

        normal = cross_vector3d(line1, line2);

        normal = unit_vector3d(normal);

        if(dot_vector3d(normal, sub_vector3d(rotated.points[0], camera)) > 0) continue;  //normal.x * (rotated.points[0].x - camera.x) + normal.y * (rotated.points[0].y - camera.y) + normal.z * (rotated.points[0].z - camera.z) > 0) continue;


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

        draw_line(make_vector2d(p1.x, p1.y), make_vector2d(p2.x, p2.y), 15);
        draw_line(make_vector2d(p2.x, p2.y), make_vector2d(p3.x, p3.y), 15);
        draw_line(make_vector2d(p1.x, p1.y), make_vector2d(p3.x, p3.y), 15);
    }

    
}


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