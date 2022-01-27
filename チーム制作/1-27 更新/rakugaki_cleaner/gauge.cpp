//=============================================================================
//
// ゲージ処理 [gauge.cpp]
// Author : 
//
//=============================================================================

#include "gauge.h"
#include "renderer.h"
#include "manager.h"

#include "texture.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CGauge::CGauge() : CScene2D(PRIORITY_UI)
{
	m_AddSize = D3DXVECTOR2(0.0f, 0.0f);
}

//=============================================================================
// デストラクタ
//=============================================================================
CGauge::~CGauge()
{
}

//=============================================================================
// ゲージの生成
//=============================================================================
CGauge * CGauge::Create(const D3DXVECTOR3 & pos, const D3DXVECTOR2 & size,int nTex)
{
	CGauge *pGauge = NULL;

	if (pGauge == NULL)
	{
		pGauge = new CGauge();
		pGauge->m_pos = pos;
		pGauge->m_size = size;
		pGauge->Init(pos,size, nTex);
	}

	return pGauge;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CGauge::Init(const D3DXVECTOR3& pos, const D3DXVECTOR2& size, int nTex)
{
	// テクスチャ情報の取得
	CTexture *pTex = CManager::GetTexture();
	m_pos = pos;
	m_size = size;
	CScene2D::Init(m_pos, D3DXVECTOR2(m_size.x, m_size.y));
	CScene2D::BindTexture(pTex->GetAddress(nTex));

	AddPosTex(D3DXVECTOR3(m_pos.x-size.x,0.0f,0.0f),D3DXVECTOR2(0.0f,0.0f),2,3);
	m_posDef = m_pos;
	m_sizeDef = m_size;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CGauge::Uninit(void)
{
	CScene2D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CGauge::Update(void)
{
	CScene2D::Update();
}

//=============================================================================
// 描画処理
//=============================================================================
void CGauge::Draw(void)
{
	CScene2D::Draw();
}

//=============================================================================
// どのくらいの割合で加算・減算するかの値を設定
// ( 割合「小数点で」 )
//=============================================================================
void CGauge::SetValue(float fPercent)
{
	m_AddSize = this->GetSize() * fPercent;
	m_fValue = fPercent;
}

//=============================================================================
// ゲージの加算・減算
// ( 加算 or 減算 , 頂点座標No１ , 頂点座標No２ )
//=============================================================================
void CGauge::AddGauge(ADD_GAUGE type, int nVtxNo1, int nVtxNo2)
{
	switch (type)
	{
	case CGauge::ADD_UP:
		
		this->AddPosTex(D3DXVECTOR3(0.0f, m_AddSize.y, 0.0f), D3DXVECTOR2(0.0f, m_fValue), nVtxNo1, nVtxNo2);
		
		break;

	case CGauge::ADD_DOWN:

		this->AddPosTex(D3DXVECTOR3(0.0f, -m_AddSize.y, 0.0f), D3DXVECTOR2(-0.0f, -m_fValue), nVtxNo1, nVtxNo2);
		
		break;

	case CGauge::ADD_RIGHT:

		this->AddPosTex(D3DXVECTOR3(m_AddSize.x, 0.0f, 0.0f), D3DXVECTOR2(m_fValue, 0.0f), nVtxNo1, nVtxNo2);

		break;

	case ADD_LEFT:

		this->AddPosTex(D3DXVECTOR3(-m_AddSize.x, 0.0f, 0.0f), D3DXVECTOR2(-m_fValue, 0.0f), nVtxNo1, nVtxNo2);

		break;

	default:
		break;
	}
}
