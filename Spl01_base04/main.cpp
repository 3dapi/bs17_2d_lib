
#pragma comment(lib, "legacy_stdio_definitions.lib")
#ifdef _DEBUG
	#pragma comment(lib, "./lib/SpLib_.lib")
#else
	#pragma comment(lib, "./lib/SpLib.lib")
#endif


#include <stdio.h>
#include "./include/SpLib.h"


LPDIRECT3DTEXTURE9	m_pTx;
D3DXIMAGE_INFO		m_Img;

int m_MouseX = 0;
int m_MouseY = 0;
int m_MouseZ = 0;

int Render()
{
	RECT rc={0,0, m_Img.Width, m_Img.Height};

	D3DXVECTOR2	vcPos(m_MouseX, m_MouseY);

	SpLib_Draw2D(m_pTx, &rc, &vcPos, NULL, NULL, NULL, 0xFFFFFFFF);

	return 0;
}


int FrameMove()
{
	SpLib_SetWindowTitle("Keyboard Test");


	m_MouseX = SpLib_GetMouseX();
	m_MouseY = SpLib_GetMouseY();
	m_MouseZ = SpLib_GetMouseZ();

	BYTE* pKey = SpLib_GetKeyboard();

	for(int i=0; i<256; ++i)
	{
		if( pKey[i])
			SpLib_SetWindowTitle("You Presed 0x%x key!!!", i);
	}

	if( pKey[VK_SPACE])
	{
		SpLib_SetWindowTitle("Mouse %d %d %d", m_MouseX, m_MouseY, m_MouseZ);
	}

	return 0;
}


int main()
{
	SpLib_SetClearColor(0xFF006699);
	SpLib_SetWindowStyle(WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU| WS_VISIBLE);

	SpLib_CreateWin(100, 100, 800, 600, "McUtil Keyboard Test");

	SpLib_TextureLoad("texture/lena.png", m_pTx, &m_Img);

	

	SpLib_SetRender(Render);
	SpLib_SetFrameMove(FrameMove);

	SpLib_Run();

	SpLib_TextureRelease(m_pTx);

	SpLib_DestroyWin();


	return 0;
}