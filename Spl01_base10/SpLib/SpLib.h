#ifndef _SpLib_H_
#define _SpLib_H_

#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>

typedef D3DXVECTOR2							VEC2;
typedef D3DXVECTOR3							VEC3;
typedef D3DXCOLOR							DCLR;

typedef	LPDIRECT3D9							PD3D;
typedef LPDIRECT3DDEVICE9					PDEV;
typedef	LPD3DXSPRITE						PDSP;

typedef LPDIRECT3DTEXTURE9					PDTX;
typedef LPDIRECT3DSURFACE9					PDSF;
typedef	D3DXIMAGE_INFO						DIMG;


INT		SpLib_CreateWin(INT x,INT y,INT ScnW,INT ScnH,char* sName,bool bFull);	//윈도우와 DirectX생성
void	SpLib_DestroyWin();											// 윈도우와 DirectX소멸
INT		SpLib_Run();												// 게임 루프

// 함수 포인터 설정
void	SpLib_SetFrameMove( INT (*)() );							// 게임 데이터 update 함수
void	SpLib_SetRender( INT (*)() );								// 게임 데이터 렌더링 함수
void	SpLib_SetKeyboard( INT (*)(BYTE*) );						// 게임엔진에 키보드 이벤트를 가져오는 함수를 연결해주는 함수
void	SpLib_SetMouse( INT (*)(INT x,INT y,INT z,INT _event) );	// 게임엔진에 마우스 이벤트를 가져오는 함수를 연결해주는 함수

// 인풋
const BYTE*	SpLib_GetKeyboard();									// 키보드(0~255)
INT		SpLib_GetMouseX();											// 마우스 위치 X
INT		SpLib_GetMouseY();											// 마우스 위치 Y
INT		SpLib_GetMouseZ();											// 마우스 위치 Z
INT		SpLib_GetMouseEvent(INT nMouse);							// 마우스 이벤트 (0: LButton, 1: RButton, 2: MButton)

void	SpLib_SetWindowStyle(DWORD dSty);
DWORD	SpLib_GetWindowStyle();
HWND	SpLib_GetHwnd();											// 윈도우 핸들 얻기
INT		SpLib_GetScnW();											// 화면의 크기를 가져오기
INT		SpLib_GetScnH();											// 화면의 너비를 가져오기

void	SpLib_SetStateShow(bool _bShow);							// State 보여주기
void	SpLib_SetCursorShow(bool _bShow);							// Cursor 보여주기
void	SpLib_SetClearColor(DWORD dC);								// 배경화면 클리어 색상설정
DWORD	SpLib_GetClearColor();										// 배령화면 클리어 색상 가져오기
void	SpLib_SetWindowTitle(const char *format, ...);				// 윈도우 타이틀

// 텍스처
INT		SpLib_TextureLoad(char* sFileName, DWORD dc=0x00FFFFFF);	// 이미지 파일 로딩
INT		SpLib_TextureRelease(INT _nKey);							// 이미지 파일 해제
INT		SpLib_TextureWidth(INT _nKey);								// 이미지 파일 너비
INT		SpLib_TextureHeight(INT _nKey);								// 이미지 파일 높이
INT		SpLib_Draw2D(INT _nKey
					 , RECT* pSrcRect
					 , VEC2* pTranslate=NULL
					 , VEC2* pScaling=NULL
					 , VEC2* pRotation=NULL
					 , FLOAT fAngle=0
					 , DWORD dC=0xFFFFFFFF);						// 이미지 파일 그리기


// 폰트
INT		SpLib_FontCreate(char* sName, LONG iH, LONG iItalic=0);		// 폰트 객체 생성
INT		SpLib_FontDrawText(INT nIdx									// 문자열 출력
					   , LONG left
					   , LONG top
					   , LONG right
					   , LONG bottom
					   , DWORD fontColor
					   , const char *format, ...);

// 사운드
INT		SpLib_SoundLoad(char* sFileName);				// 사운드 로딩
INT		SpLib_SoundRelease(INT _nKey);					// 사운드 객체 해제
void	SpLib_SoundPlay(INT _nKey);						// 사운드 실행
void	SpLib_SoundStop(INT _nKey);						// 사운드 멈춤
void	SpLib_SoundReset(INT _nKey);					// 사운드 리셋
BOOL	SpLib_SoundIsPlaying(INT _nKey);				// 사운드 실행중?

#endif