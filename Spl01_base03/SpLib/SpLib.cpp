//
//
////////////////////////////////////////////////////////////////////////////////


#pragma warning( disable : 4996)


#define IDR_MAIN_ACCEL                  113

#pragma comment(lib, "legacy_stdio_definitions.lib")
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#include <windows.h>

#include <d3d9.h>
#include <d3dx9.h>
#include <stdio.h>

#include "SpLib.h"


LRESULT CALLBACK WndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam);


int		RenderEnv();
int		FrameMove2D();
int		Render2D();



HINSTANCE			m_hInst;
HWND				m_hWnd;
char				m_sCls[512];

int					m_dScnX;
int					m_dScnY;
int					m_dScnW;
int					m_dScnH;

DWORD				m_dWinStyle	= WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU| WS_VISIBLE;
RECT				m_rcWindowBounds	;								// Saved window bounds for mode switches
RECT				m_rcWindowClient	;								// Saved client area size for mode switches

D3DDISPLAYMODE			m_d3dDM		;						// Display Mode
D3DPRESENT_PARAMETERS	m_d3dpp		;
LPDIRECT3D9			m_pD3D			;						// D3D
LPDIRECT3DDEVICE9	m_pd3dDevice	;						// Device
LPDIRECT3DSURFACE9	m_pBackBuffer	;						// Backbuffer point
LPD3DXSPRITE		m_pd3dSprite	;						// 2D Sprite

bool				m_bWindowed	= true		;				// Window mode?
bool				m_bActive	= true		;				// Is Active Window?
DWORD				m_dColor = 0xFF006699;					// D3D Clear Color


int (*SpLib_Render2D)();									// 렌더링 함수 포인터

void SpLib_SetRender( int (*_Render)() )					// 렌더링 함수 포인터 설정 함수
{
	SpLib_Render2D = _Render;
}


int SpLib_CreateWin(int x, int y, int width, int height, char* sName)
{

	strcpy(m_sCls, sName);

	m_dScnX = x;
	m_dScnY = y;
	m_dScnW = width;
	m_dScnH = height;


	m_hInst =(HINSTANCE)GetModuleHandle(NULL);

	WNDCLASS wc =
	{
		CS_CLASSDC | CS_DBLCLKS
		, (WNDPROC)WndProc
		, 0
		, 0
		, m_hInst
		, LoadIcon(NULL,IDI_APPLICATION)
		, LoadCursor(NULL,IDC_ARROW)
		, (HBRUSH)GetStockObject(WHITE_BRUSH)
		, NULL
		, m_sCls
	};

	RegisterClass(&wc);


	RECT rc;									//Create the application's window

	SetRect( &rc, 0, 0, m_dScnW, m_dScnH);
	AdjustWindowRect( &rc, m_dWinStyle, false );


	m_hWnd =CreateWindow( m_sCls, m_sCls
		, m_dWinStyle
		, m_dScnX
		, m_dScnY
		, (rc.right-rc.left)
		, (rc.bottom-rc.top)
		, NULL
		, NULL, m_hInst, NULL );


//	m_dWinStyle = GetWindowLong( m_hWnd, GWL_STYLE );
	GetWindowRect( m_hWnd, &m_rcWindowBounds );
	GetClientRect( m_hWnd, &m_rcWindowClient );


	// D3D생성
	if( NULL == ( m_pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) )
		return -1;


	// D3D를 통해서 화면 모드를 찾는다.
	if( FAILED( m_pD3D->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &m_d3dDM ) ) )
		return -1;


	// 디바이스를 생성하기 위해서는 프리젠트 파라메터 구조체가 필요
	// 전부 0으로 셋팅한다음 일부만 변수세팅

	memset(&m_d3dpp, 0, sizeof(m_d3dpp));

	m_d3dpp.Windowed				= m_bWindowed;
	m_d3dpp.SwapEffect				= D3DSWAPEFFECT_DISCARD;
	m_d3dpp.BackBufferFormat		= D3DFMT_X8R8G8B8;
	m_d3dpp.BackBufferCount			= 2;
	m_d3dpp.BackBufferWidth			= m_dScnW;
	m_d3dpp.BackBufferHeight		= m_dScnH;
	m_d3dpp.EnableAutoDepthStencil	= TRUE;
	m_d3dpp.AutoDepthStencilFormat	= D3DFMT_D16;
	m_d3dpp.Flags					= D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;

	// D3DADAPTER_DEFAULT: 대부분의 그래픽카드는 모노 듀얼일 경우 이부분을 수정
	// D3DDEVTYPE_HAL : 하드웨어 가속(가장 큰 속도)을 받을 것인가.. 하드웨어 지
	// 원이 없을 경우 D3D는 소프트웨어로 이를 대체 할 수 있다.

	if( FAILED( m_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING, &m_d3dpp, &m_pd3dDevice ) ) )
	{
		if( FAILED( m_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_hWnd,
			D3DCREATE_MIXED_VERTEXPROCESSING, &m_d3dpp, &m_pd3dDevice ) ) )
		{
			if( FAILED( m_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING, &m_d3dpp, &m_pd3dDevice ) ) )
			{
				m_pd3dDevice->Release();
				m_pD3D->Release();
				return -1;
			}
		}
	}

	// DX의 스프라이트는 디바이스가 생성된 후에 만들어야 한다.
	if(FAILED(D3DXCreateSprite(m_pd3dDevice, &m_pd3dSprite)))
	{
		m_pd3dDevice->Release();
		m_pD3D->Release();
		return -1;
	}


	return 0;
}


