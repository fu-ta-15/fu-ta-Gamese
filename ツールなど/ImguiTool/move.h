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
// 名前空間
//-----------------------------------------------------------------------------
namespace Move
{
	D3DXVECTOR3 MoveControl(D3DXVECTOR3& move, D3DXVECTOR3& deceleration);
	D3DXVECTOR3 TargetPosMove(D3DXVECTOR3& tagpos, D3DXVECTOR3& mypos, float Decele);
	float MoveSnake(float pos, float move, float Uplimit, float Downlimit, float Speed);
	float SinWave(float CenterPos, float nHeight, float nCycle, float nTime);
	float CosWave(float CenterPos, float nHeight, float nCycle, float nTime);
	float SinMove(float nHeight, float nCycle, float nTime);
	float CosMove(float nHeight, float nCycle, float nTime);
	float HomingMove(float Angle, float fSpeed);
	float AnglePoint(float PointX, float PointY);
}


#endif // !_MOVE_H_
