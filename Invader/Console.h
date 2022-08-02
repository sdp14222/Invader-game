#ifndef __CONSOLE_H__
#define __CONSOLE_H__

#include <windows.h>
#include <wincon.h>

typedef struct {
	int x;
	int y;
} UPOINT;


void InitConsole();
void gotoxy(UPOINT pt);
void ClearScreen();

#endif