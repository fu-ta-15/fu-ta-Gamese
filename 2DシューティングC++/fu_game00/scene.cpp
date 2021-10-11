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
CScene *CScene::m_apScene[OBJ_MAX][MAX_OBJECT] = {};
CScene *CScene::m_pPauseScene = NULL;
CScene *CScene::m_pPauseObj[PAUSE_MAX] = {};
int CScene::m_nNumAll[OBJ_MAX] = {};
bool CScene::m_bPause = false;

CScene *CScene::m_pTop[OBJ_MAX] = {};
CScene *CScene::m_pCur[OBJ_MAX] = {};

//=============================================================================
// コンストラクタ
//=============================================================================
CScene::CScene()
{
}

//=============================================================================
// オブジェクトのコンストラクタ
//=============================================================================
CScene::CScene(ObjType type)
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
	m_nNumAll[m_type]++;

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
	if (m_pPauseScene == NULL)
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
// 位置の設定（特定のオブジェクトの位置を知るため）
//=============================================================================
void CScene::SetPos(D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//=============================================================================
// サイズの設定（特定のオブジェクトのサイズを知るため）
//=============================================================================
void CScene::SetSize(D3DXVECTOR3 size)
{
	m_size = size;
}

//=============================================================================
// 色の設定（特定のオブジェクトのサイズを知るため）
//=============================================================================
void CScene::SetCol(D3DXCOLOR col)
{
	m_col = col;
}

//=============================================================================
// flagの設定
//=============================================================================
void CScene::SetBool(bool bflag)
{
	m_bBool = bflag;
}

//=============================================================================
// 特定のオブジェクトの数を取得
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
// 特定のオブジェクトの位置を取得
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
// 特定のオブジェクトサイズを取得
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
// 特定のオブジェクト色を取得
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
// 特定のオブジェクトの個々の削除
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