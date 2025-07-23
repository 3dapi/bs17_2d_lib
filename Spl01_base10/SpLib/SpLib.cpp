

#pragma warning(disable: 4996)

#pragma comment(lib, "legacy_stdio_definitions.lib")
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "DxErr.lib")
#pragma comment(lib, "dxguid.lib")

#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "dsound.lib")


#include <windows.h>
#include <stdio.h>

#include "ILcInput.h"
#include "ILcFont.h"
#include "ILcTexture.h"
#include "ILcSound.h"
#include "ILcModel.h"

#include "DsUtil.h"
#include "SpLib.h"



namespace SpLib
{
HINSTANCE			m_hInst;
HWND				m_hWnd;
char				m_sCls[512];

INT					m_dScnX;
INT					m_dScnY;
INT					m_dScnW;
INT					m_dScnH;

DWORD				m_dWinStyle	= WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU| WS_VISIBLE;

LPDIRECT3D9			m_pD3D		;			// D3D
LPDIRECT3DDEVICE9	m_pd3dDevice;			// Device
LPD3DXSPRITE		m_pd3dSprite;			// 2D Sprite

DWORD				m_dColor = 0xFF006699;
DWORD				m_dTimeBgn=0;
DWORD				m_dTimeElapsed=0;

ILcInput*			m_pInput;

INT (*SpLib_FrameMove2D)();
INT (*SpLib_Render2D)();

INT (*SpLib_Keyboard)(BYTE* key);
INT (*SpLib_Mouse)(INT x, INT y, INT z, INT _event);

}// namespace SpLib



INT SpLib_DefaultFrameMove()	{	return 0;	}
INT SpLib_DefaultRender()		{	return 0;	}
INT SpLib_DefaultKeyboard(BYTE*){	return 0;	}
INT SpLib_DefaultMouse(INT, INT, INT, INT){	return 0;	}

void SpLib_SetFrameMove(INT (*v)() )
{
	SpLib::SpLib_FrameMove2D = v;
}


void SpLib_SetRender(INT (*v)() )
{
	SpLib::SpLib_Render2D = v;
}



void SpLib_SetKeyboard(INT (*v)(BYTE*) )
{
	SpLib::SpLib_Keyboard	= v;
}

void SpLib_SetMouse(INT (*v)(INT, INT, INT, INT) )
{
	SpLib::SpLib_Mouse = v;
}


const BYTE*	SpLib_GetKeyboard()
{
	return SpLib::m_pInput->GetKeyMap();
}

INT	SpLib_GetMouseX()
{
	const FLOAT* vcPos = SpLib::m_pInput->GetMousePos();
	return INT(vcPos[0]);
}

INT	SpLib_GetMouseY()
{
	const FLOAT* vcPos = SpLib::m_pInput->GetMousePos();
	return INT(vcPos[1]);
}

INT SpLib_GetMouseZ()
{
	const FLOAT* vcPos = SpLib::m_pInput->GetMousePos();
	return INT(vcPos[2]);
}

INT SpLib_GetMouseEvent(INT nMouse)
{
	INT	nState = SpLib::m_pInput->BtnState(nMouse);
	return nState;
}

DWORD	SpLib_GetWindowStyle()	{	return SpLib::m_dWinStyle;			}
HWND	SpLib_GetHwnd()			{	return SpLib::m_hWnd;				}
INT		SpLib_GetScnW()			{	return SpLib::m_dScnW;				}
INT		SpLib_GetScnH()			{	return SpLib::m_dScnH;				}


void	SpLib_SetWindowStyle(DWORD dSty)	{	SpLib::m_dWinStyle		= dSty;		}
void	SpLib_SetClearColor(DWORD dC)		{	SpLib::m_dColor = dC;				}
DWORD	SpLib_GetClearColor()				{	return SpLib::m_dColor;			}


