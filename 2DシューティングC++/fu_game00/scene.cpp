//*****************************************************************************
//
// �V�[������ [scene.cpp]
// Author : SUZUKI FUUTA
//
//*****************************************************************************
//-----------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
//-----------------------------------------------------------------------------
#include "scene.h"
#include "scene2D.h"
#include "renderer.h"

//-----------------------------------------------------------------------------
// �ÓI�����o�ϐ�
//-----------------------------------------------------------------------------
CScene *CScene::m_apScene[OBJ_MAX][MAX_OBJECT] = {};
CScene *CScene::m_pPauseScene = NULL;
CScene *CScene::m_pPauseObj[PAUSE_MAX] = {};
int CScene::m_nNumAll[OBJ_MAX] = {};
bool CScene::m_bPause = false;

CScene *CScene::m_pTop[OBJ_MAX] = {};
CScene *CScene::m_pCur[OBJ_MAX] = {};

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CScene::CScene()
{
}

//=============================================================================
// �I�u�W�F�N�g�̃R���X�g���N�^
//=============================================================================
CScene::CScene(ObjType type)
{
	m_type = type;
	m_bDeath = false;
	if (m_pTop[m_type] != NULL)
	{
		m_pCur[m_type]->m_pNext = this; // �Ō������A�ǉ�����B
		this->m_pNext = NULL;			// ���g�̎���NULL�ł���B
		this->m_pPrev = m_pCur[m_type];	// ���g�̑O�͍��̍Ō���ł���B
	}
	else
	{
		m_pTop[m_type] = this;			// ���Ȃ��͐擪�ł��B
		this->m_pNext = NULL;			// ����NULL
		this->m_pPrev = NULL;			// �O��NULL
	}

	// ���g���Ō��
	m_pCur[m_type] = this;
	m_nNumAll[m_type]++;

}

//=============================================================================
// �|�[�Y�I�u�W�F�N�g�̃R���X�g���N�^
//=============================================================================
CScene::CScene(PauseType type)
{
	if (m_pPauseObj[type] == NULL)
	{// �|�[�Y�V�[���ɏ�������
		m_pPauseObj[type] = this;
	}
}

