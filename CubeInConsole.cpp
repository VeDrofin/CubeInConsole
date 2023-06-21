#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
using namespace std;

const long double Pi = 3.14159265358979;

// Точка в декартовой системе координат.
struct point
{
    // Координаты.
    long double x;
    long double y;
    long double z;

    // Конструкторы.
    point(long double x = 0, long double y = 0, long double z = 0)
    {
        this->x = x;
        this->y = y;
        this->z = z;

    }

    // Необходимые операторы.
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

    // Приведение вектора к единичной длине.
    void normalization()
    {
        long double znam = sqrt(x * x + y * y + z * z);
        x /= znam;
        y /= znam;
        z /= znam;
    }
    // Поворот на 90 градусов.
    void orthogonal()
    {
        long double c = x;
        x = -y;
        y = c;
    }
};
// Матрица поворота на определённый градус.
struct matRotate
{
public:
    vector<vector<long double> > mat;
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

    long double determinant()
    {
        long double First = mat[0][0] * (mat[1][1] * mat[2][2] - mat[1][2] * mat[2][1]);
        long double Second = mat[0][1] * (mat[1][0] * mat[2][2] - mat[2][0] * mat[1][2]);
        long double Third = mat[0][2] * (mat[1][0] * mat[2][1] - mat[1][1] * mat[2][0]);
        //cout << First << endl;
        //cout << Second << endl;
        //cout << Third << endl;
        return First - Second + Third;
    }
};
// Поворот вектора.
point vectorRotate(point p, matRotate m)
{
    point rez;
    rez.x = p.x * m.mat[0][0] + p.y * m.mat[1][0] + p.z * m.mat[2][0];
    rez.y = p.x * m.mat[0][1] + p.y * m.mat[1][1] + p.z * m.mat[2][1];
    rez.z = p.x * m.mat[0][2] + p.y * m.mat[1][2] + p.z * m.mat[2][2];
    return rez;
}

// Получение квадрата расстояния между двумя точками.
long double dist2(point elem1, point elem2)
{
    elem2 = elem2 - elem1;
    return elem2.x * elem2.x + elem2.y * elem2.y + elem2.z * elem2.z;
}
// Получение длины вектора.
long double dist2(point elem)
{
    return elem.x * elem.x + elem.y * elem.y;
}

// Векторное произведение.
point cross(point a, point b)
{
    point rez;
    rez.x = a.y * b.z - a.z * b.y;
    rez.y = -a.x * b.z + a.z * b.x;
    rez.z = a.x * b.y - a.y * b.x;
    return rez;
}

// Класс прямой.
class Line
{
public:
    point First;
    point Second;

    // Конструктор класса.
    Line() { First = point(0, 0), Second = point(1, 0); }
    // По двум точкам.
    Line(point First, point Second)
    {
        this->First = First;
        this->Second = Second;
    }
    // По коэф. наклона и смещению.
    Line(long double ratio, long double shift)
    {
        First = point(0, shift);
        Second = point(100, shift + ratio * 100);
    }
    // По точке и коэф. наклона.
    Line(point First, long double ratio)
    {
        this->First = First;
        this->Second = point(First.x + 100, First.y + ratio * 100);
    }

    bool betweenPoints(point point)
    {
        return (sqrt(dist2(point, First)) + sqrt(dist2(point, Second)) <= sqrt(dist2(First, Second)));
    }

    // Оператор совпадения прямых.
    bool operator==(Line elem)
    {
        long double a = First.x - Second.x;
        long double b = First.y - Second.y;

        long double a_ = elem.First.x - elem.Second.x;
        long double b_ = elem.First.y - elem.Second.y;

        long double d = a * (elem.First.y - First.y) - b * (elem.First.x - First.x);
        cout << d << ' ' << b * a_ - a * b_ << endl;

        // Условия, при которых прямые совпадают.
        if (d == 0 && b * a_ - a * b_ == 0)
            return true;
        return false;
    }
    // Несовпадения прямых.
    bool operator!=(Line elem)
    {
        return(!(*this == elem));
    }
};

