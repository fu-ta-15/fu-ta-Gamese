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
	void NoneBoss(void);
	void DamageBoss(void);
	void NotDamageBoss(void);

	void SetState(BOSS_STATE state)		{ m_State = state; }
	void SetDamage(bool bDamage)		{ m_bDamage = bDamage; }
	void SetShield(bool bShield)		{ m_bShield = bShield; }
	void SetShieldAlpha(float fAlpha)	{ m_nA_Shield = fAlpha; }

	BOSS_STATE GetState(void)			{ return m_State; }
	bool GetShield(void)				{ return m_bShield; }
	static void SetAlive(bool bAlive)	{ m_bBoss_Alive = bAlive; }
	static bool GetAlive(void)			{ return m_bBoss_Alive; }

private:
	static bool			m_bBoss_Alive;

	BOSS_STATE			m_State;
	CEffect				*m_pDamage;
	CEffect				*m_pShield;
	bool				m_bShield;
	bool				m_bDamage;

	D3DXCOLOR			m_StateCol;
	D3DXCOLOR			m_ShieldCol;
	float				m_nA_Damage = 0.0f;
	float				m_nA_Shield = 0.0f;
	int					m_nDamageCnt = 0;
	int					m_nCnt = 0;
};

#endif // !_BOSS_H_
