#pragma once

const long double Pi = 3.14159265358979;

// ������������ ������ �� ������ ��������� � �������������� �����������.
// ��� ������ ���������� �������� ������ ������� � consoleSettings.h - system("mode con cols=300 lines=150");
const int SizeView = 40;
const int SizeCube = 15;

// ������ ���������� ������� ���� ������ �� ��������� ������� ����.
vector<vector<long double> > distMatrix(SizeView * 2, vector<long double>(SizeView * 4, 100000000));