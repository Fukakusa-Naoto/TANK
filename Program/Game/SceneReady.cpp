//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   SceneReady.cpp
//!
//! @brief  �v���C�O�����V�[���̃\�[�X�t�@�C��
//!
//! @date   ���t
//!
//! @author ����Җ�
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// �w�b�_�t�@�C���̓ǂݍ��� ================================================
#include "GameMain.h"
#include "SceneReady.h"


//�f�[�^�^�̒�`============================================================





//�萔�̒�`================================================================

//�}�b�v�̍ő吔
#define MAX_MAP (100)

//�}�b�v�̕�
#define MAP_WIDTH (20)

//�}�b�v�̍���
#define MAP_HEIGHT (15)


// �O���[�o���ϐ��̒�` ====================================================

//�}�b�v�f�[�^
int g_mapData[MAP_HEIGHT][MAP_WIDTH];
//�ʒu�f�[�^
int g_posData[MAP_HEIGHT][MAP_WIDTH];
//�X�e�[�W
extern int g_stageNumber;

//�摜
HGRP g_readyImage[10];

//����
HGRP g_readyNumber[16];


//�֐��̐錾================================================================

//�}�b�v�f�[�^�̓ǂݍ���
void LoadMap(char *mapFile, int mapWidth, int mapHeight);

//�ʒu�f�[�^�̓ǂݍ���
void LoadPosition(char *posFile, int mapWidth, int mapHeight);


// �֐��̒�` ==============================================================
//----------------------------------------------------------------------
//! @brief �v���C�O�����̏���������
//!
//! @param[in] �Ȃ�
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void InitializeReady(void)
{
	//���[�J���ϐ��̐錾//----------------------------------------------

	//�ǂݍ��ރ}�b�v�t�@�C��
	char *mapFile[MAX_MAP];

	//�ǂݍ��ވʒu�t�@�C��
	char *posFile[MAX_MAP];

	//���[�J���ϐ��̏�����//--------------------------------------------

	//�}�b�v�t�@�C���̓ǂݍ���
	mapFile[0] = "Resources/MapData/map.txt";
	mapFile[1] = "Resources/MapData/map1.txt";
	mapFile[2] = "Resources/MapData/map2.txt";
	mapFile[3] = "Resources/MapData/map3.txt";
	mapFile[4] = "Resources/MapData/map4.txt";
	mapFile[5] = "Resources/MapData/map5.txt";
	mapFile[6] = "Resources/MapData/map6.txt";
	mapFile[7] = "Resources/MapData/map7.txt";
	mapFile[8] = "Resources/MapData/map8.txt";
	mapFile[9] = "Resources/MapData/map9.txt";
	mapFile[10] = "Resources/MapData/map10.txt";

	//�ʒu�t�@�C���̓ǂݍ���
	posFile[0] = "Resources/PositionData/pos.txt";
	posFile[1] = "Resources/PositionData/pos1.txt";
	posFile[2] = "Resources/PositionData/pos2.txt";
	posFile[3] = "Resources/PositionData/pos3.txt";
	posFile[4] = "Resources/PositionData/pos4.txt";
	posFile[5] = "Resources/PositionData/pos5.txt";
	posFile[6] = "Resources/PositionData/pos6.txt";
	posFile[7] = "Resources/PositionData/pos7.txt";
	posFile[8] = "Resources/PositionData/pos8.txt";
	posFile[9] = "Resources/PositionData/pos9.txt";
	posFile[10] = "Resources/PositionData/pos10.txt";

	//�����̓��e//------------------------------------------------------

	//�}�b�v
	LoadMap(mapFile[g_stageNumber], MAP_WIDTH, MAP_HEIGHT);
	//�ʒu
	LoadPosition(posFile[g_stageNumber], MAP_WIDTH, MAP_HEIGHT);

	//�摜�̓ǂݍ���
	g_readyImage[0] = LoadGraph("Resources/Images/Ready.png");
	g_readyImage[1] = LoadGraph("Resources/Images/�X�e�[�W.png");

	//�����摜�̓ǂݍ���
	LoadDivGraph("Resources/Images/stageNumber.png", 16, 4, 4, 83, 83, g_readyNumber);

}



