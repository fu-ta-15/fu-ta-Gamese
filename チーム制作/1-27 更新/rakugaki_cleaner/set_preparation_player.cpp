//-----------------------------------------------------------------
//
//  �v���C���[�ҋ@��ʔz�u (set_preparation_player.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#include "set_preparation_player.h"
#include "sceneX.h"
#include "debugproc.h"

#include "manager.h"
#include "texture.h"

#include "preparation_player.h"
#include "preparation_player_manager.h"
#include "press_logo.h"
#include "bg.h"
#include "Scene2D.h"

//-----------------------------------------------------------------
// �ÓI�����o�ϐ��錾
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// CSetPreparationPlayer�̃R���X�g���N�^
//-----------------------------------------------------------------
CSetPreparationPlayer::CSetPreparationPlayer()
{

}

//-----------------------------------------------------------------
// CSetPreparationPlayer�̃f�X�g���N�^
//-----------------------------------------------------------------
CSetPreparationPlayer::~CSetPreparationPlayer()
{

}

//-----------------------------------------------------------------
// ���f���z�u���̏�����
//-----------------------------------------------------------------
void CSetPreparationPlayer::Init(void)
{
	// �����o�ϐ��̏�����
	for (int nCnt =0;nCnt < 4;nCnt++)
	{
		m_pFrame[nCnt] = NULL;
	}
	m_nCntFrame = 0;

	// ���̓ǂݍ���
	LoadData();
}

//-----------------------------------------------------------------
// ���f���z�u���̍X�V����
//-----------------------------------------------------------------
void CSetPreparationPlayer::Update(void)
{

}

//---------------------------------------------------------------- -
// UI���̓ǂݍ���
//-----------------------------------------------------------------
void CSetPreparationPlayer::LoadData(void)
{
	// FILE�^�̃|�C���^�֐�
	FILE *pFile;

	// �t�@�C�����J��
	pFile = fopen("data/TEXT/02-1_preparation_player/preparation_player.txt", "r");

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
				// BGSET�̂Ƃ��w�i�����i�[����
				if (strcmp(&aText[0], "BGSET") == 0)
				{
					LoadBg(pFile, nNumTex, pTextureFileName);
				}
				// PRESSLOGOSET�̂Ƃ��v���X���S�����i�[����
				if (strcmp(&aText[0], "PRESSLOGOSET") == 0)
				{
					LoadPressLogo(pFile, nNumTex, pTextureFileName);
				}
				// LOGOSET�̂Ƃ����S�����i�[����
				if (strcmp(&aText[0], "LOGOSET") == 0)
				{
					LoadLogo(pFile, nNumTex, pTextureFileName);
				}
				// FRAMESET�̂Ƃ��t���[�������i�[����
				if (strcmp(&aText[0], "FRAMESET") == 0)
				{
					LoadFrame(pFile, nNumTex, pTextureFileName);
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
	}
}

