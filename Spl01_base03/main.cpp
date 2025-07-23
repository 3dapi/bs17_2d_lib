
#pragma comment(lib, "legacy_stdio_definitions.lib")
#ifdef _DEBUG
	#pragma comment(lib, "./lib/SpLib_.lib")
#else
	#pragma comment(lib, "./lib/SpLib.lib")
#endif


#include <Windows.h>
#include <stdio.h>

#include <d3d9.h>
#include <d3dx9.h>

#include "./include/SpLib.h"

LPDIRECT3DTEXTURE9	m_pTx;
D3DXIMAGE_INFO		m_Img;


int Render()
{
	RECT rc={0,0, m_Img.Width, m_Img.Height};

	SpLib_Draw2D(m_pTx, &rc,NULL, NULL, NULL, NULL, 0xFFFFFFFF);

	return 0;
}


int main()
{
	printf("Starting SpLib\n\n");

	// Create Window
	SpLib_CreateWin(100, 100, 800, 600, "SpLib App");
	SpLib_SetClearColor(0xFF0000FF);

	SpLib_TextureLoad("texture/lena.png", m_pTx, &m_Img);

	SpLib_SetRender(Render);

	// Run
	SpLib_Run();

	SpLib_TextureRelease(m_pTx);

	//Destroy
	SpLib_DestroyWin();

	return 0;
}