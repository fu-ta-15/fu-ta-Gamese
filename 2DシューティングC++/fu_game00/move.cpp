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
// �v���C���[�̈ʒu����
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
// �v���C���[�̈ʒu����
//=============================================================================
float CMove::MoveSnake(float pos, float move, float Uplimit, float Downlimit,float Speed)
{
	if (pos > Downlimit)
	{
		move = -move;
	}
	else if (pos < Uplimit)
	{
		move = (move) * -1;
	}
	return move;
}

//=============================================================================
// �v���C���[�̈ʒu����
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
// �v���C���[�̈ʒu����
//=============================================================================
float CMove::SinWave(const float CenterPos, const float nHeight, const float nCycle, const float nTime)
{
	float WavePos = CenterPos + sinf((D3DX_PI * 2) / nCycle * (nTime + nTime)) * nHeight;

	return WavePos;
}

//=============================================================================
// �v���C���[�̈ʒu����
//=============================================================================
float CMove::CosWave(const float CenterPos, const float nHeight, const float nCycle, const float nTime)
{
	float WavePos = CenterPos + cosf((D3DX_PI * 2) / nCycle * (nTime + nTime)) * nHeight;

	return WavePos;
}

float CMove::HomingMove(float Angle, float fSpeed)
{
	return Angle * fSpeed;
}

float CMove::AnglePoint(float PointX, float PointY)
{
	return atan2f(PointX, PointY);
}
