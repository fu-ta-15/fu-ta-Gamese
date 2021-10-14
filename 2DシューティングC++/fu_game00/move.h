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
	static float MoveSnake(float pos, float move, float Uplimit, float Downlimit, float Speed);
	static D3DXVECTOR3 TargetPosMove(D3DXVECTOR3 tagpos, D3DXVECTOR3 mypos, float Decele);

	static float SinWave(const float CenterPos, const float nHeight, const float nCycle, const float nTime);
	static float CosWave(const float CenterPos, const float nHeight, const float nCycle, const float nTime);

	static float SinWave(const float fAngle,const float CenterPos, const float nHeight, const float nCycle, const float nTime);
	static float CosWave(const float fAngle,const float CenterPos, const float nHeight, const float nCycle, const float nTime);

	static float SinMove(const float nHeight, const float nCycle, const float nTime);
	static float CosMove(const float nHeight, const float nCycle, const float nTime);

	static float SinMove(const float fAngle, const float nHeight, const float nCycle, const float nTime);
	static float CosMove(const float fAngle, const float nHeight, const float nCycle, const float nTime);

	static float HomingMove(float Angle, float fSpeed);
	static float AnglePoint(float PointX, float PointY);

	static float getDistance(float x, float y, float x2, float y2)
	{
		float distance = sqrt((x2 - x) * (x2 - x) + (y2 - y) * (y2 - y));

		return (float)distance;
	}

	static float GetFoce(float Side, float Center, float tension, float delta)
	{
		return ((Side - Center)*tension / delta);
	}

private:

};


#endif // !_MOVE_H_
