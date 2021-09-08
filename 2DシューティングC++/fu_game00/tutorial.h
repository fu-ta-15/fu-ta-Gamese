//*****************************************************************************
//
// �`���[�g���A������ [tutorial.h]
// Author : SUZUKI FUUTA
//
//*****************************************************************************
#ifndef _TUTORIAL_H_
#define	_TUTORIAL_H_

//-----------------------------------------------------------------------------
//�@�C���N���[�h�t�@�C��
//-----------------------------------------------------------------------------
#include "scene.h"
#include "keyinput.h"

//-----------------------------------------------------------------------------
//�O���錾
//-----------------------------------------------------------------------------
class CPlayer;
class CScene2D;
class CMesh;

//-----------------------------------------------------------------------------
//�N���X
//-----------------------------------------------------------------------------
class CTutorial : public CScene
{
public:

	typedef enum OPERA_KEY
	{
		KEY_A = 0,
		KEY_D,
		KEY_W,
		KEY_NUM_4,
		KEY_NUM_6,
		OPERA_MAX
	}OPERA_KEY;

	//-----------------------------------------------------------------------------
	// �����o�֐�
	//-----------------------------------------------------------------------------
	CTutorial();
	~CTutorial();

	static CTutorial *Create(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CMesh* GetMesh(void) { return m_pField; }

private:

	void OnKeyOperat(CKey *pKey);
	void OperatUpdate(void);

	static CPlayer		*m_pPlayer;
	static CScene2D		*m_pBg;
	static CMesh		*m_pField;
	static CMesh		*m_pOperation[OPERA_MAX];
	static CScene2D		*m_pOperationPolygon;
	static bool			 m_OperationStop;
	bool				 m_bButton[OPERA_MAX];
	int					 m_nCntTimeOP = 0;
	int					 m_nCntTime = 0;

};




#endif // !1
