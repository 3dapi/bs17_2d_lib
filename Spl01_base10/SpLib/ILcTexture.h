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

	virtual	INT		Create(void* p1=NULL,void* p2=NULL,void* p3=NULL,void* p4=NULL)=0;
	virtual	void	Destroy()=0;
	virtual	INT		GetID()=0;

	virtual	INT		GetImageWidth()=0;
	virtual	INT		GetImageHeight()=0;
	virtual	void*	GetTexture()=0;
	virtual	void	GetImageRect(void* rc)=0;
};


INT			LcDev_TextureCreate(char* sCmd
					, ILcTexture** pData
					, void* p1=NULL
					, void* p2=NULL
					, void* p3=NULL
					, void* p4=NULL
					);


INT			LcDev_TextureInit(void* pd3dDevice);
ILcTexture*	LcDev_TextureFind(INT nID);
void		LcDev_TextureDestroy();

}//namespace SpLib

#endif

