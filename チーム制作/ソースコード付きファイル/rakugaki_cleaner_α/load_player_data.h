//-------------------------------------------------------------------
//
// データ読み込みヘッダー (set_game.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _LOADPLAYERDETA_H_
#define _LOADPLAYERDETA_H_

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#include "main.h"

//-------------------------------------------------------------------
// プレイヤーデータ読み込みクラス(基本クラス)
//-------------------------------------------------------------------
class CLoadPlayerData
{
public:
	// プレイヤーの種類
	typedef enum
	{
		PLAYER_NAME_CHUL = 0,
		PLAYER_NAME_KANO,
		PLAYER_NAME_RUI,
		PLAYER_NAME_MAHITO,
		PLAYER_NAME_MAX,
	}PLayer_Name;

	// プレイヤーモデル情報
	typedef struct
	{
		int *nIndexParent;				// 親モデルのINDEX
		float **aOffset;				// 親モデルからのオフセット
		char **apModelFileName;			// モデルファイルネーム
		int nNumModel;					// モデル数
	}Player_Data;

	CLoadPlayerData();
	~CLoadPlayerData();

	void Init(void);
	void Uninit(void);
	Player_Data GetPlayerData(int nIdx) { return m_player_data[nIdx]; }

private:
	void LoadPlayerData(void);

	static char m_apTexName[PLAYER_NAME_MAX][128];		// 読み込むテキスト名
	Player_Data *m_player_data;					// プレイヤーデータ情報のポインタ
};

#endif