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

//-----------------------------------------------------------------------------
// マクロ変数
//-----------------------------------------------------------------------------
#define BULLET_TEXTURE		("data/TEXTURE/Spark002.png")

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
CBullet * CBullet::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, const D3DXVECTOR3 move)
{
	/* バレットの要素を設定 */
	CBullet* pBullet = new CBullet;		// インスタンス生成

	pBullet->m_pos = pos;					// 位置を設定
	pBullet->m_size = size;					// サイズを設定
	pBullet->m_move = move;					// 移動量を設定
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

		if (CollisionBullet())
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
	if (m_bUse == true)
	{// している場合
		CScene2D::Draw();	// ポリゴンの描画
	}
}

//=============================================================================
// 当たり判定
//=============================================================================
bool CBullet::CollisionBullet(void)
{
	D3DXVECTOR3 posEnemy;							// 敵の位置
	D3DXVECTOR3 sizeEnemy;							// 敵のサイズ
	D3DXVECTOR3 posBoss;							// 敵の位置
	D3DXVECTOR3 sizeBoss;							// 敵のサイズ

	CScene *pScene = CScene::GetScene(OBJ_ENEMY);
	CScene *pSceneBoss = CScene::GetScene(OBJ_BOSS);

	m_Collision = false;	// 当たり判定は無し
	if (true)
	{

	}
	while (pScene != NULL) 
	{
		CScene *pSceneNext = pScene->GetSceneNext();

		posEnemy = pScene->GetPos();

		sizeEnemy = pScene->GetSize();

		if (Collision::CollisionCycle(m_pos, posEnemy, sizeEnemy.x) == true)
		{/* 敵の範囲に弾が存在したら */
			pScene->SetBool(true);
			m_Collision = true;							// 当たり判定は有
			break;
		}

		pScene = pSceneNext;

	}

	while (pSceneBoss != NULL)
	{
		CScene *pSceneBNext = pSceneBoss->GetSceneNext();

		posBoss = pSceneBoss->GetPos();

		sizeBoss = pSceneBoss->GetSize();

		if (Collision::CollisionCycle(m_pos, posBoss, sizeBoss.x) == true)
		{/* 敵の範囲に弾が存在したら */
			pSceneBoss->SetBool(true);
			m_Collision = true;							// 当たり判定は有
			break;
		}

		pSceneBoss = pSceneBNext;
	} 

	if (m_pos.x - m_size.x > SCREEN_WIDTH || m_pos.x + m_size.x < 0.0f)
	{// 画面外処理
		m_Collision = true;				// 存在を消す
	}

	return m_Collision;	// 判定結果を返す
}

