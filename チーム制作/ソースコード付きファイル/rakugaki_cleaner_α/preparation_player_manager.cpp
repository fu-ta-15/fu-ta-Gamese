//=============================================================================
//
// �v���C���[�ҋ@��ʊǗ����� [preparation_player_manager.cpp]
// Author : itsuki takaiwa
//
//=============================================================================
#include "preparation_player_manager.h"

#include "manager.h"
#include "input_keyboard.h"
#include "input_joypad.h"
#include "base.h"
#include "preparation_player.h"
#include "set_preparation_player.h"
#include "press_logo.h"
#include "Scene2D.h"
#include "keep_object.h"
#include "texture.h"
#include "debugproc.h"
#include "sound.h"

//=============================================================================
// CPreparationPlayerManager�̃R���X�g���N�^
//=============================================================================
CPreparationPlayerManager::CPreparationPlayerManager()
{
	// �����o�ϐ��̏�����
	m_pPressLogo = NULL;
	m_bPlayerReadyOK = false;
}

//=============================================================================
// CPreparationPlayerManager�̃f�X�g���N�^
//=============================================================================
CPreparationPlayerManager::~CPreparationPlayerManager()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CPreparationPlayerManager::Init(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	// �����o�ϐ��̏�����
	m_pPressLogo = NULL;
	m_bPlayerReadyOK = false;
	m_bOK = false;

	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		m_logoInfo[nCnt].bJoin = false;
		m_logoInfo[nCnt].bReady = false;

		m_pJoinLogo[nCnt] = NULL;
		m_pCompLogo[nCnt] = NULL;
		m_pOkLogo[nCnt] = NULL;
		m_pFrame[nCnt] = NULL;
	}

	// �v���C���[������ʏ��̓ǂݍ���
	CSetPreparationPlayer *pSet = new CSetPreparationPlayer;

	if (pSet != NULL)
	{
		pSet->Init();
	}

	// �L�����I������������
	InitCharaSelect();

	return S_OK;  
}

//=============================================================================
// �I������
//=============================================================================
void CPreparationPlayerManager::Uninit(void)
{
	// �폜
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CPreparationPlayerManager::Update(void)
{
	// �L�����I��
	CharaSelect();

	// �J�ڏ���
	Transition();
}

//=============================================================================
// �`�揈��
//=============================================================================
void CPreparationPlayerManager::Draw(void)
{
	// �eUI�̕`�揈��
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		if (m_logoInfo[nCnt].bJoin == false)
		{
			if (m_pJoinLogo[nCnt] != NULL)
			{
				m_pJoinLogo[nCnt]->Draw();
			}
		}
		else if (m_logoInfo[nCnt].bJoin == true && m_logoInfo[nCnt].bReady == false)
		{
			if (m_pCompLogo[nCnt] != NULL)
			{
				m_pCompLogo[nCnt]->Draw();
			}
		}
		else if (m_logoInfo[nCnt].bJoin == true && m_logoInfo[nCnt].bReady == true)
		{
			if (m_pOkLogo[nCnt] != NULL)
			{
				m_pOkLogo[nCnt]->Draw();
			}
		}
	}
}

//=============================================================================
// �J�ڏ���
//=============================================================================
void CPreparationPlayerManager::Transition(void)
{
	// �L�[�{�[�h���̎擾
	CInputKeyboard *pKey = GET_KEYBOARD;

	// �Q�[���p�b�h���̎擾
	CInputJoypad *pJoypad = CManager::GetInputJoypad();

	if (m_bPlayerReadyOK == false)
	{
		// �S���������������Ă��Ȃ��Ƃ�����
		for (int nCnt = 0; nCnt < 4; nCnt++)
		{
			if (m_logoInfo[nCnt].bJoin == true)
			{
				// �Q�����Ă��邩�ǂ���
				if (m_logoInfo[nCnt].bReady != true)
				{
					// �����������Ă��Ȃ��Ȃ烍�S��\�������Ȃ�
					m_bPlayerReadyOK = false;

					m_pPressLogo->SetStartCnt(m_bPlayerReadyOK);

					break;
				}
				else
				{
					// �����������Ă����烍�S���o��������
					m_bPlayerReadyOK = true;

					m_pPressLogo->SetStartCnt(m_bPlayerReadyOK);
				}
			}
		}
	}
	else if (m_bPlayerReadyOK == true && m_bOK == false)
	{
		for (int nCnt = 0; nCnt < 4; nCnt++)
		{
			if (m_logoInfo[nCnt].bJoin == true)
			{
				// �Q�����Ă���Ƃ�����
				if (m_logoInfo[nCnt].bReady != true)
				{
					// �����������Ă��Ȃ��Ȃ烍�S��\�������Ȃ�
					m_bPlayerReadyOK = false;

					m_pPressLogo->SetStartCnt(m_bPlayerReadyOK);

					return;
				}
			}
		}
		// �{�^������������true�ɂ���
		if (pKey->GetTrigger(KEY_OK) || pJoypad->GetButtonTrigger(0, JOYPAD_PAUSE))
		{
			m_pPressLogo->PressButton();

			m_bOK = true;
		}
	}

	// �J�ڂł���Ƃ�����
	if (m_pPressLogo->GetTransition())
	{
		m_bTransition = true;

		// �ۑ��p�I�u�W�F�N�g�ɐl����ۑ�
		int nNumPlayer = 0;

		for (int nCnt = 0; nCnt < 4; nCnt++)
		{
			if (m_logoInfo[nCnt].bReady == true)
			{
				nNumPlayer++;
			}
		}
		CKeepObject *pKeepObject = CManager::GetKeepObject();
		pKeepObject->SetNumPlayer(nNumPlayer);
	}

	if (pJoypad->GetButtonTrigger(0, JOYPAD_CANCEL) && m_logoInfo[0].bJoin == false)
	{
		m_pPressLogo->SetStartCnt(false);

		// ���ʉ�
		CSound::Play(CSound::SOUND_LABEL_SE_CANCEL);

		CBase *pBase = CManager::GetBase();

		if (pBase != NULL)
		{
			CPreparationPlayer *pPP = (CPreparationPlayer*)pBase;

			pPP->SetMode(CPreparationPlayer::PREPARATIONPLAYER_TRANSITION_MODESELECT);
			m_bTransition = true;
		}
	}
}

