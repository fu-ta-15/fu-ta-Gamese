//*****************************************************************************
//
// �p�[�e�B�N������ [particle.cpp]
// Author : SUZUKI FUUTA
//
//*****************************************************************************
//-----------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
//-----------------------------------------------------------------------------
#include "particle.h"
#include "manager.h"
#include "renderer.h"

void Particle::SetParticle(const D3DXVECTOR3 & pos, const D3DXVECTOR3 & size, int EfeectNum, ParticleType type, const LPCSTR pSrcFile)
{
	D3DXVECTOR3			move = ZeroVector3;
	LPDIRECT3DTEXTURE9	pTex = NULL;			// �e�N�X�`���ւ̃|�C���g
	float				fAngle = 0.0f;

	pTex = Particle::TextureCreate(pSrcFile, pTex);

	for (int nCnt = 0; nCnt < EfeectNum; nCnt++)
	{
		switch (type)
		{
		case Particle::TYPE_EXPLOSION:
			fAngle = (float)((rand() % 628) - 628) / 100.f;
			break;

		case Particle::TYPE_RIGHT_FAN:
			fAngle = (float)(rand() % -314) / 100.f;
			break;

		case Particle::TYPE_LEFT_FAN:
			fAngle = (float)(rand() % 314) / 100.f;
			break;

		default:
			break;
		}

		// �����
		move.x = sinf(fAngle + D3DX_PI) * 3.0f;
		move.y = cosf(fAngle - D3DX_PI) * 3.0f;
		move.z = 0.0f;

		CEffect::Create(pos, size, move, pTex);
	}
}

LPDIRECT3DTEXTURE9 Particle::TextureCreate(LPCSTR pSrcFile, LPDIRECT3DTEXTURE9 pTex)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, pSrcFile, &pTex);

	return pTex;
}
