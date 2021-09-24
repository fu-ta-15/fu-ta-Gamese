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
int CScene::m_nNumAll = 0;

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
	for (int nCnt = 0; nCnt < MAX_OBJECT; nCnt++)
	{
		if (m_apScene[type][nCnt] == NULL)
		{// ���g������ۂ̏ꍇ
			m_apScene[type][nCnt] = this;
			m_nID = nCnt;
			m_type = type;
			m_nNumAll++;
			break;
		}
	}
}

//=============================================================================
// �|�[�Y�I�u�W�F�N�g�̃R���X�g���N�^
//=============================================================================
CScene::CScene(PauseType type)
{
	if (m_pPauseObj[type] == NULL)
	{
		m_pPauseObj[type] = this;
	}
}

//=============================================================================
// �|�[�Y���}�p�̃R���X�g���N�^
//=============================================================================
CScene::CScene(bool bpause)
{
	if (bpause == true && m_pPauseScene == NULL)
	{
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
		for (int nCntScene = 0; nCntScene < MAX_OBJECT; nCntScene++)
		{
			if (m_apScene[nCntType][nCntScene] != NULL)
			{
				m_apScene[nCntType][nCntScene]->Uninit();
			}
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
	if (m_pPauseScene == NULL)
	{
		for (int nCntType = 0; nCntType < OBJ_MAX; nCntType++)
		{
			for (int nCntScene = 0; nCntScene < MAX_OBJECT; nCntScene++)
			{
				if (m_apScene[nCntType][nCntScene] != NULL)
				{
					m_apScene[nCntType][nCntScene]->Update();
				}
			}
		}
	}
	if (m_pPauseScene != NULL)
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
		for (int nCntScene = 0; nCntScene < MAX_OBJECT; nCntScene++)
		{
			if (m_apScene[nCntType][nCntScene] != NULL)
			{
				m_apScene[nCntType][nCntScene]->Draw();
			}
		}
	}
	if (m_pPauseScene != NULL)
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
// ����̃I�u�W�F�N�g�̐����擾
//=============================================================================
int CScene::GetObjeNum(ObjType type)
{
	int Num = 0;
	for (int nCntObj = 0; nCntObj < MAX_OBJECT; nCntObj++)
	{
		if (m_apScene[type][nCntObj] != NULL)
		{
			Num++;
		}
		else
		{

		}
	}
	return Num;
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

//=============================================================================
// �V�[���̍폜����
//=============================================================================
void CScene::Release(void)
{
	if (m_apScene[m_type][m_nID] != NULL)
	{
		int nID = m_nID;
		ObjType type = m_type;
		delete m_apScene[type][nID];
		m_apScene[type][nID] = NULL;
		m_nNumAll--;
	}
}

//=============================================================================
// ����̃I�u�W�F�N�g�̌X�̍폜
//=============================================================================
void CScene::ObjRelease(ObjType type, int nID)
{
	if (m_apScene[type][nID] != NULL)
	{
		delete m_apScene[type][nID];
		m_apScene[type][nID] = NULL;
		m_nNumAll--;
	}
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