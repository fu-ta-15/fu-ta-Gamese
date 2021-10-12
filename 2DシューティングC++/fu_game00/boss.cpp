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

//-----------------------------------------------------------------------------
// マクロ変数
//-----------------------------------------------------------------------------
#define DAMAGE			(CBoss::STATE_DAMAGE)				// ダメージ状態
#define NOT_DAMAGE		(CBoss::STATE_NOT_DAMAGE)			// ダメージNO!
#define NONE			(CBoss::STATE_NONE)					// 何もない状態
															 
#define DAMAGE_TEXTUER	("data/TEXTURE/stateBoss.png")		// ダメージ状態テクスチャのリンク
#define DAMAGE_COLOR	(D3DXCOLOR(1.0f,1.0f,1.0f,0.0f))	// 色
															
#define SHILED_TEXTUER	("data/TEXTURE/AuroraRing.png")			// シールドテクスチャのリンク
#define SHILED_COLOR	(D3DXCOLOR(1.0f,1.0f,1.0f,0.0f))	// 色
#define SHILED_SIZE		(m_size*2.5f)
#define LIFE_POS		(D3DXVECTOR3(SCREEN_WIDTH - 100.0f, 100.0f + (10 * nCntLife), 0.0f))
#define LIFE_SIZE		(D3DXVECTOR3(10.0f, 5.0f, 0.0f))
#define CreateEnemy		(CNormalEnemy::Create)

//-----------------------------------------------------------------------------
// 静的変数
//-----------------------------------------------------------------------------
bool CBoss::m_bBoss_Alive = true;	// 生存しているかどうか

//=============================================================================
// コンストラクタ
//=============================================================================
CBoss::CBoss() : CEnemy(OBJ_ENEMY)
{
	this->m_pDamage = NULL;			// ダメージエフェクト
	this->m_pShield = NULL;			// シールドエフェクト
	this->m_bShield = false;		// シールド出すか出さないか
	this->m_bDamage = false;		// ダメージ受けているかいないか
	this->m_State = STATE_NONE;		// 状態
	this->m_LifeState = LIFE_RATE_0;// ライフの状態
	m_bBoss_Alive = true;			// 生存する
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
		pBoss = new CBoss;
		pBoss->SetPos(pos);				// 位置
		pBoss->SetSize(size);			// サイズ
		pBoss->m_fLife = nLife;			// ライフ
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
	srand((unsigned)time(NULL));	// 現在時刻の情報で初期化

	// ライフ設定
	for (int nCntLife = 0; nCntLife < BOSS_LIFE_STOCK; nCntLife++)
	{
		D3DXVECTOR3 pos = LIFE_POS;
		D3DXVECTOR3 size = LIFE_SIZE;
		m_pLife[nCntLife] = CScene2D::Create(pos, size);
		m_pLife[nCntLife]->CreateTexture("data/TEXTURE/BossLife.png");
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

	CEnemy::Update();

	// 現在の状態
	switch (m_State)
	{// 現在の状態
	case NONE:
		break;

	case DAMAGE:
		m_bDamage = true;	// ダメージ合図
		break;

	case NOT_DAMAGE:
		break;
	}

	if (m_fLife <= 0)
	{
		m_bBoss_Alive = false;
		Release();
	}

	// ボスの更新
	UpdateBoss();		// 普通の更新
	DamageBoss();		// ダメージ状態更新
	NotDamageBoss();	// ダメージNO！状態更新
	StateUpdate();		// 状態の公人
	MoveBoss();			// 移動処理
	SummonsEnemy();

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
		if (m_fLife < nCntLife * 10)
		{// ライフが区切りの値を越えたら
			switch (nCntLife)
			{
			case 12:
				m_LifeState = LIFE_RATE_2;
				break;
			case 7:
				m_LifeState = LIFE_RATE_5;
				break;
			case 2:
				m_LifeState = LIFE_RATE_8;
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
			m_State = STATE_NOT_DAMAGE;					   // ダメージ受け付けない状態
			m_fA_Damage = 0.065f;						   // 透明度加算用
			m_fLife -= (m_fLife * 0.17f + 0.425f);		   // 体力の減少

			m_pDamage = CEffect::Create(m_pos, m_size);	   // 生成
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
			m_pShield = CEffect::Create(m_pos, SHILED_SIZE); // 生成
			m_pShield->CreateTexture(SHILED_TEXTUER);		 // テクスチャ設定
			m_pShield->SetColor(SHILED_COLOR);				 // 色の設定
			m_ShieldCol = m_pShield->GetColor();			 // 色の取得
			m_fA_Shield = 0.04f;							 // 透明度加算用
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
	m_fMoveTime += 0.54f;

	switch (m_LifeState)
	{
	case CBoss::LIFE_RATE_0:
		m_pos.y = CMove::CosWave(HEIGHT_HALF, 50.0f, 65.5f, m_fMoveTime);
		break;

	case CBoss::LIFE_RATE_2:

		break;

	case CBoss::LIFE_RATE_5:
		
		break;

	case CBoss::LIFE_RATE_8:
		
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

	int nCntRand = 0;				// 乱数保管用

	nCntRand += Rand(60, 10);		// 乱数生成
	m_nCnt++;						// カウント加算

	int nEnemy0 = Rand(19, 45);

	if ((m_nCnt % nCntRand) == nEnemy0)
	{
		CreateEnemy(m_pos, ENEMY_SIZE, ENEMY_TYPE0, MOVE3);
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

		if (m_ShieldCol.a >= 0.7f)
		{// α値が一定を越えたら
			m_fA_Shield = -0.005f;	// 減算
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

		if (m_StateCol.a >= 0.7f)
		{// α値が一定を越えたら
			m_fA_Damage = -0.005f;	// 減算
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
