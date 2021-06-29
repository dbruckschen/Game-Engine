#include "mathlib.h"

v2 V2(int x, int y)
{
    v2 tmp = {(float)x, (float)y};
    return tmp;
}

bool V2Compare(v2 v1, v2 v2)
{
    return ((v1.x == v2.x) && (v1.y == v2.y));
}

int Floor(float f)
{
    return (int)(f - 0.5);
}

float V2Dot(v2 vec1, v2 vec2)
{
    return (vec1.x * vec2.x) + (vec1.y * vec2.y);
}

void V2ScalarMult(v2 *vec, float scalar)
{
    vec->x *= scalar;
    vec->y *= scalar;
}

double V2Length(v2 vec)
{
    return sqrt((vec.x * vec.x) + (vec.y * vec.y));
}

v2 V2Div(v2 vec, float div_by)
{
    vec.x /= div_by;
    vec.y /= div_by;
    
    return vec;
}

v2 V2Sub(v2 vec1, v2 vec2)
{
    v2 result = {0};
    result.x = vec1.x - vec2.x;
    result.y = vec1.y - vec2.y;
    
    return result;
}

v2 V2Add(v2 vec1, v2 vec2)
{
    v2 result = {0};
    result.x = vec1.x + vec2.x;
    result.y = vec1.y + vec2.y;
    
    return result;
}

v2 V2Normalize(v2 vector)
{
    v2 result = {0};
    double vector_len  = V2Length(vector);
    
    result.x = (float)(vector.x/vector_len);
    result.y = (float)(vector.y/vector_len);
    
    return result;
}

void InitRandomNumberGen()
{
    srand((unsigned)time(NULL));
}

float GenRandomFloat(float Min, float Max)
{
    return ((float)rand() / (float)RAND_MAX) * (Max - Min) + Min;
}

int GenRandomInt(int lower, int upper)
{
    return (rand() % (upper + 1 - lower)) + lower;
}

double DegToRad(float deg)
{
    return (double)(deg * (PI / 180));
}

double RadToDeg(float rad)
{
    return (double)(rad * (180 / PI));
}
