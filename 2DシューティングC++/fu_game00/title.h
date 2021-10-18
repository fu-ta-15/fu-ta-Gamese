//*****************************************************************************
//
// タイトル処理 [title.h]
// Author : SUZUKI FUUTA
//
//*****************************************************************************
#ifndef _TITLE_H_
#define	_TITLE_H_

//=============================================================================
//　インクルードファイル
//=============================================================================
#include "scene.h"
#include "scene2D.h"
#include "mesh.h"
#include "sound.h"


class CTitle : public CScene
{
public:
	// タイトルUI
	typedef enum TitleUI
	{
		UI_BG = 0,
		UI_BUTTON,
		UI_MAX
	}TitleUI;

	CTitle();
	~CTitle();

	static CTitle *Create(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	void ButtonUI(void);								
	void TitleLogo(int nTime);

	static CScene2D*			m_paTitleUI[UI_MAX];	// タイトルUIポリゴン
	static CMesh*				m_pTitleLogo;			// メッシュポリゴン
	float						AddCol = 0.0f;			// α値
	int							m_nCntTime = 0;			// カウントタイマー
	D3DXVECTOR2					m_tex;					// テクスチャ座標
	D3DXVECTOR3					m_pos;					// 位置
	D3DXVECTOR3					m_rot;					// 角度
	D3DXVECTOR3					m_size;					// サイズ
	D3DXVECTOR3					m_move;					// 移動量
	D3DXCOLOR					m_col;					// カラー
	D3DXCOLOR					m_ButtonCol;			// Buttonの色情報

};




#endif // !1

