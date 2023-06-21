#include <iostream>
#include <Windows.h>
#include <array>
#include <string>
#include <cstddef>
#include <cstdlib>
#include <vector>
#include <cmath>
#include <iomanip>

using namespace std;
const long double Pi = 3.14159265358979;

// Отдаленность камеры от центра координат и маштабирование изображения.
// НЕОБХОДИМО ЗАМЕНИТЬ ПЕРВУЮ СТРОЧКУ В int main(){...}
const int SizeView = 75;
const int SizeCube = 30;

// Вектор расстояний каждого луча камеры до ближайшей стороны куба.
vector<vector<long double> > distMatrix(SizeView*2, vector<long double>(SizeView*4, 100000000));

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

    void print()
    {
        cout << x << ' ' << y << ' ' << z << endl;
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
    //return rez;
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
    return elem.x * elem.x + elem.y * elem.y + elem.z * elem.z;
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
        //cout << d << ' ' << b * a_ - a * b_ << endl;

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



// Квадрат в пространстве.
class square
{
public:
    point p1;
    point p2;
    point p3;
    point p4;

    square(point p1 = point(0, 0, 0), point p3 = point(0, 0, 0))
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

// Проверка пренадлежности точки квадрату в пространстве.
bool pointInSquare(point& p, square& sq)
{
    // Расстаяние от точки до стороны квадрата должно быть меньше, чем расстояние между противоположными сторонами.

    // Расстояние от точки до прямой нахожу через векторное произведение.
    // Модуль векторное произведение равен площади параллелограмма, построенного на них.
    // Поэтому длина равна = |вект. произ| / длина напр. вектора прямой.

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

// Класс камеры.
class Camera
{
public:
    // Координаты камеры.
    point coord;
    // Ширина и высота области видимости.
    int widthVision, highVision;
    // Вектор лучей камеры.
    vector<vector<Line> > vect;

    // Конструктор для класса camera.
    Camera(point coord = point(0, 0, 0), int widthVision = 0, int highVision = 0)
    {
        this->coord = coord;
        this->widthVision = widthVision;
        this->highVision = highVision;

        vect = vector<vector<Line> >(highVision, vector<Line>(widthVision));
        for (int i = 0; i < highVision; i++)
            for (int j = 0; j < widthVision; j++)
                vect[i][j] = Line(coord, point(( - highVision / 2 + i ) * 2, -widthVision / 2 + j, -SizeView));
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
                    if ((sq.p1.x <= ans.x && ans.x <= sq.p3.x || sq.p3.x <= ans.x && ans.x <= sq.p1.x) &&
                        (sq.p1.y <= ans.y && ans.y <= sq.p3.y || sq.p3.y <= ans.y && ans.y <= sq.p1.y) &&
                        (sq.p1.z <= ans.z && ans.z <= sq.p3.z || sq.p3.z <= ans.z && ans.z <= sq.p1.z))
                        cout << 1;
                    else if ((sq.p2.x <= ans.x && ans.x <= sq.p4.x || sq.p4.x <= ans.x && ans.x <= sq.p2.x) &&
                        (sq.p2.y <= ans.y && ans.y <= sq.p4.y || sq.p4.y <= ans.y && ans.y <= sq.p2.y) &&
                        (sq.p2.z <= ans.z && ans.z <= sq.p4.z || sq.p4.z <= ans.z && ans.z <= sq.p2.z))
                        cout << 1;
                    else
                        cout << ' ';
                }
            }
            cout << endl;
        }
    }
};

// Поворот квадрата.
void squareRotate(square& sq, matRotate& m)
{
    vectorRotate(sq.p1, m);
    vectorRotate(sq.p2, m);
    vectorRotate(sq.p3, m);
    vectorRotate(sq.p4, m);
}

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


vector<vector<int> > colorizeMat( Camera& camera, cube Cube)
{
    vector<vector<long double> > distMatrix(camera.highVision, vector<long double>(camera.widthVision, 100000000));
    vector<vector<int> > answer(camera.highVision, vector<int>(camera.widthVision, -1));

    vector<square*> sqs = { &Cube.sq1, &Cube.sq2, &Cube.sq3, &Cube.sq4, &Cube.sq5, &Cube.sq6 };

    for (int k = 0; k < 6; k++)
    {
        square& sq = *sqs[k];
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

        for (int i = 0; i < camera.highVision; i++)
        {
            for (int j = 0; j < camera.widthVision; j++)
            {
                // a, b, c - направляющий вектор прямой.
                long double a, b, c;
                point buf = camera.vect[i][j].Second - camera.vect[i][j].First;
                a = buf.x;
                b = buf.y;
                c = buf.z;
                // Прямая проходит через точку, задающую координаты камеры this->coord
                // Найдем значения параметра для прямой, через уравнение плоскости / решим систему уравнений.
                long double tCnt = A * a + B * b + C * c;
                long double t = -A * camera.coord.x - B * camera.coord.y - C * camera.coord.z - D;
                if (t == 0)
                    continue;
                else
                {
                    t = t / tCnt;
                    point ans;
                    ans.x = camera.coord.x + a * t;
                    ans.y = camera.coord.y + b * t;
                    ans.z = camera.coord.z + c * t;
                    
                    if (pointInSquare(ans, sq))
                        if (dist2(camera.coord, ans) < distMatrix[i][j])
                        {
                            distMatrix[i][j] = dist2(camera.coord, ans);
                            answer[i][j] = k;
                        }
                }
            }
        }

    }

    return answer;

};

