#include <os/graphics3d.h>
#include <os/graphics.h>
#include <os/math.h>

// utils

float z_buffer[SCREEN_HEIGHT][SCREEN_WIDTH];

static double triangle_area(vector2d_t p1, vector2d_t p2, vector2d_t p3)
{
   return absd((p1.x*(p2.y-p3.y) + p2.x*(p3.y-p1.y)+ p3.x*(p1.y-p2.y))/2.0);
}
  
/* A function to check whether point P(x, y) lies inside the triangle formed 
   by A(x1, y1), B(x2, y2) and C(x3, y3) */
static uint8_t is_inside_triangle(vector2d_t p1, vector2d_t p2, vector2d_t p3, vector2d_t p)
{   
   /* Calculate area of triangle ABC */
   double A = triangle_area(p1, p2, p3);
  
   /* Calculate area of triangle PBC */ 
   double A1 = triangle_area(p, p2, p3);
  
   /* Calculate area of triangle PAC */ 
   double A2 = triangle_area(p1, p, p3);
  
   /* Calculate area of triangle PAB */  
   double A3 = triangle_area(p1, p2, p);
    
   /* Check if sum of A1, A2 and A3 is same as A */
   return (A == A1 + A2 + A3);
}

void draw_triangle(vector2d_t p1, vector2d_t p2, vector2d_t p3, float z, uint8_t color)
{
    int start_x = min(min(p1.x, p2.x), p3.x);
    int start_y = min(min(p1.y, p2.y), p3.y);

    int end_x = max(max(p1.x, p2.x), p3.x);
    int end_y = max(max(p1.y, p2.y), p3.y);

    if(start_x < 0) start_x = 0;
    if(start_y < 0) start_y = 0;
    if(end_x > SCREEN_WIDTH) end_x = SCREEN_WIDTH;
    if(end_y > SCREEN_HEIGHT) end_y = SCREEN_HEIGHT;

    for(int i = start_y; i < end_y; i++)
    {
        for(int j = start_x; j < end_x; j++)
        {
            //if(i > SCREEN_HEIGHT*3 || j > SCREEN_WIDTH*3 || i < -SCREEN_HEIGHT || j < -SCREEN_WIDTH) break;
            //if(i < 0 || j < 0 || i >= SCREEN_HEIGHT || j >= SCREEN_WIDTH) continue;
            if(is_inside_triangle(p1, p2, p3, make_vector2d(j, i)) && z < z_buffer[i][j]) {
                graphics_buffer[i][j] = color;
                z_buffer[i][j] = z;
            }
        }
    }
}

void draw_triangle2(vector2d_t p1, vector2d_t p2, vector2d_t p3, float z, uint8_t color)
{
    if(p3.y < p2.y)
    {
        vector2d_t temp = p2;
        p2 = p3;
        p3 = temp;
    }
    if(p2.y < p1.y)
    {
        vector2d_t temp = p1;
        p1 = p2;
        p2 = temp;
    }
    if(p3.y < p2.y)
    {
        vector2d_t temp = p2;
        p2 = p3;
        p3 = temp;
    }

    int dx = p2.x - p1.x;
    int dy = p2.y - p1.y;

    float xi = (float) dx / dy;




    // yi = 1

    /*int steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);

    float xi = (float) dx / steps;
    float yi = (float) dy / steps;

    float x = p1.x, y = p1.y;
    for(int i = 0; i < steps; i++)
    {
        x += xi;
        y += yi;
        if((int)x <= 0 || (int)y <= 0 || (int)x >= SCREEN_WIDTH || (int)y >= SCREEN_HEIGHT) break;
        draw_pixel(make_vector2d((int)x, (int)y), color);
    }*/
}

vector3d_t make_vector3d(float x, float y, float z)
{
    vector3d_t v = {x, y, z};
    return v;
}

vector4d_t make_vector4d(float x, float y, float z, float w)
{
    vector4d_t v = {x, y, z, w};
    return v;
}

matrix3d_t make_matrix3d(vector3d_t row0, vector3d_t row1, vector3d_t row2)
{
    matrix3d_t m = {row0, row1, row2};
    return m;
}

matrix4d_t make_matrix4d(vector4d_t row0, vector4d_t row1, vector4d_t row2, vector4d_t row3)
{
    matrix4d_t m = {row0, row1, row2, row3};
    return m;
}

vector3d_t apply_matrix3d(matrix3d_t matrix, vector3d_t vector)
{
    vector3d_t result = {0, 0, 0};
    
    result.x += matrix.rows[0].x * vector.x;
    result.x += matrix.rows[1].x * vector.y;
    result.x += matrix.rows[2].x * vector.z;

    result.y += matrix.rows[0].y * vector.x;
    result.y += matrix.rows[1].y * vector.y;
    result.y += matrix.rows[2].y * vector.z;

    result.z += matrix.rows[0].z * vector.x;
    result.z += matrix.rows[1].z * vector.y;
    result.z += matrix.rows[2].z * vector.z;

    return result;
}

