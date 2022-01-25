//-----------------------------------------------------------------
//
//  ���[�h�I����ʔz�u (set_mode_select.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#include "set_mode_select.h"
#include "sceneX.h"
#include "debugproc.h"

#include "manager.h"
#include "texture.h"

#include "press_logo.h"
#include "title_logo.h"
#include "bg.h"
#include "mode_logo.h"
#include "Scene2D.h"

//-----------------------------------------------------------------
// �ÓI�����o�ϐ��錾
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// CSetModeSelect�̃R���X�g���N�^
//-----------------------------------------------------------------
CSetModeSelect::CSetModeSelect()
{

}

//-----------------------------------------------------------------
// CSetModeSelect�̃f�X�g���N�^
//-----------------------------------------------------------------
CSetModeSelect::~CSetModeSelect()
{

}

//-----------------------------------------------------------------
// ���f���z�u���̏�����
//-----------------------------------------------------------------
void CSetModeSelect::Init(void)
{
	CModeLogo *pModeLogo = CModeLogo::Create({ 0.0f,0.0f,0.0f }, { 0.0f,0.0f });

	LoadData();
}

//-----------------------------------------------------------------
// ���f���z�u���̍X�V����
//-----------------------------------------------------------------
void CSetModeSelect::Update(void)
{

}

//---------------------------------------------------------------- -
// UI���̓ǂݍ���
//-----------------------------------------------------------------
void CSetModeSelect::LoadData(void)
{
	// FILE�^�̃|�C���^�֐�
	FILE *pFile;

	// �t�@�C�����J��
	pFile = fopen("data/TEXT/01_mode_select/mode_select.txt", "r");

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
				// TITLELOGOSET�̂Ƃ��^�C�g�����S�����i�[����
				if (strcmp(&aText[0], "LOGOSET") == 0)
				{
					LoadLogo(pFile, nNumTex, pTextureFileName);
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
bool CSetModeSelect::LoadBg(FILE *pFile, int nNumTex, char **pTextureFileName)
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
// ���[�h�I����ʃ��S���̓ǂݍ���
//-----------------------------------------------------------------
bool CSetModeSelect::LoadLogo(FILE *pFile, int nNumTex, char **pTextureFileName)
{
	// �ϐ��錾
	char aLine[128];
	char aText[128];
	int nNum = 0;
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
		// END_TITLELOGOSET����������^�C�g�����S�̏����i�[���I����
		if (strcmp(&aText[0], "END_LOGOSET") == 0)
		{
			if (nNum < nNumTex)
			{
				// �e�N�X�`�����̎擾
				CTexture *pTex = CManager::GetTexture();

				if (pTex != NULL)
				{
					// �v���X���S�̐���
					CScene2D::Create(pos, D3DXVECTOR3(size.x,size.y,0.0f),CScene::PRIORITY_UI, pTex->SetTexture(pTextureFileName[nNum]));
					return true;
				}
			}
		}
	}
	return false;
}