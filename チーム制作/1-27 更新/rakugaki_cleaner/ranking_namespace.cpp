//=============================================================================
//
// ランキングのnamespace処理 [ranking_namespace.cpp]
// Author : 
//
//=============================================================================
#include "ranking_namespace.h"

#include "manager.h"
#include "renderer.h"
#include "texture.h"

//=============================================================================
// 昇順に並べ替える
//=============================================================================
int *RankingSpace::SetAcsendingOrder(int *pOut, int *pScore, int nNumScore)
{
	// 変数宣言
	int nSubScore = 0;

	// 並べ替え
	for (int nCnt = 0; nCnt < nNumScore - 1; nCnt++)
	{
		for (int nCntSub = nCnt + 1; nCntSub < nNumScore; nCntSub++)
		{
			if (pScore[nCnt] <= pScore[nCntSub])
			{// ランキングスコア更新したとき
				nSubScore = pScore[nCnt];
				pScore[nCnt] = pScore[nCntSub];
				pScore[nCntSub] = nSubScore;
			}
		}
	}
	pOut = pScore;

	return pOut;
}

//=============================================================================
// 降順に並べ替える
//=============================================================================
int *RankingSpace::SetDessendingOrder(int *pOut, int *pScore, int nNumScore)
{
	// 変数宣言
	int nSubScore = 0;

	// 並べ替え
	for (int nCnt = 0; nCnt < nNumScore - 1; nCnt++)
	{
		for (int nCntSub = nCnt + 1; nCntSub < nNumScore; nCntSub++)
		{
			if (pScore[nCnt] >= pScore[nCntSub])
			{// ランキングスコア更新したとき
				nSubScore = pScore[nCnt];
				pScore[nCnt] = pScore[nCntSub];
				pScore[nCntSub] = nSubScore;
			}
		}
	}
	pOut = pScore;

	return pOut;
}

//=============================================================================
// 順位をつける
//=============================================================================
int *RankingSpace::SetRanking(int *pOut, int *pScore, int nNumScore)
{
	// 変数宣言
	int nSubScore = 0;

	// 並べ替え
	for (int nCnt = 0; nCnt < nNumScore; nCnt++)
	{
		pOut[nCnt] = 1;

		for (int nCntSub = 0; nCntSub < nNumScore; nCntSub++)
		{
			if (nCnt != nCntSub)
			{
				if (pScore[nCnt] < pScore[nCntSub])
				{
					pOut[nCnt]++;
				}
			}
		}
	}

	return pOut;
}

//=============================================================================
// 同率を考慮
//=============================================================================
int *RankingSpace::SetSameRate(int *pOut, int *pScore, int nNumScore)
{
	// 変数宣言
	int nSubScore = 0;

	// 並べ替え
	for (int nCnt = 0; nCnt < nNumScore; nCnt++)
	{
		pOut[nCnt] = 1;

		for (int nCntSub = 0; nCntSub < nNumScore; nCntSub++)
		{
			if (nCnt != nCntSub)
			{
				if (pScore[nCnt] < pScore[nCntSub])
				{
					pOut[nCnt]++;
				}
				else if (pScore[nCnt] == pScore[nCntSub] && nCnt > nCntSub)
				{
					pOut[nCnt]++;
				}
			}
		}
	}

	return pOut;
}