// Нахождение пересечения двух прямых.
point intersection(Line l1, Line l2)
{
    long double a = l1.First.x - l1.Second.x;
    long double b = l1.First.y - l1.Second.y;

    long double a_ = l2.First.x - l2.Second.x;
    long double b_ = l2.First.y - l2.Second.y;

    long double d = a * (l2.First.y - l1.First.y) - b * (l2.First.x - l1.First.x);
    cout << d << ' ' << b * a_ - a * b_ << endl;
    // Случай, когда прямые совпадают.
    if (d == 0 && b * a_ - a * b_ == 0)
    {
        cout << "\n\n Warning: Straight lines match! \n\n";
        return point(0, 0);
    }
    // Случай, когда пересечений нет.
    if (b * a_ - a * b_ == 0)
    {
        cout << "\n\n Warning: Straight lines don't intersect! \n\n";
        return point(0, 0);
    }

    d /= b * a_ - a * b_;

    point rez(l2.First.x + a_ * d, l2.First.y + b_ * d);
    return rez;
}
// Нахождение координаты точки, которой будет лежать на биссектрисе, выход. из точки a.
point Bis(point a, point b, point c)
{
    // Находим биссектриссу.
    point l2 = c - a;
    point l1 = b - a;
    l1.normalization();
    l2.normalization();

    point bis1 = (l2 - l1) * 0.5;
    return b + bis1;
}


// Квадрат в пространстве.
class square
{
public:
    point p1;
    point p2;
    point p3;
    point p4;

