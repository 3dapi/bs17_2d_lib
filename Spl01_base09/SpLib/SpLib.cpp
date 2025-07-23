
#pragma warning(disable: 4996)


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

#include <stdio.h>

#include "DsUtil.h"

#include "SpLib.h"

#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }



namespace SpLib
{

DWORD		m_nKeyTx=0;			// Texture
DWORD		m_nKeySnd=0;		// Sound


struct McTexture
{
	DWORD				nKey;
	char				sFile[256];
	LPDIRECT3DTEXTURE9	pTx;
	DIMG				pImg;

	McTexture()
	{
		memset(sFile, 0, sizeof(sFile));
		pTx	= NULL;

		nKey = ++m_nKeyTx;
	}

	~McTexture()
	{
		if(pTx)	{ pTx->Release(); pTx = NULL; }
	}
};


struct McSound
{
	DWORD		nKey;
	char		sFile[256];
	CSound*		pSnd;

	McSound()
	{
		memset(sFile, 0, sizeof(sFile));
		pSnd = NULL;

		nKey = ++m_nKeySnd;
	}

	~McSound()
	{
		if(pSnd){ delete pSnd; pSnd = NULL; }
	}
};


typedef std::vector<McTexture* >	lsPDTX;
typedef lsPDTX::iterator			itPDTX;


typedef std::vector<ID3DXFont* >	lsDxFont;
typedef lsDxFont::iterator			itDxFont;


typedef std::vector<McSound* >		lsMcSound;
typedef	lsMcSound::iterator			itMcSound;


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
lsDxFont			m_vFont				;			// Font

CSoundManager*		m_pSndMn			;			// DirectSound manager class
lsMcSound			m_vSound			;			// Sound List


int (*SpLib_FrameMove2D)();
int (*SpLib_Render2D)();


int (*SpLib_Keyboard)(BYTE* key);
int (*SpLib_Mouse)(int x, int y, int z, int _event);

}// namespace SpLib



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
	SpLib::SpLib_FrameMove2D = _FrameMove;
}


void SpLib_SetRender(int (*_Render)() )
{
	SpLib::SpLib_Render2D = _Render;
}



void SpLib_SetKeyboard(int (*_Keyboard)(BYTE* key) )
{
	SpLib::SpLib_Keyboard	= _Keyboard;
}

void SpLib_SetMouse(int (*_Mouse)(int x, int y, int z, int _event) )
{
	SpLib::SpLib_Mouse = _Mouse;
}




BYTE*	SpLib_GetKeyboard()		{	return SpLib::m_KeyCur;			}
int		SpLib_GetMouseX()		{	return SpLib::m_mouseX;			}
int		SpLib_GetMouseY()		{	return SpLib::m_mouseY;			}
int		SpLib_GetMouseZ()		{	return SpLib::m_mouseZ;			}

int		SpLib_GetMouseEvent(int nMouse)
{
	if(0 == nMouse)			return SpLib::m_KeyCur[VK_LBUTTON];
	else if(1 == nMouse)	return SpLib::m_KeyCur[VK_RBUTTON];
	else if(2 == nMouse)	return SpLib::m_KeyCur[VK_MBUTTON];
	return 0;
}

DWORD	SpLib_GetWindowStyle()	{	return SpLib::m_dWinStyle;			}
HWND	SpLib_GetHwnd()			{	return SpLib::m_hWnd;				}
int		SpLib_GetScnW()			{	return SpLib::m_dScnW;				}
int		SpLib_GetScnH()			{	return SpLib::m_dScnH;				}


void	SpLib_SetWindowStyle(DWORD dSty)	{	SpLib::m_dWinStyle		= dSty;		}
void	SpLib_SetClearColor(DWORD dC)		{	SpLib::m_dColor = dC;				}
DWORD	SpLib_GetClearColor()				{	return SpLib::m_dColor;			}



