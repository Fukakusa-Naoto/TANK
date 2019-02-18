//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   GameMain.h
//!
//! @brief  �Q�[���֘A�̃w�b�_�t�@�C��
//!
//! @date   2017.7.31
//!
//! @author GF1.21.�[�����l
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// ���d�C���N���[�h�̖h�~ ==================================================
#pragma once




// �萔�̒�` ==============================================================

// <�V�X�e��> ----------------------------------------------------------
#define GAME_TITLE "ShootingGame"        // �Q�[���^�C�g��


// <���> --------------------------------------------------------------
#define SCREEN_WIDTH    (640)                  // ��ʂ̕�[pixel]
#define SCREEN_HEIGHT   (480)                  // ��ʂ̍���[pixel]

#define SCREEN_TOP      (0)                    // ��ʂ̏�[
#define SCREEN_BOTTOM   (SCREEN_HEIGHT)        // ��ʂ̉��[
#define SCREEN_LEFT     (0)                    // ��ʂ̍��[
#define SCREEN_RIGHT    (SCREEN_WIDTH)         // ��ʂ̉E�[

#define SCREEN_CENTER_X (SCREEN_WIDTH  / 2)    // ��ʂ̒���(X���W)
#define SCREEN_CENTER_Y (SCREEN_HEIGHT / 2)    // ��ʂ̒���(Y���W)

//�Q�[���ɕK�v�ȃV�[����ID��o�^
enum tag_SceneId
{
	SCENE_NONE,		//�Ȃ�
	SCENE_LOGO,		//���S
	SCENE_TITLE,	//�^�C�g��
	SCENE_READY,	//�v���C�O����
	SCENE_PLAYING,		//�v���C
	SCENE_RESULT	//����
};
typedef enum tag_SceneId SceneId;



// �֐��̐錾 ==============================================================

// �Q�[���̏���������
void InitializeGame(void);

// �Q�[���̍X�V����
void UpdateGame(void);

// �Q�[���̕`�揈��
void RenderGame(void);

// �Q�[���̏I������
void FinalizeGame(void);

//�V�[���̐؂�ւ�
void ChangeScene(SceneId nextSceneId);



//�O���[�o���ϐ��̐錾======================================================
//�L�[�̓��͏��
extern int g_currentInputState;	//����

extern int g_lastInputState;	//�O��
