//-----------------------------------------------------------------
//
// �`���[�g���A����� (tutorial.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "tutorial.h"
#include "fade.h"
#include "input_keyboard.h"
#include "sound.h"
#include "manager.h"
#include "renderer.h"
#include "Scene2D.h"
#include "input_joypad.h"
#include "texture.h"

//=============================================================================
// CTutorial�̃R���X�g���N�^
//=============================================================================
CTutorial::CTutorial(int nPriority) :CScene(nPriority)
{

}

//=============================================================================
// CTutorial�̃f�X�g���N�^
//=============================================================================
CTutorial::~CTutorial()
{

}

//=============================================================================
// ��������
//=============================================================================
CTutorial *CTutorial::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	// �|�C���^�ϐ��̐���
	CTutorial *pTutorial;
	pTutorial = new CTutorial(CScene::PRIORITY_PAUSE);

	// NULL�`�F�b�N
	if (pTutorial != NULL)
	{
		// ����������
		pTutorial->Init(pos, size);

		// �I�u�W�F�N�g�^�C�v�̐ݒ�
		pTutorial->SetObjType(OBJTYPE_TUTORIAL);
	}

	return pTutorial;
}

//=============================================================================
// �`���[�g���A���̏���������
//=============================================================================
HRESULT CTutorial::Init(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	// �����o�ϐ��̏�����
	m_nCntTutorial = 0;
	m_nCntLogo = 0;

	// �`���[�g���A�����̓ǂݍ���
	LoadData();

	// �`���[�g���A����ʂ̌��𔖂�����
	m_pBg = new CScene2D(CScene::PRIORITY_NONE);
	m_pBg->Init(D3DXVECTOR3(640.0f, 360.0f, 0.0f), D3DXVECTOR2(1280.0f, 720.0f));
	m_pBg->SetCol(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.3f));

	CManager::SetPause(true);
	m_nCursor = 0;

	return S_OK;
}

//=============================================================================
// �`���[�g���A���̏I������
//=============================================================================
void CTutorial::Uninit(void)
{
	// �������̊J��
	delete m_pTutorial;
	m_pTutorial = NULL;

	delete m_pLogo;
	m_pLogo = NULL;

	// ���̏I������
	if (m_pLeftArrow != NULL)
	{
		m_pLeftArrow->Uninit();
	}
	if (m_pRightArrow != NULL)
	{
		m_pRightArrow->Uninit();
	}

	// �I�u�W�F�N�g�j��
	Release();
}

//=============================================================================
// �`���[�g���A���̍X�V����
//=============================================================================
void CTutorial::Update(void)
{
	// �L�[�{�[�h���̎擾
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();

	// �Q�[���p�b�h���̎擾
	CInputJoypad *pJoypad = CManager::GetInputJoypad();
	bool *bCrossTrigger = pJoypad->GetCrossKeyTrigger(0);

	// �t�F�[�h���̎擾
	CFade *pFade = CManager::GetFade();

	if (pKeyboard->GetTrigger(CInputKeyboard::KEYINFO_LEFT) || bCrossTrigger[CInputJoypad::STICK_AND_CROSSKEY_LEFT])
	{
		// ��ɂ����Ƃ��ԍ������炷
		m_nCursor--;

		if (m_nCursor < 0)
		{
			// �R���e�B�j���[��菬�����Ȃ����Ƃ�
			m_nCursor = 0;
		}
		else
		{
			// ���ʉ�
			CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_TURN_PAGE);
		}
	}
	else if (pKeyboard->GetTrigger(CInputKeyboard::KEYINFO_RIGHT) || bCrossTrigger[CInputJoypad::STICK_AND_CROSSKEY_RIGHT])
	{
		// ���ɂ����Ƃ��ԍ��𑝂₷
		m_nCursor++;

		if (m_nCursor > m_nNumTutorial - 1)
		{
			// �N�C�b�g���傫���Ȃ����Ƃ�
			m_nCursor = m_nNumTutorial - 1;
		}
		else
		{
			// ���ʉ�
			CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_TURN_PAGE);
		}
	}

	// �J�[�\�����Ōォ�G���^�[�L�[���������Ƃ��`���[�g���A����ʂ����
	if (pKeyboard->GetTrigger(CInputKeyboard::KEYINFO_OK) || pJoypad->GetButtonTrigger(0, JOYPAD_CANCEL))
	{
		CManager::SetPause(false);

		// �I������
		Uninit();

		return;
	}
}

