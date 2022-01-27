//=============================================================================
//
// �v���C���[���� [player.h]
// Author : 
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "scene.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CModel;			// ���f���N���X
class CMotion;			// ���[�V�����N���X

//-----------------------------------------------------------------------------
// �v���C���[�N���X(�h���N���X)
//-----------------------------------------------------------------------------
class CPlayer : public CScene
{
public:
	// ���[�V�����̎��
	typedef enum
	{
		MOTIONTYPE_NEUTORAL = 0,	// �j���[�g����
		MOTIONTYPE_MOVE,			// �ړ�
		MOTIONTYPE_DELETE,			// ��������
		MOTIONTYPE_SKILL,			// �X�L��
		MOTIONTYPE_VICTORY,			// ����
		MOTIONTYPE_LOSE,			// �s�k
		MOTIONTYPE_MAX,
	}MotionType;

	CPlayer(int nPriority);
	~CPlayer();

	static CPlayer *Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, int nNumber);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	CModel *GetModel(int nCnt) { return m_apModel[nCnt]; }
	MotionType GetMotionType(void) { return m_motionType; }
	int GetNumber(void) { return m_nNumber; }
	D3DXVECTOR3 GetOffsetPos(int nCnt) { return D3DXVECTOR3(m_aOffset[nCnt][0], m_aOffset[nCnt][1], m_aOffset[nCnt][2]); }
	D3DXVECTOR3 GetOffsetRot(int nCnt) { return D3DXVECTOR3(m_aOffset[nCnt][3], m_aOffset[nCnt][4], m_aOffset[nCnt][5]); }
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	D3DXVECTOR3 GetRotDest(void) { return m_rotDest; }
	D3DXVECTOR3 GetSize(void) { return m_size; }
	bool GetFinishMotion(void) { return m_bFinishMotion; }

	void SetMotionType(MotionType motiontype) { m_motionType = motiontype; }
	void SetNumber(int nNumber) { m_nNumber = nNumber; }
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
	void SetRotDest(D3DXVECTOR3 rotDest) { m_rotDest = rotDest; }
	void SetFinishMotion(bool bFinish) { m_bFinishMotion = bFinish; }

protected:
	void Motion(void);						// ���[�V����
	CMotion *m_pMotion;						// ���[�V�������̃|�C���^

private:
	CModel *m_apModel[10];					// ���f���ւ̃|�C���^
	int m_nIndexParent[10];					// �e���f����INDEX
	float m_aOffset[10][6];					// �e���f������̃I�t�Z�b�g
	char m_apModelFileName[10][128];		// ���f���t�@�C���l�[��
	int m_nNumModel;						// ���f����

	MotionType m_motionType;				// ���[�V�����̎��

	D3DXMATRIX m_mtxWorld;					// ���[���h�}�g���b�N�X
	D3DXVECTOR3 m_pos;						// ���݂̈ʒu
	D3DXVECTOR3 m_posOld;					// �O��̈ʒu
	D3DXVECTOR3 m_size;						// �T�C�Y
	D3DXVECTOR3 m_rot;						// ����
	D3DXVECTOR3 m_rotDest;					// �ړI�̌���

	int m_nNumber;							// ���Ԗڂ̃v���C���[��
	bool m_bFinishMotion;					// ���[�v���Ȃ����[�V�������I��������ǂ���
};
#endif