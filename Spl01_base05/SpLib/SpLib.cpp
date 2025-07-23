//
//
////////////////////////////////////////////////////////////////////////////////


#pragma warning( disable : 4996)


#define IDM_TOGGLEFULLSCREEN            40003
#define IDR_MAIN_ACCEL                  113

// 휠마우스를 쓰려면 이 구문은 반드시 필요.
#define _WIN32_WINNT 0x0400

#include <vector>

#pragma comment(lib, "legacy_stdio_definitions.lib")
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "dsound.lib")

#include <windows.h>

#include <d3d9.h>
#include <d3dx9.h>
#include <stdio.h>

#include "SpLib.h"


LRESULT CALLBACK WndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam);


int		RenderEnv();
int		ResetEnv();
int		FrameMove2D();
int		Render2D();
void	UpdateFrame();



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
float				m_fFps=60.f;

DWORD				m_dColor = 0xFF006699;					// D3D Clear Color

BYTE				m_KeyCur[256];
int					m_mouseX;
int					m_mouseY;
int					m_mouseZ;
int					m_mouseEvent;

DWORD				m_dTimeBgn=0;
DWORD				m_dTimeElapsed=0;

int (*SpLib_FrameMove2D)();
int (*SpLib_Render2D)();

int (*SpLib_Keyboard)(BYTE* key);
int (*SpLib_Mouse)(int x, int y, int z, int _event);





int SpLib_DefaultFrameMove()
{
	//printf("SpLib_Default FrameMove\n");
	return 0;
}

int SpLib_DefaultRender()
{
	//printf("SpLib_Default Render\n");
	return 0;
}

int SpLib_DefaultKeyboard(BYTE* key)
{
	//printf("SpLib_Default Keyboard\n");
	return 0;
}

int SpLib_DefaultMouse(int x, int y, int z, int _event)
{
	//printf("SpLib_Default Mouse\n");
	return 0;
}



void SpLib_SetFrameMove( int (*_FrameMove)() )
{
	SpLib_FrameMove2D = _FrameMove;
}


void SpLib_SetRender( int (*_Render)() )
{
	SpLib_Render2D = _Render;
}



void SpLib_SetKeyboard( int (*_Keyboard)(BYTE* key) )
{
	SpLib_Keyboard	= _Keyboard;
}

void SpLib_SetMouse( int (*_Mouse)(int x, int y, int z, int _event) )
{
	SpLib_Mouse = _Mouse;
}










int SpLib_CreateWin(int x, int y, int width, int height, char* sName)
{
	SpLib_SetFrameMove( SpLib_DefaultFrameMove);
	SpLib_SetRender( SpLib_DefaultRender );
	SpLib_SetKeyboard( SpLib_DefaultKeyboard );
	SpLib_SetMouse( SpLib_DefaultMouse );

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


	BYTE	sKey[256]={0};
	::SetKeyboardState(sKey);

	m_dTimeBgn = timeGetTime();

	return 0;
}


void SpLib_DestroyWin()
{
	if(m_pd3dSprite)
	{
		m_pd3dSprite->Release();
		m_pd3dSprite = NULL;
	}

	if(m_pBackBuffer)
	{
		m_pBackBuffer->Release();
		m_pBackBuffer = NULL;
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
			if( FAILED( RenderEnv() ) )
				SendMessage( m_hWnd, WM_CLOSE, 0, 0 );
		}
	}

	if( hAccel != NULL )
		DestroyAcceleratorTable( hAccel );

	return 0;
}


BYTE* SpLib_GetKeyboard()
{
	return m_KeyCur;
}


int		SpLib_GetMouseX()
{
	return m_mouseX;
}


int		SpLib_GetMouseY()
{
	return m_mouseY;
}


int		SpLib_GetMouseZ()
{
	return m_mouseZ;
}


