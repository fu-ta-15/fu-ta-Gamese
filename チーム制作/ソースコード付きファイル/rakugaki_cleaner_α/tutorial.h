//=============================================================================
//
// ���[�h��ʃw�b�_�[ (tutorial.h)
// Author:Itsuki Takaiwa
//
//=============================================================================
#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#include "scene.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_CURSOR	(TUTORIALUI_MAX - 1)		// �ő�J�[�\����
#define MAX_ARROW	(2)							// �y�[�W����p���

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CScene2D;			// �|���S��2D�N���X
class CLogo;			// ���S�N���X

//*****************************************************************************
// �`���[�g���A����ʃN���X(�h���N���X)
//*****************************************************************************
class CTutorial : public CScene
{
public:
	CTutorial(int nPriolity);
	~CTutorial();

	static CTutorial *Create(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	bool LoadData(void);																// �`���[�g���A���̓ǂݍ���
	bool LoadLogo(FILE *pFile, int nNumTex, char **pTextureFileName);					// ���S
	bool LoadBg(FILE *pFile, int nNumTex, char **pTextureFileName);						// �w�i
	bool LoadTutorialSheet(FILE *pFile, int nNumTex, char **pTextureFileName);			// �`���[�g���A���V�[�g
	bool LoadLeftArrow(FILE *pFile, int nNumTex, char **pTextureFileName);				// �����
	bool LoadRightArrow(FILE *pFile, int nNumTex, char **pTextureFileName);				// �E���

	bool Logo(FILE *pFile, int nNumTex, char **pTextureFileName);					// ���S
	bool TuotorialSheet(FILE *pFile, int nNumTex, char **pTextureFileName);				// �w�i

	CScene2D *m_pBg;									// �w�i���̃|�C���^
	CScene2D **m_pTutorial;								// �`���[�g���A�����̐擪�A�h���X��ۑ�����|�C���^
	CScene2D *m_pLeftArrow;								// �y�[�W����p�������̃|�C���^
	CScene2D *m_pRightArrow;							// �y�[�W����p�E�����̃|�C���^
	CScene2D **m_pLogo;									// ���S�̃|�C���^
	int m_nCursor;										// �J�[�\���̈ʒu
	int m_nNumTutorial;									// �`���[�g���A���̖���
	int m_nNumLogo;										// ���S�̌�

	int m_nCntLogo;										// ���S�̃J�E���g
	int m_nCntTutorial;									// �`���[�g���A���̃J�E���g
};

#endif
