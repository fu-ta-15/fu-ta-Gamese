//*****************************************************************************
//
// 敵処理 [enemy.cpp]
// Author : SUZUKI FUUTA
//
//*****************************************************************************

//-----------------------------------------------------------------------------
// インクルードファイル
//-----------------------------------------------------------------------------
#include "game.h"
#include "enemy.h"
#include "normalenemy.h"
#include "boss.h"
#include "player.h"
#include "collision.h"
#include "mesh.h"
#include "particle.h"


//-----------------------------------------------------------------------------
// マクロ変数
//-----------------------------------------------------------------------------
#define BOSS_DAMAGE					(CBoss::STATE_DAMAGE)
#define BOSS_NOT_DAMAGE				(CBoss::STATE_NOT_DAMAGE)
#define BOSS_NONE					(CBoss::STATE_NONE)
#define BOSS_GET_LIFE				(CGame::GetBoss()->GetLife())
#define ENEMY_ID					(m_paEnemy[nID])
#define ENEMY_CNT					(m_paEnemy[nCnt])

//=============================================================================
// コンストラクタ
//=============================================================================
CEnemy::CEnemy(Priority type) : CScene2D(type)
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
	if (this->CollisionPlayer() == true && CGame::GetPlayer()->GetState() != CPlayer::STATE_KNOCKUP)
	{// プレイヤーとの当たり判定
		switch (this->m_type)
		{
		case ENEMY_BLACK:
			CGame::GetPlayer()->SetCollEnemy(true);
			this->Release();
			break;

		case ENEMY_WHITE:
			this->Release();
			break;

		default:
			break;
		}
	}
	// 弾との当たり判定
	CollisionEnemy();
	CollisionField();
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
	{// 当たり判定がTRUEだったら
		
		CBoss *pBoss = BOSS;	// ゲームシーンから取得

		// 敵の種類
		switch (this->m_type)
		{
		case ENEMY_TYPE0:

			this->DamegeLife(1);	// ライフ減少

			if (this->GetLife() == 0)
			{// ライフがゼロだったら
				CScene2D::Uninit();
				Particle::SetParticle(m_pos, m_size, 10, Particle::TYPE_EXPLOSION, "data/TEXTURE/Crystal001.png");
			}
			break;

		case ENEMY_TYPE1:

			CScene2D::Uninit();
			Particle::SetParticle(m_pos, m_size, 10, Particle::TYPE_EXPLOSION, "data/TEXTURE/Crystal001.png");

			break;

		case ENEMY_TYPE2:

			if (pBoss->GetState() == BOSS_NONE)
			{// 敵が通常の状態だったら
				pBoss->SetState(BOSS_DAMAGE);
			}
			break;
		}
		this->SetBool(false);	// フラグを下す
	}
}

//=============================================================================
// プレイヤーとの当たり判定
//=============================================================================
bool CEnemy::CollisionPlayer(void)
{
	D3DXVECTOR3 pos = CGame::GetPlayer()->GetPos();
	D3DXVECTOR3 size = CGame::GetPlayer()->GetSize();

	return  Collision::CollisionCycle(m_pos, pos, size.x);
}

//=============================================================================
// 地面との当たり判定
//=============================================================================
void CEnemy::CollisionField(void)
{
	CMesh* pMesh = MESH_GAME;

	// 頂点情報の取得
	VERTEX_2D *pVtx = pMesh->GetVERTEX();

	// 底辺の中心座標設定
	D3DXVECTOR3 posA = D3DXVECTOR3(m_pos.x, m_pos.y + m_size.y, 0.0f);

	for (int nCnt = 0; nCnt < pMesh->GetVtxNum() / 2; nCnt++)
	{// メッシュポリゴン上辺のみ算出
		if (m_pos.x > pVtx[nCnt].pos.x && m_pos.x < pVtx[nCnt + 1].pos.x)
		{// 二つの頂点と点の外積判定
			if (Collision::OutProduct(pVtx[nCnt].pos, pVtx[nCnt + 1].pos, posA))
			{// 点が二点より下にいたら

				Uninit();
				Particle::SetParticle(m_pos, m_size, 10, Particle::TYPE_EXPLOSION, "data/TEXTURE/Crystal001.png");
				break;
			}
		}
	}
}
