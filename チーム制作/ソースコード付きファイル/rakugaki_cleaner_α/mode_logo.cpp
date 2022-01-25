//=============================================================================
//
// �|�[�Y (pause.cpp)
// Author:Itsuki Takaiwa
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#include "mode_logo.h"
#include "fade.h"
#include "input_keyboard.h"
#include "sound.h"
#include "manager.h"
#include "renderer.h"
#include "Scene2D.h"
#include "input_joypad.h"
#include "texture.h"
#include "base.h"
#include "mode_select.h"
#include "tutorial.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//=============================================================================
// CModeLogo�̃R���X�g���N�^
//=============================================================================
CModeLogo::CModeLogo(int nPriority) :CScene(nPriority)
{

}

//=============================================================================
// CModeLogo�̃f�X�g���N�^
//=============================================================================
CModeLogo::~CModeLogo()
{

}

//=============================================================================
// ��������
//=============================================================================
CModeLogo *CModeLogo::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	// �|�C���^�ϐ��̐���
	CModeLogo *pModeLogo;
	pModeLogo = new CModeLogo(CScene::PRIORITY_UI);

	// NULL�`�F�b�N
	if (pModeLogo != NULL)
	{
		// ����������
		pModeLogo->Init(pos, size);

		// �I�u�W�F�N�g�^�C�v�̐ݒ�
		pModeLogo->SetObjType(OBJTYPE_UI);
	}

	return pModeLogo;
}

//=============================================================================
// �|�[�Y�̏���������
//=============================================================================
HRESULT CModeLogo::Init(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	// �����o�ϐ��̏�����
	m_nCntLogo = 0;
	m_nNumLogo = 0;
	m_nNumMode = 0;
	m_nNumSelect = 0;

	// ���S���̓ǂݍ���
	LoadModeLogo();

	return S_OK;
}

//=============================================================================
// �|�[�Y�̏I������
//=============================================================================
void CModeLogo::Uninit(void)
{
	// �������̊J��
	delete[] m_pLogo;
	m_pLogo = NULL;

	delete[] m_nNumMode;
	m_nNumMode = NULL;

	// �I�u�W�F�N�g�j��
	Release();
}

//=============================================================================
// �|�[�Y�̍X�V����
//=============================================================================
void CModeLogo::Update(void)
{
	// �L�[�{�[�h���̎擾
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();

	// �Q�[���p�b�h���̎擾
	CInputJoypad *pJoypad = CManager::GetInputJoypad();
	bool *bCrossTrigger = pJoypad->GetCrossKeyTrigger(0);

	// �t�F�[�h���̎擾
	CFade *pFade = CManager::GetFade();

	// �eUI�̐F��ݒ�
	for (int nCnt = 0; nCnt < m_nNumLogo; nCnt++)
	{
		if (m_nNumSelect == nCnt)
		{
			// ���ݑI������Ă�����̂̓��l1.0f�ɐݒ�
			m_pLogo[nCnt]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
		else
		{
			// ����ȊO�̓��l0.3f�ɐݒ�
			m_pLogo[nCnt]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f));
		}
	}

	if (pFade->GetFade() == CFade::FADE_NONE)
	{
		if (pKeyboard->GetTrigger(CInputKeyboard::KEYINFO_LEFT) || bCrossTrigger[CInputJoypad::STICK_AND_CROSSKEY_LEFT])
		{
			// ��ɂ����Ƃ��ԍ������炷
			m_nNumSelect--;

			if (m_nNumSelect < 0)
			{
				// �R���e�B�j���[��菬�����Ȃ����Ƃ�
				m_nNumSelect = m_nNumLogo - 1;
			}
		}
		else if (pKeyboard->GetTrigger(CInputKeyboard::KEYINFO_RIGHT) || bCrossTrigger[CInputJoypad::STICK_AND_CROSSKEY_RIGHT])
		{
			// ���ɂ����Ƃ��ԍ��𑝂₷
			m_nNumSelect++;

			if (m_nNumSelect > m_nNumLogo - 1)
			{
				// �N�C�b�g���傫���Ȃ����Ƃ�
				m_nNumSelect = 0;
			}
		}
		if (pKeyboard->GetTrigger(CInputKeyboard::KEYINFO_OK) || pJoypad->GetButtonTrigger(0, JOYPAD_DECISION))
		{
			if (m_nNumMode[m_nNumSelect] == (int)CModeSelect::MODESELECT_TRANSITION_GAME_TUTORIAL)
			{
				CTutorial *pTutorial = CTutorial::Create({ 0.0f,0.0f,0.0f }, { 0.0f,0.0f });
			}
			else
			{
				// �x�[�X�̎擾
				CBase *pBase = CManager::GetBase();

				if (pBase != NULL)
				{
					// ���[�h�I���ɃL���X�g
					CModeSelect *pModeSelect = (CModeSelect*)pBase;

					pModeSelect->SetMode((CModeSelect::MODESELECT)m_nNumMode[m_nNumSelect]);
				}
			}
		}
	}
}