//=============================================================================
// �`���[�g���A���̕`�揈��
//=============================================================================
void CTutorial::Draw(void)
{
	// �`���[�g���A���̕`�揈��
	if (m_pBg != NULL)
	{
		m_pBg->Draw();
	}
	if (m_pTutorial[m_nCursor] != NULL)
	{
		m_pTutorial[m_nCursor]->Draw();
	}
	for (int nCnt = 0; nCnt < m_nNumLogo; nCnt++)
	{
		if (m_pLogo[nCnt] != NULL)
		{
			m_pLogo[nCnt]->Draw();
		}
	}

	// ���̕`�揈��
	if (m_nCursor == 0)
	{
		if (m_pRightArrow != NULL)
		{
			m_pRightArrow->Draw();
		}
	}
	else if (m_nCursor == m_nNumTutorial - 1)
	{
		if (m_pLeftArrow != NULL)
		{
			m_pLeftArrow->Draw();
		}
	}
	else
	{
		if (m_pLeftArrow != NULL && m_pRightArrow != NULL)
		{
			m_pRightArrow->Draw();
			m_pLeftArrow->Draw();
		}
	}
}

//=============================================================================
// �|�[�Y�f�[�^�̃��[�h
//=============================================================================
bool CTutorial::LoadData(void)
{
	// FILE�^�̃|�C���^�֐�
	FILE *pFile;

	// �t�@�C�����J��
	pFile = fopen("data/TEXT/03_tutorial/tutorial.txt", "r");

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
				// LOGOSET�̂Ƃ��X�L���Q�[�W�����i�[���鋖��^����
				if (strcmp(&aText[0], "LOGOSET") == 0)
				{
					LoadLogo(pFile, nNumTex, pTextureFileName);
				}
				// LEFTARROWSET�̂Ƃ��X�L���Q�[�W�����i�[���鋖��^����
				if (strcmp(&aText[0], "LEFTARROWSET") == 0)
				{
					LoadLeftArrow(pFile, nNumTex, pTextureFileName);
				}
				// RIGHTARROWSET�̂Ƃ��X�L���Q�[�W�����i�[���鋖��^����
				if (strcmp(&aText[0], "RIGHTARROWSET") == 0)
				{
					LoadRightArrow(pFile, nNumTex, pTextureFileName);
				}
				// PAUSEMENUSET�̂Ƃ��X�L���Q�[�W�����i�[���鋖��^����
				if (strcmp(&aText[0], "TUTORIALSET") == 0)
				{
					LoadTutorialSheet(pFile, nNumTex, pTextureFileName);
				}
				// END_SCRIPT����������ǂݍ��݂��I����
				if (strcmp(&aText[0], "END_SCRIPT") == 0)
				{
					// �������̊J��
					delete[] pTextureFileName;

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

//=============================================================================
// ���S�̃��[�h
//=============================================================================
bool CTutorial::LoadLogo(FILE *pFile, int nNumTex, char **pTextureFileName)
{
	// �ϐ��錾
	char aLine[128];
	char aText[128];
	D3DXVECTOR3 pos = { 0.0f,0.0f,0.0f };
	D3DXVECTOR2 size = { 0.0f,0.0f };

	while (fgets(aLine, 128, pFile) != NULL)
	{
		// ���s�󔒂�������܂œǂݍ���
		fscanf(pFile, "%s", &aText[0]);

		// POS������������W�����i�[
		if (strcmp(&aText[0], "NUM_LOGO") == 0)
		{
			fscanf(pFile, "%s %d", &aText[0], &m_nNumLogo);

			// �������̊m��
			m_pLogo = new CScene2D*[m_nNumLogo];
		}
		// LOGO�̂Ƃ��X�L���Q�[�W�����i�[���鋖��^����
		if (strcmp(&aText[0], "LOGO") == 0)
		{
			Logo(pFile, nNumTex, pTextureFileName);
		}
		// END_PARTSSET����������e�p�[�c�̏����i�[���I����
		if (strcmp(&aText[0], "END_LOGOSET") == 0)
		{
			return true;
		}
	}
	return false;
}

//=============================================================================
// ���S���̃��[�h
//=============================================================================
bool CTutorial::Logo(FILE *pFile, int nNumTex, char **pTextureFileName)
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
		// END_LOGOSET����������e�p�[�c�̏����i�[���I����
		if (strcmp(&aText[0], "END_LOGO") == 0)
		{
			if (nTex < nNumTex && m_nCntLogo < m_nNumLogo)
			{
				// �e�N�X�`�����̎擾
				CTexture *pTex = CManager::GetTexture();

				if (pTex != NULL)
				{
					m_pLogo[m_nCntLogo] = CScene2D::Create(pos, D3DXVECTOR3(size.x, size.y, 0.0f), CScene::PRIORITY_NONE, pTex->SetTexture(pTextureFileName[nTex]));
					m_nCntLogo++;

					return true;
				}
				else
				{
					m_pLogo[m_nCntLogo] = CScene2D::Create(pos, D3DXVECTOR3(size.x, size.y, 0.0f), CScene::PRIORITY_NONE, -1);
					m_nCntLogo++;

					return true;
				}
			}
		}
	}
	return false;
}

