//=============================================================================
//
// ���U���g��ʏ��� [result.cpp]
// Author : itsuki takaiwa
//
//=============================================================================
#include "result.h"
#include "manager.h"
#include "input_keyboard.h"
#include "fade.h"
#include "input_joypad.h"
#include "sound.h"
#include "stage.h"
#include "motion.h"
#include "camera.h"
#include "player_result.h"
#include "keep_object.h"
#include "player_marker.h"
#include "rank.h"
#include "texture.h"
#include "ranking_namespace.h"
#include "result_manager.h"
#include "score.h"
#include "number.h"
#include "texture.h"
#include "result_ui.h"
#include "set_result.h"

#include "title.h"
#include "game.h"
#include "mode_select.h"

#include "debugproc.h"

//*****************************************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************************************
CPlayer_Result **CResult::m_pPlayer = NULL;
CCamera *CResult::m_pCamera = NULL;
CResult::ResultMode CResult::m_resultMode = RESULTMODE_NORMAL;

//=============================================================================
// CGame�̃R���X�g���N�^
//=============================================================================
CResult::CResult()
{

}

//=============================================================================
// CManager�̃f�X�g���N�^
//=============================================================================
CResult::~CResult()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CResult::Init(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	// �����o�ϐ��̏�����
	m_resultMode = RESULTMODE_NORMAL;
	m_pResult_UI = NULL;

	// �e�N�X�`���Ǘ����̎擾
	CTexture *pTex = CManager::GetTexture();

	// �f�[�^�ۑ��p�I�u�W�F�N�g�̎擾
	CKeepObject *pKeepObject = CManager::GetKeepObject();

	// ���ʂ̊m��
	pKeepObject->Rank();

	// �J�����̐���
	m_pCamera = new CCamera;

	if (m_pCamera != NULL)
	{
		D3DVIEWPORT9 viewport;
		viewport.X = 0;
		viewport.Y = 0;
		viewport.Width = SCREEN_WIDTH;
		viewport.Height = SCREEN_HEIGHT;
		viewport.MinZ = 0;
		viewport.MaxZ = 1;
		m_pCamera= CCamera::Create(D3DXVECTOR3(0.0f, 35.0f, -120.0f), D3DXVECTOR3(0.0f, 30.0f, 80.0f), viewport);
		m_pCamera->SetParallel(D3DXVECTOR2(160.0f, 90.0f));
	}

	// �v���C���[�̐���
	int nNumPlayer = pKeepObject->GetNumPlayer();

	// �v���C�l�����ƂɃ��������m��
	m_pPlayer = new CPlayer_Result*[nNumPlayer];

	for (int nCnt = 0; nCnt < nNumPlayer; nCnt++)
	{
		// ���ʂ̎擾
		int nRank = pKeepObject->GetRank(nCnt) - 1;			// �����N
		int nNumRank = pKeepObject->GetNumRank(nCnt) - 1;	// �ʒu

		if (nNumPlayer >= 2)
		{
			// �v���C���[�̔z�u
			m_pPlayer[nCnt] = CPlayer_Result::Create(D3DXVECTOR3(-50.0f + (((50.0f * 2.0f) / (float)(nNumPlayer - 1)) * nNumRank), 0.0f, 0.0f), D3DXVECTOR2(20.0f, 100.0f), nCnt);
		}
		else
		{
			m_pPlayer[nCnt] = CPlayer_Result::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(20.0f, 100.0f), nCnt);
		}

		// �v���C���[�}�[�J�[�̔z�u
		m_pPlayer[nCnt]->SetPlayerMarker(CPlayerMarker::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(60.0f, 40.0f), D3DXVECTOR2(4, 1), pTex->SetTexture("data/TEXTURE/player_Logo00.png")), m_pCamera);
	}

	if (CManager::GetKeepObject()->GetGameMode() == CKeepObject::GAMEMODE_COOPERATION)
	{
		m_pResult_UI = new CResult_UI;

		if (m_pResult_UI != NULL)
		{
			m_pResult_UI->Init({ 0.0f,0.0f,0.0f }, { 0.0f,0.0f });

			for (int nCnt = 0; nCnt < nNumPlayer; nCnt++)
			{
				if (m_pResult_UI->GetAchivement())
				{
					m_pPlayer[nCnt]->SetMotion(CPlayer::MOTIONTYPE_VICTORY);
				}
				else
				{
					m_pPlayer[nCnt]->SetMotion(CPlayer::MOTIONTYPE_LOSE);
				}
			}
		}
	}
	else
	{
		for (int nCnt = 0; nCnt < nNumPlayer; nCnt++)
		{
			// ���ʂ̎擾
			int nRank = pKeepObject->GetRank(nCnt) - 1;			// �����N

			// ���ʂŃ��[�V������ύX
			if (nRank == 0)
			{
				m_pPlayer[nCnt]->SetMotion(CPlayer::MOTIONTYPE_VICTORY);
			}
			else
			{
				m_pPlayer[nCnt]->SetMotion(CPlayer::MOTIONTYPE_LOSE);
			}

			// �����L���O���S�̔z�u
			m_pPlayer[nCnt]->SetRank(CRank::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(90.0f, 130.0f), D3DXVECTOR2(4, 1), pTex->SetTexture("data/TEXTURE/Ranking00.png")), nRank, m_pCamera);
		}
	}
	// ���[�V�����f�[�^�̓ǂݍ���
	CMotion::LoadMotionData();

	// �^�C�g�����̓ǂݍ���
	CSetResult *pSet = new CSetResult;

	if (pSet != NULL)
	{
		pSet->Init();
	}

	// �X�e�[�W�̔z�u
	CStage::Create(D3DXVECTOR3(0.0f, -402.5f, 0.0f), D3DXVECTOR2(0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), "data/MODEL/table.x");

	// ���U���g�Ǘ��̐���
	m_pResultManager = new CResultManager;

	if (m_pResultManager != NULL)
	{
		m_pResultManager->Init({ 0.0f,0.0f,0.0f }, { 0.0f,0.0f });
	}

	// �T�E���h
	CSound::Play(CSound::SOUND_LABEL_RESULT);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CResult::Uninit(void)
{
	// �T�E���h
	CSound::Stop();

	// �J�����̏I������
	if (m_pCamera != NULL)
	{
		m_pCamera->Uninit();
		delete m_pCamera;
		m_pCamera = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void CResult::Update(void)
{
	CBase *pBase = NULL;

	switch (m_resultMode)
	{
		// �ʏ�
	case RESULTMODE_NORMAL:
		break;
		// �^�C�g���ɑJ��
	case RESULTMODE_RESTART:
		pBase = new CGame;

		if (pBase != NULL)
		{
			CManager::GetFade()->SetFade(CFade::FADE_OUT, pBase);
		}
		break;
		// �Q�[���ɑJ��
	case RESULTMODE_TRANSITION_MODESELECT:
		pBase = new CModeSelect;

		if (pBase != NULL)
		{
			CManager::GetFade()->SetFade(CFade::FADE_OUT, pBase);
		}
		break;
		// ���U���g�ɑJ��
	case RESULTMODE_TRANSITION_TITLE:
		pBase = new CTitle;

		if (pBase != NULL)
		{
			CManager::GetFade()->SetFade(CFade::FADE_OUT, pBase);
		}
		break;
	}
	m_pCamera->Update();
}

//=============================================================================
// �`�揈��
//=============================================================================
void CResult::Draw(void)
{	
	// �J�����̔z�u
	m_pCamera->SetCamera();
}