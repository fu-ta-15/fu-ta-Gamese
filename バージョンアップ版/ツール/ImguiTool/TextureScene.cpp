//*****************************************************************************
//
// テクスチャ処理 [TextureScene.cpp]
// Author : SUZUKI FUUTA
//
//*****************************************************************************
//-----------------------------------------------------------------------------
// インクルードファイル
//-----------------------------------------------------------------------------
#include "TextureScene.h"
#include "manager.h"
#include "renderer.h"
#include "keyinput.h"

//-----------------------------------------------------------------------------
// 静的メンバ変数
//-----------------------------------------------------------------------------
list<CTextureScene*> CTextureScene::m_TextureScene;

//=============================================================================
// コンストラクタ
//=============================================================================
CTextureScene::CTextureScene()
{
	m_sLink = NULL;
	m_pTexture = NULL;
}

//=============================================================================
// デストラクタ
//=============================================================================
CTextureScene::~CTextureScene()
{

}

//=============================================================================
// テクスチャのロード
//=============================================================================
void CTextureScene::LoadTexture(void)
{

}

//=============================================================================
// テクスチャのアンロード
//=============================================================================
void CTextureScene::UnLoadTexture(void)
{

}

//=============================================================================
// テクスチャの生成
//=============================================================================
void CTextureScene::ListInTexture(const char * sTexName)
{
	// 文字数を確認しchar型の変数を動的に確保
	size_t Length = strlen("TEXTURE/") + strlen(sTexName);
	char *sLink = new char[Length];

	// 文字列の合体
	strcpy(sLink, "TEXTURE/");
	strcat(sLink, sTexName);

	// リストへプッシュバックするための変数に情報代入
	CTextureScene *pTexScene = TextureListIn(sLink);

	if (pTexScene->m_pTexture)
	{
		printf("\n生成完了");
		// リストへプッシュバックする
		m_TextureScene.push_back(pTexScene);
	}

	// 中身のサイズを確認
	m_nListSize = m_TextureScene.size();
}

//=============================================================================
// ドロップされたテクスチャファイルの生成
//=============================================================================
void CTextureScene::FileDrop(const char * sTexName)
{
	// リターン用のクラス変数
	CTextureScene *pTexScene = new CTextureScene;

	// 名前のコピー
	pTexScene->m_sLink = (char*)sTexName;

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, pTexScene->m_sLink, &pTexScene->m_pTexture);

	if (pTexScene->m_pTexture)
	{
		printf("\n生成完了");
		// リストへプッシュバックする
		m_TextureScene.push_back(pTexScene);
	}

	// 中身のサイズを確認
	m_nListSize = m_TextureScene.size();
}

//=============================================================================
// テクスチャの生成
//=============================================================================
CTextureScene *CTextureScene::TextureListIn(const char *sTexName)
{
	// リターン用のクラス変数
	CTextureScene *pTexScene = new CTextureScene;

	// 名前のコピー
	pTexScene->m_sLink = (char*)sTexName;

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, pTexScene->m_sLink, &pTexScene->m_pTexture);

	return pTexScene;
}
