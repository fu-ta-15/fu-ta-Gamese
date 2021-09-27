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

//-----------------------------------------------------------------------------
// マクロ
//-----------------------------------------------------------------------------
#define ENEMY_TYPE1_MOVE		(D3DXVECTOR3(-1.0f, -5.0f, 0.0f))
#define TYPE1_COLOR				(D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f))

//=============================================================================
// コンストラクタ
//=============================================================================
CNormalEnemy::CNormalEnemy()
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
CNormalEnemy * CNormalEnemy::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, ENEMYTYPE type)
{
	CNormalEnemy *pNormalEnemy = NULL;

	if (pNormalEnemy == NULL)
	{// NULLチェック
		pNormalEnemy = new CNormalEnemy;
		pNormalEnemy->m_move = ENEMY_TYPE1_MOVE;  // 移動量
		pNormalEnemy->m_bCollision = false;		  // 当たり判定
		pNormalEnemy->SetPos(pos);				  // 位置
		pNormalEnemy->SetSize(size);			  // サイズ
		pNormalEnemy->SetLife(ENEMY_LIFE);		  // 体力
		pNormalEnemy->SetType(type);			  // 種類
		pNormalEnemy->SetCol(TYPE1_COLOR);		  // 色
		pNormalEnemy->Init();					  // 初期化
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
		break;
		// 白い敵
	case ENEMY_WHITE:
		UpdateWhite();	// 更新
		break;
	default:
		break;
	}
	CEnemy::Update();
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
	
	// 位置に移動量を加算
	m_pos += m_move;
	m_move.y = CMove::MoveSnake(m_pos.y, m_move.y, 100.0f, 550.0f, 6.5f);

	if (CollisionPlayer() == true)
	{
		CGame::GetPlayer()->SetCollEnemy(true);
		
		printf("当たりました");
	}
	CScene2D::SetPos(m_pos);	// 移動量の更新
}

//=============================================================================
// タイプWHITEの更新
//=============================================================================
void CNormalEnemy::UpdateWhite(void)
{
	CPlayer *pPlayer = CGame::GetPlayer();

	m_move.y = CMove::MoveSnake(m_pos.y, m_move.y, 200.0f, 500.0f, 6.5f);
	m_pos.y += m_move.y;

	CScene2D::SetPos(m_pos);	// 移動量の更新
	CScene2D::SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
}

bool CNormalEnemy::CollisionPlayer(void)
{
	bool bCollision = false;

	D3DXVECTOR3 pos = CGame::GetPlayer()->GetPos();
	D3DXVECTOR3 size = CGame::GetPlayer()->GetSize();

	bCollision = CCollision::CollisionCycle(m_pos, pos, size.x);

	return bCollision;
}
