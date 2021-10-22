//*****************************************************************************
//
// バレット処理 [bullet.h]
// Author : SUZUKI FUUTA
//
//*****************************************************************************
#ifndef _BULLET_H_
#define	_BULLET_H_

//-----------------------------------------------------------------------------
// インクルードファイル
//-----------------------------------------------------------------------------
#include "scene2D.h"

//-----------------------------------------------------------------------------
// クラス
//-----------------------------------------------------------------------------
class CBullet : public CScene2D		
{
public:

	CBullet(Priority nPriority);
	~CBullet();	  

	// create関数
	static CBullet *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, const D3DXVECTOR3 move, Priority nPriority);

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

	int					m_BulletStock;
	bool				m_bUse;			// 使用しているか
	bool				m_Collision;	// 当たり判定
	bool				m_ColiPlayer;
};
#endif // !_BULLET_H_
