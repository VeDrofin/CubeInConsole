#pragma once

#include "head.h"
#include "point.h"
#include "line.h"
#include "square.h"
#include "cube.h"

//  ласс камеры.
class camera
{
public:
    //  оординаты камеры.
    point coord;
    // Ўирина и высота области видимости.
    int widthVision, highVision;
    // ¬ектор лучей камеры.
    vector<vector<line> > vect;

    //  онструктор дл€ класса Camera.
    camera(point coord = point(0, 0, 0), int widthVision = 0, int highVision = 0)
    {
        this->coord = coord;
        this->widthVision = widthVision;
        this->highVision = highVision;

        vect = vector<vector<line> >(highVision, vector<line>(widthVision));
        for (int i = 0; i < highVision; i++)
            for (int j = 0; j < widthVision; j++)
                vect[i][j] = line(coord, point((-highVision / 2 + i) * 2, -widthVision / 2 + j, -SizeView));
    }

    void draw(square sq)
    {
        // “очки, через которую проходит плоскость.
        long double x = sq.p1.x;
        long double y = sq.p1.y;
        long double z = sq.p1.z;

        // A, B, C, D - члены нормального уравнени€ плоскости.
        point buf = cross(sq.p2 - sq.p1, sq.p3 - sq.p1);
        long double A = buf.x;
        long double B = buf.y;
        long double C = buf.z;
        long double D = -A * x - B * y - C * z;


        for (int i = 0; i < highVision; i++)
        {
            for (int j = 0; j < widthVision; j++)
            {
                // a, b, c - направл€ющий вектор пр€мой.
                long double a, b, c;
                point buf = vect[i][j].Second - vect[i][j].First;
                a = buf.x;
                b = buf.y;
                c = buf.z;

                // ѕр€ма€ проходит через точку, задающую координаты камеры this->coord
                // Ќайдем значени€ параметра дл€ пр€мой, через уравнение плоскости / решим систему уравнений.
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


                    if (pointInSquare(ans, sq))
                        cout << 1;
                    else
                        cout << ' ';
                }
            }
            cout << endl;
        }
    }
};

vector<vector<int> > colorizeMat(camera& Camera, cube Cube)
{
    vector<vector<long double> > distMatrix(Camera.highVision, vector<long double>(Camera.widthVision, 100000000));
    vector<vector<int> > answer(Camera.highVision, vector<int>(Camera.widthVision, -1));

    vector<square*> sqs = { &Cube.sq1, &Cube.sq2, &Cube.sq3, &Cube.sq4, &Cube.sq5, &Cube.sq6 };

    for (int k = 0; k < 6; k++)
    {
        square& sq = *sqs[k];
        // “очки, через которую проходит плоскость.
        long double x = sq.p1.x;
        long double y = sq.p1.y;
        long double z = sq.p1.z;


        // A, B, C, D - члены нормального уравнени€ плоскости.
        point buf = cross(sq.p2 - sq.p1, sq.p3 - sq.p1);
        long double A = buf.x;
        long double B = buf.y;
        long double C = buf.z;
        long double D = -A * x - B * y - C * z;

        for (int i = 0; i < Camera.highVision; i++)
        {
            for (int j = 0; j < Camera.widthVision; j++)
            {
                // a, b, c - направл€ющий вектор пр€мой.
                long double a, b, c;
                point buf = Camera.vect[i][j].Second - Camera.vect[i][j].First;
                a = buf.x;
                b = buf.y;
                c = buf.z;
                // ѕр€ма€ проходит через точку, задающую координаты камеры this->coord
                // Ќайдем значени€ параметра дл€ пр€мой, через уравнение плоскости / решим систему уравнений.
                long double tCnt = A * a + B * b + C * c;
                long double t = -A * Camera.coord.x - B * Camera.coord.y - C * Camera.coord.z - D;
                if (t == 0)
                    continue;
                else
                {
                    t = t / tCnt;
                    point ans;
                    ans.x = Camera.coord.x + a * t;
                    ans.y = Camera.coord.y + b * t;
                    ans.z = Camera.coord.z + c * t;

                    if (pointInSquare(ans, sq))
                        if (dist2(Camera.coord, ans) < distMatrix[i][j])
                        {
                            distMatrix[i][j] = dist2(Camera.coord, ans);
                            answer[i][j] = k;
                        }
                }
            }
        }

    }

    return answer;

};

// (попытка в оптимизацию)
// –асставление символов в буфер вывода.
void colorizeMat(array<wchar_t, SizeView * 4 * SizeView * 2>& buf, camera& Camera, cube& Cube)
{

    for (int i = 0; i < Camera.highVision; i++)
        for (int j = 0; j < Camera.widthVision; j++)
        {
            distMatrix[i][j] = 100000000;
            buf[i * 4 * SizeView + j] = ' ';
        }

    // ¬ектор с сылками на стороны куба.
    vector<square*> sqs = { &Cube.sq1, &Cube.sq2, &Cube.sq3, &Cube.sq4, &Cube.sq5, &Cube.sq6 };

    for (int k = 0; k < 6; k++)
    {
        square* sq = sqs[k];
        // “очки, через которую проходит плоскость.
        long double x = sq->p1.x;
        long double y = sq->p1.y;
        long double z = sq->p1.z;


        // A, B, C, D - члены нормального уравнени€ плоскости.
        point l = cross(sq->p2 - sq->p1, sq->p3 - sq->p1);
        long double A = l.x;
        long double B = l.y;
        long double C = l.z;
        long double D = -A * x - B * y - C * z;

        for (int i = 0; i < Camera.highVision; i++)
        {
            for (int j = 0; j < Camera.widthVision; j++)
            {
                // a, b, c - направл€ющий вектор пр€мой.
                long double a, b, c;
                point l = Camera.vect[i][j].Second - Camera.vect[i][j].First;
                a = l.x;
                b = l.y;
                c = l.z;
                // ѕр€ма€ проходит через точку, задающую координаты камеры this->coord
                // Ќайдем значени€ параметра дл€ пр€мой, через уравнение плоскости / решим систему уравнений.
                long double tCnt = A * a + B * b + C * c;
                long double t = -A * Camera.coord.x - B * Camera.coord.y - C * Camera.coord.z - D;
                if (t == 0)
                    continue;
                else
                {
                    t = t / tCnt;
                    point ans;
                    ans.x = Camera.coord.x + a * t;
                    ans.y = Camera.coord.y + b * t;
                    ans.z = Camera.coord.z + c * t;

                    if (pointInSquare(ans, *sq))
                        if (dist2(Camera.coord, ans) < distMatrix[i][j])
                        {
                            distMatrix[i][j] = dist2(Camera.coord, ans);
                            buf[i * 4 * SizeView + j] = char(k + '1');
                        }
                }
            }
        }
    }

    return;
};