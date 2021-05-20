//##################################################################################################################################################################//
//
// ランキングヘッダファイル [ranking.h]
// Author : SUZUKI FUUTA
//
//##################################################################################################################################################################//


#ifndef _RANKING_H_

#define _RANKING_H_

#include "main.h"


//ランキングの構造体
typedef struct
{
	int NewScore;
	int OldScore;
	int ChangScore;
}RANKING_SCORE;


//プロトタイプ宣言
HRESULT InitRanking(void);
void UninitRanking(void);
void UpdateRanking(void);
void DrawRanking(void);
void OldRankingScore(void);
#endif