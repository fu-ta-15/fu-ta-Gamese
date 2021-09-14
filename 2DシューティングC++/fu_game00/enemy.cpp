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
// 静的変数
//-----------------------------------------------------------------------------
CEnemy *CEnemy::m_paEnemy[MAX_OBJECT] = {};
CEnemy::ENEMY CEnemy::m_paEnemyType[MAX_OBJECT] = {};

//=============================================================================
// コンストラクタ
//=============================================================================
CEnemy::CEnemy() : CScene2D(OBJ_ENEMY)
{
	for (int nCnt = 0; nCnt < MAX_OBJECT; nCnt++)
	{
		if (m_paEnemy[nCnt] == NULL)
		{// 中身が空っぽの場合
			m_paEnemy[nCnt] = this;	// 情報代入

			if (m_paEnemyType[nCnt] == ENEMY_NULL)
			{// タイプがなかったら
				m_paEnemyType[nCnt] = m_type;	// タイプを設定
			}
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

	switch (m_type)
	{
	case ENEMY_BLACK:	// 黒い敵
		break;

	case ENEMY_WHITE:	// 白い敵
		break;

	case ENEMY_BOSS:	// ボス敵
		break;

	default:
		break;
	}
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CEnemy::Uninit(void)
{
	for (int nCnt = 0; nCnt < MAX_OBJECT; nCnt++)
	{// 中身をNULLにする
		m_paEnemy[nCnt] = NULL;
		m_paEnemyType[nCnt] = ENEMY_NULL;
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
	switch (m_paEnemy[nID]->m_type)
	{
	case ENEMY_BLACK:	// 黒い敵
		m_paEnemy[nID]->DamegeLife(1);
		if (m_paEnemy[nID]->GetLife() == 0)
		{
			m_paEnemy[nID]->m_type = ENEMY_WHITE;
		}
		break;

	case ENEMY_WHITE:	// 白い敵
		CScene::ObjRelease(OBJ_ENEMY, nID);
		m_paEnemy[nID] = NULL;
		break;

	case ENEMY_BOSS:	// ボス敵
		CGame::GetBoss()->DamegeLife(1);
		CGame::GetBoss()->SetState(CBoss::STATE_DAMAGE);
		if (CGame::GetBoss()->GetLife() == 0)
		{
			CGame::GetBoss()->SetAlive(false);
			CScene::ObjRelease(OBJ_ENEMY, nID);
			m_paEnemy[nID] = NULL;
		}
		break;

	default:
		break;
	}
}
