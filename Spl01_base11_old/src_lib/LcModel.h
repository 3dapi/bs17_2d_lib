//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef _LcModel_H_
#define _LcModel_H_


#pragma warning(disable: 4786)
#include <vector>


namespace LcsLib
{
	
class CLcModel2D : public ILcModel
{
public:

	struct TAniInfo
	{
		DWORD	dTime;				// RECT 애니 이미지
		RECT	ImgRc;				// RECT 애니 이미지

		TAniInfo(){};
		TAniInfo(DWORD v1, RECT v2)
		{
			dTime = v1;
			ImgRc = v2;
		}
	};

	typedef std::vector<TAniInfo >	lsAniInfo;
	typedef lsAniInfo::iterator		itAniInfo;

protected:
	INT						m_nID;
	char					m_sFile[128];
	INT						m_AniTot	;	// Ani Total Number
	DWORD					m_AniDelta	;	// 시간 간격;
	ILcTexture*				m_AniTex	;	// 애니 텍스처
	std::vector<TAniInfo >	m_AniLst	;	// Animation List


	//for Rendering
	RECT			m_ImgRc		;	// Rendering RECT
	D3DXVECTOR3		m_vcPos		;	// Rendering Position
	DWORD			m_dColor	;	// color

public:
	CLcModel2D();
	virtual ~CLcModel2D();

	virtual INT		Create(void* p1=NULL, void* p2=NULL, void* p3=NULL, void* p4=NULL);
	virtual void	Destroy();
	virtual void	Render();

	virtual	INT			GetID();
	virtual const char*	GetName();

	virtual	void	SetPos(const FLOAT* =NULL);
	virtual	void	SetColor(const DWORD=0xFFFFFFFF);
	virtual	void	SetRect(void* rc=NULL);
	
 	virtual	INT		FindAniIndex(INT* pOut/*Out*/, DWORD dTimeCur, DWORD dTimeBgn);
	virtual	INT		FindImageRect(void* pOut/*Out*/, INT nIndex);

	
	typedef std::vector<ILcModel*>		lsModel;
	typedef lsModel::iterator			itModel;
	
	static INT			m_nIDModel;		// Model9 ID
	static LPD3DXSPRITE	m_pSprite;
	static lsModel*		m_vModel;
};

ILcModel*	LcDev_ModelFind(char* sFile);

}//namespace LcsLib

#endif

