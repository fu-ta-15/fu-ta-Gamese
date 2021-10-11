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
#define PLAYER_MOVE					(5.0f)
#define PLAYER_JUNP					(20.0f)
#define PLAYER_RETURN_FLOOR			(SCREEN_HEIGHT - m_size.y)
#define MOVE_DECELERATION			(D3DXVECTOR3(1.0f, 0.02f, 0.0f))

#define BULLET_SIZE					(D3DXVECTOR3(20.0f, 20.0f, 0.0f))
#define BULLET_MOVE					(10.0f)
#define BULLET_MOVE_RIGHT			(D3DXVECTOR3(BULLET_MOVE, 0.0f, 0.0f))
#define BULLET_MOVE_LEFT			(D3DXVECTOR3(-BULLET_MOVE, 0.0f, 0.0f))

#define GRAVITY						(1.0f)
#define PLAYER_COLL_POS				(D3DXVECTOR3(m_pos.x, m_pos.y + m_size.y, m_pos.z))

#define MESH_GAME					(CGame::GetMesh())
#define MESH_TUTORIAL				(CTutorial::GetMesh())

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
	void SetLife(int nLife)			{ m_nLife = nLife; }

	// Get関数
	D3DXVECTOR3 GetSize(void)		{ return m_size; }
	StateType GetState(void)		{ return m_state; }
	bool GetStay(void)				{ return m_bStay; }
	bool GetDamage(void)			{ return m_bDamage; }
	bool GetAlive(void)				{ return m_bAlive; }
	int GetLife(void)				{ return m_nLife; }

private:
	void PosControl(void);
	void PlayerAction(void);
	void PlayerMove(void);
	void PlayerMoveControl(void);
	void FieldControl(void);
	void PlayerAnime(void);
	void PlayerState(void);
	void DamagePlayer(void);

	int					m_nAnimeCnt;		// アニメーションカウンター
	int					m_nDamageCnt;		// ダメージカウント
	int					m_nLife;			// ライフ
	float				m_fStayTime;		// とどまってる時間

	bool				m_bUse;				// 生存確認
	bool				m_bJunp;			// ジャンプ
	bool				m_bStay;			// 止まっているかどうか
	bool				m_bCollEnemy;		// 敵との当たり判定
	bool				m_bFall;			// 落下判定
	bool				m_bDamage;			// ダメージの判定
	bool				m_bAlive;			// 生きているかどうか

	D3DXVECTOR3			m_pos;				// 位置
	D3DXVECTOR3			m_KnockUpPos;		// ノックアップの位置
	D3DXVECTOR3			m_move;				// 移動量
	D3DXVECTOR3			m_size;				// サイズ
	D3DXVECTOR2			m_tex;				// テクスチャの分割数
	D3DXVECTOR2			m_number;			// テクスチャ何番目か
	D3DXCOLOR			m_col;				// 色

	StateType			m_state;

	CEffect				*m_pShield;
	CScene2D			*m_pLife[PLAYER_LIFE/10];
	CScene2D			*m_pWeapon[10];
};


#endif // !_PLAYER_H_
