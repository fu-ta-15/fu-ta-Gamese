

#include "gauge.h"

CGauge::CGauge()
{
	m_pInPolygon = NULL;
	m_pOutPolygon = NULL;
}

CGauge::~CGauge()
{
}

CGauge * CGauge::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, int Stock)
{
	CGauge *pGauge = NULL;

	pGauge = new CGauge;

	pGauge->m_pos = pos;
	pGauge->m_size = size;
	pGauge->m_nStock = Stock;
	pGauge->Init();

	return pGauge;
}

HRESULT CGauge::Init(void)
{
	m_pInPolygon = new CPolygon[m_nStock];
	for (int nCnt = 0; nCnt < m_nStock; nCnt++,m_pInPolygon++)
	{
		if (m_pInPolygon != NULL)
		{
			m_pInPolygon->Init(m_pos, m_size);
			m_pInPolygon->CreateTexture("data/TEXTURE/EDGE4.png");
			m_pos.x += m_size.x * 2;
		}
	}

	m_pOutPolygon = new CPolygon;
	if (m_pOutPolygon != NULL)
	{
		D3DXVECTOR3 size = m_size;
		size.x += 100.0f;
		m_pOutPolygon->Init(m_pos, m_size);
		m_pOutPolygon->CreateTexture("data/TEXTURE/gauge.png");
	}

	return S_OK;
}

void CGauge::Uninit(void)
{
	if (m_pInPolygon != NULL)
	{
		m_pInPolygon->Uninit();
		delete[m_nStock] m_pInPolygon;
		m_pInPolygon = NULL;
	}

	delete m_pOutPolygon;
	m_pOutPolygon = NULL;
}

void CGauge::Update(void)
{
	for (int nCnt = 0; nCnt < m_nStock; nCnt++, m_pInPolygon++)
	{
		if (m_pInPolygon != NULL)
		{
			m_pInPolygon->Update();
		}
	}
	m_pOutPolygon->Update();
}

void CGauge::Draw(void)
{
	for (int nCnt = 0; nCnt < m_nStock; nCnt++)
	{
		m_pInPolygon[nCnt].Draw();
	}
	m_pOutPolygon->Draw();
}

void CGauge::Relese(void)
{
	for (int nCnt = 0; nCnt < m_nStock; nCnt++, m_pInPolygon++)
	{
		if (m_pInPolygon != NULL)
		{
			m_pInPolygon->SetUse(false);
			break;
		}
	}
}
