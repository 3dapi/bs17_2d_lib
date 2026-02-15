//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef _LcTexture_H_
#define _LcTexture_H_


namespace SpLib
{

class CLcTexture : public ILcTexture
{
protected:
	int					m_nID;
	char				m_sFile[128];
	LPDIRECT3DTEXTURE9	m_pTex;
	D3DXIMAGE_INFO		m_pImg;

public:
	CLcTexture();
	virtual	~CLcTexture();

	virtual	int		Create(void* p1=NULL,void* p2=NULL,void* p3=NULL,void* p4=NULL);
	virtual	void	Destroy();
	virtual	int		GetID();

	virtual	int		GetImageWidth();
	virtual	int		GetImageHeight();
	virtual	void*	GetTexture();
	virtual	void	GetImageRect(void* rc);

	char*	GetName();

	static	int					m_nIDTex;		// Texture ID
	static	LPDIRECT3DDEVICE9	m_pDevice;		// Direct3DDevice
};


ILcTexture*	LcDev_TextureFind(char* sFile);

}//namespace SpLib

#endif

