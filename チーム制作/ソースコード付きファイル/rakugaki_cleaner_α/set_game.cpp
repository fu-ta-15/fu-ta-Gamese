//-----------------------------------------------------------------
//
// �f�[�^�ǂݍ��� (set_game.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "set_game.h"

#include "set_model.h"
#include "floor.h"
#include "skillgauge.h"
#include "scoregauge.h"
#include "graffiti_gauge.h"
#include "gauge_manager.h"
#include "Scene2D.h"
#include "timer.h"

#include "manager.h"
#include "base.h"
#include "game.h"
#include "player_game.h"
#include "texture.h"
#include "debugproc.h"

//-----------------------------------------------------------------
// CSetGame�̃R���X�g���N�^
//-----------------------------------------------------------------
CSetGame::CSetGame()
{

}

//-----------------------------------------------------------------
// CSetGame�̃f�X�g���N�^
//-----------------------------------------------------------------
CSetGame::~CSetGame()
{

}

//-----------------------------------------------------------------
// ����������
//-----------------------------------------------------------------
void CSetGame::Init(void)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR2(0.0f, 0.0f);
	m_nNumTex = -1;
	m_nCntSkillGauge = 0;
	m_nCntScoreGauge = 0;

	CSetModel *pSetModel = new CSetModel;

	// ���f���z�u���̓ǂݍ���
	pSetModel->LoadData();

	// �����̓ǂݍ���
	LoadFloor();

	// �Q�[�W���̓ǂݍ���
	LoadUIInfo();
}

//-----------------------------------------------------------------
// �X�V����
//-----------------------------------------------------------------
void CSetGame::Update(void)
{

}

