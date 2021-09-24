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
// 前方宣言
//-----------------------------------------------------------------------------
class CBullet;

//-----------------------------------------------------------------------------
// クラス
//-----------------------------------------------------------------------------
class CPlayer : public CScene2D
{
public:
	//-----------------------------------------------------------------------------
	// メンバ関数
	//-----------------------------------------------------------------------------
	CPlayer();
	~CPlayer();

	static CPlayer *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	D3DXVECTOR3 GetSize(void) { return m_size; }

private:
	void PosControl(void);
	void PlayerAction(void);
	void PlayerMove(void);
	void PlayerMoveControl(void);
	void FieldControl(void);
	void PlayerAnime(void);

	int							m_nAnimeCnt;		// アニメーションカウンター
	float						m_fStayTime;		// とどまってる時間
	bool						m_bUse;				// 生存確認
	bool						m_bJunp;			// ジャンプ
	bool						m_bStay;			// 止まっているかどうか
	bool						m_bBound;			// メッシュの反発
	D3DXVECTOR3					m_pos;				// 位置
	D3DXVECTOR3					m_move;				// 移動量
	D3DXVECTOR3					m_size;				// サイズ
	D3DXVECTOR3					m_moveBullet;		// バレットの移動量
	D3DXVECTOR2					m_tex;				// テクスチャの分割数
	D3DXVECTOR2					m_number;			// テクスチャ何番目か
	D3DXCOLOR					m_col;				// 色

	CEffect						*m_pShield;
};


#endif // !_PLAYER_H_
