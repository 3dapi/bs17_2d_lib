//
//
////////////////////////////////////////////////////////////////////////////////

#pragma warning( disable : 4996)

#include <windows.h>
#include <stdio.h>
#include "SpLib.h"


LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
int		RenderEnv();
int		FrameMove2D();
int		Render2D();



HINSTANCE	m_hInst;
HWND		m_hWnd;
char		m_sCls[512];

int			m_dScnX;
int			m_dScnY;
int			m_dScnW;
int			m_dScnH;

DWORD		m_dWinStyle	= WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU| WS_VISIBLE;
RECT		m_rcWindowBounds	;					// Saved window bounds for mode switches
RECT		m_rcWindowClient	;					// Saved client area size for mode switches

DWORD		m_dColor = 0xFF006699;					// D3D Clear Color
bool		m_bActive = true;



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

	return 0;
}


void SpLib_DestroyWin()
{
}


int	SpLib_Run()
{
	bool bGotMsg =false;
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
			TranslateMessage( &msg );
			DispatchMessage( &msg );
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

unsigned long	SpLib_GetClearColor()
{
	return m_dColor;
}


void SpLib_SetWindowTitle(const char *format, ...)
{
	va_list ap;
	char s[512];

	if (format == NULL) return;

	va_start(ap, format);
	vsprintf((char *)s, format, ap);
	va_end(ap);

	if (s == NULL)
		return;

	SetWindowText(m_hWnd, s);
}


void	SpLib_SetWindowStyle(DWORD dSty)	{	m_dWinStyle		= dSty;		}
DWORD	SpLib_GetWindowStyle()				{	return m_dWinStyle;			}



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

	}

	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}






////////////////////////////////////////////////////////////////////////////////
// Library 내부에서 사용하는 함수들

int RenderEnv()
{
	if(FAILED(FrameMove2D()))
		return -1;

	if(FAILED(Render2D()))
		return -1;

	return 0;
}


int FrameMove2D()
{
	// Rendering

	return 0;
}


int Render2D()
{
	// Rendering

	return 0;
}

