// Para.h: Para �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include <fstream>
#include <string>

class Para  
{
public:
	Para(const char* fname);
	virtual ~Para();

	short TreeLog;			// �w�K�ߒ��̖؂��t�@�C���ɏ������ނƂ�1�C�������܂Ȃ��Ƃ�0
	short TreeLog2;			// �w�K�ߒ��̊e�f�[�^���t�@�C���ɏ������ނƂ�1�C�������܂Ȃ��Ƃ�0
	short CrossValNum;		// �N���X�o���f�[�V�����񐔁i�e�X�g�̂Ƃ�1�j
	float ProbMutate;		// �ˑR�ψيm��
	short TreePopNum;		// tree�W�c�̌̐�
	short SprigPopNum;		// sprig�W�c�̌̐�
	short ChildNodeNumMax;	// �P�̃m�[�h����o��q�m�[�h�̐��̍ő�l
	short SprigChromLen;	// sprig�̐��F�̂̒���
	int GenerationMax;		// ����̍ő�l
	short DepthMax;			// �؂̍����̍ő�l�i�������Ɠ����Ƃ���0�j
};