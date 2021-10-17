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



namespace Particle
{
	// Particle���
	enum ParticleType
	{
		TYPE_NONE = 0,
		TYPE_EXPLOSION,
		TYPE_RIGHT_FAN,
		TYPE_LEFT_FAN,
		TYPE_UP_FAN,
		TYPE_DOWN_FAN,
		PARTICLE_MAX
	};

	void SetParticle(const D3DXVECTOR3& pos, const D3DXVECTOR3& size, int EfeectNum, ParticleType type, const LPCSTR pSrcFile);
	LPDIRECT3DTEXTURE9 TextureCreate(LPCSTR pSrcFile, LPDIRECT3DTEXTURE9 pTex);

}

#endif // !_PARTICLE_H_
