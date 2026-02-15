

#pragma warning(disable: 4996)

//#pragma comment(lib, "legacy_stdio_definitions.lib")
#pragma comment(lib, "d3d9.lib")
//#pragma comment(lib, "d3dx9.lib")
//#pragma comment(lib, "DxErr.lib")
#pragma comment(lib, "dxguid.lib")

#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "dsound.lib")


#include <windows.h>
#include <stdio.h>

#include "ILcInput.h"
#include "ILcFont.h"
#include "ILcTexture.h"
#include "ILcSpriteX.h"
#include "ILcSound.h"
#include "ILcModel.h"

#include "DsUtil.h"
#include "LcsLib.h"


namespace LcsLib
{

int					m_bActive = TRUE;
int					m_bCursor = TRUE;
int					m_bState  = TRUE;

HINSTANCE			m_hInst;
HWND				m_hWnd;
char				m_sCls[512];

int					m_dScnX;
int					m_dScnY;
int					m_dScnW;
int					m_dScnH;

DWORD				m_dWinStyle	= WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU| WS_VISIBLE;

LPDIRECT3D9			m_pD3D		;			// D3D
LPDIRECT3DDEVICE9	m_pd3dDevice;			// Device
ILcSpriteX*			m_pSprite	;			// 2D Sprite

DWORD				m_dColor = 0xFF006699;
DWORD				m_dTimeBgn= ::timeGetTime();
DWORD				m_dTimeElapsed=0;


ILcInput*			m_pInput;

int (*LcsLib_FrameMove2D)();
int (*LcsLib_Render2D)();

int (*LcsLib_Keyboard)(unsigned char* key);
int (*LcsLib_Mouse)(int x, int y, int z, int _event);

}// namespace LcsLib



int LcsLib_DefaultFrameMove()	{	return 0;	}
int LcsLib_DefaultRender()		{	return 0;	}
int LcsLib_DefaultKeyboard(unsigned char*){	return 0;	}
int LcsLib_DefaultMouse(int, int, int, int){	return 0;	}

void LcsLib_SetFrameMove(int (*v)() )
{
	LcsLib::LcsLib_FrameMove2D = v;
}


void LcsLib_SetRender(int (*v)() )
{
	LcsLib::LcsLib_Render2D = v;
}



void LcsLib_SetKeyboard(int (*v)(unsigned char*) )
{
	LcsLib::LcsLib_Keyboard	= v;
}

void LcsLib_SetMouse(int (*v)(int, int, int, int) )
{
	LcsLib::LcsLib_Mouse = v;
}


const unsigned char*	LcsLib_GetKeyboard()
{
	return LcsLib::m_pInput->GetKeyMap();
}

int	LcsLib_GetMouseX()
{
	const FLOAT* vcPos = LcsLib::m_pInput->GetMousePos();
	return int(vcPos[0]);
}

int	LcsLib_GetMouseY()
{
	const FLOAT* vcPos = LcsLib::m_pInput->GetMousePos();
	return int(vcPos[1]);
}

int LcsLib_GetMouseZ()
{
	const FLOAT* vcPos = LcsLib::m_pInput->GetMousePos();
	return int(vcPos[2]);
}

int LcsLib_GetMouseEvent(int nMouse)
{
	int	nState = LcsLib::m_pInput->BtnState(nMouse);
	return nState;
}

DWORD	LcsLib_GetWindowStyle()	{	return LcsLib::m_dWinStyle;			}
HWND	LcsLib_GetHwnd()			{	return LcsLib::m_hWnd;				}
int		LcsLib_GetScnW()			{	return LcsLib::m_dScnW;				}
int		LcsLib_GetScnH()			{	return LcsLib::m_dScnH;				}


void	LcsLib_SetWindowStyle(DWORD dSty)	{	LcsLib::m_dWinStyle		= dSty;		}
void	LcsLib_SetStateShow(int _bShow)		{	LcsLib::m_bState		= _bShow;	}
void	LcsLib_SetCursorShow(int _bShow)
{
	LcsLib::m_bCursor		= _bShow;
	ShowCursor(LcsLib::m_bCursor);
}

void	LcsLib_SetClearColor(DWORD dC)		{	LcsLib::m_dColor = dC;				}
DWORD	LcsLib_GetClearColor()				{	return LcsLib::m_dColor;			}


