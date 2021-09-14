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
		pBoss->m_Effect = NULL;
		pBoss->SetPos(pos);					  // 位置
		pBoss->SetSize(size);				  // サイズ
		pBoss->SetLife(nLife);				  // ライフ
		pBoss->SetCol(WhiteColor);			  // 色
		pBoss->SetType(CEnemy::ENEMY_BOSS);	  // タイプ
		pBoss->m_State = STATE_NONE;		  // 状態
		m_bBoss_Alive = true;				  // 生存する
		pBoss->Init();						  // 初期化
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
	CScene2D::SetPos(m_pos);	// 移動量の更新

	switch (m_State)
	{
	case NONE:

		break;

	case DAMAGE:
		DamageBoss();
		break;

	case NOT_DAMAGE:

		break;

	default:
		break;
	}

	// ボスの更新
	UpdateBoss();
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
// タイプBOSSの更新
//=============================================================================
void CBoss::UpdateBoss(void)
{
	srand((unsigned)time(NULL)); // 現在時刻の情報で初期化

	m_pos = CScene2D::GetPos();

	int nCntRand = 0;

	nCntRand = rand() % 60 + 100;
	m_nCnt++;

	int nRandEne0 = rand() % 250 + nCntRand;
	int nEnemy0 = rand() % 50 + 11;

	if ((m_nCnt % nRandEne0) == nEnemy0)
	{
		CNormalEnemy::Create(m_pos, ENEMY_SIZE, ENEMY_TYPE0);
	}
}

void CBoss::DamageBoss(void)
{
	if (m_Effect == NULL)
	{
		m_Effect = CEffect::Create(m_pos, m_size);
		m_Effect->CreateTexture("data/TEXTURE/stateBoss.png");
	}

	m_nDamageCnt++;

	if ((m_nDamageCnt % 300) == 0)
	{
		m_Effect->Uninit();
		m_Effect = NULL;
		m_State = STATE_NONE;
	}


}
