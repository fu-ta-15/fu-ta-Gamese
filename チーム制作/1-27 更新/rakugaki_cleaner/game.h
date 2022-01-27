//=============================================================================
//
// �Q�[����ʏ��� [game.h]
// Author : 
//
//=============================================================================
#ifndef _GAME_H_
#define _GAME_H_

#include "base.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CTimer;			// �^�C�}�[�N���X
class CPlayer_Game;		// �v���C���[�N���X
class CStage;			// �X�e�[�W�N���X
class CSetGame;			// �f�[�^�ǂݍ��݃N���X
class CCountdown;		// �J�E���g�_�E���N���X
class CGraffitiManager;	// ���N�K�L�Ǘ��N���X
class CGaugeManager;	// �Q�[�W�Ǘ��N���X
class CCamera;			// �J�����N���X

//*****************************************************************************
// �Q�[����ʃN���X(�h���N���X)
//*****************************************************************************
class CGame : public CBase
{
public:
	// �Q�[�����[�h
	typedef enum
	{
		GAMEMODE_NORMAL = 0,			// �ʏ�
		GAMEMODE_FINISH,				// �I��
		GAMEMODE_TRANSITION_MODESELECT,	// ���[�h�I���ɑJ��
		GAMEMODE_TRANSITION_GAME,		// �Q�[���ɑJ��
		GAMEMODE_TRANSITION_RESULT,		// ���U���g�ɑJ��
		GAMEMODE_MAX,
	}GAMEMODE;

	CGame();
	~CGame();

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CTimer *GetTimer(void) { return m_pTimer; }
	static void SetTimer(CTimer *pTimer) { m_pTimer = pTimer; }
	static CPlayer_Game **GetPlayer(void) { return m_pPlayer; }
	static CStage *GetStage(void) { return m_pStage; }
	static CCountdown *GetCountdwn(void) { return m_pCountdown; }
	static CGraffitiManager *GetGraffitiManager(void) { return m_pGraffitiManager; }
	static CGaugeManager *GetGaugeManager(void) { return m_pGaugeManager; }
	static int GetnCntPlayer(void) { return m_nCntPlayerOP; }
	static void SetGameMode(GAMEMODE gamemode) { m_gamemode = gamemode; }
	static GAMEMODE GetGameMode(void) { return m_gamemode; }
	static CCamera *GetCamera(void) { return m_pCamera; }

	int GetNumPlayer(void) { return m_nNumPlayer; }

private:
	static CTimer *m_pTimer;						// �^�C�}�[���̃|�C���^
	static CPlayer_Game **m_pPlayer;				// �v���C���[���̃|�C���^
	static CSetGame *m_pLoadData;					// �f�[�^�ǂݍ��ݏ��̃|�C���^
	static CStage *m_pStage;						// �X�e�[�W���̃|�C���^
	static CCountdown *m_pCountdown;				// �J�E���g�_�E�����̃|�C���^
	static CGraffitiManager *m_pGraffitiManager;	// ���N�K�L�Ǘ����̃|�C���^
	static CGaugeManager *m_pGaugeManager;			// �Q�[�W�Ǘ����̃|�C���^

	static int m_nCntPlayerOP;
	static GAMEMODE m_gamemode;
	static CCamera *m_pCamera;						// �J���������i�[����擪�A�h���X�̃|�C���^

	int m_nNumPlayer;
};
#endif