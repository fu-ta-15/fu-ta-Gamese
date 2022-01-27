//-----------------------------------------------------------------
//
// �J���� (camera.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "camera.h"
#include "manager.h"
#include "renderer.h"
#include "input_keyboard.h"
#include "input_mouse.h"
#include "debugproc.h"

//-----------------------------------------------------------------
// �}�N����`
//-----------------------------------------------------------------
#define LENGTH					(600.0f)		// ���s
#define MOVE_ROT_LookAtPoint	(0.05f)			// �����_��]��
#define MOVE_ROT_Point			(0.05f)			// ���_��]��
#define MOVE_LookAtPoint		(0.5f)			// �����_�ړ���
#define MOVE_Point				(10.0f)			// ���_�ړ���

//-----------------------------------------------------------------
// CCamera�̃R���X�g���N�^
//-----------------------------------------------------------------
CCamera::CCamera()
{

}

//-----------------------------------------------------------------
// CCamera�̃f�X�g���N�^
//-----------------------------------------------------------------
CCamera::~CCamera()
{

}

//=============================================================================
// ��������
//=============================================================================
CCamera *CCamera::Create(D3DXVECTOR3 posV, D3DXVECTOR3 posR, D3DVIEWPORT9 viewport)
{
	// �|�C���^�ϐ��̐���
	CCamera *pCamera;
	pCamera = new CCamera;

	// NULL�`�F�b�N
	if (pCamera != NULL)
	{
		// ����������
		pCamera->Init(posV, posR, viewport);
	}

	return pCamera;
}

//-----------------------------------------------------------------
// �J�����̏���������
//-----------------------------------------------------------------
void CCamera::Init(D3DXVECTOR3 posV, D3DXVECTOR3 posR, D3DVIEWPORT9 viewport)
{
	// �ϐ��̏�����
	m_posR = posR;
	m_posV = posV;
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_viewport.X = 0;
	m_viewport.Y = 0;
	m_viewport.Width = SCREEN_WIDTH;
	m_viewport.Height = SCREEN_HEIGHT;
	m_viewport.MinZ = 0.0f;
	m_viewport.MaxZ = 1.0f;
	m_viewport = viewport;
	m_bParallel = false;

#ifdef _DEBUG
	m_posVDef = m_posV;
	m_posRDef = m_posR;
	m_rotDef = m_rot;
#endif

	SetCamera();
}

//-----------------------------------------------------------------
// �J�����̏I������
//-----------------------------------------------------------------
void CCamera::Uninit(void)
{

}

//-----------------------------------------------------------------
// �J�����̍X�V����
//-----------------------------------------------------------------
void CCamera::Update(void)
{
#ifdef _DEBUG
	// �|�[�Y���̎擾
	bool bPause = CManager::GetPause();

	if (bPause != true)
	{
		// �}�E�X���̎擾
		CInputMouse *pMouse = GET_MOUSE;

		/*if (pMouse->GetPress(MOUSE_LEFT))
		{
			D3DXVECTOR2 move = pMouse->GetMouseMove();

			m_rot.y += move.x * 0.005f;

			// ���_�̍X�V
			m_posV.y += move.y;

			// ���_�̍�������
			if (m_posV.y < -m_fLength)
			{
				m_posV.y = -m_fLength;
			}
			else if (m_posV.y > m_fLength)
			{
				m_posV.y = m_fLength;
			}
			float fAngle = asinf(m_posV.y / m_fLength);
			m_posV.x = m_posR.x - sinf(m_rot.y) * m_fLength * cosf(fAngle);
			m_posV.z = m_posR.z - cosf(m_rot.y) * m_fLength * cosf(fAngle);
		}
		else if (pMouse->GetPress(MOUSE_RIGHT))
		{
			D3DXVECTOR2 move = pMouse->GetMouseMove();

			m_rot.y += move.x * 0.005f;

			// �����_�̈ړ�
			float fAngle = asinf(m_posV.y / m_fLength);
			m_posR.x = m_posV.x + sinf(m_rot.y) * m_fLength * cosf(fAngle);
			m_posR.z = m_posV.z + cosf(m_rot.y) * m_fLength * cosf(fAngle);
			m_posR.y -= move.y;
		}*/

		// �����_����
		LookAtPointRotate();

		// ���_����
		PointRotate();

		// �����_�ړ�
		LookAtPointMove();

		// ���_�ړ�
		PointMove();

		Reset();
	}
	// �f�o�b�N�\��
	CDebugProc::Print("\n--- �J������� ---\n");
	CDebugProc::Print("�J�����̎��_  :%f %f %f\n", m_posV.x, m_posV.y, m_posV.z);
	CDebugProc::Print("�J�����̒����_:%f %f %f\n", m_posR.x, m_posR.y, m_posR.z);
#endif
}

