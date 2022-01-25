//=============================================================================
//
// ラクガキ管理処理 [graffiti_manager.h]
// Author : 
//
//=============================================================================
#ifndef _GRAFFITIMANAGER_H_
#define _GRAFFITIMANAGER_H_

#include "scene.h"

//*****************************************************************************
// ラクガキ管理クラス(基本クラス)
//*****************************************************************************
class CGraffitiManager : public CScene
{
public:
	CGraffitiManager();
	~CGraffitiManager();

	HRESULT Init(D3DXVECTOR3 pos ,D3DXVECTOR2 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	int GetTotalGraffiti(void) { return m_nNumGraffiti; }
	void SetTotalGraffiti(int nAdd) { m_nNumGraffiti += nAdd; }
	int GetCurGraffiti(void) { return m_nNumCurGraffiti; }
	void SetCurGraffiti(int nAdd) { m_nNumCurGraffiti += nAdd; }

private:
	int m_nNumGraffiti;				// ラクガキの総数
	int m_nNumCurGraffiti;			// 現在のラクガキ数
};
#endif