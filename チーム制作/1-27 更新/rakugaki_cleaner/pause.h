//=============================================================================
//
// �|�[�Y�w�b�_�[ (pause.h)
// Author:Itsuki Takaiwa
//
//=============================================================================
#ifndef _PAUSE_H_
#define _PAUSE_H_

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#include "scene.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CScene2D;			// �|���S��2D�N���X
class CPlayerMarker;	// �v���C���[�}�[�J�[

//*****************************************************************************
// �|�[�Y��ʃN���X(�h���N���X)
//*****************************************************************************
class CPause : public CScene
{
public:
	CPause(int nPriolity);
	~CPause();

	static CPause *Create(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetPause();
	void SetPause(int nNumPlayer);
	void SetPause(bool bPause) { m_bPause = bPause; }

private:
	bool m_bPause;
	int m_nNumController;

	bool LoadData(void);

	bool LoadPlayerMarkerData(FILE *pFile, int nNumTex, char **pTextureFileName);
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
	CScene2D *m_pPlayerMarker;			// �v���C���[�}�[�J�[���̃|�C���^
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
};
#endif