//=============================================================================
// �|�[�Y�̕`�揈��
//=============================================================================
void CModeLogo::Draw(void)
{
	// �w�i�̕`��
	for (int nCnt = 0; nCnt < m_nCntLogo; nCnt++)
	{
		m_pLogo[nCnt]->Draw();
	}
}

//=============================================================================
// ���S���̃��[�h
//=============================================================================
bool CModeLogo::LoadModeLogo(void)
{
	// FILE�^�̃|�C���^�֐�
	FILE *pFile;

	// �t�@�C�����J��
	pFile = fopen("data/TEXT/01_mode_select/logo.txt", "r");

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

				// POS������������W�����i�[
				if (strcmp(&aText[0], "NUM_LOGO") == 0)
				{
					fscanf(pFile, "%s %d", &aText[0], &m_nNumLogo);

					// �������̊m��
					m_pLogo = new CScene2D*[m_nNumLogo];
					m_nNumMode = new int[m_nNumLogo];
				}
				// END_PARTSSET����������e�p�[�c�̏����i�[���I����
				if (strcmp(&aText[0], "LOGOSET") == 0)
				{
					ModeLogo(pFile, nNumTex, pTextureFileName);
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

		return true;
	}
	return false;
}

//=============================================================================
// ���S���̃��[�h
//=============================================================================
bool CModeLogo::ModeLogo(FILE *pFile, int nNumTex, char **pTextureFileName)
{
	// �ϐ��錾
	char aLine[128];
	char aText[128];
	int nTex;
	D3DXVECTOR3 pos = { 0.0f,0.0f,0.0f };
	D3DXVECTOR2 size = { 0.0f,0.0f };
	D3DXCOLOR col = { 1.0f,1.0f,1.0f,1.0f };
	int nNumTra = -1;	// �J�ڐ�

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
		// MODE������������W�����i�[
		if (strcmp(&aText[0], "MODE") == 0)
		{
			fscanf(pFile, "%s %d", &aText[0], &m_nNumMode[m_nCntLogo]);
		}
		// END_PARTSSET����������e�p�[�c�̏����i�[���I����
		if (strcmp(&aText[0], "END_LOGOSET") == 0)
		{
			if (nTex < nNumTex)
			{
				// �e�N�X�`�����̎擾
				CTexture *pTex = CManager::GetTexture();

				if (pTex != NULL && m_nCntLogo < m_nNumLogo)
				{
					// �X�L���Q�[�W�̐���
					m_pLogo[m_nCntLogo] = CScene2D::Create(pos, D3DXVECTOR3(size.x, size.y, 0.0f), CScene::PRIORITY_NONE, pTex->SetTexture(&pTextureFileName[nTex][0]));
					m_pLogo[m_nCntLogo]->SetCol(col);
					m_nCntLogo++;
					return true;
				}
			}
		}
	}
	return false;
}