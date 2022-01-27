//=============================================================================
//
// ���S���� [logo.h]
// Author : 
//
//=============================================================================
#ifndef _LOGO_H_
#define _LOGO_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define FVF_VERTEX_2D (D3DFVF_XYZRHW | D3DFVF_DIFFUSE |D3DFVF_TEX1) // ���_�t�H�[�}�b�g
#define MAX_VERTEX (4)												// ���_�̐�

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef  struct
{
	D3DXVECTOR3 pos; // ���_���W
	float rhw;	     // 1.0�ŌŒ�
	D3DCOLOR col;    // ���_�J���[
	D3DXVECTOR2 tex;
} VERTEX_2D;

//-----------------------------------------------------------------------------
// �I�u�W�F�N�g�N���X(��{�N���X)
//-----------------------------------------------------------------------------
class CLogo
{
public:
	CLogo();
	~CLogo();

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void BindTexture(LPDIRECT3DTEXTURE9 pTexture);

	D3DXVECTOR3 GetPosition(void) { return m_CorLogoInfo.pos; }
	D3DXVECTOR2 GetSize(void) { return m_CorLogoInfo.size; }
	D3DXCOLOR GetCol(void) { return m_CorLogoInfo.col; }
	void SetPosition(D3DXVECTOR3 pos);
	void SetSize(D3DXVECTOR2 size);
	void SetRotate(float fAngle);
	void SetVertex(void);
	void SetTex(D3DXVECTOR2 texUV, D3DXVECTOR2 texAnimetionSheets);
	void SetCol(D3DXCOLOR col);

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff = NULL;	// ���_�o�b�t�@�ւ̃|�C���^
	float m_fAngle;								// �|���S���̑Ίp���̊p�x
	float m_fLength;							// �|���S���̑Ίp���̒���
	float m_fWidth;								// �|���S���̕�
	float m_fHeight;							// �|���S���̍���

	int m_nTex;									// �e�N�X�`���ԍ�

	// ���S�ɕK�v�ȏ��
	typedef struct
	{
		D3DXVECTOR3 pos;	// �ʒu
		D3DXVECTOR3 rot;	// ����
		D3DXVECTOR2 size;	// �T�C�Y
		D3DXCOLOR col;		// �F
		D3DXVECTOR2 tex;	// �e�N�X�`�����W
	}LogoInfo;

	LogoInfo m_CorLogoInfo;	// ���݂̃��S���
	LogoInfo m_DefLogoInfo;	// �f�t�H���g(����)�̃��S���
};
#endif