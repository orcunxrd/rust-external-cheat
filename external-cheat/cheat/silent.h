#pragma once
#include <math.h>
#include "Vector.h"

double ToRad(double degree)
{
    double pi = 3.14159265359;
    return (degree * (pi / 180));
}
Vector4 ToQuat(Vector3 Euler) {

    double heading = ToRad(Euler.x);
    double attitude = ToRad(Euler.y);
    double bank = ToRad(Euler.z);

    double c1 = cos(heading / 2);
    double s1 = sin(heading / 2);
    double c2 = cos(attitude / 2);
    double s2 = sin(attitude / 2);
    double c3 = cos(bank / 2);
    double s3 = sin(bank / 2);
    double c1c2 = c1 * c2;
    double s1s2 = s1 * s2;
    Vector4 Quat;
    Quat.w = c1c2 * c3 - s1s2 * s3;
    Quat.x = c1c2 * s3 + s1s2 * c3;
    Quat.y = s1 * c2 * c3 + c1 * s2 * s3;
    Quat.z = c1 * s2 * c3 - s1 * c2 * s3;
    Vector4 Quat2;
    Quat2 = { Quat.y, Quat.z, (Quat.x * -1), Quat.w };
    return Quat2;
}