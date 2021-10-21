//*****************************************************************************
//
// プレイヤー処理 [player.h]
// Author : SUZUKI FUUTA
//
//*****************************************************************************
#ifndef _PLAYER_H_
#define _PLAYER_H_

//-----------------------------------------------------------------------------
// インクルードファイル
//-----------------------------------------------------------------------------
#include "scene2D.h"
#include "effect.h"

//-----------------------------------------------------------------------------
// マクロ変数
//-----------------------------------------------------------------------------
#define PLAYER_LIFE					(100)
#define PLAYER_LIFE_STOCK			(10)
#define PLAYER_BULLET_STOCK			(10)


//-----------------------------------------------------------------------------
// クラス
//-----------------------------------------------------------------------------
class CPlayer : public CScene2D
{
public:
	typedef enum StateType
	{
		STATE_NONE = 0,
		STATE_DAMAGE,
		STATE_KNOCKUP,
		STATE_MAX
	}StateType;

	//-------------------------------------------------------------------------
	// メンバ関数
	//-------------------------------------------------------------------------
	CPlayer();
	~CPlayer();

	static CPlayer *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// Set関数
	void SetCollEnemy(bool bcoll)	{ m_bCollEnemy = bcoll; }
	void SetState(StateType state)	{ m_state = state; }
	void SetLife(float fLife)			{ m_fLife = fLife; }

	// Get関数
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

	int					m_nAnimeCnt;		// アニメーションカウンター
	int					m_nDamageCnt;		// ダメージカウント
	int					m_nBullet;
	int					m_nBulletTime;		// バレットタイム
	float				m_fLife;			// ライフ

	bool				m_bUse;				// 生存確認
	bool				m_bJunp;			// ジャンプ
	bool				m_bCollEnemy;		// 敵との当たり判定
	bool				m_bFall;			// 落下判定
	bool				m_bDamage;			// ダメージの判定
	bool				m_bAlive;

	D3DXVECTOR3			m_pos;				// 位置
	D3DXVECTOR3			m_KnockUpPos;		// ノックアップの位置
	D3DXVECTOR3			m_posOld;
	D3DXVECTOR3			m_move;				// 移動量
	D3DXVECTOR3			m_size;				// サイズ
	D3DXVECTOR2			m_tex;				// テクスチャの分割数
	D3DXVECTOR2			m_number;			// テクスチャ何番目か
	D3DXCOLOR			m_col;				// 色

	StateType			m_state;

	CEffect				*m_pShield;
	CScene2D			*m_pLife[PLAYER_LIFE_STOCK];
	CScene2D			*m_pWeapon[PLAYER_BULLET_STOCK];
};


#endif // !_PLAYER_H_
