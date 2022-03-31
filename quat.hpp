#pragma once
#include <math.h>

typedef struct tQuat{
    float w, x, y, z;
}quat;


void q_mult(quat q1, quat q2, quat *buff){
    buff->w = q1.w*q2.w - q1.x*q2.x - q1.y*q2.y - q1.z*q2.z;
    buff->x = q1.w*q2.x + q1.x*q2.w + q1.y*q2.z - q1.z*q2.y;
    buff->y = q1.w*q2.y - q1.x*q2.z + q1.y*q2.w + q1.z*q2.x;
    buff->z = q1.w*q2.z + q1.x*q2.y - q1.y*q2.x + q1.z*q2.w;
}

void q_rotate(float v[], float theta, float axis[], float *buff){
    float c = cos(theta / 2.0);
    float s = sin(theta / 2.0);
    quat q1 = {c, axis[0] * s, axis[1] * s, axis[2] * s};
    quat q2 = {c, -axis[0] * s, -axis[1] * s, -axis[2] * s};
    quat p = {0.0, v[0], v[1], v[2]};
    quat tmp1;
    q_mult(q1, p, &tmp1);
    quat tmp2;
    q_mult(tmp1, q2, &tmp2);
    buff[0] = tmp2.x;
    buff[1] = tmp2.y;
    buff[2] = tmp2.z;
}