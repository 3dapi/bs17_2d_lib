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

	unsigned char		m_KeyCur[ILcInput::MAX_INPUT_KEY];	// 키보드 현재 상태
	unsigned char		m_KeyOld[ILcInput::MAX_INPUT_KEY];	// 키보드 이전 상태
	unsigned char		m_KeyMap[ILcInput::MAX_INPUT_KEY];	// 키보드 맵

	unsigned char		m_BtnCur[ILcInput::MAX_INPUT_BTN];	// 마우스 현재 상태
	unsigned char		m_BtnOld[ILcInput::MAX_INPUT_BTN];	// 마우스 이전 상태
	unsigned char		m_BtnMap[ILcInput::MAX_INPUT_BTN];	// 마우스 맵

	D3DXVECTOR3	m_vcCur;					// 휠 마우스 Z
	D3DXVECTOR3 m_vcOld;
	D3DXVECTOR3 m_vcEps;

	DWORD		m_dTimeDC;					// Double Click Time Interval
	DWORD		m_dBtnBgn[ILcInput::MAX_INPUT_BTN];	// Double Click Start
	int			m_dBtnCnt[ILcInput::MAX_INPUT_BTN];	// Double Click Count


public:
	CLcInput();
	virtual ~CLcInput();

	virtual	int		Create(void* p1=NULL,void* p2=NULL,void* p3=NULL,void* p4=NULL);
	virtual int		FrameMove();
	virtual void	Destroy();
	virtual LRESULT	MsgProc(HWND,unsigned int,WPARAM,LPARAM);

public:
	virtual BOOL	KeyDown	(int nKey);
	virtual BOOL	KeyUp	(int nKey);
	virtual BOOL	KeyPress(int nKey);
	virtual int		KeyState(int nKey);

	virtual BOOL	BtnDown	 (int nBtn);
	virtual BOOL	BtnUp	 (int nBtn);
	virtual BOOL	BtnPress (int nBtn);
	virtual int		BtnState (int nBtn);

	virtual const FLOAT* GetMousePos();
	virtual const FLOAT* GetMouseEps();

	virtual const unsigned char* GetKeyMap();
	virtual const unsigned char* GetBtnMap();
};


}//namespace SpLib

#endif