//-----------------------------------------------------------------
// �w�i���̓ǂݍ���
//-----------------------------------------------------------------
bool CSetPreparationPlayer::LoadBg(FILE *pFile, int nNumTex, char **pTextureFileName)
{
	// �ϐ��錾
	char aLine[128];
	char aText[128];
	int nNum = 0;
	D3DXVECTOR3 pos = { 0.0f,0.0f,0.0f };
	D3DXVECTOR2 size = { 0.0f,0.0f };
	D3DXCOLOR col = { 1.0f,1.0f,1.0f,1.0f };
	D3DXVECTOR2 moveTex = { 0.0f,0.0f };

	while (fgets(aLine, 128, pFile) != NULL)
	{
		// ���s�󔒂�������܂œǂݍ���
		fscanf(pFile, "%s", &aText[0]);

		// POS������������W�����i�[
		if (strcmp(&aText[0], "TYPE") == 0)
		{
			fscanf(pFile, "%s %d", &aText[0], &nNum);
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
		// MOVETEX����������e�N�X�`�����W�ړ��ʏ����i�[
		if (strcmp(&aText[0], "MOVETEX") == 0)
		{
			fscanf(pFile, "%s %f %f", &aText[0],
				&moveTex.x,
				&moveTex.y);
		}
		// END_BGSET����������w�i�̏����i�[���I����
		if (strcmp(&aText[0], "END_BGSET") == 0)
		{
			if (nNum < nNumTex)
			{
				// �e�N�X�`�����̎擾
				CTexture *pTex = CManager::GetTexture();

				if (pTex != NULL)
				{
					// �v���X���S�̐���
					CBg::Create(pos, size, col, moveTex, pTex->SetTexture(pTextureFileName[nNum]));
					return true;
				}
			}
		}
	}
	return false;
}

//-----------------------------------------------------------------
// �v���X���S���̓ǂݍ���
//-----------------------------------------------------------------
bool CSetPreparationPlayer::LoadPressLogo(FILE *pFile, int nNumTex, char **pTextureFileName)
{
	// �ϐ��錾
	char aLine[128];
	char aText[128];
	int nNum = 0;
	int nNumSwap = 0;
	D3DXVECTOR3 pos = { 0.0f,0.0f,0.0f };
	D3DXVECTOR2 size = { 0.0f,0.0f };
	D3DXCOLOR col = { 0.0f,0.0f,0.0f,0.0f };

	while (fgets(aLine, 128, pFile) != NULL)
	{
		// ���s�󔒂�������܂œǂݍ���
		fscanf(pFile, "%s", &aText[0]);

		// POS������������W�����i�[
		if (strcmp(&aText[0], "TYPE") == 0)
		{
			fscanf(pFile, "%s %d", &aText[0], &nNum);
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
		// SWAP����������܂�Ԃ������i�[
		if (strcmp(&aText[0], "SWAP") == 0)
		{
			fscanf(pFile, "%s %d", &aText[0], &nNumSwap);
		}
		// END_PRESSLOGO����������v���X���S�̏����i�[���I����
		if (strcmp(&aText[0], "END_PRESSLOGOSET") == 0)
		{
			if (nNum < nNumTex)
			{
				// �e�N�X�`�����̎擾
				CTexture *pTex = CManager::GetTexture();

				if (pTex != NULL)
				{
					// �v���X���S�̐���
					CPressLogo *pLogo =CPressLogo::Create(pos, size, nNumSwap, pTex->SetTexture(pTextureFileName[nNum]));
					pLogo->SetStartCnt(false);
					CPreparationPlayer::GetPPM()->SetPressLogo(pLogo);
					
					return true;
				}
			}
		}
	}
	return false;
}

//-----------------------------------------------------------------
// ���S���̓ǂݍ���
//-----------------------------------------------------------------
bool CSetPreparationPlayer::LoadLogo(FILE *pFile, int nNumTex, char **pTextureFileName)
{
	// �ϐ��錾
	char aLine[128];
	char aText[128];
	int nNum = 0;
	int nNumSwap = 0;
	D3DXVECTOR3 pos = { 0.0f,0.0f,0.0f };
	D3DXVECTOR2 size = { 0.0f,0.0f };
	D3DXCOLOR col = { 0.0f,0.0f,0.0f,0.0f };

	while (fgets(aLine, 128, pFile) != NULL)
	{
		// ���s�󔒂�������܂œǂݍ���
		fscanf(pFile, "%s", &aText[0]);

		// POS������������W�����i�[
		if (strcmp(&aText[0], "TYPE") == 0)
		{
			fscanf(pFile, "%s %d", &aText[0], &nNum);
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
		// END_LOGOSET���������烍�S�̏����i�[���I����
		if (strcmp(&aText[0], "END_LOGOSET") == 0)
		{
			if (nNum < nNumTex)
			{
				// �e�N�X�`�����̎擾
				CTexture *pTex = CManager::GetTexture();

				if (pTex != NULL)
				{
					// ���S�̐���
					CScene2D::Create(pos, D3DXVECTOR3(size.x,size.y,0.0f), CScene::PRIORITY_UI, pTex->SetTexture(pTextureFileName[nNum]));
					return true;
				}
			}
		}
	}
	return false;
}

//-----------------------------------------------------------------
// �t���[�����̓ǂݍ���
//-----------------------------------------------------------------
bool CSetPreparationPlayer::LoadFrame(FILE *pFile, int nNumTex, char **pTextureFileName)
{
	// �ϐ��錾
	char aLine[128];
	char aText[128];
	int nNum = 0;
	int nNumSwap = 0;
	D3DXVECTOR3 pos = { 0.0f,0.0f,0.0f };
	D3DXVECTOR2 size = { 0.0f,0.0f };
	D3DXVECTOR2 texsheet = { 0.0f,0.0f };
	D3DXVECTOR2 tex = { 0.0f,0.0f };

	while (fgets(aLine, 128, pFile) != NULL)
	{
		// ���s�󔒂�������܂œǂݍ���
		fscanf(pFile, "%s", &aText[0]);

		// POS������������W�����i�[
		if (strcmp(&aText[0], "TYPE") == 0)
		{
			fscanf(pFile, "%s %d", &aText[0], &nNum);
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
		// TEXSHEET����������UV�l�����i�[
		if (strcmp(&aText[0], "TEXSHEET") == 0)
		{
			fscanf(pFile, "%s %f %f", &aText[0],
				&texsheet.x,
				&texsheet.y);
		}
		// TEX����������e�N�X�`�����W�����i�[
		if (strcmp(&aText[0], "TEX") == 0)
		{
			fscanf(pFile, "%s %f %f", &aText[0],
				&tex.x,
				&tex.y);
		}
		// END_FRAMESET����������t���[���̏����i�[���I����
		if (strcmp(&aText[0], "END_FRAMESET") == 0)
		{
			if (nNum < nNumTex)
			{
				// �e�N�X�`�����̎擾
				CTexture *pTex = CManager::GetTexture();

				if (pTex != NULL)
				{
					// �t���[���̐���
					CScene2D *pFrame = CScene2D::Create(pos, D3DXVECTOR3(size.x, size.y, 0.0f), CScene::PRIORITY_UI, pTex->SetTexture(pTextureFileName[nNum]));
					pFrame->SetTexSheet(texsheet);
					pFrame->SetTex(tex);
					CPreparationPlayer::GetPPM()->SetFrame(pFrame,m_nCntFrame);

					// �J�E���g
					m_nCntFrame++;

					return true;
				}
			}
		}
	}
	return false;
}