//=============================================================================
// �L�����I������������
//=============================================================================
void CPreparationPlayerManager::InitCharaSelect(void)
{
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		if (m_pFrame[nCnt] != NULL)
		{
			// �e�N�X�`���Ǘ����̎擾
			CTexture *pTex = CManager::GetTexture();

			// �t���[���̈ʒu���擾
			D3DXVECTOR3 posFrame = m_pFrame[nCnt]->GetPosition();

			// �eUI�̏�����
			m_pJoinLogo[nCnt] = CScene2D::Create(D3DXVECTOR3(posFrame.x, posFrame.y, 0.0f),	// �Q��
				D3DXVECTOR3(160.0f, 60.0f, 0.0f),
				PRIORITY_NONE,
				pTex->SetTexture("data/TEXTURE/Entry00.png"));
			m_pCompLogo[nCnt] = CScene2D::Create(D3DXVECTOR3(posFrame.x, posFrame.y + 130.0f, 0.0f),	// ��������
				D3DXVECTOR3(130.0f, 30.0f, 0.0f),
				PRIORITY_NONE,
				pTex->SetTexture("data/TEXTURE/ENTER06.png"));
			m_pOkLogo[nCnt] = CScene2D::Create(D3DXVECTOR3(posFrame.x, posFrame.y + 130.0f, 0.0f),	// OK
				D3DXVECTOR3(80.0f, 30.0f, 0.0f),
				PRIORITY_NONE,
				pTex->SetTexture("data/TEXTURE/return00.png"));
		}
	}
}

//=============================================================================
// �L�����I������������
//=============================================================================
void CPreparationPlayerManager::CharaSelect(void)
{
	// ����v���C���[���擾
	int nCntPlayerOP = CPreparationPlayer::GetnCntPlayer();

	// �L�[�{�[�h���̎擾
	CInputKeyboard *pKey = GET_KEYBOARD;

	// �Q�[���p�b�h���̎擾
	CInputJoypad *pJoypad = CManager::GetInputJoypad();

	if (pKey->GetTrigger(KEY_OK))
	{
		if (m_logoInfo[nCntPlayerOP].bJoin != true)
		{
			m_logoInfo[nCntPlayerOP].bJoin = true;
		}
		else if (m_logoInfo[nCntPlayerOP].bReady != true)
		{
			m_logoInfo[nCntPlayerOP].bReady = true;
		}
	}
	if (pKey->GetTrigger(KEY_SKILL))
	{
		if (m_logoInfo[nCntPlayerOP].bReady == true)
		{
			m_logoInfo[nCntPlayerOP].bReady = false;
		}
		else if (m_logoInfo[nCntPlayerOP].bJoin == true)
		{
			m_logoInfo[nCntPlayerOP].bJoin = false;
		}	
	}

	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		if (pJoypad->GetButtonTrigger(nCnt, JOYPAD_DECISION))
		{
			if (m_logoInfo[nCnt].bJoin != true)
			{
				m_logoInfo[nCnt].bJoin = true;
			}
			else if (m_logoInfo[nCnt].bReady != true)
			{
				m_logoInfo[nCnt].bReady = true;
			}
		}
		else if (pJoypad->GetButtonTrigger(nCnt, JOYPAD_CANCEL))
		{
			if (m_logoInfo[nCnt].bReady == true)
			{
				m_logoInfo[nCnt].bReady = false;
			}
			else if (m_logoInfo[nCnt].bJoin == true)
			{
				m_logoInfo[nCnt].bJoin = false;
			}
		}
	}
}