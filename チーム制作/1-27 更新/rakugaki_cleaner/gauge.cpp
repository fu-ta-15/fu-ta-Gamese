//=============================================================================
//
// �Q�[�W���� [gauge.cpp]
// Author : 
//
//=============================================================================

#include "gauge.h"
#include "renderer.h"
#include "manager.h"

#include "texture.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CGauge::CGauge() : CScene2D(PRIORITY_UI)
{
	m_AddSize = D3DXVECTOR2(0.0f, 0.0f);
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CGauge::~CGauge()
{
}

//=============================================================================
// �Q�[�W�̐���
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
// ����������
//=============================================================================
HRESULT CGauge::Init(const D3DXVECTOR3& pos, const D3DXVECTOR2& size, int nTex)
{
	// �e�N�X�`�����̎擾
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
// �I������
//=============================================================================
void CGauge::Uninit(void)
{
	CScene2D::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CGauge::Update(void)
{
	CScene2D::Update();
}

//=============================================================================
// �`�揈��
//=============================================================================
void CGauge::Draw(void)
{
	CScene2D::Draw();
}

//=============================================================================
// �ǂ̂��炢�̊����ŉ��Z�E���Z���邩�̒l��ݒ�
// ( �����u�����_�Łv )
//=============================================================================
void CGauge::SetValue(float fPercent)
{
	m_AddSize = this->GetSize() * fPercent;
	m_fValue = fPercent;
}

//=============================================================================
// �Q�[�W�̉��Z�E���Z
// ( ���Z or ���Z , ���_���WNo�P , ���_���WNo�Q )
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