void LcsLib_SetWindowTitle(const char *format, ...)
{
	va_list ap;
	char s[512];

	if (format == NULL) return;

	va_start(ap, format);
	vsprintf((char *)s, format, ap);
	va_end(ap);

	if (s == NULL)	return;

	SetWindowText(LcsLib::m_hWnd, s);
}



int LcsLib_Draw2D(int _nID
				  , RECT* pRct
				  , VEC2* pTrn
				  , VEC2* pScl
				  , VEC2* pRot
				  , FLOAT fRad
				  , DWORD dC
				  , int	bMono
				  )
{
	int	hr=-1;


	LcsLib::ILcTexture* pTex = LcsLib::LcDev_TextureFind(_nID);

	if(NULL == pTex)
		return 0;


	hr = LcsLib::m_pSprite->Begin();
	hr = LcsLib::m_pSprite->DrawEx(pTex, pRct, pScl, pTrn, pRot, fRad, dC, bMono);
	LcsLib::m_pSprite->End();

	return hr;
}


int LcsLib_DrawAlphaOption(int nAlphaMethod)
{
	if(NULL == LcsLib::m_pSprite)
		return -1;

	LcsLib::m_pSprite->AlphaOption(nAlphaMethod);
	return 0;
}



int FrameMove2D()
{
	LcsLib::m_dTimeElapsed = ::timeGetTime() - LcsLib::m_dTimeBgn;

	if(LcsLib::m_pInput)
		LcsLib::m_pInput->FrameMove();


	return LcsLib::LcsLib_FrameMove2D();
}


int Render3D()
{
	LcsLib::m_pd3dDevice->Clear( 0
						, NULL
						, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER
						, LcsLib::m_dColor
						, 1.0f, 0 );

	if( FAILED( LcsLib::m_pd3dDevice->BeginScene() ) )
		return -1;

	if( FAILED( LcsLib::LcsLib_Render2D() ) )
	{
		LcsLib::m_pd3dDevice->EndScene();
		LcsLib::m_pd3dDevice->Present( 0, 0, 0, 0);
		return 0;
	};

	LcsLib::m_pd3dDevice->EndScene();

	return LcsLib::m_pd3dDevice->Present( 0, 0, 0, 0);
}


LRESULT CALLBACK WndProc(HWND hWnd,unsigned int uMsg,WPARAM wParam,LPARAM lParam)
{
	WPARAM wHi = HIWORD(wParam);
	WPARAM wLo = LOWORD(wParam);


	if(LcsLib::m_pInput)
		LcsLib::m_pInput->MsgProc(hWnd, uMsg, wParam, lParam);

	switch(uMsg)
	{
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
	}

	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}





