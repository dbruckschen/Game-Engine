#ifndef MATH_H
#define MATH_H

#include <math.h>
#include <stdlib.h>
#include "common.h"

#define V2(value1, value2) V2(value1, value2)
#define PI 3.14159265359L

typedef struct 
{
    float x;
    float y;

} v2;

// returns a v2 struct with x and y values
v2 V2(int x, int y);
// returns true if v1 == v2
bool V2Compare(v2 v1, v2 v2);
// rounds f to the lower value
int Floor(float f);

// vector functions
v2 V2Sub(v2 vec1, v2 vec2);
v2 V2Add(v2 vec1, v2 vec2);
v2 V2Div(v2 vec, float div_by);
void V2ScalarMult(v2 *vec, float scalar);
float V2Dot(v2 vec1, v2 vec2);
double V2Length(v2 vec);
v2 V2Normalize(v2 vector);

// random number generation
void InitRandomNumberGen();
float GenRandomFloat(float Min, float Max);
int GenRandomInt(int lower, int upper);

// degree and radions convertion
float DegToRad(float deg);
float RadToDeg(float rad);

#endif //MATH_H

