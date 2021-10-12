//*****************************************************************************
//
// シーン処理 [scene.cpp]
// Author : SUZUKI FUUTA
//
//*****************************************************************************
//-----------------------------------------------------------------------------
// インクルードファイル
//-----------------------------------------------------------------------------
#include "scene.h"
#include "scene2D.h"
#include "renderer.h"

//-----------------------------------------------------------------------------
// 静的メンバ変数
//-----------------------------------------------------------------------------
CScene *CScene::m_pPauseScene = NULL;
CScene *CScene::m_pPauseObj[PAUSE_MAX] = {};
CScene *CScene::m_pTop[OBJ_MAX] = {};
CScene *CScene::m_pCur[OBJ_MAX] = {};

//=============================================================================
// オブジェクトのコンストラクタ
//=============================================================================
CScene::CScene(Priority type)
{
	m_type = type;
	m_bDeath = false;
	if (m_pTop[m_type] != NULL)
	{
		m_pCur[m_type]->m_pNext = this; // 最後尾から、追加する。
		this->m_pNext = NULL;			// 自身の次はNULLである。
		this->m_pPrev = m_pCur[m_type];	// 自身の前は今の最後尾である。
	}
	else
	{
		m_pTop[m_type] = this;			// あなたは先頭です。
		this->m_pNext = NULL;			// 次はNULL
		this->m_pPrev = NULL;			// 前はNULL
	}

	// 自身を最後尾
	m_pCur[m_type] = this;

}

//=============================================================================
// ポーズオブジェクトのコンストラクタ
//=============================================================================
CScene::CScene(PauseType type)
{
	if (m_pPauseObj[type] == NULL)
	{// ポーズシーンに情報を入れる
		m_pPauseObj[type] = this;
	}
}

//=============================================================================
// ポーズ合図用のコンストラクタ
//=============================================================================
CScene::CScene(bool bpause)
{
	if (bpause == true && m_pPauseScene == NULL)
	{// ポーズを行う合図
		m_pPauseScene = this;
	}
}

//=============================================================================
// デストラクタ
//=============================================================================
CScene::~CScene()
{
}

//=============================================================================
// すべての終了処理
//=============================================================================
void CScene::ReleaseAll(void)
{
	for (int nCntType = 0; nCntType < OBJ_MAX; nCntType++)
	{
		CScene *pScene = m_pTop[nCntType];

		while (pScene)
		{
			CScene *pSceneNext = pScene->m_pNext;

			// 更新処理
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
// すべての更新処理
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
						// 更新処理
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
// すべての描画処理
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
					// 更新処理
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
// シーンの削除処理
//=============================================================================
void CScene::Release(void)
{
	m_bDeath = true;
}

//=============================================================================
// 死亡フラグが立っているシーンの削除
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
// ポーズの削除
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