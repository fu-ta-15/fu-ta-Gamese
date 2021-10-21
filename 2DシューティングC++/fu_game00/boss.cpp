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
#define DAMAGE_TEXTUER	("data/TEXTURE/stateBoss.png")					// ダメージ状態テクスチャリンク
#define DAMAGE_COLOR	(D3DXCOLOR(1.0f,1.0f,1.0f,0.0f))				// ダメージ状態の色	
#define DAMAGE_ADD_A	(0.065f)										// ダメージのα値加算値
#define SHILED_TEXTUER	("data/TEXTURE/AuroraRing.png")					// バリアテクスチャリンク
#define SHILED_COLOR	(D3DXCOLOR(1.0f,1.0f,1.0f,0.0f))				// バリアの色
#define SHILED_SIZE		(m_size * 2.5f)									// バリアのサイズ
#define SHILED_ADD_A	(0.04f)											// バリアのα値加算値
#define LIFE_POS_X		(SCREEN_WIDTH - 100.0f)							// ライフのX座標
#define LIFE_POS_Y		(200.0f - (10 * nCntLife))						// ライフのY座標
#define LIFE_POS		(D3DXVECTOR3(LIFE_POS_X, LIFE_POS_Y, 0.0f))		// ライフの位置
#define LIFE_SIZE		(D3DXVECTOR3(10.0f, 5.0f, 0.0f))				// ライフのサイズ
#define LIFE_TEXTURE	("data/TEXTURE/BossLife.png")					// ライフテクスチャリンク
#define LIFE_DOWN		((m_fLife * 0.17f + 0.425f))					// ライフの減少計算
#define LIFE_CNT		(nCntLife * 10)									// ライフカウント
#define ADD_MOVE_TIME	(0.54f)											// 移動のカウントの加算値
#define DECREASE_A		(0.008f)										// α値の減少値
#define ADD_MAX_A		(0.7f)											// α値の最大値
#define CreateEnemy		(CNormalEnemy::Create)							// 敵の生成
#define CreateEffect	(CEffect::Create)								// エフェクトの生成


//-----------------------------------------------------------------------------
// 静的変数
//-----------------------------------------------------------------------------
bool CBoss::m_bBoss_Alive = true;	// 生存しているかどうか

//=============================================================================
// コンストラクタ
//=============================================================================
CBoss::CBoss() : CEnemy(OBJ_BOSS)
{
	this->m_pDamage = NULL;				// ダメージエフェクト
	this->m_pShield = NULL;				// シールドエフェクト
	this->m_bShield = false;			// シールド出すか出さないか
	this->m_bDamage = false;			// ダメージ受けているかいないか
	this->m_State = STATE_NONE;			// 状態
	this->m_LifeState = LIFE_RATE_0;	// ライフの状態
	this->m_move = ZeroVector3;			// 移動量初期化
	m_bBoss_Alive = true;				// 生存する
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
	CBoss *pBoss = NULL;

	if (pBoss == NULL)
	{// NULLチェック
		pBoss = new CBoss;				// インスタンス生成
		pBoss->SetPos(pos);				// 位置
		pBoss->SetSize(size);			// サイズ
		pBoss->m_fLife = (float)nLife;	// ライフ
		pBoss->SetCol(WhiteColor);		// 色
		pBoss->SetType(ENEMY_TYPE2);	// タイプ
		pBoss->Init();					// 初期化
	}

	return pBoss;
}

