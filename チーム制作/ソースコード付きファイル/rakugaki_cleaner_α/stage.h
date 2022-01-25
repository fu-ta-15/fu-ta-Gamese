//=============================================================================
//
// ステージ処理 [stage.h]
// Author : 
//
//=============================================================================
#ifndef _STAGE_H_
#define _STAGE_H_

#include "sceneX.h"

//-----------------------------------------------------------------------------
// ステージクラス(派生クラス)
//-----------------------------------------------------------------------------
class CStage : public CSceneX
{
public:
	CStage(int nPriority);
	~CStage();

	static CStage *Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, D3DXVECTOR3 rot,char *pFileName);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	bool Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 size);			// ステージの当たり判定
	bool CollisionUp(D3DXVECTOR3 *pPos, D3DXVECTOR3 size);
};
#endif
