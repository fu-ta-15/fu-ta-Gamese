//*****************************************************************************
//
// 敵処理 [scene.cpp]
// Author : SUZUKI FUUTA
//
//*****************************************************************************

//-----------------------------------------------------------------------------
// インクルードファイル
//-----------------------------------------------------------------------------
#include "enemy.h"
#include "boss.h"
#include "normalenemy.h"
#include "game.h"

//-----------------------------------------------------------------------------
// マクロ変数
//-----------------------------------------------------------------------------
#define BOSS				(CGame::GetBoss())
#define BOSS_DAMAGE			(CBoss::STATE_DAMAGE)
#define BOSS_NOT_DAMAGE		(CBoss::STATE_NOT_DAMAGE)
#define BOSS_NONE			(CBoss::STATE_NONE)
#define BOSS_GET_LIFE		(CGame::GetBoss()->GetLife())

#define ENEMY_ID			(m_paEnemy[nID])
#define ENEMY_CNT			(m_paEnemy[nCnt])

//-----------------------------------------------------------------------------
// 静的変数
//-----------------------------------------------------------------------------
CEnemy *CEnemy::m_paEnemy[MAX_OBJECT] = {};

//=============================================================================
// コンストラクタ
//=============================================================================
CEnemy::CEnemy() : CScene2D(OBJ_ENEMY)
{
	for (int nCnt = 0; nCnt < MAX_OBJECT; nCnt++)
	{
		if (ENEMY_CNT == NULL)
		{// 中身が空っぽの場合
			ENEMY_CNT = this;	// 情報代入
			break;
		}
	}
}

//=============================================================================
// デストラクタ
//=============================================================================
CEnemy::~CEnemy()
{
}

//=============================================================================
// 初期化
//=============================================================================
HRESULT CEnemy::Init(void)
{
	CScene2D::Init(m_pos, m_size);	// ポリゴンの生成
	CScene2D::SetSize(m_size);		// サイズの設定
	CScene2D::SetCol(m_col);		// 色の設定
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CEnemy::Uninit(void)
{
	for (int nCnt = 0; nCnt < MAX_OBJECT; nCnt++)
	{// 中身をNULLにする
		ENEMY_CNT = NULL;
	}
	CScene2D::Uninit();	// 終了処理
}

//=============================================================================
// 更新処理
//=============================================================================
void CEnemy::Update(void)
{

}

//=============================================================================
// 描画処理
//=============================================================================
void CEnemy::Draw(void)
{
	CScene2D::Draw();	// 描画処理
}

//=============================================================================
// 種類ごとの当たり判定後の処理
//=============================================================================
void CEnemy::CollisionEnemy(int nID)
{
	// 敵の種類
	switch (ENEMY_ID->m_type)
	{
	case ENEMY_TYPE0:

		ENEMY_ID->DamegeLife(1);

		if (ENEMY_ID->GetLife() == 0)
		{
			ENEMY_ID->m_type = ENEMY_WHITE;
		}
		break;

	case ENEMY_TYPE1:
		//CScene::ObjRelease(OBJ_ENEMY, nID);
		//ENEMY_ID = NULL;
		break;

	case ENEMY_TYPE2:

		if (BOSS->GetState() == BOSS_NONE && BOSS->GetShield() == false)
		{
			BOSS->DamegeLife(1);
			BOSS->SetState(BOSS_DAMAGE);
		}
		else if (BOSS->GetState() == BOSS_NOT_DAMAGE)
		{
			BOSS->SetShield(true);
		}

		if (BOSS_GET_LIFE == 0)
		{
			BOSS->SetAlive(false);
			CScene::ObjRelease(OBJ_ENEMY, nID);
			m_paEnemy[nID] = NULL;
		}
		break;
	}
}
