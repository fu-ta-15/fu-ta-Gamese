//=============================================================================
//
// �v���C���[�ҋ@��ʏ��� [mode_select.cpp]
// Author : itsuki takaiwa
//
//=============================================================================
#include "preparation_player.h"
#include "manager.h"
#include "fade.h"
#include "sound.h"
#include "set_preparation_player.h"
#include "preparation_player_manager.h"
#include "mode_select.h"
#include "input_keyboard.h"
#include "keep_object.h"

#include "game.h"
#include "tutorial.h"
#include "debugproc.h"

//*****************************************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************************************
CPreparationPlayerManager *CPreparationPlayer::m_pPPM = NULL;
int CPreparationPlayer::m_nCntPlayerOP = 0;

//=============================================================================
// CPreparationPlayer�̃R���X�g���N�^
//=============================================================================
CPreparationPlayer::CPreparationPlayer()
{
	// �����o�ϐ��̏�����
	m_pPPM = NULL;
}

//=============================================================================
// CPreparationPlayer�̃f�X�g���N�^
//=============================================================================
CPreparationPlayer::~CPreparationPlayer()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CPreparationPlayer::Init(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	m_nCntPlayerOP = 0;
	m_nNumPlayer = 4;
	// �v���C���[������ʊǗ����̐���
	m_pPPM = new CPreparationPlayerManager;

	if (m_pPPM != NULL)
	{
		m_pPPM->Init({ 0.0f,0.0f,0.0f }, { 0.0f,0.0f });
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CPreparationPlayer::Uninit(void)
{
	// �T�E���h�̒�~
	CSound::Stop();
}

//=============================================================================
// �X�V����
//=============================================================================
void CPreparationPlayer::Update(void)
{
	// ��ʑJ��
	CBase *pBase = NULL;
	CKeepObject *pKeepObject = CManager::GetKeepObject();

	if (m_pPPM->GetTransition() == true)
	{
		switch (m_preparationPlayer)
		{
			// �ʏ�
		case PREPARATIONPLAYER_NORMAL:
			break;
			// �V���O�����[�h�J��
		case PREPARATIONPLAYER_TRANSITION_GAME_BATTLE:
			pKeepObject->SetGameMode(CKeepObject::GAMEMODE_BATTLE);
			pBase = new CGame;

			if (pBase != NULL)
			{
				CManager::GetFade()->SetFade(CFade::FADE_OUT, pBase);
			}
			break;
			// �}���`���[�h�J��
		case PREPARATIONPLAYER_TRANSITION_GAME_COOPERATION:
			pKeepObject->SetGameMode(CKeepObject::GAMEMODE_COOPERATION);
			pBase = new CGame;

			if (pBase != NULL)
			{
				CManager::GetFade()->SetFade(CFade::FADE_OUT, pBase);
			}
			break;
			// ���[�őI��
		case PREPARATIONPLAYER_TRANSITION_MODESELECT:
			pBase = new CModeSelect;

			if (pBase != NULL)
			{
				CManager::SetMode(pBase);
			}
			break;
		}
	}

#ifdef _DEBUG
	// �L�[�{�[�h���̎擾
	CInputKeyboard *pKey = GET_KEYBOARD;

	// ����\�v���C���[�̑���
	if (pKey->GetTrigger(KEY_PLAYEROP_UP))
	{
		m_nCntPlayerOP++;

		if (m_nCntPlayerOP > m_nNumPlayer - 1)
		{
			m_nCntPlayerOP = 0;
		}
	}
	else if (pKey->GetTrigger(KEY_PLAYEROP_DOWN))
	{
		m_nCntPlayerOP--;

		if (m_nCntPlayerOP < 0)
		{
			m_nCntPlayerOP = m_nNumPlayer - 1;
		}
	}
	CDebugProc::Print("����\�v���C���[ : (%d / %d)\n", m_nCntPlayerOP, m_nNumPlayer - 1);
#endif
}

//=============================================================================
// �`�揈��
//=============================================================================
void CPreparationPlayer::Draw(void)
{
	// �v���C���[������ʊǗ����̕`�揈��
	m_pPPM->Draw();
}