//=============================================================================
//
// ゲージ管理処理 [gauge_manager.h]
// Author : 
//
//=============================================================================
#ifndef _GAUGEMANAGER_H_
#define _GAUGEMANAGER_H_

#include "scene.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CGraffitiGauge;		// ラクガキゲージクラス
class CCrown;				// 王冠クラス
class CPlayerMarker;		// プレイヤーマーカークラス

//*****************************************************************************
// ゲージ管理クラス(基本クラス)
//*****************************************************************************
class CGaugeManager : public CScene
{
public:
	CGaugeManager();
	~CGaugeManager();

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetGraffitiGauge(CGraffitiGauge *pGauge) { m_pGraffitiGauge = pGauge; }

private:
	void ScoreGauge(void);
	void SkillGauge(void);
	void GraffitiGauge(void);

	int m_nNumPlayer;			// プレイヤー数
	float *m_pAddSizeOld;		// 前回の増加量
	int m_nCntRank_One;			// 一位の人数

	CGraffitiGauge *m_pGraffitiGauge;	// ラクガキゲージ情報のポインタ
	CCrown **m_pCrown;					// 王冠情報のポインタ
	CPlayerMarker **m_pPlayerMarker;	// プレイヤーマーカー情報のポインタ
};
#endif