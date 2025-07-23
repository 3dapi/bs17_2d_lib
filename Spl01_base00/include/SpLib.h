#ifndef _SpLib_H_
#define _SpLib_H_

#include <windows.h>


int		SpLib_CreateWin(int x, int y, int width, int height, char* sName);		//윈도우와 DirectX생성
void	SpLib_DestroyWin();														// 윈도우와 DirectX소멸
int		SpLib_Run();															// 게임 루프


#endif