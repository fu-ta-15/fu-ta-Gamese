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
#include "scene2D.h"
#include "player.h"
#include "sound.h"

//-----------------------------------------------------------------------------
// マクロ変数
//-----------------------------------------------------------------------------
#define PLAYER_SIZE		(D3DXVECTOR3(20.0f, 20.0f, 0.0f))
#define PLAYER_POS		(D3DXVECTOR3(SCREEN_WIDTH / 3, SCREEN_HEIGHT / 3, 0.0f))

#define BG_SIZE			(D3DXVECTOR3(WIDTH_HALF,HEIGHT_HALF,0.0f))

#define FIELD_POS		(D3DXVECTOR3(0.0f, 668.0f, 0.0f))
#define FIELD_SIZE		(D3DXVECTOR3(WIDTH_HALF+100.0f, 100.0f, 0.0f))
#define FIELD_VERTICAL	(40)
#define FIELD_SIDE		(0)

#define OPE_POS			(D3DXVECTOR3((100.0f+(150.0f*nCnt)), 100.0f, 0.0f))
#define OPE_SIZE		(D3DXVECTOR3(70.0f, 70.0f, 0.0f))
#define OPE_VERTICAL	(30)
#define OPE_SIDE		(0)

//-----------------------------------------------------------------------------
// 静的メンバ変数
//-----------------------------------------------------------------------------
CPlayer*	CTutorial::m_pPlayer = NULL;
CScene2D*	CTutorial::m_pBg = NULL;
CMesh*		CTutorial::m_pField = NULL;
CBoss*		CTutorial::m_pBoss = NULL;
CMesh*		CTutorial::m_pOperation[OPERA_MAX] = {};
bool		CTutorial::m_OperationStop = false;

//=============================================================================
// コンストラクタ
//=============================================================================
CTutorial::CTutorial() : CScene(OBJ_NONE)
{

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
	CSound *pSound = CManager::GetSound();

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
	CKey *pKey = CManager::GetKey();	   // キー情報
	CFade::FADE Fade = CFade::GetFade();   // フェード情報


	if (pKey->GetState(CKey::STATE_RELEASE, DIK_SPACE) && Fade == CFade::FADE_NONE)
	{// RETURNキーが押されたら
		CManager::GetFade()->SetFade(CManager::MODE_GAME);	// GAME・MODEへ
	}
	// 初めて押されたキーの確認
	OnKeyOperat(pKey);
	OperatUpdate();
}

//=============================================================================
// 描画処理
//=============================================================================
void CTutorial::Draw(void)
{
}

//=============================================================================
// そのキーが初めて押されたらメッシュを揺らすための確認
//=============================================================================
void CTutorial::OnKeyOperat(CKey *pKey)
{// そのキーを押されたら、MOVEオンにする
	if (pKey->GetState(CKey::STATE_TRIGGER, DIK_A))			{ m_bButton[KEY_A] = true; }
	if (pKey->GetState(CKey::STATE_TRIGGER, DIK_D))			{ m_bButton[KEY_D] = true; }
	if (pKey->GetState(CKey::STATE_TRIGGER, DIK_W))			{ m_bButton[KEY_W] = true; }
	if (pKey->GetState(CKey::STATE_TRIGGER, DIK_NUMPAD6))	{ m_bButton[KEY_NUM_6] = true; }
}

//=============================================================================
// 説明用画像の更新
//=============================================================================
void CTutorial::OperatUpdate(void)
{
}
