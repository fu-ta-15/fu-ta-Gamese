//=============================================================================
//
// ���f������ [model.h]
// Author : 
//
//=============================================================================
#ifndef _MODEL_H_
#define _MODEL_H_

#include "sceneX.h"

//-----------------------------------------------------------------------------
// �O���錾
//-----------------------------------------------------------------------------
class CShadow;			// �e�N���X

//-----------------------------------------------------------------------------
// ���f���N���X(��{�N���X)
//-----------------------------------------------------------------------------
class CModel
{
public:
	CModel();
	~CModel();

	static CModel *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot,char *pModelFileName);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, char *pModelFileName);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetParent(CModel *pModel) { m_pParent = pModel; }		// �e���f���̐ݒ�
	D3DXMATRIX GetMtxWorld(void) { return m_mtxWorld; }			// ���[���h�}�g���b�N�X�擾
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
	void SetPosition(D3DXVECTOR3 pos) { m_pos = pos; }
	D3DXVECTOR3 GetPosition(void) { return m_pos; }

private:
	LPDIRECT3DTEXTURE9 m_pTexture[8];	// �e�N�X�`���ւ̃|�C���^
	LPD3DXMESH m_pMesh;					// ���b�V��(���_���)�ւ̃|�C���^
	LPD3DXBUFFER m_pBuffMat;			// �}�e���A��(�ގ����)�ւ̃|�C���^
	DWORD m_nNumMat;					// �}�e���A���̐�
	D3DXMATRIX m_mtxWorld;				// ���[���h�}�g���b�N�X
	D3DXVECTOR3 m_pos;					// ���S���W
	D3DXVECTOR3 m_rot;					// ��]�p
	CModel *m_pParent;					// �e���f���̃|�C���^

	int *m_pTex;						// �e�N�X�`���ԍ�
	CShadow *m_pShadow;					// �e�N���X
};
#endif