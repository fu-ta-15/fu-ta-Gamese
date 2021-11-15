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

//-----------------------------------------------------------------------------
//�O���錾
//-----------------------------------------------------------------------------
class CPlayer;
class CEnemy;
class CTime;
class CBoss;
class CScene2D;
class CMesh;

//-----------------------------------------------------------------------------
// �N���X
//-----------------------------------------------------------------------------
class CGame : public CScene
{
public:
	CGame();
	~CGame();

	static CGame *Create(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// Get�֐�
	static CBoss* GetBoss(void)		{ return m_pBoss; }
	static CMesh* GetMesh(void)		{ return m_pField; }
	static CPlayer* GetPlayer(void)	{ return m_pPlayer; }
	static CTime* GetTime(void)		{ return m_pTime; }

private:
	float				m_fWaveHeight;		// �g�̍���
	float				m_nWaveCnt;			// �g���N�������߂̃J�E���g
	CMesh				*m_pLifeMesh;		// �{�X�̃_���[�W�\���̃��b�V���|���S��
	static CPlayer		*m_pPlayer;			// �v���C���[�̃|�C���^
	static CTime		*m_pTime;			// �^�C���̃|�C���^
	static CBoss		*m_pBoss;			// �{�X�̃|�C���^
	static CScene2D		*m_pBg;				// �w�i�̃|�C���^
	static CMesh		*m_pField;			// ���b�V���̃|�C���^�i�n�ʁj
};

#endif // !1
