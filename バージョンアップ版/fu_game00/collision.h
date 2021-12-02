//*****************************************************************************
//
// 当たり判定処理 [collision.h]
// Author : SUZUKI FUUTA
//
//*****************************************************************************
#ifndef _COLLISION_H_
#define _COLLISION_H_

//-----------------------------------------------------------------------------
// インクルードファイル
//-----------------------------------------------------------------------------
#include "scene.h"

namespace Collision
{
	bool CollisionSquare(D3DXVECTOR3 Mypos, D3DXVECTOR3 Mysize, D3DXVECTOR3 Tagepos, D3DXVECTOR3 Tagesize);
	bool CollisionCycle(D3DXVECTOR3 pointpos, D3DXVECTOR3 Cyclepos, float radius);
	bool OutProduct(const D3DXVECTOR3& lineStart1, const D3DXVECTOR3& lineEnd1, const D3DXVECTOR3& point);

	D3DXVECTOR3 MeshCollision(const D3DXVECTOR3& lineStart1, const D3DXVECTOR3& lineEnd1, const D3DXVECTOR3& point);
	D3DXVECTOR3 CrossProduct(const D3DXVECTOR3& v1, const D3DXVECTOR3& v2);
	D3DXVECTOR3 WaveCollision(const D3DXVECTOR3& start, const D3DXVECTOR3& end, const D3DXVECTOR3& nowpos);
}

#endif // !_COLLISION_H_