int LcsLib_CreateWin(int x,int y,int ScnW,int ScnH,char* sName,int bFull)
{
	LcsLib_SetFrameMove( LcsLib_DefaultFrameMove);
	LcsLib_SetRender( LcsLib_DefaultRender );
	LcsLib_SetKeyboard( LcsLib_DefaultKeyboard );
	LcsLib_SetMouse( LcsLib_DefaultMouse );

	strcpy(LcsLib::m_sCls, sName);

	LcsLib::m_dScnX = x;
	LcsLib::m_dScnY = y;
	LcsLib::m_dScnW = ScnW;
	LcsLib::m_dScnH = ScnH;

	LcsLib::m_hInst =(HINSTANCE)GetModuleHandle(NULL);

	WNDCLASS wc =
	{
		CS_CLASSDC | CS_DBLCLKS
		, (WNDPROC)WndProc
		, 0
		, 0
		, LcsLib::m_hInst
		, LoadIcon(NULL,IDI_APPLICATION)
		, LoadCursor(NULL,IDC_ARROW)
		, (HBRUSH)GetStockObject(WHITE_BRUSH)
		, NULL
		, LcsLib::m_sCls
	};

	RegisterClass(&wc);


	RECT rc;									//Create the application's window

	SetRect( &rc, 0, 0, LcsLib::m_dScnW, LcsLib::m_dScnH);
	AdjustWindowRect( &rc, LcsLib::m_dWinStyle, FALSE);


	LcsLib::m_hWnd =CreateWindow(
					LcsLib::m_sCls
					, LcsLib::m_sCls
					, LcsLib::m_dWinStyle
					, LcsLib::m_dScnX
					, LcsLib::m_dScnY
					, (rc.right-rc.left)
					, (rc.bottom-rc.top)
					, NULL
					, NULL
					, LcsLib::m_hInst
					, NULL );


	// D3D생성
	if( NULL == ( LcsLib::m_pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) )
		return -1;

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory( &d3dpp, sizeof(d3dpp) );

	d3dpp.Windowed					= TRUE;
	d3dpp.SwapEffect				= D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat			= D3DFMT_X8R8G8B8;
	d3dpp.BackBufferCount			= 2;
	d3dpp.BackBufferWidth			= LcsLib::m_dScnW;
	d3dpp.BackBufferHeight			= LcsLib::m_dScnH;
	d3dpp.EnableAutoDepthStencil	= TRUE;
	d3dpp.AutoDepthStencilFormat	= D3DFMT_D16;
	d3dpp.PresentationInterval		= D3DPRESENT_INTERVAL_IMMEDIATE;
	//d3dpp.Flags						= D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;

	if( FAILED( LcsLib::m_pD3D->CreateDevice( D3DADAPTER_DEFAULT
									, D3DDEVTYPE_HAL
									, LcsLib::m_hWnd
									, D3DCREATE_MIXED_VERTEXPROCESSING
									, &d3dpp
									, &LcsLib::m_pd3dDevice ) ) )
	{
		if( FAILED( LcsLib::m_pD3D->CreateDevice( D3DADAPTER_DEFAULT
										, D3DDEVTYPE_HAL
										, LcsLib::m_hWnd
										, D3DCREATE_SOFTWARE_VERTEXPROCESSING
										, &d3dpp, &LcsLib::m_pd3dDevice ) ) )
		{
			LcsLib::m_pd3dDevice->Release();
			LcsLib::m_pD3D->Release();
			return -1;
		}
	}

	// DX의 스프라이트는 디바이스가 생성된 후에 만들어야 한다.
	if(FAILED(LcsLib::LcDev_SpriteCreate(NULL, &LcsLib::m_pSprite, LcsLib::m_pd3dDevice)))
	{
		LcsLib::m_pd3dDevice->Release();
		LcsLib::m_pD3D->Release();
		return -1;
	}


	if(FAILED(LcsLib::LcDev_InputCreate(NULL, &LcsLib::m_pInput, LcsLib::m_hWnd)))
		return -1;

	LcsLib::LcDev_FontInit(LcsLib::m_pd3dDevice);
	LcsLib::LcDev_TextureInit(LcsLib::m_pd3dDevice);
	LcsLib::LcDev_SoundInit(LcsLib::m_hWnd);
	LcsLib::LcDev_ModelInit(LcsLib::m_pSprite);

	return 0;
}

int LcsLib_ChangeWindow(int bFull)
{
	return -1;
}

void LcsLib_DestroyWin()
{
	int i=0;

	LcsLib::LcDev_FontDestroy();
	LcsLib::LcDev_TextureDestroy();
	LcsLib::LcDev_SoundDestroy();
	LcsLib::LcDev_ModelDestroy();

	if(LcsLib::m_pInput)		{	delete LcsLib::m_pInput;			LcsLib::m_pInput = NULL;		}
	if(LcsLib::m_pSprite)		{	delete LcsLib::m_pSprite;			LcsLib::m_pSprite = NULL;	}
	if(LcsLib::m_pd3dDevice)	{	LcsLib::m_pd3dDevice->Release();	LcsLib::m_pd3dDevice = NULL;	}
	if(LcsLib::m_pD3D)			{	LcsLib::m_pD3D->Release();			LcsLib::m_pD3D = NULL;		}
}


int	LcsLib_Run()
{
	int hr;

	int bGotMsg = FALSE;
    MSG  msg;
    msg.message = WM_NULL;
    PeekMessage( &msg, NULL, 0U, 0U, PM_NOREMOVE );




	while( WM_QUIT != msg.message  )
    {
        if( LcsLib::m_bActive )
            bGotMsg = ( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) != 0 );
        else
            bGotMsg = ( GetMessage( &msg, NULL, 0U, 0U ) != 0 );

        if( bGotMsg )
        {
            TranslateMessage( &msg );
            DispatchMessage( &msg );
        }
        else
        {
			if(FAILED(hr = FrameMove2D()))
				break;

			if(FAILED(hr =Render3D()))
				break;
        }
    }

	return 0;
}




DWORD LcsLib_TimeGetTime()
{
	return ::timeGetTime() - LcsLib::m_dTimeBgn;
}