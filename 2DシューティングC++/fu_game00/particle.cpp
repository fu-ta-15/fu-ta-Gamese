//*****************************************************************************
//
// パーティクル処理 [particle.cpp]
// Author : SUZUKI FUUTA
//
//*****************************************************************************
//-----------------------------------------------------------------------------
// インクルードファイル
//-----------------------------------------------------------------------------
#include "particle.h"
#include "manager.h"
#include "renderer.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CParticle::CParticle()
{
}

//=============================================================================
// デストラクタ
//=============================================================================
CParticle::~CParticle()
{
}

//=============================================================================
// ポーズの生成
//=============================================================================
CParticle * CParticle::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, ParticleType type)
{
	CParticle *pParticle = NULL;

	if (pParticle == NULL)
	{
		pParticle = new CParticle;
		pParticle->m_pos = pos;
		pParticle->m_size = size;
		pParticle->m_type = type;
		pParticle->m_apParticle[MAX_EFFECT_NUM] = {};
		pParticle->Init();
	}

	return pParticle;
}

//=============================================================================
// 初期化
//=============================================================================
HRESULT CParticle::Init(void)
{

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CParticle::Uninit(void)
{
	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT_NUM; nCntEffect++)
	{
		if (m_apParticle[nCntEffect] != NULL)
		{
			m_apParticle[nCntEffect]->Uninit();
			delete m_apParticle[nCntEffect];
			m_apParticle[nCntEffect] = NULL;
		}
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void CParticle::Update(void)
{
}

//=============================================================================
// 描画処理
//=============================================================================
void CParticle::Draw(void)
{
}

void CParticle::SetGravity(void)
{
	for (int nCntEffect = 0; nCntEffect < m_EffectNum; nCntEffect++)
	{
		if (m_apParticle[nCntEffect] != NULL)
		{
			m_apParticle[nCntEffect]->SetGravity(true);
		}
	}
}

void CParticle::SetParticle(int EfeectNum)
{
	m_EffectNum = EfeectNum;
	float fAngle = 0.0f;
	for (int nCntEffect = 0; nCntEffect < EfeectNum; nCntEffect++)
	{
		switch (m_type)
		{
		case CParticle::TYPE_EXPLOSION:
			fAngle = (float)((rand() % 628) - 628) / 100.f;
			break;

		case CParticle::TYPE_RIGHT_FAN:
			fAngle = (float)(rand() % -314) / 100.f;
			break;

		case CParticle::TYPE_LEFT_FAN:
			fAngle = (float)(rand() % 314) / 100.f;
			break;

		default:
			break;
		}
		// 情報代入
		m_move.x = sinf(fAngle + D3DX_PI) * 3.0f;
		m_move.y = cosf(fAngle - D3DX_PI) * 3.0f;
		m_move.z = 0.0f;

		if (m_apParticle[nCntEffect] == NULL)
		{
			m_apParticle[nCntEffect] = CEffect::Create(m_pos, m_size, m_move);
			m_apParticle[nCntEffect]->SetTexture(m_pTex);
		}
	}
}

HRESULT CParticle::SetTexture(const LPCSTR pSrcFile)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, pSrcFile, &m_pTex);

	return S_OK;
}

