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
#define MOVE0		(CNormalEnemy::MOVE_0)
#define MOVE1		(CNormalEnemy::MOVE_1)
#define MOVE2		(CNormalEnemy::MOVE_2)
#define MOVE3		(CNormalEnemy::MOVE_3)
#define MOVE4		(CNormalEnemy::MOVE_4)

//-----------------------------------------------------------------------------
// クラス
//-----------------------------------------------------------------------------
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
		MOVE_5,
		MOVE_6,
		MOVE_MAX
	}EnemyMove;

	//-------------------------------------------------------------------------
	// メンバ関数
	//-------------------------------------------------------------------------
	CNormalEnemy();
	~CNormalEnemy();

	static CNormalEnemy *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, ENEMYTYPE type, EnemyMove movetype);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetMoveType(EnemyMove movetype) { m_MoveType = movetype; }

	void UpdateBlack(void);
	void BlackMove(void);
	void UpdateWhite(void);
private:
	EnemyMove m_MoveType;
	bool m_bCollision;
	bool m_bHomingTime;
	float m_fSinWaveCnt = 0.0f;
	float m_fCosWaveCnt = 0.0f;
	int m_nMoveChangeCnt = 0;
	int m_nHomingTime;
};

#endif // !_NORMALENEMY_H_
