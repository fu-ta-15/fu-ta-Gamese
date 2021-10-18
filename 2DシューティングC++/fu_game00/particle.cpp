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

//=============================================================================
// パーティクルの生成
//=============================================================================
void Particle::SetParticle(const D3DXVECTOR3 & pos, const D3DXVECTOR3 & size, int EfeectNum, ParticleType type, const LPCSTR pSrcFile)
{
	D3DXVECTOR3			move = ZeroVector3;		// 移動量
	LPDIRECT3DTEXTURE9	pTex = NULL;			// テクスチャへのポイント
	float				fAngle = 0.0f;			// 角度

	// テクスチャの生成
	pTex = Particle::TextureCreate(pSrcFile, pTex);

	for (int nCnt = 0; nCnt < EfeectNum; nCnt++)
	{// 指定された数で生成
		switch (type)
		{// 種類で角度設定

			// 全体
		case Particle::TYPE_EXPLOSION:
			fAngle = (float)((rand() % 628) - 628) / 100.f;
			break;

			// 右半分
		case Particle::TYPE_RIGHT_FAN:
			fAngle = (float)(rand() % -314) / 100.f;
			break;

			// 左半分
		case Particle::TYPE_LEFT_FAN:
			fAngle = (float)(rand() % 314) / 100.f;
			break;

		default:
			break;
		}

		// 移動量代入
		move.x = sinf(fAngle + D3DX_PI) * 3.0f;
		move.y = cosf(fAngle - D3DX_PI) * 3.0f;
		move.z = 0.0f;

		// ポリゴン生成
		CEffect::Create(pos, size, move, pTex);
	}
}

//=============================================================================
// テクスチャの設定
//=============================================================================
LPDIRECT3DTEXTURE9 Particle::TextureCreate(LPCSTR pSrcFile, LPDIRECT3DTEXTURE9 pTex)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, pSrcFile, &pTex);

	return pTex;
}
