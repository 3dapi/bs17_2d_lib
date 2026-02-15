//
////////////////////////////////////////////////////////////////////////////////


#ifndef _ILcTexture_H_
#define _ILcTexture_H_


#ifndef interface
#define struct interface
#endif

interface ILcTexture
{
	enum ELcTexture
	{
		FILE	= 1,
		RESOURCE = 2,
		MEMORY	= 3,
	};

	virtual ~ILcTexture(){};

	virtual int		Create(void* p1=NULL, void* p2=NULL, void* p3=NULL, void* p4=NULL) =0;
	virtual void	Destroy() =0;
	virtual const char*	GetName() =0;

	virtual int		GetImageWidth() =0;
	virtual int		GetImageHeight() =0;
	virtual void*	GetTexture() = 0;
};


int		LcDev_TextureInit(void* pDev);
int		LcDev_TextureCreate(char* sCmd, ILcTexture** pData, void* pTextureName, DWORD dColorKey);
void	LcDev_TextureDestroy();



#endif

