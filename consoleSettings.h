#pragma once
#include "head.h"
#include "globalVariables.h"

HANDLE const output{ ::GetStdHandle(STD_OUTPUT_HANDLE) };

void consoleInit()
{
    // Размер шрифта в консоли.
    CONSOLE_FONT_INFOEX cfi;
    cfi.cbSize = sizeof(cfi);
    cfi.dwFontSize.X = 0;                   // Ширина каждого символа в шрифте
    cfi.dwFontSize.Y = 6;
    SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);

    // Кол-во колон необходимо заменить на SizeView * 4, строк на SizeView * 2.
    system("mode con cols=300 lines=150");

    // Отключить отображение курсора в конце консоли.
    ::CONSOLE_CURSOR_INFO cursor_info{ sizeof(cursor_info), FALSE };
    ::SetConsoleCursorInfo(output, ::std::addressof(cursor_info));
}