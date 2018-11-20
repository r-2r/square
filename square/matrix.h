// 4 x 4 Matrix

#pragma once

class CMatrix
{

// variable

private:
    float m[16];

public:

// function

private:

public:
    CMatrix();
    ~CMatrix();

    void Orthographic(float l, float r, float b, float t, float n, float f);

    operator float* ();
};
