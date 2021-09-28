//*****************************************************************************
//
// 通常の敵処理 [normalenemy.h]
// Author : SUZUKI FUUTA
//
//*****************************************************************************
#ifndef _NORMALENEMY_H_
#define _NORMALENEMY_H_

//-----------------------------------------------------------------------------
// インクルードファイル
//-----------------------------------------------------------------------------
#include "enemy.h"

class CNormalEnemy : public CEnemy
{
public:

	typedef enum EnemyMove
	{
		MOVE_0 = 0,
		MOVE_1,
		MOVE_2,
		MOVE_3,
		MOVE_4,
		MOVE_MAX
	}EnemyMove;

	//-----------------------------------------------------------------------------
	// メンバ関数
	//-----------------------------------------------------------------------------
	CNormalEnemy();
	~CNormalEnemy();

	static CNormalEnemy *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, ENEMYTYPE type);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void UpdateBlack(void);
	void UpdateWhite(void);
private:
	bool m_bCollision;
};

#endif // !_NORMALENEMY_H_
