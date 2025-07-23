//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef _LcFont_H_
#define _LcFont_H_


namespace LcsLib
{

class CLcFont : public ILcFont
{
protected:
	INT					m_nID;
	D3DXFONT_DESC		m_Dsc;
	ID3DXFont*			m_pFnt;

public:
	CLcFont();
	virtual	~CLcFont();

	virtual	INT		Create(void* p1=NULL,void* p2=NULL,void* p3=NULL,void* p4=NULL);
	virtual	void	Destroy();
	virtual	INT		GetID();

	virtual	void*	GetFont();

	static INT					m_nIDFnt;		// Font ID
	static LPDIRECT3DDEVICE9	m_pDevice;		// Window Handle
};


}//namespace LcsLib

#endif

