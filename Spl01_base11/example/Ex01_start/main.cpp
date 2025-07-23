#pragma comment(lib, "legacy_stdio_definitions.lib")


// link the 2d game library
#if defined(_DEBUG)
  #pragma comment(lib, "LcsLib_.lib")
#else
  #pragma comment(lib, "LcsLib.lib")
#endif

// include the 2d game header file
#include <LcsLib.h>


#include <stdio.h>

int main()
{
	printf("Starting ...\n\n");

	// 윈도우 생성
	LcsLib_CreateWin(100, 100, 1024, 600, "My First Game Window");

	// 실행
	LcsLib_Run();

	// 윈도우 해제
	LcsLib_DestroyWin();

	return 0;
}