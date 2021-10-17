//*****************************************************************************
//
// パーティクル処理 [particle.cpp]
// Author : SUZUKI FUUTA
//
//*****************************************************************************
//-----------------------------------------------------------------------------
// インクルードファイル
//-----------------------------------------------------------------------------
#include "particle.h"
#include "manager.h"
#include "renderer.h"

void Particle::SetParticle(const D3DXVECTOR3 & pos, const D3DXVECTOR3 & size, int EfeectNum, ParticleType type, const LPCSTR pSrcFile)
{
	D3DXVECTOR3			move = ZeroVector3;
	LPDIRECT3DTEXTURE9	pTex = NULL;			// テクスチャへのポイント
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

		// 情報代入
		move.x = sinf(fAngle + D3DX_PI) * 3.0f;
		move.y = cosf(fAngle - D3DX_PI) * 3.0f;
		move.z = 0.0f;

		CEffect::Create(pos, size, move, pTex);
	}
}

LPDIRECT3DTEXTURE9 Particle::TextureCreate(LPCSTR pSrcFile, LPDIRECT3DTEXTURE9 pTex)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, pSrcFile, &pTex);

	return pTex;
}
