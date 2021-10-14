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
// マクロ
//-----------------------------------------------------------------------------
#define MAX_OBJECT		(128)

//-----------------------------------------------------------------------------
// クラス
//-----------------------------------------------------------------------------
class CScene
{
public:
	// オブジェクトタイプ
	typedef enum Priority	
	{
		OBJ_NONE = 0,	// シンプルオブジェクト
		OBJ_NONE2,	// シンプルオブジェクト
		OBJ_BULLET,		// バレットオブジェクト
		OBJ_PLAYER,		// プレイヤーオブジェクト
		OBJ_BOSS,		// ボスオブジェクト
		OBJ_ENEMY,		// 敵オブジェクト
		OBJ_EFFECT,
		OBJ_MAX
	}ObjectType;	
	
	enum PauseType
	{
		PAUSE_BG = 0,
		PAUSE_BUTTON1,
		PAUSE_BUTTON2,
		PAUSE_BUTTON3,
		PAUSE_MAX
	};

	CScene(Priority type);					
	CScene(PauseType type);					
	CScene(bool bpause);					

	// 仮想純粋関数
	virtual ~CScene();						
	virtual HRESULT Init(void)	= 0;		
	virtual void Uninit(void)	= 0;		
	virtual void Update(void)	= 0;		
	virtual void Draw(void)		= 0;		

	static void ReleaseAll(void);			
	static void UpdateAll(void);			
	static void DrawAll(void);				

	void Release(void);						
	void DeathRelease(void);				
	static void PauseRelease(void);			


	/* Set関数 */
	void SetPos(D3DXVECTOR3 pos)	{ m_pos = pos; }		// 位置の設定
	void SetSize(D3DXVECTOR3 size)	{ m_size = size; }		// サイズの設定
	void SetCol(D3DXCOLOR col)		{ m_col = col; }		// 色の設定
	void SetBool(bool bflag)		{ m_bBool = bflag; }	// 何かのフラグ

	/* Get関数 */
	D3DXVECTOR3 GetPos(void)	{ return m_pos; }			// 特定のオブジェクトの位置取得
	D3DXVECTOR3 GetSize(void)	{ return m_size; }			// 特定のオブジェクトのサイズ取得
	D3DXCOLOR GetCol(void)		{ return m_col; }			// 色の取得
	bool GetBool(void)			{ return m_bBool; }			// フラグの取得
	CScene *GetSceneNext(void)	{ return this->m_pNext; }	// 次のシーンオブジェクトの取得
	static CScene *GetScene(Priority type) { 				// シーンオブジェクトの先頭の取得
		return m_pTop[type]; }

protected:

private:
	static CScene		*m_pTop[OBJ_MAX];			// 先頭のオブジェクトへのポインタ
	static CScene		*m_pCur[OBJ_MAX];			// 現在（最後尾）のオブジェクトへのポインタ
	static CScene		*m_pPauseScene;				// シーンを止める静的変数
	static CScene		*m_pPauseObj[PAUSE_MAX];
	CScene				*m_pPrev;					// 前のオブジェクトへのポインタ
	CScene				*m_pNext;					// 次のオブジェクトへのポインタ
	Priority			 m_type;					// オブジェクタイプ
	D3DXVECTOR3			 m_pos;						// 位置
	D3DXVECTOR3			 m_size;					// サイズ
	D3DXCOLOR			 m_col;						// 色
	bool				 m_bBool;
	bool				 m_bDeath;					// 死亡フラグ
};


#endif