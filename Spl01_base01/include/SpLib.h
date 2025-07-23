#ifndef _SpLib_H_
#define _SpLib_H_

#include <windows.h>


int		SpLib_CreateWin(int x, int y, int width, int height, char* sName);		//윈도우와 DirectX생성
void	SpLib_DestroyWin();														// 윈도우와 DirectX소멸
int		SpLib_Run();															// 게임 루프

HWND	SpLib_GetHwnd();														// 윈도우 핸들 얻기
int		SpLib_GetScnW();														// 화면의 크기를 가져오기
int		SpLib_GetScnH();														// 화면의 너비를 가져오기

void	SpLib_SetWindowTitle(const char *format, ...);							// Window Title
void	SpLib_SetWindowStyle(DWORD dSty);										// window Style
DWORD	SpLib_GetWindowStyle();


#endif