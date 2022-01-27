//=============================================================================
//
// プレスロゴ処理 [press_logo.cpp]
// Author : 
//
//=============================================================================
#include "press_logo.h"

#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "input_keyboard.h"
#include "input_joypad.h"

#include "base.h"
#include "title.h"

//=============================================================================
// CPressLogoのコンストラクタ
//=============================================================================
CPressLogo::CPressLogo(int nPriority) : CScene2D(nPriority)
{
	// メンバ変数の初期化
	m_nCntInterval = 0;
	m_nCntSwap = 0;
	m_bPressButton = false;
	m_bCounter = true;
}

//=============================================================================
// CPressLogoのデストラクタ
//=============================================================================
CPressLogo::~CPressLogo()
{

}

//=============================================================================
// 生成処理
//=============================================================================
CPressLogo *CPressLogo::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, int nCntSwap, int nTex)
{
	// ポインタ変数の生成
	CPressLogo *pPressLogo;
	pPressLogo = new CPressLogo(CScene::PRIORITY_UI);

	// NULLチェック
	if (pPressLogo != NULL)
	{
		// 初期化処理
		pPressLogo->Init(pos, size, nCntSwap, nTex);

		// オブジェクトタイプの設定
		pPressLogo->SetObjType(OBJTYPE_LOGO);
	}

	return pPressLogo;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CPressLogo::Init(D3DXVECTOR3 pos, D3DXVECTOR2 size, int nCntSwap, int nTex)
{
	// CScene2Dの初期化処理
	CScene2D::Init(pos, size);

	// テクスチャ管理情報の取得
	CTexture *pTex = CManager::GetTexture();

	// 情報がNULLじゃないときテクスチャを適用する
	if (pTex != NULL)
	{
		BindTexture(pTex->GetAddress(nTex));
	}

	// メンバ変数の初期化
	m_nCntInterval = 0;
	m_nCntSwap = nCntSwap;
	m_bPressButton = false;
	m_bCounter = true;
	m_bTransition = false;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CPressLogo::Uninit(void)
{
	// CScene2Dの終了処理
	CScene2D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CPressLogo::Update(void)
{
	// CScene2Dの更新処理
	CScene2D::Update();

	// カウント更新
	if (m_bCounter == true)
	{
		m_nCntInterval++;
	}

	// 色変更
	ChangeColor();
}

//=============================================================================
// 描画処理
//=============================================================================
void CPressLogo::Draw(void)
{
	// CScene2Dの描画処理
	CScene2D::Draw();
}

//=============================================================================
// ボタンを押したかどうか
//=============================================================================
void CPressLogo::PressButton(void)
{
	// trueにする
	m_bPressButton = true;

	// カウントを初期化
	m_nCntInterval = 0;
}

//=============================================================================
// 色変更処理
//=============================================================================
void CPressLogo::ChangeColor(void)
{
	// 点滅させる
	if (m_bPressButton != true)
	{
		// ボタンを押していないとき
		if (m_nCntInterval % (m_nCntSwap * 2) == 0)
		{
			SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));		// 透明
		}
		else if (m_nCntInterval % (m_nCntSwap * 2) == m_nCntSwap)
		{
			SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));		// 不透明
		}
	}
	else
	{
		// ボタンを押したとき
		if (m_nCntInterval % (m_nCntSwap / 2) == 0)
		{
			SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));		// 透明
		}
		else if (m_nCntInterval % (m_nCntSwap / 2) == (m_nCntSwap / 4))
		{
			SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));		// 不透明
		}

		// カウントが60以上の時
		if (m_nCntInterval > 60)
		{
			m_bTransition = true;
		}
	}
}

//=============================================================================
// 色変更処理
//=============================================================================
void CPressLogo::SetStartCnt(bool bStart)
{
	// 保存
	m_bCounter = bStart;

	// カウンタを初期化
	m_nCntInterval = 0;
}