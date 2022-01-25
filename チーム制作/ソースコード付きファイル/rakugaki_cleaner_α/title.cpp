//=============================================================================
//
// �^�C�g����ʏ��� [title.cpp]
// Author : itsuki takaiwa
//
//=============================================================================
#include "title.h"
#include "manager.h"
#include "fade.h"

#include "set_title.h"
#include "mode_select.h"
#include "title_manager.h"

#include "debugproc.h"
#include "game.h"
#include "sound.h"

//*****************************************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************************************
CTitleManager *CTitle::m_pTitleManager = NULL;

//=============================================================================
// CTitle�̃R���X�g���N�^
//=============================================================================
CTitle::CTitle()
{
	// �����o�ϐ��̏�����
	m_titlemode = TITLEMODE_NORMAL;
	m_pTitleManager = NULL;
}

//=============================================================================
// CTitle�̃f�X�g���N�^
//=============================================================================
CTitle::~CTitle()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CTitle::Init(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	// �����o�ϐ��̏�����
	m_titlemode = TITLEMODE_NORMAL;
	m_pTitleManager = NULL;

	// �^�C�g���Ǘ����̐���
	m_pTitleManager = new CTitleManager;

	if (m_pTitleManager != NULL)
	{
		m_pTitleManager->Init({ 0.0f,0.0f,0.0f }, { 0.0f,0.0f });
	}

	// �^�C�g�����̓ǂݍ���
	CSetTitle *pSet = new CSetTitle;

	if (pSet != NULL)
	{
		pSet->Init();
	}

	// �T�E���h
	CSound::Play(CSound::SOUND_LABEL_TITLE);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CTitle::Uninit(void)
{
	// �T�E���h���~�߂�
	CSound::Stop();
}

//=============================================================================
// �X�V����
//=============================================================================
void CTitle::Update(void)
{
	// ��ʑJ��
	switch (m_titlemode)
	{
		// �ʏ�
	case TITLEMODE_NORMAL:
		break;
		// �J��
	case TITLEMODE_TRANSITION:
		CModeSelect *pModeSelect = new CModeSelect;

		if (pModeSelect != NULL)
		{
			CManager::GetFade()->SetFade(CFade::FADE_OUT, pModeSelect);
		}
		break;
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CTitle::Draw(void)
{

}