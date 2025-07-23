#ifndef _LcsLib_H_
#define _LcsLib_H_

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


//윈도우와 DirectX생성
INT		LcsLib_CreateWin(INT x,INT y								// Screen position x, y
						 ,INT ScnW,INT ScnH							// Screen Width, height
						 ,char* sName,INT bFull=FALSE				// Class name, Full screen mode
						 );

void	LcsLib_DestroyWin();										// 윈도우와 DirectX소멸
INT		LcsLib_ChangeWindow(INT bFull=TRUE);						// Change Window Mode
INT		LcsLib_Run();												// 게임 루프

// 함수 포인터 설정
void	LcsLib_SetFrameMove( INT (*)() );							// 게임 데이터 update 함수
void	LcsLib_SetRender( INT (*)() );								// 게임 데이터 렌더링 함수
void	LcsLib_SetKeyboard( INT (*)(BYTE*) );						// 게임엔진에 키보드 이벤트를 가져오는 함수를 연결해주는 함수
void	LcsLib_SetMouse( INT (*)(INT x,INT y,INT z,INT _event) );	// 게임엔진에 마우스 이벤트를 가져오는 함수를 연결해주는 함수

// 인풋
const BYTE*	LcsLib_GetKeyboard();									// 키보드(0~255)
INT		LcsLib_GetMouseX();											// 마우스 위치 X
INT		LcsLib_GetMouseY();											// 마우스 위치 Y
INT		LcsLib_GetMouseZ();											// 마우스 위치 Z
INT		LcsLib_GetMouseEvent(INT nMouse);							// 마우스 이벤트 (0: LButton, 1: RButton, 2: MButton)

void	LcsLib_SetWindowStyle(DWORD dSty);
DWORD	LcsLib_GetWindowStyle();
HWND	LcsLib_GetHwnd();											// 윈도우 핸들 얻기
INT		LcsLib_GetScnW();											// 화면의 크기를 가져오기
INT		LcsLib_GetScnH();											// 화면의 너비를 가져오기

void	LcsLib_SetStateShow(INT _bShow);							// State 보여주기
void	LcsLib_SetCursorShow(INT _bShow);							// Cursor 보여주기
void	LcsLib_SetClearColor(DWORD dC);								// 배경화면 클리어 색상설정
DWORD	LcsLib_GetClearColor();										// 배령화면 클리어 색상 가져오기
void	LcsLib_SetWindowTitle(const char *format, ...);				// 윈도우 타이틀

// 텍스처
INT		LcsLib_TextureLoad(char* sFileName, DWORD dc=0x00FFFFFF);	// 이미지 파일 로딩
INT		LcsLib_TextureRelease(INT _nKey);							// 이미지 파일 해제
INT		LcsLib_TextureWidth(INT _nKey);								// 이미지 파일 너비
INT		LcsLib_TextureHeight(INT _nKey);							// 이미지 파일 높이
INT		LcsLib_Draw2D(INT _nKey
					 , RECT* pSrcRect								// 그릴 영역
					 , VEC2* pTranslate=NULL						// 이동 위치
					 , VEC2* pScaling=NULL							// 크기 변환 값
					 , VEC2* pRotation=NULL							// 회전 중심 위치
					 , FLOAT fAngle=0								// 회전 각도: Radian
					 , DWORD dC=0xFFFFFFFF);						// 이미지 파일 그리기


// 폰트
INT		LcsLib_FontCreate(char* sName, LONG iH, LONG iItalic=0);	// 폰트 객체 생성
INT		LcsLib_FontDrawText(INT nIdx								// 문자열 출력
					   , LONG left
					   , LONG top
					   , LONG right
					   , LONG bottom
					   , DWORD fontColor
					   , const char *format, ...);

// 사운드
INT		LcsLib_SoundLoad(char* sFileName);				// 사운드 로딩
INT		LcsLib_SoundRelease(INT _nKey);					// 사운드 객체 해제
void	LcsLib_SoundPlay(INT _nKey);					// 사운드 실행
void	LcsLib_SoundStop(INT _nKey);					// 사운드 멈춤
void	LcsLib_SoundReset(INT _nKey);					// 사운드 리셋
BOOL	LcsLib_SoundIsPlaying(INT _nKey);				// 사운드 실행중?

#endif