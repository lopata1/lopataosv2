#include <os/graphics3d.h>
#include <os/graphics.h>
#include <os/math.h>

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