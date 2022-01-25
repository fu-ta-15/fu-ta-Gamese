//*****************************************************************************
//
// パーティクル処理 [particle.h]
// Author : SUZUKI FUUTA
//
//*****************************************************************************
#ifndef _PARTICLE_H_
#define _PARTICLE_H_

//-----------------------------------------------------------------------------
// インクルードファイル
//-----------------------------------------------------------------------------
#include "effect.h"

//-----------------------------------------------------------------------------
// 名前空間
//-----------------------------------------------------------------------------
namespace Particle
{
	// パーティクルの種類
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

	// パーティクルの生成
	void SetParticle(const D3DXVECTOR3& pos, const D3DXVECTOR3& size, int EfeectNum, ParticleType type, const LPCSTR pSrcFile);

	// テクスチャの設定
	LPDIRECT3DTEXTURE9 TextureCreate(LPCSTR pSrcFile, LPDIRECT3DTEXTURE9 pTex);
}

#endif // !_PARTICLE_H_
