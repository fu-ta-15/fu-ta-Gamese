//-------------------------------------------------------------------
//
// �J�����w�b�_�[ (camera.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "main.h"

//-------------------------------------------------------------------
// �J�����̎��(�񋓌^)
//-------------------------------------------------------------------
typedef enum
{
	CAMERATYPE_PLAYER_LOOKDOWN = 0,		// �v���C���[�����낵�J����
	CAMERATYPE_PLAYER,				// �v���C���[�Ǐ]�J����
	CAMERATYPE_MAX,
}CAMERATYPE;

//-------------------------------------------------------------------
// �J�����N���X(��{�N���X)
//-------------------------------------------------------------------
class CCamera
{
public:
	CCamera();
	~CCamera();

	static CCamera *Create(D3DXVECTOR3 posV, D3DXVECTOR3 posR, D3DVIEWPORT9 viewport);
	void Init(D3DXVECTOR3 posV, D3DXVECTOR3 posR, D3DVIEWPORT9 viewport);
	void Uninit(void);		
	void Update(void);		
	void SetCamera(void);	

	D3DXVECTOR3 GetRotate(void) { return m_rot; }
	D3DXVECTOR3 GetPositionV(void) { return m_posV; }
	D3DXVECTOR3 GetPositionR(void) { return m_posR; }
	D3DXVECTOR3 GetVecU(void) { return m_vecU; }

	D3DXMATRIX GetProjection(void) { return m_mtxProjection; }
	D3DXMATRIX GetView(void) { return m_mtxView; }

	D3DXVECTOR3 ChangePos_WorldtoScreen(D3DXVECTOR3 pos);

	void SetParallel(D3DXVECTOR2 size);

private:
	void LookAtPointRotate(void);	// �J�����̒����_��]����
	void PointRotate(void);			// �J�����̎��_��]����
	void LookAtPointMove(void);		// �J�����̒����_�ړ�
	void PointMove(void);			// �J�����̎��_�ړ�
	void Reset(void);

	D3DXVECTOR3 m_posV;				// ���_
	D3DXVECTOR3 m_posR;				// �����_
	D3DXVECTOR3 m_vecU;				// ������x�N�g��
	D3DXVECTOR3 m_rot;				// ����
	D3DXMATRIX m_mtxProjection;		// �v���W�F�N�V�����}�g���b�N�X
	D3DXMATRIX m_mtxView;			// �r���[�}�g���b�N�X
	D3DVIEWPORT9 m_viewport;		// �r���[�|�[�g

	float m_fLength;

	D3DXVECTOR2 m_parallelSize;		// ���s���e���̃T�C�Y
	bool m_bParallel;				// ���s���e���ǂ���

#ifdef _DEBUG
	D3DXVECTOR3 m_posVDef;				// ���_
	D3DXVECTOR3 m_posRDef;				// �����_
	D3DXVECTOR3 m_rotDef;				// ����
#endif
};
#endif