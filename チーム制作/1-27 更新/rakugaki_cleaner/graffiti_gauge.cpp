//=============================================================================
//
// ラクガキ用ゲージ処理 [graffiti_gauge.cpp]
// Author : itsuki takaiwa
//
//=============================================================================
#include "graffiti_gauge.h"
#include "manager.h"
#include "score.h"
#include "number.h"
#include "texture.h"
#include "Scene2D.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CGraffitiGauge::CGraffitiGauge()
{
}

//=============================================================================
// デストラクタ
//=============================================================================
CGraffitiGauge::~CGraffitiGauge()
{
}

//=============================================================================
// ゲージの生成
//=============================================================================
CGraffitiGauge * CGraffitiGauge::Create(const D3DXVECTOR3 & pos, const D3DXVECTOR2 & size, int nTex)
{
	CGraffitiGauge *pGraffitiGauge = new CGraffitiGauge;

	pGraffitiGauge->m_pos = pos;
	pGraffitiGauge->m_size = size;
	pGraffitiGauge->Init(nTex);

	return pGraffitiGauge;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CGraffitiGauge::Init(int nTex)
{
	CGauge::Init(m_pos, m_size, nTex);
	CGauge::SetValue(0.075f);
	AddPosTex(D3DXVECTOR3(m_pos.x + m_size.x/ 2.0f, 0.0f, 0.0f), D3DXVECTOR2(0.0f, 0.0f), 2, 3);

	// メンバ変数の初期化
	m_fRatio = 0.0f;
	m_fPosXRightSide = 0.0f;

	// テクスチャ管理情報取得
	CTexture *pTex = CManager::GetTexture();

	// ラクガキゲージの整数部分の生成
	m_pScoreInteger = CScore::Create(D3DXVECTOR3(0.0f,0.0f,0.0f),
		D3DXVECTOR2(10.0f,20.0f),
		14.5f,
		CScore::SCOREPLACEMENT_CENTERED,
		pTex->SetTexture("data/TEXTURE/SCORE_LOGO04.png"));

	// ラクガキゲージの小数部分の生成
	m_pNumberDecimal = CNumber::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f),D3DXVECTOR2(10.0f, 20.0f));
	m_pNumberDecimal->SetTexNum(pTex->SetTexture("data/TEXTURE/SCORE_LOGO04.png"));
	
	// ロゴ情報の生成
	m_pLogoComma = CScene2D::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(20.0f, 20.0f, 0.0f), CScene::PRIORITY_UI, pTex->SetTexture("data/TEXTURE/SCORE_LOGO06.png"));
	m_pLogoComma->SetTexSheet(D3DXVECTOR2(2,1));
	m_pLogoComma->SetTex(D3DXVECTOR2(0, 0));

	m_pLogoPercent = CScene2D::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(20.0f, 20.0f, 0.0f), CScene::PRIORITY_UI, pTex->SetTexture("data/TEXTURE/SCORE_LOGO06.png"));
	m_pLogoPercent->SetTexSheet(D3DXVECTOR2(2, 1));
	m_pLogoPercent->SetTex(D3DXVECTOR2(1, 0));

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CGraffitiGauge::Uninit(void)
{
	CGauge::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CGraffitiGauge::Update(void)
{
	CGauge::Update();

	m_pScoreInteger->SetScore((int)(m_fRatio * 100.0f));
	m_pScoreInteger->SetPosition(D3DXVECTOR3(m_fPosXRightSide,GetPosition().y + 30.0f,0.0f));

	m_pNumberDecimal->SetNumber((int)((m_fRatio * 10000.0f) / powf(10.0f, 1.0f)));
	D3DXVECTOR3 pos = m_pScoreInteger->GetPosition(m_pScoreInteger->GetDigit() - 1);
	m_pNumberDecimal->SetPosition(D3DXVECTOR3(pos.x + 20.5f, pos.y, pos.z));

	D3DXVECTOR3 posNum = m_pNumberDecimal->GetPosition();
	m_pLogoComma->SetPosition(D3DXVECTOR3(posNum.x - 11.5f, posNum.y + 6.0f, posNum.z));
	m_pLogoPercent->SetPosition(D3DXVECTOR3(posNum.x + 15.5f, posNum.y + 6.0f, posNum.z));
}

//=============================================================================
// 描画処理
//=============================================================================
void CGraffitiGauge::Draw(void)
{
	CGauge::Draw();

	m_pNumberDecimal->Draw();
}