//-----------------------------------------------------------------
// �J�����̐ݒu
//-----------------------------------------------------------------
void CCamera::SetCamera(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;	// �f�o�C�X�ւ̃|�C���^

	// �ϐ��̎󂯓n��
	pDevice = CManager::GetRenderer()->GetDevice();

	// �r���[�|�[�g�̐ݒ�
	pDevice->SetViewport(&m_viewport);

	// �v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxProjection);

	// �v���W�F�N�V�����}�g���b�N�X���쐬
	if (m_bParallel == false)
	{
		D3DXMatrixPerspectiveFovLH(&m_mtxProjection,
			D3DXToRadian(45.0f),
			(float)m_viewport.Width / (float)m_viewport.Height,
			10.0f,
			3000.0f);
	}
	else
	{
		// �v���W�F�N�V�����}�g���b�N�X���쐬(���s���e)
		D3DXMatrixOrthoLH(&m_mtxProjection, m_parallelSize.x, m_parallelSize.y, -3000.0f, 3000.0f);
	}

	// �v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);

	// �r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxView);

	// �r���[�}�g���b�N�X�̍쐬
	D3DXMatrixLookAtLH(&m_mtxView,
		&m_posV,
		&m_posR,
		&m_vecU);

	// �r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);
}

//-----------------------------------------------------------------
// �J�����̒����_��]����
//-----------------------------------------------------------------
void CCamera::LookAtPointRotate(void)
{
	// �L�[�{�[�h���̎擾
	CInputKeyboard *pKey = GET_KEYBOARD;

	if (pKey->GetPress(KEY_LookAtPoint_LEFT) == true)
	{// ���_�̍���]
		m_rot.y -= MOVE_ROT_LookAtPoint;						// ��]��

		if (m_rot.y < -D3DX_PI)
		{// 3.14f���傫���Ȃ����Ƃ��l��-3.14f�ɂ���
			m_rot.y += D3DX_PI * 2.0f;
		}

		// �����_�̍X�V
		m_posR.x = m_posV.x + sinf(m_rot.y) * LENGTH;	
		m_posR.z = m_posV.z + cosf(m_rot.y) * LENGTH;	
	}
	if (pKey->GetPress(KEY_LookAtPoint_RIGHT) == true)
	{// ���_�̉E��]
		m_rot.y += MOVE_ROT_LookAtPoint;						// ��]��

		if (m_rot.y > D3DX_PI)
		{// -3.14f��菬�����Ȃ����Ƃ��l��3.14f�ɂ���
			m_rot.y -= D3DX_PI * 2.0f;
		}

		// �����_�̍X�V
		m_posR.x = m_posV.x + sinf(m_rot.y) * LENGTH;	
		m_posR.z = m_posV.z + cosf(m_rot.y) * LENGTH;	
	}
}

//-----------------------------------------------------------------
// �J�����̎��_��]����
//-----------------------------------------------------------------
void CCamera::PointRotate(void)
{
	// �L�[�{�[�h���̎擾
	CInputKeyboard *pKey = GET_KEYBOARD;

	if (pKey->GetPress(KEY_Point_LEFT) == true)
	{// ���_�̍���]
		m_rot.y -= MOVE_ROT_Point;						// ��]��

		if (m_rot.y < -D3DX_PI)
		{// 3.14f���傫���Ȃ����Ƃ��l��-3.14f�ɂ���
			m_rot.y += D3DX_PI * 2.0f;
		}

		// ���_�̍X�V
		m_posV.x = m_posR.x - sinf(m_rot.y) * LENGTH;	
		m_posV.z = m_posR.z - cosf(m_rot.y) * LENGTH;	
	}
	if (pKey->GetPress(KEY_Point_RIGHT) == true)
	{// ���_�̉E��]
		m_rot.y += MOVE_ROT_Point;						// ��]��

		if (m_rot.y > D3DX_PI)
		{// -3.14f��菬�����Ȃ����Ƃ��l��3.14f�ɂ���
			m_rot.y -= D3DX_PI * 2.0f;
		}
		// ���_�̍X�V
		m_posV.x = m_posR.x - sinf(m_rot.y) * LENGTH;	
		m_posV.z = m_posR.z - cosf(m_rot.y) * LENGTH;	
	}
}

//-----------------------------------------------------------------
// �J�����̒����_�ړ�����
//-----------------------------------------------------------------
void CCamera::LookAtPointMove(void)
{
	// �L�[�{�[�h���̎擾
	CInputKeyboard *pKey = GET_KEYBOARD;

	if (pKey->GetPress(KEY_MOVE_LookAtPoint_UP) == true)
	{
		// �����_�̏�ړ�
		m_posR.y += MOVE_Point;
	}
	if (pKey->GetPress(KEY_MOVE_LookAtPoint_DOWN) == true)
	{
		// �����_�̉��ړ�
		m_posR.y -= MOVE_Point;
	}
}

