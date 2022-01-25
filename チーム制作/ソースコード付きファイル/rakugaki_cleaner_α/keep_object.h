//=============================================================================
//
// データ保存用ヘッダー (keep_object.h)
// Author:Itsuki Takaiwa
//
//=============================================================================
#ifndef _KEEPOBJECT_H_
#define _KEEPOBJECT_H_

#include "main.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CLoadPlayerData;		// プレイヤーデータ読み込みクラス
class CMotion;				// モーションクラス

//*****************************************************************************
// データ保存用クラス(基本クラス)
//*****************************************************************************
class CKeepObject
{
public:
	// ゲームモード
	typedef enum
	{
		GAMEMODE_BATTLE = 0,
		GAMEMODE_COOPERATION,
		GAMEMODE_MAX,
	}GAMEMODE;

	CKeepObject();
	~CKeepObject();

	void Init(void);
	void Uninit(void);
	void Update(void);

	CLoadPlayerData *GetLoadPlayerData(void) { return m_pLoadPlayerData; }
	CMotion *GetMotionData(void) { return m_pMotionData; }
	int GetDeleteGraffiti(int nCnt) { return m_nNumDeleteGraffiti[nCnt]; }
	void SetDeleteGraffiti(int nNumDelete, int nCnt) { m_nNumDeleteGraffiti[nCnt] = nNumDelete; }
	int GetNumPlayer(void) { return m_nNumPlayer; }
	void SetNumPlayer(int nNumPlayer);
	int GetRank(int nCnt) { return m_nRank[nCnt]; }
	int GetNumRank(int nCnt) { return m_nNumberRank[nCnt]; }
	GAMEMODE GetGameMode(void) { return m_gamemode; }
	void SetGameMode(GAMEMODE gamemode) { m_gamemode = gamemode; }

	float GetAchivement(void) { return m_fNumAchivement; }
	void SetAchivement(float fNumAchivement) { m_fNumAchivement = fNumAchivement; }
	int GetTotalGraffiti(void) { return m_nNumTotalGraffiti;}
	void SetTotalGraffiti(int nTotal) { m_nNumTotalGraffiti = nTotal; }

	void DeleteMem(void);					// メモリの削除
	void Rank(void);						// ランキング確定
	void Achivement(void);					// 達成率の確定

private:
	CLoadPlayerData *m_pLoadPlayerData;		// プレイヤーデータ読み込み情報のポインタ
	CMotion *m_pMotionData;					// モーションデータ読み込み情報のポインタ
	int *m_nNumDeleteGraffiti;				// ラクガキを消した数の先頭アドレスを保存
	int m_nNumPlayer;						// プレイヤー人数の保存
	int *m_nRank;							// 順位を保存する先頭アドレスのポインタ
	int *m_nNumberRank;						// 順位を保存する先頭アドレスのポインタ
	GAMEMODE m_gamemode;					// ゲームモード保存用

	float m_fNumAchivement;
	int m_nNumTotalGraffiti;
};

#endif