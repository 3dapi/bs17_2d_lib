
#pragma comment(lib, "legacy_stdio_definitions.lib")
#ifdef _DEBUG
	#pragma comment(lib, "./lib/SpLib_.lib")
#else
	#pragma comment(lib, "./lib/SpLib.lib")
#endif


#include <stdio.h>
#include "./include/SpLib.h"

int main()
{
	printf("Starting SpLib\n\n");

	// Create Window
	SpLib_CreateWin(100, 100, 800, 600, "SpLib App");
	SpLib_SetClearColor(0xFF0000FF);

	// Run
	SpLib_Run();

	//Destroy
	SpLib_DestroyWin();

	return 0;
}