    square(point p1, point p3)
    {
        // Вектор диагонали.
        point d = p3 - p1;
        // Вектор, перпендикулярный диагонали.
        point p = p1 + d * 0.5;
        
        // Вектор, ортогональный двум предыдущим.
        point supportivVector;
        supportivVector.x = d.y * p.z - d.z * p.y;
        supportivVector.y = -d.x * p.z + d.z * p.x;
        supportivVector.z = d.x * p.y - d.y * p.x;


        supportivVector.normalization();
        // хз почему работает след. формула, методом тыка.
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


class cam
{
public:
    point coord;
    int widthVision, highVision;
    vector<vector<Line> > vect;
    cam(point Point = point(0, 0, 0), int w = 0, int h = 0)
    {
        coord = Point;
        widthVision = w;
        highVision = h;

        vect = vector<vector<Line> >(highVision, vector<Line>(widthVision));
        for (int i = 0; i < highVision; i++)
            for (int j = 0; j < widthVision; j++)
                vect[i][j] = Line(coord, point(-h / 2 + i, -w / 2 + j, -10));
    }

    void draw(square sq)
    {

        // Точки, через которую проходит плоскость.
        long double x = sq.p1.x;
        long double y = sq.p1.y;
        long double z = sq.p1.z;

        // A, B, C, D - члены нормального уравнения плоскости.
        point buf = cross(sq.p2 - sq.p1, sq.p3 - sq.p1);
        long double A = buf.x;
        long double B = buf.y;
        long double C = buf.z;
        long double D = -A * x - B * y - C * z;
        cout << (sq.p2 - sq.p1).x << ' ' << (sq.p2 - sq.p1).y << ' ' << (sq.p2 - sq.p1).z << endl;
        cout << (sq.p3 - sq.p1).x << ' ' << (sq.p3 - sq.p1).y << ' ' << (sq.p3 - sq.p1).z << endl;
        cout << A << ' ' << B << ' ' << C << endl;

        // a, b, c - направляющий вектор прямой.
        long double a, b, c;
        a = 0;
        b = 0;
        c = 1;

        // Прямая проходит через точку, задающую координаты камеры this->coord
        // Найдем значения параметра для прямой, через уравнение плоскости / решим систему уравнений.

        long double tCnt = A * a + B * b + C * c;
        cout << tCnt << endl;
        long double t = -A * coord.x - B * coord.y - C * coord.z - D;

        for (int i = 0; i < highVision; i++)
        {
            for (int j = 0; j < widthVision; j++)
            {
                // a, b, c - направляющий вектор прямой.
                long double a, b, c;
                point buf = vect[i][j].Second - vect[i][j].First;
                a = buf.x;
                b = buf.y;
                c = buf.z;
                // Прямая проходит через точку, задающую координаты камеры this->coord
                // Найдем значения параметра для прямой, через уравнение плоскости / решим систему уравнений.
                long double tCnt = A * a + B * b + C * c;
                long double t = -A * coord.x - B * coord.y - C * coord.z - D;
                if (t == 0)
                    cout << ' ';
                else
                {
                    t = t / tCnt;
                    point ans;
                    ans.x = coord.x + a * t;
                    ans.y = coord.y + b * t;
                    ans.z = coord.z + c * t;
                    //if (i == highVision / 2 && j == widthVision / 2)
                    //    cout << "Look at me: ";
                    //cout << ans.x << ' ' << ans.y << ' ' << ans.z << endl;
                    if (sq.p1.x <= ans.x && ans.x <= sq.p3.x &&
                        sq.p1.y <= ans.y && ans.y <= sq.p3.y &&
                        sq.p1.z <= ans.z && ans.z <= sq.p3.z)
                        cout << 1;
                    else if (sq.p3.x <= ans.x && ans.x <= sq.p1.x &&
                        sq.p3.y <= ans.y && ans.y <= sq.p1.y &&
                        sq.p3.z <= ans.z && ans.z <= sq.p1.z)
                        cout << 1;
                    else
                        cout << ' ';
                }
            }
            cout << endl;
        }

        //if (t == 0)
        //    cout << ' ';
        //else
        //{
        //    t = t / tCnt;
        //    point ans;
        //    ans.x = coord.x + a * t;
        //    ans.y = coord.y + b * t;
        //    ans.z = coord.z + c * t;
        //    cout << t << ' ' << ans.x << ' ' << ans.y << ' ' << ans.z << endl;
        //    if (sq.p1.x <= ans.x && ans.x <= sq.p3.x &&
        //        sq.p1.y <= ans.y && ans.y <= sq.p3.y &&
        //        sq.p1.z <= ans.z && ans.z <= sq.p3.z)
        //        cout << 1;
        //    else if (sq.p3.x <= ans.x && ans.x <= sq.p1.x &&
        //        sq.p3.y <= ans.y && ans.y <= sq.p1.y &&
        //        sq.p3.z <= ans.z && ans.z <= sq.p1.z)
        //        cout << 1;
        //    else
        //        cout << ' ';
        //}
    }
};

int main()
{
    point p1(-5, -5, 5);
    point p3(5, 5, 5);

    square sq1(p1, p3);

    sq1.print();

    p1 = point(-5, -5, -5);
    p3 = point(5, 5, -5);

    square sq2(p1, p3);

    p1 = point(-5, -5, -5);
    p3 = point(5, -5, 5);

    square sq3(p1, p3);

    p1 = point(-5, 5, -5);
    p3 = point(5, 5, 5);

    square sq4(p1, p3);


    //sq5
    p1 = point(-5, 5, 5);
    p3 = point(-5, -5, -5);
    square sq5(p1, p3);

    //sq6
    p1 = point(5, 5, 5);
    p3 = point(5, -5, -5);
    square sq6(p1, p3);

    cam c(point(0, 0, 25), 60, 30);
    c.draw(sq1);
    c.draw(sq2);
    c.draw(sq3);
    c.draw(sq4);
    c.draw(sq5);
    c.draw(sq6);

    //matRotate r(0, 90, );
    //vectorRotate(p1, r);
    //vectorRotate(p3, r);
    //sq6 = square(p1, p3);
    //c.draw(sq6);

    //c.draw(sq5);
    //c.draw(sq6);

}