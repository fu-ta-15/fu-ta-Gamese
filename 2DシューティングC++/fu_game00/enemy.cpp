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
	// ポリゴンの生成
	CScene2D::Init(m_pos, m_size);	

	// サイズの設定
	CScene2D::SetSize(m_size);		

	// 色の設定
	CScene2D::SetCol(m_col);		

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CEnemy::Uninit(void)
{
	// 終了処理
	CScene2D::Uninit();	
}

//=============================================================================
// 更新処理
//=============================================================================
void CEnemy::Update(void)
{
	// プレイヤーが通常の状態であれば
	if (this->CollisionPlayer() == true && CGame::GetPlayer()->GetState() != CPlayer::STATE_KNOCKUP)
	{
		// 種類別
		switch (this->m_type)
		{
		case ENEMY_BLACK:

			// 当たった合図を送る
			CGame::GetPlayer()->SetCollEnemy(true);

			// 死亡フラグを立てる
			this->Release();

			break;

		case ENEMY_WHITE:

			// 死亡フラグを立てる
			this->Release();

			break;

		default:
			break;
		}
	}

	// 弾との当たり判定
	CollisionEnemy();

	// 地面との当たり判定
	CollisionField();
}

//=============================================================================
// 描画処理
//=============================================================================
void CEnemy::Draw(void)
{
	// 描画処理
	CScene2D::Draw();	
}

//=============================================================================
// 種類ごとの当たり判定後の処理
//=============================================================================
void CEnemy::CollisionEnemy(void)
{
	// 当たり判定がTRUEだったら
	if (this->GetBool() == true)
	{
		// ゲームシーンから取得
		CBoss *pBoss = CGame::GetBoss();	

		// 敵の種類
		switch (this->m_type)
		{
		case ENEMY_TYPE0:

			// ライフ減少
			this->DamegeLife(1);	

			// ライフがゼロだったら
			if (this->GetLife() == 0)
			{
				// 終了処理
				CScene2D::Uninit();

				// パーティクルの発生
				Particle::SetParticle(m_pos, m_size, 10, Particle::TYPE_EXPLOSION, "data/TEXTURE/Crystal001.png");
			}
			break;

		case ENEMY_TYPE1:

			// 終了処理
			CScene2D::Uninit();

			// パーティクルの発生
			Particle::SetParticle(m_pos, m_size, 10, Particle::TYPE_EXPLOSION, "data/TEXTURE/Crystal001.png");

			break;

		case ENEMY_TYPE2:

			// 敵が通常の状態だったら
			if (pBoss->GetState() == BOSS_NONE)
			{
				// ダメージを食らっている状態にする
				pBoss->SetState(BOSS_DAMAGE);
			}
			break;
		}

		// フラグを戻す
		this->SetBool(false);	
	}
}

//=============================================================================
// プレイヤーとの当たり判定
//=============================================================================
bool CEnemy::CollisionPlayer(void)
{
	// プレイヤーの位置取得
	D3DXVECTOR3 pos = CGame::GetPlayer()->GetPos();

	// プレイヤーのサイズ取得
	D3DXVECTOR3 size = CGame::GetPlayer()->GetSize();

	// 当たり判定の結果を返す
	return  Collision::CollisionCycle(m_pos, pos, size.x);
}

//=============================================================================
// 地面との当たり判定
//=============================================================================
void CEnemy::CollisionField(void)
{
	// メッシュの情報取得（ゲームモードの）
	CMesh* pMesh = CGame::GetMesh();

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

				// 終了処理
				Uninit();

				// パーティクルの発生
				Particle::SetParticle(m_pos, m_size, 10, Particle::TYPE_EXPLOSION, "data/TEXTURE/Crystal001.png");
				break;
			}
		}
	}
}
