//==============================================================================================================
//
// ���b�V���t�B�[���h (meshfield.cpp)
// Author:Itsuki Takaiwa
//
//==============================================================================================================
#include "sceneMeshfield.h"
#include "manager.h"
#include "renderer.h"
#include "debugproc.h"

#include "texture.h"

//--------------------------------------------------------------------------------------------------------------
// �}�N����`
//--------------------------------------------------------------------------------------------------------------
#define WIDTH_NUM			(m_meshField.nCntBlockX)										// ��
#define DEPTH_NUM			(m_meshField.nCntBlockZ)										// �s
#define WIDTH_VEC			(WIDTH_NUM + 1)								// ��̒��_��
#define DEPTH_VEC			(DEPTH_NUM + 1)								// �s�̒��_��

#define MAX_MF_VERTEX		(DEPTH_NUM * 2 * (WIDTH_NUM + 2) - 2)		// �����_��
#define MAX_INDEX			(WIDTH_NUM * DEPTH_NUM * 2 + (4 * (DEPTH_NUM - 1)))	// ���C���f�b�N�X��
#define MAX_MESHFIELD		((WIDTH_NUM + 1) * (DEPTH_NUM + 1))					// 

//=============================================================================
// CScene�̃R���X�g���N�^
//=============================================================================
CSceneMeshfield::CSceneMeshfield(int nPriority) : CScene(nPriority)
{
	// �����o�ϐ��̏�����
	m_meshField.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_meshField.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_meshField.nCntBlockX = 1;
	m_meshField.nCntBlockZ = 1;
	m_meshField.fWidth = 0.0f;
	m_meshField.fDepth = 0.0f;
	m_meshField.fSizeWidth = m_meshField.nCntBlockX * m_meshField.fWidth;
	m_meshField.fSizeDepth = m_meshField.nCntBlockZ * m_meshField.fDepth;
}

//=============================================================================
// CScene�̃f�X�g���N�^
//=============================================================================
CSceneMeshfield::~CSceneMeshfield()
{

}

