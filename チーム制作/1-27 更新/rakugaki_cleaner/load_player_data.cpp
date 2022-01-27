//---------------------------------------------------------------- -
//
// �f�[�^�ǂݍ��� (set_game.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "load_player_data.h"

#include "set_model.h"
#include "floor.h"
#include "skillgauge.h"
#include "scoregauge.h"
#include "Scene2D.h"
#include "timer.h"

#include "manager.h"
#include "base.h"
#include "game.h"
#include "player.h"
#include "texture.h"
#include "debugproc.h"

//-----------------------------------------------------------------
// �ÓI�����o�ϐ��錾
//-----------------------------------------------------------------
char CLoadPlayerData::m_apTexName[PLAYER_NAME_MAX][128] = {
"data/TEXT/PLAYER/player_chul.txt",
"data/TEXT/PLAYER/player_kano.txt" ,
"data/TEXT/PLAYER/player_rui.txt" ,
"data/TEXT/PLAYER/player_mahito.txt" };

//-----------------------------------------------------------------
// CLoadPlayerData�̃R���X�g���N�^
//-----------------------------------------------------------------
CLoadPlayerData::CLoadPlayerData()
{

}

//-----------------------------------------------------------------
// CSetGame�̃f�X�g���N�^
//-----------------------------------------------------------------
CLoadPlayerData::~CLoadPlayerData()
{

}

//-----------------------------------------------------------------
// ����������
//-----------------------------------------------------------------
void CLoadPlayerData::Init(void)
{
	// �������̊m��
	m_player_data = new Player_Data[PLAYER_NAME_MAX];

	// �v���C���[���̓ǂݍ���
	LoadPlayerData();
}

//-----------------------------------------------------------------
// �I������
//-----------------------------------------------------------------
void CLoadPlayerData::Uninit(void)
{
	// �������̊J��
	for (int nCnt = 0; nCnt < PLAYER_NAME_MAX; nCnt++)
	{
		delete[] m_player_data[nCnt].apModelFileName;
		m_player_data[nCnt].apModelFileName = NULL;

		delete[] m_player_data[nCnt].nIndexParent;
		m_player_data[nCnt].nIndexParent = NULL;

		delete[] m_player_data[nCnt].aOffset;
		m_player_data[nCnt].aOffset = NULL;
	}
	delete[] m_player_data;
	m_player_data = NULL;
}

//-----------------------------------------------------------------
// �v���C���[���̓ǂݍ���
//-----------------------------------------------------------------
void CLoadPlayerData::LoadPlayerData(void)
{
	// FILE�^�̃|�C���^�֐�
	FILE *pFile;

	for (int nCnt = 0; nCnt < PLAYER_NAME_MAX; nCnt++)
	{
		// �t�@�C�����J��
		pFile = fopen(&m_apTexName[nCnt][0], "r");

		// NULL�`�F�b�N
		if (pFile != NULL)
		{
			// �ϐ��錾
			char aLine[128];
			char aText[128];
			bool bScript = false;
			bool bPartsSet = false;
			int nCntModel = 0;
			int nCntParts = 0;

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
					// NUM_MODEL���������烂�f�������i�[
					if (strcmp(&aText[0], "NUM_MODEL") == 0)
					{
						fscanf(pFile, "%s %d", &aText[0], &m_player_data[nCnt].nNumModel);

						// �������̊m��
						m_player_data[nCnt].apModelFileName = new char*[m_player_data[nCnt].nNumModel];
						for (int nCntModel = 0; nCntModel < m_player_data[nCnt].nNumModel; nCntModel++)
						{
							m_player_data[nCnt].apModelFileName[nCntModel] = new char[128];
						}
						m_player_data[nCnt].nIndexParent = new int[m_player_data[nCnt].nNumModel];

						m_player_data[nCnt].aOffset = new float*[m_player_data[nCnt].nNumModel];
						for (int nCntModel = 0; nCntModel < m_player_data[nCnt].nNumModel; nCntModel++)
						{
							m_player_data[nCnt].aOffset[nCntModel] = new float[6];
						}
					}
					// MODEL_FILENAME����������X�t�@�C�������i�[
					if (strcmp(&aText[0], "MODEL_FILENAME") == 0 && nCntModel < m_player_data[nCnt].nNumModel)
					{
						fscanf(pFile, "%s %s", &aText[0], &m_player_data[nCnt].apModelFileName[nCntModel][0]);

						// ���f���̑��������i�[����̂ŃJ�E���g�𑝂₷
						nCntModel++;
					}
				}
				// PARTSSET��&aText[0]�e�p�[�c�̏����i�[���鋖��^����
				if (strcmp(&aText[0], "PARTSSET") == 0)
				{
					bPartsSet = true;
				}
				// �e�p�[�c�̏����i�[���鋖������Ƃ�����
				if (bPartsSet == true)
				{
					// PARENT����������X�t�@�C�������i�[
					if (strcmp(&aText[0], "PARENT") == 0)
					{
						fscanf(pFile, "%s %d", &aText[0], &m_player_data[nCnt].nIndexParent[nCntParts]);
					}
					// POS������������W�����i�[
					if (strcmp(&aText[0], "POS") == 0)
					{
						fscanf(pFile, "%s %f %f %f", &aText[0],
							&m_player_data[nCnt].aOffset[nCntParts][0],
							&m_player_data[nCnt].aOffset[nCntParts][1],
							&m_player_data[nCnt].aOffset[nCntParts][2]);
					}
					// ROT����������p�x�����i�[
					if (strcmp(&aText[0], "ROT") == 0)
					{
						fscanf(pFile, "%s %f %f %f", &aText[0],
							&m_player_data[nCnt].aOffset[nCntParts][3],
							&m_player_data[nCnt].aOffset[nCntParts][4],
							&m_player_data[nCnt].aOffset[nCntParts][5]);
					}
					// END_PARTSSET����������e�p�[�c�̏����i�[���I����
					if (strcmp(&aText[0], "END_PARTSSET") == 0)
					{
						bPartsSet = false;

						// �p�[�c�̑��������i�[����̂ŃJ�E���g�𑝂₷
						nCntParts++;
					}
					// END_SCRIPT����������ǂݍ��݂��I����
					if (strcmp(&aText[0], "END_SCRIPT") == 0)
					{
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
}