int	SpLib_TextureLoad(char* sFileName, DWORD dc)
{
	PDTX	pTx	= NULL;
	DIMG	pImg;

	if(FAILED(D3DXCreateTextureFromFileEx(
		SpLib::m_pd3dDevice
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

	SpLib::McTexture*	p = new SpLib::McTexture;

	p->pTx = pTx;
	strcpy(p->sFile, sFileName);
	memcpy(&p->pImg, &pImg, sizeof(DIMG));

	SpLib::m_vTx.push_back(p);

	// Key를 돌려 준다.
	return p->nKey;
}


int SpLib_TextureRelease(int _nKey)
{
	int iSize = SpLib::m_vTx.size();
	int	nIdx=-1;

	for(int i=0; i<iSize; ++i)
	{
		if(SpLib::m_vTx[i]->nKey == _nKey)
		{
			nIdx = i;
			break;
		}
	}

	if(nIdx<0 || nIdx>=iSize)
		return -1;


	SpLib::lsPDTX::iterator	itTx;

	itTx = SpLib::m_vTx.begin() + nIdx;
	SAFE_DELETE(	SpLib::m_vTx[nIdx]	);
	SpLib::m_vTx.erase(itTx);

	iSize = SpLib::m_vTx.size();

	return iSize;
}

int SpLib_TextureWidth(int _nKey)
{
	int iSize = SpLib::m_vTx.size();
	int	nIdx=-1;

	for(int i=0; i<iSize; ++i)
	{
		if(SpLib::m_vTx[i]->nKey == _nKey)
		{
			nIdx = i;
			break;
		}
	}

	if(nIdx<0 || nIdx>=iSize)
		return -1;

	return (int)SpLib::m_vTx[nIdx]->pImg.Width;
}


int SpLib_TextureHeight(int _nKey)
{
	int iSize = SpLib::m_vTx.size();
	int	nIdx=-1;

	for(int i=0; i<iSize; ++i)
	{
		if(SpLib::m_vTx[i]->nKey == _nKey)
		{
			nIdx = i;
			break;
		}
	}

	if(nIdx<0 || nIdx>=iSize)
		return -1;

	return (int)SpLib::m_vTx[nIdx]->pImg.Height;
}


int		SpLib_FontCreate(char* sName, LONG iH, BYTE iItalic)
{
	ID3DXFont*	pD3DXFont;

	D3DXFONT_DESC hFont =
	{
		iH, 0
		, FW_NORMAL, 1
		, iItalic
		, HANGUL_CHARSET, OUT_DEFAULT_PRECIS
		, ANTIALIASED_QUALITY, FF_DONTCARE, "Arial"
	};

	strcpy(hFont.FaceName, sName);

    if( FAILED(D3DXCreateFontIndirect( SpLib::m_pd3dDevice, &hFont, &pD3DXFont ) ) )
        return -1;

	SpLib::m_vFont.push_back(	pD3DXFont);

	return (SpLib::m_vFont.size()-1);
}


int 	SpLib_FontDrawText(int nIdx
						   , LONG lLeft
						   , LONG lTop
						   , LONG lRight
						   , LONG lBottom
						   , DWORD fontColor
						   , const char *format, ...)
{
	int iSize = SpLib::m_vFont.size();

	if(nIdx<0 || nIdx>=iSize)
		return -1;


	va_list ap;
	char s[1024];

	if (format == NULL)
		return -1;

	va_start(ap, format);
	vsprintf((char *)s, format, ap);
	va_end(ap);

	if (s == NULL)
		return -2;

	ID3DXFont*	pD3DXFont = SpLib::m_vFont[nIdx];

    RECT rc;
	rc.left		= lLeft;
	rc.top		= lTop;
    rc.right	= lRight+20;
	rc.bottom	= lBottom;

    return pD3DXFont->DrawText(NULL, s, -1, &rc, 0, fontColor );
}


int SpLib_SoundLoad(char* sFileName)
{
	CSound*	pSnd = NULL;

	if(FAILED(SpLib::m_pSndMn->Create( &pSnd, sFileName, 0, GUID_NULL, 1 )))
		return -1;

	if(NULL ==pSnd)
		return -1;

	SpLib::McSound* p = new SpLib::McSound;

	p->pSnd = pSnd;
	strcpy(p->sFile, sFileName);

	SpLib::m_vSound.push_back(p);

	// Key를 돌려 준다.
	return p->nKey;
}


int SpLib_SoundRelease(int _nKey)
{
	int iSize = SpLib::m_vSound.size();
	int	nIdx=-1;

	for(int i=0; i<iSize; ++i)
	{
		if(SpLib::m_vSound[i]->nKey == _nKey)
		{
			nIdx = i;
			break;
		}
	}

	if(nIdx<0 || nIdx>=iSize)
		return -1;


	SpLib::lsMcSound::iterator	itSound;

	itSound = SpLib::m_vSound.begin() + nIdx;
	SAFE_DELETE( SpLib::m_vSound[nIdx]		);
	SpLib::m_vSound.erase(itSound);

	iSize = SpLib::m_vSound.size();

	return iSize;
}




void	SpLib_SoundPlay(int _nKey)
{
	int iSize = SpLib::m_vSound.size();
	int	nIdx=-1;

	for(int i=0; i<iSize; ++i)
	{
		if(SpLib::m_vSound[i]->nKey == _nKey)
		{
			nIdx = i;
			break;
		}
	}

	if(nIdx<0 || nIdx>=iSize)
		return;

	SpLib::m_vSound[nIdx]->pSnd->Play();
}

void	SpLib_SoundStop(int _nKey)
{
	int iSize = SpLib::m_vSound.size();
	int	nIdx=-1;

	for(int i=0; i<iSize; ++i)
	{
		if(SpLib::m_vSound[i]->nKey == _nKey)
		{
			nIdx = i;
			break;
		}
	}

	if(nIdx<0 || nIdx>=iSize)
		return;

	SpLib::m_vSound[nIdx]->pSnd->Stop();
}

void	SpLib_SoundReset(int _nKey)
{
	int iSize = SpLib::m_vSound.size();
	int	nIdx=-1;

	for(int i=0; i<iSize; ++i)
	{
		if(SpLib::m_vSound[i]->nKey == _nKey)
		{
			nIdx = i;
			break;
		}
	}

	if(nIdx<0 || nIdx>=iSize)
		return;

	SpLib::m_vSound[nIdx]->pSnd->Reset();
}

BOOL	SpLib_SoundIsPlaying(int _nKey)
{
	int iSize = SpLib::m_vSound.size();
	int	nIdx=-1;

	for(int i=0; i<iSize; ++i)
	{
		if(SpLib::m_vSound[i]->nKey == _nKey)
		{
			nIdx = i;
			break;
		}
	}

	if(nIdx<0 || nIdx>=iSize)
		return 0;

	return SpLib::m_vSound[nIdx]->pSnd->IsSoundPlaying();
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

	SetWindowText(SpLib::m_hWnd, s);
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

	int iSize = SpLib::m_vTx.size();
	int	nIdx=-1;

	for(int i=0; i<iSize; ++i)
	{
		if(SpLib::m_vTx[i]->nKey == _nKey)
		{
			nIdx = i;
			break;
		}
	}

	if(nIdx<0 || nIdx>=iSize)
	{
		return 0;
	}

	PDTX	pTx =  SpLib::m_vTx[nIdx]->pTx;

//	m_pd3dSprite->Draw(pTx, pSrcRect, pScaling, pRotation, fAngle, pTranslation, dC);
	hr = SpLib::m_pd3dSprite->Begin(D3DXSPRITE_ALPHABLEND);
	hr = SpLib::m_pd3dSprite->Draw(pTx, pSrcRect, NULL, &D3DXVECTOR3(pTranslation->x, pTranslation->y, 0), dC);

	if(FAILED(hr))
		return hr;

	return SpLib::m_pd3dSprite->End();
}


int FrameMove2D()
{
	SpLib::m_dTimeElapsed = ::timeGetTime() - SpLib::m_dTimeBgn;

	POINT mouse;

	memset(SpLib::m_KeyCur, 0, sizeof(SpLib::m_KeyCur));

	::GetKeyboardState(SpLib::m_KeyCur);


	for(int i=0; i<256; ++i)
		SpLib::m_KeyCur[i] = (SpLib::m_KeyCur[i] & 0x80);

	::GetCursorPos(&mouse);
	::ScreenToClient(SpLib::m_hWnd, &mouse );

	SpLib::m_mouseX = mouse.x;
	SpLib::m_mouseY = mouse.y;

	return SpLib::SpLib_FrameMove2D();
}


int Render3D()
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

	switch(uMsg)
	{
		case WM_MOUSEWHEEL:
		{
			SpLib::m_mouseZ += short( wHi);

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

	// 디바이스를 생성하기 위해서는 프리젠트 파라메터 구조체가 필요
	// 전부 0으로 셋팅한다음 일부만 변수세팅

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
	d3dpp.Flags						= D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;		// Back Buffer를 이용하려면..

	// D3DADAPTER_DEFAULT: 대부분의 그래픽카드는 모노 듀얼일 경우 이부분을 수정
	// D3DDEVTYPE_HAL : 하드웨어 가속(가장 큰 속도)을 받을 것인가.. 하드웨어 지
	// 원이 없을 경우 D3D는 소프트웨어로 이를 대체 할 수 있다.

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


	BYTE	sKey[256]={0};
	::SetKeyboardState(sKey);

	SpLib::m_pSndMn = new CSoundManager;


	if( FAILED(SpLib::m_pSndMn->Initialize( SpLib::m_hWnd, DSSCL_PRIORITY ) ) )
	{
		MessageBox( SpLib::m_hWnd
					, "Sound Manager Initialize Failed"
					, "Err"
					, MB_ICONEXCLAMATION);
		return -1;
	}

	if( FAILED( SpLib::m_pSndMn->SetPrimaryBufferFormat( 2, 22050, 16 ) ) )
	{
		MessageBox( SpLib::m_hWnd
					, "SetPrimaryBufferFormat Failed"
					, "Err"
					, MB_ICONEXCLAMATION);
		return -1;
	}

	return 0;
}


void SpLib_DestroyWin()
{
	int i=0;
	int iSize = SpLib::m_vFont.size();

	for(i=0; i<iSize; ++i)
	{
		SAFE_RELEASE(	SpLib::m_vFont[i]	);
	}

	SpLib::m_vFont.clear();


	iSize = SpLib::m_vTx.size();

	for(i=0; i<iSize; ++i)
	{
		SAFE_DELETE(	SpLib::m_vTx[i]	);
	}

	SpLib::m_vTx.clear();


	iSize = SpLib::m_vSound.size();

	for(i=0; i<iSize; ++i)
	{
		SAFE_DELETE(	SpLib::m_vSound[i]	);
	}

	SpLib::m_vSound.clear();


	SAFE_DELETE(	SpLib::m_pSndMn	);


	if(SpLib::m_pSndMn)
	{
		delete SpLib::m_pSndMn;
		SpLib::m_pSndMn = NULL;
	}

	if(SpLib::m_pd3dSprite)
	{
		SpLib::m_pd3dSprite->Release();
		SpLib::m_pd3dSprite = NULL;
	}

	if(SpLib::m_pd3dDevice)
	{
		SpLib::m_pd3dDevice->Release();
		SpLib::m_pd3dDevice = NULL;
	}

	if(SpLib::m_pD3D)
	{
		SpLib::m_pD3D->Release();
		SpLib::m_pD3D = NULL;
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


