//*****************************************************************************
//
// チュートリアル処理 [tutorial.cpp]
// Author : SUZUKI FUUTA
//
//*****************************************************************************
//-----------------------------------------------------------------------------
// インクルードファイル
//-----------------------------------------------------------------------------
#include "tutorial.h"
#include "keyinput.h"
#include "manager.h"
#include "fade.h"
#include "sound.h"
#include "mesh3D.h"

//-----------------------------------------------------------------------------
// 静的メンバ変数
//-----------------------------------------------------------------------------
CMesh3D*	CTutorial::m_pTutorialMesh3D[3] = {};

//=============================================================================
// コンストラクタ
//=============================================================================
CTutorial::CTutorial() : CScene(OBJ_NONE)
{
	// メンバ変数の設定
	m_nCntCycleLimit = 180;
	m_bNextPage = false;
	m_nPageID = 0;
}

//=============================================================================
// デストラクタ
//=============================================================================
CTutorial::~CTutorial()
{

}

//=============================================================================
// チュートリアルの生成
//=============================================================================
CTutorial * CTutorial::Create(void)
{
	// メモリ確保
	CTutorial* pTutorial = new CTutorial();

	// 初期化処理
	pTutorial->Init();

	return pTutorial;
}

//=============================================================================
// 初期化
//=============================================================================
HRESULT CTutorial::Init(void)
{
	// チュートリアルの画面
	for (int nCnt = 0; nCnt < 3; nCnt++)
	{
		m_pTutorialMesh3D[nCnt] = CMesh3D::Create(40, 40, D3DXVECTOR3(10.0f, (-nCnt*0.1f) , -5.0f), D3DXVECTOR3(150.0f, 0.0f, 150.0f));
	}
	m_pTutorialMesh3D[0]->CreateTexture("data/TEXTURE/tutorial.png");
	m_pTutorialMesh3D[1]->CreateTexture("data/TEXTURE/tutorial2.png");
	m_pTutorialMesh3D[2]->CreateTexture("data/TEXTURE/tutorial3.png");

	// サウンドの開始
	CSound *pSound = CManager::GetSound();
	pSound->PlaySound(CSound::SOUND_LABEL_BGM001);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CTutorial::Uninit(void)
{
	// サウンドの停止
	CSound *pSound = CManager::GetSound();
	pSound->StopSound();

	//オブジェクトの破棄
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CTutorial::Update(void)
{
	// キー情報
	CKey *pKey = CManager::GetKey();	   

	// フェード情報
	CFade::FADE Fade = CFade::GetFade();   

	// SPACEキーが押されたら
	if (pKey->GetState(CKey::STATE_RELEASE, DIK_SPACE) && Fade == CFade::FADE_NONE)
	{
		// GAME・MODEへ
		CManager::GetFade()->SetFade(CManager::MODE_GAME);	
	}

	// RETURNキーが押されたら
	if (pKey->GetState(CKey::STATE_RELEASE, DIK_RETURN) && Fade == CFade::FADE_NONE)
	{
		// 次のページに行く合図
		m_bNextPage = true;
	}

	// 次のページへ
	if (m_bNextPage)
	{
		// メッシュの回転
		m_pTutorialMesh3D[m_nPageID]->MeshCycleMove(m_nCntCycleLimit);

		// 右に回転させるためにマイナスの加算
		m_nCntCycleLimit--;
	}
	if (m_nCntCycleLimit < 0)
	{
		m_bNextPage = false;
		m_nCntCycleLimit = 180;
		m_nPageID++;
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CTutorial::Draw(void)
{

}
