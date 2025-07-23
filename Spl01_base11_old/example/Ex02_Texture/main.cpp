
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

	VEC2	vcPos(50, 300);
	LcsLib_Draw2D(nTx, &rt1, &vcPos);

	VEC2	vcScl(2, 3);

	vcPos = VEC2(300, 150);
	LcsLib_Draw2D(nTx, &rt1, &vcPos, &vcScl);



	VEC2	vcRot(50, 300);
	FLOAT	fRot = 3.14159f * 45/180;

	vcPos = VEC2(300, 200);
	LcsLib_Draw2D(nTx, &rt1, NULL, &vcScl, &vcRot, fRot, D3DCOLOR_ARGB(128, 128, 255, 255));

	return 0;
}

int main()
{
	printf("그림 올리기.......................\n\n");

	//배경색을 바꾼다.
	LcsLib_SetClearColor(0xFF336699);
	LcsLib_CreateWin(100, 100, 800, 600, "My First Game Window");


	// 그림을 프로그램에 로딩
	nTx = LcsLib_TextureLoad("Texture/lena.png");
	iImgW = LcsLib_TextureWidth(nTx);
	iImgH = LcsLib_TextureHeight(nTx);


	// 화면에 출력하기 위해서 함수를 연결한다.
	LcsLib_SetRender(Render);
	

	
	
	LcsLib_Run();


	LcsLib_TextureRelease(nTx);

	LcsLib_DestroyWin();

	
	return 0;
}