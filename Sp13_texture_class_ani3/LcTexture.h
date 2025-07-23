//
////////////////////////////////////////////////////////////////////////////////


#ifndef _LcTexture_H_
#define _LcTexture_H_


class CLcTexture: public ILcTexture
{
protected:
	char				m_sFile[128];
	D3DXIMAGE_INFO		m_Img;
	LPDIRECT3DTEXTURE9	m_pTx;

public:
	CLcTexture();
	virtual ~CLcTexture();

	virtual INT		Create(void* p1=NULL, void* p2=NULL, void* p3=NULL, void* p4=NULL);
	virtual void	Destroy();
	virtual const char*	GetName();

	virtual INT		GetImageWidth();
	virtual INT		GetImageHeight();
	virtual void*	GetTexture();

	static LPDIRECT3DDEVICE9 m_pDevice;
};


#endif

