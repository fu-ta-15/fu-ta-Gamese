//=============================================================================
//
// �Q�[���V�[���̃v���C���[���� [player_game.h]
// Author : 
//
//=============================================================================
#ifndef _PLAYERGAME_H_
#define _PLAYERGAME_H_

#include "player.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define DELETE_GARAFFITI_NUMBER_MAX		(3000)				// �X�L���p

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CEraser;			// �����S���N���X
class CScoreGauge;		// �X�R�A�Q�[�W�N���X
class CSkillGauge;		// �X�L���Q�[�W�N���X
class CPlayerMarker;	// �v���C���[�}�[�J�[�N���X
class CCamera;
class CShadow;

//-----------------------------------------------------------------------------
// �v���C���[�N���X(�h���N���X)
//-----------------------------------------------------------------------------
class CPlayer_Game : public CPlayer
{
public:
	// �v���C���[�̏��
	typedef enum
	{
		PLAYER_STATE_NORMAL = 0,		// �ʏ�
		PLAYER_STATE_FLYING,			// �������
		PLAYER_STATE_MAX,
	}Player_State;

	CPlayer_Game(int nPriority);
	~CPlayer_Game();

	static CPlayer_Game *Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, int nNumber);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	int GetDeleteGraffiti(void) { return m_nCntDeleteGraffiti; }
	int GetCntSkillGauge(void) { return m_nCntSkillGauge; }
	CScoreGauge *GetScoreGauge(void) { return m_pScoreGauge; }
	void SetScoreGauge(CScoreGauge *pScoreGauge) { m_pScoreGauge = pScoreGauge; }
	CSkillGauge *GetSkillGauge(void) { return m_pSkillGauge; }
	void SetSkillGauge(CSkillGauge *pSkillGauge) { m_pSkillGauge = pSkillGauge; }
	CPlayerMarker *GetPlayerMarker(void) { return m_pPlayerMarker; }
	void SetPlayerMarker(CPlayerMarker *pPlayerMarker, CCamera *pCamera);
	CEraser *GetEraser(void) { return m_pEraser; }
	void SetEraser(CEraser *pEraser);

	bool GetMaxSkillGauge(void) { return m_bMaxSkillGauge; }
	void SetMaxSkillGauge(bool bMax) { m_bMaxSkillGauge = bMax; }

	int GetDeleteGraffitiNum(void) { return m_nCntDeleteGraffiti; }

	D3DXVECTOR3 GetMove(void) { return m_move; }
	void SetMove(D3DXVECTOR3 move) { m_move = move; }
	void SetRank(int nRank) { m_nRank = nRank; }

private:
	void Pause(void);
	void Move(void);						// �ړ�����
	void MoveKeyboard(void);				// �v���C���[�̃L�[�{�[�h����
	bool SetMotion(MotionType motionType);	// ���[�V�����̐ݒ�
	bool CollisionStage(void);				// �X�e�[�W�Ƃ̓����蔻��
	void CollisionGraffiti(void);			// ���N�K�L�Ƃ̓����蔻��
	void CollisionEraserDust(void);			// �����J�X�Ƃ̓����蔻��
	void CollisionSkill(void);				// �X�L���Ƃ̓����蔻��
	void CollisionPlayer(void);				// �v���C���[���m�̓����蔻��
	void LoadEraser(void);

	CEraser *m_pEraser;						// �����S�����̃|�C���^

	D3DXVECTOR3 m_move;						// �ړ���
	bool m_bCleaner;						// �����Ă�r�����ǂ���
	bool m_bSkill;							// �X�L�����������ǂ���
	bool m_bColEraser;						// �����S�����X�e�[�W�Ɠ����������ǂ���
	bool m_bColSkill;						// �X�L���������������ǂ���

	int m_nRank;							// ����
	int m_nCntDeleteGraffiti;				// ���������N�K�L�̐����J�E���g
	int m_nCntSkillGauge;					// �X�L���Q�[�W�p�̃J�E���g
	CScoreGauge *m_pScoreGauge;				// �X�R�A�Q�[�W
	CSkillGauge *m_pSkillGauge;				// �X�L���Q�[�W
	bool m_bMaxSkillGauge;					// �X�L���Q�[�W�����܂������ǂ���

	CPlayerMarker *m_pPlayerMarker;			// �v���C���[�}�[�J�[���̃|�C���^
	CShadow *m_pShadow;
};
#endif