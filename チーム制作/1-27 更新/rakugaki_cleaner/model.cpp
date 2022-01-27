//=============================================================================
//
// ���f������ [model.cpp]
// Author : 
//
//=============================================================================
#include "model.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "shadow.h"

#include "debugproc.h"

//*****************************************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************************************

//=============================================================================
// CModel�̃R���X�g���N�^
//=============================================================================
CModel::CModel()
{
	// �ϐ��̏�����
}

//=============================================================================
// CModel�̃f�X�g���N�^
//=============================================================================
CModel::~CModel()
{

}

//=============================================================================
// ��������
//=============================================================================
CModel *CModel::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, char *pModelFileName)
{
	// �|�C���^�ϐ��̐���
	CModel *pModel;
	pModel = new CModel;

	// NULL�`�F�b�N
	if (pModel != NULL)
	{
		// ����������
		pModel->Init(pos, rot,pModelFileName);
	}

	return pModel;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CModel::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, char *pModelFileName)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �ϐ��̎󂯓n���A������
	pDevice = CManager::GetRenderer()->GetDevice();

	// �e�N�X�`���Ǘ����̎擾
	CTexture *pTexture = CManager::GetTexture();

	// ���̏�����
	m_pMesh = NULL;
	m_pBuffMat = NULL;
	m_nNumMat = NULL;
	m_pos = pos;
	m_rot = rot;

	// X�t�@�C���̓ǂݍ���
	if (FAILED(D3DXLoadMeshFromX(pModelFileName,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pBuffMat,
		NULL,
		&m_nNumMat,
		&m_pMesh)))
	{
		return E_FAIL;
	}

	// �ϐ��錾
	D3DXMATERIAL *pMat;		// �}�e���A���f�[�^�ւ̃|�C���^

	// �}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	// �������̊m��
	m_pTex = new int[m_nNumMat];

	for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
	{
		if (pMat[nCntMat].pTextureFilename != NULL)
		{
			if (pTexture != NULL)
			{
				m_pTex[nCntMat] = pTexture->SetTexture(pMat[nCntMat].pTextureFilename);
			}
		}
		else
		{
			m_pTex[nCntMat] = -1;
		}
	}

	// �e���̐���
	//m_pShadow = CShadow::Create({ 0.0f,0.0f,0.0f }, {0.0f,0.0f});

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CModel::Uninit(void)
{
	// ���b�V�����̊J��
	if (m_pBuffMat != NULL)
	{
		m_pBuffMat->Release();
		m_pBuffMat = NULL;
	}

	// �}�e���A�����̊J��
	if (m_pMesh != NULL)
	{
		m_pMesh->Release();
		m_pMesh = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void CModel::Update(void)
{

}

//=============================================================================
// �`�揈��
//=============================================================================
void CModel::Draw(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;		// �f�o�C�X�ւ̃|�C���^
	D3DXMATRIX mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;			// ���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;				// �}�e���A���f�[�^�ւ̃|�C���^
	D3DXMATRIX mtxParent;			// �e���f���̃}�g���b�N�X

	// �ϐ��̎󂯓n��
	pDevice = CManager::GetRenderer()->GetDevice();

	// �e�N�X�`���Ǘ����̎擾
	CTexture *pTexture = CManager::GetTexture();

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	if (m_pParent != NULL)
	{
		// �e������Ƃ��e���f���̃}�g���b�N�X���擾����
		mtxParent = m_pParent->GetMtxWorld();
	}
	else
	{
		// �e�����Ȃ��Ƃ��ŐV�̃}�g���b�N�X�����擾
		pDevice->GetTransform(D3DTS_WORLD, &mtxParent);
	}
	// �e�̃}�g���b�N�X���������킹��
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxParent);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	// �}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
	{
		// �}�e���A���̐ݒ�
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		if (m_pTex[nCntMat] != -1)
		{
			if (pTexture != NULL)
			{
				pDevice->SetTexture(0, pTexture->GetAddress(m_pTex[nCntMat]));
			}
		}
		else
		{
			pDevice->SetTexture(0, NULL);
		}
		// ���f��(�p�[�c)�̕`��
		m_pMesh->DrawSubset(nCntMat);
	}

	// �ۑ����Ă����}�e���A����߂�
	pDevice->SetMaterial(&matDef);
}