//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef _ILcTexture_H_
#define _ILcTexture_H_


#ifndef interface
#define interface	struct
#endif

namespace SpLib
{

interface	ILcTexture
{
	virtual	~ILcTexture(){};

	virtual	int		Create(void* p1=NULL,void* p2=NULL,void* p3=NULL,void* p4=NULL)=0;
	virtual	void	Destroy()=0;
	virtual	int		GetID()=0;

	virtual	int		GetImageWidth()=0;
	virtual	int		GetImageHeight()=0;
	virtual	void*	GetTexture()=0;
	virtual	void	GetImageRect(void* rc)=0;
};


int			LcDev_TextureCreate(char* sCmd
					, ILcTexture** pData
					, void* p1=NULL
					, void* p2=NULL
					, void* p3=NULL
					, void* p4=NULL
					);


int			LcDev_TextureInit(void* pd3dDevice);
ILcTexture*	LcDev_TextureFind(int nID);
void		LcDev_TextureDestroy();

}//namespace SpLib

#endif

