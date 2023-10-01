#ifndef _GRAPHICS3D_H_
#define _GRAPHICS3D_H_

typedef struct {
    float x;
    float y;
    float z;
} vector3d_t;

typedef struct {
    vector3d_t row0;
    vector3d_t row1;
    vector3d_t row2;
} matrix3d_t;

vector3d_t make_vector3d(float x, float y, float z);
matrix3d_t make_matrix3d(vector3d_t row0, vector3d_t row1, vector3d_t row2);
vector3d_t apply_matrix3d(matrix3d_t matrix, vector3d_t vector);

#endif