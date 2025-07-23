

#include <windows.h>

#include <d3d9.h>
#include <d3dx9.h>

#include "ILcTexture.h"
#include "LcTexture.h"



LPDIRECT3DDEVICE9 CLcTexture::m_pDevice = NULL;

INT LcDev_TextureInit(void* pDev)
{
	CLcTexture::m_pDevice = (LPDIRECT3DDEVICE9)pDev;
	return 0;
}



void LcDev_TextureDestroy()
{
	CLcTexture::m_pDevice = NULL;
}



CLcTexture::CLcTexture()
{
	memset(&m_sFile, 0, sizeof m_sFile);
	memset(&m_Img, 0, sizeof m_Img);

	m_pTx = NULL;
}

CLcTexture::~CLcTexture()
{
	Destroy();
}


void CLcTexture::Destroy()
{
	if(m_pTx)
	{
		m_pTx->Release();
		m_pTx = NULL;
	}

	memset(&m_sFile, 0, sizeof m_sFile);
	memset(&m_Img, 0, sizeof m_Img);
}


INT CLcTexture::GetImageWidth()
{
	return m_Img.Width;
}

INT CLcTexture::GetImageHeight()
{
	return m_Img.Height;
}

void*  CLcTexture::GetTexture()
{
	return m_pTx;
}


const char* CLcTexture::GetName()
{
	return m_sFile;
}


INT CLcTexture::Create(void* p1, void* p2, void* p3, void* p4)
{
	DWORD	dColorKey = 0x0;
	
	strcpy(m_sFile, (char*)p1);
	dColorKey = *((DWORD*)p2);


	if( FAILED( D3DXCreateTextureFromFileEx(
						CLcTexture::m_pDevice		// 디바이스 포인터
						, m_sFile					// 텍스처 이름
						, D3DX_DEFAULT
						, D3DX_DEFAULT
						, 1							// 밉 레벨(2D의 경우 반드시 1)
						, 0
						, D3DFMT_UNKNOWN
						, D3DPOOL_MANAGED
						, 0x0000001					// 필터링
						, 0x0000001					// 밉 필터
						, dColorKey					// 컬러 키
						, &m_Img					// 텍스처 인포메이션
						, NULL
						, &m_pTx					// 텍스처 포인터
						)) )
	{
		memset(&m_sFile, 0, sizeof m_sFile);
		m_pTx = NULL;
		return -1;
	}

	return 0;
}


INT LcDev_TextureCreate(char* sCmd, ILcTexture** pData, void* pTextureName, DWORD dColorKey)
{
	*pData = NULL;

	CLcTexture* pObj = new CLcTexture;

	if(FAILED(pObj->Create(pTextureName, &dColorKey)))
	{
		delete pObj;
		return -1;
	}

	*pData = pObj;
	return 0;
}