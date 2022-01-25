//-----------------------------------------------------------------
//
// ���N�K�L�z�u (set_graffiti.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#include "set_graffiti.h"
#include "graffiti.h"
#include "manager.h"
#include "base.h"
#include "game.h"
#include "graffiti_manager.h"

#include "debugproc.h"

//-----------------------------------------------------------------
// �ÓI�����o�ϐ��錾
//-----------------------------------------------------------------
char *CSetGraffiti::m_apTextName = {};							// �ǂݍ��ރe�L�X�g�̏��
CSetGraffiti::Graffiti_Info *CSetGraffiti::m_graffiti_info = {};	// ���N�K�L�ݒu�ɕK�v�ȍ\���̂̕ϐ�

char CSetGraffiti::m_pFileName_30[TYPE_30_MAX][128] = 
{"data/GRAFFITIDATA/30/txt/30_dumpling.txt",
"data/GRAFFITIDATA/30/txt/30_MountFuji.txt" ,
"data/GRAFFITIDATA/30/txt/30_Strawberry.txt" ,
"data/GRAFFITIDATA/30/txt/30_���~.txt" ,
"data/GRAFFITIDATA/30/txt/30_cat.txt" ,
"data/GRAFFITIDATA/30/txt/30_labyrinch.txt" ,
"data/GRAFFITIDATA/30/txt/30_redball.txt" ,
"data/GRAFFITIDATA/30/txt/30_snake.txt" ,
"data/GRAFFITIDATA/30/txt/30_tomato.txt",
"data/GRAFFITIDATA/30/txt/30_raf00.txt" ,
"data/GRAFFITIDATA/30/txt/30_raf01.txt" ,
"data/GRAFFITIDATA/30/txt/30_raf02.txt" };

char CSetGraffiti::m_pFileName_35[TYPE_35_MAX][128] = 
{ "data/GRAFFITIDATA/35/txt/35_test.txt" };

char CSetGraffiti::m_pFileName_40[TYPE_40_MAX][128] = 
{ "data/GRAFFITIDATA/40/txt/40_test.txt" };

char CSetGraffiti::m_pFileName_45[TYPE_45_MAX][128] = 
{ "data/GRAFFITIDATA/45/txt/45_Churu.txt" ,
"data/GRAFFITIDATA/45/txt/45_Kano.txt" ,
"data/GRAFFITIDATA/45/txt/45_raf03.txt" ,
"data/GRAFFITIDATA/45/txt/45_raf04.txt" ,
"data/GRAFFITIDATA/45/txt/45_raf05.txt" };

//-----------------------------------------------------------------
// CSetGraffiti�̃R���X�g���N�^
//-----------------------------------------------------------------
CSetGraffiti::CSetGraffiti()
{

}

//-----------------------------------------------------------------
// CSetGraffiti�̃f�X�g���N�^
//-----------------------------------------------------------------
CSetGraffiti::~CSetGraffiti()
{

}

//-----------------------------------------------------------------
// ���f���z�u���̏�����
//-----------------------------------------------------------------
void CSetGraffiti::Init(void)
{

}

//-----------------------------------------------------------------
// ���f���z�u���̍X�V����
//-----------------------------------------------------------------
void CSetGraffiti::Update(void)
{

}

