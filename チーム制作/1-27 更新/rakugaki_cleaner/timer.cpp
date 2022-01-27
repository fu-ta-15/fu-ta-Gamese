//=============================================================================
//
// �^�C�}�[���� [timer.cpp]
// Author : itsuki takaiwa
//
//=============================================================================
#include "timer.h"
#include "number.h"
#include "manager.h"
#include "renderer.h"
#include "base.h"
#include "game.h"
#include "countdown.h"
#include "sound.h"

//=============================================================================
// CTimer�̃R���X�g���N�^
//=============================================================================
CTimer::CTimer(int nPriority) : CScene(nPriority)
{
	// �ϐ��̏�����
	for (int nCntNumber = 0; nCntNumber < MAX_DIGIT; nCntNumber++)
	{
		m_apNumber[nCntNumber] = NULL;
	}
	m_nCntInterval = 0;
	m_nTimer = 0;
	m_fSpace = 0.0f;
}

//=============================================================================
// CTimer�̃f�X�g���N�^
//=============================================================================
CTimer::~CTimer()
{

}

//=============================================================================
// ��������
//=============================================================================
CTimer *CTimer::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, int nDigit, float fSpace,int nTimer,int nTex)
{
	// �|�C���^�ϐ��̐���
	CTimer *pTimer;
	pTimer = new CTimer(CScene::PRIORITY_UI);

	// NULL�`�F�b�N
	if (pTimer != NULL)
	{
		// �����̐ݒ�
		pTimer->SetDigit(nDigit);

		// �e�N�X�`���ԍ��̐ݒ�
		pTimer->SetTexNum(nTex);

		// �^�C�}�[�z�u�Ԋu��ۑ�
		pTimer->SetSpace(fSpace);

		// �^�C�}�[�̐ݒ�
		pTimer->SetTimer(nTimer);

		// ����������
		pTimer->Init(pos, size);

		// �I�u�W�F�N�g�^�C�v�̐ݒ�
		pTimer->SetObjType(OBJTYPE_TIMER);
	}

	return pTimer;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CTimer::Init(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	// �i���o�[�̏���������
	for (int nCntNumber = 0; nCntNumber < m_nDigit; nCntNumber++)
	{
		m_apNumber[nCntNumber] = CNumber::Create(D3DXVECTOR3(pos.x + m_fSpace * nCntNumber, pos.y, 0.0f), size);

		// �e�N�X�`���ԍ��̐ݒ�
		m_apNumber[nCntNumber]->SetTexNum(m_nTex);
	}

	// �`��X�V
	UpdateDraw();

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CTimer::Uninit(void)
{
	for (int nCntNumber = 0; nCntNumber < m_nDigit; nCntNumber++)
	{
		// �i���o�[�̏I������
		m_apNumber[nCntNumber]->Uninit();
	}

	// �I�u�W�F�N�g�̔j��
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CTimer::Update(void)
{
	// �J�E���g�_�E�������擾
	CBase *pBase = CManager::GetBase();

	if (pBase != NULL)
	{
		CGame *pGame = (CGame*)pBase;

		CCountdown *pCountdown = pGame->GetCountdwn();

		if (pCountdown != NULL && pCountdown->GetFinish())
		{
			for (int nCntNumber = 0; nCntNumber < m_nDigit; nCntNumber++)
			{
				// �i���o�[�̍X�V����
				m_apNumber[nCntNumber]->Update();
			}

			// �J�E���g�̍X�V
			m_nCntInterval++;

			if (m_nTimer == 0 || pGame->GetGameMode() == CGame::GAMEMODE_FINISH)
			{
				// �_�ŏ���
				FlashTimer();

				// �Q�[�����[�h���I���ɂ���
				pGame->SetGameMode(CGame::GAMEMODE_FINISH);
			}
			else
			{
				// 60�ɂȂ�����^�C�}�[�����炷
				if (m_nCntInterval % 60 == 0)
				{
					m_nTimer--;
					m_nCntInterval = 0;

					if (m_nTimer == 0)
					{
						// �T�E���h�I��
						CSound::Stop();

						// ���ʉ�
						CSound::Play(CSound::SOUND_LABEL_SE_TIMEUP);
					}
				}

				// �`��X�V
				UpdateDraw();
			}
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CTimer::Draw(void)
{
	for (int nCntNumber = 0; nCntNumber < m_nDigit; nCntNumber++)
	{
		// �i���o�[�̕`�揈��
		m_apNumber[nCntNumber]->Draw();
	}
}

//=============================================================================
// �`��X�V����
//=============================================================================
void CTimer::UpdateDraw(void)
{
	// �^�C�}�[���i�[
	for (int nCntNumber = 0; nCntNumber < m_nDigit; nCntNumber++)
	{
		// ��������i�[
		int nNumber = (m_nTimer / (int)powf(10.0f, (float)(m_nDigit - (nCntNumber + 1))));

		// �i���o�[�Z�b�g
		m_apNumber[nCntNumber]->SetNumber(nNumber);
	}
}

//=============================================================================
// �^�C�}�[�̓_�ŏ���
//=============================================================================
void CTimer::FlashTimer(void)
{
	// �J�E���g�ɉ����ă��l��ς���
	for (int nCntNumber = 0; nCntNumber < m_nDigit; nCntNumber++)
	{
		if (m_nCntInterval % 20 == 10)
		{
			m_apNumber[nCntNumber]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
		}
		else if (m_nCntInterval % 20 == 0)
		{
			m_apNumber[nCntNumber]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
	}
}