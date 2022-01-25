//=============================================================================
//
// プレイヤー準備画面管理処理 [preparation_player_manager.h]
// Author : itsuki takaiwa
//
//=============================================================================
#ifndef _PREPARATIONPLAYERMANAGER_H_
#define _PREPARATIONPLAYERMANAGER_H_

#include "scene.h"

//*****************************************************************************
// プレイヤー待機画面クラス(派生クラス)
//*****************************************************************************
class CPressLogo;			// プレスロゴクラス
class CScene2D;				// Scene2Dクラス

//*****************************************************************************
// プレイヤー待機画面管理クラス(派生クラス)
//*****************************************************************************
class CPreparationPlayerManager : public CScene
{
public:
	CPreparationPlayerManager();
	~CPreparationPlayerManager();

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	CPressLogo *GetPressLogo(void) { return m_pPressLogo; }
	void SetPressLogo(CPressLogo *pPressLogo) { m_pPressLogo = pPressLogo; }
	CScene2D *GetFrame(int nCnt) { return m_pFrame[nCnt]; }
	void SetFrame(CScene2D *pFrame,int nCnt) { m_pFrame[nCnt] = pFrame; }
	bool GetTransition(void) { return m_bTransition; }

private:
	typedef struct
	{
		bool bJoin;		// 参加しているかどうか
		bool bReady;	// 準備できたかどうか
	}LogoInfo;

	void Transition(void);				// 遷移
	void InitCharaSelect(void);			// キャラ選択初期化
	void CharaSelect(void);				// キャラ選択

	LogoInfo m_logoInfo[4];				// ロゴの表示情報
	CPressLogo *m_pPressLogo;			// プレスロゴ情報のポインタ
	bool m_bPlayerReadyOK;				// 準備できているかどうか
	bool m_bOK;
	bool m_bTransition;					// 遷移するかどうか

	CScene2D *m_pFrame[4];				// フレーム
	CScene2D *m_pJoinLogo[4];			// 参加ボタンロゴ
	CScene2D *m_pCompLogo[4];			// 準備完了ロゴ
	CScene2D *m_pOkLogo[4];				// OKロゴ
};
#endif