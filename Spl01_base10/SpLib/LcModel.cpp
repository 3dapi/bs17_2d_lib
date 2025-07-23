//
////////////////////////////////////////////////////////////////////////////////

#pragma warning(disable: 4996)

#include <vector>

using namespace std;

#include <windows.h>

#include <d3d9.h>
#include <d3dx9.h>

#include "ILcTexture.h"
#include "ILcModel.h"
#include "LcModel.h"


namespace SpLib
{
INT				CLcModel2D::m_nIDModel	= 0;		// Sound ID
LPD3DXSPRITE	CLcModel2D::m_pSprite = NULL;
CLcModel2D::lsModel* CLcModel2D::m_vModel	= NULL;


INT LcDev_ModelInit(void* pSprite)
{
	CLcModel2D::m_pSprite	= (LPD3DXSPRITE)pSprite;
	CLcModel2D::m_vModel	= new CLcModel2D::lsModel;

	return 0;
}

void LcDev_ModelDestroy()
{
	if(CLcModel2D::m_vModel)
	{
		CLcModel2D::itModel	_F = CLcModel2D::m_vModel->begin();
		CLcModel2D::itModel	_L = CLcModel2D::m_vModel->end();

		for(; _F != _L; ++_F)
			delete (*_F);

		CLcModel2D::m_vModel->clear();

		delete CLcModel2D::m_vModel;
		CLcModel2D::m_vModel = NULL;
	}
}



CLcModel2D::CLcModel2D()
{
	m_AniDelta		= 0;
	m_AniTot		= 0;

	m_AniTex		= NULL;

	memset(&m_sFile, 0, sizeof m_sFile);
}


CLcModel2D::~CLcModel2D()
{
	Destroy();
}


void CLcModel2D::Destroy()
{
	if(m_AniTex)
	{
		delete m_AniTex;
		m_AniTex = NULL;
	}

	m_AniLst.clear();
}


INT CLcModel2D::GetID()
{
	return m_nID;
}

const char* CLcModel2D::GetName()
{
	return m_sFile;
}

void CLcModel2D::SetPos(const FLOAT* pPos)
{
	if(NULL == pPos)
		m_vcPos = D3DXVECTOR3(0,0,0);

	else
		::memcpy(&m_vcPos, pPos, sizeof(D3DXVECTOR3));
}


void CLcModel2D::SetColor(DWORD dColor)
{
	m_dColor = dColor;
}


void CLcModel2D::SetRect(void* rc)
{
	if(NULL == rc)
	{
		INT nW = m_AniTex->GetImageWidth();
		INT nH = m_AniTex->GetImageHeight();
		::SetRect(&m_ImgRc, 0,0, nW, nH);
	}
	else
		::memcpy(&m_ImgRc, rc, sizeof(RECT));
}



INT	CLcModel2D::FindAniIndex(INT* pOut/*Out*/, DWORD dTimeCur, DWORD dTimeBgn)
{
	BOOL	nFind = -1;
	
	DWORD	dDelta =0;
	
	
	dDelta = dTimeCur- dTimeBgn;
	
	for(INT i=0; i<m_AniTot; ++i)
	{
		if( m_AniLst[i].dTime<=dDelta && dDelta < (m_AniLst[i].dTime+ m_AniDelta))
		{
			nFind = i;
			break;
		}
	}
	
	if(-1 == nFind)
		return -1;
	
	*pOut = nFind;
	
	return 0;
}


INT CLcModel2D::FindImageRect(void* pOut/*Out*/, INT nIndex)
{
	if( nIndex<0 || nIndex>=m_AniTot)
		return -1;
	
	memcpy(pOut, &m_AniLst[nIndex].ImgRc, sizeof(RECT));
	return 0;
}




INT CLcModel2D::Create(void* p1, void* p2, void* p3, void* p4)
{
	::strcpy(m_sFile, (char*)p1);


	char	sLine[512]={0};

	FILE* fp = ::fopen(m_sFile, "rt");

	if(NULL == fp)
		return -1;

	char	sTexFile[128]={0};
	DWORD	dColorKey=0;
	::fgets(sLine, 512, fp);
	::sscanf(sLine, "%*s %s %x", sTexFile, &dColorKey);
	

	::fgets(sLine, 512, fp);
	::sscanf(sLine, "%*s %d", &m_AniTot);

	::fgets(sLine, 512, fp);
	::sscanf(sLine, "%*s %ld", &m_AniDelta);

	INT nCnt = 0;

	while(!feof(fp))
	{
		DWORD	dTime;
		RECT	rc;
		::fgets(sLine, 512, fp);

		if(::strlen(sLine)>5)
		{
			::sscanf(sLine, "%ld "
						"%ld %ld %ld %ld"
						, &dTime
						, &rc.left, &rc.top, &rc.right, &rc.bottom);

			m_AniLst.push_back( CLcModel2D::TAniInfo(dTime, rc) );

			++nCnt;

			if(nCnt>=m_AniTot)
				break;
		}
	}

	::fclose(fp);


	if(FAILED(LcDev_TextureCreate("File", &m_AniTex, sTexFile, &dColorKey)))
		return -1;

	

	INT _nID = CLcModel2D::m_nIDModel;
	++_nID;
	
	// overflow....
	if(_nID<0)
	{
		printf("Overflow Model List\n");
		return -1;
	}

	CLcModel2D::m_nIDModel = _nID;
	m_nID = _nID;


	return 0;
}


void CLcModel2D::Render()
{
	LPDIRECT3DTEXTURE9	pTex = (LPDIRECT3DTEXTURE9)m_AniTex->GetTexture();

	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
	m_pSprite->Draw(pTex, &m_ImgRc, NULL, &m_vcPos, m_dColor);
	m_pSprite->End();
}



INT LcDev_ModelCreate(char* sCmd, ILcModel** pData, void* p1, void* p2, void* p3, void* p4)
{
	char* sModelName = (char*)p1;

	*pData = NULL;


	ILcModel* pObj = LcDev_ModelFind((char*)sModelName);

	if(pObj)
	{
		*pData = pObj;
		return 0;
	}


	pObj = new CLcModel2D;

	if(FAILED(pObj->Create(sModelName)))
	{
		delete pObj;
		return -1;
	}

	CLcModel2D::m_vModel->push_back(pObj);

	*pData = pObj;
	return 0;
}



ILcModel* LcDev_ModelFind(char* sFile)
{
	CLcModel2D::itModel	_F = CLcModel2D::m_vModel->begin();
	CLcModel2D::itModel	_L = CLcModel2D::m_vModel->end();
	
	for(; _F != _L; ++_F)
	{
		if( 0 == _stricmp((*_F)->GetName(), sFile))
			return *_F;
	}

	return NULL;
}


ILcModel* LcDev_ModelFind(INT _nID)
{
	CLcModel2D::itModel	_F = CLcModel2D::m_vModel->begin();
	CLcModel2D::itModel	_L = CLcModel2D::m_vModel->end();

	for(; _F != _L; ++_F)
	{
		int nID = (*_F)->GetID();

		if( nID == _nID)
			return *_F;
	}
	
	return NULL;
}

};// namespace SpLib
////////////////////////////////////////////////////////////////////////////////

