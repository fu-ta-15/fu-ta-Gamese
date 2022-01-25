//=============================================================================
//
// ランキングのnamespace処理 [ranking_namespace.h]
// Author : itsuki takaiwa
//
//=============================================================================
#ifndef _RANKINGNAMESPACE_H_
#define _RANKINGNAMESPACE_H_

#include "main.h"

//*****************************************************************************
// ランキングのnamesoace
//*****************************************************************************
namespace RankingSpace
{
	int *SetAcsendingOrder(int *pOut, int *pScore, int nNumScore);
	int *SetDessendingOrder(int *pOut, int *pScore, int nNumScore);
	int *SetRanking(int *pOut, int *pScore, int nNumScore);
	int *SetSameRate(int *pOut, int *pScore, int nNumScore);
};
#endif