//*****************************************************************************
//
// �{�X�G���� [boss.h]
// Author : SUZUKI FUUTA
//
//*****************************************************************************
#ifndef _BOSS_H_
#define _BOSS_H_

//-----------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
//-----------------------------------------------------------------------------
#include "enemy.h"
#include "effect.h"
#include "mesh.h"
#include "core.h"


//-----------------------------------------------------------------------------
// �}�N���ϐ�
//-----------------------------------------------------------------------------
#define BOSS_LIFE_STOCK		(BOSS_LIFE / 10)				// ���C�t�̃X�g�b�N
#define DAMAGE				(CBoss::STATE_DAMAGE)			// �_���[�W���
#define NOT_DAMAGE			(CBoss::STATE_NOT_DAMAGE)		// �_���[�WNO!
#define NONE				(CBoss::STATE_NONE)				// �����Ȃ����	


//-----------------------------------------------------------------------------
// �N���X
//-----------------------------------------------------------------------------
class CBoss : public CEnemy
{
public:
	// �{�X�̏��
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
	// �����o�֐�
	//-----------------------------------------------------------------------------
	CBoss();
	~CBoss();

	static CBoss *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size,const int nLife);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);


	void SetState(BOSS_STATE state)		{ m_State = state; }

	CCore **GetCore(void)				{ return &m_pCore[0]; }

	BOSS_STATE GetState(void)			{ return m_State; }
	LIFE_STATE GetLifeState(void)		{ return m_LifeState; }
	float GetLife(void)					{ return m_fLife; }
	void SetAlive(bool bAlive)			{ m_bBoss_Alive = bAlive; }
	bool GetAlive(void)					{ return m_bBoss_Alive; }

private:

	void UpdateBoss(void);
	void DamageBoss(void);
	void MoveBoss(void);
	void SummonsEnemy(void);
	void StateUpdate(void);

	CEffect				*m_pDamage;
	CScene2D			*m_pLife[BOSS_LIFE_STOCK];
	CScene2D			*m_pShiled;
	CCore				*m_pCore[3];

	static bool			m_bBoss_Alive;

	BOSS_STATE			m_State;
	LIFE_STATE			m_LifeState;
	D3DXCOLOR			m_StateCol;

	D3DXVECTOR3			m_OldPos;
	float				m_fLife;
	bool				m_bDamage;
	float				m_fA_Damage;
	int					m_nDamageCnt;
	float				m_fMoveTime;
};

#endif // !_BOSS_H_
