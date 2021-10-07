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
#include "collision.h"
#include "player.h"

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

//=============================================================================
// コンストラクタ
//=============================================================================
CEnemy::CEnemy() : CScene2D(OBJ_ENEMY)
{

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
	CScene2D::Uninit();	// 終了処理
}

//=============================================================================
// 更新処理
//=============================================================================
void CEnemy::Update(void)
{
	if (this->CollisionPlayer() == true && GET_PLAYER->GetState() != CPlayer::STATE_KNOCKUP)
	{
		if (this->GetEnemyType() == ENEMY_BLACK)
		{
			GET_PLAYER->SetCollEnemy(true);
			this->Release();
		}
		else if (this->GetEnemyType() == ENEMY_WHITE)
		{
			GET_PLAYER->SetStock();
			this->Release();
		}
	}
	CollisionEnemy();
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
void CEnemy::CollisionEnemy(void)
{
	if (this->GetBool() == true)
	{
		CBoss *pBoss = BOSS;

		// 敵の種類
		switch (this->m_type)
		{
		case ENEMY_TYPE0:

			this->DamegeLife(1);

			if (this->GetLife() == 0)
			{
				this->m_type = ENEMY_WHITE;
			}
			break;

		case ENEMY_TYPE1:
			break;

		case ENEMY_TYPE2:

			if (pBoss->GetState() == BOSS_NONE && pBoss->GetShield() == false)
			{
				pBoss->DamegeLife(1);
				pBoss->SetState(BOSS_DAMAGE);
			}
			else if (pBoss->GetState() == BOSS_NOT_DAMAGE)
			{
				pBoss->SetShield(true);
			}

			if (pBoss->GetLife() == 0)
			{
				pBoss->SetAlive(false);
				pBoss->Release();
			}
			break;
		}
		this->SetBool(false);
	}
}

//=============================================================================
// プレイヤーとの当たり判定
//=============================================================================
bool CEnemy::CollisionPlayer(void)
{
	bool bCollision = false;

	D3DXVECTOR3 pos = CGame::GetPlayer()->GetPos();
	D3DXVECTOR3 size = CGame::GetPlayer()->GetSize();

	bCollision = CCollision::CollisionCycle(m_pos, pos, size.x);

	return bCollision;
}
