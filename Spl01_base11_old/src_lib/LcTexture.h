//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef _LcTexture_H_
#define _LcTexture_H_


namespace LcsLib
{

class CLcTexture : public ILcTexture
{
protected:
	INT					m_nID;
	char				m_sFile[128];
	LPDIRECT3DTEXTURE9	m_pTex;
	D3DXIMAGE_INFO		m_pImg;

public:
	CLcTexture();
	virtual	~CLcTexture();

	virtual	INT		Create(void* p1=NULL,void* p2=NULL,void* p3=NULL,void* p4=NULL);
	virtual	void	Destroy();
	virtual	INT		GetID();

	virtual	INT		GetImageWidth();
	virtual	INT		GetImageHeight();
	virtual	void*	GetTexture();
	virtual	void	GetImageRect(void* rc);

	char*	GetName();

	static	INT					m_nIDTex;		// Texture ID
	static	LPDIRECT3DDEVICE9	m_pDevice;		// Direct3DDevice
};


ILcTexture*	LcDev_TextureFind(char* sFile);

}//namespace LcsLib

#endif

