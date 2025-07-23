//
////////////////////////////////////////////////////////////////////////////////


#ifndef _ILcModel_H_
#define _ILcModel_H_


#ifndef interface
#define struct interface
#endif

interface ILcModel
{
	virtual ~ILcModel(){};
	
	virtual INT		Create(void* p1=NULL, void* p2=NULL, void* p3=NULL, void* p4=NULL) =0;
	virtual void	Destroy() =0;
	virtual void	Render() =0;
	virtual const char*	GetName() = 0;

	virtual	void	SetPos(const FLOAT* =NULL) =0;
	virtual	void	SetColor(const DWORD=0xFFFFFFFF) =0;
	virtual	void	SetRect(void* rc=NULL) =0;

	virtual	INT		FindAniIndex(INT* pOut/*Out*/, DWORD TimeCur, DWORD TimeBegin) =0;
	virtual	INT		FindImageRect(void* pRECT/*Out*/, INT nIndex) =0;
};

INT		LcDev_ModelCreate(char* sCmd, ILcModel** pData, void* pModelName);
void	LcDev_ModelInit(void* pDev);
void	LcDev_ModelDestroy();



#endif


