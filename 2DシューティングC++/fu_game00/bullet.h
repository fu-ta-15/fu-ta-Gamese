//*****************************************************************************
//
// バレット処理 [bullet.h]
// Author : SUZUKI FUUTA
//
//*****************************************************************************
#ifndef _BULLET_H_
#define	_BULLET_H_

//-----------------------------------------------------------------------------
// インクルードファイル
//-----------------------------------------------------------------------------
#include "scene2D.h"

//-----------------------------------------------------------------------------
// マクロ変数
//-----------------------------------------------------------------------------
#define MAX_BULLET		(256)	// バレットの最大数

//-----------------------------------------------------------------------------
// 前方宣言
//-----------------------------------------------------------------------------
class CEnemy;

//-----------------------------------------------------------------------------
// バレットクラス
//-----------------------------------------------------------------------------
class CBullet : public CScene2D		// ポリゴン2Dを継承
{
public:

	typedef enum BULLET
	{
		BULLET_PLAYER = 0,
		BULLET_MAX
	}BULLETTYPE;

	CBullet();	   // コンストラクタ
	~CBullet();	   // デストラクタ

	// create関数
	static CBullet *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, const D3DXVECTOR3 move, BULLET type);	// バレットのインスタンス生成関数

	// メンバ関数
	HRESULT Init(void);		
	void Uninit(void);		
	void Update(void);		
	void Draw(void);		

	// BOOL関数
	bool CollisionBullet(D3DXVECTOR3 pos ,D3DXVECTOR3 size);	

	// Set関数
	void SetType(BULLET type) { m_type = type; }

private:
	static CEnemy		*m_pBoss;

	D3DXVECTOR3			m_pos;			// 位置
	D3DXVECTOR3			m_size;			// サイズ
	D3DXVECTOR3			m_move;			// 移動量
	bool				m_bUse;			// 使用しているか
	bool				m_Collision;	// 当たり判定
	bool				m_ColiPlayer;
	BULLET				m_type;			// バレットのタイプ
};
#endif // !_BULLET_H_