//=============================================================================
// 初期化
//=============================================================================
HRESULT CBoss::Init(void)
{	
	m_nDamageCnt = 0;	 // ダメージカウント
	m_fMoveTime = 0.0f;	 // 移動用カウント

	// ライフ設定
	for (int nCntLife = 0; nCntLife < BOSS_LIFE_STOCK; nCntLife++)
	{
		D3DXVECTOR3 pos = LIFE_POS;						 // 位置
		D3DXVECTOR3 size = LIFE_SIZE;					 // サイズ
		m_pLife[nCntLife] = CScene2D::Create(pos, size); // 生成
		m_pLife[nCntLife]->CreateTexture(LIFE_TEXTURE);	 // テクスチャの設定
	}

	CEnemy::Init();	// 初期化

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CBoss::Uninit(void)
{
	CEnemy::Uninit();	// 終了処理
}

//=============================================================================
// 更新処理
//=============================================================================
void CBoss::Update(void)
{

	// メッシュポリゴンの情報取得
	switch (CManager::GetMode())
	{
		// チュートリアル
	case CManager::MODE_TUTORIAL:


		break;

		// ゲーム
	case CManager::MODE_GAME:

		// 更新
		CEnemy::Update();

		if (m_State == DAMAGE)
		{
			// ダメージ合図
			m_bDamage = true;	
		}
		if (m_fLife <= 0)
		{
			// ライフがゼロの場合、死亡
			m_bBoss_Alive = false;	
			Release();				
		}

		// ボスの更新
		UpdateBoss();		// 普通の更新
		DamageBoss();		// ダメージ状態更新
		NotDamageBoss();	// ダメージNO！状態更新
		StateUpdate();		// 状態の公人
		MoveBoss();			// 移動処理
		SummonsEnemy();		// 敵召喚処理
		break;

	default:
		break;
	}

	// 位置の更新
	CScene2D::SetPos(m_pos);
	CScene2D::SetSize(m_size);
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
// BOSSの更新
//=============================================================================
void CBoss::UpdateBoss(void)
{
	// ライフブロックの更新
	for (int nCntLife = 0; nCntLife < BOSS_LIFE_STOCK; nCntLife++)
	{
		if (m_fLife < LIFE_CNT)
		{// ライフが区切りの値を越えたら
			switch (nCntLife)
			{// 残り体力
			case 12:
				if (m_LifeState == LIFE_RATE_0)
				{
					m_LifeState = LIFE_RATE_2;	// ２割減少
					m_fMoveTime = 0.0f;
				}
				break;

			case 7:
				if (m_LifeState == LIFE_RATE_2)
				{
					m_LifeState = LIFE_RATE_5;	// ５割減少
					m_fMoveTime = 0.0f;
				}
				break;

			case 2:
				if (m_LifeState == LIFE_RATE_5)
				{
					m_LifeState = LIFE_RATE_8;	// ８割減少
					m_fMoveTime = 0.0f;
				}
				break;

			default:
				break;
			}

			if (m_pLife[nCntLife] != NULL)
			{// NULLチェック
				m_pLife[nCntLife]->Release();  // ライフのブロック削除
				m_pLife[nCntLife] = NULL;	   // NULL代入
				break;
			}
		}
	}
}

//=============================================================================
// ダメージを受けた時
//=============================================================================
void CBoss::DamageBoss(void)
{
	if (m_bDamage == true)
	{// ダメージ状態
		if (m_pDamage == NULL)
		{// NULLチェック
			m_State = STATE_NOT_DAMAGE;		// ダメージ受け付けない状態
			m_fA_Damage = DAMAGE_ADD_A;		// 透明度加算用
			m_fLife -= LIFE_DOWN;			// 体力の減少

			m_pDamage = CreateEffect(m_pos, m_size);	   // 生成
			m_pDamage->CreateTexture(DAMAGE_TEXTUER);	   // テクスチャの設定
			m_pDamage->SetColor(DAMAGE_COLOR);			   // 色の設定
			m_StateCol = m_pDamage->GetColor();			   // 色の取得

		}
		if (m_pDamage != NULL)
		{// NULLじゃなかったら
			m_pDamage->SetPos(m_pos);	// 位置を更新
		}
	}
}

//=============================================================================
// ダメージ受け付けない状態
//=============================================================================
void CBoss::NotDamageBoss(void)
{
	if (m_bShield == true)
	{// シールド展開中
		if (m_pShield == NULL)
		{// NULLチェック
			m_pShield = CreateEffect(m_pos, SHILED_SIZE);	// 生成
			m_pShield->CreateTexture(SHILED_TEXTUER);		// テクスチャ設定
			m_pShield->SetColor(SHILED_COLOR);				// 色の設定
			m_ShieldCol = m_pShield->GetColor();			// 色の取得
			m_fA_Shield = SHILED_ADD_A;						// 透明度加算用
		}
		if (m_pShield != NULL)
		{// NULLじゃなかったら
			m_pShield->SetPos(m_pos);	// 位置を更新
		}
	}
}

//=============================================================================
// 移動処理
//=============================================================================
void CBoss::MoveBoss(void)
{
	// カウント加算
	m_fMoveTime += ADD_MOVE_TIME;

	// ライフの状態で移動変更
	switch (m_LifeState)
	{// 体力の状態
	case LIFE_RATE_0:
		m_pos.y = Move::CosWave(HEIGHT_HALF,100.0f, 65.5f, m_fMoveTime);
		m_OldPos = m_pos;
		break;

	case LIFE_RATE_2:
		m_pos.y = Move::CosWave(m_OldPos.y,60.0f, 100.5f, m_fMoveTime);
		m_pos.x += Move::SinMove(-10.0f, 60.0f, m_fMoveTime);
		break;

	case LIFE_RATE_5:

		m_pos.y = Move::CosWave(HEIGHT_HALF, 50.0f, 65.5f, m_fMoveTime);

		break;

	case LIFE_RATE_8:

		m_move = Move::TargetPosMove(D3DXVECTOR3(WIDTH_HALF + 200.0f, HEIGHT_HALF - 100.0f, 0.0f), m_pos, 0.0025f);

		break;

	default:
		break;
	}
	CScene2D::SetPos(m_pos);

}

//=============================================================================
// 敵の召喚
//=============================================================================
void CBoss::SummonsEnemy(void)
{
	m_pos = CScene2D::GetPos();		// 位置の取得
	int nFrame = TIME->GetFrame();	// 時間の取得

	// 体力の状態で敵の召喚方法変更
	switch (m_LifeState)
	{// 体力の状態
	case LIFE_RATE_0:
		if ((nFrame % 130) == 0)
		{
			CreateEnemy(m_pos, ENEMY_SIZE, ENEMY_TYPE0, MOVE3);

		}
		if ((nFrame % 100) == 0)
		{
			CreateEnemy(m_pos, ENEMY_SIZE, ENEMY_TYPE0, MOVE4);
		}
		break;

	case LIFE_RATE_2:
		if ((nFrame % 120) == 0)
		{
			CreateEnemy(m_pos, ENEMY_SIZE, ENEMY_TYPE0, MOVE3);
		}
		if ((nFrame % 160) == 0)
		{
			CreateEnemy(m_pos, ENEMY_SIZE, ENEMY_TYPE0, MOVE2);
			CreateEnemy(m_pos, ENEMY_SIZE, ENEMY_TYPE0, MOVE2);
			CreateEnemy(m_pos, ENEMY_SIZE, ENEMY_TYPE0, MOVE2);
			CreateEnemy(m_pos, ENEMY_SIZE, ENEMY_TYPE0, MOVE2);
			CreateEnemy(m_pos, ENEMY_SIZE, ENEMY_TYPE0, MOVE2);
		}
		break;

	case LIFE_RATE_5:
		if ((nFrame % 156) == 0)
		{
			CreateEnemy(m_pos, ENEMY_SIZE, ENEMY_TYPE0, MOVE2);
			CreateEnemy(m_pos, ENEMY_SIZE, ENEMY_TYPE0, MOVE2);
			CreateEnemy(m_pos, ENEMY_SIZE, ENEMY_TYPE0, MOVE2);
			CreateEnemy(m_pos, ENEMY_SIZE, ENEMY_TYPE0, MOVE2);
			CreateEnemy(m_pos, ENEMY_SIZE, ENEMY_TYPE0, MOVE2);
		}
		if ((nFrame % 150) == 0)
		{
			CreateEnemy(m_pos, ENEMY_SIZE, ENEMY_TYPE1, MOVE2);
		}
		break;

	case LIFE_RATE_8:
		if ((nFrame % 80) == 0)
		{
			CreateEnemy(m_pos, ENEMY_SIZE, ENEMY_TYPE0, MOVE0);
			CreateEnemy(m_pos, ENEMY_SIZE, ENEMY_TYPE0, MOVE0);
			CreateEnemy(m_pos, ENEMY_SIZE, ENEMY_TYPE0, MOVE0);
		}
		break;

	default:
		break;
	}
}

//=============================================================================
// 状態管理
//=============================================================================
void CBoss::StateUpdate(void)
{
	if (m_pShield != NULL)
	{// シールド展開中
		// カラーの更新
		m_pShield->SetColor(m_ShieldCol);
		m_ShieldCol.a += m_fA_Shield;

		if (m_ShieldCol.a >= ADD_MAX_A)
		{// α値が一定を越えたら
			m_fA_Shield = -DECREASE_A;	// 減算
		}
		if (m_ShieldCol.a < 0.0f)
		{// α値が一定を越えたら
			m_pShield->Uninit();	// 終了処理
			m_pShield = NULL;		// NULL代入
			m_bShield = false;		// FALSE代入
		}
	}
	if (m_pDamage != NULL)
	{// ダメージ状態
		// カラーの更新
		m_pDamage->SetColor(m_StateCol);
		m_StateCol.a += m_fA_Damage;

		if (m_StateCol.a >= ADD_MAX_A)
		{// α値が一定を越えたら
			m_fA_Damage = -DECREASE_A;	// 減算
		}
		if (m_StateCol.a <= 0.0f)
		{// α値が一定を越えたら
			m_pDamage->Uninit();	// 終了処理
			m_pDamage = NULL;		// NULL代入
			m_bDamage = false;		// FALSE代入
			m_State = STATE_NONE;	// 状態を戻す
		}
	}
}
