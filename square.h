#pragma once

#include "head.h"
#include "globalVariables.h"

#include "point.h"

// ������� � ������������.
class square
{
public:
    point p1;
    point p2;
    point p3;
    point p4;

    square(point p1 = point(0, 0, 0), point p3 = point(0, 0, 0))
    {
        // ������ ���������.
        point d = p3 - p1;
        // ������, ���������������� ���������.
        point p = p1 + d * 0.5;

        // ������, ������������� ���� ����������.
        point supportivVector;
        supportivVector.x = d.y * p.z - d.z * p.y;
        supportivVector.y = -d.x * p.z + d.z * p.x;
        supportivVector.z = d.x * p.y - d.y * p.x;


        supportivVector.normalization();
        // �� ������ �������� ����. �������, ������� ����.
        supportivVector = supportivVector * sqrt(dist2(p1, p3)) * 0.5;

        this->p1 = p1;
        this->p2 = (p3 + p1) * 0.5 + supportivVector;
        this->p3 = p3;
        this->p4 = (p3 + p1) * 0.5 - supportivVector;
    }

    void print()
    {
        cout << p1.x << ' ' << p1.y << ' ' << p1.z << endl;
        cout << p2.x << ' ' << p2.y << ' ' << p2.z << endl;
        cout << p3.x << ' ' << p3.y << ' ' << p3.z << endl;
        cout << p4.x << ' ' << p4.y << ' ' << p4.z << endl;
    }
};

// �������� �������������� ����� �������� � ������������.
bool pointInSquare(point& p, square& sq)
{
    // ���������� �� ����� �� ������� �������� ������ ���� ������, ��� ���������� ����� ���������������� ���������.

    // ���������� �� ����� �� ������ ������ ����� ��������� ������������.
    // ������ ��������� ������������ ����� ������� ���������������, ������������ �� ���.
    // ������� ����� ����� = |����. �����| / ����� ����. ������� ������.

    long double distanceToPoint, sideLenght;
    sideLenght = dist2(sq.p1, sq.p2);
    distanceToPoint = dist2(cross(sq.p2 - sq.p1, p - sq.p1)) / dist2(sq.p2 - sq.p1);
    if (distanceToPoint > sideLenght) return false;
    distanceToPoint = dist2(cross(sq.p3 - sq.p2, p - sq.p2)) / dist2(sq.p3 - sq.p2);
    if (distanceToPoint > sideLenght) return false;
    distanceToPoint = dist2(cross(sq.p4 - sq.p3, p - sq.p3)) / dist2(sq.p4 - sq.p3);
    if (distanceToPoint > sideLenght) return false;
    distanceToPoint = dist2(cross(sq.p1 - sq.p4, p - sq.p4)) / dist2(sq.p1 - sq.p4);
    if (distanceToPoint > sideLenght) return false;

    return true;
}

// ������� ��������.
void squareRotate(square& sq, matRotate& m)
{
    vectorRotate(sq.p1, m);
    vectorRotate(sq.p2, m);
    vectorRotate(sq.p3, m);
    vectorRotate(sq.p4, m);
}