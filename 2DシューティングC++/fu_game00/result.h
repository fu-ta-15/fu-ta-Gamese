//*****************************************************************************
//
// リザルト処理 [result.h]
// Author : SUZUKI FUUTA
//
//*****************************************************************************
#ifndef _RESULT_H_
#define	_RESULT_H_

//=============================================================================
//　インクルードファイル
//=============================================================================
#include "scene.h"

#define MAX_RANKING		(5)

//-----------------------------------------------------------------------------
//前方宣言
//-----------------------------------------------------------------------------
class CScore;
class CScene2D;


class CResult : public CScene
{
public:
	typedef enum Result
	{
		RESULT_UI_BG = 0,
		RESULT_UI_LOGO,
		RESULT_UI_BUTTON,
		RESULT_UI_MAX
	}Result;

	CResult();
	~CResult();

	static CResult *Create(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	D3DXVECTOR3 m_pos;

	static CScore	*m_paRanking[MAX_RANKING];
	//static CScene2D *m_paResultUI[]

};




#endif // !1
