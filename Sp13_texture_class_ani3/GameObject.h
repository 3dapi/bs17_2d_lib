//
////////////////////////////////////////////////////////////////////////////////


#ifndef _GameObject_H_
#define _GameObject_H_


#ifndef interface
#define struct interface
#endif

interface IGameObject
{
	virtual ~IGameObject(){};

	virtual INT		Create() = 0;
	virtual void	Destroy() = 0;

	virtual INT		FrameMove() = 0;
	virtual void	Render() = 0;

	virtual	INT		SetModel(char* sModel)=0;
	virtual	void	SetPos(const FLOAT*)=0;
};



class CGameObject : public IGameObject
{
protected:
	D3DXVECTOR3		m_vcPos		;
	ILcModel*		m_pModel	;

	INT				m_bAni		;		// Animation or Not
	DWORD			m_dTimeBgn	;		// 시작 타임
	DWORD			m_dTimeCur	;		// 현재 타임
	INT				m_nAniCur	;		// Current Ani Index

	D3DXVECTOR3		m_AniPos	;		// Animation Position
	RECT			m_AniRc		;		// Animation Image Rect
	DWORD			m_AniColor	;		// Animation Color

public:
	CGameObject();
	virtual ~CGameObject();

	virtual INT		Create();
	virtual void	Destroy();

	virtual INT		FrameMove();
	virtual void	Render();

	virtual	INT		SetModel(char* sModel);
	virtual	void	SetPos(const FLOAT* pPos);
};



#endif