void SpLib_DestroyWin()
{
	if(m_pd3dSprite)
	{
		m_pd3dSprite->Release();
		m_pd3dSprite = NULL;
	}

	if(m_pd3dDevice)
	{
		m_pd3dDevice->Release();
		m_pd3dDevice = NULL;
	}

	if(m_pD3D)
	{
		m_pD3D->Release();
		m_pD3D = NULL;
	}
}


int	SpLib_Run()
{
	bool bGotMsg =false;
    HACCEL hAccel = LoadAccelerators( NULL, MAKEINTRESOURCE(IDR_MAIN_ACCEL) );

	MSG  msg;

	memset(&msg, 0, sizeof(MSG));
	PeekMessage( &msg, NULL, 0U, 0U, PM_NOREMOVE );

	while(WM_QUIT != msg.message)
	{
		if( m_bActive )
			bGotMsg = ( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) != 0 );
		else
			bGotMsg = ( GetMessage( &msg, NULL, 0U, 0U ) != 0 );

		if( bGotMsg )
		{
			if(0 == TranslateAccelerator( m_hWnd, hAccel, &msg ))
			{
				TranslateMessage( &msg );
				DispatchMessage( &msg );
			}
		}
		else
		{
			if( m_bActive )
			{
				if( FAILED( RenderEnv() ) )
					SendMessage( m_hWnd, WM_CLOSE, 0, 0 );
			}
		}
	}

	return 0;
}



HWND	SpLib_GetHwnd()		{	return m_hWnd;	}
int		SpLib_GetScnW()		{	return m_dScnW;	}
int		SpLib_GetScnH()		{	return m_dScnH;	}


void	SpLib_SetClearColor(unsigned long dC)
{
	m_dColor = dC;
}

DWORD	SpLib_GetClearColor()
{
	return m_dColor;
}








int	SpLib_TextureLoad(char* sFileName, LPDIRECT3DTEXTURE9& pTx, D3DXIMAGE_INFO* pImg, unsigned long dc)
{
	if(FAILED(D3DXCreateTextureFromFileEx(
		m_pd3dDevice
		, sFileName
		, D3DX_DEFAULT
		, D3DX_DEFAULT
		, 1
		, 0
		, D3DFMT_UNKNOWN
		, D3DPOOL_MANAGED
		, D3DX_FILTER_NONE
		, D3DX_FILTER_NONE
		, dc
		, pImg
		, NULL
		, &pTx
		)) )
	{
		printf("Create Texture Failed: %s\n", sFileName);
		pTx = NULL;
		return -1;
	}

	return 0;
}


void SpLib_TextureRelease(LPDIRECT3DTEXTURE9& pTx)
{
	if(pTx)
	{
		pTx->Release();
		pTx = NULL;
	}
}



void SpLib_SetWindowTitle(const char *format, ...)
{
	va_list ap;
	char s[512];

	if (format == NULL) return;

	va_start(ap, format);
	vsprintf((char *)s, format, ap);
	va_end(ap);

	if (s == NULL)	return;

	SetWindowText(m_hWnd, s);
}


void	SpLib_SetWindowStyle(DWORD dSty)	{	m_dWinStyle		= dSty;		}
DWORD	SpLib_GetWindowStyle()				{	return m_dWinStyle;			}


int SpLib_Draw2D(LPDIRECT3DTEXTURE9 pSrc, RECT* pSrcRect, D3DXVECTOR2* pTranslation, D3DXVECTOR2* pScaling, D3DXVECTOR2* pRot, FLOAT fAngle, unsigned long dC)
{
	int hr=-1;

	hr = m_pd3dSprite->Begin(D3DXSPRITE_ALPHABLEND);
// DX9.0b
//	m_pd3dSprite->Draw(pSrc, pSrcRect, pScaling, pRot, fAngle, pTranslation, dC);

// DX9.0c
	D3DXVECTOR3	vRot(0, 0, 0);
	D3DXVECTOR3	vTrans(0, 0, 0);

	if(pRot)
		vRot = D3DXVECTOR3(pRot->x, pRot->y, 0);

	if(pTranslation)
		vTrans = D3DXVECTOR3(pTranslation->x, pTranslation->y, 0);

	hr = m_pd3dSprite->Draw(pSrc, pSrcRect, &vRot, &vTrans, dC);

	if(FAILED(hr))
		return hr;

	return m_pd3dSprite->End();
}






////////////////////////////////////////////////////////////////////////////////
// Library 내부에서 사용하는 함수들

int RenderEnv()
{
	if( NULL == m_pd3dDevice )
		return -1;

	HRESULT hr;

	if(FAILED(FrameMove2D()))
		return -1;

	if(FAILED(Render2D()))
		return -1;

	hr = m_pd3dDevice->Present( NULL, NULL, NULL, NULL );


	return 0;
}

int FrameMove2D()
{
	return 0;
}



int Render2D()
{
	m_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, m_dColor, 1.0f, 0 );

	if( FAILED( m_pd3dDevice->BeginScene() ) )
		return -1;

	// 드라이버의 렌더링 함수 호출
	if( FAILED( SpLib_Render2D() ) )
	{
		m_pd3dDevice->EndScene();

		return 0;
	};

	m_pd3dDevice->EndScene();

	return 0;
}


LRESULT CALLBACK WndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	WPARAM wHi = HIWORD(wParam);
	WPARAM wLo = LOWORD(wParam);

	switch(uMsg)
	{
		case WM_CLOSE:
		case WM_DESTROY:
		{
			SpLib_DestroyWin();
			PostQuitMessage(0);
			return 0;
		}


		case WM_ACTIVATE:
		{
			if (WA_INACTIVE == wLo)
				m_bActive = false;
			else
				m_bActive = true;

			return 0;
		}
	}

	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}