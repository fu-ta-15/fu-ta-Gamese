//#############################################################################
//
// 移動ヘッダファイル [move.h]
// Author : SUZUKI FUUTA
//
//#############################################################################
#ifndef _MOVE_H_
#define _MOVE_H_

//-----------------------------------------------------------------------------
// インクルードファイル
//-----------------------------------------------------------------------------
#include "main.h"

//-----------------------------------------------------------------------------
// クラス
//-----------------------------------------------------------------------------
class CMove
{
public:
	//-----------------------------------------------------------------------------
	// メンバ関数
	//-----------------------------------------------------------------------------
	CMove() {}
	~CMove() {}

	static D3DXVECTOR3 MoveControl(D3DXVECTOR3 move, D3DXVECTOR3 deceleration);
	static float MoveSnake(float pos, float move, float Uplimit, float Downlimit);
	static D3DXVECTOR3 TargetPosMove(D3DXVECTOR3 tagpos, D3DXVECTOR3 mypos, float Decele);
	static float SinWave(const float CenterPos, const float nHeight, const float nCycle, const float nTime);
	static float CosWave(const float CenterPos, const float nHeight, const float nCycle, const float nTime);


private:

};


#endif // !_MOVE_H_
