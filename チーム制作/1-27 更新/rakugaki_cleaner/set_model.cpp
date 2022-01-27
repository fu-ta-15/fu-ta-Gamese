//-----------------------------------------------------------------
//
// ���f���z�u (set_model.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#include "set_model.h"
#include "sceneX.h"
#include "stage.h"
#include "floor.h"
#include "texture.h"
#include "manager.h"
#include "debugproc.h"

//-----------------------------------------------------------------
// �ÓI�����o�ϐ��錾
//-----------------------------------------------------------------
char *CSetModel::m_apTextName = {};				// �ǂݍ��ރe�L�X�g�̏��
char *CSetModel::m_apTextNameSetModel[64] = {};	// ���f���z�u����ǂݍ��ރe�L�X�g�̏��
int CSetModel::m_nNumType = 0;					// ���f���̎�ސ�
int CSetModel::m_nNumModel = 0;					// ���f���̑���

//-----------------------------------------------------------------
// CSetModel�̃R���X�g���N�^
//-----------------------------------------------------------------
CSetModel::CSetModel()
{

}

//-----------------------------------------------------------------
// CSetModel�̃f�X�g���N�^
//-----------------------------------------------------------------
CSetModel::~CSetModel()
{

}

//-----------------------------------------------------------------
// ���f���z�u���̏�����
//-----------------------------------------------------------------
void CSetModel::Init(void)
{

}

//-----------------------------------------------------------------
// ���f���z�u���̍X�V����
//-----------------------------------------------------------------
void CSetModel::Update(void)
{
	// �\��
	CDebugProc::Print("\n--- ���f����� ---\n");
	CDebugProc::Print("���f���̎�� (%d)\n", m_nNumType);
	CDebugProc::Print("���f���̑��� (%d)\n", m_nNumModel);
}

