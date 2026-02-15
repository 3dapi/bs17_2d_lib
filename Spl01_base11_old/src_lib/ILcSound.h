//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef _ILcSound_H_
#define _ILcSound_H_


#ifndef interface
#define interface	struct
#endif

namespace LcsLib
{

interface	ILcSound
{
	virtual	~ILcSound(){};

	virtual	int		Create(void* p1=NULL,void* p2=NULL,void* p3=NULL,void* p4=NULL)=0;
	virtual	void	Destroy()=0;
	virtual	int		GetID()=0;

	virtual	int		Play()=0;
	virtual	int		Stop()=0;
	virtual	int		Reset()=0;
	virtual int		GetState()=0;
};


int			LcDev_SoundCreate(char* sCmd
					, ILcSound** pData
					, void* p1=NULL		// char* : Sound File Name
					, void* p2=NULL
					, void* p3=NULL
					, void* p4=NULL
					);

int			LcDev_SoundInit(void* hWnd);
ILcSound*	LcDev_SoundFind(int nID);
void		LcDev_SoundDestroy();

}//namespace LcsLib

#endif

