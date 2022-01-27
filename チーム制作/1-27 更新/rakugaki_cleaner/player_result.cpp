//=============================================================================
//
// リザルトシーンのプレイヤー処理 [player_result.cpp]
// Author : 
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#include "player_result.h"
#include "manager.h"
#include "renderer.h"
#include "motion.h"
#include "camera.h"
#include "stage.h"
#include "player_marker.h"
#include "rank.h"
#include "shadow.h"

#include "keep_object.h"
#include "load_player_data.h"
#include "texture.h"
#include "result.h"

#include "debugproc.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MOVE_PLAYER		(1.8f)		// プレイヤーの移動量
#define JUMP_PLAYER		(15.0f)		// プレイヤーのジャンプ量
#define GRAVITY_PLAYER	(1.0f)		// プレイヤーの重力
#define PI_QUARTER		(D3DX_PI / 4.0f)

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************

//=============================================================================
// CPlayer_Resultのコンストラクタ
//=============================================================================
CPlayer_Result::CPlayer_Result(int nPriority) : CPlayer(nPriority)
{

}

//=============================================================================
// CPlayer_Resultのデストラクタ
//=============================================================================
CPlayer_Result::~CPlayer_Result()
{

}

//=============================================================================
// 生成処理
//=============================================================================
CPlayer_Result *CPlayer_Result::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, int nNumber)
{
	// ポインタ変数の生成
	CPlayer_Result *pPlayer_Result;
	pPlayer_Result = new CPlayer_Result(CScene::PRIORITY_PLAYER);

	// NULLチェック
	if (pPlayer_Result != NULL)
	{
		// 何番目か保存
		pPlayer_Result->SetNumber(nNumber);

		// 初期化処理
		pPlayer_Result->Init(pos, size);

		// オブジェクトタイプの設定
		pPlayer_Result->SetObjType(OBJTYPE_PLAYER);
	}

	return pPlayer_Result;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CPlayer_Result::Init(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	// プレイヤーの初期化
	CPlayer::Init(pos,size);

	// メンバ変数の初期化
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pShadow = CShadow::Create(pos, D3DXVECTOR2(25.0f, 25.0f));

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CPlayer_Result::Uninit(void)
{
	// プレイヤーの終了処理
	CPlayer::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CPlayer_Result::Update(void)
{
	// プレイヤーの更新処理
	CPlayer::Update();

	// 重力
	m_move.y -= GRAVITY_PLAYER;

	// 位置の取得
	D3DXVECTOR3 pos = GetPos();

	// 位置の更新
	pos += m_move;

	// 位置の設定
	SetPos(pos);

	// ステージとの当たり判定
	CollisionStage();
}

//=============================================================================
// 描画処理
//=============================================================================
void CPlayer_Result::Draw(void)
{
	// プレイヤーの描画処理
	CPlayer::Draw();
}

//=============================================================================
// ステージとの当たり判定
//=============================================================================
bool CPlayer_Result::CollisionStage(void)
{
	// 床情報の取得
	CScene *pScene = CScene::GetScene(CScene::PRIORITY_MODEL);

	// NULLチェック
	if (pScene != NULL)
	{
		while (pScene)
		{
			// 次の情報を保存
			CScene *pSceneNext = pScene->GetNext();

			// オブジェクトの種類が床だったら床にキャスト
			if (pScene->GetObjType() == CScene::OBJTYPE_STAGE)
			{
				CStage *pStage = (CStage*)pScene;

				// 位置の取得
				D3DXVECTOR3 pos = GetPos();

				// あたり判定
				if (pStage->Collision(&pos, GetSize()))
				{
					m_move.y = 0.0f;

					// 位置の設定
					SetPos(pos);

					return true;
				}
			}

			// 次の情報にする
			pScene = pSceneNext;
		}
	}
	return false;
}

//=============================================================================
// モーションの設定
//=============================================================================
bool CPlayer_Result::SetMotion(MotionType motionType)
{
	// モーションの種類によって処理を変える
	switch (motionType)
	{
		// ニュートラル
	case MOTIONTYPE_NEUTORAL:
		break;

		// 移動
	case MOTIONTYPE_MOVE:
		break;

		// アクション
	case MOTIONTYPE_DELETE:
		break;
	}

	if (GetMotionType() != motionType)
	{
		// モーションの種類設定
		SetMotionType(motionType);

		// 現在のモーション情報の取得
		CMotion::MOTION motion = m_pMotion->GetMotion();

		// モーションカウンタを初期化
		motion.nCounterMotion = 0;

		// 現在のキーを初期化
		motion.nKey = 0;

		// 現在のモーション情報を設定
		m_pMotion->SetMotion(motion);

		return true;
	}
	return false;
}

//=============================================================================
// プレイヤーマーカー情報の設定
//=============================================================================
void CPlayer_Result::SetPlayerMarker(CPlayerMarker *pPlayerMarker, CCamera *pCamera)
{
	m_pPlayerMarker = pPlayerMarker;

	m_pPlayerMarker->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	m_pPlayerMarker->SetTex(D3DXVECTOR2((float)GetNumber(), 1));

	D3DXVECTOR3 pos = pCamera->ChangePos_WorldtoScreen(GetPos());
	m_pPlayerMarker->SetPosition(D3DXVECTOR3(pos.x, pos.y + 10.0f, pos.z));
}

//=============================================================================
// ランク情報の設定
//=============================================================================
void CPlayer_Result::SetRank(CRank *pRank, int nRank, CCamera *pCamera)
{
	m_pRank = pRank;

	m_pRank->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	m_pRank->SetTex(D3DXVECTOR2((float)nRank, 1));

	// マーカーの位置を変更
	D3DXVECTOR3 pos = pCamera->ChangePos_WorldtoScreen(GetPos());
	m_pRank->SetPosition(D3DXVECTOR3(pos.x, pos.y - 400.0f, pos.z));
}