//-----------------------------------------------------------------
// ���f���z�u���̓ǂݍ���
//-----------------------------------------------------------------
bool CSetGraffiti::LoadGraffitiData(void)
{
	// FILE�^�̃|�C���^�֐�
	FILE *pFile;

	// �t�@�C�����J��
	pFile = fopen("data/TEXT/04_game/graffiti_info.txt", "r");

	if (pFile != NULL)
	{
		// �ϐ��錾
		char aLine[128] = {};
		char aText[128] = {};
		int nNumGraffiti = 0;
		int nCntGraffiti = 0;
		int nCntCol = 1;
		bool bScript = false;
		bool bGraffiti = false;
		bool bCol = false;

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
				// PARTSSET��&aText[0]�e�p�[�c�̏����i�[���鋖��^����
				if (strcmp(&aText[0], "GRAFFITISET") == 0)
				{
					// �������̊m��
					m_graffiti_info = new Graffiti_Info;

					bGraffiti = true;
				}
				// �e�p�[�c�̏����i�[���鋖������Ƃ�����
				if (bGraffiti == true)
				{
					// POS������������W�����i�[
					if (strcmp(&aText[0], "TYPE") == 0)
					{
						fscanf(pFile, "%s %d", &aText[0], &m_graffiti_info->nType);
					}
					// POS������������W�����i�[
					if (strcmp(&aText[0], "POS") == 0)
					{
						fscanf(pFile, "%s %f %f %f", &aText[0],
							&m_graffiti_info->pos.x,
							&m_graffiti_info->pos.y,
							&m_graffiti_info->pos.z);
					}
					// ROT����������p�x�����i�[
					if (strcmp(&aText[0], "SIZE") == 0)
					{
						fscanf(pFile, "%s %f", &aText[0],&m_graffiti_info->fSize);

						if (!LoadColInfo())
						{
							return false;
						}
					}
					// END_PARTSSET����������e�p�[�c�̏����i�[���I����
					if (strcmp(&aText[0], "END_GRAFFITISET") == 0)
					{
						if (!SetGraffiti())
						{
							return false;
						}

						// �������̊J��
						delete[] m_graffiti_info->pCol;
						m_graffiti_info->pCol = NULL;

						delete[] m_graffiti_info->pFileName;
						m_graffiti_info->pFileName = NULL;

						delete m_graffiti_info;
						m_graffiti_info = NULL;

						bGraffiti = false;
					}
					// END_SCRIPT����������ǂݍ��݂��I����
					if (strcmp(&aText[0], "END_SCRIPT") == 0)
					{
						break;
					}
				}
			}
		}
		// �t�@�C�������
		fclose(pFile);
	}
	else
	{
	}

	return true;
}

