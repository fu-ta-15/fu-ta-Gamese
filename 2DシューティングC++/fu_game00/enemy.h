//*****************************************************************************
//
// 敵処理 [enemy.h]
// Author : SUZUKI FUUTA
//
//*****************************************************************************
#ifndef _ENEMY_H_
#define _ENEMY_H_

//-----------------------------------------------------------------------------
// インクルードファイル
//-----------------------------------------------------------------------------
#include "scene2D.h"
#include "bullet.h"

//-----------------------------------------------------------------------------
// 敵の派生クラスに使用するためのマクロ変数
//-----------------------------------------------------------------------------
#define MAX_ENEMY		(256)

#define ENEMY_POS		(D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT / 2, 0.0f))
#define ENEMY_SIZE		(D3DXVECTOR3(15.0f, 15.0f, 0.0f))
#define ENEMY_MOVE		(D3DXVECTOR3(-2.0f, 0.0f, 0.0f))
#define ENEMY_LIFE		(2)

#define BOSS_POS_Y		(368)
#define BOSS_POS_X		(1066)
#define BOSS_SIZE_Y		(75)
#define BOSS_SIZE_X		(75)

#define BOSS_POS		(D3DXVECTOR3(BOSS_POS_X, BOSS_POS_Y, 0.0f))
#define BOSS_SIZE		(D3DXVECTOR3(BOSS_SIZE_X, BOSS_SIZE_Y, 0.0f))
#define BOSS_LIFE		(10)

#define ENEMY_TYPE0		(CEnemy::ENEMY_BLACK)
#define ENEMY_TYPE1		(CEnemy::ENEMY_WHITE)
#define ENEMY_TYPE2		(CEnemy::ENEMY_BOSS)

#define ENEMY_TEXTURE0	("data/TEXTURE/Enemy0_1.png")	

//-----------------------------------------------------------------------------
// クラス
//-----------------------------------------------------------------------------
class CEnemy : public CScene2D
{
public:
	// 敵の種類
	typedef enum ENEMY
	{
		ENEMY_BLACK = 0, 
		ENEMY_WHITE,
		ENEMY_BOSS,
		ENEMY_NULL,
		ENEMY_MAX
	}ENEMYTYPE;

	//-----------------------------------------------------------------------------
	// メンバ関数
	//-----------------------------------------------------------------------------
	CEnemy();
	~CEnemy();

	HRESULT Init(void);
	void Uninit(void);	
	void Update(void);	
	void Draw(void);	

	// 当たり判定処理
	void CollisionEnemy(void);
	bool CollisionPlayer(void);

	// ダメージ
	void DamegeLife(int nDamege)		{ m_nLife -= nDamege; }

	// Set関数
	void SetPos(D3DXVECTOR3 pos)		{ m_pos = pos; }
	void SetSize(D3DXVECTOR3 size)		{ m_size = size; }
	void SetCol(D3DXCOLOR col)			{ m_col = col; }
	void SetLife(int nLife)				{ m_nLife = nLife; }
	void SetType(ENEMY type)			{ m_type = type; }

	// Get関数
	ENEMY GetEnemyType(void)			{ return m_type; }
	int GetLife(void)					{ return m_nLife; }

protected:

	D3DXVECTOR3				m_pos;							// 位置
	D3DXVECTOR3				m_size;							// サイズ
	D3DXVECTOR3				m_move;							// 移動量
	D3DXCOLOR				m_col;							// 色
	int						m_nLife;						// 弾の継続時間
	ENEMY					m_type;							// 敵のタイプ

private:
};



#endif // !1
