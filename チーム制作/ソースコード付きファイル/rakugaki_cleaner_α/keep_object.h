//=============================================================================
//
// �f�[�^�ۑ��p�w�b�_�[ (keep_object.h)
// Author:Itsuki Takaiwa
//
//=============================================================================
#ifndef _KEEPOBJECT_H_
#define _KEEPOBJECT_H_

#include "main.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CLoadPlayerData;		// �v���C���[�f�[�^�ǂݍ��݃N���X
class CMotion;				// ���[�V�����N���X

//*****************************************************************************
// �f�[�^�ۑ��p�N���X(��{�N���X)
//*****************************************************************************
class CKeepObject
{
public:
	// �Q�[�����[�h
	typedef enum
	{
		GAMEMODE_BATTLE = 0,
		GAMEMODE_COOPERATION,
		GAMEMODE_MAX,
	}GAMEMODE;

	CKeepObject();
	~CKeepObject();

	void Init(void);
	void Uninit(void);
	void Update(void);

	CLoadPlayerData *GetLoadPlayerData(void) { return m_pLoadPlayerData; }
	CMotion *GetMotionData(void) { return m_pMotionData; }
	int GetDeleteGraffiti(int nCnt) { return m_nNumDeleteGraffiti[nCnt]; }
	void SetDeleteGraffiti(int nNumDelete, int nCnt) { m_nNumDeleteGraffiti[nCnt] = nNumDelete; }
	int GetNumPlayer(void) { return m_nNumPlayer; }
	void SetNumPlayer(int nNumPlayer);
	int GetRank(int nCnt) { return m_nRank[nCnt]; }
	int GetNumRank(int nCnt) { return m_nNumberRank[nCnt]; }
	GAMEMODE GetGameMode(void) { return m_gamemode; }
	void SetGameMode(GAMEMODE gamemode) { m_gamemode = gamemode; }

	float GetAchivement(void) { return m_fNumAchivement; }
	void SetAchivement(float fNumAchivement) { m_fNumAchivement = fNumAchivement; }
	int GetTotalGraffiti(void) { return m_nNumTotalGraffiti;}
	void SetTotalGraffiti(int nTotal) { m_nNumTotalGraffiti = nTotal; }

	void DeleteMem(void);					// �������̍폜
	void Rank(void);						// �����L���O�m��
	void Achivement(void);					// �B�����̊m��

private:
	CLoadPlayerData *m_pLoadPlayerData;		// �v���C���[�f�[�^�ǂݍ��ݏ��̃|�C���^
	CMotion *m_pMotionData;					// ���[�V�����f�[�^�ǂݍ��ݏ��̃|�C���^
	int *m_nNumDeleteGraffiti;				// ���N�K�L�����������̐擪�A�h���X��ۑ�
	int m_nNumPlayer;						// �v���C���[�l���̕ۑ�
	int *m_nRank;							// ���ʂ�ۑ�����擪�A�h���X�̃|�C���^
	int *m_nNumberRank;						// ���ʂ�ۑ�����擪�A�h���X�̃|�C���^
	GAMEMODE m_gamemode;					// �Q�[�����[�h�ۑ��p

	float m_fNumAchivement;
	int m_nNumTotalGraffiti;
};

#endif