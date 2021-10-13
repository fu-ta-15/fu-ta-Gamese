//*****************************************************************************
//
// ボス敵処理 [boss.h]
// Author : SUZUKI FUUTA
//
//*****************************************************************************
#ifndef _BOSS_H_
#define _BOSS_H_

//-----------------------------------------------------------------------------
// インクルードファイル
//-----------------------------------------------------------------------------
#include "enemy.h"
#include "effect.h"


//-----------------------------------------------------------------------------
// マクロ変数
//-----------------------------------------------------------------------------
#define BOSS_LIFE_STOCK		(BOSS_LIFE / 10)


//-----------------------------------------------------------------------------
// クラス
//-----------------------------------------------------------------------------
class CBoss : public CEnemy
{
public:
	// ボスの状態
	typedef enum BOSS_STATE
	{
		STATE_NONE = 0,
		STATE_DAMAGE,
		STATE_NOT_DAMAGE,

		STATE_MAX

	}BOSS_STATE;

	typedef enum LIFE_STATE
	{
		LIFE_RATE_0 = 0,
		LIFE_RATE_2,
		LIFE_RATE_5,
		LIFE_RATE_8,
		LIFE_RATE_MAX
	}LIFE_STATE;

	//-----------------------------------------------------------------------------
	// メンバ関数
	//-----------------------------------------------------------------------------
	CBoss();
	~CBoss();

	static CBoss *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size,const int nLife);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void UpdateBoss(void);
	void DamageBoss(void);
	void NotDamageBoss(void);
	void MoveBoss(void);
	void SummonsEnemy(void);

	void StateUpdate(void);

	void SetState(BOSS_STATE state)		{ m_State = state; }
	void SetShield(bool bShield)		{ m_bShield = bShield; }
	void SetShieldAlpha(float fAlpha)	{ m_fA_Shield = fAlpha; }

	BOSS_STATE GetState(void)			{ return m_State; }
	LIFE_STATE GetLifeState(void)		{ return m_LifeState; }
	bool GetShield(void)				{ return m_bShield; }
	float GetLife(void)					{ return m_fLife; }
	static void SetAlive(bool bAlive)	{ m_bBoss_Alive = bAlive; }
	static bool GetAlive(void)			{ return m_bBoss_Alive; }

private:


	CEffect				*m_pDamage;
	CEffect				*m_pShield;
	CScene2D			*m_pLife[BOSS_LIFE_STOCK];

	static bool			m_bBoss_Alive;

	BOSS_STATE			m_State;
	LIFE_STATE			m_LifeState;
	D3DXCOLOR			m_StateCol;
	D3DXCOLOR			m_ShieldCol;

	D3DXVECTOR3			m_OldPos;
	float				m_fLife;

	bool				m_bShield;
	bool				m_bDamage;
	float				m_fA_Damage;
	float				m_fA_Shield;

	int					m_nDamageCnt;
	int					m_nEnemyCnt;
	int					m_nBossTime;
	float				m_fMoveTime;
};

#endif // !_BOSS_H_
