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

//-----------------------------------------------------------------------------
// マクロ
//-----------------------------------------------------------------------------
#define ENEMY_TYPE1_MOVE		(D3DXVECTOR3(-2.5f, -5.0f, 0.0f))
#define TYPE1_COLOR				(D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f))
#define PLAYER_GET_POS			(CGame::GetPlayer()->GetPos())


//-----------------------------------------------------------------------------
// 静的メンバ変数
//-----------------------------------------------------------------------------
bool CNormalEnemy::m_bHomingTime = false;


//=============================================================================
// コンストラクタ
//=============================================================================
CNormalEnemy::CNormalEnemy() : CEnemy(OBJ_ENEMY)
{
	this->m_nMoveChangeCnt = 0;
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
		pNormalEnemy->SetType(type);			 // 種類
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
	CEnemy::Init();	// 初期化
	
	switch (CEnemy::GetEnemyType())
	{// 敵の種類別の初期化
		
	case ENEMY_BLACK:
		CScene2D::CreateTexture(ENEMY_TEXTURE0);
		break;
		
	case ENEMY_WHITE:
		
		break;
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CNormalEnemy::Uninit(void)
{
	CEnemy::Uninit();	// 終了処理
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
		UpdateBlack();	// 更新
		BlackMove();
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
		Release();
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
// タイプBLACKの更新
//=============================================================================
void CNormalEnemy::UpdateBlack(void)
{
	// カウントアップ
	m_nMoveChangeCnt++;

	if ((m_nMoveChangeCnt % 1200) == 0)
	{// 指定の値で割って余りが０の場合
		if (m_bHomingTime == false)
		{// ホーミングタイムではなかったら
			m_bHomingTime = true;
		}
		else
		{// ホーミングタイムだったら
			m_bHomingTime = false;
		}
		m_nMoveChangeCnt = 0;	// カウント初期化
	}

	if (m_bHomingTime == true)
	{// ホーミングタイム
		if ((m_nMoveChangeCnt%Rand(60, 20)) == 0)
		{// 指定の値で割って余りがゼロの場合
			float fAngle = CMove::AnglePoint(m_pos.x - PLAYER_GET_POS.x, m_pos.y - PLAYER_GET_POS.y);
			m_move.x = CMove::HomingMove(sinf(D3DX_PI + fAngle), 2.0f);
			m_move.y = CMove::HomingMove(cosf(D3DX_PI - fAngle), 2.0f);
			m_MoveType = MOVE1;
		}
	}
}

//=============================================================================
// タイプBLACKの移動処理
//=============================================================================
void CNormalEnemy::BlackMove(void)
{
	float fAngle = CMove::AnglePoint(m_pos.x - CGame::GetPlayer()->GetPos().x, m_pos.y - CGame::GetPlayer()->GetPos().y);
	CBoss *pBoss = CGame::GetBoss();

	switch (m_MoveType)
	{
	case MOVE0:
		m_move.y = CMove::MoveSnake(m_pos.y, m_move.y, 100.0f, 550.0f, 5.5f);
		m_pos += m_move;	// 位置に移動量を加算
		break;

	case MOVE1:
		m_pos += m_move;	// 位置に移動量を加算
		break;

	case MOVE2:
		m_fSinWaveCnt += 0.035f;
		m_pos.x = CMove::CosWave(m_pos.x, 15.0f, 3.5f, m_fSinWaveCnt);
		m_move.y = CMove::MoveSnake(m_pos.y, m_move.y, 80.0f, 400.0f, 5.5f);
		m_pos += m_move;	// 位置に移動量を加算
		break;

	case MOVE3:
		m_fCosWaveCnt += 0.035f;
		m_fSinWaveCnt += 0.035f;

		m_pos.x = CMove::SinWave(pBoss->GetPos().x, 140.0f, 14.0f, m_fSinWaveCnt);
		m_pos.y = CMove::CosWave(pBoss->GetPos().y, 140.0f, 14.0f, m_fCosWaveCnt);
		break;

	case MOVE4:
		m_fCosWaveCnt += 0.035f;
		m_fSinWaveCnt += 0.035f;

		m_pos.x += CMove::SinMove(2.5f, 14.0f, m_fSinWaveCnt);
		m_pos.y += CMove::CosMove(2.5f, 14.0f, m_fCosWaveCnt);
		break;

	default:
		break;
	}

	CScene2D::SetPos(m_pos);	// 移動量の更新
}

//=============================================================================
// タイプWHITEの更新
//=============================================================================
void CNormalEnemy::UpdateWhite(void)
{
	CPlayer *pPlayer = CGame::GetPlayer();
	m_fSinWaveCnt += 0.035f;

	m_pos.y = CMove::SinWave(m_pos.y, 10.0f, 4.0f, m_fSinWaveCnt);

	CScene2D::SetPos(m_pos);	// 移動量の更新
	CScene2D::SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
}