//----------------------------------------------------------------------
//! @brief �v���C�O�����̍X�V����
//!
//! @param[in] �Ȃ�
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void UpdateReady(void)
{
	//���[�J���ϐ��̐錾//----------------------------------------------
	static int timer = 0;


	//���[�J���ϐ��̏�����//--------------------------------------------



	//�����̓��e//------------------------------------------------------
	timer++;

	if (timer == 180)
	{
		timer = 0;
		//�V�[���̐؂�ւ�
		ChangeScene(SCENE_PLAYING);	//�v���C�V�[����
	}
}



//----------------------------------------------------------------------
//! @brief �v���C�O�����̕`�揈��
//!
//! @param[in] �Ȃ�
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void RenderReady(void)
{
	//���[�J���ϐ��̐錾//----------------------------------------------
	


	//���[�J���ϐ��̏�����//--------------------------------------------



	//�����̓��e//------------------------------------------------------

	//�w�i�̕`��
	DrawGraph(0, 0, g_readyImage[0], TRUE);

	//�����̕`��
	DrawGraph(150, 100, g_readyImage[1], TRUE);

	//�����̕`��
	//�\�̌�
	DrawGraph(360, 105, g_readyNumber[g_stageNumber / 10], TRUE);
	//��̌�
	DrawGraph(400, 107, g_readyNumber[g_stageNumber % 10], TRUE);
}



//----------------------------------------------------------------------
//! @brief �v���C�O�����̏I������
//!
//! @param[in] �Ȃ�
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void FinalizeReady(void)
{
	//���[�J���ϐ��̐錾//----------------------------------------------
	


	//���[�J���ϐ��̏�����//--------------------------------------------



	//�����̓��e//------------------------------------------------------
	//�摜�f�[�^�̍폜
	InitGraph();


}



//*---------------------------------------------------------------------
//* �}�b�v�f�[�^�̓ǂݍ���
//*
//* @param [*mapFile] �}�b�v�t�@�C����
//* @param [mapWidth] �}�b�v�̕�
//* @param [mapHeight] �}�b�v�̍���
//*
//* @return �Ȃ�
//*---------------------------------------------------------------------
void LoadMap(char *mapFile, int mapWidth, int mapHeight)
{
	//���[�J���ϐ��̐錾--------------------------------
	// �t�@�C���\����
	FILE *fp;
	char buf[512];

	int i;	//�z��Ɏg�p
	int j;	//�z��Ɏg�p

			//���[�J���ϐ��̏�����------------------------------
	fp = fopen(mapFile, "r");

	i = 0;
	j = 0;
	//�����̓��e----------------------------------------
	if (fp == NULL)
	{
		fprintf(stderr, "�t�@�C�����I�[�v���ł��܂���\n");
		// �����I��
		exit(1);
	}

	while (fgets(buf, 512, fp) != NULL)
	{
		for (i = 0; i < mapWidth; i++)
		{
			if ((buf[i] >= '0') && (buf[i] <= '9'))	//1�`9�̕����ł���
			{
				g_mapData[j][i] = buf[i] - 0x30;	//�����𐔎��ɕϊ�
			}
		}
		j++;
	}

	fclose(fp);

}



//*---------------------------------------------------------------------
//* �ʒu�f�[�^�̓ǂݍ���
//*
//* @param [*mapFile] �ʒu�t�@�C����
//* @param [mapWidth] �}�b�v�̕�
//* @param [mapHeight] �}�b�v�̍���
//*
//* @return �Ȃ�
//*---------------------------------------------------------------------
void LoadPosition(char *posFile, int mapWidth, int mapHeight)
{
	//���[�J���ϐ��̐錾--------------------------------
	// �t�@�C���\����
	FILE *fp;
	char buf[512];

	int i;	//�z��Ɏg�p
	int j;	//�z��Ɏg�p

			//���[�J���ϐ��̏�����------------------------------
	fp = fopen(posFile, "r");

	i = 0;
	j = 0;
	//�����̓��e----------------------------------------
	if (fp == NULL)
	{
		fprintf(stderr, "�t�@�C�����I�[�v���ł��܂���\n");
		// �����I��
		exit(1);
	}

	while (fgets(buf, 512, fp) != NULL)
	{
		for (i = 0; i < mapWidth; i++)
		{
			if ((buf[i] >= '0') && (buf[i] <= '9'))	//1�`9�̕����ł���
			{
				g_posData[j][i] = buf[i] - 0x30;	//�����𐔎��ɕϊ�
			}
		}
		j++;
	}

	fclose(fp);

}
