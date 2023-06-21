#include "head.h"
#include "globalVariables.h"

#include "consoleSettings.h"

#include "point.h"
#include "matrixRotate.h"
#include "line.h"
#include "square.h"
#include "square.h"
#include "camera.h"
#include "cube.h"


// Вывод куба в консоль.
void print(cube Cube, camera Camera)
{
    vector<vector<int> > vision = colorizeMat(Camera, Cube);
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
    consoleInit();

    // Инициализация матрицы поворота.
    matRotate rotate(1, 0.6, 0); // Параметры - поворот по осям x, y, z.
    // Инициализация буфера вывода.
    array<wchar_t, SizeView * 4 * SizeView * 2> buf{};
    // Инициализация камеры.
    camera Camera(point(0, 0, SizeView), SizeView * 4, SizeView * 2);
    // Инициализация куба.
    cube Cube(SizeCube);

    while(true)
    {
        colorizeMat(buf, Camera, Cube);
        ::WriteConsoleW(output, buf.data(), SizeView * 2 * SizeView * 4, {}, {});
        Cube.rotate(rotate);
    }
}