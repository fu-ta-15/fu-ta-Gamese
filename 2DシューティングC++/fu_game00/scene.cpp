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
CScene *CScene::m_pPauseScene = NULL;
CScene *CScene::m_pPauseObj[PAUSE_MAX] = {};
CScene *CScene::m_pTop[OBJ_MAX] = {};
CScene *CScene::m_pCur[OBJ_MAX] = {};

//=============================================================================
// �I�u�W�F�N�g�̃R���X�g���N�^
//=============================================================================
CScene::CScene(Priority type)
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
	if (bpause == true && m_pPauseScene == NULL)
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
	if (m_pPauseScene == NULL)
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