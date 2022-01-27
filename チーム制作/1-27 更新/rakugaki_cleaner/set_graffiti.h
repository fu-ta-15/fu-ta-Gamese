//-------------------------------------------------------------------
//
// ���N�K�L�z�u�w�b�_�[ (set_graffiti.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _SETGRAFFITI_H_
#define _SETGRAFFITI_H_

#include "main.h"

//-------------------------------------------------------------------
// ���N�K�L�z�u�N���X(��{�N���X)
//-------------------------------------------------------------------
class CSetGraffiti
{
public:
	// �z�u�p�^�[��
	typedef enum
	{
		SETTYPE_01 = 0,
		SETTYPE_02,
		SETTYPE_03,
		SETTYPE_04,
		SETTYPE_MAX,
	}SETTYPE;

	// 30x30
	typedef enum
	{
		TYPE_30_DUMPLING = 0,
		TYPE_30_MtFUJI,
		TYPE_30_STRAWBWRRY,
		TYPE_30_MaruBatsu,
		TYPE_30_CAT,
		TYPE_30_LABYRINCH,
		TYPE_30_REDBALL,
		TYPE_30_SNAKE,
		TYPE_30_TOMATO,
		TYPE_30_RAF00,
		TYPE_30_RAF01,
		TYPE_30_RAF02,
		TYPE_30_MAX,
	}TYPE_30;

	// 35x35
	typedef enum
	{
		TYPE_35_CLOCK = 0,
		TYPE_35_DEVIL,
		TYPE_35_GHOST,
		TYPE_35_OCTOPUS,
		TYPE_35_RAF00,
		TYPE_35_RAF01,
		TYPE_35_RAF02,
		TYPE_35_USAEBI,
		TYPE_35_MAX,
	}TYPE_35;

	// 40x40
	typedef enum
	{
		TYPE_40_RAF00 = 0,
		TYPE_40_RAF01,
		TYPE_40_RAF02,
		TYPE_40_JOBI,
		TYPE_40_MAX,
	}TYPE_40;

	// 45x45
	typedef enum
	{
		TYPE_45_CHURU = 0,
		TYPE_45_KANO,
		TYPE_45_RAF03,
		TYPE_45_RAF04,
		TYPE_45_RAF05,
		TYPE_45_MAX,
	}TYPE_45;

	CSetGraffiti();
	~CSetGraffiti();

	void Init(void);
	static void Update(void);
	static bool LoadGraffitiData(void);

private:
	static bool SetGraffiti(void);
	static bool LoadColInfo(void);
	static void RandomGraffiti(void);

	// ���N�K�L�z�u�����ɕK�v�ȍ\����
	typedef struct
	{
		char *pFileName;		// �t�@�C���l�[���i�[�p
		char *pColFileName;
		int nType;
		int nCntX;				// ��
		int nCntY;				// �c
		D3DXVECTOR3 pos;		// �ʒu
		float fSize;			// �T�C�Y
		int nNumCol;			// �F�̑���
		D3DXCOLOR *pCol;		// �F���̃|�C���^(�������𓮓I�Ɋm�ۂ���)
	}Graffiti_Info;

	static char *m_apTextName;					// �ǂݍ��ރe�L�X�g�̏��

	static Graffiti_Info *m_graffiti_info;		// ���N�K�L�̏ڍ׏��
	
	static char m_pFileName_30[TYPE_30_MAX][128];
	static char m_pFileName_35[TYPE_35_MAX][128];
	static char m_pFileName_40[TYPE_40_MAX][128];
	static char m_pFileName_45[TYPE_45_MAX][128];
	static char m_pFileName_SetPattern[SETTYPE_MAX][128];
};

#endif