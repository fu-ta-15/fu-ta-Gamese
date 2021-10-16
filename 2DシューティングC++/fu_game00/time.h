//*****************************************************************************
//
// 時間処理 [time.h]
// Author : SUZUKI FUUTA
//
//*****************************************************************************
#ifndef _TIME_H_
#define _TIME_H_

//-----------------------------------------------------------------------------
// インクルードファイル
//-----------------------------------------------------------------------------
#include "number.h"
#include "scene.h"

//-----------------------------------------------------------------------------
// マクロ変数
//-----------------------------------------------------------------------------
#define MAX_TIME	(3)

//-----------------------------------------------------------------------------
// クラス
//-----------------------------------------------------------------------------
class CTime : public CScene
{
public:
	CTime();
	~CTime();

	static CTime *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, const int nTime);

	HRESULT Init();
	void  Uninit();
	void Update();
	void Draw();

	void SetTime(int nTime);
	void AddTime(int nValue);
	int AddCnt(int nValue);
	int GetTime(void);
	int GetFrame(void);

private:
	CNumber  *m_apNumber[MAX_TIME];
	int m_nTime;
	int m_nCntTime;
	int m_nFrame;
	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_size;
};

#endif // !_TIME_H_
