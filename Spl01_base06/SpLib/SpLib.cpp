//
//
////////////////////////////////////////////////////////////////////////////////


#pragma warning( disable : 4996)


// 휠마우스를 쓰려면 이 구문은 반드시 필요.
#define _WIN32_WINNT 0x0400

#pragma comment(lib, "legacy_stdio_definitions.lib")
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "DxErr.lib")

#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxguid.lib")


#include <vector>

#include <windows.h>

#include <d3d9.h>
#include <d3dx9.h>
#include <DxErr.h>
#include <dsound.h>

#include <stdio.h>

#include "SpLib.h"

#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }



DWORD		m_nKeyTx=0;					// 텍스처 개체를 위한 키

struct McTexture
{
	DWORD				nKey;			// Key
	char				sFile[256];		// 파일 이름
	LPDIRECT3DTEXTURE9	pTx;			// 텍스처 포인터
	D3DXIMAGE_INFO		pImg;			// 텍스처 정보

	McTexture()
	{
		memset(sFile, 0, sizeof(sFile));
		pTx	= NULL;

		nKey = ++m_nKeyTx;
	}

	~McTexture()
	{
		if(	pTx	)	{	pTx->Release();		pTx = NULL;		}
	}
};


typedef std::vector<McTexture* >	lsPDTX;
typedef lsPDTX::iterator			itPDTX;


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
LPD3DXSPRITE		m_pd3dSprite;			// 2D Sprite

DWORD				m_dColor = 0xFF006699;

BYTE				m_KeyCur[256];
int					m_mouseX;
int					m_mouseY;
int					m_mouseZ;
int					m_mouseEvent;

DWORD				m_dTimeBgn=0;
DWORD				m_dTimeElapsed=0;

lsPDTX				m_vTx				;			// Texture


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



void SpLib_SetFrameMove(int (*_FrameMove)() )
{
	SpLib_FrameMove2D = _FrameMove;
}


void SpLib_SetRender(int (*_Render)() )
{
	SpLib_Render2D = _Render;
}



void SpLib_SetKeyboard(int (*_Keyboard)(BYTE* key) )
{
	SpLib_Keyboard	= _Keyboard;
}

void SpLib_SetMouse(int (*_Mouse)(int x, int y, int z, int _event) )
{
	SpLib_Mouse = _Mouse;
}




BYTE*	SpLib_GetKeyboard()		{	return m_KeyCur;			}
int		SpLib_GetMouseX()		{	return m_mouseX;			}
int		SpLib_GetMouseY()		{	return m_mouseY;			}
int		SpLib_GetMouseZ()		{	return m_mouseZ;			}

int		SpLib_GetMouseEvent(int nMouse)
{
	if(0 == nMouse)			return m_KeyCur[VK_LBUTTON];
	else if(1 == nMouse)	return m_KeyCur[VK_RBUTTON];
	else if(2 == nMouse)	return m_KeyCur[VK_MBUTTON];
	return 0;
}

DWORD	SpLib_ElapsedTime()		{	return m_dTimeElapsed;		}

DWORD	SpLib_GetWindowStyle()	{	return m_dWinStyle;			}
HWND	SpLib_GetHwnd()			{	return m_hWnd;				}
int		SpLib_GetScnW()			{	return m_dScnW;				}
int		SpLib_GetScnH()			{	return m_dScnH;				}

void	SpLib_SetWindowStyle(DWORD dSty)	{	m_dWinStyle		= dSty;		}
void	SpLib_SetClearColor(DWORD dC)		{	m_dColor = dC;				}
DWORD	SpLib_GetClearColor()				{	return m_dColor;			}



int	SpLib_TextureLoad(char* sFileName, DWORD dc)
{
	PDTX	pTx	= NULL;
	DIMG	pImg;

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
		, &pImg
		, NULL
		, &pTx
		)) )
	{
		printf("Create Texture Failed: %s\n", sFileName);
		pTx = NULL;
		return -1;
	}

	McTexture*	p = new McTexture;

	p->pTx = pTx;
	strcpy(p->sFile, sFileName);
	memcpy(&p->pImg, &pImg, sizeof(DIMG));

	m_vTx.push_back(p);

	// Key를 돌려 준다.
	return p->nKey;
}


int SpLib_TextureRelease(int _nKey)
{
	int iSize = m_vTx.size();
	int	nIdx=-1;

	for(int i=0; i<iSize; ++i)
	{
		if(m_vTx[i]->nKey == _nKey)
		{
			nIdx = i;
			break;
		}
	}

	if(nIdx<0 || nIdx>=iSize)
		return -1;


	lsPDTX::iterator	itTx;

	itTx = m_vTx.begin() + nIdx;
	SAFE_DELETE(	m_vTx[nIdx]	);
	m_vTx.erase(itTx);

	iSize = m_vTx.size();

	return iSize;
}

