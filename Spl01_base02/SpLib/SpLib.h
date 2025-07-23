#ifndef _SpLib_
#define _SpLib_

#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>

int		SpLib_CreateWin(int x, int y, int width, int height, char* sName);		//윈도우와 DirectX생성
void	SpLib_DestroyWin();														// 윈도우와 DirectX소멸
int		SpLib_Run();															// 게임 루프

void	SpLib_SetClearColor(unsigned long dC);									// 배경화면 클리어 색상설정
DWORD	SpLib_GetClearColor();													// 배경화면 클리어 색상 가져오기

void	SpLib_SetWindowTitle(const char *format, ...);							// Window Title
void	SpLib_SetWindowStyle(DWORD dSty);										// window Style
DWORD	SpLib_GetWindowStyle();


#endif