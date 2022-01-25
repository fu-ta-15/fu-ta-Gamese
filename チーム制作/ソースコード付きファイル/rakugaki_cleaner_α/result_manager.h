//=============================================================================
//
// ���U���g��ʊǗ����� [result_manager.h]
// Author : itsuki takaiwa
//
//=============================================================================
#ifndef _RESULTMANAGER_H_
#define _RESULTMANAGER_H_

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#include "scene.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CScene2D;			// �|���S��2D�N���X

//*****************************************************************************
// ���U���g��ʊǗ��N���X(�h���N���X)
//*****************************************************************************
class CResultManager : public CScene
{
public:
	CResultManager(int nPriority = PRIORITY_UI);
	~CResultManager();

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	bool LoadData(void);

	bool LoadLogo(FILE *pFile, int nNumTex, char **pTextureFileName);		// ���̑����S
	bool LoadBg(FILE *pFile, int nNumTex, char **pTextureFileName);			// �w�i
	bool LoadPauseMenu(FILE *pFile, int nNumTex, char **pTextureFileName);	// �|�[�Y���j���[

	bool Bg(FILE *pFile, int nNumTex, char **pTextureFileName);				// �w�i
	bool MenuBg(FILE *pFile, int nNumTex, char **pTextureFileName);			// ���j���[�w�i
	bool MenuLogo(FILE *pFile, int nNumTex, char **pTextureFileName);		// ���j���[���S
	bool MenuLogoBg(FILE *pFile, int nNumTex, char **pTextureFileName);		// ���j���[���S�w�i

	CScene2D **m_pBg;					// �w�i���
	CScene2D **m_pPauseMenuBg;			// �|�[�Y���j���[�w�i���̐擪�A�h���X��ۑ�����|�C���^
	CScene2D **m_pPauseMenuLogo;		// �|�[�Y���j���[���S���̐擪�A�h���X��ۑ�����|�C���^
	CScene2D **m_pPauseMenuLogoBg;		// �|�[�Y���j���[���S�w�i���̐擪�A�h���X��ۑ�����|�C���^
	CScene2D **m_pLogo;					// ���̑����S���̐擪�A�h���X��ۑ�����|�C���^
	int m_nPauseSelect;					// �|�[�Y���j���[����J�ڂ���ꏊ��ۑ�

	int m_nNumBg;						// �w�i��
	int m_nNumMenuBg;					// �|�[�Y���j���[�w�i��
	int m_nNumLogo;						// ���S��
	int m_nNumLogoBg;					// ���S�w�i
	int m_nLogo;						// ���̑����S

	int m_nCntBg;
	int m_nCntMenuBg;
	int m_nCntMenuLogo;
	int m_nCntMenuLogoBg;
	int m_nCntLogo;

	bool m_bOK;

	int m_nCounter;
	CScene2D *m_pLogoNext;
};
#endif