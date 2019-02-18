//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   SceneResult.cpp
//!
//! @brief  SceneResult�̃\�[�X�t�@�C��
//!
//! @date   
//!
//! @author �[�����l
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// �w�b�_�t�@�C���̓ǂݍ��� ================================================
#include "SceneResult.h"
#include "GameMain.h"


//�萔�̒�`================================================================





//�f�[�^�^�̒�`============================================================
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
extern Score g_score;

//�摜
HGRP g_resultImage[10];

//����
HGRP g_resultNumber[16];

//�\���^�C�~���O���v�鎞��
int g_resultTimer;

// �֐��̒�` ==============================================================
//----------------------------------------------------------------------
//! @brief �Q�[���̏���������
//!
//! @param[in] �Ȃ�
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void InitializeResult(void)
{
	//���[�J���ϐ��̐錾------------------------------------------------



	//���[�J���ϐ��̏�����----------------------------------------------



	//�����̓��e--------------------------------------------------------

	//�摜�̓ǂݍ���
	g_resultImage[0] = LoadGraph("Resources/Images/Result.png");

	//�����摜�̓ǂݍ���
	LoadDivGraph("Resources/Images/resultNumber.png", 16, 4, 4, 45, 45, g_resultNumber);

	//����
	g_resultTimer = 0;
}



//----------------------------------------------------------------------
//! @brief �Q�[���̍X�V����
//!
//! @param[in] �Ȃ�
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void UpdateResult(void)
{
	//���[�J���ϐ��̐錾------------------------------------------------
	


	//���[�J���ϐ��̏�����----------------------------------------------



	//�����̓��e--------------------------------------------------------

	//�J�E���g
	g_resultTimer++;

	//���U���g��ʂ̑���
	if (((g_lastInputState&g_currentInputState)&PAD_INPUT_10) != 0)	//�X�y�[�X�L�[���������
	{
		if ((g_resultTimer>0)&&(g_resultTimer < 60))
		{
			g_resultTimer = 60;
		}

		if ((g_resultTimer>60) && (g_resultTimer < 120))
		{
			g_resultTimer = 120;
		}

		if ((g_resultTimer>120) && (g_resultTimer < 180))
		{
			g_resultTimer = 180;
		}

		if ((g_resultTimer>180) && (g_resultTimer < 240))
		{
			g_resultTimer = 240;
		}

		if ((g_resultTimer>240) && (g_resultTimer < 300))
		{
			g_resultTimer = 300;
		}

		if ((g_resultTimer>300) && (g_resultTimer < 360))
		{
			g_resultTimer = 360;
		}

		if ((g_resultTimer>360) && (g_resultTimer < 420))
		{
			g_resultTimer = 420;
		}

		if (g_resultTimer>420)
		{
			//�V�[���̐؂�ւ�
			ChangeScene(SCENE_TITLE);	//�^�C�g����
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
void RenderResult(void)
{
	//���[�J���ϐ��̐錾------------------------------------------------
	int total;



	//���[�J���ϐ��̏�����----------------------------------------------
	total = g_score.type1 + g_score.type2 + g_score.type3 + g_score.type4 + g_score.type5 + g_score.type6 + g_score.type7;



	//�����̓��e--------------------------------------------------------

	//�w�i�̕`��
	DrawGraph(0, 0, g_resultImage[0], TRUE);

	//�X�R�A�̕`��
	//type1
	if (g_resultTimer >= 60)
	{
		//�\�̌�
		DrawGraph(SCREEN_CENTER_X, 100, g_resultNumber[g_score.type1 / 10], TRUE);
		//��̌�
		DrawGraph(SCREEN_CENTER_X + 15, 100, g_resultNumber[g_score.type1 % 10], TRUE);
	}
	//type2
	if (g_resultTimer >= 120)
	{
		//�\�̌�
		DrawGraph(SCREEN_CENTER_X, 140, g_resultNumber[g_score.type2 / 10], TRUE);
		//��̌�
		DrawGraph(SCREEN_CENTER_X + 15, 140, g_resultNumber[g_score.type2 % 10], TRUE);
	}
	//type3
	if (g_resultTimer >= 180)
	{
		//�\�̌�
		DrawGraph(SCREEN_CENTER_X, 180, g_resultNumber[g_score.type3 / 10], TRUE);
		//��̌�
		DrawGraph(SCREEN_CENTER_X + 15, 180, g_resultNumber[g_score.type3 % 10], TRUE);
	}
	
	//type4
	if (g_resultTimer >= 240)
	{
		//�\�̌�
		DrawGraph(SCREEN_CENTER_X, 220, g_resultNumber[g_score.type4 / 10], TRUE);
		//��̌�
		DrawGraph(SCREEN_CENTER_X + 15, 220, g_resultNumber[g_score.type4 % 10], TRUE);
	}

	//type5
	if (g_resultTimer >= 300)
	{
		//�\�̌�
		DrawGraph(SCREEN_CENTER_X, 260, g_resultNumber[g_score.type5 / 10], TRUE);
		//��̌�
		DrawGraph(SCREEN_CENTER_X + 15, 260, g_resultNumber[g_score.type5 % 10], TRUE);
	}

	//type6
	if (g_resultTimer >= 360)
	{
		//�\�̌�
		DrawGraph(SCREEN_CENTER_X, 300, g_resultNumber[g_score.type6 / 10], TRUE);
		//��̌�
		DrawGraph(SCREEN_CENTER_X + 15, 300, g_resultNumber[g_score.type6 % 10], TRUE);
	}

	//type7
	if (g_resultTimer >= 420)
	{
		//�\�̌�
		DrawGraph(SCREEN_CENTER_X, 340, g_resultNumber[g_score.type7 / 10], TRUE);
		//��̌�
		DrawGraph(SCREEN_CENTER_X + 15, 340, g_resultNumber[g_score.type7 % 10], TRUE);
	}

	//���v
	if (g_resultTimer >= 480)
	{
		//�\�̌�
		DrawGraph(SCREEN_CENTER_X + 30, 420, g_resultNumber[total/10], TRUE);
		//��̌�
		DrawGraph(SCREEN_CENTER_X + 45, 420, g_resultNumber[total % 10], TRUE);
	}

}



//----------------------------------------------------------------------
//! @brief �Q�[���̏I������
//!
//! @param[in] �Ȃ�
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void FinalizeResult(void)
{
	//���[�J���ϐ��̐錾------------------------------------------------



	//���[�J���ϐ��̏�����----------------------------------------------



	//�����̓��e--------------------------------------------------------
	//�摜�f�[�^�̍폜
	InitGraph();


}
