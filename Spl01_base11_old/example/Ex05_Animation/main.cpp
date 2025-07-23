
#pragma comment(lib, "legacy_stdio_definitions.lib")
#pragma comment(lib, "../../lib/LcsLib_.lib")

#include "../../include/LcsLib.h"


#include <stdio.h>


// Lena
int		nTx1;
int		iImgW1;
int		iImgH1;


// Super Mario
int		nTx2;
int		iImgW2;
int		iImgH2;


RECT	m_ImgRc;				// RECT 애니 이미지
int		iImgW=50;


DWORD	m_dTimeBegin;				// 시작 타임
DWORD	m_dTimeEnd;					// 끝 타임


int FrameMove()
{
	int mouseX = LcsLib_GetMouseX();
	int mouseY = LcsLib_GetMouseY();
	int mouseZ = LcsLib_GetMouseZ();

	BYTE* pKey = (BYTE*)LcsLib_GetKeyboard();



	m_dTimeEnd	= timeGetTime();

	if( (m_dTimeEnd-m_dTimeBegin)>120)
	{
		m_ImgRc.left +=iImgW;

		if(m_ImgRc.left +iImgW >=(int)iImgW2)
			m_ImgRc.left = 0;

		m_ImgRc.right =m_ImgRc.left +iImgW;
		m_dTimeBegin = m_dTimeEnd;
	}
	
	return 0;
}



int Render()
{
	RECT	rt1 = {0,0,iImgW1, iImgH1};

	LcsLib_Draw2D(nTx1, &rt1);
	LcsLib_Draw2D(nTx2, &m_ImgRc, &VEC2(300, 300), &VEC2(2, 2));

	return 0;
}

int main()
{
	LcsLib_SetClearColor(0xFF006699);
	LcsLib_SetWindowStyle(WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU| WS_VISIBLE);
	
	LcsLib_CreateWin(100, 100, 800, 600, "McUtil Animation Test", false);
	LcsLib_SetRender(Render);
	LcsLib_SetFrameMove(FrameMove);

	nTx1	= LcsLib_TextureLoad("Texture/lena.png");
	iImgW1	= LcsLib_TextureWidth(nTx1);
	iImgH1	= LcsLib_TextureHeight(nTx1);

	nTx2	= LcsLib_TextureLoad("Texture/mario.png");
	iImgW2	= LcsLib_TextureWidth(nTx2);
	iImgH2	= LcsLib_TextureHeight(nTx2);

	SetRect(&m_ImgRc, 0, 0, iImgW, iImgH2);
	
	m_dTimeBegin	=timeGetTime();
	
	
	
	

	LcsLib_Run();

	LcsLib_DestroyWin();

	
	return 0;
}