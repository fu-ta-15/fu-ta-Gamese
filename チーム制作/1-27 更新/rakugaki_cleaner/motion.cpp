//=============================================================================
//
// ���[�V���� [motion.cpp]
// Author : 
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#include "motion.h"
#include "manager.h"
#include "renderer.h"
#include "player.h"
#include "game.h"

//*****************************************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************************************
char CMotion::m_aTextMotion[MOTIONTYPE_MAX][128] = {"data/TEXT/motion.txt"};
CMotion::MOTION_INFO CMotion::m_aMotionInfo[MOTIONTYPE_MAX][10] = {};

//=============================================================================
// CMotion�̃R���X�g���N�^
//=============================================================================
CMotion::CMotion()
{
	// �����o�ϐ��̏�����
	m_motion.nCounterMotion = 0;
	m_motion.nKey = 0;
}

//=============================================================================
// CMotion�̃f�X�g���N�^
//=============================================================================
CMotion::~CMotion()
{

}

//=============================================================================
// ��������
//=============================================================================
CMotion *CMotion::Create(void)
{
	// �|�C���^�ϐ��̐���
	CMotion *pMotion;
	pMotion = new CMotion;

	// NULL�`�F�b�N
	if (pMotion != NULL)
	{
		// ����������
		pMotion->Init();
	}

	return pMotion;
}

//=============================================================================
// ����������
//=============================================================================
void CMotion::Init(void)
{
	// �����o�ϐ��̏�����
	m_motion.nCounterMotion = 0;
	m_motion.nKey = 0;
}

//=============================================================================
// ���[�V�������ǂݍ���
//=============================================================================
void CMotion::LoadMotionData(void)
{
	// FILE�^�̃|�C���^�֐�
	FILE *pFile;

	for (int nCntText = 0; nCntText < MOTIONTYPE_MAX; nCntText++)
	{
		// �t�@�C�����J��
		pFile = fopen(&m_aTextMotion[nCntText][0],"r");

		// NULL�`�F�b�N
		if (pFile != NULL)
		{
			// �ϐ��錾
			char aLine[128];
			char aText[128];
			bool bScript = false;
			bool bMotion = false;
			bool bSetKey = false;
			bool bKey = false;
			int nCntKey = 0;
			int nCntKeyNum = 0;
			int nCntMotion = 0;
			int nNumKey = 0;

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
					// MODEL_FILENAME����������X�t�@�C�������i�[
					if (strcmp(&aText[0], "MOTIONSET") == 0)
					{
						bMotion = true;
					}
					if (bMotion == true)
					{
						// MODEL_FILENAME����������X�t�@�C�������i�[
						if (strcmp(&aText[0], "LOOP") == 0)
						{
							int nNum = 0;
							fscanf(pFile, "%s %d", &aText[0], &nNum);
							if (nNum == 1)
							{
								m_aMotionInfo[0][nCntMotion].bLoop = true;
							}
						}
						// PARTSSET��&aText[0]�e�p�[�c�̏����i�[���鋖��^����
						if (strcmp(&aText[0], "NUM_KEY") == 0)
						{
							fscanf(pFile, "%s %d", &aText[0], &m_aMotionInfo[0][nCntMotion].nNumKey);
						}
						// PARTSSET��&aText[0]�e�p�[�c�̏����i�[���鋖��^����
						if (strcmp(&aText[0], "KEYSET") == 0 && nCntKeyNum < m_aMotionInfo[0][nCntMotion].nNumKey)
						{
							bSetKey = true;
						}
						if (bSetKey == true)
						{
							// PARENT����������X�t�@�C�������i�[
							if (strcmp(&aText[0], "FRAME") == 0)
							{
								fscanf(pFile, "%s %d", &aText[0], &m_aMotionInfo[0][nCntMotion].aKeyInfo[nCntKeyNum].nFrame);
							}
							// PARTSSET��&aText[0]�e�p�[�c�̏����i�[���鋖��^����
							if (strcmp(&aText[0], "KEY") == 0)
							{
								bKey = true;
							}
							if (bKey == true)
							{
								// POS������������W�����i�[
								if (strcmp(&aText[0], "POS") == 0)
								{
									fscanf(pFile, "%s %f %f %f", &aText[0],
										&m_aMotionInfo[0][nCntMotion].aKeyInfo[nCntKeyNum].aKey[nCntKey].fPosX,
										&m_aMotionInfo[0][nCntMotion].aKeyInfo[nCntKeyNum].aKey[nCntKey].fPosY,
										&m_aMotionInfo[0][nCntMotion].aKeyInfo[nCntKeyNum].aKey[nCntKey].fPosZ);
								}
								// ROT����������p�x�����i�[
								if (strcmp(&aText[0], "ROT") == 0)
								{
									fscanf(pFile, "%s %f %f %f", &aText[0],
										&m_aMotionInfo[0][nCntMotion].aKeyInfo[nCntKeyNum].aKey[nCntKey].fRotX,
										&m_aMotionInfo[0][nCntMotion].aKeyInfo[nCntKeyNum].aKey[nCntKey].fRotY,
										&m_aMotionInfo[0][nCntMotion].aKeyInfo[nCntKeyNum].aKey[nCntKey].fRotZ);
								}
								// END_SCRIPT����������ǂݍ��݂��I����
								if (strcmp(&aText[0], "END_KEY") == 0)
								{
									bKey = false;
									nCntKey++;
								}
							}
							// END_SCRIPT����������ǂݍ��݂��I����
							if (strcmp(&aText[0], "END_KEYSET") == 0)
							{
								bSetKey = false;
								nCntKey = 0;
								nCntKeyNum++;
							}
						}
						// END_SCRIPT����������ǂݍ��݂��I����
						if (strcmp(&aText[0], "END_MOTIONSET") == 0)
						{
							bMotion = false;
							nCntKeyNum = 0;
							nCntMotion++;
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
	}
}