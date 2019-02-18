//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   SceneTitle.cpp
//!
//! @brief  �^�C�g���V�[���֘A�̃\�[�X�t�@�C��
//!
//! @date   
//!
//! @author �[�����l
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// �w�b�_�t�@�C���̓ǂݍ��� ================================================
#include "SceneTitle.h"
#include "GameMain.h"


//�萔�̒�`================================================================





//�f�[�^�^�̒�`============================================================
struct Tag_Vec2D
{
	float x;
	float y;
};
typedef struct Tag_Vec2D Vec2D;

struct Tag_Image
{
	double extRate;
	double angle;
	HGRP handle;
};
typedef struct Tag_Image Image;

struct Tag_Title
{
	Vec2D pos;
	Image image;
};
typedef struct Tag_Title Title;

struct Tag_Score
{
	int type1;
	int type2;
	int type3;
	int type4;
	int type5;
	int type6;
	int type7;
};
typedef struct Tag_Score Score;

//�֐��̃v���g�^�C�v�錾====================================================




// �O���[�o���ϐ��̒�` ====================================================

//�摜
Title g_titleImage[10];

//�X�e�[�W
int g_stageNumber;

//�X�R�A
Score g_score;


// �֐��̒�` ==============================================================
//----------------------------------------------------------------------
//! @brief �Q�[���̏���������
//!
//! @param[in] �Ȃ�
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void InitializeTitle(void)
{
	//���[�J���ϐ��̐錾------------------------------------------------



	//���[�J���ϐ��̏�����----------------------------------------------



	//�����̓��e--------------------------------------------------------

	g_titleImage[0].pos.x = SCREEN_CENTER_X;
	g_titleImage[0].pos.y = SCREEN_CENTER_Y;
	g_titleImage[0].image.angle = 0.0;
	g_titleImage[0].image.extRate = 1.0;
	g_titleImage[0].image.handle = LoadGraph("Resources/Images/�͂��߂�.png");

	g_titleImage[1].pos.x = SCREEN_CENTER_X;
	g_titleImage[1].pos.y = SCREEN_CENTER_Y;
	g_titleImage[1].image.angle = 0.0;
	g_titleImage[1].image.extRate = 1.0;
	g_titleImage[1].image.handle = LoadGraph("Resources/Images/�����.png");

	g_titleImage[2].pos.x = 0.0f;
	g_titleImage[2].pos.y = 0.0f;
	g_titleImage[2].image.angle = 0.0;
	g_titleImage[2].image.extRate = 1.0;
	g_titleImage[2].image.handle = LoadGraph("Resources/Images/title.png");

}



//----------------------------------------------------------------------
//! @brief �Q�[���̍X�V����
//!
//! @param[in] �Ȃ�
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void UpdateTitle(void)
{
	//���[�J���ϐ��̐錾------------------------------------------------
	static int select = 0;


	//���[�J���ϐ��̏�����----------------------------------------------



	//�����̓��e--------------------------------------------------------

	//�L�[����
	if (((g_lastInputState&g_currentInputState)&PAD_INPUT_8) != 0)	//W�L�[���������
	{
		select = 0;	//'�͂��߂�'
	}
	if (((g_lastInputState&g_currentInputState)&PAD_INPUT_5) != 0)	//S�L�[���������
	{
		select = 1;	//'�����'
	}

	//'�͂��߂�'���I������Ă���Ƃ�
	if (select == 0)
	{
		g_titleImage[0].image.extRate = 1.0;
		g_titleImage[1].image.extRate = 0.8;

		if (((g_lastInputState&g_currentInputState)&PAD_INPUT_10) != 0)	//�X�y�[�X�L�[���������
		{
			//�X�e�[�W��������
			g_stageNumber = 1;

			//�X�R�A�̏�����
			g_score.type1 = 0;
			g_score.type2 = 0; 
			g_score.type3 = 0;
			g_score.type4 = 0;
			g_score.type5 = 0;
			g_score.type6 = 0;
			g_score.type7 = 0;

			//���̃V�[����
			ChangeScene(SCENE_READY);	//�v���C�O������
		}
	}

	//'�����'���I������Ă���Ƃ�
	if (select == 1)
	{
		g_titleImage[0].image.extRate = 0.8;
		g_titleImage[1].image.extRate = 1.0;

		if (((g_lastInputState&g_currentInputState)&PAD_INPUT_10) != 0)	//�X�y�[�X�L�[���������
		{
			ExitGame();	//�Q�[�����I������
		}

	}

}



//----------------------------------------------------------------------
//! @brief �Q�[���̕`�揈��
//!
//! @param[in] �Ȃ�
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void RenderTitle(void)
{
	//���[�J���ϐ��̐錾------------------------------------------------



	//���[�J���ϐ��̏�����----------------------------------------------



	//�����̓��e--------------------------------------------------------

	//�w�i�̕`��
	DrawGraph((int)g_titleImage[2].pos.x, (int)g_titleImage[2].pos.y, g_titleImage[2].image.handle, TRUE);

	//'�͂��߂�'�̕`��
	DrawRotaGraph((int)g_titleImage[0].pos.x, (int)g_titleImage[0].pos.y+80, g_titleImage[0].image.extRate, g_titleImage[0].image.angle, g_titleImage[0].image.handle, TRUE, FALSE);

	//'�����'�̕`��
	DrawRotaGraph((int)g_titleImage[1].pos.x, (int)g_titleImage[1].pos.y+130, g_titleImage[1].image.extRate, g_titleImage[1].image.angle, g_titleImage[1].image.handle, TRUE, FALSE);

}



//----------------------------------------------------------------------
//! @brief �Q�[���̏I������
//!
//! @param[in] �Ȃ�
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void FinalizeTitle(void)
{
	//���[�J���ϐ��̐錾------------------------------------------------



	//���[�J���ϐ��̏�����----------------------------------------------



	//�����̓��e--------------------------------------------------------
	//�摜�f�[�^�̍폜
	InitGraph();


}
