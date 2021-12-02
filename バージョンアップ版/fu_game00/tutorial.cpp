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
#include "mesh.h"
#include "sound.h"
#include "boss.h"
#include "mesh3D.h"

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
CMesh3D*	CTutorial::m_pTutorialMesh3D = NULL;

//=============================================================================
// コンストラクタ
//=============================================================================
CTutorial::CTutorial() : CScene(OBJ_NONE)
{
	m_nCntCycleLimit = 180;
	m_bNextPage = false;
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

	//m_pBg = CScene2D::Create(CENTER_POS, BG_SIZE);													// 背景
	//m_pField = CMesh::Create(FIELD_VERTICAL, FIELD_SIDE, FIELD_POS, FIELD_SIZE,CScene::OBJ_NONE);	// 地面
	//m_pPlayer = CPlayer::Create(PLAYER_POS, PLAYER_SIZE);											// プレイヤー
	//m_pBoss = CBoss::Create(BOSS_POS, BOSS_SIZE, BOSS_LIFE);										// ボス

	//for (int nCnt = 0; nCnt < OPERA_MAX; nCnt++)
	//{// 操作説明画像
	//	D3DXVECTOR3 pos = D3DXVECTOR3((100.0f + (150.0f*nCnt)), 100.0f, 0.0f);						// 位置の更新
	//	m_pOperation[nCnt] = CMesh::Create(OPE_VERTICAL, OPE_SIDE, pos, OPE_SIZE,CScene::OBJ_NONE); // ポリゴンの生成
	//	m_bButton[nCnt] = false;																	// ウェーブの有無
	//}

	//// 画像の貼り付け
	//m_pBoss->CreateTexture("data/TEXTURE/Boss_tutorial.png");
	//m_pBg->CreateTexture("data/TEXTURE/BG.png");
	//m_pField->CreateTexture("data/TEXTURE/Field.png");
	//m_pOperation[KEY_A]->CreateTexture("data/TEXTURE/Move_a.png");
	//m_pOperation[KEY_D]->CreateTexture("data/TEXTURE/Move_d.png");
	//m_pOperation[KEY_W]->CreateTexture("data/TEXTURE/Move_w.png");
	//m_pOperation[KEY_NUM_6]->CreateTexture("data/TEXTURE/Bullet_6.png");

	m_pTutorialMesh3D = CMesh3D::Create(40, 40, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(200.0f, 0.0f, 200.0f));

	// サウンドの開始
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
	CKey *pKey = CManager::GetKey();	   // キー情報
	CFade::FADE Fade = CFade::GetFade();   // フェード情報


	if (pKey->GetState(CKey::STATE_RELEASE, DIK_SPACE) && Fade == CFade::FADE_NONE)
	{// RETURNキーが押されたら
		CManager::GetFade()->SetFade(CManager::MODE_GAME);	// GAME・MODEへ
	}

	if (pKey->GetState(CKey::STATE_RELEASE, DIK_RETURN) && Fade == CFade::FADE_NONE)
	{
		m_bNextPage = true;
	}


	if (m_bNextPage)
	{
		m_pTutorialMesh3D->MeshCycleMove(m_nCntCycleLimit);
		m_nCntCycleLimit--;
	}

	// 初めて押されたキーの確認
	//OnKeyOperat(pKey);
	//OperatUpdate();
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
	//if (pKey->GetState(CKey::STATE_TRIGGER, DIK_A))			{ m_bButton[KEY_A] = true; }
	//if (pKey->GetState(CKey::STATE_TRIGGER, DIK_D))			{ m_bButton[KEY_D] = true; }
	//if (pKey->GetState(CKey::STATE_TRIGGER, DIK_W))			{ m_bButton[KEY_W] = true; }
	//if (pKey->GetState(CKey::STATE_TRIGGER, DIK_NUMPAD6))	{ m_bButton[KEY_NUM_6] = true; }
}

//=============================================================================
// 説明用画像の更新
//=============================================================================
void CTutorial::OperatUpdate(void)
{
	//// カウントを加算
	//m_nCntTimeOP++;

	//for (int nCnt = 0; nCnt < OPERA_MAX; nCnt++)
	//{
	//	if (m_bButton[nCnt] == true)
	//	{// そのキーが押されたことがわかっていたら
	//		for (int nVtx = 0; nVtx < m_pOperation[nCnt]->GetVtxNum()/2; nVtx++)
	//		{// 波を起こす処理
	//			D3DXVECTOR3 pos = ZeroVector3;
	//			pos.y = Move::SinWave(100.0f, 10.0f, 60.0f, (float)m_nCntTimeOP + nVtx);
	//			m_pOperation[nCnt]->SetVtxPosY(nVtx, pos.y);
	//			pos.y += 70.0f;
	//			m_pOperation[nCnt]->SetVtxPosY(nVtx + (m_pOperation[nCnt]->GetVtxNum() / 2), pos.y);
	//		}
	//	}
	//}
}
