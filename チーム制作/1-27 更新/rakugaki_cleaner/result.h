//=============================================================================
//
// リザルト画面処理 [result.h]
// Author : itsuki takaiwa
//
//=============================================================================
#ifndef _RESULT_H_
#define _RESULT_H_

#include "base.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CPlayer_Result;			// スコアクラス
class CCamera;			// スコアクラス
class CResultManager;
class CScore;			// スコアクラス
class CNumber;			// ナンバークラス
class CScene2D;			// Cscene2Dクラス
class CResult_UI;		// リザルトUIクラス

//*****************************************************************************
// リザルト画面クラス(派生クラス)
//*****************************************************************************
class CResult : public CBase
{
public:
	// モード
	typedef enum
	{
		RESULTMODE_NORMAL = 0,
		RESULTMODE_RESTART,
		RESULTMODE_TRANSITION_MODESELECT,
		RESULTMODE_TRANSITION_TITLE,
		RESULTMODE_MAX,
	}ResultMode;

	CResult();
	~CResult();

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CPlayer_Result **GetPlayer(void) { return m_pPlayer; }
	static CCamera *GetCamera(void) { return m_pCamera; }

	int GetNumPlayer(void) { return m_nNumPlayer; }
	static void SetResultMode(ResultMode resultMode) { m_resultMode = resultMode; }

private:
	static CPlayer_Result **m_pPlayer;						// プレイヤー情報のポインタ
	static CCamera *m_pCamera;						// カメラ情報を格納する先頭アドレスのポインタ
	static ResultMode m_resultMode;
	CResultManager *m_pResultManager;
	int m_nNumPlayer;

	CResult_UI *m_pResult_UI;
};
#endif