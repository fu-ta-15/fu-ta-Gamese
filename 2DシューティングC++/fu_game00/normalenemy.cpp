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
	this->m_nMoveChangeCnt = 0;			// カウント
	this->m_bHomingTime = false;		// ホーミングしているか
	this->m_nHomingTime = HOMING_TIME1;	// ホーミングタイム
	this->m_move = ENEMY_TYPE1_MOVE;  	// 初期の移動量
	this->m_bCollision = false;			// 当たり判定
	this->SetLife(ENEMY_LIFE);			// 体力
	this->SetCol(TYPE1_COLOR);			// 色
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
	// クラスのポインタ
	CNormalEnemy *pNormalEnemy = NULL;

	// NULLチェック
	if (pNormalEnemy == NULL)
	{
		pNormalEnemy = new CNormalEnemy;
		pNormalEnemy->SetPos(pos);				// 位置
		pNormalEnemy->SetSize(size);			// サイズ
		pNormalEnemy->SetMoveType(movetype);	// 移動のタイプ
		pNormalEnemy->SetType(type);			// 種類
		pNormalEnemy->Init();					// 初期化
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

	// 移動量の種類
	switch (m_MoveType)
	{
	case CNormalEnemy::MOVE_0:

		// プレイヤーの角度算出
		float fAngle = Move::AnglePoint(ANGLE_POINT_X, ANGLE_POINT_Y);

		// プレイヤーの角度の方向に向かって移動量設定
		m_move.x = sinf(fAngle - D3DX_PI) * 2.5f;
		m_move.y = cosf(fAngle + D3DX_PI) * 2.5f;
		break;

	case CNormalEnemy::MOVE_2:

		// ランダムな角度生成
		float fAngle = (float)(rand() % 314) / 100.f;

		// 設定された角度の方向に移動する設定
		m_move.x = sinf(fAngle - D3DX_PI) * 2.5f;
		m_move.y = cosf(fAngle + D3DX_PI) * 2.5f;
		break;

	default:
		break;
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
	case ENEMY_BLACK:

		// 更新
		UpdateBlack();	

		// 移動処理
		BlackMove();	
		break;

	case ENEMY_WHITE:

		// 更新
		UpdateWhite();	
		break;

	default:
		break;
	}

	// 更新処理
	CEnemy::Update();

	// 画面外処理
	if (m_pos.x - m_size.x > SCREEN_WIDTH || m_pos.x + m_size.x < 0.0f &&
		m_pos.y - m_size.y > SCREEN_HEIGHT || m_pos.y + m_size.y < 0.0f)
	{
		// 終了処理
		CEnemy::Uninit();
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CNormalEnemy::Draw(void)
{
	// 描画
	CEnemy::Draw();	
}

//=============================================================================
// タイプBLACKの更新
//=============================================================================
void CNormalEnemy::UpdateBlack(void)
{
	// カウントアップ
	m_nMoveChangeCnt++;

	// ホーミングの開始と終了
	if ((m_nMoveChangeCnt % m_nHomingTime) == 0)
	{// 指定の値で割って余りが０の場合

		if (m_bHomingTime == false)
		{// ホーミングタイムではなかったら
			m_bHomingTime = true;
			m_nHomingTime = Rand(100, 400);
		}
		else
		{// ホーミングタイムだったら
			m_bHomingTime = false;
			m_nHomingTime = HOMING_TIME1;
		}

		// カウント初期化
		m_nMoveChangeCnt = 0;	
	}

	if (m_bHomingTime == true)
	{// ホーミングタイム
		if ((m_nMoveChangeCnt % Rand(60, 20)) == 0)
		{// 指定の値で割って余りがゼロの場合

			// プレイヤーとの角度
			float fAngle = Move::AnglePoint(ANGLE_POINT_X, ANGLE_POINT_Y);	

			// 追従
			m_move.x = Move::HomingMove(ANGLE_SIN, HOMING_SPEED);			
			m_move.y = Move::HomingMove(ANGLE_COS, HOMING_SPEED);			

			// 移動方法
			m_MoveType = MOVE1;												
		}
	}
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

	// 移動の種類
	switch (m_MoveType)
	{
	case MOVE0:
		// 位置に移動量を加算
		m_pos += m_move;	
		break;

	case MOVE1:
		// 位置に移動量を加算
		m_pos += m_move;	
		break;

	case MOVE2:
		// 位置に移動量を加算
		m_pos += m_move;	
		break;

	case MOVE3:
		// カウントアップ
		m_fCosWaveCnt += 0.035f;
		m_fSinWaveCnt += 0.035f;

		// 位置に移動の値代入
		m_pos.x = Move::SinWave(pBoss->GetPos().x, 140.0f, 14.0f, m_fSinWaveCnt);
		m_pos.y = Move::CosWave(pBoss->GetPos().y, 140.0f, 14.0f, m_fCosWaveCnt);
		break;

	default:
		break;
	}

	// 位置の更新
	CScene2D::SetPos(m_pos);	
}

//=============================================================================
// タイプWHITEの更新
//=============================================================================
void CNormalEnemy::UpdateWhite(void)
{
	// ボスの情報
	CBoss *pBoss = CGame::GetBoss();

	// カウントアップ
	m_fCosWaveCnt += 0.035f;
	m_fSinWaveCnt += 0.035f;

	// 位置の変更
	m_pos.x = Move::SinWave(pBoss->GetPos().x, 140.0f, 14.0f, m_fSinWaveCnt);
	m_pos.y = Move::CosWave(pBoss->GetPos().y, 140.0f, 14.0f, m_fCosWaveCnt);

	// 位置の更新
	CScene2D::SetPos(m_pos);	

	// 色の更新
	CScene2D::SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
}

//=============================================================================
// 敵の配置
//=============================================================================
void ENEMY::SetEnemy(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, CEnemy::ENEMYTYPE type, CNormalEnemy::EnemyMove movetype, int nNum)
{
	for (int nCnt = 0; nCnt < nNum; nCnt++)
	{
		// 敵の生成
		CNormalEnemy::Create(pos, size, type, movetype);
	}
}
