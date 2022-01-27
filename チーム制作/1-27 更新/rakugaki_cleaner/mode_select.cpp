//=============================================================================
//
// ���[�h�I����ʏ��� [mode_select.cpp]
// Author : itsuki takaiwa
//
//=============================================================================
#include "mode_select.h"
#include "manager.h"
#include "fade.h"
#include "sound.h"
#include "set_mode_select.h"

#include "preparation_player.h"
#include "game.h"
#include "tutorial.h"
#include "debugproc.h"

//=============================================================================
// CModeSelect�̃R���X�g���N�^
//=============================================================================
CModeSelect::CModeSelect()
{

}

//=============================================================================
// CModeSelect�̃f�X�g���N�^
//=============================================================================
CModeSelect::~CModeSelect()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CModeSelect::Init(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	// �����o�ϐ��̏�����
	m_modeselect = MODESELECT_NORMAL;

	// �^�C�g�����̓ǂݍ���
	CSetModeSelect *pSet = new CSetModeSelect;

	if (pSet != NULL)
	{
		pSet->Init();
	}

	// �T�E���h
	CSound::Play(CSound::SOUND_LABEL_MODESELECT);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CModeSelect::Uninit(void)
{
	
}

//=============================================================================
// �X�V����
//=============================================================================
void CModeSelect::Update(void)
{
	// ��ʑJ��
	CBase *pBase = NULL;
	CPreparationPlayer *pPP = NULL;

	switch (m_modeselect)
	{
		// �ʏ�
	case MODESELECT_NORMAL:
		break;
		// �V���O�����[�h�J��
	case MODESELECT_TRANSITION_GAME_BATTLE:
		pPP = new CPreparationPlayer;
		pPP->SetMode(CPreparationPlayer::PREPARATIONPLAYER_TRANSITION_GAME_BATTLE);
		pBase = pPP;

		if (pBase != NULL)
		{
			CManager::SetMode(pBase);
		}
		break;
		// �}���`���[�h�J��
	case MODESELECT_TRANSITION_GAME_COOPERATION:
		pPP = new CPreparationPlayer;
		pPP->SetMode(CPreparationPlayer::PREPARATIONPLAYER_TRANSITION_GAME_COOPERATION);
		pBase = pPP;

		if (pBase != NULL)
		{
			CManager::SetMode(pBase);
		}
		break;
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CModeSelect::Draw(void)
{

}