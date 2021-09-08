//*****************************************************************************
//
// スコア処理 [score.h]
// Author : SUZUKI FUUTA
//
//*****************************************************************************
#ifndef _SCORE_H_
#define _SCORE_H_

//-----------------------------------------------------------------------------
// インクルードファイル
//-----------------------------------------------------------------------------
#include "number.h"
#include "scene.h"

//-----------------------------------------------------------------------------
// マクロ変数
//-----------------------------------------------------------------------------
#define MAX_SCORE	(3)

//-----------------------------------------------------------------------------
// クラス
//-----------------------------------------------------------------------------
class CScore : public CScene
{
public:
	CScore();
	~CScore();

	static CScore *Create(const D3DXVECTOR3 pos,const D3DXVECTOR3 size);

	HRESULT Init();
	void  Uninit();
	void Update();
	void Draw();

	void SetScore(int nScore);
	void AddScore(int nValue);
	int GetScore();

private:
	CNumber  *m_apNumber[MAX_SCORE];
	int m_nScore;
	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_size;
};




#endif // !_SCORE_H_
