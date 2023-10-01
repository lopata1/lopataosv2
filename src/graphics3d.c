#include <os/graphics3d.h>
#include <os/graphics.h>

vector3d_t make_vector3d(float x, float y, float z)
{
    vector3d_t v = {x, y, z};
    return v;
}

matrix3d_t make_matrix3d(vector3d_t row0, vector3d_t row1, vector3d_t row2)
{
    matrix3d_t m = {row0, row1, row2};
    return m;
}

vector3d_t apply_matrix3d(matrix3d_t matrix, vector3d_t vector)
{
    vector3d_t result = {0, 0, 0};
    
    result.x += matrix.row0.x * vector.x;
    result.x += matrix.row0.y * vector.y;
    result.x += matrix.row0.z * vector.z;

    result.y += matrix.row1.x * vector.x;
    result.y += matrix.row1.y * vector.y;
    result.y += matrix.row1.z * vector.z;

    result.z += matrix.row2.x * vector.x;
    result.z += matrix.row2.y * vector.y;
    result.z += matrix.row2.z * vector.z;

    return result;
}