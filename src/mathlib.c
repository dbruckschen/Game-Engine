#include "mathlib.h"

v2 V2(float x, float y) {
    v2 tmp = {0};
    tmp.x = x;
    tmp.y = y;
    return tmp;
}

bool V2Compare(v2 v1, v2 v2) {
    return ((v1.x == v2.x) && (v1.y == v2.y));
}

int Floor(float f) {
    return (int)(f - 0.5);
}

float V2Dot(v2 vec1, v2 vec2) {
    return (vec1.x * vec2.x) + (vec1.y * vec2.y);
}

void V2ScalarMult(v2 *vec, float scalar) {
    vec->x *= scalar;
    vec->y *= scalar;
}

double V2Length(v2 vec) {
    return sqrt((vec.x * vec.x) + (vec.y * vec.y));
}

v2 V2Div(v2 vec, float div_by) {
    vec.x /= div_by;
    vec.y /= div_by;
    return vec;
}

v2 V2Sub(v2 vec1, v2 vec2) {
    v2 result = {0};
    result.x = vec1.x - vec2.x;
    result.y = vec1.y - vec2.y;

    return result;
}

v2 V2Add(v2 vec1, v2 vec2) {
    v2 result = {0};
    result.x = vec1.x + vec2.x;
    result.y = vec1.y + vec2.y;

    return result;
}

v2 V2Normalize(v2 vector) {
    v2 result = {0};
    double vector_len  = V2Length(vector);

    result.x = (float)(vector.x / vector_len);
    result.y = (float)(vector.y / vector_len);

    return result;
}

void InitRandomNumberGen() {
    srand((unsigned)time(NULL));
}

float GenRandomFloat(float Min, float Max) {
    return ((float)rand() / (float)RAND_MAX) * (Max - Min) + Min;
}

int GenRandomInt(int lower, int upper) {
    return (rand() % (upper + 1 - lower)) + lower;
}

double DegToRad(float deg) {
    return (double)(deg * (PI / 180));
}

double RadToDeg(float rad) {
    return (double)(rad * (180 / PI));
}

float DC_TO_NDC(float v, int width_height) {
    return (v * 2) / width_height - 1;
}

float NDC_TO_DC(float v, int width_height) {
    return (float)fabs((v / 2) * width_height);
}

bool BBAA(v2 b1, int width1, int height1, v2 b2, int width2, int height2) {
    if(b1.x < b2.x + width2 && b1.x + width1 > b2.x &&
	   b1.y < b2.y + height2 && b1.y + height1 > b2.y)
        return true;
    else
        return false;
}

v2 GetCenteredCoordinates(int rec_x, int rec_y, int rec_width, int rec_height, int width_of_center_obj, int height_of_center_obj) {
	v2 result = {0};

	result.x = (float)(rec_x + ((rec_width - width_of_center_obj) / 2));
	result.y = (float)(rec_y + ((rec_height - height_of_center_obj) / 2));

	return result;
}
