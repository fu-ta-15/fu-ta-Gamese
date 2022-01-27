//=============================================================================
//
// ���N�K�L���� [graffiti.cpp]
// Author : 
//
//=============================================================================
#include "graffiti.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "eraserdust.h"
#include "eraser.h"
#include "base.h"
#include "game.h"
#include "graffiti_manager.h"

#include "debugproc.h"

//=============================================================================
// CGraffiti�̃R���X�g���N�^
//=============================================================================
CGraffiti::CGraffiti(int nPriority) : CScene3D(nPriority)
{
	// �ϐ��̏�����
}

//=============================================================================
// CGraffiti�̃f�X�g���N�^
//=============================================================================
CGraffiti::~CGraffiti()
{

}

//=============================================================================
// ��������
//=============================================================================
CGraffiti *CGraffiti::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, D3DXCOLOR col)
{
	// �|�C���^�ϐ��̐���
	CGraffiti *pGraffiti;
	pGraffiti = new CGraffiti(CScene::PRIORITY_GRAFFITI);

	// NULL�`�F�b�N
	if (pGraffiti != NULL)
	{
		// ����������
		pGraffiti->Init(pos, size, col);

		// �I�u�W�F�N�g�^�C�v�̐ݒ�
		pGraffiti->SetObjType(OBJTYPE_GRAFFITI);
	}

	return pGraffiti;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CGraffiti::Init(D3DXVECTOR3 pos, D3DXVECTOR2 size, D3DXCOLOR col)
{
	// CScene2D�̏���������
	CScene3D::Init(pos, size);

	// �F�̎w��
	SetCol(col);

	// �e�N�X�`���Ǘ����̎擾
	CTexture *pTexture = CManager::GetTexture();

	if (pTexture != NULL)
	{
		// �e�N�X�`���̊��蓖��
		BindTexture(NULL);
	}

	if (col == D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f))
	{
		m_graffitiCol = GRAFFITICOLOR_BLACK;
	}
	else
	{
		m_graffitiCol = GRAFFITICOLOR_COLORFUL;
	}
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CGraffiti::Uninit(void)
{
	// CScene2D�̏I������
	CScene3D::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CGraffiti::Update(void)
{
	// CScene2D�̍X�V����
	CScene3D::Update();
}

//=============================================================================
// �`�揈��
//=============================================================================
void CGraffiti::Draw(void)
{
	// CScene2D�̕`�揈��
	CScene3D::Draw();
}

//=============================================================================
// ���N�K�L�̓����蔻�菈��
//=============================================================================
bool CGraffiti::Collision(D3DXVECTOR3 pos, D3DXVECTOR3 size, int nType)
{
	// �ʒu�ƃT�C�Y�̎擾
	D3DXVECTOR3 posGraffiti = GetPosition();
	D3DXVECTOR2 sizeGraffiti = GetSize();

	if ((m_graffitiCol == GRAFFITICOLOR_BLACK && nType == CEraser::ERASERTYPE_NORMAL) ||
		(m_graffitiCol == GRAFFITICOLOR_COLORFUL && nType == CEraser::ERASERTYPE_SAND))
	{
		// �������Ă�����F�𔖂����Ă���
		if (pos.x - size.x < posGraffiti.x + sizeGraffiti.x && pos.x + size.x > posGraffiti.x - sizeGraffiti.x &&
			pos.z - size.z < posGraffiti.z + sizeGraffiti.y && pos.z + size.z > posGraffiti.z - sizeGraffiti.y)
		{
			// �F�̎擾
			D3DXCOLOR col = GetCol();

			// ���l��0.0f���傫���Ȃ珈��(�����������)
			if (col.a > 0.0f)
			{
				// �������ɂ���
				col.a -= 0.1f;

				// �F�̐ݒ�
				SetCol(col);

				// 0.0f�ȉ��ɂȂ�����폜
				if (col.a <= 0.0f)
				{
					Uninit();

					// �����J�X�𐶐�
					if (rand() % 2 == 0)
					{
						CEraserDust::Create(posGraffiti, D3DXVECTOR2(0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), "data/MODEL/ErasingRemaining_00.x");
					}
				}

				// ���݂̃��N�K�L�������炷
				CBase *pBase = CManager::GetBase();

				if (pBase != NULL)
				{
					CGame *pGame = (CGame*)pBase;
					CGraffitiManager *pGraffitiManager = pGame->GetGraffitiManager();

					pGraffitiManager->SetCurGraffiti(-1);
				}

				return true;
			}
		}
	}
	return false;
}