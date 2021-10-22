//*****************************************************************************
//
// コア処理 [core.h]
// Author : SUZUKI FUUTA
//
//*****************************************************************************
#ifndef _CORE_H_
#define	_CORE_H_

//-----------------------------------------------------------------------------
// インクルードファイル
//-----------------------------------------------------------------------------
#include "scene2D.h"

//-----------------------------------------------------------------------------
// クラス
//-----------------------------------------------------------------------------
class CCore : public CScene2D
{
public:

	CCore(Priority nPriority);
	~CCore();

	// create関数
	static CCore *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, Priority nPriority);

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// BOOL関数
	bool CollisionBullet(void);

	// Set関数


private:

	D3DXVECTOR3			m_pos;			// 位置
	D3DXVECTOR3			m_size;			// サイズ
	D3DXVECTOR3			m_move;			// 移動量

	bool				m_bUse;			// 使用しているか
	bool				m_Collision;	// 当たり判定
};
#endif // !_BULLET_H_
