#pragma once
#include "head.h"
#include "globalVariables.h"
#include "point.h"

// Класс прямой.
class line
{
public:
    point First;
    point Second;

    // Конструктор класса.
    line() { First = point(0, 0), Second = point(1, 0); }
    // По двум точкам.
    line(point First, point Second)
    {
        this->First = First;
        this->Second = Second;
    }
    // По коэф. наклона и смещению.
    line(long double ratio, long double shift)
    {
        First = point(0, shift);
        Second = point(100, shift + ratio * 100);
    }
    // По точке и коэф. наклона.
    line(point First, long double ratio)
    {
        this->First = First;
        this->Second = point(First.x + 100, First.y + ratio * 100);
    }

    bool betweenPoints(point point)
    {
        return (sqrt(dist2(point, First)) + sqrt(dist2(point, Second)) <= sqrt(dist2(First, Second)));
    }

    // Оператор совпадения прямых.
    bool operator==(line elem)
    {
        long double a = First.x - Second.x;
        long double b = First.y - Second.y;

        long double a_ = elem.First.x - elem.Second.x;
        long double b_ = elem.First.y - elem.Second.y;

        long double d = a * (elem.First.y - First.y) - b * (elem.First.x - First.x);
        //cout << d << ' ' << b * a_ - a * b_ << endl;

        // Условия, при которых прямые совпадают.
        if (d == 0 && b * a_ - a * b_ == 0)
            return true;
        return false;
    }
    // Несовпадения прямых.
    bool operator!=(line elem)
    {
        return(!(*this == elem));
    }
};