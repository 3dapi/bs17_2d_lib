
#pragma comment(lib, "legacy_stdio_definitions.lib")
#ifdef _DEBUG
	#pragma comment(lib, "./lib/SpLib_.lib")
#else
	#pragma comment(lib, "./lib/SpLib.lib")
#endif


#include <stdio.h>
#include "./include/SpLib.h"


DIMG	Img1;
PDTX	pTx1;

DIMG	Img2;
PDTX	pTx2;
RECT	m_ImgRc;				// RECT 애니 이미지
int		iImgW=50;


DWORD	m_dTimeBegin;				// 시작 타임
DWORD	m_dTimeEnd;					// 끝 타임


int Render()
{
	RECT	rt1 = {0,0,Img1.Width, Img1.Height};

	SpLib_Draw2D(pTx1, &rt1);

	SpLib_Draw2D(pTx2, &m_ImgRc, &VEC2(300, 300), &VEC2(2, 2));

	return 1;
}


int FrameMove()
{
	int mouseX = SpLib_GetMouseX();
	int mouseY = SpLib_GetMouseY();
	int mouseZ = SpLib_GetMouseZ();

	BYTE* pKey = SpLib_GetKeyboard();


	m_dTimeEnd	= SpLib_ElapsedTime();

	if( (m_dTimeEnd-m_dTimeBegin)>120)
	{
		m_ImgRc.left +=iImgW;

		if(m_ImgRc.left +iImgW >=(int)Img2.Width)
			m_ImgRc.left = 0;

		m_ImgRc.right =m_ImgRc.left +iImgW;
		m_dTimeBegin = m_dTimeEnd;
	}

	return 1;
}


int main()
{
	SpLib_SetClearColor(0xFF006699);
	SpLib_SetWindowStyle(WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU| WS_VISIBLE);

	SpLib_CreateWin(100, 100, 800, 600, "Animation");
	SpLib_SetRender(Render);
	SpLib_SetFrameMove(FrameMove);

	SpLib_TextureLoad("Texture/lena.png", pTx1, &Img1);
	SpLib_TextureLoad("Texture/mario.png", pTx2, &Img2);

	SetRect(&m_ImgRc, 0, 0, iImgW, Img2.Height);

	m_dTimeBegin	= SpLib_ElapsedTime();





	SpLib_Run();

	SpLib_DestroyWin();


	return 1;
}