//=============================================================================
//
// ���U���g�V�[���̃v���C���[���� [player_result.h]
// Author : 
//
//=============================================================================
#ifndef _PLAYERREULT_H_
#define _PLAYERREULT_H_

#include "player.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CPlayerMarker;	// �v���C���[�}�[�J�[�N���X
class CRank;			// �����N�N���X
class CCamera;

//-----------------------------------------------------------------------------
// �v���C���[�N���X(�h���N���X)
//-----------------------------------------------------------------------------
class CPlayer_Result : public CPlayer
{
public:
	CPlayer_Result(int nPriority);
	~CPlayer_Result();

	static CPlayer_Result *Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, int nNumber);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	CPlayerMarker *GetPlayerMarker(void) { return m_pPlayerMarker; }
	void SetPlayerMarker(CPlayerMarker *pPlayerMarker, CCamera *pCamera);
	void SetRank(CRank *pRank, int nRank, CCamera *pCamera);

	bool SetMotion(MotionType motionType);	// ���[�V�����̐ݒ�

private:
	bool CollisionStage(void);				// �X�e�[�W�̓����蔻��

	D3DXVECTOR3 m_move;
	CPlayerMarker *m_pPlayerMarker;			// �v���C���[�}�[�J�[���̃|�C���^
	CRank *m_pRank;
};
#endif