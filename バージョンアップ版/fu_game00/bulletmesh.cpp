//*****************************************************************************
//
// バレットメッシュポリゴン処理 [bulletmesh.h]
// Author : SUZUKI FUUTA
//
//*****************************************************************************

//-----------------------------------------------------------------------------
// インクルードファイル
//-----------------------------------------------------------------------------
#include "bulletmesh.h"
#include "manager.h"
#include "game.h"
#include "move.h"
#include "player.h"
#include "collision.h"
#include "particle.h"
#include "core.h"
#include "boss.h"
#include "tutorial.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CBulletMesh::CBulletMesh(Priority nPriority) : CMesh(nPriority)
{
	// メンバ変数の初期化
	m_nFrameTime = 0;
	m_nVtxID = 0;
}

//=============================================================================
// デストラクタ
//=============================================================================
CBulletMesh::~CBulletMesh()
{

}

//=============================================================================
// Meshのバレット生成
//=============================================================================
CBulletMesh * CBulletMesh::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, const D3DXVECTOR3 move ,bool bWave, Priority nPriority)
{
	CBulletMesh *pMBullet = NULL;

	// NULLチェック
	if (pMBullet == NULL)
	{
		pMBullet = new CBulletMesh(nPriority);			// インスタンス生成
		pMBullet->m_pos = pos;							// 位置
		pMBullet->m_size = size;						// サイズ
		pMBullet->m_move = move;						// 移動量
		pMBullet->m_bWave = bWave;						// 波の表現にするか
		pMBullet->CreateTexture("data/TEXTURE/02.png");	// テクスチャの設定
		pMBullet->Init();
	}

	return pMBullet;
}

