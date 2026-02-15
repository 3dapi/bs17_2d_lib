// Interface for the ILcSpriteX class.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _ILcSpriteX_H_
#define _ILcSpriteX_H_


#ifndef interface
#define interface	struct
#endif

namespace LcsLib
{

interface ILcSpriteX
{
	virtual ~ILcSpriteX(){};

	virtual	int		Begin(DWORD=0)=0;
	virtual	int		End(DWORD=0)=0;

	virtual	int		OnResetDevice()=0;
	virtual	int		OnLostDevice()=0;

	virtual	int		Draw( void* pTex				// Texture
						, RECT* pRct				// Draw Region
						, void* pScl=NULL			// Scaling
						, void* pTrn=NULL			// Position
						, DWORD dColor=0xFFFFFFFF	// color
						, int   bMono=0				// Monotone
						)=0;

	virtual	int		DrawEx( void* pTex				// Texture
						, RECT* pRct				// Draw Region
						, void* pScl				// Scaling
						, void* pTrn				// Position
						, void* pRot				// Rotation Center
						, FLOAT	fAngle				// Rotation Angle(Radian)
						, DWORD dColor=0xFFFFFFFF	// color
						, int   bMono=0				// Monotone
						)=0;

	virtual	void	AlphaOption(int opt=0)=0;
};



int	LcDev_SpriteCreate(char* sCmd
					, ILcSpriteX** pData
					, void* p1=NULL
					, void* p2=NULL
					, void* p3=NULL
					, void* p4=NULL
					);


}//namespace LcsLib

#endif

