#pragma once

#include "head.h"

#include "matrixRotate.h"
#include "point.h"
#include "line.h"
#include "square.h"


class cube
{
public:
    square sq1;
    square sq2;
    square sq3;
    square sq4;
    square sq5;
    square sq6;

    // Конструктор для куба.
    cube(square& sq1, square& sq2, square& sq3, square& sq4, square& sq5, square& sq6)
    {
        this->sq1 = sq1;
        this->sq2 = sq2;
        this->sq3 = sq3;
        this->sq4 = sq4;
        this->sq5 = sq5;
        this->sq6 = sq6;
    }

    cube(const int& SizeCube)
    {
        //sq1
        point p1(-1 * SizeCube, -1 * SizeCube, SizeCube);
        point p3(SizeCube, SizeCube, SizeCube);
        square sq1(p1, p3);
        //sq2
        p1 = point(-SizeCube, -SizeCube, -SizeCube);
        p3 = point(SizeCube, SizeCube, -SizeCube);
        square sq2(p1, p3);
        //sq3
        p1 = point(-SizeCube, -SizeCube, -SizeCube);
        p3 = point(SizeCube, -SizeCube, SizeCube);
        square sq3(p1, p3);
        //sq4
        p1 = point(-SizeCube, SizeCube, -SizeCube);
        p3 = point(SizeCube, SizeCube, SizeCube);
        square sq4(p1, p3);
        //sq5
        p1 = point(-SizeCube, SizeCube, SizeCube);
        p3 = point(-SizeCube, -SizeCube, -SizeCube);
        square sq5(p1, p3);
        //sq6
        p1 = point(SizeCube, SizeCube, SizeCube);
        p3 = point(SizeCube, -SizeCube, -SizeCube);
        square sq6(p1, p3);

        this->sq1 = sq1;
        this->sq2 = sq2;
        this->sq3 = sq3;
        this->sq4 = sq4;
        this->sq5 = sq5;
        this->sq6 = sq6;
    }

    // Повернуть все грани куба.
    void rotate(matRotate m)
    {
        squareRotate(sq1, m);
        squareRotate(sq2, m);
        squareRotate(sq3, m);
        squareRotate(sq4, m);
        squareRotate(sq5, m);
        squareRotate(sq6, m);
    }
};