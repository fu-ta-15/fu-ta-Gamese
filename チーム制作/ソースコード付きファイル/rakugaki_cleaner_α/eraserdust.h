//#############################################################################
//
// 消しくずヘッダファイル [eraserdust.h]
// Author : SUZUKI FUUTA
//
//#############################################################################
#ifndef _ERASERDUST_H_
#define _ERASERDUST_H_

//-----------------------------------------------------------------------------
// インクルードファイル
//-----------------------------------------------------------------------------
#include "sceneX.h"

//-----------------------------------------------------------------------------
// クラス
//-----------------------------------------------------------------------------
class CEraserDust : public CSceneX
{
public:

	CEraserDust(int nPriority);
	~CEraserDust();

	static CEraserDust *Create(const D3DXVECTOR3& pos, const D3DXVECTOR2& size, D3DXVECTOR3 rot, char *pModelFileName);
	HRESULT Init(const D3DXVECTOR3& pos, const D3DXVECTOR2& size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	bool Collision(D3DXVECTOR3 pos, D3DXVECTOR3 rot);

private:
	bool CollisionStage(void);				// ステージとの当たり判定
	void CollisionSkill(void);				// スキルとの当たり判定

	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_rot;
	D3DXVECTOR3 m_move;
};

#endif
