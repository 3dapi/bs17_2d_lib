
#pragma comment(lib, "legacy_stdio_definitions.lib")
#pragma comment(lib, "../../lib/LcsLib_.lib")

#include "../../include/LcsLib.h"

#include <stdio.h>

int main()
{
	printf("Starting ...\n\n");

	// Create Window
	LcsLib_CreateWin(100, 100, 800, 600, "My First Game Window");
	
	// Run
	LcsLib_Run();

	//Destroy
	LcsLib_DestroyWin();
	
	return 0;
}