//=============================================================================
// �|�[�Y���}�p�̃R���X�g���N�^
//=============================================================================
CScene::CScene(bool bpause)
{
	if (m_pPauseScene == NULL)
	{// �|�[�Y���s�����}
		m_pPauseScene = this;
	}
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CScene::~CScene()
{
}

//=============================================================================
// ���ׂĂ̏I������
//=============================================================================
void CScene::ReleaseAll(void)
{
	for (int nCntType = 0; nCntType < OBJ_MAX; nCntType++)
	{
		CScene *pScene = m_pTop[nCntType];

		while (pScene)
		{
			CScene *pSceneNext = pScene->m_pNext;

			// �X�V����
			pScene->Uninit();

			pScene = pSceneNext;
		}

	}
	for (int nCntType = 0; nCntType < OBJ_MAX; nCntType++)
	{
		if (m_pTop[nCntType] != NULL)
		{
			CScene *pScene = m_pTop[nCntType];

			do
			{
				CScene *pSceneNext = pScene->m_pNext;
				if (pScene->m_bDeath == true)
				{
					pScene->DeathRelease();
					pScene = NULL;
				}
				pScene = pSceneNext;
			} while (pScene != NULL);
		}
	}
	if (m_pPauseScene != NULL)
	{
		for (int nCntPause = 0; nCntPause < PAUSE_MAX; nCntPause++)
		{
			if (m_pPauseObj[nCntPause] != NULL)
			{
				m_pPauseObj[nCntPause]->Uninit();
			}
		}
	}
}

//=============================================================================
// ���ׂĂ̍X�V����
//=============================================================================
void CScene::UpdateAll(void)
{
	if (m_bPause != true)
	{
		for (int nCntType = 0; nCntType < OBJ_MAX; nCntType++)
		{
			if (m_pTop[nCntType] != NULL)
			{
				CScene *pScene = m_pTop[nCntType];
				do
				{
					CScene *pSceneNext = pScene->m_pNext;

					if (pScene->m_bDeath != true)
					{
						// �X�V����
						pScene->Update();
					}
					pScene = pSceneNext;

				} while (pScene != NULL);
			}
		}
		for (int nCntType = 0; nCntType < OBJ_MAX; nCntType++)
		{
			if (m_pTop[nCntType] != NULL)
			{
				CScene *pScene = m_pTop[nCntType];
				do
				{
					CScene *pSceneNext = pScene->m_pNext;

					if (pScene->m_bDeath == true)
					{
						pScene->DeathRelease();
						pScene = NULL;
					}
					pScene = pSceneNext;

				} while (pScene != NULL);
			}
		}
	}
	if (m_bPause == true)
	{
		m_pPauseScene->Update();
		for (int nCntPause = 0; nCntPause < PAUSE_MAX; nCntPause++)
		{
			if (m_pPauseObj[nCntPause] != NULL)
			{
				m_pPauseObj[nCntPause]->Update();
			}
		}
	}
}

//=============================================================================
// ���ׂĂ̕`�揈��
//=============================================================================
void CScene::DrawAll(void)
{
	for (int nCntType = 0; nCntType < OBJ_MAX; nCntType++)
	{
		if (m_pTop[nCntType] != NULL)
		{
			CScene *pScene = m_pTop[nCntType];
			do
			{
				CScene *pSceneNext = pScene->m_pNext;
				if (pScene->m_bDeath != true)
				{
					// �X�V����
					pScene->Draw();
				}

				pScene = pSceneNext;
			} while (pScene != NULL);
		}
	}
	if (m_bPause == true)
	{
		for (int nCntPause = 0; nCntPause < PAUSE_MAX; nCntPause++)
		{
			if (m_pPauseObj[nCntPause] != NULL)
			{
				m_pPauseObj[nCntPause]->Draw();
			}
		}
	}
}

//=============================================================================
// �ʒu�̐ݒ�i����̃I�u�W�F�N�g�̈ʒu��m�邽�߁j
//=============================================================================
void CScene::SetPos(D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//=============================================================================
// �T�C�Y�̐ݒ�i����̃I�u�W�F�N�g�̃T�C�Y��m�邽�߁j
//=============================================================================
void CScene::SetSize(D3DXVECTOR3 size)
{
	m_size = size;
}

//=============================================================================
// �F�̐ݒ�i����̃I�u�W�F�N�g�̃T�C�Y��m�邽�߁j
//=============================================================================
void CScene::SetCol(D3DXCOLOR col)
{
	m_col = col;
}

//=============================================================================
// flag�̐ݒ�
//=============================================================================
void CScene::SetBool(bool bflag)
{
	m_bBool = bflag;
}

//=============================================================================
// ����̃I�u�W�F�N�g�̐����擾
//=============================================================================
int CScene::GetObjeNum(ObjType type)
{
	//int Num = 0;
	//for (int nCntObj = 0; nCntObj < MAX_OBJECT; nCntObj++)
	//{
	//	if (m_apScene[type][nCntObj] != NULL)
	//	{
	//		Num++;
	//	}
	//	else
	//	{
	//	}
	//}
	return m_nNumAll[type];
}

//=============================================================================
// ����̃I�u�W�F�N�g�̈ʒu���擾
//=============================================================================
D3DXVECTOR3 CScene::GetPos(ObjType type, int nID)
{
	D3DXVECTOR3 pos;
	if (m_apScene[type][nID] == NULL)
	{
		pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
	else if(m_apScene[type][nID] != NULL)
	{
		pos = m_apScene[type][nID]->m_pos;
	}
	
	return pos;
}

//=============================================================================
// ����̃I�u�W�F�N�g�T�C�Y���擾
//=============================================================================
D3DXVECTOR3 CScene::GetSize(ObjType type, int nID)
{
	D3DXVECTOR3 size;
	if (m_apScene[type][nID] == NULL)
	{
		size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
	else if(m_apScene[type][nID] != NULL)
	{
		size = m_apScene[type][nID]->m_size;
	}
	return size;
}

//=============================================================================
// ����̃I�u�W�F�N�g�F���擾
//=============================================================================
D3DXCOLOR CScene::GetCol(ObjType type, int nID)
{
	D3DXCOLOR col;
	if (m_apScene[type][nID] == NULL)
	{
		col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	}
	else if (m_apScene[type][nID] != NULL)
	{
		col = m_apScene[type][nID]->m_col;
	}
	return col;
}

D3DXVECTOR3 CScene::GetPos(void)
{
	return this->m_pos;
}

D3DXVECTOR3 CScene::GetSize(void)
{
	return this->m_size;
}

D3DXCOLOR CScene::GetCol(void)
{
	return this->m_col;
}

bool CScene::GetBool(void)
{
	return m_bBool;
}

CScene * CScene::GetSceneNext(void)
{
	CScene *pScene;

	if (this->m_pNext != NULL)
	{
		pScene = this->m_pNext;
	}
	else
	{
		pScene = NULL;
	}
	return pScene;
}

CScene * CScene::GetScene(ObjType type)
{
	return m_pTop[type];
}

//=============================================================================
// �V�[���̍폜����
//=============================================================================
void CScene::Release(void)
{
	m_bDeath = true;
}

//=============================================================================
// ���S�t���O�������Ă���V�[���̍폜
//=============================================================================
void CScene::DeathRelease(void)
{
	for (int nCntType = 0; nCntType < OBJ_MAX; nCntType++)
	{
		if (m_pTop[nCntType] == this)
		{
			m_pTop[nCntType] = this->m_pNext;
		}

		if (m_pCur[nCntType] == this)
		{
			m_pCur[nCntType] = this->m_pPrev;
		}

		if (this->m_pPrev != NULL)
		{
			this->m_pPrev->m_pNext = this->m_pNext;
		}

		if (this->m_pNext != NULL)
		{
			this->m_pNext->m_pPrev = this->m_pPrev;
		}
	}

	delete this;
}

//=============================================================================
// ����̃I�u�W�F�N�g�̌X�̍폜
//=============================================================================
void CScene::ObjRelease(ObjType type, int nID)
{
	//if (m_apScene[type][nID] != NULL)
	//{
	//	delete m_apScene[type][nID];
	//	m_apScene[type][nID] = NULL;
	//	m_nNumAll--;
	//}
}

//=============================================================================
// �|�[�Y�̍폜
//=============================================================================
void CScene::PauseRelease(void)
{
	if (m_pPauseScene != NULL)
	{
		for (int nCntPause = 0; nCntPause < PAUSE_MAX; nCntPause++)
		{
			if (m_pPauseObj[nCntPause] != NULL)
			{
				delete m_pPauseObj[nCntPause];
				m_pPauseObj[nCntPause] = NULL;
			}
		}
		delete m_pPauseScene;
		m_pPauseScene = NULL;
	}
}