//---------------------------------------------------------------- -
//
// データ読み込み (set_game.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "load_player_data.h"

#include "set_model.h"
#include "floor.h"
#include "skillgauge.h"
#include "scoregauge.h"
#include "Scene2D.h"
#include "timer.h"

#include "manager.h"
#include "base.h"
#include "game.h"
#include "player.h"
#include "texture.h"
#include "debugproc.h"

//-----------------------------------------------------------------
// 静的メンバ変数宣言
//-----------------------------------------------------------------
char CLoadPlayerData::m_apTexName[PLAYER_NAME_MAX][128] = {
"data/TEXT/PLAYER/player_chul.txt",
"data/TEXT/PLAYER/player_kano.txt" ,
"data/TEXT/PLAYER/player_rui.txt" ,
"data/TEXT/PLAYER/player_mahito.txt" };

//-----------------------------------------------------------------
// CLoadPlayerDataのコンストラクタ
//-----------------------------------------------------------------
CLoadPlayerData::CLoadPlayerData()
{

}

//-----------------------------------------------------------------
// CSetGameのデストラクタ
//-----------------------------------------------------------------
CLoadPlayerData::~CLoadPlayerData()
{

}

//-----------------------------------------------------------------
// 初期化処理
//-----------------------------------------------------------------
void CLoadPlayerData::Init(void)
{
	// メモリの確保
	m_player_data = new Player_Data[PLAYER_NAME_MAX];

	// プレイヤー情報の読み込み
	LoadPlayerData();
}

//-----------------------------------------------------------------
// 終了処理
//-----------------------------------------------------------------
void CLoadPlayerData::Uninit(void)
{
	// メモリの開放
	for (int nCnt = 0; nCnt < PLAYER_NAME_MAX; nCnt++)
	{
		delete[] m_player_data[nCnt].apModelFileName;
		m_player_data[nCnt].apModelFileName = NULL;

		delete[] m_player_data[nCnt].nIndexParent;
		m_player_data[nCnt].nIndexParent = NULL;

		delete[] m_player_data[nCnt].aOffset;
		m_player_data[nCnt].aOffset = NULL;
	}
	delete[] m_player_data;
	m_player_data = NULL;
}

//-----------------------------------------------------------------
// プレイヤー情報の読み込み
//-----------------------------------------------------------------
void CLoadPlayerData::LoadPlayerData(void)
{
	// FILE型のポインタ関数
	FILE *pFile;

	for (int nCnt = 0; nCnt < PLAYER_NAME_MAX; nCnt++)
	{
		// ファイルを開く
		pFile = fopen(&m_apTexName[nCnt][0], "r");

		// NULLチェック
		if (pFile != NULL)
		{
			// 変数宣言
			char aLine[128];
			char aText[128];
			bool bScript = false;
			bool bPartsSet = false;
			int nCntModel = 0;
			int nCntParts = 0;

			// SCRIPTを探す
			while (fgets(aLine, 128, pFile) != NULL)
			{
				// 毎行空白を見つけるまで読み込む
				fscanf(pFile, "%s", &aText[0]);

				// SCRIPTを見つけたらそれ以降を読み込む許可を与える
				if (strcmp(&aText[0], "SCRIPT") == 0)
				{
					bScript = true;
				}
				if (bScript == true)
				{
					// NUM_MODELを見つけたらモデル数を格納
					if (strcmp(&aText[0], "NUM_MODEL") == 0)
					{
						fscanf(pFile, "%s %d", &aText[0], &m_player_data[nCnt].nNumModel);

						// メモリの確保
						m_player_data[nCnt].apModelFileName = new char*[m_player_data[nCnt].nNumModel];
						for (int nCntModel = 0; nCntModel < m_player_data[nCnt].nNumModel; nCntModel++)
						{
							m_player_data[nCnt].apModelFileName[nCntModel] = new char[128];
						}
						m_player_data[nCnt].nIndexParent = new int[m_player_data[nCnt].nNumModel];

						m_player_data[nCnt].aOffset = new float*[m_player_data[nCnt].nNumModel];
						for (int nCntModel = 0; nCntModel < m_player_data[nCnt].nNumModel; nCntModel++)
						{
							m_player_data[nCnt].aOffset[nCntModel] = new float[6];
						}
					}
					// MODEL_FILENAMEを見つけたらXファイル名を格納
					if (strcmp(&aText[0], "MODEL_FILENAME") == 0 && nCntModel < m_player_data[nCnt].nNumModel)
					{
						fscanf(pFile, "%s %s", &aText[0], &m_player_data[nCnt].apModelFileName[nCntModel][0]);

						// モデルの総数だけ格納するのでカウントを増やす
						nCntModel++;
					}
				}
				// PARTSSETを&aText[0]各パーツの情報を格納する許可を与える
				if (strcmp(&aText[0], "PARTSSET") == 0)
				{
					bPartsSet = true;
				}
				// 各パーツの情報を格納する許可があるとき処理
				if (bPartsSet == true)
				{
					// PARENTを見つけたらXファイル名を格納
					if (strcmp(&aText[0], "PARENT") == 0)
					{
						fscanf(pFile, "%s %d", &aText[0], &m_player_data[nCnt].nIndexParent[nCntParts]);
					}
					// POSを見つけたら座標情報を格納
					if (strcmp(&aText[0], "POS") == 0)
					{
						fscanf(pFile, "%s %f %f %f", &aText[0],
							&m_player_data[nCnt].aOffset[nCntParts][0],
							&m_player_data[nCnt].aOffset[nCntParts][1],
							&m_player_data[nCnt].aOffset[nCntParts][2]);
					}
					// ROTを見つけたら角度情報を格納
					if (strcmp(&aText[0], "ROT") == 0)
					{
						fscanf(pFile, "%s %f %f %f", &aText[0],
							&m_player_data[nCnt].aOffset[nCntParts][3],
							&m_player_data[nCnt].aOffset[nCntParts][4],
							&m_player_data[nCnt].aOffset[nCntParts][5]);
					}
					// END_PARTSSETを見つけたら各パーツの情報を格納を終える
					if (strcmp(&aText[0], "END_PARTSSET") == 0)
					{
						bPartsSet = false;

						// パーツの総数だけ格納するのでカウントを増やす
						nCntParts++;
					}
					// END_SCRIPTを見つけたら読み込みを終える
					if (strcmp(&aText[0], "END_SCRIPT") == 0)
					{
						break;
					}
				}
			}
			// ファイルを閉じる
			fclose(pFile);
		}
		else
		{
		}
	}
}