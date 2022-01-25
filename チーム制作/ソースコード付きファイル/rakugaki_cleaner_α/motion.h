//=============================================================================
//
// �f�o�b�N�v���b�N�w�b�_�[ (debugproc.h)
// Author:Itsuki Takaiwa
//
//=============================================================================
#ifndef _MOTION_H_
#define _MOTION_H_

#include "main.h"

//*****************************************************************************
// ���[�V�����N���X(��{�N���X)
//*****************************************************************************
class CMotion
{
public:
	// ���[�V�����̎��
	typedef enum
	{
		MOTIONTYPE_PLAYER = 0,
		MOTIONTYPE_MAX,
	}MotionType;

	// �L-
	typedef struct
	{
		float fPosX;			// �ʒuX
		float fPosY;			// �ʒuY
		float fPosZ;			// �ʒuZ
		float fRotX;			// ����X
		float fRotY;			// ����Y
		float fRotZ;			// ����Z
	}KEY;						

	// �L�[���
	typedef struct
	{
		int nFrame;				// �t���[����
		KEY aKey[10];			// �e���f���̃L�[�v�f
	}KEY_INFO;					

	// ���[�V�������
	typedef struct
	{
		bool bLoop;				// ���[�v���邩�ǂ���
		int nNumKey;			// �L�[�̑���
		KEY_INFO aKeyInfo[5];	// �L�[���
	}MOTION_INFO;				

	typedef struct
	{
		int nKey;				// �L�[No.
		int nCounterMotion;		// ���[�V�����J�E���^
	}MOTION;

	CMotion();
	~CMotion();

	static CMotion *Create(void);
	void Init(void);
	static void LoadMotionData(void);

	static MOTION_INFO *GetMotionInfo(int nNum) { return &m_aMotionInfo[nNum][0]; }
	MOTION GetMotion(void) { return m_motion; }
	void SetMotion(MOTION motion) { m_motion = motion; }

private:
	static char m_aTextMotion[MOTIONTYPE_MAX][128];
	static MOTION_INFO m_aMotionInfo[MOTIONTYPE_MAX][10];		// ���[�V�������̃|�C���^
	MOTION m_motion;											// ���[�V�����̏��
};

#endif