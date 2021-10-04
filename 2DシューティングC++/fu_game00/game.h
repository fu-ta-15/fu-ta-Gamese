//*****************************************************************************
//
// �Q�[������ [game.h]
// Author : SUZUKI FUUTA
//
//*****************************************************************************
#ifndef _GAME_H_
#define	_GAME_H_

//-----------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
//-----------------------------------------------------------------------------
#include "scene.h"
#include "move.h"

//-----------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
//-----------------------------------------------------------------------------
#define GET_PLAYER		(CGame::GetPlayer())

//-----------------------------------------------------------------------------
//�O���錾
//-----------------------------------------------------------------------------
class CPlayer;
class CEnemy;
class CScore;
class CTime;
class CBoss;
class CScene2D;
class CMesh;

class CGame : public CScene
{
public:
	//-----------------------------------------------------------------------------
	// �����o�֐�
	//-----------------------------------------------------------------------------
	CGame();
	~CGame();

	static CGame *Create(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CBoss* GetBoss(void) { return m_pBoss; }
	static CMesh* GetMesh(void) { return m_pField; }
	static CPlayer* GetPlayer(void) { return m_pPlayer; }

private:

	void WaveInit(void);
	void WaveUpdate(void);

	static CPlayer		*m_pPlayer;
	static CScore		*m_pScore;
	static CTime		*m_pTime;
	static CBoss		*m_pBoss;
	static CScene2D		*m_pBg;
	static CMesh		*m_pField;
	float				m_fWaveTime = 0.0f;
	CMove::WaveInfo		m_WaveInfo;
};




#endif // !1
