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
// 移動量の制御
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
// 往復移動
//=============================================================================
float CMove::MoveSnake(float pos, float move, float Uplimit, float Downlimit,float Speed)
{
	if (pos < Downlimit)
	{
		move = Speed;
	}
	if (pos > Uplimit)
	{
		move = -Speed;
	}
	return move;
}

//=============================================================================
// 減速付き・目標の位置までの移動
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
// その位置からのSin波を利用した移動
//=============================================================================
float CMove::SinWave(const float CenterPos, const float nHeight, const float nCycle, const float nTime)
{
	return CenterPos + sinf((D3DX_PI * 2) / nCycle * (nTime + nTime)) * nHeight;
}

//=============================================================================
// その位置からのCos波を利用した移動
//=============================================================================
float CMove::CosWave(const float CenterPos, const float nHeight, const float nCycle, const float nTime)
{
	return CenterPos + cosf((D3DX_PI * 2) / nCycle * (nTime + nTime)) * nHeight;
}

//=============================================================================
// Sin波を利用した移動
//=============================================================================
float CMove::SinMove(const float nHeight, const float nCycle, const float nTime)
{
	return sinf((D3DX_PI * 2) / nCycle * (nTime + nTime)) * nHeight;
}

//=============================================================================
// Cos波を利用した移動
//=============================================================================
float CMove::CosMove(const float nHeight, const float nCycle, const float nTime)
{
	return cosf((D3DX_PI * 2) / nCycle * (nTime + nTime)) * nHeight;
}

//=============================================================================
// 指定された角度に移動
//=============================================================================
float CMove::HomingMove(float Angle, float fSpeed)
{
	return Angle * fSpeed;
}

//=============================================================================
// 角度の計算
//=============================================================================
float CMove::AnglePoint(float PointX, float PointY)
{
	return atan2f(PointX, PointY);
}
