//=============================================================================
//
// ���f�� (model.cpp)
// Author:Itsuki Takaiwa
//
//=============================================================================
#include "sceneX.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//=============================================================================
// CSceneX�̃R���X�g���N�^
//=============================================================================
CSceneX::CSceneX(int nPriority) : CScene(nPriority)
{
	// �����o�ϐ��̏�����
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//=============================================================================
// CSceneX�̃f�X�g���N�^
//=============================================================================
CSceneX::~CSceneX()
{

}

//=============================================================================
// ��������
//=============================================================================
CSceneX *CSceneX::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, D3DXVECTOR3 rot, char *pModelFileName)
{
	// �|�C���^�ϐ��̐���
	CSceneX *pSceneX;
	pSceneX = new CSceneX(CScene::PRIORITY_MODEL);

	// NULL�`�F�b�N
	if (pSceneX != NULL)
	{
		// ���Ԗڂ��ۑ�
		pSceneX->BindXFileName(pModelFileName);

		// �����̕ۑ�
		pSceneX->SetRotate(rot);

		// ����������
		pSceneX->Init(pos, size);

		// �I�u�W�F�N�g�^�C�v�̐ݒ�
		pSceneX->SetObjType(OBJTYPE_MODEL);
	}

	return pSceneX;
}

//=============================================================================
// �V�[��X�̏���������
//=============================================================================
HRESULT CSceneX::Init(D3DXVECTOR3 pos, D3DXVECTOR2 size)
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
	m_vtxMaxModel = D3DXVECTOR3(-1000.0f, -1000.0f, -1000.0f);
	m_vtxMinModel = D3DXVECTOR3(1000.0f, 1000.0f, 1000.0f);

	// X�t�@�C���̓ǂݍ���
	if (FAILED(D3DXLoadMeshFromX(m_pModelFileName,
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

	m_pTex = new int[m_nNumMat];

	for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
	{
		if (pMat[nCntMat].pTextureFilename != NULL)
		{
			m_pTex[nCntMat] = pTexture->SetTexture(pMat[nCntMat].pTextureFilename);
		}
		else
		{
			m_pTex[nCntMat] = -1;
		}
	}

	// �ϐ��錾
	int nNumVtx;			// ���_��
	DWORD sizeFVF;			// ���_�t�H�[�}�b�g�̃T�C�Y
	BYTE *pVtxBuff;			// ���_�o�b�t�@�ւ̃|�C���^

							// ���_���̎擾
	nNumVtx = m_pMesh->GetNumVertices();

	// ���_�t�H�[�}�b�g�̃T�C�Y���擾
	sizeFVF = D3DXGetFVFVertexSize(m_pMesh->GetFVF());

	// ���_�o�b�t�@�̃��b�N
	m_pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		// ���_���W�̑��
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

		// ���_���r���ă��f���̍ŏ��l�ő�l�𔲂��o��
		if (m_vtxMinModel.x > vtx.x)
		{// X���W�̍ŏ��l
			m_vtxMinModel.x = vtx.x;
		}
		else if (m_vtxMaxModel.x < vtx.x)
		{// X���W�̍ő�l
			m_vtxMaxModel.x = vtx.x;
		}

		if (m_vtxMinModel.y > vtx.y)
		{// Y���W�̍ŏ��l
			m_vtxMinModel.y = vtx.y;
		}
		else if (m_vtxMaxModel.y < vtx.y)
		{// Y���W�̍ő�l
			m_vtxMaxModel.y = vtx.y;
		}

		if (m_vtxMinModel.z > vtx.z)
		{// Z���W�̍ŏ��l
			m_vtxMinModel.z = vtx.z;
		}
		else if (m_vtxMaxModel.z < vtx.z)
		{// Z���W�̍ő�l
			m_vtxMaxModel.z = vtx.z;
		}
		// ���_�t�H�[�}�b�g�̃T�C�Y���|�C���^��i�߂�
		pVtxBuff += sizeFVF;
	}
	// ���_�o�b�t�@�̃A�����b�N
	m_pMesh->UnlockVertexBuffer();

	if (m_vtxMaxModel.x < 0.0f)
	{
		m_vtxMaxModel.x *= -1.0f;
	}
	if (m_vtxMaxModel.y < 0.0f)
	{
		m_vtxMaxModel.y *= -1.0f;
	}
	if (m_vtxMaxModel.z < 0.0f)
	{
		m_vtxMaxModel.z *= -1.0f;
	}

	if (m_vtxMinModel.x < 0.0f)
	{
		m_vtxMinModel.x *= -1.0f;
	}
	if (m_vtxMinModel.y < 0.0f)
	{
		m_vtxMinModel.y *= -1.0f;
	}
	if (m_vtxMinModel.z < 0.0f)
	{
		m_vtxMinModel.z *= -1.0f;
	}

	return S_OK;
}

//=============================================================================
// �V�[��X�̏I������
//=============================================================================
void CSceneX::Uninit(void)
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
	// �j���@
	Release();
}

//=============================================================================
// �V�[��X�̍X�V����
//=============================================================================
void CSceneX::Update(void)
{

}

//=============================================================================
// �V�[��X�̕`�揈��
//=============================================================================
void CSceneX::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice;		// �f�o�C�X�ւ̃|�C���^
	D3DXMATRIX mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;			// ���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;				// �}�e���A���f�[�^�ւ̃|�C���^

	// �ϐ��̎󂯓n��
	pDevice = CManager::GetRenderer()->GetDevice();

	// �e�N�X�`���Ǘ����̎擾
	CTexture *pTexture = CManager::GetTexture();

	//if (m_pModelFileName != NULL)
	{
		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&m_mtxWorld);

		// �����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

		// �ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

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
				pDevice->SetTexture(0, pTexture->GetAddress(m_pTex[nCntMat]));
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
}

//=============================================================================
// �V�[��X�̃}�e���A���ݒ�
//=============================================================================
void CSceneX::SetMat(D3DXMATERIAL *pMat,int nCntMat)
{
	// �ϐ��̎󂯓n��
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// ���݂̃}�e���A���ۑ��p
	D3DMATERIAL9 matDef;			

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	// �}�e���A���̐ݒ�
	pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

	// �ۑ����Ă����}�e���A����߂�
	pDevice->SetMaterial(&matDef);
}

//=============================================================================
// �V�[��X�̃}�e���A�����̎擾
//=============================================================================
D3DXMATERIAL *CSceneX::GetMat(void)
{
	// �ϐ��̎󂯓n��
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �}�e���A���f�[�^�ւ̃|�C���^
	D3DXMATERIAL *pMat;	

	// �}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	return pMat;
}

//=============================================================================
// �V�[��X�̉�]�l�ݒ�
//=============================================================================
void CSceneX::SetRotate(D3DXVECTOR3 rot)
{
	// �p�x�̐���
	// x��
	if (rot.x >= D3DX_PI)
	{
		rot.x -= D3DX_PI * 2.0f;
	}
	else if (rot.x <= -D3DX_PI)
	{
		rot.x += D3DX_PI * 2.0f;
	}

	// y��
	if (rot.y >= D3DX_PI)
	{
		rot.y -= D3DX_PI * 2.0f;
	}
	else if (rot.y <= -D3DX_PI)
	{
		rot.y += D3DX_PI * 2.0f;
	}

	// z��
	if (rot.z >= D3DX_PI)
	{
		rot.z -= D3DX_PI * 2.0f;
	}
	else if (rot.z <= -D3DX_PI)
	{
		rot.z += D3DX_PI * 2.0f;
	}

	// �p�x�̐ݒ�
	m_rot = rot;
}