//=============================================================================
// �������̃��[�h
//=============================================================================
bool CTutorial::LoadLeftArrow(FILE *pFile, int nNumTex, char **pTextureFileName)
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
		// END_LEFTARROWSET����������e�p�[�c�̏����i�[���I����
		if (strcmp(&aText[0], "END_LEFTARROWSET") == 0)
		{
			if (nTex < nNumTex)
			{
				// �e�N�X�`�����̎擾
				CTexture *pTex = CManager::GetTexture();

				if (pTex != NULL)
				{
					m_pLeftArrow = CScene2D::Create(pos, D3DXVECTOR3(size.x, size.y, 0.0f), CScene::PRIORITY_NONE, pTex->SetTexture(pTextureFileName[nTex]));

					return true;
				}
				else
				{
					m_pLeftArrow = CScene2D::Create(pos, D3DXVECTOR3(size.x, size.y, 0.0f), CScene::PRIORITY_NONE, -1);

					return true;
				}
			}
		}
	}
	return false;
}

//=============================================================================
// �E�����̃��[�h
//=============================================================================
bool CTutorial::LoadRightArrow(FILE *pFile, int nNumTex, char **pTextureFileName)
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
		// END_RIGHTARROWSET����������e�p�[�c�̏����i�[���I����
		if (strcmp(&aText[0], "END_RIGHTARROWSET") == 0)
		{
			if (nTex < nNumTex)
			{
				// �e�N�X�`�����̎擾
				CTexture *pTex = CManager::GetTexture();

				if (pTex != NULL)
				{
					m_pRightArrow = CScene2D::Create(pos, D3DXVECTOR3(size.x, size.y, 0.0f), CScene::PRIORITY_NONE, pTex->SetTexture(pTextureFileName[nTex]));

					return true;
				}
				else
				{
					m_pRightArrow = CScene2D::Create(pos, D3DXVECTOR3(size.x, size.y, 0.0f), CScene::PRIORITY_NONE, -1);

					return true;
				}
			}
		}
	}
	return false;
}

//=============================================================================
// �`���[�g���A���̃��[�h
//=============================================================================
bool CTutorial::LoadTutorialSheet(FILE *pFile, int nNumTex, char **pTextureFileName)
{
	// �ϐ��錾
	char aLine[128];
	char aText[128];
	D3DXVECTOR3 pos = { 0.0f,0.0f,0.0f };
	D3DXVECTOR2 size = { 0.0f,0.0f };

	while (fgets(aLine, 128, pFile) != NULL)
	{
		// ���s�󔒂�������܂œǂݍ���
		fscanf(pFile, "%s", &aText[0]);

		// POS������������W�����i�[
		if (strcmp(&aText[0], "NUM_TUTORIAL") == 0)
		{
			fscanf(pFile, "%s %d", &aText[0], &m_nNumTutorial);

			// �������̊m��
			m_pTutorial = new CScene2D*[m_nNumTutorial];
		}
		// SKILLGAUGESET�̂Ƃ��X�L���Q�[�W�����i�[���鋖��^����
		if (strcmp(&aText[0], "TUTORIALSHEET") == 0)
		{
			TuotorialSheet(pFile, nNumTex, pTextureFileName);
		}
		// END_PARTSSET����������e�p�[�c�̏����i�[���I����
		if (strcmp(&aText[0], "END_TUTORIALSET") == 0)
		{
			return true;
		}
	}
	return false;
}

//=============================================================================
// ���S���̃��[�h
//=============================================================================
bool CTutorial::TuotorialSheet(FILE *pFile, int nNumTex, char **pTextureFileName)
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
		// END_LOGOSET����������e�p�[�c�̏����i�[���I����
		if (strcmp(&aText[0], "END_TUTORIALSHEET") == 0)
		{
			if (nTex < nNumTex && m_nCntTutorial < m_nNumTutorial)
			{
				// �e�N�X�`�����̎擾
				CTexture *pTex = CManager::GetTexture();

				if (pTex != NULL)
				{
					m_pTutorial[m_nCntTutorial] = CScene2D::Create(pos, D3DXVECTOR3(size.x, size.y, 0.0f), CScene::PRIORITY_NONE, pTex->SetTexture(pTextureFileName[nTex]));
					m_nCntTutorial++;

					return true;
				}
				else
				{
					m_pTutorial[m_nCntTutorial] = CScene2D::Create(pos, D3DXVECTOR3(size.x, size.y, 0.0f), CScene::PRIORITY_NONE, -1);
					m_nCntTutorial++;

					return true;
				}
			}
		}
	}
	return false;
}