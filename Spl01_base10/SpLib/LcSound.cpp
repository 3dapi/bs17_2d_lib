

#include <vector>

#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <DxErr.h>
#include <dsound.h>
#include <stdio.h>

#include "DsUtil.h"

#include "ILcSound.h"
#include "LcSound.h"


#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }



namespace SpLib
{
INT				CLcSound::m_nIDSound= 0;		// Sound ID
HWND			CLcSound::m_hWnd	= NULL;		// Window Handle
CSoundManager*	CLcSound::m_pSndMn	= NULL;		//	Sound Manager

CLcSound::CLcSound()
{
	m_nID	= -1;
	m_pSnd	= NULL;
	
	memset(m_sFile, 0, sizeof m_sFile);
}

CLcSound::~CLcSound()
{
	Destroy();
}

void CLcSound::Destroy()
{
	SAFE_DELETE(	m_pSnd	);
	memset(m_sFile, 0, sizeof m_sFile);
}


INT CLcSound::GetID()
{
	return m_nID;
}

INT CLcSound::Create(void* p1,void* p2,void* p3,void* p4)
{
	char*	sFile	= (char*)p1;
	CSound*	pSnd	= NULL;
	
	if(FAILED(CLcSound::m_pSndMn->Create( &pSnd, sFile, 0, GUID_NULL, 1 )))
	{
		printf("Create Sound Failed: %s\n", sFile);
		m_pSnd = NULL;
		return -1;
	}

	if(NULL ==pSnd)
		return -1;

	m_pSnd = pSnd;

	// 새로운 아이디 부여
	INT _nID = CLcSound::m_nIDSound;

	++_nID;

	// overflow....
	if(_nID<0)
	{
		printf("Overflow Sound List\n");
		delete pSnd;
		pSnd = NULL;
		return -1;
	}


	CLcSound::m_nIDSound = _nID;
	m_nID = _nID;

	return 0;
}


INT CLcSound::Play()
{
	return m_pSnd->Play();
}

INT CLcSound::Stop()
{
	return m_pSnd->Stop();
}


INT CLcSound::Reset()
{
	return m_pSnd->Reset();
}


INT CLcSound::GetState()
{
	return	m_pSnd->IsSoundPlaying();
}


INT LcDev_SoundCreate(char* sCmd
					, ILcSound** pData
					, void* p1
					, void* p2
					, void* p3
					, void* p4
					)
{
	*pData = NULL;

	CLcSound* pObj=new CLcSound;

	if(FAILED(pObj->Create(p1, p2, p3, p4)))
	{
		delete pObj;
		return -1;
	}

	*pData = pObj;
	return 0;
}


typedef std::vector<ILcSound* >	lsLcSound;
typedef lsLcSound::iterator		itLcSound;


lsLcSound	m_vSound;			// Sound List


INT LcDev_SoundInit(void* hWnd)
{
	CLcSound::m_hWnd = (HWND)hWnd;

	CLcSound::m_pSndMn = new CSoundManager;

	if( FAILED(CLcSound::m_pSndMn->Initialize( CLcSound::m_hWnd, DSSCL_PRIORITY ) ) )
	{
		printf("Error: Sound Manager Initialize Failed\n");
		return -1;
	}
	
	if( FAILED( CLcSound::m_pSndMn->SetPrimaryBufferFormat( 2, 22050, 16 ) ) )
	{
		printf("Error: SetPrimaryBufferFormat Failed\n");
		return -1;
	}

	return 0;
}

void LcDev_SoundDestroy()
{
	INT iSize = SpLib::m_vSound.size();
	
	for(INT i=0; i<iSize; ++i)
	{
		SAFE_DELETE( m_vSound[i]	);
	}
	
	m_vSound.clear();


	SAFE_DELETE(	CLcSound::m_pSndMn	);
}


ILcSound* LcDev_SoundFind(INT _nID)
{
	INT iSize = m_vSound.size();
	INT	nIdx=-1;
	
	for(INT i=0; i<iSize; ++i)
	{
		if(m_vSound[i]->GetID() == _nID)
		{
			nIdx = i;
			break;
		}
	}
	
	if(nIdx<0 || nIdx>=iSize)
	{
		return 0;
	}
	
	return m_vSound[nIdx];
}


};// namespace SpLib
////////////////////////////////////////////////////////////////////////////////









INT SpLib_SoundLoad(char* sFileName)
{
	SpLib::ILcSound*	p = NULL;
	
	if(FAILED(SpLib::LcDev_SoundCreate( "File", &p, sFileName)))
		return -1;
	
	SpLib::m_vSound.push_back(p);
	
	// ID를 돌려 준다.
	return p->GetID();
}


INT SpLib_SoundRelease(INT _nID)
{
	INT iSize = SpLib::m_vSound.size();
	INT	nIdx=-1;

	for(INT i=0; i<iSize; ++i)
	{
		if(SpLib::m_vSound[i]->GetID() == _nID)
		{
			nIdx = i;
			break;
		}
	}

	if(nIdx<0 || nIdx>=iSize)
		return -1;


	SpLib::lsLcSound::iterator	itSound;

	itSound = SpLib::m_vSound.begin() + nIdx;
	SAFE_DELETE( SpLib::m_vSound[nIdx]		);
	SpLib::m_vSound.erase(itSound);

	iSize = SpLib::m_vSound.size();

	return iSize;
}



void	SpLib_SoundPlay(INT _nID)
{
	INT iSize = SpLib::m_vSound.size();
	INT	nIdx=-1;

	for(INT i=0; i<iSize; ++i)
	{
		if(SpLib::m_vSound[i]->GetID() == _nID)
		{
			nIdx = i;
			break;
		}
	}

	if(nIdx<0 || nIdx>=iSize)
		return;

	SpLib::m_vSound[nIdx]->Play();
}

void	SpLib_SoundStop(INT _nID)
{
	INT iSize = SpLib::m_vSound.size();
	INT	nIdx=-1;

	for(INT i=0; i<iSize; ++i)
	{
		if(SpLib::m_vSound[i]->GetID() == _nID)
		{
			nIdx = i;
			break;
		}
	}

	if(nIdx<0 || nIdx>=iSize)
		return;

	SpLib::m_vSound[nIdx]->Stop();
}

void	SpLib_SoundReset(INT _nID)
{
	INT iSize = SpLib::m_vSound.size();
	INT	nIdx=-1;

	for(INT i=0; i<iSize; ++i)
	{
		if(SpLib::m_vSound[i]->GetID() == _nID)
		{
			nIdx = i;
			break;
		}
	}

	if(nIdx<0 || nIdx>=iSize)
		return;

	SpLib::m_vSound[nIdx]->Reset();
}

BOOL	SpLib_SoundIsPlaying(INT _nID)
{
	INT iSize = SpLib::m_vSound.size();
	INT	nIdx=-1;

	for(INT i=0; i<iSize; ++i)
	{
		if(SpLib::m_vSound[i]->GetID() == _nID)
		{
			nIdx = i;
			break;
		}
	}

	if(nIdx<0 || nIdx>=iSize)
		return 0;

	return SpLib::m_vSound[nIdx]->GetState();
}