//-----------------------------------------------------------------
// ���f���z�u���̓ǂݍ���
//-----------------------------------------------------------------
bool CSetGraffiti::LoadColInfo(void)
{
	// �e�L�X�g�������_���Ɍ��߂�
	RandomGraffiti();

	// FILE�^�̃|�C���^�֐�
	FILE *pFile;

	// �t�@�C�����J��
	if (m_graffiti_info->pColFileName != NULL)
	{
		pFile = fopen(m_graffiti_info->pColFileName, "r");

		if (pFile != NULL)
		{
			// �ϐ��錾
			char aLine[128] = {};
			char aText[128] = {};
			int nNumGraffiti = 0;
			int nCntGraffiti = 0;
			int nCntCol = 1;
			bool bScript = false;
			bool bGraffiti = false;
			bool bCol = false;

			// SCRIPT��T��
			while (fgets(aLine, 128, pFile) != NULL)
			{
				// ���s�󔒂�������܂œǂݍ���
				fscanf(pFile, "%s", &aText[0]);

				// FILE_GRAFFITI����������X�t�@�C�������i�[
				if (strcmp(&aText[0], "FILE_GRAFFITI") == 0)
				{
					m_graffiti_info->pFileName = new char[128];
					fscanf(pFile, "%s %s", &aText[0], m_graffiti_info->pFileName);
				}

				// COLSET����������F���̊i�[���n�߂�
				if (strcmp(&aText[0], "COLSET") == 0)
				{
					bCol = true;
				}
				if (bCol == true)
				{
					// NUMCOLOR����������F�̑������i�[
					if (strcmp(&aText[0], "NUMCOLOR") == 0)
					{
						fscanf(pFile, "%s %d", &aText[0], &m_graffiti_info->nNumCol);

						nCntCol = 1;

						// �������̊m��
						m_graffiti_info->pCol = new D3DXCOLOR[m_graffiti_info->nNumCol + 1];

						m_graffiti_info->pCol[0] = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
					}
					// COLOR����������F�����i�[
					if (strcmp(&aText[0], "COLOR") == 0 && nCntCol < (m_graffiti_info->nNumCol + 1))
					{
						fscanf(pFile, "%s %f %f %f %f", &aText[0],
							&m_graffiti_info->pCol[nCntCol].r,
							&m_graffiti_info->pCol[nCntCol].g,
							&m_graffiti_info->pCol[nCntCol].b,
							&m_graffiti_info->pCol[nCntCol].a);

						nCntCol++;
					}
					// END_COLSET����������F���̊i�[���I����
					if (strcmp(&aText[0], "END_COLSET") == 0)
					{
						bCol = false;
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
	}
	else
	{
		return false;
	}

	return true;
}

//-----------------------------------------------------------------
// ���f���z�u���̓ǂݍ���
//-----------------------------------------------------------------
bool CSetGraffiti::SetGraffiti(void)
{
	// �ϐ��錾
	int nCntX = 0, nCntY = 0;
	int **aData;

	// FILE�^�̃|�C���^�֐�
	FILE *pFileStage;

	// �t�@�C�����J��
	pFileStage = fopen(&m_graffiti_info->pFileName[0], "r");

	// �������̊m��
	aData = new int*[m_graffiti_info->nCntX];

	for (int nCnt = 0; nCnt < m_graffiti_info->nCntX; nCnt++)
	{
		aData[nCnt] = new int[m_graffiti_info->nCntY];
	}

	if (pFileStage != NULL)
	{
		// �ϐ��錾
		char aLine[128] = {};

		// �ꕶ�����ׂ�
		while (fgets(aLine, 128, pFileStage) != NULL)
		{
			nCntX = 0;
			char *pTokun = strtok(aLine, ",");

			while (pTokun != NULL)
			{
				int nNum = atoi(pTokun);
				aData[nCntY][nCntX] = nNum;
				pTokun = strtok(NULL, ",");
				nCntX++;
			}
			nCntY++;
		}
		fclose(pFileStage);

		// ���N�K�L�Ǘ����̎擾
		CGraffitiManager *pGrafittiManager = NULL;
		CBase *pBase = CManager::GetBase();

		if (pBase != NULL)
		{
			CGame *pGame = (CGame*)pBase;
			pGrafittiManager = pGame->GetGraffitiManager();
		}
		// �X�e�[�W�̐���
		for (int nCntY = 0; nCntY < m_graffiti_info->nCntY; nCntY++)
		{
			for (int nCntX = 0; nCntX < m_graffiti_info->nCntX; nCntX++)
			{
				// �ϐ��錾
				if (aData[nCntY][nCntX] != 0)
				{
					if (m_graffiti_info->nNumCol >= aData[nCntY][nCntX])
					{
						// �^�C���̐���
						CGraffiti::Create(D3DXVECTOR3(m_graffiti_info->pos.x + m_graffiti_info->fSize * (nCntX - (m_graffiti_info->nCntX / 2.0f)),
							m_graffiti_info->pos.y + 0.5f,
							m_graffiti_info->pos.z + m_graffiti_info->fSize * ((m_graffiti_info->nCntY - nCntY - 1) - m_graffiti_info->nCntY / 2.0f)),
							D3DXVECTOR2(m_graffiti_info->fSize, m_graffiti_info->fSize),
							m_graffiti_info->pCol[aData[nCntY][nCntX]]);

						if (pGrafittiManager != NULL)
						{
							pGrafittiManager->SetTotalGraffiti(10);
						}
					}
				}
			}
		}
		// �������̊J��
		for (int nCnt = 0; nCnt < m_graffiti_info->nCntX; nCnt++)
		{
			delete[] aData[nCnt];
			aData[nCnt] = NULL;
		}
		delete[] aData;
		aData = NULL;
	}
	else
	{
		return false;
	}
	return true;
}

//-----------------------------------------------------------------
// ���f���z�u���̓ǂݍ���
//-----------------------------------------------------------------
void CSetGraffiti::RandomGraffiti(void)
{
	switch (m_graffiti_info->nType)
	{
	case 0:
		m_graffiti_info->pColFileName = new char[128];
		m_graffiti_info->pColFileName = m_pFileName_30[rand() % TYPE_30_MAX];
		m_graffiti_info->nCntX = 30;
		m_graffiti_info->nCntY = 30;
		break;

	case 1:
		m_graffiti_info->pColFileName = new char[128];
		m_graffiti_info->pColFileName = m_pFileName_35[rand() % TYPE_35_MAX];
		m_graffiti_info->nCntX = 35;
		m_graffiti_info->nCntY = 35;
		break;

	case 2:
		m_graffiti_info->pColFileName = new char[128];
		m_graffiti_info->pColFileName = m_pFileName_40[rand() % TYPE_40_MAX];
		m_graffiti_info->nCntX = 40;
		m_graffiti_info->nCntY = 40;
		break;

	case 3:
		m_graffiti_info->pColFileName = new char[128];
		m_graffiti_info->pColFileName = m_pFileName_45[rand() % TYPE_45_MAX];
		m_graffiti_info->nCntX = 45;
		m_graffiti_info->nCntY = 45;
		break;
	}
}