int SpLib_TextureWidth(int _nKey)
{
	int iSize = m_vTx.size();
	int	nIdx=-1;

	for(int i=0; i<iSize; ++i)
	{
		if(m_vTx[i]->nKey == _nKey)
		{
			nIdx = i;
			break;
		}
	}

	if(nIdx<0 || nIdx>=iSize)
		return -1;

	return (int)m_vTx[nIdx]->pImg.Width;
}


int SpLib_TextureHeight(int _nKey)
{
	int iSize = m_vTx.size();
	int	nIdx=-1;

	for(int i=0; i<iSize; ++i)
	{
		if(m_vTx[i]->nKey == _nKey)
		{
			nIdx = i;
			break;
		}
	}

	if(nIdx<0 || nIdx>=iSize)
		return -1;

	return (int)m_vTx[nIdx]->pImg.Height;
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



int SpLib_Draw2D(int _nKey
				  , RECT* pSrcRect
				  , VEC2* pTranslation
				  , VEC2* pScaling
				  , VEC2* pRotation
				  , FLOAT fAngle
				  , DWORD dC)
{
	int	hr=-1;

	int iSize = m_vTx.size();
	int	nIdx=-1;

	for(int i=0; i<iSize; ++i)
	{
		if(m_vTx[i]->nKey == _nKey)
		{
			nIdx = i;
			break;
		}
	}

	if(nIdx<0 || nIdx>=iSize)
	{
		return 0;
	}

	PDTX	pTx =  m_vTx[nIdx]->pTx;

//	m_pd3dSprite->Draw(pTx, pSrcRect, pScaling, pRotation, fAngle, pTranslation, dC);
	hr = m_pd3dSprite->Begin(D3DXSPRITE_ALPHABLEND);
	hr = m_pd3dSprite->Draw(pTx, pSrcRect, NULL, &D3DXVECTOR3(pTranslation->x, pTranslation->y, 0), dC);

	if(FAILED(hr))
		return hr;

	return m_pd3dSprite->End();
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

	return SpLib_FrameMove2D();
}


int Render3D()
{
	m_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, m_dColor, 1.0f, 0 );

	if( FAILED( m_pd3dDevice->BeginScene() ) )
		return -1;

	if( FAILED( SpLib_Render2D() ) )
	{
		m_pd3dDevice->EndScene();
		m_pd3dDevice->Present( 0, 0, 0, 0);
		return 0;
	};

	m_pd3dDevice->EndScene();

	return m_pd3dDevice->Present( 0, 0, 0, 0);
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


		case WM_LBUTTONDOWN	:
		{
			return 0;
		}
		case WM_LBUTTONUP:
		{
			return 0;
		}
		case WM_LBUTTONDBLCLK:
		{
			return 0;
		}


		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
	}

	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}





int SpLib_CreateWin(int x,int y,int ScnW,int ScnH,char* sName,bool bFull)
{
	SpLib_SetFrameMove( SpLib_DefaultFrameMove);
	SpLib_SetRender( SpLib_DefaultRender );
	SpLib_SetKeyboard( SpLib_DefaultKeyboard );
	SpLib_SetMouse( SpLib_DefaultMouse );

	strcpy(m_sCls, sName);

	m_dScnX = x;
	m_dScnY = y;
	m_dScnW = ScnW;
	m_dScnH = ScnH;

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


	// D3D생성
	if( NULL == ( m_pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) )
		return -1;

	// 디바이스를 생성하기 위해서는 프리젠트 파라메터 구조체가 필요
	// 전부 0으로 셋팅한다음 일부만 변수세팅

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory( &d3dpp, sizeof(d3dpp) );

	d3dpp.Windowed					= TRUE;
	d3dpp.SwapEffect				= D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat			= D3DFMT_X8R8G8B8;
	d3dpp.BackBufferCount			= 2;
	d3dpp.BackBufferWidth			= m_dScnW;
	d3dpp.BackBufferHeight			= m_dScnH;
	d3dpp.EnableAutoDepthStencil	= TRUE;
	d3dpp.AutoDepthStencilFormat	= D3DFMT_D16;
	d3dpp.Flags						= D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;		// Back Buffer를 이용하려면..

	// D3DADAPTER_DEFAULT: 대부분의 그래픽카드는 모노 듀얼일 경우 이부분을 수정
	// D3DDEVTYPE_HAL : 하드웨어 가속(가장 큰 속도)을 받을 것인가.. 하드웨어 지
	// 원이 없을 경우 D3D는 소프트웨어로 이를 대체 할 수 있다.

	if( FAILED( m_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_hWnd,
		D3DCREATE_MIXED_VERTEXPROCESSING, &d3dpp, &m_pd3dDevice ) ) )
	{
		if( FAILED( m_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &m_pd3dDevice ) ) )
		{
			m_pd3dDevice->Release();
			m_pD3D->Release();
			return -1;
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
	int i;
	int iSize = 0;

	iSize = m_vTx.size();

	for(i=0; i<iSize; ++i)
	{
		SAFE_DELETE(	m_vTx[i]	);
	}

	m_vTx.clear();



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
	int hr;

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
