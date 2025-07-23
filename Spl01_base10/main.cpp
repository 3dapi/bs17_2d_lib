
#pragma comment(lib, "legacy_stdio_definitions.lib")
#ifdef _DEBUG
	#pragma comment(lib, "./lib/SpLib_.lib")
#else
	#pragma comment(lib, "./lib/SpLib.lib")
#endif


#include <stdio.h>
#include "./include/SpLib.h"



int		nTx_01;
int		iImgW_01;
int		iImgH_01;


int		nTx_02;
int		iImgW_02;
int		iImgH_02;


int		nSound1;
int		nSound2;

int		nFont1;
int		nFont2;

int		mouseX;
int		mouseY;
int		mouseZ;

const BYTE*	pKey;


int FrameMove()
{
	mouseX = SpLib_GetMouseX();
	mouseY = SpLib_GetMouseY();
	mouseZ = SpLib_GetMouseZ();

	pKey = SpLib_GetKeyboard();


	SpLib_SetWindowTitle("%d %d %d", mouseX, mouseY,mouseZ);




	if(pKey[VK_LEFT] && !SpLib_SoundIsPlaying(nSound1))
	{
		SpLib_SoundReset(nSound1);
		SpLib_SoundPlay(nSound1);
	}

	if(pKey[VK_RIGHT] && !SpLib_SoundIsPlaying(nSound2))
	{
		SpLib_SoundReset(nSound2);
		SpLib_SoundPlay(nSound2);
	}

	return 0;
}

int Render()
{
	RECT	rt1 = {0,0,iImgW_01, iImgH_01};
	VEC2	vcPos(100, 50);
	SpLib_Draw2D(nTx_01, &rt1, &vcPos);

	SetRect(&rt1, 0,0,iImgW_02, iImgH_02);

	VEC2	vcScl(2, 3);
	VEC2	vcRot(250, 150);
	FLOAT	fRot = 3.14159f * 45/180;

	vcPos = VEC2(350, 150);
	SpLib_Draw2D(nTx_02, &rt1, &vcPos, &vcScl, &vcRot, fRot, D3DCOLOR_ARGB(128, 128, 255, 255));


	SpLib_FontDrawText(nFont1, 20, 200, 500, 340, 0xffFFFFFF, "Tri to Press Left Key");
	SpLib_FontDrawText(nFont2, 20, 300, 500, 340, 0xffFFAAFF, "Tri to Press Right Key");

	return 0;
}




int main()
{
	//배경색을 바꾼다.
	SpLib_SetClearColor(0xFF336699);
	SpLib_CreateWin(100, 100, 800, 600, "SpLib App", false);


	// 그림을 프로그램에 로딩
	nTx_01 = SpLib_TextureLoad("Texture/lena.png");

	iImgW_01 = SpLib_TextureWidth(nTx_01);
	iImgH_01 = SpLib_TextureHeight(nTx_01);

	nTx_02 = SpLib_TextureLoad("Texture/mario2.png");
	iImgW_02 = SpLib_TextureWidth(nTx_02);
	iImgH_02 = SpLib_TextureHeight(nTx_02);


	nSound1 = SpLib_SoundLoad("sound/move3.wav");
	nSound2 = SpLib_SoundLoad("sound/trample.wav");


	nFont1 = SpLib_FontCreate("Arial", 20, 0);
	nFont2 = SpLib_FontCreate("궁서", 25, 0);

	// 데이터를 갱신하기 위해서 함수를 연결한다.
	SpLib_SetFrameMove(FrameMove);


	// 화면에 출력하기 위해서 함수를 연결한다.
	SpLib_SetRender(Render);




	SpLib_Run();


	SpLib_TextureRelease(nTx_01);

	SpLib_DestroyWin();


	return 0;
}