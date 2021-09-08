//*****************************************************************************
//
// 当たり判定処理 [collision.h]
// Author : SUZUKI FUUTA
//
//*****************************************************************************
//-----------------------------------------------------------------------------
// インクルードファイル
//-----------------------------------------------------------------------------
#include "collision.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CCollision::CCollision()
{
}

//=============================================================================
// デストラクタ
//=============================================================================
CCollision::~CCollision()
{
}

//=============================================================================
// 四角同士の当たり判定
//=============================================================================
bool CCollision::CollisionSquare(D3DXVECTOR3 Mypos, D3DXVECTOR3 Mysize, D3DXVECTOR3 Tagepos, D3DXVECTOR3 Tagesize)
{
	bool bCol = false;

			if (Mypos.y + Mysize.y > Tagepos.y - Tagesize.y && Mypos.y - Mysize.y < Tagepos.y + Tagesize.y &&
				Mypos.x + Mysize.x > Tagepos.x - Tagesize.x && Mypos.x - Mysize.x < Tagepos.x + Tagesize.x)
			{/* 敵の範囲に弾が存在したら */
				bCol = true;							// 当たり判定は有
			}

	return bCol;
}

bool CCollision::CollisionCycle(D3DXVECTOR3 pointpos, D3DXVECTOR3 Cyclepos, float radius)
{
	bool bColl = false;

	float A = pointpos.x - Cyclepos.x;
	float B = pointpos.y - Cyclepos.y;
	float C = sqrt(A * A + B * B);

	if (C <= radius)
	{
		bColl = true;
	}

	return bColl;
}

D3DXVECTOR3 CCollision::MeshCollision(const D3DXVECTOR3 lineStart1, const D3DXVECTOR3 lineEnd1, const D3DXVECTOR3 point, COLLISION coltype)
{
	D3DXVECTOR3 pos;

	if (OutProduct(lineStart1, lineEnd1, point) == true)
	{
		pos = WaveCollision(lineStart1, lineEnd1, point, coltype);
	}

	return pos;
}

//=============================================================================
// 内積による反射ベクトル
//=============================================================================
D3DXVECTOR3 CCollision::CrossProduct(const D3DXVECTOR3 v1, const D3DXVECTOR3 v2)
{
	D3DXVECTOR3 VecF;						// 進行ベクトル
	float VecN;								// 壁の法線ベクトル(法線のため1固定）

	float VecAX;							// 未知の値（内積で使う値）X
	float VecAZ;							// 未知の値（内積で使う値）Z

	float VecPX;							// 壁に平行なベクトル X
	float VecPZ;							// 壁に平行なベクトル Z

	float VecWX;	// 壁ずりベクトル						// 反射ベクトル X
	float VecWY;	// 壁ずりベクトル						// 反射ベクトル Z


	// 進行ベクトルの代入
	VecF = v1;

	// 垂直ベクトル
	VecN = -1.0f;

	// 内積で使用する未知の数値を求める
	VecAX = -VecF.x * VecN;
	VecAZ = -VecF.y * VecN;

	// 当たったものとの並行ベクトルを求める
	VecPX = VecF.x + VecAX * VecN;
	VecPZ = VecF.y + VecAZ * VecN;

	// 反射ベクトルを求める
	VecWX = VecF.x + (VecPX) * VecN;
	VecWY = VecF.y + (VecPZ) * VecN;

	return D3DXVECTOR3(VecWX, VecWY, 0.0f);
}

//=============================================================================
// 外積の当たり判定
//=============================================================================
bool CCollision::OutProduct(const D3DXVECTOR3 lineStart1, const D3DXVECTOR3 lineEnd1, const D3DXVECTOR3 point)
{
	bool Collision = false;

	float V1_X = (lineEnd1.x - lineStart1.x);
	float V1_Y = (lineEnd1.y - lineStart1.y);

	float V2_X = (point.x - lineStart1.x);
	float V2_Y = (point.y - lineStart1.y);

	float L1 = V1_Y * V2_X;
	float L2 = V2_Y * V1_X;
	float L3 = L1 - L2;

	if (L3 < 0)
	{
		Collision = true;
	}

	return Collision;
}

//=============================================================================
// Y座標が？の時に求める処理
//=============================================================================
D3DXVECTOR3 CCollision::WaveCollision(const D3DXVECTOR3 start, const D3DXVECTOR3 end, const D3DXVECTOR3 nowpos, COLLISION coltype)
{
	D3DXVECTOR3 tagPos = {};
	D3DXVECTOR3 startPos = start;
	D3DXVECTOR3 endPos = end;
	D3DXVECTOR3 LengthPos;

	float pos_x;
	float pos_y;

	LengthPos.x = endPos.x - startPos.x;
	LengthPos.y = endPos.y - startPos.y;

	pos_x = nowpos.x / LengthPos.x;
	pos_y = nowpos.y / LengthPos.y;

	float Xpercent = 100 - pos_x;
	float Ypercent = 100 - pos_y;

	switch (coltype)
	{
	case CCollision::TYPE_COL_Y:

		tagPos.y = LengthPos.y *(pos_x / 100);
		tagPos.x = nowpos.x;

		break;
	case CCollision::TYPE_COL_X:

		tagPos.x = LengthPos.x * Ypercent;
		tagPos.y = nowpos.y;

		break;
	default:
		break;
	}

	if (start.y < end.y)
	{
		tagPos.y = end.y + tagPos.y;
	}
	if (start.y > end.y)
	{
		tagPos.y = start.y + tagPos.y;
	}

	if (tagPos.y == 0.0f)
	{
		tagPos.y = start.y;
	}

	if (tagPos.y < 0)
	{
		tagPos.y *= -1;
	}
	if (tagPos.x < 0)
	{
		tagPos.x *= -1;
	}
	return tagPos;
}
