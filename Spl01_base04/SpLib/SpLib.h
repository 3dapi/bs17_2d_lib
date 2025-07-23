#ifndef _SpLib_
#define _SpLib_

#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>

typedef D3DXVECTOR2							VEC2;
typedef D3DXVECTOR3							VEC3;
typedef D3DXVECTOR4							VEC4;

typedef D3DXMATRIX							MATA;
typedef D3DXQUATERNION						QUAT;
typedef D3DXCOLOR							DCLR;

typedef	LPDIRECT3D9							PD3D;
typedef LPDIRECT3DDEVICE9					PDEV;
typedef	LPD3DXSPRITE						PDSP;

typedef LPDIRECT3DTEXTURE9					PDTX;
typedef LPDIRECT3DSURFACE9					PDSF;
typedef	D3DXIMAGE_INFO						DIMG;


int		SpLib_CreateWin(int x, int y, int width, int height, char* sName);		//윈도우와 DirectX생성
void	SpLib_DestroyWin();														// 윈도우와 DirectX소멸
int		SpLib_Run();															// 게임 루프

void	SpLib_SetFrameMove( int (*_FrameMove)() );								// 게임 데이터 update 함수
void	SpLib_SetRender( int (*_Render)() );									// 게임 데이터 렌더링 함수
void	SpLib_SetKeyboard( int (*_Keyboard)(unsigned char* key) );				// 게임엔진에 키보드 이벤트를 가져오는 함수를 연결해주는 함수
void	SpLib_SetMouse( int (*_Mouse)(int x, int y, int z, int _event) );		// 게임엔진에 마우스 이벤트를 가져오는 함수를 연결해주는 함수


int		SpLib_TextureLoad(char* sFileName
						  , LPDIRECT3DTEXTURE9& pTx
						  , D3DXIMAGE_INFO* pImg= NULL
						  , unsigned long dc=0x00FFFFFF);						// 이미지 파일 로딩

void	SpLib_TextureRelease(LPDIRECT3DTEXTURE9& pTx);							// 이미지 파일 해제


int		SpLib_Draw2D(LPDIRECT3DTEXTURE9 pSrc
					 , RECT* pSrcRect
					 , D3DXVECTOR2* pTranslate=NULL
					 , D3DXVECTOR2* pScaling=NULL
					 , D3DXVECTOR2* pRot=NULL, FLOAT fAngle=0
					 , unsigned long dC=0xFFFFFFFF);							// 이미지 파일 그리기


unsigned char* SpLib_GetKeyboard();
int		SpLib_GetMouseX();
int		SpLib_GetMouseY();
int		SpLib_GetMouseZ();
int		SpLib_GetMouseEvent(int nMouse);

HWND	SpLib_GetHwnd();														// 윈도우 핸들 얻기
int		SpLib_GetScnW();														// 화면의 크기를 가져오기
int		SpLib_GetScnH();														// 화면의 너비를 가져오기

void	SpLib_SetClearColor(unsigned long dC);									// 배경화면 클리어 색상설정
DWORD	SpLib_GetClearColor();											// 배령화면 클리어 색상 가져오기


void	SpLib_SetWindowTitle(const char *format, ...);							// Window Title
void	SpLib_SetWindowStyle(DWORD dSty);										// window Style
DWORD	SpLib_GetWindowStyle();

#endif