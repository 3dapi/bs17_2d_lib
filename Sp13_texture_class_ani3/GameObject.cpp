//
////////////////////////////////////////////////////////////////////////////////

#include <vector>

using namespace std;

#include <windows.h>

#include <d3d9.h>
#include <d3dx9.h>

#include "ILcTexture.h"
#include "ILcModel.h"

#include "GameObject.h"


CGameObject::CGameObject()
{
	m_vcPos		= D3DXVECTOR3(0,0,0);
	m_pModel	= NULL;

	m_bAni			= 0;
	m_dTimeBgn		= timeGetTime();
	m_dTimeCur		= m_dTimeBgn;
	m_nAniCur		= 0;
	
	m_AniPos	= D3DXVECTOR3(0,0,0);
	memset(&m_AniRc, 0, sizeof(m_AniRc));
	m_AniColor	= 0xFFFFFFFF;
}


CGameObject::~CGameObject()
{
	Destroy();
}


void CGameObject::Destroy()
{
//	if(m_pModel)
//	{
//		delete m_pModel;
//		m_pModel = NULL;
//	}
}


void CGameObject::SetPos(const FLOAT* pPos)
{
	memcpy(&m_vcPos, pPos, sizeof(D3DXVECTOR3));
}


INT CGameObject::Create()
{
	return 0;
}


INT CGameObject::FrameMove()
{
	INT		nIndex = -1;
	
	m_dTimeCur	= timeGetTime();

	if(FAILED(m_pModel->FindAniIndex(&nIndex, m_dTimeCur, m_dTimeBgn)))
	{
		m_dTimeBgn = m_dTimeCur;
		nIndex = 0;
	}

	m_nAniCur = nIndex;

	RECT	rc;
	if(SUCCEEDED(m_pModel->FindImageRect(&rc, nIndex)))
	{
		m_AniRc = rc;
	}



	m_AniPos = m_vcPos;

	return 0;
}


void CGameObject::Render()
{
	if(m_pModel)
	{
		m_pModel->SetPos(m_AniPos);
		m_pModel->SetColor(m_AniColor);
		m_pModel->SetRect(&m_AniRc);
		m_pModel->Render();
	}

}



INT CGameObject::SetModel(char* sModel)
{
	if(FAILED(LcDev_ModelCreate("File", &m_pModel, sModel)))
		return -1;

	return 0;
}