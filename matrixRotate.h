#pragma once
#include "globalVariables.h"
#include "head.h"

#include "point.h"

// Матрица поворота на определённый градус.
struct matRotate
{
public:
    vector<vector<long double> > mat;

    // Матрица поворота в R^3
    matRotate(long double a, long double b, long double c)
    {
        mat.resize(3);
        for (int i = 0; i < 3; i++)
            mat[i].resize(3);

        a = 2 * Pi / 360 * a;
        b = 2 * Pi / 360 * b;
        c = 2 * Pi / 360 * c;

        mat[0][0] = cos(b) * cos(c);
        mat[0][1] = -sin(c) * cos(b);
        mat[0][2] = sin(b);
        mat[1][0] = sin(a) * sin(b) * cos(c) + sin(c) * cos(a);
        mat[1][1] = -sin(a) * sin(b) * sin(c) + cos(a) * cos(c);
        mat[1][2] = -sin(a) * cos(b);
        mat[2][0] = sin(a) * sin(c) - sin(b) * cos(a) * cos(c);
        mat[2][1] = sin(a) * cos(c) + sin(b) * sin(c) * cos(a);
        mat[2][2] = cos(a) * cos(b);
    }

    // Найти определитель матрицы. (по определению должен равняться единице)
    long double determinant()
    {
        long double First = mat[0][0] * (mat[1][1] * mat[2][2] - mat[1][2] * mat[2][1]);
        long double Second = mat[0][1] * (mat[1][0] * mat[2][2] - mat[2][0] * mat[1][2]);
        long double Third = mat[0][2] * (mat[1][0] * mat[2][1] - mat[1][1] * mat[2][0]);
        return First - Second + Third;
    }
};

// Поворот вектора.
void vectorRotate(point& p, matRotate& m)
{
    point rez;
    rez.x = p.x * m.mat[0][0] + p.y * m.mat[1][0] + p.z * m.mat[2][0];
    rez.y = p.x * m.mat[0][1] + p.y * m.mat[1][1] + p.z * m.mat[2][1];
    rez.z = p.x * m.mat[0][2] + p.y * m.mat[1][2] + p.z * m.mat[2][2];

    p = rez;
}