vector4d_t apply_matrix4d(matrix4d_t matrix, vector4d_t vector)
{
    vector4d_t result = {0, 0, 0, 0};
    
    result.x += matrix.rows[0].x * vector.x;
    result.x += matrix.rows[1].x * vector.y;
    result.x += matrix.rows[2].x * vector.z;
    result.x += matrix.rows[3].x * vector.w;

    result.y += matrix.rows[0].y * vector.x;
    result.y += matrix.rows[1].y * vector.y;
    result.y += matrix.rows[2].y * vector.z;
    result.y += matrix.rows[3].y * vector.w;

    result.z += matrix.rows[0].z * vector.x;
    result.z += matrix.rows[1].z * vector.y;
    result.z += matrix.rows[2].z * vector.z;
    result.z += matrix.rows[3].z * vector.w;

    result.w += matrix.rows[0].w * vector.x;
    result.w += matrix.rows[1].w * vector.y;
    result.w += matrix.rows[2].w * vector.z;
    result.w += matrix.rows[3].w * vector.w;

    return result;
}


triangle3d_t make_triangle3d(vector3d_t p1, vector3d_t p2, vector3d_t p3)
{
    triangle3d_t t = {p1, p2, p3};
    return t;
}

vector3d_t add_vector3d(vector3d_t v1, vector3d_t v2)
{
    vector3d_t res = {v1.x+v2.x, v1.y+v2.y, v1.z+v2.z};
    return res;
}

vector3d_t sub_vector3d(vector3d_t v1, vector3d_t v2)
{
    vector3d_t res = {v1.x-v2.x, v1.y-v2.y, v1.z-v2.z};
    return res;
}

vector3d_t cross_vector3d(vector3d_t v1, vector3d_t v2)
{
    vector3d_t res;
    res.x = v1.y * v2.z - v1.z * v2.y;
    res.y = v1.z * v2.x - v1.x * v2.z;
    res.z = v1.x * v2.y - v1.y * v2.x;
    return res;
}

vector3d_t unit_vector3d(vector3d_t v)
{
    vector3d_t res;
    float l = sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
    res.x = v.x / l; 
    res.y = v.y / l; 
    res.z = v.z / l;
    return res;
}

triangle3d_t transform_triangle(triangle3d_t t, matrix3d_t m)
{
    triangle3d_t res;
    res.points[0] = apply_matrix3d(m, t.points[0]);
    res.points[1] = apply_matrix3d(m, t.points[1]);
    res.points[2] = apply_matrix3d(m, t.points[2]);
    return res;
}

float dot_vector3d(vector3d_t v1, vector3d_t v2)
{
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

vector3d_t rotate_vector3d_x(vector3d_t v, float rads)
{
    matrix3d_t rotation_x = make_matrix3d(make_vector3d(1, 0, 0),
                                          make_vector3d(0, cos(rads), -sin(rads)),
                                          make_vector3d(0, sin(rads), cos(rads)));
    vector3d_t res = apply_matrix3d(rotation_x, v);
    return res;
}

mesh_t move_mesh(mesh_t mesh, vector3d_t move)
{
    mesh_t res = mesh;
    for(int i = 0; i < mesh.size; i++)
    {
        res.triangles[i].points[0] = add_vector3d(mesh.triangles[i].points[0], move);
        res.triangles[i].points[1] = add_vector3d(mesh.triangles[i].points[1], move);
        res.triangles[i].points[2] = add_vector3d(mesh.triangles[i].points[2], move);
    }
    return res;
}

void draw_linef(vector2df_t point1, vector2df_t point2, uint8_t color)
{
    int dx = point2.x - point1.x;
    int dy = point2.y - point1.y;

    int steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);

    float xi = (float) dx / steps;
    float yi = (float) dy / steps;

    float x = point1.x, y = point1.y;
    for(int i = 0; i < steps; i++)
    {
        if(i > SCREEN_WIDTH) return;
        x += xi;
        y += yi;
        if(x < 0 || y < 0) continue;
        if(x >= SCREEN_WIDTH || y >= SCREEN_HEIGHT) continue;
        draw_pixel(make_vector2d((int)x, (int)y), color);
    }
}




// logic

static const float aspect_ratio = (float) SCREEN_HEIGHT/ (float) SCREEN_WIDTH;
static float fov;
static const float z_far = 1000;
static const float z_near = 0.1;

