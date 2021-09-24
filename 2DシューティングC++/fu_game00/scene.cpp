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
int CScene::m_nNumAll = 0;

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
	for (int nCnt = 0; nCnt < MAX_OBJECT; nCnt++)
	{
		if (m_apScene[type][nCnt] == NULL)
		{// 中身が空っぽの場合
			m_apScene[type][nCnt] = this;
			m_nID = nCnt;
			m_type = type;
			m_nNumAll++;
			break;
		}
	}
}

//=============================================================================
// ポーズオブジェクトのコンストラクタ
//=============================================================================
CScene::CScene(PauseType type)
{
	if (m_pPauseObj[type] == NULL)
	{
		m_pPauseObj[type] = this;
	}
}

//=============================================================================
// ポーズ合図用のコンストラクタ
//=============================================================================
CScene::CScene(bool bpause)
{
	if (bpause == true && m_pPauseScene == NULL)
	{
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
// すべての更新処理
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
// すべての描画処理
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
// 特定のオブジェクトの数を取得
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

//=============================================================================
// シーンの削除処理
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
// 特定のオブジェクトの個々の削除
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