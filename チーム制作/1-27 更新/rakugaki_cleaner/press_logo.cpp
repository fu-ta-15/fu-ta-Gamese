//=============================================================================
//
// �v���X���S���� [press_logo.cpp]
// Author : 
//
//=============================================================================
#include "press_logo.h"

#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "input_keyboard.h"
#include "input_joypad.h"

#include "base.h"
#include "title.h"

//=============================================================================
// CPressLogo�̃R���X�g���N�^
//=============================================================================
CPressLogo::CPressLogo(int nPriority) : CScene2D(nPriority)
{
	// �����o�ϐ��̏�����
	m_nCntInterval = 0;
	m_nCntSwap = 0;
	m_bPressButton = false;
	m_bCounter = true;
}

//=============================================================================
// CPressLogo�̃f�X�g���N�^
//=============================================================================
CPressLogo::~CPressLogo()
{

}

//=============================================================================
// ��������
//=============================================================================
CPressLogo *CPressLogo::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, int nCntSwap, int nTex)
{
	// �|�C���^�ϐ��̐���
	CPressLogo *pPressLogo;
	pPressLogo = new CPressLogo(CScene::PRIORITY_UI);

	// NULL�`�F�b�N
	if (pPressLogo != NULL)
	{
		// ����������
		pPressLogo->Init(pos, size, nCntSwap, nTex);

		// �I�u�W�F�N�g�^�C�v�̐ݒ�
		pPressLogo->SetObjType(OBJTYPE_LOGO);
	}

	return pPressLogo;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CPressLogo::Init(D3DXVECTOR3 pos, D3DXVECTOR2 size, int nCntSwap, int nTex)
{
	// CScene2D�̏���������
	CScene2D::Init(pos, size);

	// �e�N�X�`���Ǘ����̎擾
	CTexture *pTex = CManager::GetTexture();

	// ���NULL����Ȃ��Ƃ��e�N�X�`����K�p����
	if (pTex != NULL)
	{
		BindTexture(pTex->GetAddress(nTex));
	}

	// �����o�ϐ��̏�����
	m_nCntInterval = 0;
	m_nCntSwap = nCntSwap;
	m_bPressButton = false;
	m_bCounter = true;
	m_bTransition = false;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CPressLogo::Uninit(void)
{
	// CScene2D�̏I������
	CScene2D::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CPressLogo::Update(void)
{
	// CScene2D�̍X�V����
	CScene2D::Update();

	// �J�E���g�X�V
	if (m_bCounter == true)
	{
		m_nCntInterval++;
	}

	// �F�ύX
	ChangeColor();
}

//=============================================================================
// �`�揈��
//=============================================================================
void CPressLogo::Draw(void)
{
	// CScene2D�̕`�揈��
	CScene2D::Draw();
}

//=============================================================================
// �{�^�������������ǂ���
//=============================================================================
void CPressLogo::PressButton(void)
{
	// true�ɂ���
	m_bPressButton = true;

	// �J�E���g��������
	m_nCntInterval = 0;
}

//=============================================================================
// �F�ύX����
//=============================================================================
void CPressLogo::ChangeColor(void)
{
	// �_�ł�����
	if (m_bPressButton != true)
	{
		// �{�^���������Ă��Ȃ��Ƃ�
		if (m_nCntInterval % (m_nCntSwap * 2) == 0)
		{
			SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));		// ����
		}
		else if (m_nCntInterval % (m_nCntSwap * 2) == m_nCntSwap)
		{
			SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));		// �s����
		}
	}
	else
	{
		// �{�^�����������Ƃ�
		if (m_nCntInterval % (m_nCntSwap / 2) == 0)
		{
			SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));		// ����
		}
		else if (m_nCntInterval % (m_nCntSwap / 2) == (m_nCntSwap / 4))
		{
			SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));		// �s����
		}

		// �J�E���g��60�ȏ�̎�
		if (m_nCntInterval > 60)
		{
			m_bTransition = true;
		}
	}
}

//=============================================================================
// �F�ύX����
//=============================================================================
void CPressLogo::SetStartCnt(bool bStart)
{
	// �ۑ�
	m_bCounter = bStart;

	// �J�E���^��������
	m_nCntInterval = 0;
}