//*****************************************************************************
//
// シーン処理 [scene.h]
// Author : SUZUKI FUUTA
//
//*****************************************************************************
#ifndef _SCENE_H_
#define _SCENE_H_

//-----------------------------------------------------------------------------
// インクルードファイル
//-----------------------------------------------------------------------------
#include "main.h"

//-----------------------------------------------------------------------------
// クラス
//-----------------------------------------------------------------------------
class CScene
{
public:
	// オブジェクトタイプ
	typedef enum Priority	
	{
		OBJ_NONE = 0,	// シンプルオブジェクト１
		OBJ_NONE2,		// シンプルオブジェクト２
		OBJ_BOSS,		// ボスオブジェクト
		OBJ_BULLET1,	// バレットオブジェクト１
		OBJ_BULLET2,	// バレットオブジェクト２
		OBJ_PLAYER,		// プレイヤーオブジェクト
		OBJ_CORE,		// コアオブジェクト
		OBJ_ENEMY,		// 敵オブジェクト
		OBJ_EFFECT1,	// エフェクトオブジェクト１
		OBJ_MAX
	}ObjectType;	
	
	// ポーズの種類
	enum PauseType
	{
		PAUSE_BG = 0,	// ポーズの背景
		PAUSE_BUTTON1,	// ポーズのボタン１
		PAUSE_BUTTON2,	// ポーズのボタン２
		PAUSE_BUTTON3,	// ポーズのボタン３
		PAUSE_MAX
	};

	CScene(Priority type);				// コンストラクタ
	CScene(PauseType type);				// ポーズ用コンストラクタ
	CScene(bool bpause);				// ポーズ生成合図用コンストラクタ

	// 仮想純粋関数
	virtual ~CScene();					// デストラク
	virtual HRESULT Init(void)	= 0;	// 初期化
	virtual void Uninit(void)	= 0;	// 終了
	virtual void Update(void)	= 0;	// 更新
	virtual void Draw(void)		= 0;	// 描画

	static void ReleaseAll(void);		// すべてのRelease
	static void UpdateAll(void);		// すべての更新
	static void DrawAll(void);			// すべての描画

	void Release(void);					// Release 
	void DeathRelease(void);			// 死亡フラグを持っている物のRelease
	void PauseRelease(void);			// ポーズのRelease


	// Set関数 
	void SetPos(D3DXVECTOR3 pos)	{ m_pos = pos; }		// 位置の設定
	void SetSize(D3DXVECTOR3 size)	{ m_size = size; }		// サイズの設定
	void SetCol(D3DXCOLOR col)		{ m_col = col; }		// 色の設定
	void SetBool(bool bflag)		{ m_bBool = bflag; }	// 何かのフラグ

	// Get関数 
	D3DXVECTOR3 GetPos(void)	{ return m_pos; }			// 特定のオブジェクトの位置取得
	D3DXVECTOR3 GetSize(void)	{ return m_size; }			// 特定のオブジェクトのサイズ取得
	D3DXCOLOR GetCol(void)		{ return m_col; }			// 色の取得
	bool GetBool(void)			{ return m_bBool; }			// フラグの取得
	CScene *GetSceneNext(void);								// 次のシーンオブジェクトの取得
	static CScene *GetScene(Priority type) { 				// シーンオブジェクトの先頭の取得
		return m_pTop[type]; }

private:
	static CScene		*m_pTop[OBJ_MAX];			// 先頭のオブジェクトへのポインタ
	static CScene		*m_pCur[OBJ_MAX];			// 現在（最後尾）のオブジェクトへのポインタ
	static CScene		*m_pPauseScene;				// シーンを止める静的変数
	static CScene		*m_pPauseObj[PAUSE_MAX];	// ポーズのオブジェクト
	CScene				*m_pPrev;					// 前のオブジェクトへのポインタ
	CScene				*m_pNext;					// 次のオブジェクトへのポインタ
	Priority			 m_type;					// オブジェクタイプ
	D3DXVECTOR3			 m_pos;						// 位置
	D3DXVECTOR3			 m_size;					// サイズ
	D3DXCOLOR			 m_col;						// 色
	bool				 m_bBool;					// 何かのフラグ
	bool				 m_bDeath;					// 死亡フラグ
};


#endif