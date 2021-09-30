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

//-----------------------------------------------------------------------------
// マクロ変数
//-----------------------------------------------------------------------------
#define MOVE_TYPE0		(CNormalEnemy::MOVE_0)
#define MOVE_TYPE1		(CNormalEnemy::MOVE_1)
#define MOVE_TYPE2		(CNormalEnemy::MOVE_2)
#define MOVE_TYPE3		(CNormalEnemy::MOVE_3)
#define MOVE_TYPE4		(CNormalEnemy::MOVE_4)


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
	static CNormalEnemy *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, ENEMYTYPE type, EnemyMove movetype);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetMoveType(EnemyMove movetype) { m_MoveType = movetype; }

	void UpdateBlack(void);
	void UpdateWhite(void);
private:
	EnemyMove m_MoveType;
	bool m_bCollision;
};

#endif // !_NORMALENEMY_H_