// (попытка в оптимизацию)
// Расставление символов в буфер вывода.
void colorizeMat(array<wchar_t, SizeView * 4 * SizeView * 2>& buf, Camera& camera, cube& Cube)
{

    for (int i = 0; i < camera.highVision; i++)
        for (int j = 0; j < camera.widthVision; j++)
        {
            distMatrix[i][j] = 100000000;
            buf[i * 4 * SizeView + j] = ' ';
        }

    // Вектор с сылками на стороны куба.
    vector<square*> sqs = { &Cube.sq1, &Cube.sq2, &Cube.sq3, &Cube.sq4, &Cube.sq5, &Cube.sq6 };

    for (int k = 0; k < 6; k++)
    {
        square* sq = sqs[k];
        // Точки, через которую проходит плоскость.
        long double x = sq->p1.x;
        long double y = sq->p1.y;
        long double z = sq->p1.z;


        // A, B, C, D - члены нормального уравнения плоскости.
        point l = cross(sq->p2 - sq->p1, sq->p3 - sq->p1);
        long double A = l.x;
        long double B = l.y;
        long double C = l.z;
        long double D = -A * x - B * y - C * z;

        for (int i = 0; i < camera.highVision; i++)
        {
            for (int j = 0; j < camera.widthVision; j++)
            {
                // a, b, c - направляющий вектор прямой.
                long double a, b, c;
                point l = camera.vect[i][j].Second - camera.vect[i][j].First;
                a = l.x;
                b = l.y;
                c = l.z;
                // Прямая проходит через точку, задающую координаты камеры this->coord
                // Найдем значения параметра для прямой, через уравнение плоскости / решим систему уравнений.
                long double tCnt = A * a + B * b + C * c;
                long double t = -A * camera.coord.x - B * camera.coord.y - C * camera.coord.z - D;
                if (t == 0)
                    continue;
                else
                {
                    t = t / tCnt;
                    point ans;
                    ans.x = camera.coord.x + a * t;
                    ans.y = camera.coord.y + b * t;
                    ans.z = camera.coord.z + c * t;

                    if (pointInSquare(ans, *sq))
                        if (dist2(camera.coord, ans) < distMatrix[i][j])
                        {
                            distMatrix[i][j] = dist2(camera.coord, ans);
                            buf[i * 4 * SizeView + j] = char(k + '1');
                        }
                }
            }
        }
    }

    return;
};

// Вывод куба в консоль.
void print(cube Cube, Camera camera)
{
    vector<vector<int> > vision = colorizeMat(camera, Cube);
    for (int i = 0; i < vision.size(); i++)
    {
        for (int j = 0; j < vision[0].size(); j++)
        {
            if (vision[i][j] == -1)
                cout << ' ';
            else
                cout << vision[i][j];
        }
        cout << endl;
    }
}

int main()
{
    // Размер шрифта в консоли.
    CONSOLE_FONT_INFOEX cfi;
    cfi.cbSize = sizeof(cfi);
    cfi.dwFontSize.X = 0;                   // Ширина каждого символа в шрифте
    cfi.dwFontSize.Y = 6;
    SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);

    // Кол-во колон необходимо заменить на SizeView * 4, строк на SizeView * 2.
    system("mode con cols=300 lines=150");

    // Матрица поворота.
    matRotate rotate(1, 0, 0); // Параметры - поворот по осям x, y, z.

    // Инициализация буфера вывода.
    array<wchar_t, SizeView * 4 * SizeView * 2> buf{};
    HANDLE const output{ ::GetStdHandle(STD_OUTPUT_HANDLE) };
    
    // Отключить отображение курсора в конце консоли.
    ::CONSOLE_CURSOR_INFO cursor_info{ sizeof(cursor_info), FALSE };
    ::SetConsoleCursorInfo(output, ::std::addressof(cursor_info));
    

    // Инициализация камеры и куба.
    Camera camera(point(0, 0, SizeView), SizeView * 4, SizeView * 2);
    cube Cube(SizeCube);

    while(true)
    {
        colorizeMat(buf, camera, Cube);
        ::WriteConsoleW(output, buf.data(), SizeView * 2 * SizeView * 4, {}, {});
        Cube.rotate(rotate);
    }
}