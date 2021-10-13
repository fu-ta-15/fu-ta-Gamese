//#############################################################################
//
// �ړ��\�[�X�t�@�C�� [move.cpp]
// Author : SUZUKI FUUTA
//
//#############################################################################
//-----------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
//-----------------------------------------------------------------------------
#include "move.h"

//=============================================================================
// �ړ��ʂ̐���
//=============================================================================
D3DXVECTOR3 CMove::MoveControl(D3DXVECTOR3 move, D3DXVECTOR3 deceleration)
{

	// �ړ��E��]�ʂ̌���
	move.x += (0 - move.x) * deceleration.x;
	move.y += (0 - move.y) * deceleration.y;
	move.z += (0 - move.z) * deceleration.z;

	return move;
}

//=============================================================================
// �����ړ�
//=============================================================================
float CMove::MoveSnake(float pos, float move, float Uplimit, float Downlimit,float Speed)
{
	if (pos < Downlimit)
	{
		move = Speed;
	}
	if (pos > Uplimit)
	{
		move = -Speed;
	}
	return move;
}

//=============================================================================
// �����t���E�ڕW�̈ʒu�܂ł̈ړ�
//=============================================================================
D3DXVECTOR3 CMove::TargetPosMove(D3DXVECTOR3 tagpos, D3DXVECTOR3 mypos, float Decele)
{
	// �ړ��E��]�ʂ̌���
	mypos.x += (tagpos.x - mypos.x) * Decele;
	mypos.y += (tagpos.y - mypos.y) * Decele;
	mypos.z += (tagpos.z - mypos.z) * Decele;
					     
	return mypos;
}

//=============================================================================
// ���̈ʒu�����Sin�g�𗘗p�����ړ�
//=============================================================================
float CMove::SinWave(const float CenterPos, const float nHeight, const float nCycle, const float nTime)
{
	return CenterPos + sinf((D3DX_PI * 2) / nCycle * (nTime + nTime)) * nHeight;
}

//=============================================================================
// ���̈ʒu�����Cos�g�𗘗p�����ړ�
//=============================================================================
float CMove::CosWave(const float CenterPos, const float nHeight, const float nCycle, const float nTime)
{
	return CenterPos + cosf((D3DX_PI * 2) / nCycle * (nTime + nTime)) * nHeight;
}

//=============================================================================
// Sin�g�𗘗p�����ړ�
//=============================================================================
float CMove::SinMove(const float nHeight, const float nCycle, const float nTime)
{
	return sinf((D3DX_PI * 2) / nCycle * (nTime + nTime)) * nHeight;
}

//=============================================================================
// Cos�g�𗘗p�����ړ�
//=============================================================================
float CMove::CosMove(const float nHeight, const float nCycle, const float nTime)
{
	return cosf((D3DX_PI * 2) / nCycle * (nTime + nTime)) * nHeight;
}

//=============================================================================
// �w�肳�ꂽ�p�x�Ɉړ�
//=============================================================================
float CMove::HomingMove(float Angle, float fSpeed)
{
	return Angle * fSpeed;
}

//=============================================================================
// �p�x�̌v�Z
//=============================================================================
float CMove::AnglePoint(float PointX, float PointY)
{
	return atan2f(PointX, PointY);
}
