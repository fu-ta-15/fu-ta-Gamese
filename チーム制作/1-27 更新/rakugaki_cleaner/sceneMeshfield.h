//=============================================================================
//
// �V�[�����b�V���t�B�[���h���� [sceneMeshfield.h]
// Author : 
//
//=============================================================================
#ifndef _SCENEMESHFIELD_H_
#define _SCENEMESHFIELD_H_

#include "scene.h"

//-----------------------------------------------------------------------------
// �V�[�����b�V���t�B�[���h�N���X(��{�N���X)
//-----------------------------------------------------------------------------
class CSceneMeshfield : public CScene
{
public:
	typedef struct
	{
		D3DXMATRIX mtxWorld;		// ���[���h�}�g���b�N�X
		D3DXVECTOR3 pos;			// �ǂ̒��S���W
		D3DXVECTOR3 rot;			// �ǂ̌���
		int nCntBlockX;				// �u���b�N��(��)
		int nCntBlockZ;				// �u���b�N��(�s)
		int nCntVecX;				// ��̒��_��
		int nCntVecZ;				// �s�̒��_��
		float fWidth;				// ��
		float fDepth;				// ���s
		float fSizeWidth;			// ���̃T�C�Y
		float fSizeDepth;			// ���s�̃T�C�Y
	}MeshField;

	CSceneMeshfield(int nPriority);
	~CSceneMeshfield();

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void BindTextureNum(int nTex) { m_nTex = nTex; }

	D3DXVECTOR3 GetPosition(void) { return m_meshField.pos; }
	D3DXVECTOR2 GetSize(void) { return D3DXVECTOR2(m_meshField.fWidth, m_meshField.fDepth); }
	D3DXCOLOR GetCol(void) { return m_col; }
	void SetPosition(D3DXVECTOR3 pos);
	void SetSize(D3DXVECTOR2 size);
	void SetVertex(void);
	void SetTex(D3DXVECTOR2 texUV, D3DXVECTOR2 texAnimetionSheets);
	void SetCol(D3DXCOLOR col);
	void SetDeath(bool bDeath) { m_bDeath = bDeath; }
	void SetBlock(D3DXVECTOR2 block);
	D3DXVECTOR2 GetBlock(void) { return D3DXVECTOR2((float)m_meshField.nCntBlockX, (float)m_meshField.nCntBlockZ); }

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff = NULL;	// ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;			// �C���f�b�N�X�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DTEXTURE9 m_pTexture = NULL;		// �e�N�X�`�����̃|�C���^
	MeshField m_meshField;						// ���b�V���t�B�[���h���
	D3DXCOLOR m_col;							// �F���
	int m_nTex;

	bool m_bDeath;								// ���S�t���O
};
#endif