//-----------------------------------------------------------------
// ���f���z�u���̓ǂݍ���
//-----------------------------------------------------------------
void CSetModel::LoadData(void)
{
	// FILE�^�̃|�C���^�֐�
	FILE *pFile;

	// �t�@�C�����J��
	pFile = fopen("data/TEXT/04_game/model.txt", "r");

	// NULL�`�F�b�N
	if (pFile != NULL)
	{
		// �ϐ��錾
		char aLine[128];
		char aText[128];
		bool bScript = false;
		bool bPartsSet = false;
		int nCntTex = 0;
		int nCntModel = 0;
		int nNumModelType = -1;
		D3DXVECTOR3 pos = { 0.0f,0.0f,0.0f };
		D3DXVECTOR3 rot = { 0.0f,0.0f,0.0f };
		int nNumModel = 0;
		char **apModelFileName;
		int nNumTex = 0;
		char **apTextureFileName;

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
					apTextureFileName = new char*[nNumTex];

					for (int nCnt = 0; nCnt < nNumTex; nCnt++)
					{
						apTextureFileName[nCnt] = new char[128];
					}
				}
				// MODEL_FILENAME����������X�t�@�C�������i�[
				if (strcmp(&aText[0], "TEXTURE_FILENAME") == 0 && nCntTex < nNumTex)
				{
					fscanf(pFile, "%s %s", &aText[0], &apTextureFileName[nCntTex][0]);

					nCntTex++;
				}
				if (strcmp(&aText[0], "NUM_MODEL") == 0)
				{
					fscanf(pFile, "%s %d", &aText[0], &nNumModel);

					m_nNumType = nNumModel;

					// ���f���t�@�C���l�[���i�[�p�ϐ��̗̈�𓮓I�Ɋm�ۂ���
					apModelFileName = new char*[nNumModel];

					for (int nCnt = 0; nCnt < nNumModel; nCnt++)
					{
						apModelFileName[nCnt] = new char[128];
					}
				}
				// MODEL_FILENAME����������X�t�@�C�������i�[
				if (strcmp(&aText[0], "MODEL_FILENAME") == 0 && nCntModel < nNumModel)
				{
					fscanf(pFile, "%s %s", &aText[0], &apModelFileName[nCntModel][0]);

					nCntModel++;
				}
				// FLOORSET���������珰�����i�[���鋖��^����
				if (strcmp(&aText[0], "FLOORSET") == 0)
				{
					LoadFloor(pFile, nNumTex, apTextureFileName);
				}
				// STAGESET����������X�e�[�W�����i�[���鋖��^����
				if (strcmp(&aText[0], "STAGESET") == 0)
				{
					LoadStage(pFile, nNumModel, apModelFileName);
				}
				// MODELSET���������烂�f�������i�[���鋖��^����
				if (strcmp(&aText[0], "MODELSET") == 0)
				{
					LoadModel(pFile, nNumModel, apModelFileName);
				}
				// END_SCRIPT����������ǂݍ��݂��I����
				if (strcmp(&aText[0], "END_SCRIPT") == 0)
				{
					// �������̊J��
					delete[] apModelFileName;

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
// �����̓ǂݍ���
//-----------------------------------------------------------------
bool CSetModel::LoadFloor(FILE *pFile, int nNumTex, char **pTextureFileName)
{
	// �ϐ��錾
	char aLine[128];
	char aText[128];
	int nTex;
	D3DXVECTOR3 pos = { 0.0f,0.0f,0.0f };
	D3DXVECTOR2 size = { 0.0f,0.0f };
	D3DXVECTOR2 block = { 0.0f,0.0f };

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
		// BLOCK���������烁�b�V���̃u���b�N�������i�[
		if (strcmp(&aText[0], "BLOCK") == 0)
		{
			fscanf(pFile, "%s %f %f", &aText[0],
				&block.x,
				&block.y);
		}
		// END_PARTSSET����������e�p�[�c�̏����i�[���I����
		if (strcmp(&aText[0], "END_FLOORSET") == 0)
		{
			if (nTex < nNumTex)
			{
				// �e�N�X�`�����̎擾
				CTexture *pTex = CManager::GetTexture();

				if (pTex != NULL)
				{
					// ���f���̐���
					CFloor::Create(pos, size, block, pTex->SetTexture(&pTextureFileName[nTex][0]));
				}
			}
			return true;
		}
	}
	return false;
}

//-----------------------------------------------------------------
// �X�e�[�W���̓ǂݍ���
//-----------------------------------------------------------------
bool CSetModel::LoadStage(FILE *pFile, int nNumModel, char **pModelFileName)
{
	// �ϐ��錾
	char aLine[128];
	char aText[128];
	int nTex;
	D3DXVECTOR3 pos = { 0.0f,0.0f,0.0f };
	D3DXVECTOR3 rot = { 0.0f,0.0f ,0.0f};

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
		// ROT����������p�x�����i�[
		if (strcmp(&aText[0], "ROT") == 0)
		{
			fscanf(pFile, "%s %f %f %f", &aText[0],
				&rot.x,
				&rot.y,
				&rot.z);

			rot.x = (rot.x / 180.0f) * D3DX_PI;
			rot.y = (rot.y / 180.0f) * D3DX_PI;
			rot.z = (rot.z / 180.0f) * D3DX_PI;
		}
		// END_STAGESET����������X�e�[�W�̏����i�[���I����
		if (strcmp(&aText[0], "END_STAGESET") == 0)
		{
			if (nTex < m_nNumType)
			{
				// ���f���̐���
				CStage::Create(pos, D3DXVECTOR2(0.0f, 0.0f), rot, &pModelFileName[nTex][0]);
			}

			// ���f�������̃J�E���g
			m_nNumModel++;

			return true;
		}
	}
	return false;
}

//-----------------------------------------------------------------
// ���f�����̓ǂݍ���
//-----------------------------------------------------------------
bool CSetModel::LoadModel(FILE *pFile, int nNumModel, char **pModelFileName)
{
	// �ϐ��錾
	char aLine[128];
	char aText[128];
	int nTex;
	D3DXVECTOR3 pos = { 0.0f,0.0f,0.0f };
	D3DXVECTOR3 rot = { 0.0f,0.0f ,0.0f };

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
		// ROT����������p�x�����i�[
		if (strcmp(&aText[0], "ROT") == 0)
		{
			fscanf(pFile, "%s %f %f %f", &aText[0],
				&rot.x,
				&rot.y,
				&rot.z);

			rot.x = (rot.x / 180.0f) * D3DX_PI;
			rot.y = (rot.y / 180.0f) * D3DX_PI;
			rot.z = (rot.z / 180.0f) * D3DX_PI;
		}
		// END_PARTSSET����������e�p�[�c�̏����i�[���I����
		if (strcmp(&aText[0], "END_MODELSET") == 0)
		{
			if (nTex < m_nNumType)
			{
				// ���f���̐���
				CSceneX::Create(pos, D3DXVECTOR2(0.0f, 0.0f), rot, &pModelFileName[nTex][0]);
			}

			// ���f�������̃J�E���g
			m_nNumModel++;

			return true;
		}
	}
	return false;
}