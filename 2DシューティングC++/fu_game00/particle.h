//*****************************************************************************
//
// �p�[�e�B�N������ [particle.h]
// Author : SUZUKI FUUTA
//
//*****************************************************************************
#ifndef _PARTICLE_H_
#define _PARTICLE_H_

//-----------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
//-----------------------------------------------------------------------------
#include "effect.h"

//-----------------------------------------------------------------------------
// �}�N���ϐ�
//-----------------------------------------------------------------------------
#define MAX_EFFECT_NUM			(256)

//-----------------------------------------------------------------------------
// �N���X
//-----------------------------------------------------------------------------
class CParticle
{
public:
	// Particle���
	typedef enum ParticleType
	{
		TYPE_NONE = 0,
		TYPE_EXPLOSION,
		TYPE_RIGHT_FAN,
		TYPE_LEFT_FAN,
		PARTICLE_MAX
	}ParticleType;

	//-----------------------------------------------------------------------------
	// �����o�֐�
	//-----------------------------------------------------------------------------
	CParticle();
	~CParticle();

	CParticle *Create(int EfeectNum, D3DXVECTOR3 pos, D3DXVECTOR3 size, ParticleType type);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// Particle�̕\���֐�
	void Explosion(void);

private:
	int				m_EffectNum;
	D3DXVECTOR3		m_pos;
	D3DXVECTOR3		m_size;
	D3DXVECTOR3		m_move;

	ParticleType	m_type;

	CEffect			*m_apParticle[MAX_EFFECT_NUM];
};

#endif // !_PARTICLE_H_
