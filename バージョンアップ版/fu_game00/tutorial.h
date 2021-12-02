//*****************************************************************************
//
// チュートリアル処理 [tutorial.h]
// Author : SUZUKI FUUTA
//
//*****************************************************************************
#ifndef _TUTORIAL_H_
#define	_TUTORIAL_H_

//-----------------------------------------------------------------------------
//　インクルードファイル
//-----------------------------------------------------------------------------
#include "scene.h"
#include "keyinput.h"
#include "move.h"

//-----------------------------------------------------------------------------
//前方宣言
//-----------------------------------------------------------------------------
class CPlayer;
class CScene2D;
class CMesh;
class CEnemy;
class CBoss;
class CMesh3D;


//-----------------------------------------------------------------------------
//クラス
//-----------------------------------------------------------------------------
class CTutorial : public CScene
{
public:

	typedef enum OPERA_KEY
	{
		KEY_A = 0,
		KEY_D,
		KEY_W,
		KEY_NUM_6,
		OPERA_MAX
	}OPERA_KEY;

	//-----------------------------------------------------------------------------
	// メンバ関数
	//-----------------------------------------------------------------------------
	CTutorial();
	~CTutorial();

	static CTutorial *Create(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CMesh* GetMesh(void) { return m_pField; }
	static CBoss* GetBoss(void) { return m_pBoss; }

private:

	void OnKeyOperat(CKey *pKey);
	void OperatUpdate(void);

	static CPlayer		*m_pPlayer;
	static CScene2D		*m_pBg;
	static CMesh		*m_pField;
	static CMesh		*m_pOperation[OPERA_MAX];
	static CScene2D		*m_pOperationPolygon;
	static CBoss		*m_pBoss;
	static CMesh3D		*m_pTutorialMesh3D;

	int					 m_nCntCycleLimit;
	bool				 m_bNextPage;

	static bool			 m_OperationStop;
	bool				 m_bButton[OPERA_MAX];
	int					 m_nCntTimeOP = 0;
	float				 m_fCntTime = 0.0f;

};




#endif // !1
