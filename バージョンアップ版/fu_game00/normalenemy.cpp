//*****************************************************************************
//
// 通常の敵処理 [normalenemy.h]
// Author : SUZUKI FUUTA
//
//*****************************************************************************
//-----------------------------------------------------------------------------
// インクルードファイル
//-----------------------------------------------------------------------------
#include "normalenemy.h"
#include "collision.h"
#include "move.h"
#include "player.h"
#include "game.h"
#include "boss.h"
#include "time.h"

//-----------------------------------------------------------------------------
// マクロ
//-----------------------------------------------------------------------------
#define ENEMY_TYPE1_MOVE		(D3DXVECTOR3(-2.5f, -5.0f, 0.0f))
#define TYPE1_COLOR				(D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f))
#define PLAYER_GET_POS			(CGame::GetPlayer()->GetPos())
#define HOMING_TIME1			(200)
#define HOMING_SPEED			(3.0f)
#define ANGLE_SIN				(sinf(D3DX_PI + fAngle))
#define ANGLE_COS				(cosf(D3DX_PI - fAngle))
#define ANGLE_POINT_X			(m_pos.x - PLAYER_GET_POS.x)
#define ANGLE_POINT_Y			(m_pos.y - PLAYER_GET_POS.y)

//=============================================================================
// コンストラクタ
//=============================================================================
CNormalEnemy::CNormalEnemy() : CEnemy(OBJ_ENEMY)
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CNormalEnemy::~CNormalEnemy()
{

}

//=============================================================================
// 通常の敵の生成
//=============================================================================
CNormalEnemy * CNormalEnemy::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, ENEMYTYPE type, EnemyMove movetype)
{
	CNormalEnemy *pNormalEnemy = NULL;

	if (pNormalEnemy == NULL)
	{// NULLチェック
		pNormalEnemy = new CNormalEnemy;
		pNormalEnemy->m_bCollision = false;		 // 当たり判定
		pNormalEnemy->m_move = ENEMY_TYPE1_MOVE;  // 移動量
		pNormalEnemy->SetPos(pos);				 // 位置
		pNormalEnemy->SetSize(size);			 // サイズ
		pNormalEnemy->SetMoveType(movetype);	 // 移動のタイプ
		pNormalEnemy->SetLife(ENEMY_LIFE);		 // 体力
		pNormalEnemy->SetType(type);		 // 種類
		pNormalEnemy->SetCol(TYPE1_COLOR);		 // 色
		pNormalEnemy->Init();					 // 初期化
	}

	return pNormalEnemy;
}

//=============================================================================
// 初期化
//=============================================================================
HRESULT CNormalEnemy::Init(void)
{
	// 初期化
	CEnemy::Init();	
	
	// テクスチャの設定
	CScene2D::CreateTexture(ENEMY_TEXTURE0);

	// 移動のタイプ別に移動方向の設定
	if (m_MoveType == MOVE0)
	{
		float fAngle = Move::AnglePoint(ANGLE_POINT_X, ANGLE_POINT_Y);
		m_move.x = sinf(fAngle - D3DX_PI) * 2.5f;
		m_move.y = cosf(fAngle + D3DX_PI) * 2.5f;
	}
	else if (m_MoveType == MOVE2)
	{
		float fAngle = (float)(rand() % 314) / 100.f;
		m_move.x = sinf(fAngle - D3DX_PI) * 2.5f;
		m_move.y = cosf(fAngle + D3DX_PI) * 2.5f;
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CNormalEnemy::Uninit(void)
{
	// 終了処理
	CEnemy::Uninit();	
}

//=============================================================================
// 更新処理
//=============================================================================
void CNormalEnemy::Update(void)
{
	// 敵の種類別の更新
	switch (CEnemy::GetEnemyType())
	{
		// 黒い敵
	case ENEMY_BLACK:
		BlackMove();	// 移動処理
		break;
		// 白い敵
	case ENEMY_WHITE:
		UpdateWhite();	// 更新
		break;
	default:
		break;
	}

	CEnemy::Update();

	if (m_pos.x - m_size.x > SCREEN_WIDTH || m_pos.x + m_size.x < 0.0f)
	{// 画面外処理
		CEnemy::Uninit();
	}
	if (m_pos.y - m_size.y > SCREEN_HEIGHT || m_pos.y + m_size.y < 0.0f)
	{
		CEnemy::Uninit();
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CNormalEnemy::Draw(void)
{
	CEnemy::Draw();	// 描画
}

//=============================================================================
// タイプBLACKの移動処理
//=============================================================================
void CNormalEnemy::BlackMove(void)
{
	// プレイヤーとの角度
	float fPlayerAngle = Move::AnglePoint(ANGLE_POINT_X, ANGLE_POINT_Y);

	// ボスの情報
	CBoss *pBoss = CGame::GetBoss();

	switch (m_MoveType)
	{
	case MOVE0:
		m_pos += m_move;	// 位置に移動量を加算
		break;

	case MOVE1:
		m_pos += m_move;	// 位置に移動量を加算
		break;

	case MOVE2:
		m_pos += m_move;	// 位置に移動量を加算
		break;

	case MOVE3:
		m_fCosWaveCnt += 0.035f;
		m_fSinWaveCnt += 0.035f;

		m_pos.x = Move::SinWave(pBoss->GetPos().x, 140.0f, 14.0f, m_fSinWaveCnt);
		m_pos.y = Move::CosWave(pBoss->GetPos().y, 140.0f, 14.0f, m_fCosWaveCnt);
		break;

	default:
		break;
	}

	// 移動量の更新
	CScene2D::SetPos(m_pos);	
}

//=============================================================================
// タイプWHITEの更新
//=============================================================================
void CNormalEnemy::UpdateWhite(void)
{
	// ボスの情報
	CBoss *pBoss = CGame::GetBoss();

	m_fCosWaveCnt += 0.035f;
	m_fSinWaveCnt += 0.035f;

	m_pos.x = Move::SinWave(pBoss->GetPos().x, 140.0f, 14.0f, m_fSinWaveCnt);
	m_pos.y = Move::CosWave(pBoss->GetPos().y, 140.0f, 14.0f, m_fCosWaveCnt);

	CScene2D::SetPos(m_pos);	// 移動量の更新
	CScene2D::SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
}

void ENEMY::SetEnemy(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, CEnemy::ENEMYTYPE type, CNormalEnemy::EnemyMove movetype, int nNum)
{
	for (int nCnt = 0; nCnt < nNum; nCnt++)
	{
		CNormalEnemy::Create(pos, size, type, movetype);
	}
}
