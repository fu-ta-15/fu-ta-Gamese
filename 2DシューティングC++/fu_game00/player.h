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

private:
	void PosControl(const D3DXVECTOR3 pos, const D3DXVECTOR3 move);
	void PlayerAction(const D3DXVECTOR3 pos, const D3DXVECTOR3 move);

	float						m_fG;			// 重力
	bool						m_bJunp;		// ジャンプ

	D3DXVECTOR3					m_pos;			// 位置
	D3DXVECTOR3					m_move;			// 位置
	D3DXVECTOR3					m_size;			// サイズ
	D3DXVECTOR3					m_moveBullet;	// バレットの移動量
	D3DXCOLOR					m_col;			// 色
	bool						m_bUse;			// 生存確認
};


#endif // !_PLAYER_H_
