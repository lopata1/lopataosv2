#ifndef _GRAPHICS3D_H_
#define _GRAPHICS3D_H_

#include <stdint.h>
#include <os/graphics.h>

typedef struct {
    float x;
    float y;
    float z;
} vector3d_t;

typedef struct {
    float x;
    float y;
    float z;
    float w;
} vector4d_t;

typedef struct {
    vector3d_t rows[3];
} matrix3d_t;

typedef struct {
    vector4d_t rows[4];
} matrix4d_t;

typedef struct {
    vector3d_t points[3];
} triangle3d_t;

typedef struct {
    triangle3d_t triangles[20];
    uint16_t size;
} mesh_t;

typedef struct {
    mesh_t mesh;
    vector3d_t position;
    vector3d_t rotation;
    vector3d_t scale;
} object_t;

typedef struct {
    object_t objects[256];
    vector3d_t camera;
    float camera_rotation;
    int number_of_objects;
} graphics3d_t;

vector3d_t make_vector3d(float x, float y, float z);
vector4d_t make_vector4d(float x, float y, float z, float w);;
matrix3d_t make_matrix3d(vector3d_t row0, vector3d_t row1, vector3d_t row2);
matrix4d_t make_matrix4d(vector4d_t row0, vector4d_t row1, vector4d_t row2, vector4d_t row3);
vector3d_t apply_matrix3d(matrix3d_t matrix, vector3d_t vector);
vector4d_t apply_matrix4d(matrix4d_t matrix, vector4d_t vector);
triangle3d_t make_triangle3d(vector3d_t p1, vector3d_t p2, vector3d_t p3);

vector3d_t add_vector3d(vector3d_t v1, vector3d_t v2);
vector3d_t sub_vector3d(vector3d_t v1, vector3d_t v2);
vector3d_t cross_vector3d(vector3d_t v1, vector3d_t v2);
triangle3d_t transform_triangle(triangle3d_t t, matrix3d_t m);
vector3d_t unit_vector3d(vector3d_t v);
float dot_vector3d(vector3d_t v1, vector3d_t v2);
mesh_t move_mesh(mesh_t mesh, vector3d_t move);

void draw_linef(vector2df_t point1, vector2df_t point2, uint8_t color);
void draw_triangle(vector2d_t p1, vector2d_t p2, vector2d_t p3, float z, uint8_t color);

void init_3d();
void draw_3d();

extern matrix3d_t rotation_matrix_x;
extern matrix3d_t rotation_matrix_y;
extern matrix3d_t rotation_matrix_z;
extern graphics3d_t graphics3d;
extern float z_buffer[SCREEN_HEIGHT][SCREEN_WIDTH];

#endif