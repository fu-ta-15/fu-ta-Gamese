//-------------------------------------------------------------------
//
// �f�[�^�ǂݍ��݃w�b�_�[ (set_game.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _LOADPLAYERDETA_H_
#define _LOADPLAYERDETA_H_

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#include "main.h"

//-------------------------------------------------------------------
// �v���C���[�f�[�^�ǂݍ��݃N���X(��{�N���X)
//-------------------------------------------------------------------
class CLoadPlayerData
{
public:
	// �v���C���[�̎��
	typedef enum
	{
		PLAYER_NAME_CHUL = 0,
		PLAYER_NAME_KANO,
		PLAYER_NAME_RUI,
		PLAYER_NAME_MAHITO,
		PLAYER_NAME_MAX,
	}PLayer_Name;

	// �v���C���[���f�����
	typedef struct
	{
		int *nIndexParent;				// �e���f����INDEX
		float **aOffset;				// �e���f������̃I�t�Z�b�g
		char **apModelFileName;			// ���f���t�@�C���l�[��
		int nNumModel;					// ���f����
	}Player_Data;

	CLoadPlayerData();
	~CLoadPlayerData();

	void Init(void);
	void Uninit(void);
	Player_Data GetPlayerData(int nIdx) { return m_player_data[nIdx]; }

private:
	void LoadPlayerData(void);

	static char m_apTexName[PLAYER_NAME_MAX][128];		// �ǂݍ��ރe�L�X�g��
	Player_Data *m_player_data;					// �v���C���[�f�[�^���̃|�C���^
};

#endif