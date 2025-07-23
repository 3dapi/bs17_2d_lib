
#pragma comment(lib, "legacy_stdio_definitions.lib")
#pragma comment(lib, "../../lib/LcsLib_.lib")

#include "../../include/LcsLib.h"


#include <stdio.h>


int		iImgW;
int		iImgH;
int		nTx;


int Render()
{
	RECT	rt1 = {0,0,iImgW, iImgH};
	LcsLib_Draw2D(nTx, &rt1);

	return 0;
}


int FrameMove()
{
	LcsLib_SetWindowTitle("Change Window Mode. Try to Press Space Bar");


	int mouseX = LcsLib_GetMouseX();
	int mouseY = LcsLib_GetMouseY();
	int mouseZ = LcsLib_GetMouseZ();

	BYTE* pKey = (BYTE*)LcsLib_GetKeyboard();

	for(int i=0; i<256; ++i)
	{
		if( pKey[i])
			printf("You Presed %d key!!!\n", i);
	}


	return 0;
}


int main()
{
	LcsLib_SetClearColor(0xFF006699);
	LcsLib_SetWindowStyle(WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU| WS_VISIBLE);
	
	LcsLib_CreateWin(100, 100, 800, 600, "McUtil Keyboard Test");
	
	nTx = LcsLib_TextureLoad("Texture/lena.png");
	iImgW = LcsLib_TextureWidth(nTx);
	iImgH = LcsLib_TextureHeight(nTx);

	
	LcsLib_SetRender(Render);
	LcsLib_SetFrameMove(FrameMove);

	LcsLib_Run();

	LcsLib_DestroyWin();

	
	return 0;
}