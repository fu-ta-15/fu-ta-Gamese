//=============================================================================
//
// �J�E���g�_�E������ [countdown.cpp]
// Author : itsuki takaiwa
//
//=============================================================================
#include "countdown.h"
#include "number.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "base.h"
#include "game.h"

#include "timer.h"
#include "sound.h"

//=============================================================================
// CCountdown�̃R���X�g���N�^
//=============================================================================
CCountdown::CCountdown(int nPriority) : CScene(nPriority)
{
	// �ϐ��̏�����
	m_apNumber = NULL;
	m_nCntInterval = 0;
	m_nTimer = 0;
	m_fSpace = 0.0f;
}

//=============================================================================
// CCountdown�̃f�X�g���N�^
//=============================================================================
CCountdown::~CCountdown()
{

}

//=============================================================================
// ��������
//=============================================================================
CCountdown *CCountdown::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	// �|�C���^�ϐ��̐���
	CCountdown *pCountdown;
	pCountdown = new CCountdown(CScene::PRIORITY_UI);

	// NULL�`�F�b�N
	if (pCountdown != NULL)
	{ 
		// ����������
		pCountdown->Init(pos, size);

		// �I�u�W�F�N�g�^�C�v�̐ݒ�
		pCountdown->SetObjType(OBJTYPE_TIMER);
	}

	return pCountdown;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CCountdown::Init(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	// �J�E���g�_�E�����̓ǂݍ���
	LoadCountdown();

	// �`��X�V
	UpdateDraw();

	m_bFinish = false;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CCountdown::Uninit(void)
{
	// �i���o�[�̏I������
	m_apNumber->Uninit();
	m_pStartLogo->Uninit();
	m_pBg->Uninit();

	// �I�u�W�F�N�g�̔j��
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CCountdown::Update(void)
{
	if (m_bFinish != true)
	{
		// �i���o�[�̍X�V����
		m_apNumber->Update();

		// 60�ɂȂ�����^�C�}�[�����炷
		if (m_nCntInterval % 60 == 0)
		{
			m_nTimer--;
			m_nCntInterval = 0;


			if (m_nTimer < 0)
			{
				m_bFinish = true;

				// �T�E���h
				CSound::Play(CSound::SOUND_LABEL_GAME);
			}
			else if(m_nTimer == 0)
			{
				// ���ʉ�
				CSound::Play(CSound::SOUND_LABEL_SE_GAME_START);
			}
			else
			{
				// ���ʉ�
				CSound::Play(CSound::SOUND_LABEL_SE_COUNTDOWN);
			}
		}

		// �J�E���g�̍X�V
		m_nCntInterval++;

		// �`��X�V
		UpdateDraw();
	}

	if (m_bTimerFinish != true)
	{
		// �Q�[�������擾
		CBase *pBase = CManager::GetBase();

		if (pBase != NULL)
		{
			// �Q�[�������擾
			CGame *pGame = (CGame*)pBase;

			if(pGame->GetGameMode() == CGame::GAMEMODE_FINISH)
			{
				// �I��������Ƃ�`����
				m_bTimerFinish = true;
			}
		}
	}
	else
	{
		// �J�E���g�̍X�V
		m_nCntInterval++;

		// 60�ɂȂ�����J�ڂ���
		if (m_nCntInterval % 60 == 0)
		{
			// �Q�[�������擾
			CBase *pBase = CManager::GetBase();

			if (pBase != NULL)
			{
				CGame *pGame = (CGame*)pBase;

				// �Q�[�����[�h�����U���g�J�ڂɕύX
				pGame->SetGameMode(CGame::GAMEMODE_TRANSITION_RESULT);
			}
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CCountdown::Draw(void)
{
	if (m_bFinish != true)
	{
		m_pBg->Draw();

		if (m_nTimer == 0)
		{
			m_pStartLogo->Draw();
		}
		else
		{
			// �i���o�[�̕`�揈��
			m_apNumber->Draw();
		}
	}
	if (m_bTimerFinish == true)
	{
		m_pBg->Draw();
		m_pFinishLogo->Draw();
	}
}

//=============================================================================
// �`��X�V����
//=============================================================================
void CCountdown::UpdateDraw(void)
{
	// �^�C�}�[���i�[
	for (int nCntNumber = 0; nCntNumber < m_nDigit; nCntNumber++)
	{
		if (m_apNumber != NULL)
		{
			// ��������i�[
			int nNumber = (m_nTimer / (int)powf(10.0f, (float)(m_nDigit - (nCntNumber + 1))));

			// �i���o�[�Z�b�g
			m_apNumber->SetNumber(nNumber);
		}
	}
}

//-----------------------------------------------------------------
// �J�E���g�_�E�����̓ǂݍ���
//-----------------------------------------------------------------
bool CCountdown::LoadCountdown(void)
{
	// FILE�^�̃|�C���^�֐�
	FILE *pFile;

	// �t�@�C�����J��
	pFile = fopen("data/TEXT/04_game/countdown.txt", "r");

	// NULL�`�F�b�N
	if (pFile != NULL)
	{
		// �ϐ��錾
		char aLine[128];
		char aText[128];
		bool bScript = false;
		int nCntTex = 0;
		int nNumTex = 0;
		char **pTextureFileName;

		// SCRIPT��T��
		while (fgets(aLine, 128, pFile) != NULL)
		{
			// ���s�󔒂�������܂œǂݍ���
			fscanf(pFile, "%s", &aText[0]);

			// SCRIPT���������炻��ȍ~��ǂݍ��ދ���^����
			if (strcmp(&aText[0], "SCRIPT") == 0)
			{
				bScript = true;
			}
			if (bScript == true)
			{
				if (strcmp(&aText[0], "NUM_TEXTURE") == 0)
				{
					fscanf(pFile, "%s %d", &aText[0], &nNumTex);

					// ���f���t�@�C���l�[���i�[�p�ϐ��̗̈�𓮓I�Ɋm�ۂ���
					pTextureFileName = new char*[nNumTex];

					for (int nCnt = 0; nCnt < nNumTex; nCnt++)
					{
						pTextureFileName[nCnt] = new char[128];
					}
				}
				// TEXTURE_FILENAME����������e�N�X�`���t�@�C�������i�[
				if (strcmp(&aText[0], "TEXTURE_FILENAME") == 0 && nCntTex < nNumTex)
				{
					fscanf(pFile, "%s %s", &aText[0], &pTextureFileName[nCntTex][0]);

					nCntTex++;
				}
				// TIMERSET�̂Ƃ��^�C�}�[�����i�[���鋖��^����
				if (strcmp(&aText[0], "TIMERSET") == 0)
				{
					LoadTimer(pFile, nNumTex, pTextureFileName);
				}
				// STARTLOGOSET�̂Ƃ��J�n���S�����i�[���鋖��^����
				if (strcmp(&aText[0], "STARTLOGOSET") == 0)
				{
					LoadStartLogo(pFile, nNumTex, pTextureFileName);
				}
				// FINISHLOGOSET�̂Ƃ��I�����S�����i�[���鋖��^����
				if (strcmp(&aText[0], "FINISHLOGOSET") == 0)
				{
					LoadFinishLogo(pFile, nNumTex, pTextureFileName);
				}
				// BGSET�̂Ƃ��w�i�����i�[���鋖��^����
				if (strcmp(&aText[0], "BGSET") == 0)
				{
					LoadBg(pFile, nNumTex, pTextureFileName);
				}
				// END_SCRIPT����������ǂݍ��݂��I����
				if (strcmp(&aText[0], "END_SCRIPT") == 0)
				{
					// �������̊J��
					delete[] pTextureFileName;
					pTextureFileName = NULL;

					break;
				}
			}
		}
		// �t�@�C�������
		fclose(pFile);
	}
	else
	{
		return false;
	}
	return true;
}

//-----------------------------------------------------------------
// �^�C�}�[���̓ǂݍ���
//-----------------------------------------------------------------
bool CCountdown::LoadTimer(FILE *pFile, int nNumTex, char **pTextureFileName)
{
	// �ϐ��錾
	char aLine[128];
	char aText[128];
	int nTex;
	D3DXVECTOR3 pos = { 0.0f,0.0f,0.0f };
	D3DXVECTOR2 size = { 0.0f,0.0f };

	while (fgets(aLine, 128, pFile) != NULL)
	{
		// ���s�󔒂�������܂œǂݍ���
		fscanf(pFile, "%s", &aText[0]);

		// POS������������W�����i�[
		if (strcmp(&aText[0], "TYPE") == 0)
		{
			fscanf(pFile, "%s %d", &aText[0], &nTex);
		}
		// POS������������W�����i�[
		if (strcmp(&aText[0], "POS") == 0)
		{
			fscanf(pFile, "%s %f %f %f", &aText[0],
				&pos.x,
				&pos.y,
				&pos.z);
		}
		// SIZE����������T�C�Y�����i�[
		if (strcmp(&aText[0], "SIZE") == 0)
		{
			fscanf(pFile, "%s %f %f", &aText[0],
				&size.x,
				&size.y);
		}
		// DIGIT���������猅�������i�[
		if (strcmp(&aText[0], "DIGIT") == 0)
		{
			fscanf(pFile, "%s %d", &aText[0], &m_nDigit);
		}
		// SPACE����������Ԋu�����i�[
		if (strcmp(&aText[0], "SPACE") == 0)
		{
			fscanf(pFile, "%s %f", &aText[0], &m_fSpace);
		}
		// TIME���������琧�����ԏ����i�[
		if (strcmp(&aText[0], "TIME") == 0)
		{
			fscanf(pFile, "%s %d", &aText[0], &m_nTimer);
		}
		// END_TIMERSET����������^�C�}�[�����i�[���I����
		if (strcmp(&aText[0], "END_TIMERSET") == 0)
		{
			if (nTex < nNumTex)
			{
				// �e�N�X�`�����̎擾
				CTexture *pTex = CManager::GetTexture();

				if (pTex != NULL)
				{
					// �e�N�X�`���̐���
					m_apNumber = CNumber::Create(pos, size);
					m_apNumber->SetTexNum(pTex->SetTexture(pTextureFileName[nTex]));

					m_nTimer++;
				}
			}
			return true;
		}
	}
	return false;
}

//=============================================================================
// �J�n���S���̃��[�h
//=============================================================================
bool CCountdown::LoadStartLogo(FILE *pFile, int nNumTex, char **pTextureFileName)
{
	// �ϐ��錾
	char aLine[128];
	char aText[128];
	int nTex;
	D3DXVECTOR3 pos = { 0.0f,0.0f,0.0f };
	D3DXVECTOR2 size = { 0.0f,0.0f };
	D3DXCOLOR col = { 1.0f,1.0f,1.0f,1.0f };

	while (fgets(aLine, 128, pFile) != NULL)
	{
		// ���s�󔒂�������܂œǂݍ���
		fscanf(pFile, "%s", &aText[0]);

		// POS������������W�����i�[
		if (strcmp(&aText[0], "TYPE") == 0)
		{
			fscanf(pFile, "%s %d", &aText[0], &nTex);
		}
		// POS������������W�����i�[
		if (strcmp(&aText[0], "POS") == 0)
		{
			fscanf(pFile, "%s %f %f %f", &aText[0],
				&pos.x,
				&pos.y,
				&pos.z);
		}
		// SIZE����������T�C�Y�����i�[
		if (strcmp(&aText[0], "SIZE") == 0)
		{
			fscanf(pFile, "%s %f %f", &aText[0],
				&size.x,
				&size.y);
		}
		// POS������������W�����i�[
		if (strcmp(&aText[0], "COLOR") == 0)
		{
			fscanf(pFile, "%s %f %f %f %f", &aText[0],
				&col.r,
				&col.g,
				&col.b,
				&col.a);
		}
		// END_PARTSSET����������e�p�[�c�̏����i�[���I����
		if (strcmp(&aText[0], "END_STARTLOGOSET") == 0)
		{
			if (nTex < nNumTex)
			{
				// �e�N�X�`�����̎擾
				CTexture *pTex = CManager::GetTexture();

				if (pTex != NULL)
				{
					// ���S�̐���
					m_pStartLogo = CScene2D::Create(pos, D3DXVECTOR3(size.x, size.y, 0.0f), CScene::PRIORITY_NONE, pTex->SetTexture(&pTextureFileName[nTex][0]));
					m_pStartLogo->SetCol(col);

					break;
				}
			}
		}
	}
	return false;
}

//=============================================================================
// �I�����S���̃��[�h
//=============================================================================
bool CCountdown::LoadFinishLogo(FILE *pFile, int nNumTex, char **pTextureFileName)
{
	// �ϐ��錾
	char aLine[128];
	char aText[128];
	int nTex;
	D3DXVECTOR3 pos = { 0.0f,0.0f,0.0f };
	D3DXVECTOR2 size = { 0.0f,0.0f };
	D3DXCOLOR col = { 1.0f,1.0f,1.0f,1.0f };

	while (fgets(aLine, 128, pFile) != NULL)
	{
		// ���s�󔒂�������܂œǂݍ���
		fscanf(pFile, "%s", &aText[0]);

		// POS������������W�����i�[
		if (strcmp(&aText[0], "TYPE") == 0)
		{
			fscanf(pFile, "%s %d", &aText[0], &nTex);
		}
		// POS������������W�����i�[
		if (strcmp(&aText[0], "POS") == 0)
		{
			fscanf(pFile, "%s %f %f %f", &aText[0],
				&pos.x,
				&pos.y,
				&pos.z);
		}
		// SIZE����������T�C�Y�����i�[
		if (strcmp(&aText[0], "SIZE") == 0)
		{
			fscanf(pFile, "%s %f %f", &aText[0],
				&size.x,
				&size.y);
		}
		// POS������������W�����i�[
		if (strcmp(&aText[0], "COLOR") == 0)
		{
			fscanf(pFile, "%s %f %f %f %f", &aText[0],
				&col.r,
				&col.g,
				&col.b,
				&col.a);
		}
		// END_PARTSSET����������e�p�[�c�̏����i�[���I����
		if (strcmp(&aText[0], "END_FINISHLOGOSET") == 0)
		{
			if (nTex < nNumTex)
			{
				// �e�N�X�`�����̎擾
				CTexture *pTex = CManager::GetTexture();

				if (pTex != NULL)
				{
					// ���S�̐���
					m_pFinishLogo = CScene2D::Create(pos, D3DXVECTOR3(size.x, size.y, 0.0f), CScene::PRIORITY_NONE, pTex->SetTexture(&pTextureFileName[nTex][0]));
					m_pFinishLogo->SetCol(col);

					break;
				}
			}
		}
	}
	return false;
}

//=============================================================================
// �w�i���̃��[�h
//=============================================================================
bool CCountdown::LoadBg(FILE *pFile, int nNumTex, char **pTextureFileName)
{
	// �ϐ��錾
	char aLine[128];
	char aText[128];
	int nTex;
	D3DXVECTOR3 pos = { 0.0f,0.0f,0.0f };
	D3DXVECTOR2 size = { 0.0f,0.0f };
	D3DXCOLOR col = { 1.0f,1.0f,1.0f,1.0f };

	while (fgets(aLine, 128, pFile) != NULL)
	{
		// ���s�󔒂�������܂œǂݍ���
		fscanf(pFile, "%s", &aText[0]);

		// POS������������W�����i�[
		if (strcmp(&aText[0], "TYPE") == 0)
		{
			fscanf(pFile, "%s %d", &aText[0], &nTex);
		}
		// POS������������W�����i�[
		if (strcmp(&aText[0], "POS") == 0)
		{
			fscanf(pFile, "%s %f %f %f", &aText[0],
				&pos.x,
				&pos.y,
				&pos.z);
		}
		// SIZE����������T�C�Y�����i�[
		if (strcmp(&aText[0], "SIZE") == 0)
		{
			fscanf(pFile, "%s %f %f", &aText[0],
				&size.x,
				&size.y);
		}
		// POS������������W�����i�[
		if (strcmp(&aText[0], "COLOR") == 0)
		{
			fscanf(pFile, "%s %f %f %f %f", &aText[0],
				&col.r,
				&col.g,
				&col.b,
				&col.a);
		}
		// END_BGSET����������w�i�̏����i�[���I����
		if (strcmp(&aText[0], "END_BGSET") == 0)
		{
			if (nTex < nNumTex)
			{
				// �e�N�X�`�����̎擾
				CTexture *pTex = CManager::GetTexture();

				if (pTex != NULL)
				{
					if (nTex <= -1)
					{
						// �w�i�̐���
						m_pBg = CScene2D::Create(pos, D3DXVECTOR3(size.x,size.y,0.0f), CScene::PRIORITY_NONE, -1);
						m_pBg->SetCol(col);
					}
					else
					{
						// �w�i�̐���
						m_pBg = CScene2D::Create(pos, D3DXVECTOR3(size.x, size.y, 0.0f), CScene::PRIORITY_NONE, pTex->SetTexture(pTextureFileName[nTex]));
						m_pBg->SetCol(col);
					}

					break;
				}
			}
		}
	}
	return false;
}