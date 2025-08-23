//
// Created on 27.04.2023.
//

#include "Vector.h"
#include "Vector2D.h"

CVector2D::CVector2D(const CVector& v3) :
        CVector2D{v3.x, v3.y}
{
}

void CVector2D::Normalise(float* outMag) {
    auto mag = Magnitude();
    if (mag > 0.0f) {
        auto recip = 1.0F / mag;
        x *= recip;
        y *= recip;
    } else {
        x = 1.0f;
    }
    if (outMag) {
        *outMag = mag;
    }
}