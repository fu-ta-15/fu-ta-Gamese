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

//-----------------------------------------------------------------------------
// マクロ変数
//-----------------------------------------------------------------------------
#define DAMAGE			(CBoss::STATE_DAMAGE)
#define NOT_DAMAGE		(CBoss::STATE_NOT_DAMAGE)
#define NONE			(CBoss::STATE_NONE)

#define DAMAGE_TEXTUER	("data/TEXTURE/stateBoss.png")
#define DAMAGE_COLOR	(D3DXCOLOR(1.0f,1.0f,1.0f,0.0f))

#define SHILED_TEXTUER	("data/TEXTURE/shield.png")
#define SHILED_COLOR	(D3DXCOLOR(1.0f,1.0f,1.0f,0.0f))


//-----------------------------------------------------------------------------
// 静的変数
//-----------------------------------------------------------------------------
bool CBoss::m_bBoss_Alive = true;	// 生存しているかどうか

//=============================================================================
// コンストラクタ
//=============================================================================
CBoss::CBoss()
{
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
		pBoss->m_pDamage = NULL;		// ダメージエフェクト
		pBoss->m_pShield = NULL;		// シールドエフェクト
		pBoss->SetPos(pos);				// 位置
		pBoss->SetSize(size);			// サイズ
		pBoss->SetLife(nLife);			// ライフ
		pBoss->SetCol(WhiteColor);		// 色
		pBoss->SetType(ENEMY_TYPE2);	// タイプ
		pBoss->m_State = STATE_NONE;	// 状態
		pBoss->m_bShield = false;		// シールド出すか出さないか
		pBoss->m_bDamage = false;		// ダメージ受けているかいないか
		m_bBoss_Alive = true;			// 生存する
		pBoss->Init();					// 初期化
	}

	return pBoss;
}

//=============================================================================
// 初期化
//=============================================================================
HRESULT CBoss::Init(void)
{
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
	// 位置の更新
	CScene2D::SetPos(m_pos);	

	switch (m_State)
	{
	case NONE:
		NoneBoss();
		break;

	case DAMAGE:
		m_bDamage = true;
		break;

	case NOT_DAMAGE:
		break;
	}

	// ボスの更新
	UpdateBoss();
	DamageBoss();
	NotDamageBoss();
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
	srand((unsigned)time(NULL));	// 現在時刻の情報で初期化

	m_pos = CScene2D::GetPos();		// 位置の取得

	int nCntRand = 0;				// 乱数保管用

	nCntRand = rand() % 60 + 100;	// 乱数生成
	m_nCnt++;						// カウント加算

	int nRandEne0 = rand() % 250 + nCntRand;
	int nEnemy0 = rand() % 50 + 11;

	if ((m_nCnt % nRandEne0) == nEnemy0)
	{
		CNormalEnemy::Create(m_pos, ENEMY_SIZE, ENEMY_TYPE0);
	}
}

//=============================================================================
// ボスの通常状態
//=============================================================================
void CBoss::NoneBoss(void)
{



}

//=============================================================================
// ダメージを受けた時
//=============================================================================
void CBoss::DamageBoss(void)
{
	if (m_bDamage == true)
	{
		if (m_pDamage == NULL)
		{
			m_State = STATE_NOT_DAMAGE;
			m_pDamage = CEffect::Create(m_pos, m_size);
			m_pDamage->CreateTexture(DAMAGE_TEXTUER);
			m_pDamage->SetColor(DAMAGE_COLOR);
			m_StateCol = m_pDamage->GetColor();
			m_nA_Damage = 0.065f;
			printf("ダメージ");
		}
		if (m_pDamage != NULL)
		{
			// カラーの更新
			m_pDamage->SetColor(m_StateCol);

			m_StateCol.a += m_nA_Damage;

			if (m_StateCol.a >= 0.7f)
			{
				m_nA_Damage = -0.005f;
			}
			if (m_StateCol.a <= 0.0f)
			{
				m_pDamage->Uninit();
				m_pDamage = NULL;
				m_State = STATE_NONE;
				m_bDamage = false;
			}
		}
	}
}

//=============================================================================
// ダメージ受け付けない状態
//=============================================================================
void CBoss::NotDamageBoss(void)
{
	if (m_bShield == true)
	{
		if (m_pShield == NULL)
		{
			m_pShield = CEffect::Create(m_pos, m_size);
			m_pShield->CreateTexture(SHILED_TEXTUER);
			m_pShield->SetColor(SHILED_COLOR);
			m_ShieldCol = m_pShield->GetColor();
			m_nA_Shield = 0.04f;
		}

		if (m_pShield != NULL)
		{
			// カラーの更新
			m_pShield->SetColor(m_ShieldCol);

			m_ShieldCol.a += m_nA_Shield;

			if (m_ShieldCol.a >= 0.7f)
			{
				m_nA_Shield = -0.1f;
			}
			if (m_ShieldCol.a < 0.0f)
			{
				m_pShield->Uninit();
				m_pShield = NULL;
				m_bShield = false;
			}
		}
	}
}