//--------------------------------------------------------------------------------------------------------------
// ���b�V���t�B�[���h�̏���������
//--------------------------------------------------------------------------------------------------------------
HRESULT CSceneMeshfield::Init(D3DXVECTOR3 pos,D3DXVECTOR2 size)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �ϐ��̎󂯓n���A������
	pDevice = CManager::GetRenderer()->GetDevice();

	// ���̏�����
	m_meshField.pos = pos;
	m_meshField.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_meshField.fWidth = size.x;
	m_meshField.fDepth = size.y;
	m_meshField.fSizeWidth = m_meshField.nCntBlockX * m_meshField.fWidth;
	m_meshField.fSizeDepth = m_meshField.nCntBlockZ * m_meshField.fDepth;

	// ���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * MAX_MESHFIELD,	// �m�ۂ���o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,									// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL)))
	{
		return E_FAIL;
	}

	// �\���̂̃|�C���^�ϐ�
	VERTEX_3D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntZ = 0; nCntZ < DEPTH_VEC; nCntZ++)
	{
		for (int nCntX = 0; nCntX < WIDTH_VEC; nCntX++)
		{
			// ���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(-m_meshField.fSizeWidth / 2.0f + (float)nCntX * m_meshField.fWidth,
				0.0f,
				m_meshField.fSizeDepth / 2.0f - (float)nCntZ * m_meshField.fDepth);

			// �@���x�N�g���̐ݒ�
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			// ���_�J���[�̐ݒ�
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// �e�N�X�`�����W
			pVtx[0].tex1 = D3DXVECTOR2(1.0f * nCntX, 1.0f * nCntZ);

			// ���̏��
			pVtx++;
		}
	}

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

	// �C���f�b�N�X�o�b�t�@�̐���
	if (FAILED(pDevice->CreateIndexBuffer(
		sizeof(WORD) * MAX_MF_VERTEX,	// �m�ۂ���o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,						// �t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		NULL)))
	{
		return E_FAIL;
	}

	// �C���f�b�N�X���ւ̃|�C���^
	WORD *pIdx;

	// �C���f�b�N�X�o�b�t�@�����b�N���A�ԍ��f�[�^�ւ̃|�C���^���擾
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCntZ = 0; nCntZ < DEPTH_NUM; nCntZ++)
	{
		for (int nCntX = 0; nCntX < WIDTH_VEC + 1; nCntX++)
		{
			if (nCntX != 0 && nCntX == WIDTH_VEC && nCntZ != DEPTH_NUM - 1)
			{// �E�[����܂�Ԃ���
				pIdx[0] = pIdx[-1];
				pIdx[1] = pIdx[-2] + 1;
			}
			else if (nCntZ == DEPTH_NUM - 1 && nCntX == WIDTH_VEC)
			{// �I�����ɖ�������
				break;
			}
			else
			{// �ʏ�z�u
				pIdx[0] = WIDTH_VEC + (WIDTH_VEC * nCntZ) + nCntX;
				pIdx[1] = pIdx[0] - WIDTH_VEC;
			}
			// ���̏��Ɉړ�
			pIdx += 2;
		}
	}

	// �C���f�b�N�X�o�b�t�@���A�����b�N����
	m_pIdxBuff->Unlock();

	return S_OK;
}

//--------------------------------------------------------------------------------------------------------------
// ���b�V���t�B�[���h�̏I������
//--------------------------------------------------------------------------------------------------------------
void CSceneMeshfield::Uninit(void)
{
	if (m_bDeath == true)
	{
		// �C���f�b�N�X�̊J��
		if (m_pIdxBuff != NULL)
		{
			m_pIdxBuff->Release();
			m_pIdxBuff = NULL;
		}

		// ���_�o�b�t�@�̊J��
		if (m_pVtxBuff != NULL)
		{
			m_pVtxBuff->Release();
			m_pVtxBuff = NULL;
		}

		// �e�N�X�`���̊J��
		if (m_pTexture != NULL)
		{
			m_pTexture->Release();
			m_pTexture = NULL;
		}
	}
	else
	{
		m_bDeath = true;
	}

	Release();
}

//--------------------------------------------------------------------------------------------------------------
// ���b�V���t�B�[���h�̍X�V����
//--------------------------------------------------------------------------------------------------------------
void CSceneMeshfield::Update(void)
{

}

//--------------------------------------------------------------------------------------------------------------
// ���b�V���t�B�[���h�̕`�揈��
//--------------------------------------------------------------------------------------------------------------
void CSceneMeshfield::Draw(void)
{
	// �e�N�X�`���Ǘ����̎擾
	CTexture *pTexture = CManager::GetTexture();

	// �f�o�C�X�̎擾
	CRenderer *pRender = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRender->GetDevice();

	// �ϐ��錾
	D3DXMATRIX mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_meshField.mtxWorld);

	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_meshField.rot.y, m_meshField.rot.x, m_meshField.rot.z);
	D3DXMatrixMultiply(&m_meshField.mtxWorld, &m_meshField.mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_meshField.pos.x, m_meshField.pos.y, m_meshField.pos.z);
	D3DXMatrixMultiply(&m_meshField.mtxWorld, &m_meshField.mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_meshField.mtxWorld);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// �C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(m_pIdxBuff);

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �|���S���̕`��
	pDevice->SetTexture(0, pTexture->GetAddress(m_nTex));
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, MAX_MESHFIELD, 0, MAX_INDEX);
}

//--------------------------------------------------------------------------------------------------------------
// �u���b�N���̐ݒ�
//-----------------------------------------------------------------------------------------------w w---------------
void CSceneMeshfield::SetBlock(D3DXVECTOR2 block)
{
	m_meshField.nCntBlockX = (int)block.x;
	m_meshField.nCntBlockZ = (int)block.y;
}