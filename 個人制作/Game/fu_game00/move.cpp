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
D3DXVECTOR3 Move::MoveControl(D3DXVECTOR3& move, D3DXVECTOR3& deceleration)
{
	move.x += (0 - move.x) * deceleration.x;
	move.y += (0 - move.y) * deceleration.y;
	move.z += (0 - move.z) * deceleration.z;

	return move;
}

//=============================================================================
// 減速付き・目標の位置までの移動
//=============================================================================
D3DXVECTOR3 Move::TargetPosMove(D3DXVECTOR3& tagpos, D3DXVECTOR3& mypos, float Decele)
{
	mypos.x += (tagpos.x - mypos.x) * Decele;
	mypos.y += (tagpos.y - mypos.y) * Decele;
	mypos.z += (tagpos.z - mypos.z) * Decele;

	return mypos;
}

//=============================================================================
// 往復移動
//=============================================================================
float Move::MoveSnake(float pos, float move, float Uplimit, float Downlimit, float Speed)
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
// その位置からのSin波を利用した移動
//=============================================================================
float Move::SinWave(float CenterPos, float nHeight, float nCycle, float nTime)
{
	return (CenterPos + sinf((D3DX_PI * 2) / nCycle * (nTime)) * nHeight);
}

//=============================================================================
// その位置からのCos波を利用した移動
//=============================================================================
float Move::CosWave( float CenterPos,  float nHeight,  float nCycle,  float nTime)
{
	return (CenterPos + cosf((D3DX_PI * 2) / nCycle * (nTime)) * nHeight);
}

float Move::SinMove(float nHeight, float nCycle, float nTime)
{
	return (sinf((D3DX_PI * 2) / nCycle * (nTime)) * nHeight);
}

float Move::CosMove(float nHeight, float nCycle, float nTime)
{
	return (cosf((D3DX_PI * 2) / nCycle * (nTime)) * nHeight);
}

//=============================================================================
// 指定された角度に移動
//=============================================================================
float Move::HomingMove(float Angle, float fSpeed)
{
	return Angle * fSpeed;
}

//=============================================================================
// 角度の計算
//=============================================================================
float Move::AnglePoint(float PointX, float PointY)
{
	return atan2f(PointX, PointY);
}