void SpLib_SetWindowTitle(const char *format, ...)
{
	va_list ap;
	char s[512];

	if (format == NULL) return;

	va_start(ap, format);
	vsprintf((char *)s, format, ap);
	va_end(ap);

	if (s == NULL)	return;

	SetWindowText(SpLib::m_hWnd, s);
}



INT SpLib_Draw2D(INT _nID
				  , RECT* pSrcRect
				  , VEC2* pTranslation
				  , VEC2* pScaling
				  , VEC2* pRotation
				  , FLOAT fAngle
				  , DWORD dC)
{
	INT	hr=-1;


	SpLib::ILcTexture* pTex = SpLib::LcDev_TextureFind(_nID);

	if(NULL == pTex)
		return 0;

	PDTX pTx =  (PDTX)pTex->GetTexture();

//	m_pd3dSprite->Draw(pTx, pSrcRect, pScaling, pRotation, fAngle, pTranslation, dC);
	hr = SpLib::m_pd3dSprite->Begin(D3DXSPRITE_ALPHABLEND);
	hr = SpLib::m_pd3dSprite->Draw(pTx, pSrcRect, NULL, &D3DXVECTOR3(pTranslation->x, pTranslation->y, 0), dC);

	if(FAILED(hr))
		return hr;

	return SpLib::m_pd3dSprite->End();
}


INT FrameMove2D()
{
	SpLib::m_dTimeElapsed = ::timeGetTime() - SpLib::m_dTimeBgn;

	if(SpLib::m_pInput)
		SpLib::m_pInput->FrameMove();


	return SpLib::SpLib_FrameMove2D();
}


INT Render3D()
{
	SpLib::m_pd3dDevice->Clear( 0
						, NULL
						, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER
						, SpLib::m_dColor
						, 1.0f, 0 );

	if( FAILED( SpLib::m_pd3dDevice->BeginScene() ) )
		return -1;

	if( FAILED( SpLib::SpLib_Render2D() ) )
	{
		SpLib::m_pd3dDevice->EndScene();
		SpLib::m_pd3dDevice->Present( 0, 0, 0, 0);
		return 0;
	};

	SpLib::m_pd3dDevice->EndScene();

	return SpLib::m_pd3dDevice->Present( 0, 0, 0, 0);
}


LRESULT CALLBACK WndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	WPARAM wHi = HIWORD(wParam);
	WPARAM wLo = LOWORD(wParam);


	if(SpLib::m_pInput)
		SpLib::m_pInput->MsgProc(hWnd, uMsg, wParam, lParam);

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