//=============================================================================
// 初期化
//=============================================================================
HRESULT CBulletMesh::Init(void)
{
	// 初期化
	CMesh::Init(10, 0, m_pos, m_size);

	// 頂点情報の取得
	VERTEX_2D *pVtx = this->GetVERTEX();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CBulletMesh::Uninit(void)
{
	// 終了処理
	CMesh::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CBulletMesh::Update(void)
{
	// 頂点情報の取得
	VERTEX_2D *pVtx = this->GetVERTEX();

	// カウントアップ
	m_fWaveTime += 0.8f;
	m_nFrameTime++;

	// フレームごとに加算
	if ((m_nFrameTime % 1) == 0)
	{
		// 指定の頂点IDが上底の最大ID以下だったら
		if (m_nVtxID < this->GetVtxNum() / 2)
		{
			// カウントアップ
			m_nVtxID++;
		}	
	}

	// 最大値からの計算
	for (int nCnt = this->GetVtxNum() / 2 - 1; nCnt > this->GetVtxNum() / 2 - 1 - m_nVtxID; nCnt--)
	{
		// 位置の保存
		D3DXVECTOR3 pos = m_pos;

		// 各頂点に移動量を加算（上底）
		pVtx[nCnt].pos.x += m_move.x;

		// 各頂点に移動量を加算（下底）
		pVtx[nCnt + this->GetVtxNum() / 2].pos.x += m_move.x;

		// 波の表現する場合
		if (m_bWave)
		{
			// 上底にサインカーブの計算代入
			pVtx[nCnt].pos.y = Move::SinWave(pos.y, 40.0f, m_move.x, m_fWaveTime+(nCnt*2));

			// 下底に移動
			pos.y += m_size.y;

			// 下底にサインカーブの計算代入
			pVtx[nCnt+ this->GetVtxNum() / 2].pos.y = Move::SinWave(pos.y, 40.0f, m_move.x, m_fWaveTime+(nCnt*2));
		}
	}

	// 敵との当たり判定
	CollisionEnemy();

	// ボスが生きていたら
	if (CGame::GetBoss()->GetAlive() == true)
	{
		if (CollisionCore())
		{
			// ボスとの当たり判定
			CollisionBoss();
		}
	}

	// 更新処理
	CMesh::Update();

	// 画面端まで行ったら
	if (pVtx[0].pos.x > SCREEN_WIDTH)
	{
		// 終了処理
		CMesh::Uninit();
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CBulletMesh::Draw(void)
{
	// 描画処理
	CMesh::Draw();
}

//=============================================================================
// 敵との当たり判定
//=============================================================================
bool CBulletMesh::CollisionEnemy(void)
{
	D3DXVECTOR3 posEnemy;		// 敵の位置
	D3DXVECTOR3 sizeEnemy;		// 敵のサイズ

	// 頂点情報の取得
	VERTEX_2D *pVtx = this->GetVERTEX();			

	// シーンの取得
	CScene *pScene = CScene::GetScene(OBJ_ENEMY);

	// 当たり判定は無し
	m_Collision = false;	

	// NULLチェック
	while (pScene != NULL)
	{
		// 情報取得
		CScene *pSceneNext = pScene->GetSceneNext();

		// 位置とサイズの取得
		posEnemy = pScene->GetPos();
		sizeEnemy = pScene->GetSize();

		for (int nCnt = 0; nCnt < this->GetVtxNum(); nCnt+=2)
		{
			if (Collision::CollisionCycle(pVtx[nCnt].pos, posEnemy, sizeEnemy.x) == true)
			{/* 敵の範囲に弾が存在したら */

				// 当たり判定は有
				pScene->SetBool(true);
				m_Collision = true;							
				break;
			}
		}

		// 次の情報を取得
		pScene = pSceneNext;
	}

	// 判定結果を返す
	return m_Collision;	
}

//=============================================================================
// ボスとの当たり判定
//=============================================================================
bool CBulletMesh::CollisionBoss(void)
{
	D3DXVECTOR3 posBoss;	// 敵の位置
	D3DXVECTOR3 sizeBoss;	// 敵のサイズ

	// 頂点情報の取得
	VERTEX_2D *pVtx = this->GetVERTEX();			

	// シーンの情報取得
	CScene *pSceneBoss = CScene::GetScene(OBJ_BOSS);

	// 当たり判定は無し
	m_Collision = false;	

	// NULLチェック
	while (pSceneBoss != NULL)
	{
		// シーンの情報取得
		CScene *pSceneBNext = pSceneBoss->GetSceneNext();

		// 位置とサイズ取得
		posBoss = pSceneBoss->GetPos();
		sizeBoss = pSceneBoss->GetSize();

		for (int nCnt = 0; nCnt < this->GetVtxNum(); nCnt+=2)
		{
			if (Collision::CollisionCycle(pVtx[nCnt].pos, posBoss, 7.5f) == true)
			{/* 敵の範囲に弾が存在したら */

				// 当たり判定有にする
				pSceneBoss->SetBool(true);
				m_Collision = true;							
				break;
			}
		}

		// 次のシーン取得
		pSceneBoss = pSceneBNext;
	}

	// 判定結果を返す
	return m_Collision;	
}

//=============================================================================
// コアとの当たり判定
//=============================================================================

bool CBulletMesh::CollisionCore(void)
{
	CCore **pCore = NULL;
	// メッシュポリゴンの情報取得
	switch (CManager::GetMode())
	{
	case CManager::MODE_TUTORIAL:
		pCore = CTutorial::GetBoss()->GetCore();
		break;

	case CManager::MODE_GAME:
		// ボスの情報
		pCore = CGame::GetBoss()->GetCore();
		break;

	default:
		break;
	}

	VERTEX_2D *pVtx = this->GetVERTEX();			// 頂点情報の取得
	m_Collision = false;	// 当たり判定は無し

	for (int nCntCore = 0; nCntCore < 3; nCntCore++)
	{
		for (int nCnt = 0; nCnt < this->GetVtxNum(); nCnt+=2)
		{
			if (Collision::CollisionCycle(pVtx[nCnt].pos, pCore[nCntCore]->GetPos(), pCore[nCntCore]->GetSize().x) == true
				&& pCore[nCntCore]->GetUse() == true)
			{
				pCore[nCntCore]->SetUse(false);
				m_Collision = true;							// 当たり判定は有
				break;
			}
		}
	}

	bool bColBoss = false;

	if (pCore[0]->GetUse() == false && pCore[1]->GetUse() == false && pCore[2]->GetUse() == false)
	{
		bColBoss = true;
	}

	return bColBoss;
}
