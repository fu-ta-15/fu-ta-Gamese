//*****************************************************************************
//
// ボス敵処理 [boss.h]
// Author : SUZUKI FUUTA
//
//*****************************************************************************
//-----------------------------------------------------------------------------
// インクルードファイル
//-----------------------------------------------------------------------------
#include "boss.h"
#include "normalenemy.h"
#include "manager.h"
#include "renderer.h"
#include "move.h"
#include "collision.h"
#include "enemy.h"
#include "time.h"
#include "game.h"
#include "bulletmesh.h"

//-----------------------------------------------------------------------------
// マクロ変数
//-----------------------------------------------------------------------------
#define EFFECT_COLOR	(D3DXCOLOR(1.0f,1.0f,1.0f,0.0f))				// エフェクトの初期色
#define LIFE_POS_X		(SCREEN_WIDTH - 100.0f)							// ライフのX座標
#define LIFE_POS_Y		(200.0f - (10 * nCntLife))						// ライフのY座標
#define LIFE_POS		(D3DXVECTOR3(LIFE_POS_X, LIFE_POS_Y, 0.0f))		// ライフの位置
#define LIFE_SIZE		(D3DXVECTOR3(10.0f, 5.0f, 0.0f))				// ライフのサイズ
#define LIFE_DOWN		((m_fLife * 0.17f + 0.025f))					// ライフの減少計算
#define LIFE_CNT		(nCntLife * 10)									// ライフカウント
#define DAMAGE_TEXTUER	("data/TEXTURE/stateBoss.png")					// ダメージ状態テクスチャリンク
#define SHILED_TEXTUER	("data/TEXTURE/AuroraRing.png")					// バリアテクスチャリンク
#define LIFE_TEXTURE	("data/TEXTURE/BossLife.png")					// ライフテクスチャリンク

//-----------------------------------------------------------------------------
// 静的メンバ変数
//-----------------------------------------------------------------------------
bool CBoss::m_bBoss_Alive = true;	  // ボスの生存確認

//=============================================================================
// コンストラクタ
//=============================================================================
CBoss::CBoss() : CEnemy(OBJ_BOSS)
{
	this->m_pDamage = NULL;				// ダメージエフェクト
	this->m_bDamage = false;			// ダメージ受けているかいないか
	this->m_State = STATE_NONE;			// 状態
	this->m_LifeState = LIFE_RATE_0;	// ライフの状態
	this->m_move = ZeroVector3;			// 移動量初期化
	this->m_bBoss_Alive = true;			// 生存する
}

//=============================================================================
// デストラクタ
//=============================================================================
CBoss::~CBoss()
{
}

//=============================================================================
// ボスの生成
//=============================================================================
CBoss * CBoss::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, const int nLife)
{
	CBoss *pBoss = new CBoss;

	// NULLチェック
	if (pBoss != NULL)
	{
		pBoss->SetPos(pos);				// 位置
		pBoss->SetSize(size);			// サイズ
		pBoss->m_fLife = (float)nLife;	// ライフ
		pBoss->SetCol(WhiteColor);		// 色
		pBoss->SetType(ENEMY_BOSS);		// タイプ
		pBoss->Init();					// 初期化
	}

	return pBoss;
}

