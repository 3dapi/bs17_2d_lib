//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef _LcInput_H_
#define _LcInput_H_


namespace SpLib
{
	
class CLcInput : public ILcInput
{
public:
	

public:
	HWND		m_hWnd;

	BYTE		m_KeyCur[ILcInput::MAX_INPUT_KEY];	// 키보드 현재 상태
	BYTE		m_KeyOld[ILcInput::MAX_INPUT_KEY];	// 키보드 이전 상태
	BYTE		m_KeyMap[ILcInput::MAX_INPUT_KEY];	// 키보드 맵

	BYTE		m_BtnCur[ILcInput::MAX_INPUT_BTN];	// 마우스 현재 상태
	BYTE		m_BtnOld[ILcInput::MAX_INPUT_BTN];	// 마우스 이전 상태
	BYTE		m_BtnMap[ILcInput::MAX_INPUT_BTN];	// 마우스 맵

	D3DXVECTOR3	m_vcCur;					// 휠 마우스 Z
	D3DXVECTOR3 m_vcOld;
	D3DXVECTOR3 m_vcEps;

	DWORD		m_dTimeDC;					// Double Click Time Interval
	DWORD		m_dBtnBgn[ILcInput::MAX_INPUT_BTN];	// Double Click Start
	INT			m_dBtnCnt[ILcInput::MAX_INPUT_BTN];	// Double Click Count


public:
	CLcInput();
	virtual ~CLcInput();

	virtual	INT		Create(void* p1=NULL,void* p2=NULL,void* p3=NULL,void* p4=NULL);
	virtual INT		FrameMove();
	virtual void	Destroy();
	virtual LRESULT	MsgProc(HWND,UINT,WPARAM,LPARAM);

public:
	virtual BOOL	KeyDown	(INT nKey);
	virtual BOOL	KeyUp	(INT nKey);
	virtual BOOL	KeyPress(INT nKey);
	virtual INT		KeyState(INT nKey);

	virtual BOOL	BtnDown	 (INT nBtn);
	virtual BOOL	BtnUp	 (INT nBtn);
	virtual BOOL	BtnPress (INT nBtn);
	virtual INT		BtnState (INT nBtn);

	virtual const FLOAT* GetMousePos();
	virtual const FLOAT* GetMouseEps();

	virtual const BYTE* GetKeyMap();
	virtual const BYTE* GetBtnMap();
};


}//namespace SpLib

#endif

