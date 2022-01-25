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
int CTextureScene::m_nListSize = 0;

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
// ドロップされたテクスチャファイルの生成
//=============================================================================
void CTextureScene::FileDrop(const char * sTexName)
{
	// リターン用のクラス変数
	CTextureScene *pTexScene = new CTextureScene;

	// 文字数を確認しchar型の変数を動的に確保
	size_t Length = strlen(sTexName);
	pTexScene->m_sLink = new char[Length];

	// 名前のコピー
	strcpy(pTexScene->m_sLink, sTexName);

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, pTexScene->m_sLink, &pTexScene->m_pTexture);

	// NULLチェック
	if (pTexScene->m_pTexture)
	{
		// 確認用
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
CTextureScene * CTextureScene::GetTexScene(int nID)
{
	// 先頭の要素のイテレーターの取得
	list<CTextureScene*>::iterator it = m_TextureScene.begin();

	// 指定された番地の要素まで進める
	for (int nCnt = 0; nCnt < nID; nCnt++)
	{
		// 進める
		it++;
	}

	// 指定された要素を返す
	return *it;
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
