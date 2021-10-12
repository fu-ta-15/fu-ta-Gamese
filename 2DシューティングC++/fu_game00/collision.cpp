//*****************************************************************************
//
// �����蔻�菈�� [collision.h]
// Author : SUZUKI FUUTA
//
//*****************************************************************************
//-----------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
//-----------------------------------------------------------------------------
#include "collision.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CCollision::CCollision()
{
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CCollision::~CCollision()
{
}

//=============================================================================
// �l�p���m�̓����蔻��
//=============================================================================
bool CCollision::CollisionSquare(D3DXVECTOR3 Mypos, D3DXVECTOR3 Mysize, D3DXVECTOR3 Tagepos, D3DXVECTOR3 Tagesize)
{
	bool bCol = false;

			if (Mypos.y + Mysize.y > Tagepos.y - Tagesize.y && Mypos.y - Mysize.y < Tagepos.y + Tagesize.y &&
				Mypos.x + Mysize.x > Tagepos.x - Tagesize.x && Mypos.x - Mysize.x < Tagepos.x + Tagesize.x)
			{/* �G�͈̔͂ɒe�����݂����� */
				bCol = true;							// �����蔻��͗L
			}

	return bCol;
}

bool CCollision::CollisionCycle(D3DXVECTOR3 pointpos, D3DXVECTOR3 Cyclepos, float radius)
{
	bool bColl = false;

	float A = pointpos.x - Cyclepos.x;
	float B = pointpos.y - Cyclepos.y;
	float C = sqrt(A * A + B * B);

	if (C <= radius)
	{
		bColl = true;
	}

	return bColl;
}

D3DXVECTOR3 CCollision::MeshCollision(const D3DXVECTOR3 lineStart1, const D3DXVECTOR3 lineEnd1, const D3DXVECTOR3 point, COLLISION coltype)
{
	D3DXVECTOR3 pos;

	if (OutProduct(lineStart1, lineEnd1, point) == true)
	{
		pos = WaveCollision(lineStart1, lineEnd1, point, coltype);
	}

	return pos;
}

//=============================================================================
// ���ςɂ�锽�˃x�N�g��
//=============================================================================
D3DXVECTOR3 CCollision::CrossProduct(const D3DXVECTOR3 v1, const D3DXVECTOR3 v2)
{
	D3DXVECTOR3 VecF;	// �i�s�x�N�g��
	float VecN;			// �ǂ̖@���x�N�g��(�@���̂���1�Œ�j

	float VecAX;		// ���m�̒l�i���ςŎg���l�jX
	float VecAZ;		// ���m�̒l�i���ςŎg���l�jZ

	float VecPX;		// �ǂɕ��s�ȃx�N�g�� X
	float VecPZ;		// �ǂɕ��s�ȃx�N�g�� Z

	float VecWX;		// ���˃x�N�g�� X
	float VecWY;		// ���˃x�N�g�� Z


	// �i�s�x�N�g���̑��
	VecF = v1;

	// �����x�N�g��
	VecN = 1.0f;

	// ���ςŎg�p���関�m�̐��l�����߂�
	VecAX = -VecF.x * VecN;
	VecAZ = -VecF.y * VecN;

	// �����������̂Ƃ̕��s�x�N�g�������߂�
	VecPX = VecF.x + VecAX * VecN;
	VecPZ = VecF.y + VecAZ * VecN;

	// ���˃x�N�g�������߂�
	VecWX = VecF.x + (VecPX*2) * VecN;
	VecWY = VecF.y + (VecPZ*2) * VecN;

	return D3DXVECTOR3(VecWX, VecWY, 0.0f);
}

//=============================================================================
// �O�ς̓����蔻��
//=============================================================================
bool CCollision::OutProduct(const D3DXVECTOR3 lineStart1, const D3DXVECTOR3 lineEnd1, const D3DXVECTOR3 point)
{
	bool Collision = false;

	float V1_X = (lineEnd1.x - lineStart1.x);
	float V1_Y = (lineEnd1.y - lineStart1.y);

	float V2_X = (point.x - lineStart1.x);
	float V2_Y = (point.y - lineStart1.y);

	float L1 = V1_Y * V2_X;
	float L2 = V2_Y * V1_X;
	float L3 = L1 - L2;

	if (L3 < 0)
	{
		Collision = true;
	}

	return Collision;
}

//=============================================================================
// Y���W���H�̎��ɋ��߂鏈��
//=============================================================================
D3DXVECTOR3 CCollision::WaveCollision(const D3DXVECTOR3 start, const D3DXVECTOR3 end, const D3DXVECTOR3 nowpos, COLLISION coltype)
{
	D3DXVECTOR3 tagPos = {};
	D3DXVECTOR3 startPos = start;
	D3DXVECTOR3 endPos = end;
	D3DXVECTOR3 LengthPos;

	float pos_x;
	float pos_y;

	LengthPos.x = endPos.x - startPos.x;
	LengthPos.y = endPos.y - startPos.y;

	pos_x = nowpos.x / LengthPos.x;
	pos_y = nowpos.y / LengthPos.y;

	float Xpercent = 100 - pos_x;
	float Ypercent = 100 - pos_y;

	tagPos.y = LengthPos.y *(pos_x / 100);

	tagPos.x = LengthPos.x * (pos_y / 100);

	if (start.y < end.y)
	{
		tagPos.y = end.y + tagPos.y;
	}
	if (start.y > end.y)
	{
		tagPos.y = start.y + tagPos.y;
	}

	if (tagPos.y == 0.0f)
	{
		tagPos.y = start.y;
	}

	if (tagPos.y < 0)
	{
		tagPos.y *= -1;
	}
	if (tagPos.x < 0)
	{
		tagPos.x *= -1;
	}
	return tagPos;
}

