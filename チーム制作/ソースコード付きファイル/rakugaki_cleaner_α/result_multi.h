//=============================================================================
//
// マルチモードのリザルト画面処理 [result_multi.h]
// Author : itsuki takaiwa
//
//=============================================================================
#ifndef _RESULTMULTI_H_
#define _RESULTMULTI_H_

#include "result.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CPlayer;			// スコアクラス
class CCamera;			// スコアクラス

//*****************************************************************************
// リザルト画面クラス(派生クラス)
//*****************************************************************************
class CResult : public CBase
{
public:
	CResult();
	~CResult();

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CPlayer **GetPlayer(void) { return m_pPlayer; }
	static CCamera **GetCamera(void) { return &m_pCamera[0]; }

	int GetNumPlayer(void) { return m_nNumPlayer; }

private:
	static CPlayer **m_pPlayer;						// プレイヤー情報のポインタ
	static CCamera **m_pCamera;						// カメラ情報を格納する先頭アドレスのポインタ

	int m_nNumPlayer;
};
#endif