#pragma once
#include "head.h"
#include "globalVariables.h"

HANDLE const output{ ::GetStdHandle(STD_OUTPUT_HANDLE) };

void consoleInit()
{
    // ������ ������ � �������.
    CONSOLE_FONT_INFOEX cfi;
    cfi.cbSize = sizeof(cfi);
    cfi.dwFontSize.X = 0;                   // ������ ������� ������� � ������
    cfi.dwFontSize.Y = 10;
    SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);

    // ���-�� ����� ���������� �������� �� SizeView * 4, ����� �� SizeView * 2.
    system("mode con cols=160 lines=80");

    // ��������� ����������� ������� � ����� �������.
    ::CONSOLE_CURSOR_INFO cursor_info{ sizeof(cursor_info), FALSE };
    ::SetConsoleCursorInfo(output, ::std::addressof(cursor_info));
}