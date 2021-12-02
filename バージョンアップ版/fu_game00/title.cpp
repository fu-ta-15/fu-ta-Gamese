//*****************************************************************************
//
// タイトル処理 [title.cpp]
// Author : SUZUKI FUUTA
//
//*****************************************************************************
//-----------------------------------------------------------------------------
// インクルードファイル
//-----------------------------------------------------------------------------
#include "title.h"
#include "scene2D.h"
#include "keyinput.h"
#include "manager.h"
#include "fade.h"
#include "player.h"
#include "mesh.h"
#include "move.h"
#include "sound.h"

//-----------------------------------------------------------------------------
// マクロ変数
//-----------------------------------------------------------------------------
#define TITLE_BG_SIZE		(D3DXVECTOR3(WIDTH_HALF, HEIGHT_HALF, 0.0f))
#define TITLE_BUTTON_POS	(D3DXVECTOR3(WIDTH_HALF, HEIGHT_HALF+250.0f, 0.0f))
#define TITLE_BUTTON_SIZE	(D3DXVECTOR3(150.0f, 50.0f, 0.0f))
#define TITLE_LOGO_POS		(D3DXVECTOR3(WIDTH_HALF-300, HEIGHT_HALF-100.0f, 0.0f))
#define TITLE_LOGO_SIZE		(D3DXVECTOR3(600.0f, 400.0f, 0.0f))

//-----------------------------------------------------------------------------
// 静的メンバ変数
//-----------------------------------------------------------------------------
CScene2D *CTitle::m_paTitleUI[UI_MAX] = {};
CMesh *CTitle::m_pTitleLogo = NULL;
CMesh3D *CTitle::m_pNote = NULL;

//=============================================================================
// コンストラクタ
//=============================================================================
CTitle::CTitle() : CScene(OBJ_NONE)
{
	m_ButtonCol = WhiteColor;	// 色の初期化
}

//=============================================================================
// デストラクタ
//=============================================================================
CTitle::~CTitle()
{
}

//=============================================================================
// タイトルの生成
//=============================================================================
CTitle * CTitle::Create(void)
{
	CTitle* pTitle = NULL;

	if (pTitle == NULL)
	{// NULLチェック

		// メモリ確保
		CTitle* pTitle = new CTitle;

		// 初期化処理
		pTitle->Init();
	}

	return pTitle;
}

//=============================================================================
// 初期化
//=============================================================================
HRESULT CTitle::Init(void)
{
	// 背景の生成
	m_paTitleUI[UI_BG] = CScene2D::Create(CENTER_POS, TITLE_BG_SIZE);
	m_paTitleUI[UI_BG]->CreateTexture("data/TEXTURE/BG.jpg");

	// Buttonの生成
	m_paTitleUI[UI_BUTTON] = CScene2D::Create(TITLE_BUTTON_POS, TITLE_BUTTON_SIZE);
	m_paTitleUI[UI_BUTTON]->CreateTexture("data/TEXTURE/Next_Mode.png");

	// LOGOの生成
	m_pTitleLogo = CMesh::Create(90, 0, TITLE_LOGO_POS, TITLE_LOGO_SIZE, CScene::OBJ_NONE);
	m_pTitleLogo->CreateTexture("data/TEXTURE/TitleUI.png");

	// サウンドの開始
	CSound *pSound = CManager::GetSound();
	pSound->PlaySound(CSound::SOUND_LABEL_BGM000);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CTitle::Uninit(void)
{
	// サウンドの削除
	CSound *pSound = CManager::GetSound();
	pSound->StopSound();

	// 削除
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CTitle::Update(void)
{
	// キー情報
	CKey *pKey = CManager::GetKey();

	// フェード情報
	CFade::FADE Fade = CFade::GetFade();   

	// カウントアップ
	m_nCntTime++;

	if (pKey->GetState(CKey::STATE_TRIGGER, DIK_SPACE) && Fade == CFade::FADE_NONE)
	{// トリガー・スペースを押したら・フェードの状態が何もない状態だったら
		CManager::GetFade()->SetFade(CManager::MODE_TUTORIAL);
	}

	// タイトルのLOGOの処理
	TitleLogo(m_nCntTime);

	// Buttonの処理
	ButtonUI();
}

//=============================================================================
// 描画処理
//=============================================================================
void CTitle::Draw(void)
{
	
}

//=============================================================================
// Buttonの処理
//=============================================================================
void CTitle::ButtonUI(void)
{
	// α値の更新
	m_paTitleUI[UI_BUTTON]->SetCol(m_ButtonCol);

	// α値の減算と加算
	m_ButtonCol.a += AddCol; 

	if (m_ButtonCol.a >= 1.0f)
	{// 減算させる値
		AddCol = -0.02f;
	}
	if (m_ButtonCol.a <= 0.0f)
	{// 加算させる値
		AddCol = 0.02f;
	}
}

//=============================================================================
// TitleLogoの処理
//=============================================================================
void CTitle::TitleLogo(int nTime)
{
	for (int nVtx = 0; nVtx < m_pTitleLogo->GetVtxNum(); nVtx++)
	{
		D3DXVECTOR3 pos = ZeroVector3;

		// Sin波の波の表現
		pos.y = Move::SinWave(HEIGHT_HALF - 350.0f, 25.0f,200.0f, ((float)nTime + nVtx));

		if (nVtx >= m_pTitleLogo->GetVtxNum() / 2)
		{// 頂点が底辺側に行ったら
			pos.y += 400.0f;
		}

		// 位置の更新
		m_pTitleLogo->SetVtxPosY(nVtx, pos.y);
	}
}
