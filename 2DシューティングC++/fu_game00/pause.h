//*****************************************************************************
//
// ポーズ処理 [pause.h]
// Author : SUZUKI FUUTA
//
//*****************************************************************************
#ifndef _PAUSE_H_
#define _PAUSE_H_

//-----------------------------------------------------------------------------
// インクルードファイル
//-----------------------------------------------------------------------------
#include "manager.h"
#include "scene.h"

class CScene2D;

//-----------------------------------------------------------------------------
// クラス
//-----------------------------------------------------------------------------
class CPause : public CScene
{
public:
	typedef enum PAUSEMENU
	{
		MENU_RETRY = 0,
		MENU_TITLE,
		MENU_QUIT,
		MENU_MAX
	}PAUSEMENU;

	CPause();
	~CPause();

	static CPause *Create(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SelectMenu(const int nMenuID);
	void MenuOk(const int nMenuID);

private:
	LPDIRECT3DTEXTURE9			m_pTex = NULL;			// テクスチャへのポイント

	D3DXVECTOR2					m_tex;					// テクスチャ座標
	D3DXVECTOR3					m_pos;					// 位置
	D3DXVECTOR3					m_ButtonPos;			// Buttonの位置
	D3DXVECTOR3					m_rot;					// 角度
	D3DXVECTOR3					m_size;					// サイズ
	D3DXVECTOR3					m_move;					// 移動量
	D3DXCOLOR					m_col;					// カラー

	static CScene2D	*m_pScene2D;
	static CScene2D	*m_pPauseButton[MENU_MAX];
	static bool m_bPause;
	static int m_nMenuID;
};

#endif // !_PAUSE_H_
