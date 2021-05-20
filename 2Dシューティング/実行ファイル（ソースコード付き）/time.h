#pragma once
#ifndef _TIME_H_
#define _TIME_H_
#include "main.h"



//プロトタイプ宣言
HRESULT InitTime(void);
void UninitTime(void);
void UpdateTime(void);
void DrawTime(void);
void AddTime(int nValue);

#endif // !_SCORE_H_
