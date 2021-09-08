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

//-----------------------------------------------------------------------------
// クラス
//-----------------------------------------------------------------------------
class CCollision
{
public:

	typedef enum COLLISION
	{
		TYPE_COL_Y = 0,
		TYPE_COL_X,
		TYPE_COL_MAX
	}COLLISION;

	//-----------------------------------------------------------------------------
	// メンバ関数
	//-----------------------------------------------------------------------------
	CCollision();
	~CCollision();

	static bool CollisionSquare(D3DXVECTOR3 Mypos, D3DXVECTOR3 Mysize,D3DXVECTOR3 Tagepos,D3DXVECTOR3 Tagesize);
	static bool CollisionCycle(D3DXVECTOR3 pointpos, D3DXVECTOR3 Cyclepos, float radius);
	static D3DXVECTOR3 MeshCollision(const D3DXVECTOR3 lineStart1, const D3DXVECTOR3 lineEnd1, const D3DXVECTOR3 point, COLLISION coltype);

	static D3DXVECTOR3 CrossProduct(const D3DXVECTOR3 v1, const D3DXVECTOR3 v2);
	static bool OutProduct(const D3DXVECTOR3 lineStart1, const D3DXVECTOR3 lineEnd1, const D3DXVECTOR3 point);
	static D3DXVECTOR3 WaveCollision(const D3DXVECTOR3 start, const D3DXVECTOR3 end, const D3DXVECTOR3 nowpos, COLLISION coltype);

private:

};


#endif // !_COLLISION_H_
