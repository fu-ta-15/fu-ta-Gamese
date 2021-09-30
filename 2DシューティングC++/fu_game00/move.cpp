//#############################################################################
//
// 移動ソースファイル [move.cpp]
// Author : SUZUKI FUUTA
//
//#############################################################################
//-----------------------------------------------------------------------------
// インクルードファイル
//-----------------------------------------------------------------------------
#include "move.h"

//=============================================================================
// プレイヤーの位置制御
//=============================================================================
D3DXVECTOR3 CMove::MoveControl(D3DXVECTOR3 move, D3DXVECTOR3 deceleration)
{

	// 移動・回転量の減衰
	move.x += (0 - move.x) * deceleration.x;
	move.y += (0 - move.y) * deceleration.y;
	move.z += (0 - move.z) * deceleration.z;

	return move;
}

//=============================================================================
// プレイヤーの位置制御
//=============================================================================
float CMove::MoveSnake(float pos, float move, float Uplimit, float Downlimit,float Speed)
{
	if (pos > Downlimit)
	{
		move = -move;
	}
	else if (pos < Uplimit)
	{
		move = (move) * -1;
	}
	return move;
}

//=============================================================================
// プレイヤーの位置制御
//=============================================================================
D3DXVECTOR3 CMove::TargetPosMove(D3DXVECTOR3 tagpos, D3DXVECTOR3 mypos, float Decele)
{
	// 移動・回転量の減衰
	mypos.x += (tagpos.x - mypos.x) * Decele;
	mypos.y += (tagpos.y - mypos.y) * Decele;
	mypos.z += (tagpos.z - mypos.z) * Decele;
					     
	return mypos;
}


//=============================================================================
// プレイヤーの位置制御
//=============================================================================
float CMove::SinWave(const float CenterPos, const float nHeight, const float nCycle, const float nTime)
{
	float WavePos = CenterPos + sinf((D3DX_PI * 2) / nCycle * (nTime + nTime)) * nHeight;

	return WavePos;
}

//=============================================================================
// プレイヤーの位置制御
//=============================================================================
float CMove::CosWave(const float CenterPos, const float nHeight, const float nCycle, const float nTime)
{
	float WavePos = CenterPos + cosf((D3DX_PI * 2) / nCycle * (nTime + nTime)) * nHeight;

	return WavePos;
}

float CMove::HomingMove(float Angle, float fSpeed)
{
	return Angle * fSpeed;
}

float CMove::AnglePoint(float PointX, float PointY)
{
	return atan2f(PointX, PointY);
}
