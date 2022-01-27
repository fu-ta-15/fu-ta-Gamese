//-------------------------------------------------------------------
//
// ���f���z�u�w�b�_�[ (set_model.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _SETMODEL_H_
#define _SETMODEL_H_

#include "main.h"

//-----------------------------------------------------------------
// �}�N����`
//-----------------------------------------------------------------
#define MAX_LIGHT		(3)				// ���C�g�̍ő吔

//-------------------------------------------------------------------
// ���f���z�u�N���X(��{�N���X)
//-------------------------------------------------------------------
class CSetModel
{
public:
	CSetModel();
	~CSetModel();

	void Init(void);
	static void Update(void);
	void LoadData(void);

private:
	bool LoadFloor(FILE *pFile, int nNumTex, char **pTextureFileName);		// ��
	bool LoadStage(FILE *pFile, int nNumModel, char **pModelFileName);
	bool LoadModel(FILE *pFile, int nNumModel, char **pModelFileName);

	static char *m_apTextName;				// �ǂݍ��ރe�L�X�g�̏��
	static char *m_apTextNameSetModel[64];	// ���f���z�u����ǂݍ��ރe�L�X�g�̏��
	static int m_nNumType;					// ���f���̎�ސ�
	static int m_nNumModel;					// ���f���̑���
};

#endif