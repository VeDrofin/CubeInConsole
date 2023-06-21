#pragma once

const long double Pi = 3.14159265358979;

// Отдаленность камеры от центра координат и маштабирование изображения.
// НЕОБХОДИМО ЗАМЕНИТЬ ПЕРВУЮ СТРОЧКУ В int main(){...}
const int SizeView = 75;
const int SizeCube = 30;

// Вектор расстояний каждого луча камеры до ближайшей стороны куба.
vector<vector<long double> > distMatrix(SizeView * 2, vector<long double>(SizeView * 4, 100000000));