//*****************************************************************************
//
// バレット処理 [bullet.cpp]
// Author : SUZUKI FUUTA
//
//*****************************************************************************
//-----------------------------------------------------------------------------
// インクルードファイル
//-----------------------------------------------------------------------------
#include "bullet.h"
#include "game.h"
#include "collision.h"
#include "enemy.h"

//-----------------------------------------------------------------------------
// インクルードファイル
//-----------------------------------------------------------------------------
CEnemy *CBullet::m_pBoss = NULL;

//-----------------------------------------------------------------------------
// マクロ変数
//-----------------------------------------------------------------------------
#define BULLET_TEXTURE		("data/TEXTURE/FlowerShot.png")
#define P_ENEMY				(pEnemy[nCntEnemy])

//=============================================================================
// コンストラクタ
//=============================================================================
CBullet::CBullet() : CScene2D(OBJ_BULLET)
{
}

//=============================================================================
// デストラクタ
//=============================================================================
CBullet::~CBullet()
{
}

//=============================================================================
// バレットの生成
//=============================================================================
CBullet * CBullet::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, const D3DXVECTOR3 move, BULLET type)
{
	/* バレットの要素を設定 */
	CBullet* pBullet = new CBullet;		// インスタンス生成

	pBullet->m_pos = pos;					// 位置を設定
	pBullet->m_size = size;					// サイズを設定
	pBullet->m_move = move;					// 移動量を設定
	pBullet->m_type = type;					// 弾のタイプ
	pBullet->CreateTexture(BULLET_TEXTURE);	// テクスチャ
	pBullet->m_Collision = false;			// 当たり判定
	pBullet->m_bUse = true;					// 生存確認
	pBullet->Init();						// 初期化

	return pBullet;						// バレット情報を返す
}

//=============================================================================
// 初期化
//=============================================================================
HRESULT CBullet::Init(void)
{
	CScene2D::Init(m_pos,m_size);	// ポリゴンの初期化処理

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CBullet::Uninit(void)
{
	CScene2D::Uninit();		// ポリゴンの終了処理
}

//=============================================================================
// 更新処理
//=============================================================================
void CBullet::Update(void)
{
	// 画面に存在しているかいないか
	if (m_bUse == true)
	{// している場合
		
		CScene2D::Update();				// ポリゴンの更新処理
		
		m_pos.x += m_move.x;			// 位置に移動量を加算

		CScene2D::SetPos(m_pos);		// 位置の更新

		switch (m_type)
		{// 誰のバレットか
		case CBullet::BULLET_PLAYER:	// プレイヤーの弾
			m_Collision = CollisionBullet(m_pos,m_size);	// 当たり判定確認
			break;

		default:
			break;
		}

		if (m_Collision == true)
		{// 当たった場合 
			m_bUse = false;				// 存在を消す
			CScene2D::Uninit();		// ポリゴンの終了処理
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CBullet::Draw(void)
{
	CScene2D::Draw();	// ポリゴンの描画
}

//=============================================================================
// 当たり判定
//=============================================================================
bool CBullet::CollisionBullet(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	D3DXVECTOR3 posEnemy;							// 敵の位置
	D3DXVECTOR3 sizeEnemy;							// 敵のサイズ
	int EnemyNum = CScene::GetObjeNum(OBJ_ENEMY);	// 画面にいる敵の数の取得
	CEnemy **pEnemy = CEnemy::GetEnemy();			// 敵の情報取得

	if (EnemyNum == 0)
	{/* 画面にいる敵がゼロの場合 */
		m_Collision = false;	// 当たり判定は無し
	}
	else if (EnemyNum >= 1)
	{/* 敵が一体以上いる場合 */
		for (int nCntEnemy = 0; nCntEnemy < MAX_OBJECT; nCntEnemy++)
		{
			posEnemy = CScene::GetPos(OBJ_ENEMY, nCntEnemy);	 // 敵の位置を取得
			sizeEnemy = CScene::GetSize(OBJ_ENEMY, nCntEnemy);	 // 敵のサイズを取得

			if (CCollision::CollisionCycle(m_pos, posEnemy, sizeEnemy.x) == true && P_ENEMY->GetEnemyType() != ENEMY_TYPE1)
			{/* 敵の範囲に弾が存在したら */
				CEnemy::CollisionEnemy(nCntEnemy);			// 当たった敵の当たり判定処理へ
				m_Collision = true;							// 当たり判定は有
				break;
			}
		}
	}
	if (m_pos.x - m_size.x > SCREEN_WIDTH || m_pos.x + m_size.x < 0.0f)
	{// 画面外処理
		m_Collision = true;				// 存在を消す
	}

	return m_Collision;	// 判定結果を返す
}

