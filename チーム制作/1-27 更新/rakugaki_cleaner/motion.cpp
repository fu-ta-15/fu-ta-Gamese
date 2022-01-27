//=============================================================================
//
// モーション [motion.cpp]
// Author : 
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#include "motion.h"
#include "manager.h"
#include "renderer.h"
#include "player.h"
#include "game.h"

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
char CMotion::m_aTextMotion[MOTIONTYPE_MAX][128] = {"data/TEXT/motion.txt"};
CMotion::MOTION_INFO CMotion::m_aMotionInfo[MOTIONTYPE_MAX][10] = {};

//=============================================================================
// CMotionのコンストラクタ
//=============================================================================
CMotion::CMotion()
{
	// メンバ変数の初期化
	m_motion.nCounterMotion = 0;
	m_motion.nKey = 0;
}

//=============================================================================
// CMotionのデストラクタ
//=============================================================================
CMotion::~CMotion()
{

}

//=============================================================================
// 生成処理
//=============================================================================
CMotion *CMotion::Create(void)
{
	// ポインタ変数の生成
	CMotion *pMotion;
	pMotion = new CMotion;

	// NULLチェック
	if (pMotion != NULL)
	{
		// 初期化処理
		pMotion->Init();
	}

	return pMotion;
}

//=============================================================================
// 初期化処理
//=============================================================================
void CMotion::Init(void)
{
	// メンバ変数の初期化
	m_motion.nCounterMotion = 0;
	m_motion.nKey = 0;
}

//=============================================================================
// モーション情報読み込み
//=============================================================================
void CMotion::LoadMotionData(void)
{
	// FILE型のポインタ関数
	FILE *pFile;

	for (int nCntText = 0; nCntText < MOTIONTYPE_MAX; nCntText++)
	{
		// ファイルを開く
		pFile = fopen(&m_aTextMotion[nCntText][0],"r");

		// NULLチェック
		if (pFile != NULL)
		{
			// 変数宣言
			char aLine[128];
			char aText[128];
			bool bScript = false;
			bool bMotion = false;
			bool bSetKey = false;
			bool bKey = false;
			int nCntKey = 0;
			int nCntKeyNum = 0;
			int nCntMotion = 0;
			int nNumKey = 0;

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
					// MODEL_FILENAMEを見つけたらXファイル名を格納
					if (strcmp(&aText[0], "MOTIONSET") == 0)
					{
						bMotion = true;
					}
					if (bMotion == true)
					{
						// MODEL_FILENAMEを見つけたらXファイル名を格納
						if (strcmp(&aText[0], "LOOP") == 0)
						{
							int nNum = 0;
							fscanf(pFile, "%s %d", &aText[0], &nNum);
							if (nNum == 1)
							{
								m_aMotionInfo[0][nCntMotion].bLoop = true;
							}
						}
						// PARTSSETを&aText[0]各パーツの情報を格納する許可を与える
						if (strcmp(&aText[0], "NUM_KEY") == 0)
						{
							fscanf(pFile, "%s %d", &aText[0], &m_aMotionInfo[0][nCntMotion].nNumKey);
						}
						// PARTSSETを&aText[0]各パーツの情報を格納する許可を与える
						if (strcmp(&aText[0], "KEYSET") == 0 && nCntKeyNum < m_aMotionInfo[0][nCntMotion].nNumKey)
						{
							bSetKey = true;
						}
						if (bSetKey == true)
						{
							// PARENTを見つけたらXファイル名を格納
							if (strcmp(&aText[0], "FRAME") == 0)
							{
								fscanf(pFile, "%s %d", &aText[0], &m_aMotionInfo[0][nCntMotion].aKeyInfo[nCntKeyNum].nFrame);
							}
							// PARTSSETを&aText[0]各パーツの情報を格納する許可を与える
							if (strcmp(&aText[0], "KEY") == 0)
							{
								bKey = true;
							}
							if (bKey == true)
							{
								// POSを見つけたら座標情報を格納
								if (strcmp(&aText[0], "POS") == 0)
								{
									fscanf(pFile, "%s %f %f %f", &aText[0],
										&m_aMotionInfo[0][nCntMotion].aKeyInfo[nCntKeyNum].aKey[nCntKey].fPosX,
										&m_aMotionInfo[0][nCntMotion].aKeyInfo[nCntKeyNum].aKey[nCntKey].fPosY,
										&m_aMotionInfo[0][nCntMotion].aKeyInfo[nCntKeyNum].aKey[nCntKey].fPosZ);
								}
								// ROTを見つけたら角度情報を格納
								if (strcmp(&aText[0], "ROT") == 0)
								{
									fscanf(pFile, "%s %f %f %f", &aText[0],
										&m_aMotionInfo[0][nCntMotion].aKeyInfo[nCntKeyNum].aKey[nCntKey].fRotX,
										&m_aMotionInfo[0][nCntMotion].aKeyInfo[nCntKeyNum].aKey[nCntKey].fRotY,
										&m_aMotionInfo[0][nCntMotion].aKeyInfo[nCntKeyNum].aKey[nCntKey].fRotZ);
								}
								// END_SCRIPTを見つけたら読み込みを終える
								if (strcmp(&aText[0], "END_KEY") == 0)
								{
									bKey = false;
									nCntKey++;
								}
							}
							// END_SCRIPTを見つけたら読み込みを終える
							if (strcmp(&aText[0], "END_KEYSET") == 0)
							{
								bSetKey = false;
								nCntKey = 0;
								nCntKeyNum++;
							}
						}
						// END_SCRIPTを見つけたら読み込みを終える
						if (strcmp(&aText[0], "END_MOTIONSET") == 0)
						{
							bMotion = false;
							nCntKeyNum = 0;
							nCntMotion++;
						}
						// END_SCRIPTを見つけたら読み込みを終える
						if (strcmp(&aText[0], "END_SCRIPT") == 0)
						{
							break;
						}
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