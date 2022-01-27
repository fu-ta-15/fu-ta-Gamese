//=============================================================================
//
// テクスチャ管理処理 [texture.cpp]
// Author : 
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "texture.h"
#include "manager.h"
#include "renderer.h"
#include "stdio.h"

#include "debugproc.h"

//=============================================================================
// CTextureのコンストラクタ
//=============================================================================
CTexture::CTexture()
{
	// メンバ変数の初期化
	m_pTexture = NULL;
}

//=============================================================================
// CTextureのデストラクタ
//=============================================================================
CTexture::~CTexture()
{

}

//=============================================================================
// 読み込み処理
//=============================================================================
HRESULT CTexture::Load(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// メモリの確保
	m_pTexture = (LPDIRECT3DTEXTURE9) new LPDIRECT3DTEXTURE9[128];
	m_apTexFileName = new char*[128];

	for (int nCnt = 0; nCnt < 128; nCnt++)
	{
		m_apTexFileName[nCnt] = new char[128];
	}

	m_nNumtextureInfo = -1;
	m_nNumFileName = 0;

	return S_OK;
}

//=============================================================================
// 破棄処理
//=============================================================================
void CTexture::Unload(void)
{
	for (int nCntTex = 0; nCntTex < m_nNumtextureInfo; nCntTex++)
	{
		if (*(LPDIRECT3DTEXTURE9*)&m_pTexture[nCntTex] != NULL)
		{
			(*(LPDIRECT3DTEXTURE9*)&m_pTexture[nCntTex])->Release();
			*(LPDIRECT3DTEXTURE9*)&m_pTexture[nCntTex] = NULL;
		}
	}
	delete[] m_pTexture;
	m_pTexture = NULL;
}

//=============================================================================
// 更新処理
//=============================================================================
void CTexture::Update(void)
{
	CDebugProc::Print("\n--- テクスチャ情報 ---\n");
	CDebugProc::Print("テクスチャ数 : %d\n",m_nNumtextureInfo + 1);
}

//=============================================================================
// テクスチャ情報取得
//=============================================================================
LPDIRECT3DTEXTURE9 CTexture::GetAddress(int nIdx)
{
	return *(LPDIRECT3DTEXTURE9*)&m_pTexture[nIdx];
}

//=============================================================================
// テクスチャ読み込み処理
//=============================================================================
int CTexture::SetTexture(char *pTexFileName)
{
	// 同じ名前がないか探す
	for (int nCnt = 0 ; nCnt < m_nNumFileName; nCnt++)
	{
  		if (strcmp(&m_apTexFileName[nCnt][0],&pTexFileName[0]) == 0)
		{
			return nCnt;
		}
	}

	// テクスチャファイルネームの保存
	strcpy(&m_apTexFileName[m_nNumFileName][0], pTexFileName);
	m_nNumFileName++;

	// テクスチャ数のカウント
	m_nNumtextureInfo++;

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャの読み込み	
 	D3DXCreateTextureFromFile(pDevice, pTexFileName, (LPDIRECT3DTEXTURE9*)&m_pTexture[m_nNumtextureInfo]);

	return m_nNumtextureInfo;
}
