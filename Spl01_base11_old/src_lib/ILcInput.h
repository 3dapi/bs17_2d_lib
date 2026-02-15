//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef _ILcInput_H_
#define _ILcInput_H_


#ifndef interface
#define struct interface
#endif


namespace LcsLib
{

interface ILcInput
{
	enum EInput
	{
		EINPUT_NONE  = 0,
		EINPUT_DOWN  = 1,
		EINPUT_UP	 = 2,
		EINPUT_PRESS = 3,
		EINPUT_DBCLC = 4,

		MAX_INPUT_KEY = 256,
		MAX_INPUT_BTN = 8,
	};
	

	virtual ~ILcInput(){};

	virtual	int		Create(void* p1=NULL,void* p2=NULL,void* p3=NULL,void* p4=NULL)=0;
	virtual int		FrameMove() = 0;
	virtual void	Destroy() = 0;
	virtual LRESULT	MsgProc(HWND,unsigned int,WPARAM,LPARAM)=0;

	virtual BOOL	KeyDown	(int nKey)=0;
	virtual BOOL	KeyUp	(int nKey)=0;
	virtual BOOL	KeyPress(int nKey)=0;
	virtual int		KeyState(int nKey)=0;

	virtual BOOL	BtnDown	 (int nBtn)=0;
	virtual BOOL	BtnUp	 (int nBtn)=0;
	virtual BOOL	BtnPress (int nBtn)=0;
	virtual int		BtnState (int nBtn)=0;

	virtual const FLOAT* GetMousePos()=0;
	virtual const FLOAT* GetMouseEps()=0;

	virtual const unsigned char* GetKeyMap()=0;
	virtual const unsigned char* GetBtnMap()=0;
};

int LcDev_InputCreate(char* sCmd, ILcInput** pData, void* hWnd);

}//namespace LcsLib

#endif

