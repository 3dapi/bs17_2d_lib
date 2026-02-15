//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef _ILcFont_H_
#define _ILcFont_H_


#ifndef interface
#define interface	struct
#endif

namespace SpLib
{

interface	ILcFont
{
	virtual	~ILcFont(){};

	virtual	int		Create(void* p1=NULL,void* p2=NULL,void* p3=NULL,void* p4=NULL)=0;
	virtual	void	Destroy()=0;
	virtual	int		GetID()=0;

	virtual	void*	GetFont()=0;
};


int LcDev_FontCreate(char* sCmd
					, ILcFont** pData
					, void* p1=NULL		// char*: fong Name
					, void* p2=NULL		// LONG: Height
					, void* p3=NULL		// LONG: Italic?
					, void* p4=NULL
					);

int			LcDev_FontInit(void* pd3dDevice);
ILcFont*	LcDev_FontFind(int _nID);
void		LcDev_FontDestroy();

}//namespace SpLib

#endif