//=============================================================================
// 初期化
//=============================================================================
HRESULT CBoss::Init(void)
{	
	// 初期化
	CEnemy::Init();

	// ダメージカウント
	m_nDamageCnt = 0;	 

	// 移動用カウント
	m_fMoveTime = 0.0f;	 

	// ライフの生成
	for (int nCntLife = 0; nCntLife < BOSS_LIFE_STOCK; nCntLife++)
	{
		D3DXVECTOR3 pos = LIFE_POS;						 // 位置
		D3DXVECTOR3 size = LIFE_SIZE;					 // サイズ
		m_pLife[nCntLife] = CScene2D::Create(pos, size); // 生成
		m_pLife[nCntLife]->CreateTexture(LIFE_TEXTURE);	 // テクスチャの設定
	}

	// ダメージ用エフェクトの生成
	m_pDamage = CEffect::Create(m_pos, m_size);	   // 生成
	m_pDamage->CreateTexture(DAMAGE_TEXTUER);	   // テクスチャの設定
	m_pDamage->SetColor(EFFECT_COLOR);			   // 色の設定
	m_StateCol = m_pDamage->GetColor();			   // 色の取得


	// コアの生成
	m_pCore[0] = CCore::Create(m_pos, D3DXVECTOR3(25.0f, 25.0f, 0.0f), 0, OBJ_CORE);
	m_pCore[1] = CCore::Create(m_pos, D3DXVECTOR3(25.0f, 25.0f, 0.0f), 50, OBJ_CORE);
	m_pCore[2] = CCore::Create(m_pos, D3DXVECTOR3(25.0f, 25.0f, 0.0f), 100, OBJ_CORE);

	// シールドの生成
	m_pShiled = CScene2D::Create(m_pos, m_size * 1.5f);
	m_pShiled->CreateTexture("data/TEXTURE/t0003.png");

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CBoss::Uninit(void)
{
	// コアの開放
	for (int nCnt = 0; nCnt < 3; nCnt++)
	{
		if (m_pCore[nCnt] != NULL)
		{
			// 終了処理
			m_pCore[nCnt]->Uninit();

			// NULLを代入
			m_pCore[nCnt] = NULL;
		}
	}

	// シールドの開放
	if (m_pShiled != NULL)
	{
		// 終了処理
		m_pShiled->Uninit();

		// NULLを代入
		m_pShiled = NULL;
	}

	// 自身の開放
	CEnemy::Uninit();	
}

//=============================================================================
// 更新処理
//=============================================================================
void CBoss::Update(void)
{
	// メッシュポリゴンの情報取得
	switch (CManager::GetMode())
	{
		// ゲームモード
	case CManager::MODE_GAME:

		// 更新
		CEnemy::Update();

		// ダメージ状態更新
		DamageBoss();

		// 状態の更新
		StateUpdate();

		// 移動処理
		MoveBoss();

		// 敵召喚処理
		SummonsEnemy();

		break;

	default:
		break;
	}

	// 位置の更新
	m_pShiled->SetPos(m_pos); 	

	// 位置の更新
	CScene2D::SetPos(m_pos);

	// サイズの更新
	CScene2D::SetSize(m_size);	

	// ライフがゼロ
	if (m_fLife <= 0)
	{
		// ボスの生存をfalse
		m_bBoss_Alive = false;

		// 自身の開放
		CEnemy::Uninit();	
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CBoss::Draw(void)
{
	// 描画処理
	CEnemy::Draw();
}

//=============================================================================
// ダメージを受けた時
//=============================================================================
void CBoss::DamageBoss(void)
{
	// ダメージを受けたら
	if (m_State == STATE_DAMAGE)
	{
		// ダメージ合図
		m_bDamage = true;

		// 体力の減少
		m_fLife -= LIFE_DOWN;

		// 現在の体力状態
		m_LifeState = LifeState((int)m_fLife);

		// 透明度加算用
		m_fA_Damage = 0.065f;

		// ダメージ無効状態にする
		m_State = STATE_NOT_DAMAGE;
	}
}

//=============================================================================
// 状態管理
//=============================================================================
void CBoss::StateUpdate(void)
{
	// ダメージを受けていたら
	if (m_State == STATE_NOT_DAMAGE)
	{
		// カラーの更新
		m_pDamage->SetColor(m_StateCol);

		// 位置を更新
		m_pDamage->SetPos(m_pos);

		// α値の加算
		m_StateCol.a += m_fA_Damage;

		// α値が一定を越えたら
		if (m_StateCol.a > 1.0f)
		{
			// 加算の数値をマイナスに
			m_fA_Damage = -0.004f;
		}
		else if (m_StateCol.a < 0.0f)
		{
			// FALSE代入
			m_bDamage = false;

			// 状態を戻す
			m_State = STATE_NONE;
		}
	}
}

//=============================================================================
// ライフの状態管理
//=============================================================================
CBoss::LIFE_STATE CBoss::LifeState(int nLife)
{
	// ライフブロックの更新
	for (int nCntLife = 0; nCntLife < BOSS_LIFE_STOCK; nCntLife++)
	{
		// ライフが区切りの値を越えたら
		if (m_fLife < LIFE_CNT)
		{
			// 残り体力
			switch (nCntLife)
			{
				// HP１２
			case 12:

				// ２割減少
				if (m_LifeState == LIFE_RATE_0){ m_LifeState = LIFE_RATE_2; }
				break;

				// HP７
			case 7:

				// ５割減少
				if (m_LifeState == LIFE_RATE_2) { m_LifeState = LIFE_RATE_5; }
				break;

				// HP２
			case 2:

				// ８割減少
				if (m_LifeState == LIFE_RATE_5) { m_LifeState = LIFE_RATE_8; }
				break;

			default:
				break;
			}

			// NULLチェック
			if (m_pLife[nCntLife] != NULL)
			{
				// ライフのブロック削除
				m_pLife[nCntLife]->Uninit(); 

				// NULL代入
				m_pLife[nCntLife] = NULL;	  
			}

			// コアの復活
			for (int nCnt = 0; nCnt < 3; nCnt++)
			{
				// コアの描画開始
				m_pCore[nCnt]->SetUse(true);
			}
		}
	}

	return m_LifeState;
}

//=============================================================================
// 移動処理
//=============================================================================
void CBoss::MoveBoss(void)
{
	// カウント加算
	m_fMoveTime += 0.5f;

	// ライフの状態で移動変更
	switch (m_LifeState)
	{
		// ０割減少
	case LIFE_RATE_0:

		// Cos波の移動
		m_pos.y = Move::CosWave(HEIGHT_HALF,100.0f, 65.5f, m_fMoveTime);

		// 現在の位置を保存
		m_OldPos = m_pos;
		break;

		// ２割減少
	case LIFE_RATE_2:

		// Cos波の移動
		m_pos.y = Move::CosWave(m_OldPos.y,60.0f, 100.5f, m_fMoveTime);

		// Sin波の加算移動
		m_pos.x = Move::SinWave(m_OldPos.x, 60.0f, 100.5f,m_fMoveTime);
		break;

		// ５割減少
	case LIFE_RATE_5:

		// 指定の位置まで移動
		m_pos = Move::TargetPosMove(D3DXVECTOR3(SCREEN_WIDTH - 150.0f , HEIGHT_HALF + 100.0f, 0.0f), m_pos, 0.025f);
		break;

		// ８割減少
	case LIFE_RATE_8:

		// 指定の位置まで移動
		m_pos = Move::TargetPosMove(D3DXVECTOR3(WIDTH_HALF + 300.0f, HEIGHT_HALF - 50.0f, 0.0f), m_pos, 0.0025f);
		break;

	default:
		break;
	}
	// 位置の更新
	CScene2D::SetPos(m_pos);
}

//=============================================================================
// 敵の召喚
//=============================================================================
void CBoss::SummonsEnemy(void)
{
	// 位置の取得
	m_pos = CScene2D::GetPos();		

	// 時間の取得
	int nFrame = CGame::GetTime()->GetFrame();	

	// 体力の状態で敵の召喚方法変更
	switch (m_LifeState)
	{
		// ０割減少
	case LIFE_RATE_0:
		if ((nFrame % 15) == 0)
		{
			ENEMY::SetEnemy(m_pos, ENEMY_SIZE, ENEMY_TYPE0, MOVE3, 1);
			ENEMY::SetEnemy(m_pos, ENEMY_SIZE, ENEMY_TYPE0, MOVE2, 10);
		}
		break;

		// ２割減少
	case LIFE_RATE_2:
		if ((nFrame % 60) == 0)
		{
			ENEMY::SetEnemy(m_pos, ENEMY_SIZE, ENEMY_TYPE0, MOVE3, 1);
			ENEMY::SetEnemy(m_pos, ENEMY_SIZE, ENEMY_TYPE0, MOVE2, 25);
			ENEMY::SetEnemy(m_pos, ENEMY_SIZE, ENEMY_TYPE0, MOVE0, 1);
		}
		break;

		// ５割減少
	case LIFE_RATE_5:
		if ((nFrame % 20) == 0)
		{
			ENEMY::SetEnemy(m_pos, ENEMY_SIZE, ENEMY_TYPE0, MOVE0, 1);
		}
		if ((nFrame % 10) == 0)
		{
			ENEMY::SetEnemy(m_pos, ENEMY_SIZE, ENEMY_TYPE0, MOVE3, 1);
		}
		break;

		// ８割減少
	case LIFE_RATE_8:
		if ((nFrame % 7) == 0)
		{
			ENEMY::SetEnemy(m_pos, ENEMY_SIZE, ENEMY_TYPE0, MOVE0, 1);
		}
		if ((nFrame % 5) == 0)
		{
			ENEMY::SetEnemy(m_pos, ENEMY_SIZE, ENEMY_TYPE0, MOVE3, 1);
		}
		if ((nFrame % 30) == 0)
		{
			ENEMY::SetEnemy(m_pos, ENEMY_SIZE, ENEMY_TYPE0, MOVE2, 10);
		}
		if ((nFrame % 100) == 0)
		{
			if (m_pCore[0]->GetUse() == true)
			{
				ENEMY::SetEnemy(m_pCore[0]->GetPos(), ENEMY_SIZE, ENEMY_TYPE0, MOVE2, 10);
			}
			if (m_pCore[1]->GetUse() == true)
			{
				ENEMY::SetEnemy(m_pCore[1]->GetPos(), ENEMY_SIZE, ENEMY_TYPE0, MOVE2, 10);
			}
		}
		break;

	default:
		break;
	}
}
