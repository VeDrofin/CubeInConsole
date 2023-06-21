#pragma once

const long double Pi = 3.14159265358979;

// ������������ ������ �� ������ ��������� � �������������� �����������.
// ���������� �������� ������ ������� � int main(){...}
const int SizeView = 75;
const int SizeCube = 30;

// ������ ���������� ������� ���� ������ �� ��������� ������� ����.
vector<vector<long double> > distMatrix(SizeView * 2, vector<long double>(SizeView * 4, 100000000));