//-----------------------------------------------------------------
// �J�����̎��_�ړ�����
//-----------------------------------------------------------------
void CCamera::PointMove(void)
{
	// �L�[�{�[�h���̎擾
	CInputKeyboard *pKey = GET_KEYBOARD;

	if (pKey->GetPress(KEY_MOVE_Up) == true)
	{
		// ���_�̑O�ړ�
		m_posV.x += MOVE_Point * sinf(m_rot.y);	
		m_posV.z += MOVE_Point * cosf(m_rot.y);
		m_posR.x = m_posV.x + sinf(m_rot.y) * LENGTH;
		m_posR.z = m_posV.z + cosf(m_rot.y) * LENGTH;
	}
	if (pKey->GetPress(KEY_MOVE_Down) == true)
	{
		// ���_�̌��ړ�
		m_posV.x -= MOVE_Point * sinf(m_rot.y);
		m_posV.z -= MOVE_Point * cosf(m_rot.y);
		m_posR.x = m_posV.x + sinf(m_rot.y) * LENGTH;
		m_posR.z = m_posV.z + cosf(m_rot.y) * LENGTH;
	}
	if (pKey->GetPress(KEY_MOVE_Right) == true)
	{
		// ���_�̉E�ړ�
		m_posV.x += MOVE_Point * cosf(m_rot.y);
		m_posV.z -= MOVE_Point * sinf(m_rot.y);
		m_posR.x = m_posV.x + sinf(m_rot.y) * LENGTH;
		m_posR.z = m_posV.z + cosf(m_rot.y) * LENGTH;
	}
	if (pKey->GetPress(KEY_MOVE_Left) == true)
	{
		// ���_�̍��ړ�
		m_posV.x -= MOVE_Point * cosf(m_rot.y);
		m_posV.z += MOVE_Point * sinf(m_rot.y);
		m_posR.x = m_posV.x + sinf(m_rot.y) * LENGTH;
		m_posR.z = m_posV.z + cosf(m_rot.y) * LENGTH;
	}
	if (pKey->GetPress(KEY_MOVE_Point_UP) == true)
	{
		// ���_�̏�ړ�
		m_posV.y += MOVE_Point;
	}
	if (pKey->GetPress(KEY_MOVE_Point_DOWN) == true)
	{
		// ���_�̉��ړ�
		m_posV.y -= MOVE_Point;
	}
}

#ifdef _DEBUG
//-----------------------------------------------------------------
// �J�����̃��Z�b�g
//-----------------------------------------------------------------
void CCamera::Reset(void)
{
	// �L�[�{�[�h���̎擾
	CInputKeyboard *pKey = GET_KEYBOARD;

	if (pKey->GetPress(KEY_RESET) == true)
	{
		// �ϐ��̏�����
		m_posV = m_posVDef;
		m_posR = m_posRDef;
		m_rot = m_rotDef;
	};
}
#endif

//-----------------------------------------------------------------
// ���[���h���W����X�N���[�����W�ɕϊ�
//-----------------------------------------------------------------
D3DXVECTOR3 CCamera::ChangePos_WorldtoScreen(D3DXVECTOR3 pos)
{
	// �ϐ��錾
	D3DXMATRIX mtx, mtxObject;
	D3DXMATRIX mtxScreen =
	{ SCREEN_WIDTH / 2.0f,0,0,0,
		0,-SCREEN_HEIGHT / 2.0f,0,0,
		0,0,1,0,
		SCREEN_WIDTH / 2.0f ,SCREEN_HEIGHT / 2.0f ,0,1 };

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtx);

	// ���[���h���W����X�N���[�����W�ɕϊ�
	D3DXMatrixTranslation(&mtxObject, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&mtx, &mtx, &mtxObject);
	D3DXMatrixMultiply(&mtx, &mtx, &m_mtxView);
	D3DXMatrixMultiply(&mtx, &mtx, &m_mtxProjection);
	D3DXMatrixMultiply(&mtx, &mtx, &mtxScreen);

	return D3DXVECTOR3(mtx._41 / mtx._44, mtx._42 / mtx._44, 0.0f);
}

//-----------------------------------------------------------------
// ���s���e�̐ݒ�
//-----------------------------------------------------------------
void CCamera::SetParallel(D3DXVECTOR2 size)
{
	// ���s���e��On�ɂ���
	m_bParallel = true;

	// �T�C�Y�̊m��
	m_parallelSize = size;

	SetCamera();
}