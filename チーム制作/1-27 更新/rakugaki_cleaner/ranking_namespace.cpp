//=============================================================================
//
// �����L���O��namespace���� [ranking_namespace.cpp]
// Author : 
//
//=============================================================================
#include "ranking_namespace.h"

#include "manager.h"
#include "renderer.h"
#include "texture.h"

//=============================================================================
// �����ɕ��בւ���
//=============================================================================
int *RankingSpace::SetAcsendingOrder(int *pOut, int *pScore, int nNumScore)
{
	// �ϐ��錾
	int nSubScore = 0;

	// ���בւ�
	for (int nCnt = 0; nCnt < nNumScore - 1; nCnt++)
	{
		for (int nCntSub = nCnt + 1; nCntSub < nNumScore; nCntSub++)
		{
			if (pScore[nCnt] <= pScore[nCntSub])
			{// �����L���O�X�R�A�X�V�����Ƃ�
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
// �~���ɕ��בւ���
//=============================================================================
int *RankingSpace::SetDessendingOrder(int *pOut, int *pScore, int nNumScore)
{
	// �ϐ��錾
	int nSubScore = 0;

	// ���בւ�
	for (int nCnt = 0; nCnt < nNumScore - 1; nCnt++)
	{
		for (int nCntSub = nCnt + 1; nCntSub < nNumScore; nCntSub++)
		{
			if (pScore[nCnt] >= pScore[nCntSub])
			{// �����L���O�X�R�A�X�V�����Ƃ�
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
// ���ʂ�����
//=============================================================================
int *RankingSpace::SetRanking(int *pOut, int *pScore, int nNumScore)
{
	// �ϐ��錾
	int nSubScore = 0;

	// ���בւ�
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
// �������l��
//=============================================================================
int *RankingSpace::SetSameRate(int *pOut, int *pScore, int nNumScore)
{
	// �ϐ��錾
	int nSubScore = 0;

	// ���בւ�
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