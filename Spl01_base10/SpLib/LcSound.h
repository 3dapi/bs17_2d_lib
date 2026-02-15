//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef _LcSound_H_
#define _LcSound_H_


namespace SpLib
{

class CLcSound : public ILcSound
{
protected:
	int					m_nID;
	char				m_sFile[128];
	CSound*				m_pSnd;

public:
	CLcSound();
	virtual	~CLcSound();

	virtual	int		Create(void* p1=NULL,void* p2=NULL,void* p3=NULL,void* p4=NULL);
	virtual	void	Destroy();
	virtual	int		GetID();

	virtual	int		Play();
	virtual	int		Stop();
	virtual	int		Reset();
	virtual	int		GetState();


	static int				m_nIDSound	;		// Sound ID
	static HWND				m_hWnd		;		// Window Handle
	static CSoundManager*	m_pSndMn	;		//	Sound Manager
};



}//namespace SpLib

#endif

