#pragma once

// ����� � ���������� ������� ���������.
struct point
{
    // ����������.
    long double x;
    long double y;
    long double z;

    // ������������.
    point(long double x = 0, long double y = 0, long double z = 0)
    {
        this->x = x;
        this->y = y;
        this->z = z;

    }

    // ����������� ���������.
    point operator+(point elem)
    {
        point rez;
        rez.x = x + elem.x;
        rez.y = y + elem.y;
        rez.z = z + elem.z;
        return rez;
    }
    point operator-(point elem)
    {
        point rez;
        rez.x = x - elem.x;
        rez.y = y - elem.y;
        rez.z = z - elem.z;
        return rez;
    }
    point operator*(long double num)
    {
        point Rez;
        Rez.x = x * num;
        Rez.y = y * num;
        Rez.z = z * num;
        return Rez;
    }
    bool operator==(point elem)
    {
        return (abs(x - elem.x) <= 0.0000001 && abs(y - elem.y) <= 0.0000001 && abs(z - elem.z) <= 0.0000001);
    }
    bool operator!=(point elem)
    {
        return (!(*this == elem));
    }

    // ����� ��������� �����.
    void print()
    {
        std::cout << x << ' ' << y << ' ' << z << std::endl;
    }

    // ���������� ������� � ��������� �����.
    void normalization()
    {
        long double znam = sqrt(x * x + y * y + z * z);
        x /= znam;
        y /= znam;
        z /= znam;
    }

    // ������� �� 90 ��������.
    void orthogonal()
    {
        long double c = x;
        x = -y;
        y = c;
    }
};

// ��������� �������� ���������� ����� ����� �������.
long double dist2(point elem1, point elem2)
{
    elem2 = elem2 - elem1;
    return elem2.x * elem2.x + elem2.y * elem2.y + elem2.z * elem2.z;
}
// ��������� ����� �������.
long double dist2(point elem)
{
    return elem.x * elem.x + elem.y * elem.y + elem.z * elem.z;
}
// ��������� ������������.
point cross(point a, point b)
{
    point rez;
    rez.x = a.y * b.z - a.z * b.y;
    rez.y = -a.x * b.z + a.z * b.x;
    rez.z = a.x * b.y - a.y * b.x;
    return rez;
}