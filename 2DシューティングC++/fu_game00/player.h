//*****************************************************************************
//
// �v���C���[���� [player.h]
// Author : SUZUKI FUUTA
//
//*****************************************************************************
#ifndef _PLAYER_H_
#define _PLAYER_H_

//-----------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
//-----------------------------------------------------------------------------
#include "scene2D.h"
#include "effect.h"

//-----------------------------------------------------------------------------
// �}�N���ϐ�
//-----------------------------------------------------------------------------
#define PLAYER_LIFE					(100)
#define PLAYER_LIFE_STOCK			(10)
#define PLAYER_BULLET_STOCK			(10)

//-----------------------------------------------------------------------------
// �N���X
//-----------------------------------------------------------------------------
class CPlayer : public CScene2D
{
public:
	// ��Ԃ̎��
	enum StateType
	{
		STATE_NONE = 0,
		STATE_DAMAGE,
		STATE_KNOCKUP,
		STATE_MAX
	};

	CPlayer();
	~CPlayer();

	static CPlayer *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// Set�֐�
	void SetCollEnemy(bool bcoll)	{ m_bCollEnemy = bcoll; }
	void SetState(StateType state)	{ m_state = state; }
	void SetLife(float fLife)			{ m_fLife = fLife; }

	// Get�֐�
	D3DXVECTOR3 GetSize(void)		{ return m_size; }
	StateType GetState(void)		{ return m_state; }
	bool GetDamage(void)			{ return m_bDamage; }
	bool GetAlive(void)				{ return m_bAlive; }
	float GetLife(void)				{ return m_fLife; }

private:
	void PosControl(void);
	void PlayerAction(void);
	void PlayerMove(void);
	void PlayerMoveControl(void);
	void FieldControl(void);
	void PlayerAnime(void);
	void PlayerState(void);
	void DamagePlayer(void);
	void PlayerLife(void);
	void PlayerBullet(int nBullet);

	StateType			m_state;			// ���
	int					m_nAnimeCnt;		// �A�j���[�V�����J�E���^�[
	int					m_nDamageCnt;		// �_���[�W�J�E���g
	int					m_nBullet;			// �o���b�g�̃X�g�b�N
	int					m_nBulletTime;		// �o���b�g�^�C��
	int					m_nBulletCharge;	// �o���b�g�̉񕜃J�E���g
	float				m_fLife;			// ���C�t
	bool				m_bUse;				// �����m�F
	bool				m_bJunp;			// �W�����v
	bool				m_bCollEnemy;		// �G�Ƃ̓����蔻��
	bool				m_bFall;			// ��������
	bool				m_bDamage;			// �_���[�W�̔���
	bool				m_bAlive;			// �����Ă��邩
	D3DXVECTOR3			m_pos;				// �ʒu
	D3DXVECTOR3			m_KnockUpPos;		// �m�b�N�A�b�v�̈ʒu
	D3DXVECTOR3			m_posOld;			// �ߋ��̈ʒu
	D3DXVECTOR3			m_move;				// �ړ���
	D3DXVECTOR3			m_size;				// �T�C�Y
	D3DXVECTOR2			m_tex;				// �e�N�X�`���̕�����
	D3DXVECTOR2			m_number;			// �e�N�X�`�����Ԗڂ�
	D3DXCOLOR			m_col;				// �F
	CEffect				*m_pShield;			// �V�[���h
	CScene2D			*m_pLife[PLAYER_LIFE_STOCK];	 // ���C�t�̃X�g�b�N�|���S��
	CScene2D			*m_pWeapon[PLAYER_BULLET_STOCK]; // �o���b�g�̃X�g�b�N�|���S��
};


#endif // !_PLAYER_H_