graphics3d_t graphics3d;

void init_3d()
{
    graphics3d.camera = make_vector3d(0, 0, 0);
    graphics3d.camera_rotation = 0;
    graphics3d.number_of_objects = 0;
    fov = cos((pi/2)/2)/sin((pi/2)/2);
}

static triangle3d_t project_triangle(triangle3d_t t);

void draw_3d()
{   
    for(int i = 0; i < graphics3d.number_of_objects; i++)
    {
        for(int j = 0; j < graphics3d.objects[i].mesh.size; j++)
        {

            matrix3d_t rotation_x = make_matrix3d(make_vector3d(1, 0, 0),
                                                 make_vector3d(0, cos(graphics3d.objects[i].rotation.x), -sin(graphics3d.objects[i].rotation.x)),
                                                 make_vector3d(0, sin(graphics3d.objects[i].rotation.x), cos(graphics3d.objects[i].rotation.x)));

            matrix3d_t rotation_y = make_matrix3d(make_vector3d(cos(graphics3d.objects[i].rotation.y), 0, sin(graphics3d.objects[i].rotation.y)),
                                                        make_vector3d(0, 1, 0),
                                                        make_vector3d(-sin(graphics3d.objects[i].rotation.y), 0, cos(graphics3d.objects[i].rotation.y)));

            matrix3d_t rotation_z = make_matrix3d(make_vector3d(cos(graphics3d.objects[i].rotation.z), -sin(graphics3d.objects[i].rotation.z), 0),
                                                        make_vector3d(sin(graphics3d.objects[i].rotation.z), cos(graphics3d.objects[i].rotation.z), 0),
                                                        make_vector3d(0, 0, 1));


            triangle3d_t distanced = graphics3d.objects[i].mesh.triangles[j];
            distanced = transform_triangle(distanced, rotation_x);
            distanced = transform_triangle(distanced, rotation_y);
            distanced = transform_triangle(distanced, rotation_z);
            for(int k = 0; k < 3; k++)
            {
                distanced.points[k].x *= graphics3d.objects[i].scale.x;
                distanced.points[k].y *= graphics3d.objects[i].scale.y;
                distanced.points[k].z *= graphics3d.objects[i].scale.z;
            }
            

            distanced.points[0] = add_vector3d(distanced.points[0], graphics3d.camera);
            distanced.points[1] = add_vector3d(distanced.points[1], graphics3d.camera);
            distanced.points[2] = add_vector3d(distanced.points[2], graphics3d.camera);

            

            distanced.points[0] = add_vector3d(distanced.points[0], graphics3d.objects[i].position);
            distanced.points[1] = add_vector3d(distanced.points[1], graphics3d.objects[i].position);
            distanced.points[2] = add_vector3d(distanced.points[2], graphics3d.objects[i].position);

            
        
            



            matrix3d_t camera_rotation_y = make_matrix3d(make_vector3d(cos(graphics3d.camera_rotation), 0, sin(graphics3d.camera_rotation)),
                                                 make_vector3d(0, 1, 0),
                                                 make_vector3d(-sin(graphics3d.camera_rotation), 0, cos(graphics3d.camera_rotation)));

            distanced = transform_triangle(distanced, camera_rotation_y);

            if(distanced.points[0].z < 0 || distanced.points[1].z < 0 || distanced.points[2].z < 0) continue;


            triangle3d_t rotated = distanced;

            

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

            //draw_linef(make_vector2df(p1.x, p1.y), make_vector2df(p2.x, p2.y), 15);
            //draw_linef(make_vector2df(p2.x, p2.y), make_vector2df(p3.x, p3.y), 15);
            //draw_linef(make_vector2df(p1.x, p1.y), make_vector2df(p3.x, p3.y), 15);

            vector3d_t light = make_vector3d(0, -1, -1);

            float illumination = dot_vector3d(normal, light);

            uint8_t color = 0x10 + (illumination < 0 ? 0 :(0xA * illumination)) + 1;

            if(max(p3.x, max(p1.x, p2.x)) < 0) continue;
            if(min(p3.y, min(p1.y, p2.y)) >= SCREEN_HEIGHT) continue;

            //draw_line(make_vector2d(p1.x, p1.y), make_vector2d(p2.x, p2.y), 15);
            //draw_line(make_vector2d(p2.x, p2.y), make_vector2d(p3.x, p3.y), 15);
            //draw_line(make_vector2d(p1.x, p1.y), make_vector2d(p3.x, p3.y), 15);
            draw_triangle(make_vector2d(p1.x, p1.y),  make_vector2d(p2.x, p2.y), make_vector2d(p3.x, p3.y), (p1.z+p2.z+p3.z)/3, color);
        }
    }

    
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