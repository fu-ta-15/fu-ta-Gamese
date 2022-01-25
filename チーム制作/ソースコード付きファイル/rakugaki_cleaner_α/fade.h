//=============================================================================
//
// �t�F�[�h�w�b�_�[ (fade.h)
// Author:Itsuki Takaiwa
//
//=============================================================================
#ifndef _FADE_H_
#define _FADE_H_

#include "manager.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CBase;			// �x�[�X�V�[���N���X

//*****************************************************************************
// �t�F�[�h�N���X(��{�N���X)
//*****************************************************************************
class CFade
{
public:
	// �t�F�[�h�R�}���h
	typedef enum
	{
		FADE_NONE = 0,	// �������Ă��Ȃ�
		FADE_IN,		// �t�F�[�h�C��
		FADE_OUT,		// �t�F�[�h�A�E�g
		FADE_MAX,
	}FADE;

	CFade();
	~CFade();

	HRESULT Init(CBase *pBase);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static void SetFade(FADE fade, CBase *pBase);
	static FADE GetFade(void);

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffFade;	// ���_�o�b�t�@�̃|�C���^
	static FADE m_fade;						// �t�F�[�h�̏��
	static CBase *m_pBaseNext;				// ���̃��[�h
	D3DXCOLOR m_colorFade;					// �t�F�[�h�F
};

#endif
