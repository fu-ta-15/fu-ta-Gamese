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
// マクロ変数
//-----------------------------------------------------------------------------
#define MAX_EFFECT_NUM			(256)

//-----------------------------------------------------------------------------
// クラス
//-----------------------------------------------------------------------------
class CParticle
{
public:
	// Particle種類
	typedef enum Particle
	{
		TYPE_NONE = 0,
		TYPE_EXPLOSION,
		PARTICLE_MAX
	}Particle;

	//-----------------------------------------------------------------------------
	// メンバ関数
	//-----------------------------------------------------------------------------
	CParticle();
	~CParticle();

	CParticle *Create(int EfeectNum, D3DXVECTOR3 pos, D3DXVECTOR3 size);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// Particleの表現関数
	void Explosion(void);

private:
	int				m_EffectNum;
	D3DXVECTOR3		m_pos;
	D3DXVECTOR3		m_size;
	D3DXVECTOR3		m_move;
	CEffect			*m_apParticle[MAX_EFFECT_NUM];
};

#endif // !_PARTICLE_H_