INT SpLib_CreateWin(INT x,INT y,INT ScnW,INT ScnH,char* sName,bool bFull)
{
	SpLib_SetFrameMove( SpLib_DefaultFrameMove);
	SpLib_SetRender( SpLib_DefaultRender );
	SpLib_SetKeyboard( SpLib_DefaultKeyboard );
	SpLib_SetMouse( SpLib_DefaultMouse );

	strcpy(SpLib::m_sCls, sName);

	SpLib::m_dScnX = x;
	SpLib::m_dScnY = y;
	SpLib::m_dScnW = ScnW;
	SpLib::m_dScnH = ScnH;

	SpLib::m_hInst =(HINSTANCE)GetModuleHandle(NULL);

	WNDCLASS wc =
	{
		CS_CLASSDC | CS_DBLCLKS
		, (WNDPROC)WndProc
		, 0
		, 0
		, SpLib::m_hInst
		, LoadIcon(NULL,IDI_APPLICATION)
		, LoadCursor(NULL,IDC_ARROW)
		, (HBRUSH)GetStockObject(WHITE_BRUSH)
		, NULL
		, SpLib::m_sCls
	};

	RegisterClass(&wc);


	RECT rc;									//Create the application's window

	SetRect( &rc, 0, 0, SpLib::m_dScnW, SpLib::m_dScnH);
	AdjustWindowRect( &rc, SpLib::m_dWinStyle, FALSE);


	SpLib::m_hWnd =CreateWindow(
					SpLib::m_sCls
					, SpLib::m_sCls
					, SpLib::m_dWinStyle
					, SpLib::m_dScnX
					, SpLib::m_dScnY
					, (rc.right-rc.left)
					, (rc.bottom-rc.top)
					, NULL
					, NULL
					, SpLib::m_hInst
					, NULL );


	// D3D생성
	if( NULL == ( SpLib::m_pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) )
		return -1;

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory( &d3dpp, sizeof(d3dpp) );

	d3dpp.Windowed					= TRUE;
	d3dpp.SwapEffect				= D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat			= D3DFMT_X8R8G8B8;
	d3dpp.BackBufferCount			= 2;
	d3dpp.BackBufferWidth			= SpLib::m_dScnW;
	d3dpp.BackBufferHeight			= SpLib::m_dScnH;
	d3dpp.EnableAutoDepthStencil	= TRUE;
	d3dpp.AutoDepthStencilFormat	= D3DFMT_D16;
	d3dpp.PresentationInterval		= D3DPRESENT_INTERVAL_IMMEDIATE;
	d3dpp.Flags						= D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;

	if( FAILED( SpLib::m_pD3D->CreateDevice( D3DADAPTER_DEFAULT
									, D3DDEVTYPE_HAL
									, SpLib::m_hWnd
									, D3DCREATE_MIXED_VERTEXPROCESSING
									, &d3dpp
									, &SpLib::m_pd3dDevice ) ) )
	{
		if( FAILED( SpLib::m_pD3D->CreateDevice( D3DADAPTER_DEFAULT
										, D3DDEVTYPE_HAL
										, SpLib::m_hWnd
										, D3DCREATE_SOFTWARE_VERTEXPROCESSING
										, &d3dpp, &SpLib::m_pd3dDevice ) ) )
		{
			SpLib::m_pd3dDevice->Release();
			SpLib::m_pD3D->Release();
			return -1;
		}
	}

	// DX의 스프라이트는 디바이스가 생성된 후에 만들어야 한다.
	if(FAILED(D3DXCreateSprite(SpLib::m_pd3dDevice, &SpLib::m_pd3dSprite)))
	{
		SpLib::m_pd3dDevice->Release();
		SpLib::m_pD3D->Release();
		return -1;
	}


	if(FAILED(SpLib::LcDev_InputCreate(NULL, &SpLib::m_pInput, SpLib::m_hWnd)))
		return -1;

	SpLib::LcDev_FontInit(SpLib::m_pd3dDevice);
	SpLib::LcDev_TextureInit(SpLib::m_pd3dDevice);
	SpLib::LcDev_SoundInit(SpLib::m_hWnd);
	SpLib::LcDev_ModelInit(SpLib::m_pd3dSprite);

	return 0;
}


void SpLib_DestroyWin()
{
	INT i=0;
	
	SpLib::LcDev_FontDestroy();
	SpLib::LcDev_TextureDestroy();
	SpLib::LcDev_SoundDestroy();
	SpLib::LcDev_ModelDestroy();

	if(SpLib::m_pInput)		{	delete SpLib::m_pInput;			SpLib::m_pInput = NULL;		}
	if(SpLib::m_pd3dSprite)	{	SpLib::m_pd3dSprite->Release();	SpLib::m_pd3dSprite = NULL;	}
	if(SpLib::m_pd3dDevice)	{	SpLib::m_pd3dDevice->Release();	SpLib::m_pd3dDevice = NULL;	}
	if(SpLib::m_pD3D)		{	SpLib::m_pD3D->Release();		SpLib::m_pD3D = NULL;		}
}


INT	SpLib_Run()
{
	INT hr;

	bool bGotMsg =false;
    MSG  msg;
    msg.message = WM_NULL;
    PeekMessage( &msg, NULL, 0U, 0U, PM_NOREMOVE );

	bool m_bActive = true;


	while( WM_QUIT != msg.message  )
    {
        if( m_bActive )
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