int		SpLib_GetMouseEvent(int nMouse)
{
	if(0 == nMouse)			return m_KeyCur[VK_LBUTTON];
	else if(1 == nMouse)	return m_KeyCur[VK_RBUTTON];
	else if(2 == nMouse)	return m_KeyCur[VK_MBUTTON];
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


DWORD	SpLib_ElapsedTime()
{
	m_dTimeElapsed = timeGetTime() - m_dTimeBgn;
	return m_dTimeElapsed;
}







int	SpLib_TextureLoad(char* sFileName, PDTX& pTx, DIMG* pImg, unsigned long dc)
{
	if(FAILED(D3DXCreateTextureFromFileEx(
		m_pd3dDevice
		, sFileName
		, D3DX_DEFAULT
		, D3DX_DEFAULT
		, D3DX_DEFAULT
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


void SpLib_TextureRelease(PDTX& pTx)
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


int  SpLib_Draw2D(PDTX pSrc, RECT* pSrcRect, VEC2* pTranslation, VEC2* pScaling, VEC2* pRot, FLOAT fAngle, unsigned long dC)
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

	UpdateFrame();

	hr = m_pd3dDevice->Present( NULL, NULL, NULL, NULL );

	return 0;
}

int FrameMove2D()
{
	m_dTimeElapsed = timeGetTime() - m_dTimeBgn;

	POINT mouse;

	memset(m_KeyCur, 0, sizeof(m_KeyCur));

	::GetKeyboardState(m_KeyCur);


	for(int i=0; i<256; ++i)
		m_KeyCur[i] = (m_KeyCur[i] & 0x80);

	::GetCursorPos(&mouse);
	::ScreenToClient(m_hWnd, &mouse );

	m_mouseX = mouse.x;
	m_mouseY = mouse.y;

	SpLib_Keyboard(m_KeyCur);
	SpLib_Mouse(m_mouseX, m_mouseY, m_mouseZ, 0);

	return SpLib_FrameMove2D();
}


int Render2D()
{
	m_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, m_dColor, 1.0f, 0 );

	if( FAILED( m_pd3dDevice->BeginScene() ) )
		return -1;

	if( FAILED( SpLib_Render2D() ) )
	{
		m_pd3dDevice->EndScene();

		return 0;
	};

	m_pd3dDevice->EndScene();

	return 0;
}


int ResetEnv()
{
	if(	m_pd3dSprite	)
		m_pd3dSprite->OnLostDevice();

	if(m_pBackBuffer)
	{
		m_pBackBuffer->Release();
		m_pBackBuffer = NULL;
	}


	if(FAILED(m_pd3dDevice->Reset(&m_d3dpp)))
		return -1;


	if(	m_pd3dSprite	)
		m_pd3dSprite->OnResetDevice();


	if(FAILED(m_pd3dDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &m_pBackBuffer )))
		return -1;

	Sleep(100);

	return 0;
}



void UpdateFrame()
{
	static int		iCnt =0;
	static DWORD	dB = timeGetTime();
	DWORD			dE = timeGetTime();

	++iCnt;

	if(iCnt>12)																	// 30Frame을 기준
	{
		m_fFps = FLOAT(dE-dB);
		m_fFps = iCnt*1000/m_fFps;

		iCnt = 0;
		dB = dE;
	}
}


LRESULT CALLBACK WndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	WPARAM wHi = HIWORD(wParam);
	WPARAM wLo = LOWORD(wParam);

	switch(uMsg)
	{
		case WM_MOUSEWHEEL:
		{
			m_mouseZ += short( wHi);

			return 0;
		}

//		case WM_LBUTTONDOWN	:
//		{
//			m_pInput->SetMouseSt(0, true);
//			return 0;
//		}
//		case WM_LBUTTONUP:
//		{
//			m_pInput->SetMouseSt(1, true);
//			return 0;
//		}
//		case WM_LBUTTONDBLCLK:
//		{
//			m_pInput->SetMouseSt(2, true);
//			return 0;
//		}

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



		case WM_COMMAND:
		{
			switch( wLo )
			{
				case IDM_TOGGLEFULLSCREEN:
				{
					m_bWindowed ^=1;
					m_d3dpp.Windowed					= m_bWindowed;
					m_d3dpp.FullScreen_RefreshRateInHz	= m_d3dpp.Windowed? 0 : m_d3dDM.RefreshRate;

					ResetEnv();

					if(m_bWindowed)
					{
						SetWindowLong(m_hWnd, GWL_STYLE, m_dWinStyle);
						SetWindowPos(m_hWnd, HWND_NOTOPMOST
							, m_rcWindowBounds.left
							, m_rcWindowBounds.top
							, m_rcWindowClient.right	- m_rcWindowClient.left
							, m_rcWindowClient.bottom	- m_rcWindowClient.top
							, SWP_SHOWWINDOW);
					}
					else
					{
						SetWindowLong( m_hWnd, GWL_STYLE, WS_POPUP|WS_SYSMENU|WS_VISIBLE );
					}

					return 0;
				}
			}

			break;
		}// WM_COMMAND
	}

	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}