//-----------------------------------------------------------------
// �����̓ǂݍ���
//-----------------------------------------------------------------
bool CSetGame::LoadFloor(void)
{
	// FILE�^�̃|�C���^�֐�
	FILE *pFile;

	// �t�@�C�����J��
	pFile = fopen("data/TEXT/04_game/floor.txt", "r");

	// NULL�`�F�b�N
	if (pFile != NULL)
	{
		// �ϐ��錾
		char aLine[128];
		char aText[128];
		bool bScript = false;
		bool bSetFloor = false;
		int nCntModel = 0;
		int nNumTexType = -1;
		D3DXVECTOR3 pos = { 0.0f,0.0f,0.0f };
		D3DXVECTOR2 size = { 0.0f,0.0f };
		D3DXVECTOR2 block = { 0.0f,0.0f };
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
				if (strcmp(&aText[0], "TEXTURE_FILENAME") == 0 && nCntModel < nNumTex)
				{
					fscanf(pFile, "%s %s", &aText[0], &apTextureFileName[nCntModel][0]);

					nCntModel++;
				}
				// PARTSSET��&aText[0]�e�p�[�c�̏����i�[���鋖��^����
				if (strcmp(&aText[0], "FLOORSET") == 0)
				{
					bSetFloor = true;
				}
				// �e�p�[�c�̏����i�[���鋖������Ƃ�����
				if (bSetFloor == true)
				{
					// POS������������W�����i�[
					if (strcmp(&aText[0], "TYPE") == 0)
					{
						fscanf(pFile, "%s %d", &aText[0], &nNumTexType);
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
						bSetFloor = false;

						if (nNumTexType < nNumTex)
						{
							// �e�N�X�`�����̎擾
							CTexture *pTex = CManager::GetTexture();

							if (pTex != NULL)
							{
								// ���f���̐���
								CFloor::Create(pos, size, block, pTex->SetTexture(&apTextureFileName[nNumTexType][0]));
							}
						}
					}
					// END_SCRIPT����������ǂݍ��݂��I����
					if (strcmp(&aText[0], "END_SCRIPT") == 0)
					{
						// �������̊J��
						delete[] apTextureFileName;
						apTextureFileName = NULL;

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
		return false;
	}
	return true;
}

//-----------------------------------------------------------------
// UI���̓ǂݍ���
//-----------------------------------------------------------------
bool CSetGame::LoadUIInfo(void)
{
	// FILE�^�̃|�C���^�֐�
	FILE *pFile;

	// �t�@�C�����J��
	pFile = fopen("data/TEXT/04_game/ui.txt", "r");

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
				// SKILLGAUGESET�̂Ƃ��X�L���Q�[�W�����i�[���鋖��^����
				if (strcmp(&aText[0], "SKILLGAUGESET") == 0)
				{
					LoadSkillGauge(pFile, nNumTex, pTextureFileName);
				}
				// SCOREGAUGESET�̂Ƃ��X�R�A�Q�[�W�����i�[���鋖��^����
				if (strcmp(&aText[0], "SCOREGAUGESET") == 0)
				{
					LoadScoreGauge(pFile, nNumTex, pTextureFileName);
				}
				// GRAFFITIGAUGESET�̂Ƃ����N�K�L�Q�[�W�����i�[���鋖��^����
				if (strcmp(&aText[0], "GRAFFITIGAUGESET") == 0)
				{
					LoadGraffitiGauge(pFile, nNumTex, pTextureFileName);
				}
				// PLAYERUISET�̂Ƃ��v���C���[UI�����i�[���鋖��^����
				if (strcmp(&aText[0], "PLAYERUISET") == 0)
				{
					LoadPlayerUI(pFile, nNumTex, pTextureFileName);
				}
				// ERASERSTATEUISET�̂Ƃ������S�����UI�����i�[���鋖��^����
				if (strcmp(&aText[0], "ERASERSTATEUISET") == 0)
				{
					LoadEraserStateUI(pFile, nNumTex, pTextureFileName);
				}
				// TIMERSET�̂Ƃ��^�C�}�[�����i�[���鋖��^����
				if (strcmp(&aText[0], "TIMERSET") == 0)
				{
					LoadTimer(pFile, nNumTex, pTextureFileName);
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

//-----------------------------------------------------------------
// �X�L���Q�[�W���̓ǂݍ���
//-----------------------------------------------------------------
bool CSetGame::LoadSkillGauge(FILE *pFile,int nNumTex,char **pTextureFileName)
{
	// �ϐ��錾
	char aLine[128];
	char aText[128];
	D3DXVECTOR3 pos = { 0.0f,0.0f,0.0f };
	D3DXVECTOR2 size = { 0.0f,0.0f };
	D3DXCOLOR col = { 1.0f,1.0f,1.0f,1.0f };

	while (fgets(aLine, 128, pFile) != NULL)
	{
		// ���s�󔒂�������܂œǂݍ���
		fscanf(pFile, "%s", &aText[0]);

		// POS������������W�����i�[
		if (strcmp(&aText[0], "GAUGE") == 0)
		{
			while (fgets(aLine, 128, pFile) != NULL)
			{
				// ���s�󔒂�������܂œǂݍ���
				fscanf(pFile, "%s", &aText[0]);

				// POS������������W�����i�[
				if (strcmp(&aText[0], "TYPE") == 0)
				{
					fscanf(pFile, "%s %d", &aText[0], &m_nNumTex);
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
				// BGSET����������w�i�����i�[
				if (strcmp(&aText[0], "BGSET") == 0)
				{
					LoadBg(pFile, nNumTex, pTextureFileName);
				}
				// END_PARTSSET����������e�p�[�c�̏����i�[���I����
				if (strcmp(&aText[0], "END_GAUGE") == 0)
				{
					if (m_nNumTex < nNumTex)
					{
						// �e�N�X�`�����̎擾
						CTexture *pTex = CManager::GetTexture();

						if (pTex != NULL)
						{
							// �v���C���[�����擾
							CBase *pBase = CManager::GetBase();

							if (pBase != NULL)
							{
								CGame *pGame = (CGame*)pBase;
								int nNumPlayer = pGame->GetNumPlayer();

								if (nNumPlayer > m_nCntSkillGauge)
								{
									CPlayer_Game **pPlayer = pGame->GetPlayer();
									
									// �X�L���Q�[�W�̐���
									CSkillGauge *pGauge = CSkillGauge::Create(pos, size, pTex->SetTexture(&pTextureFileName[m_nNumTex][0]));
									pGauge->SetCol(col);
									pPlayer[m_nCntSkillGauge]->SetSkillGauge(pGauge);

									m_nCntSkillGauge++;
								}
							}

							break;
						}
					}
				}
			}
		}
		// FRAME����������g�����i�[
		if (strcmp(&aText[0], "FRAME") == 0)
		{
			LoadGaugeFrame(pFile, nNumTex, pTextureFileName);

			return true;
		}
	}
	return false;
}

//-----------------------------------------------------------------
// �X�R�A�Q�[�W���̓ǂݍ���
//-----------------------------------------------------------------
bool CSetGame::LoadScoreGauge(FILE *pFile, int nNumTex, char **pTextureFileName)
{
	// �ϐ��錾
	char aLine[128];
	char aText[128];
	D3DXVECTOR3 pos = { 0.0f,0.0f,0.0f };
	D3DXVECTOR2 size = { 0.0f,0.0f };
	D3DXCOLOR col = { 1.0f,1.0f,1.0f,1.0f };

	while (fgets(aLine, 128, pFile) != NULL)
	{
		// ���s�󔒂�������܂œǂݍ���
		fscanf(pFile, "%s", &aText[0]);

		// POS������������W�����i�[
		if (strcmp(&aText[0], "GAUGE") == 0)
		{
			while (fgets(aLine, 128, pFile) != NULL)
			{
				// ���s�󔒂�������܂œǂݍ���
				fscanf(pFile, "%s", &aText[0]);

				// POS������������W�����i�[
				if (strcmp(&aText[0], "TYPE") == 0)
				{
					fscanf(pFile, "%s %d", &aText[0], &m_nNumTex);
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
				// BGSET����������w�i�����i�[
				if (strcmp(&aText[0], "BGSET") == 0)
				{
					LoadBg(pFile, nNumTex, pTextureFileName);
				}
				// END_PARTSSET����������e�p�[�c�̏����i�[���I����
				if (strcmp(&aText[0], "END_GAUGE") == 0)
				{
					if (m_nNumTex < nNumTex)
					{
						// �e�N�X�`�����̎擾
						CTexture *pTex = CManager::GetTexture();

						if (pTex != NULL)
						{
							// �v���C���[�����擾
							CBase *pBase = CManager::GetBase();

							if (pBase != NULL)
							{
								CGame *pGame = (CGame*)pBase;
								int nNumPlayer = pGame->GetNumPlayer();

								if (nNumPlayer > m_nCntScoreGauge)
								{
									CPlayer_Game **pPlayer = pGame->GetPlayer();

									// �X�L���Q�[�W�̐���
									CScoreGauge *pGauge = CScoreGauge::Create(pos, size, pTex->SetTexture(&pTextureFileName[m_nNumTex][0]));
									pGauge->SetCol(col);
									pPlayer[m_nCntScoreGauge]->SetScoreGauge(pGauge);

									m_nCntScoreGauge++;
								}
							}
							break;
						}
					}
				}
			}
		}
		// FRAME����������g�����i�[
		if (strcmp(&aText[0], "FRAME") == 0)
		{
			LoadGaugeFrame(pFile, nNumTex, pTextureFileName);
			
			return true;
		}
	}
	return false;
}

//-----------------------------------------------------------------
// ���N�K�L�Q�[�W���̓ǂݍ���
//-----------------------------------------------------------------
bool CSetGame::LoadGraffitiGauge(FILE *pFile, int nNumTex, char **pTextureFileName)
{
	// �ϐ��錾
	char aLine[128];
	char aText[128];
	D3DXVECTOR3 pos = { 0.0f,0.0f,0.0f };
	D3DXVECTOR2 size = { 0.0f,0.0f };
	D3DXCOLOR col = { 1.0f,1.0f,1.0f,1.0f };

	while (fgets(aLine, 128, pFile) != NULL)
	{
		// ���s�󔒂�������܂œǂݍ���
		fscanf(pFile, "%s", &aText[0]);

		// POS������������W�����i�[
		if (strcmp(&aText[0], "GAUGE") == 0)
		{
			while (fgets(aLine, 128, pFile) != NULL)
			{
				// ���s�󔒂�������܂œǂݍ���
				fscanf(pFile, "%s", &aText[0]);

				// POS������������W�����i�[
				if (strcmp(&aText[0], "TYPE") == 0)
				{
					fscanf(pFile, "%s %d", &aText[0], &m_nNumTex);
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
				// BGSET����������w�i�����i�[
				if (strcmp(&aText[0], "BGSET") == 0)
				{
					LoadBg(pFile, nNumTex, pTextureFileName);
				}
				// END_PARTSSET����������e�p�[�c�̏����i�[���I����
				if (strcmp(&aText[0], "END_GAUGE") == 0)
				{
					if (m_nNumTex < nNumTex)
					{
						// �e�N�X�`�����̎擾
						CTexture *pTex = CManager::GetTexture();

						if (pTex != NULL)
						{
							// �X�L���Q�[�W�̐���
							CGraffitiGauge *pGauge = CGraffitiGauge::Create(pos, size, pTex->SetTexture(&pTextureFileName[m_nNumTex][0]));
							pGauge->SetCol(col);

							CBase *pBase = CManager::GetBase();

							if (pBase != NULL)
							{
								CGame *pGame = (CGame*)pBase;
								pGame->GetGaugeManager()->SetGraffitiGauge(pGauge);
							}

							break;
						}
					}
				}
			}
		}
		// FRAME����������g�����i�[
		if (strcmp(&aText[0], "FRAME") == 0)
		{
			LoadGaugeFrame(pFile, nNumTex, pTextureFileName);

			return true;
		}
	}
	return false;
}

//-----------------------------------------------------------------
// �Q�[�W�̘g���̓ǂݍ���
//-----------------------------------------------------------------
bool CSetGame::LoadGaugeFrame(FILE *pFile, int nNumTex, char **pTextureFileName)
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
		if (strcmp(&aText[0], "TYPE") == 0)
		{
			fscanf(pFile, "%s %d", &aText[0], &m_nNumTex);
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
		// END_PARTSSET����������e�p�[�c�̏����i�[���I����
		if (strcmp(&aText[0], "END_FRAME") == 0)
		{
			if (m_nNumTex < nNumTex)
			{
				// �e�N�X�`�����̎擾
				CTexture *pTex = CManager::GetTexture();

				if (pTex != NULL)
				{
					// �e�N�X�`���̐���
					CScene2D::Create(pos, D3DXVECTOR3(size.x, size.y, 0.0f), CScene::PRIORITY_UI, pTex->SetTexture(&pTextureFileName[m_nNumTex][0]));
				}
			}
			return true;
		}
	}
	return false;
}

//-----------------------------------------------------------------
// �v���C���[UI���̓ǂݍ���
//-----------------------------------------------------------------
bool CSetGame::LoadPlayerUI(FILE *pFile, int nNumTex, char **pTextureFileName)
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
		// END_PLAYERUISET����������v���C���[UI�̏����i�[���I����
		if (strcmp(&aText[0], "END_PLAYERUISET") == 0)
		{
			if (m_nNumTex < nNumTex)
			{
				// �e�N�X�`�����̎擾
				CTexture *pTex = CManager::GetTexture();

				if (pTex != NULL)
				{
					// �e�N�X�`���̐���
					CScene2D::Create(pos, D3DXVECTOR3(size.x, size.y, 0.0f), CScene::PRIORITY_UI, pTex->SetTexture(&pTextureFileName[nTex][0]));
				}
			}
			return true;
		}
	}
	return false;
}

//-----------------------------------------------------------------
// �����S�����UI���̓ǂݍ���
//-----------------------------------------------------------------
bool CSetGame::LoadEraserStateUI(FILE *pFile, int nNumTex, char **pTextureFileName)
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
		// END_ERASERSTATEUISET��������������S�����UI�̏����i�[���I����
		if (strcmp(&aText[0], "END_ERASERSTATEUISET") == 0)
		{
			if (m_nNumTex < nNumTex)
			{
				// �e�N�X�`�����̎擾
				CTexture *pTex = CManager::GetTexture();

				if (pTex != NULL)
				{
					// �e�N�X�`���̐���
					CScene2D::Create(pos, D3DXVECTOR3(size.x, size.y, 0.0f), CScene::PRIORITY_UI, pTex->SetTexture(&pTextureFileName[nTex][0]));
				}
			}
			return true;
		}
	}
	return false;
}

//-----------------------------------------------------------------
// �^�C�}�[���̓ǂݍ���
//-----------------------------------------------------------------
bool CSetGame::LoadTimer(FILE *pFile, int nNumTex, char **pTextureFileName)
{
	// �ϐ��錾
	char aLine[128];
	char aText[128];
	int nTex;
	D3DXVECTOR3 pos = { 0.0f,0.0f,0.0f };
	D3DXVECTOR2 size = { 0.0f,0.0f };
	int nDigit;
	float fSpace;
	int nTime;

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
			fscanf(pFile, "%s %d", &aText[0],&nDigit);
		}
		// SPACE����������Ԋu�����i�[
		if (strcmp(&aText[0], "SPACE") == 0)
		{
			fscanf(pFile, "%s %f", &aText[0], &fSpace);
		}
		// TIME���������琧�����ԏ����i�[
		if (strcmp(&aText[0], "TIME") == 0)
		{
			fscanf(pFile, "%s %d", &aText[0], &nTime);
		}
		// BGSET����������w�i�����i�[
		if (strcmp(&aText[0], "BGSET") == 0)
		{
			LoadBg(pFile, nNumTex, pTextureFileName);
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
					// �J�E���g�_�E�������擾
					CBase *pBase = CManager::GetBase();

					if (pBase != NULL)
					{
						CGame *pGame = (CGame*)pBase;

						// �^�C�}�[�̐���
						pGame->SetTimer(CTimer::Create(pos, size, nDigit, fSpace, nTime, pTex->SetTexture(&pTextureFileName[nTex][0])));
					}
					else
					{
						// �^�C�}�[�̐���
						CTimer::Create(pos, size, nDigit, fSpace, nTime, pTex->SetTexture(&pTextureFileName[nTex][0]));
					}
				}
			}
			return true;
		}
	}
	return false;
}

//-----------------------------------------------------------------
// �e��w�i���̓ǂݍ���
//-----------------------------------------------------------------
bool CSetGame::LoadBg(FILE *pFile, int nNumTex, char **pTextureFileName)
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
		// END_BGSET����������w�i�����i�[���I����
		if (strcmp(&aText[0], "END_BGSET") == 0)
		{
			if (nTex < nNumTex)
			{
				// �e�N�X�`�����̎擾
				CTexture *pTex = CManager::GetTexture();

				if (pTex != NULL)
				{
					// �e�N�X�`���̐���
					CScene2D::Create(pos, D3DXVECTOR3(size.x, size.y, 0.0f), CScene::PRIORITY_UI, pTex->SetTexture(&pTextureFileName[nTex][0]));
				}
			}
			return true;
		}
	}
	return false;
}