#ifndef MATH_H
#define MATH_H

#include "common.h"

#define PI 3.14159265359L

typedef struct {
    float x;
    float y;
} v2;

typedef struct {
	int x;
	int y;
} v2i;

// returns a v2 struct with x and y values
__declspec(dllexport) v2 V2(float x, float y);
// returns true if v1 == v2
__declspec(dllexport) bool V2Compare(v2 v1, v2 v2);
// rounds f to the lower value
__declspec(dllexport) int Floor(float f);

// vector functions
__declspec(dllexport) v2 V2Sub(v2 vec1, v2 vec2);
__declspec(dllexport) v2 V2Add(v2 vec1, v2 vec2);
__declspec(dllexport) v2 V2Div(v2 vec, float div_by);
__declspec(dllexport) void V2ScalarMult(v2 *vec, float scalar);
__declspec(dllexport) float V2Dot(v2 vec1, v2 vec2);
__declspec(dllexport) double V2Length(v2 vec);
__declspec(dllexport) v2 V2Normalize(v2 vector);

// random number generation
__declspec(dllexport) void InitRandomNumberGen();
__declspec(dllexport) float GenRandomFloat(float Min, float Max);
__declspec(dllexport) int GenRandomInt(int lower, int upper);

// degree and radions convertion
__declspec(dllexport) double DegToRad(float deg);
__declspec(dllexport) double RadToDeg(float rad);

// NDC range: [-1, 1] DC range: [0, window width] / [0, window height]
__declspec(dllexport) float NDC_TO_DC(float v, int width_height);
__declspec(dllexport) float DC_TO_NDC(float v, int width_height);

// Bounding Box Axis Aligned Collision Detection
__declspec(dllexport) bool BBAA(v2 b1, int width1, int height1, v2 b2, int width2, int height2);

// Computes the upper left coordinates for a rectangle centered inside another rectangle
// useful for aligning text inside a button rectangle for example
__declspec(dllexport) v2 GetCenteredCoordinates(int rec_x, int rec_y, int rec_width, int rec_height, int width_of_center_obj, int height_of_center_obj);

#endif //MATH_H

