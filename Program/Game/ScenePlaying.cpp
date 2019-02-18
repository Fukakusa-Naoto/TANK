//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   ScenePlaying.cpp
//!
//! @brief  ScenePlaying�̃\�[�X�t�@�C��
//!
//! @date   
//!
//! @author �[�����l
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// �w�b�_�t�@�C���̓ǂݍ��� ================================================
#include <math.h>
#include "ScenePlaying.h"
#include "GameMain.h"


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
	HGRP handle[16];
};
typedef struct Tag_Image Image;

struct Tag_GameObject
{
	Vec2D pos;
	Vec2D vel;
	Image image;
	int state;
	int type;
	int hp;
	int bulletInterval;
	int moveCounter;
	int moveType;
};
typedef struct Tag_GameObject GameObject;

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

struct Tag_Anime
{
	int numFrame;
	int frameDuration;
	int frameNum;
	int elapsedTime;
	int numColumns;
};
typedef struct Tag_Anime Anime;

struct Tag_Effect
{
	Vec2D pos;
	Image image;
	Anime anime;
	int state;
};
typedef struct Tag_Effect Effect;


//�萔�̒�`================================================================
//�f�o�b�O
#define DEBUG 0

//�~����
#define PI    3.1415926535897932384626433832795f

//��2
#define ROOT2 (1.41421356f)

//�摜�̔{��
#define IMAGE_EXTRATE (0.07f)

//�v���C���[�̈ړ����x
#define PLAYER_SPEED (3.0f)

//����
#define FRICTION (0.5f)

//�v���C���[�̒e�̑��x
#define PLAYER_BULLET_SPEED (4.0f)

//�v���C���[�̒e�̍ő吔
#define MAX_PLAYER_BULLETS (5)

//�G�̍ő吔
#define MAX_ENEMIES (10)

//�G�̈ړ����x
//type1,type5
#define ENEMY_SPEED_ZERO (0.0f)
//type2,type4
#define ENEMY_SPEED_LATE (2.0f)
//type3
#define ENEMY_SPEED_NORMAL (3.0f)
//type6,type7
#define ENEMY_SPEED_FAST (4.0f)

//�G�̒e�̑��x
//type1,type2,type4,type6
#define ENEMY_BULLET_SPEED_NORMAL (4.0f)
//type3,type5,type7
#define ENEMY_BULLET_SPEED_FAST (5.0f)

//�G�̒e�̍ő吔
#define MAX_ENEMY_BULLETS (100)

//�}�b�v�̍ő吔
#define MAX_MAP (100)

//�}�b�v�̕�
#define MAP_WIDTH (20)

//�}�b�v�̍���
#define MAP_HEIGHT (15)

//�}�b�v�`�b�v�T�C�Y
#define MAPCHIP_SIZE (32)

//�G�t�F�N�g�̍ő吔
#define MAX_EFFECT (100)


//�֐��̃v���g�^�C�v�錾====================================================

//�ǂݍ��񂾃}�b�v�f�[�^�̕`��
void DrawMap(int mapWidth, int mapHeight);

//�}�E�X��p�����Ə�
void MouseRanging(void);

//��Ԃ̕`��
void DrawTank(GameObject body, GameObject battery);

//�Ə��ɍ��킹���摜�̉�]
void  RotaGraph(void);

//�v���C���[�̈ړ�
void MovePlayer(void);

//�}�b�v��̈ړ�����
int LimitMapArea(GameObject *name);

//�v���C���[�̒e�̔���
void PlayerShootBullet(void);

//���˂��ꂽ�e�̈ړ�
void MoveBullet(GameObject *bullet);

//�e�̈ړ�����
void LimitBulletAria(GameObject *bullet);

//�����̌�������
int Intersection(float p1x, float p1y, float p2x, float p2y, float p3x, float p3y, float p4x, float p4y);

//�}�b�v�`�b�v�Ƃ̓����蔻��
int DetectCollisionMapChip(GameObject *bullet, int mapX, int mapY);

//�G�̈ړ�
void MoveEnemy(void);

//�ړ����@
//��
void MoveUp(GameObject *name);
//��
void MoveDown(GameObject *name);
//��
void MoveLeft(GameObject *name);
//�E
void MoveRight(GameObject *name);

//�G�̒e�̔���
void EnemyShootBullet(void);

//�e���\��
int ForecastTrajectory(GameObject *shooter, GameObject *target, int mapX, int mapY);

//�e�̔��˕��@
//�����e
void ShootWayBullet(GameObject *shooter, GameObject *bullet, GameObject *target);

//�e�Ƃ̓����蔻��
int DetectCollisionBullet(GameObject *main, GameObject *partner);

//�����A�j���[�V����
void EffectExplosion(Effect *name);

// �O���[�o���ϐ��̒�` ====================================================
HGRP g_image[100];

HGRP g_tankImage[16];

//���̓f�o�C�X�̏��
int g_currentInputMouseState;	//���݂̃}�E�X�̓��͏��
int g_lastInputMouseState;	//�O��̃}�E�X�̓��͏��

int g_mousePosX;
int g_mousePosY;

//�v���C���[
//�C��
GameObject g_playerBattery;
//�ԑ�
GameObject g_playerBody;
//�e
GameObject g_playerBullets[MAX_PLAYER_BULLETS];

//�G�l�~�[
//�C��
GameObject g_enemyBattery[MAX_ENEMIES];
//�ԑ�
GameObject g_enemyBody[MAX_ENEMIES];
//�e
GameObject g_enemyBullets[MAX_ENEMY_BULLETS];

//�G�̐�
int g_enemyNumber;

//�X�R�A
extern Score g_score;

//�}�b�v�f�[�^
extern int g_mapData[MAP_HEIGHT][MAP_WIDTH];
//�ʒu�f�[�^
extern int g_posData[MAP_HEIGHT][MAP_WIDTH];
//�}�b�v�̉摜
int g_mapchip[10];
//�X�e�[�W
extern int g_stageNumber;

//���ʉ�
int g_effectSound[10];

//�G�t�F�N�g�A�j���[�V����
//��Ԃ̔���
Effect g_explosionTank[MAX_EFFECT];
//�e�̔���
Effect g_explosionBullet[MAX_EFFECT];


// �֐��̒�` ==============================================================
//----------------------------------------------------------------------
//! @brief �Q�[���̏���������
//!
//! @param[in] �Ȃ�
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void InitializePlaying(void)
{
	//���[�J���ϐ��̐錾------------------------------------------------
	int i;	//�z��Ɏg�p
	int j;	//�z��Ɏg�p
	int k;	//�z��Ɏg�p
	

	//���[�J���ϐ��̏�����----------------------------------------------




	//�����̓��e--------------------------------------------------------
	g_image[0] = LoadGraph("Resources/Images/�Ə�.png");
	g_image[5] = LoadGraph("Resources/Images/�e.png");

	LoadDivGraph("Resources/Images/tank.png", 16, 4, 4, 654, 654, g_tankImage);

	SetMouseDispFlag(FALSE);
	g_mousePosX = 0;
	g_mousePosY = 0;

	//�}�b�v�摜�̓ǂݍ���
	g_mapchip[0] = { LoadGraph("Resources/Images/�}�b�v�`�b�v2.png") };
	g_mapchip[1] = { LoadGraph("Resources/Images/�}�b�v�`�b�v1.png") };
	g_mapchip[2] = { LoadGraph("Resources/Images/�}�b�v�`�b�v3.png") };

	//���ʉ�
	//�e�̔�����
	g_effectSound[0] = LoadSoundMem("Resources/Sounds/se_maoudamashii_battle18.mp3");
	//��Ԃ̔�����
	g_effectSound[1] = LoadSoundMem("Resources/Sounds/se_maoudamashii_explosion03.mp3");
	//�e�̔��ˉ�
	g_effectSound[2] = LoadSoundMem("Resources/Sounds/se_maoudamashii_instruments_drum2_tom2.mp3");
	//�e���ǂɔ��˂��鉹
	g_effectSound[3] = LoadSoundMem("Resources/Sounds/se_maoudamashii_se_finger01.mp3");
	

	//�G��state�̏�����
	for (i = 0; i < MAX_ENEMIES; i++)
	{
		g_enemyBattery[i].state = 0;
		g_enemyBody[i].state = 0;
	}

	//�G�̐��̏�����
	g_enemyNumber = 0;

	//�e��Ԃ������ʒu�ɔz�u
	for (i = 0; i < MAP_HEIGHT; i++)
	{
		for (j = 0; j < MAP_WIDTH; j++)
		{
			switch (g_posData[i][j])
			{
			case 1:
				//�v���C���[
				//�C��
				g_playerBattery.pos.x = (float)MAPCHIP_SIZE*j;
				g_playerBattery.pos.y = (float)MAPCHIP_SIZE*i;
				g_playerBattery.vel.x = 0.0f;
				g_playerBattery.vel.y = 0.0f;
				g_playerBattery.image.extRate = IMAGE_EXTRATE;
				g_playerBattery.image.angle = 0.0;
				g_playerBattery.image.handle[0] = g_tankImage[0];
				g_playerBattery.state = 1;
				g_playerBattery.type = 0;
				g_playerBattery.hp = 1;

				//�ԑ�
				g_playerBody.pos.x = (float)MAPCHIP_SIZE*j;
				g_playerBody.pos.y = (float)MAPCHIP_SIZE*i;
				g_playerBody.vel.x = 0.0f;
				g_playerBody.vel.y = 0.0f;
				g_playerBody.image.extRate = IMAGE_EXTRATE;
				g_playerBody.image.angle = 0.0f;
				g_playerBody.image.handle[0] = g_tankImage[1];
				g_playerBody.state = 1;
				g_playerBody.type = 0;
				g_playerBody.hp = 1;

				break;

			//�G���
			case 2:
				//type1
				for (k = 0; k < MAX_ENEMIES; k++)
				{
					if ((g_enemyBattery[k].state == 0) && (g_enemyBody[k].state == 0))
					{
						//�C��
						g_enemyBattery[k].pos.x = (float)MAPCHIP_SIZE*j;
						g_enemyBattery[k].pos.y = (float)MAPCHIP_SIZE*i;
						g_enemyBattery[k].vel.x = 0.0f;
						g_enemyBattery[k].vel.y = 0.0f;
						g_enemyBattery[k].image.extRate = IMAGE_EXTRATE;
						g_enemyBattery[k].image.angle = 0.0;
						g_enemyBattery[k].image.handle[0] = g_tankImage[2];
						g_enemyBattery[k].state = 1;
						g_enemyBattery[k].type = 1;
						g_enemyBattery[k].hp = 1;
						g_enemyBattery[k].bulletInterval = 60;
						g_enemyBattery[k].moveCounter = 0;
						g_enemyBattery[k].moveType = 0;

						//�ԑ�
						g_enemyBody[k].pos.x = (float)MAPCHIP_SIZE*j;
						g_enemyBody[k].pos.y = (float)MAPCHIP_SIZE*i;
						g_enemyBody[k].vel.x = 0.0f;
						g_enemyBody[k].vel.y = 0.0f;
						g_enemyBody[k].image.extRate = IMAGE_EXTRATE;
						g_enemyBody[k].image.angle = 0.0f;
						g_enemyBody[k].image.handle[0] = g_tankImage[3];
						g_enemyBody[k].state = 1;
						g_enemyBody[k].type = 1;
						g_enemyBody[k].hp = 1;
						g_enemyBody[k].bulletInterval = 0;
						g_enemyBody[k].moveCounter = 0;
						g_enemyBody[k].moveType = 0;

						//�o�������G�̐��𐔂���
						g_enemyNumber++;

						break;
					}
				}

				break;

			case 3:
				//type2
				for (k = 0; k < MAX_ENEMIES; k++)
				{
					if ((g_enemyBattery[k].state == 0) && (g_enemyBody[k].state == 0))
					{
						//�C��
						g_enemyBattery[k].pos.x = (float)MAPCHIP_SIZE*j;
						g_enemyBattery[k].pos.y = (float)MAPCHIP_SIZE*i;
						g_enemyBattery[k].vel.x = 0.0f;
						g_enemyBattery[k].vel.y = 0.0f;
						g_enemyBattery[k].image.extRate = IMAGE_EXTRATE;
						g_enemyBattery[k].image.angle = 0.0;
						g_enemyBattery[k].image.handle[0] = g_tankImage[4];
						g_enemyBattery[k].state = 1;
						g_enemyBattery[k].type = 2;
						g_enemyBattery[k].hp = 1;
						g_enemyBattery[k].bulletInterval = 60;
						g_enemyBattery[k].moveCounter = 0;
						g_enemyBattery[k].moveType = 0;

						//�ԑ�
						g_enemyBody[k].pos.x = (float)MAPCHIP_SIZE*j;
						g_enemyBody[k].pos.y = (float)MAPCHIP_SIZE*i;
						g_enemyBody[k].vel.x = 0.0f;
						g_enemyBody[k].vel.y = 0.0f;
						g_enemyBody[k].image.extRate = IMAGE_EXTRATE;
						g_enemyBody[k].image.angle = 0.0f;
						g_enemyBody[k].image.handle[0] = g_tankImage[5];
						g_enemyBody[k].state = 1;
						g_enemyBody[k].type = 2;
						g_enemyBody[k].hp = 1;
						g_enemyBody[k].bulletInterval = 0;
						g_enemyBody[k].moveCounter = 0;
						g_enemyBody[k].moveType = 0;

						//�o�������G�̐��𐔂���
						g_enemyNumber++;

						break;
					}
				}

				break;

			case 4:
				//type3
				for (k = 0; k < MAX_ENEMIES; k++)
				{
					if ((g_enemyBattery[k].state == 0) && (g_enemyBody[k].state == 0))
					{
						//�C��
						g_enemyBattery[k].pos.x = (float)MAPCHIP_SIZE*j;
						g_enemyBattery[k].pos.y = (float)MAPCHIP_SIZE*i;
						g_enemyBattery[k].vel.x = 0.0f;
						g_enemyBattery[k].vel.y = 0.0f;
						g_enemyBattery[k].image.extRate = IMAGE_EXTRATE;
						g_enemyBattery[k].image.angle = 0.0;
						g_enemyBattery[k].image.handle[0] = g_tankImage[6];
						g_enemyBattery[k].state = 1;
						g_enemyBattery[k].type = 3;
						g_enemyBattery[k].hp = 1;
						g_enemyBattery[k].bulletInterval = 60;
						g_enemyBattery[k].moveCounter = 0;
						g_enemyBattery[k].moveType = 0;

						//�ԑ�
						g_enemyBody[k].pos.x = (float)MAPCHIP_SIZE*j;
						g_enemyBody[k].pos.y = (float)MAPCHIP_SIZE*i;
						g_enemyBody[k].vel.x = 0.0f;
						g_enemyBody[k].vel.y = 0.0f;
						g_enemyBody[k].image.extRate = IMAGE_EXTRATE;
						g_enemyBody[k].image.angle = 0.0f;
						g_enemyBody[k].image.handle[0] = g_tankImage[7];
						g_enemyBody[k].state = 1;
						g_enemyBody[k].type = 3;
						g_enemyBody[k].hp = 1;
						g_enemyBody[k].bulletInterval = 0;
						g_enemyBody[k].moveCounter = 0;
						g_enemyBody[k].moveType = 0;

						//�o�������G�̐��𐔂���
						g_enemyNumber++;

						break;
					}
				}

				break;

			case 5:
				//type4
				for (k = 0; k < MAX_ENEMIES; k++)
				{
					if ((g_enemyBattery[k].state == 0) && (g_enemyBody[k].state == 0))
					{
						//�C��
						g_enemyBattery[k].pos.x = (float)MAPCHIP_SIZE*j;
						g_enemyBattery[k].pos.y = (float)MAPCHIP_SIZE*i;
						g_enemyBattery[k].vel.x = 0.0f;
						g_enemyBattery[k].vel.y = 0.0f;
						g_enemyBattery[k].image.extRate = IMAGE_EXTRATE;
						g_enemyBattery[k].image.angle = 0.0;
						g_enemyBattery[k].image.handle[0] = g_tankImage[8];
						g_enemyBattery[k].state = 1;
						g_enemyBattery[k].type = 4;
						g_enemyBattery[k].hp = 1;
						g_enemyBattery[k].bulletInterval = 60;
						g_enemyBattery[k].moveCounter = 0;
						g_enemyBattery[k].moveType = 0;

						//�ԑ�
						g_enemyBody[k].pos.x = (float)MAPCHIP_SIZE*j;
						g_enemyBody[k].pos.y = (float)MAPCHIP_SIZE*i;
						g_enemyBody[k].vel.x = 0.0f;
						g_enemyBody[k].vel.y = 0.0f;
						g_enemyBody[k].image.extRate = IMAGE_EXTRATE;
						g_enemyBody[k].image.angle = 0.0f;
						g_enemyBody[k].image.handle[0] = g_tankImage[9];
						g_enemyBody[k].state = 1;
						g_enemyBody[k].type = 4;
						g_enemyBody[k].hp = 1;
						g_enemyBody[k].bulletInterval = 0;
						g_enemyBody[k].moveCounter = 0;
						g_enemyBody[k].moveType = 0;

						//�o�������G�̐��𐔂���
						g_enemyNumber++;

						break;
					}
				}

				break;

			case 6:
				//type5
				for (k = 0; k < MAX_ENEMIES; k++)
				{
					if ((g_enemyBattery[k].state == 0) && (g_enemyBody[k].state == 0))
					{
						//�C��
						g_enemyBattery[k].pos.x = (float)MAPCHIP_SIZE*j;
						g_enemyBattery[k].pos.y = (float)MAPCHIP_SIZE*i;
						g_enemyBattery[k].vel.x = 0.0f;
						g_enemyBattery[k].vel.y = 0.0f;
						g_enemyBattery[k].image.extRate = IMAGE_EXTRATE;
						g_enemyBattery[k].image.angle = 0.0;
						g_enemyBattery[k].image.handle[0] = g_tankImage[10];
						g_enemyBattery[k].state = 1;
						g_enemyBattery[k].type = 5;
						g_enemyBattery[k].hp = 1;
						g_enemyBattery[k].bulletInterval = 60;
						g_enemyBattery[k].moveCounter = 0;
						g_enemyBattery[k].moveType = 0;

						//�ԑ�
						g_enemyBody[k].pos.x = (float)MAPCHIP_SIZE*j;
						g_enemyBody[k].pos.y = (float)MAPCHIP_SIZE*i;
						g_enemyBody[k].vel.x = 0.0f;
						g_enemyBody[k].vel.y = 0.0f;
						g_enemyBody[k].image.extRate = IMAGE_EXTRATE;
						g_enemyBody[k].image.angle = 0.0f;
						g_enemyBody[k].image.handle[0] = g_tankImage[11];
						g_enemyBody[k].state = 1;
						g_enemyBody[k].type = 5;
						g_enemyBody[k].hp = 1;
						g_enemyBody[k].bulletInterval = 0;
						g_enemyBody[k].moveCounter = 0;
						g_enemyBody[k].moveType = 0;

						//�o�������G�̐��𐔂���
						g_enemyNumber++;

						break;
					}
				}

				break;

			case 7:
				//type6
				for (k = 0; k < MAX_ENEMIES; k++)
				{
					if ((g_enemyBattery[k].state == 0) && (g_enemyBody[k].state == 0))
					{
						//�C��
						g_enemyBattery[k].pos.x = (float)MAPCHIP_SIZE*j;
						g_enemyBattery[k].pos.y = (float)MAPCHIP_SIZE*i;
						g_enemyBattery[k].vel.x = 0.0f;
						g_enemyBattery[k].vel.y = 0.0f;
						g_enemyBattery[k].image.extRate = IMAGE_EXTRATE;
						g_enemyBattery[k].image.angle = 0.0;
						g_enemyBattery[k].image.handle[0] = g_tankImage[12];
						g_enemyBattery[k].state = 1;
						g_enemyBattery[k].type = 6;
						g_enemyBattery[k].hp = 1;
						g_enemyBattery[k].bulletInterval = 60;
						g_enemyBattery[k].moveCounter = 0;
						g_enemyBattery[k].moveType = 0;

						//�ԑ�
						g_enemyBody[k].pos.x = (float)MAPCHIP_SIZE*j;
						g_enemyBody[k].pos.y = (float)MAPCHIP_SIZE*i;
						g_enemyBody[k].vel.x = 0.0f;
						g_enemyBody[k].vel.y = 0.0f;
						g_enemyBody[k].image.extRate = IMAGE_EXTRATE;
						g_enemyBody[k].image.angle = 0.0f;
						g_enemyBody[k].image.handle[0] = g_tankImage[13];
						g_enemyBody[k].state = 1;
						g_enemyBody[k].type = 6;
						g_enemyBody[k].hp = 1;
						g_enemyBody[k].bulletInterval = 0;
						g_enemyBody[k].moveCounter = 0;
						g_enemyBody[k].moveType = 0;

						//�o�������G�̐��𐔂���
						g_enemyNumber++;

						break;
					}
				}

				break;
			case 8:
				//type7
				for (k = 0; k < MAX_ENEMIES; k++)
				{
					if ((g_enemyBattery[k].state == 0) && (g_enemyBody[k].state == 0))
					{
						//�C��
						g_enemyBattery[k].pos.x = (float)MAPCHIP_SIZE*j;
						g_enemyBattery[k].pos.y = (float)MAPCHIP_SIZE*i;
						g_enemyBattery[k].vel.x = 0.0f;
						g_enemyBattery[k].vel.y = 0.0f;
						g_enemyBattery[k].image.extRate = IMAGE_EXTRATE;
						g_enemyBattery[k].image.angle = 0.0;
						g_enemyBattery[k].image.handle[0] = g_tankImage[14];
						g_enemyBattery[k].state = 1;
						g_enemyBattery[k].type = 7;
						g_enemyBattery[k].hp = 1;
						g_enemyBattery[k].bulletInterval = 60;
						g_enemyBattery[k].moveCounter = 0;
						g_enemyBattery[k].moveType = 0;

						//�ԑ�
						g_enemyBody[k].pos.x = (float)MAPCHIP_SIZE*j;
						g_enemyBody[k].pos.y = (float)MAPCHIP_SIZE*i;
						g_enemyBody[k].vel.x = 0.0f;
						g_enemyBody[k].vel.y = 0.0f;
						g_enemyBody[k].image.extRate = IMAGE_EXTRATE;
						g_enemyBody[k].image.angle = 0.0f;
						g_enemyBody[k].image.handle[0] = g_tankImage[15];
						g_enemyBody[k].state = 1;
						g_enemyBody[k].type = 7;
						g_enemyBody[k].hp = 1;
						g_enemyBody[k].bulletInterval = 0;
						g_enemyBody[k].moveCounter = 0;
						g_enemyBody[k].moveType = 0;

						//�o�������G�̐��𐔂���
						g_enemyNumber++;

						break;
					}
				}

				break;

			default:
				break;
			}
		}
	}

	//�v���C���[�̒e
	for (i = 0; i < MAX_PLAYER_BULLETS; i++)
	{
		g_playerBullets[i].pos.x = (float)SCREEN_CENTER_X;
		g_playerBullets[i].pos.y = (float)SCREEN_CENTER_Y;
		g_playerBullets[i].vel.x = 0.0f;
		g_playerBullets[i].vel.y = 0.0f;
		g_playerBullets[i].image.extRate = IMAGE_EXTRATE;
		g_playerBullets[i].image.angle = 0.0f;
		g_playerBullets[i].image.handle[0] = g_image[5];
		g_playerBullets[i].state = 0;
		g_playerBullets[i].type = 0;
		g_playerBullets[i].hp = 2;
	}

	//�G�l�~�[�̒e
	for (i = 0; i < MAX_ENEMY_BULLETS; i++)
	{
		g_enemyBullets[i].pos.x = (float)SCREEN_CENTER_X;
		g_enemyBullets[i].pos.y = (float)SCREEN_CENTER_Y;
		g_enemyBullets[i].vel.x = 0.0f;
		g_enemyBullets[i].vel.y = 0.0f;
		g_enemyBullets[i].image.extRate = IMAGE_EXTRATE;
		g_enemyBullets[i].image.angle = 0.0f;
		g_enemyBullets[i].image.handle[0] = g_image[5];
		g_enemyBullets[i].state = 0;
		g_enemyBullets[i].type = 0;
		g_enemyBullets[i].hp = 0;
	}

	//�G�t�F�N�g
	//��Ԃ̔���
	for (i = 0; i < MAX_EFFECT; i++)
	{
		g_explosionTank[i].pos.x = 0.0f;
		g_explosionTank[i].pos.y = 0.0f;
		g_explosionTank[i].image.angle = 0.0;
		g_explosionTank[i].image.extRate = 0.8;
		LoadDivGraph("Resources/Images/Explosion-org.png", 16, 4, 4, 64, 64, g_explosionTank[i].image.handle);
		g_explosionTank[i].anime.elapsedTime = 0;
		g_explosionTank[i].anime.frameDuration = 5;
		g_explosionTank[i].anime.frameNum = 0;
		g_explosionTank[i].anime.numColumns = 4;
		g_explosionTank[i].anime.numFrame = 15;
		g_explosionTank[i].state = 0;
	}
	
	//�e�̔���
	for (i = 0; i < MAX_EFFECT; i++)
	{
		g_explosionBullet[i].pos.x = 0.0f;
		g_explosionBullet[i].pos.y = 0.0f;
		g_explosionBullet[i].image.angle = 0.0;
		g_explosionBullet[i].image.extRate = 0.25;
		LoadDivGraph("Resources/Images/Explosion-org.png", 16, 4, 4, 64, 64, g_explosionBullet[i].image.handle);
		g_explosionBullet[i].anime.elapsedTime = 0;
		g_explosionBullet[i].anime.frameDuration = 5;
		g_explosionBullet[i].anime.frameNum = 0;
		g_explosionBullet[i].anime.numColumns = 4;
		g_explosionBullet[i].anime.numFrame = 15;
		g_explosionBullet[i].state = 0;
	}
}



//----------------------------------------------------------------------
//! @brief �Q�[���̍X�V����
//!
//! @param[in] �Ȃ�
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void UpdatePlaying(void)
{
	//���[�J���ϐ��̐錾------------------------------------------------
	int i;	//�z��Ɏg�p
	int j;	//�z��Ɏg�p
	int k;	//�z��Ɏg�p

	int result;	//�����蔻��Ɏg�p

	//���[�J���ϐ��̏�����----------------------------------------------



	//�����̓��e--------------------------------------------------------

	//�}�E�X��p�����Ə�
	MouseRanging();


	//�Ə��ɍ��킹���摜�̉�]
	RotaGraph();

	//�v���C���[�̈ړ�
	MovePlayer();

	//�}�b�v��̈ړ�����
	LimitMapArea(&g_playerBody);

	//�v���C���[�̒e�̔���
	PlayerShootBullet();

	//���˂��ꂽ�e�̈ړ�
	for (i = 0; i < MAX_PLAYER_BULLETS; i++)
	{
		if (g_playerBullets[i].state == 1)
		{
			MoveBullet(&g_playerBullets[i]);
			//�e�̈ړ�����
			LimitBulletAria(&g_playerBullets[i]);
		}
	}

	//�G�̈ړ�
	MoveEnemy();

	//�G�̒e�̔���
	EnemyShootBullet();

	//�e�̈ړ�
	for (i = 0; i < MAX_ENEMY_BULLETS; i++)
	{
		if (g_enemyBullets[i].state == 1)
		{
			MoveBullet(&g_enemyBullets[i]);
			//�e�̈ړ�����
			LimitBulletAria(&g_enemyBullets[i]);
		}
	}

	//�����蔻��
	//�v���C���[�ƃv���C���[�̒e
	if (g_playerBody.state == 1)
	{
		for (i = 0; i < MAX_PLAYER_BULLETS; i++)
		{
			if ((g_playerBullets[i].state == 1) && (g_playerBullets[i].hp == 1))
			{
				result = DetectCollisionBullet(&g_playerBody, &g_playerBullets[i]);
				if (result == 1)
				{
					g_playerBattery.state = 0;
					g_playerBody.state = 0;
					g_playerBullets[i].state = 0;

					//�����G�t�F�N�g
					for (j = 0; j < MAX_EFFECT; j++)
					{
						if (g_explosionTank[j].state == 0)
						{
							g_explosionTank[j].pos = g_playerBody.pos;
							g_explosionTank[j].anime.elapsedTime = 0;
							g_explosionTank[j].anime.frameNum = 0;
							g_explosionTank[j].state = 1;
							break;
						}
					}

					//���ʉ��̍Đ�
					PlaySoundMem(g_effectSound[1], DX_PLAYTYPE_BACK, TRUE);

				}
			}
		}
	}

	//�v���C���[�̒e�ƓG
	for (i = 0; i < MAX_PLAYER_BULLETS; i++)
	{
		if (g_playerBullets[i].state == 1)
		{
			for (j = 0; j < MAX_ENEMIES; j++)
			{
				if (g_enemyBody[j].state == 1)
				{
					result = DetectCollisionBullet(&g_enemyBody[j], &g_playerBullets[i]);
					if (result == 1)
					{
						g_playerBullets[i].state = 0;
						g_enemyBattery[j].state = 0;
						g_enemyBody[j].state = 0;

						//�����G�t�F�N�g
						for (k = 0; k < MAX_EFFECT; k++)
						{
							if (g_explosionTank[k].state == 0)
							{
								g_explosionTank[k].pos = g_enemyBody[j].pos;
								g_explosionTank[k].anime.elapsedTime = 0;
								g_explosionTank[k].anime.frameNum = 0;
								g_explosionTank[k].state = 1;
								break;
							}
						}

						//�X�R�A�̃J�E���g
						switch (g_enemyBody[j].type)
						{
						case 1:
							g_score.type1++;
							break;
						case 2:
							g_score.type2++;
							break;
						case 3:
							g_score.type3++;
							break;
						case 4:
							g_score.type4++;
							break;
						case 5:
							g_score.type5++;
							break;
						case 6:
							g_score.type6++;
							break;
						case 7:
							g_score.type7++;
							break;
						default:

							break;
						}

						//�o�����Ă���G�̐������炷
						g_enemyNumber--;

						//���ʉ��̍Đ�
						PlaySoundMem(g_effectSound[1], DX_PLAYTYPE_BACK, TRUE);
					}
				}
			}
		}
	}

	//�G�̒e�ƓG
	for (i = 0; i < MAX_ENEMIES; i++)
	{
		if (g_enemyBody[i].state == 1)
		{
			for (j = 0; j < MAX_ENEMY_BULLETS; j++)
			{
				if ((g_enemyBullets[j].state == 1) && (g_enemyBullets[j].hp == 1))
				{
					result = DetectCollisionBullet(&g_enemyBody[i], &g_enemyBullets[j]);
					if (result == 1)
					{
						g_enemyBattery[i].state = 0;
						g_enemyBody[i].state = 0;
						g_enemyBullets[j].state = 0;

						//�����G�t�F�N�g
						for (k = 0; k < MAX_EFFECT; k++)
						{
							if (g_explosionTank[k].state == 0)
							{
								g_explosionTank[k].pos = g_enemyBody[i].pos;
								g_explosionTank[k].anime.elapsedTime = 0;
								g_explosionTank[k].anime.frameNum = 0;
								g_explosionTank[k].state = 1;
								break;
							}
						}

						//�o�����Ă���G�̐������炷
						g_enemyNumber--;

						//���ʉ��̍Đ�
						PlaySoundMem(g_effectSound[1], DX_PLAYTYPE_BACK, TRUE);
					}
				}
			}
		}
	}

	//�v���C���[�ƓG�̒e
	if (g_playerBody.state == 1)
	{
		for (i = 0; i < MAX_ENEMY_BULLETS; i++)
		{
			if (g_enemyBullets[i].state == 1)
			{
				result = DetectCollisionBullet(&g_playerBody, &g_enemyBullets[i]);
				if (result == 1)
				{
					g_playerBattery.state = 0;
					g_playerBody.state = 0;
					g_enemyBullets[i].state = 0;

					//�����G�t�F�N�g
					for (j = 0; j < MAX_EFFECT; j++)
					{
						if (g_explosionTank[j].state == 0)
						{
							g_explosionTank[j].pos = g_playerBody.pos;
							g_explosionTank[j].anime.elapsedTime = 0;
							g_explosionTank[j].anime.frameNum = 0;
							g_explosionTank[j].state = 1;
							break;
						}
					}
				}
			}
		}
	}

	//�v���C���[�̒e�ƒe
	for (i = 0; i < MAX_PLAYER_BULLETS; i++)
	{
		if (g_playerBullets[i].state == 1)
		{
			for (j = 0; j < MAX_PLAYER_BULLETS; j++)
			{
				if (g_playerBullets[j].state == 1)
				{
					result = DetectCollisionBullet(&g_playerBullets[i], &g_playerBullets[j]);
					if (result == 1)
					{
						g_playerBullets[i].state = 0;
						g_playerBullets[j].state = 0;

						//�����G�t�F�N�g
						for (k = 0; k < MAX_EFFECT; k++)
						{
							if (g_explosionBullet[k].state == 0)
							{
								g_explosionBullet[k].pos = g_playerBullets[i].pos;
								g_explosionBullet[k].anime.elapsedTime = 0;
								g_explosionBullet[k].anime.frameNum = 0;
								g_explosionBullet[k].state = 1;
								break;
							}
						}
						for (k = 0; k < MAX_EFFECT; k++)
						{
							if (g_explosionTank[k].state == 0)
							{
								g_explosionBullet[k].pos = g_playerBullets[j].pos;
								g_explosionBullet[k].anime.elapsedTime = 0;
								g_explosionBullet[k].anime.frameNum = 0;
								g_explosionBullet[k].state = 1;
								break;
							}
						}

						//���ʉ��̍Đ�
						PlaySoundMem(g_effectSound[0], DX_PLAYTYPE_BACK, TRUE);
					}
				}
			}
		}
	}


	//�v���C���[�̒e�ƓG�̒e
	for (i = 0; i < MAX_PLAYER_BULLETS; i++)
	{
		if (g_playerBullets[i].state == 1)
		{
			for (j = 0; j < MAX_ENEMY_BULLETS; j++)
			{
				if (g_enemyBullets[j].state == 1)
				{
					result = DetectCollisionBullet(&g_playerBullets[i], &g_enemyBullets[j]);
					if (result == 1)
					{
						g_playerBullets[i].state = 0;
						g_enemyBullets[j].state = 0;

						//�����G�t�F�N�g
						for (k = 0; k < MAX_EFFECT; k++)
						{
							if (g_explosionBullet[k].state == 0)
							{
								g_explosionBullet[k].pos = g_playerBullets[i].pos;
								g_explosionBullet[k].anime.elapsedTime = 0;
								g_explosionBullet[k].anime.frameNum = 0;
								g_explosionBullet[k].state = 1;
								break;
							}
						}
						for (k = 0; k < MAX_EFFECT; k++)
						{
							if (g_explosionBullet[k].state == 0)
							{
								g_explosionBullet[k].pos = g_enemyBullets[j].pos;
								g_explosionBullet[k].anime.elapsedTime = 0;
								g_explosionBullet[k].anime.frameNum = 0;
								g_explosionBullet[k].state = 1;
								break;
							}
						}

						//���ʉ��̍Đ�
						PlaySoundMem(g_effectSound[0], DX_PLAYTYPE_BACK, TRUE);
					}
				}
			}
		}
	}

	//�G�̒e�ƒe
	for (i = 0; i < MAX_ENEMY_BULLETS; i++)
	{
		if (g_enemyBullets[i].state == 1)
		{
			for (j = 0; j < MAX_ENEMY_BULLETS; j++)
			{
				if (g_enemyBullets[j].state == 1)
				{
					result = DetectCollisionBullet(&g_enemyBullets[i], &g_enemyBullets[j]);
					if (result == 1)
					{
						g_enemyBullets[i].state = 0;
						g_enemyBullets[j].state = 0;

						//�����G�t�F�N�g
						for (k = 0; k < MAX_EFFECT; k++)
						{
							if (g_explosionBullet[k].state == 0)
							{
								g_explosionBullet[k].pos = g_enemyBullets[i].pos;
								g_explosionBullet[k].anime.elapsedTime = 0;
								g_explosionBullet[k].anime.frameNum = 0;
								g_explosionBullet[k].state = 1;
								break;
							}
						}
						for (k = 0; k < MAX_EFFECT; k++)
						{
							if (g_explosionBullet[k].state == 0)
							{
								g_explosionBullet[k].pos = g_enemyBullets[j].pos;
								g_explosionBullet[k].anime.elapsedTime = 0;
								g_explosionBullet[k].anime.frameNum = 0;
								g_explosionBullet[k].state = 1;
								break;
							}
						}

						//���ʉ��̍Đ�
						PlaySoundMem(g_effectSound[0], DX_PLAYTYPE_BACK, TRUE);
					}
				}
			}
		}
	}



	//�����G�t�F�N�g�̏���
	//���
	for (i = 0; i < MAX_EFFECT; i++)
	{
		if (g_explosionTank[i].state == 1)
		{
			EffectExplosion(&g_explosionTank[i]);
		}
	}

	//�e
	for (i = 0; i < MAX_EFFECT; i++)
	{
		if (g_explosionBullet[i].state == 1)
		{
			EffectExplosion(&g_explosionBullet[i]);
		}
	}



	//�v���C�̏I��
	//�v���C���[�����ꂽ�ꍇ
	if (g_playerBody.state == 0)
	{
		//�V�[���̐؂�ւ�
		ChangeScene(SCENE_RESULT);	//���U���g�V�[����
	}

	//�G��S�ł������ꍇ
	if (g_enemyNumber == 0)
	{
		//�ŏI�X�e�[�W�������ꍇ
		if (g_stageNumber == 10)
		{
			//�V�[���̐؂�ւ�
			ChangeScene(SCENE_RESULT);	//���U���g�V�[����
		}
		else
		{
			//���̃X�e�[�W��
			g_stageNumber++;
			ChangeScene(SCENE_READY);	//�v���C�O�����V�[����
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
void RenderPlaying(void)
{
	//���[�J���ϐ��̐錾------------------------------------------------
	int i;	//�z��Ɏg�p

	//���[�J���ϐ��̏�����----------------------------------------------



	//�����̓��e--------------------------------------------------------
	//�w�i
	//�}�b�v�̕`��
	DrawMap(MAP_WIDTH, MAP_HEIGHT);

	//�v���C���[�̕`��//
	if ((g_playerBattery.state == 1) && (g_playerBody.state == 1))
	{
		DrawTank(g_playerBody, g_playerBattery);
	}

	//�e�̕`��
	for (i = 0; i < MAX_PLAYER_BULLETS; i++)
	{
		if (g_playerBullets[i].state == 1)
		{
			DrawRotaGraph((int)g_playerBullets[i].pos.x,(int)g_playerBullets[i].pos.y, g_playerBullets[i].image.extRate, g_playerBullets[i].image.angle, g_playerBullets[i].image.handle[0], TRUE, FALSE);
		}
	}


	//�G�̕`��//
	for (i = 0; i < MAX_ENEMIES; i++)
	{
		if ((g_enemyBattery[i].state == 1) && (g_enemyBody[i].state == 1))
		{
			DrawTank(g_enemyBody[i], g_enemyBattery[i]);
		}
	}
	//�e�̕`��
	for (i = 0; i < MAX_ENEMY_BULLETS; i++)
	{
		if (g_enemyBullets[i].state == 1)
		{
			DrawRotaGraph((int)g_enemyBullets[i].pos.x, (int)g_enemyBullets[i].pos.y, g_enemyBullets[i].image.extRate, g_enemyBullets[i].image.angle, g_enemyBullets[i].image.handle[0], TRUE, FALSE);
		}
	}

	//�����̕`��
	//���
	for (i = 0; i < MAX_EFFECT; i++)
	{
		if (g_explosionTank[i].state == 1)
		{
			DrawRotaGraph((int)g_explosionTank[i].pos.x, (int)g_explosionTank[i].pos.y, g_explosionTank[i].image.extRate, g_explosionTank[i].image.angle, g_explosionTank[i].image.handle[0], TRUE, FALSE);
		}
	}

	//�e
	for (i = 0; i < MAX_EFFECT; i++)
	{
		if (g_explosionBullet[i].state == 1)
		{
			DrawRotaGraph((int)g_explosionBullet[i].pos.x, (int)g_explosionBullet[i].pos.y, g_explosionBullet[i].image.extRate, g_explosionBullet[i].image.angle, g_explosionBullet[i].image.handle[0], TRUE, FALSE);
		}
	}

	//�����G�t�F�N�g
	//���
	for (i = 0; i < MAX_EFFECT; i++)
	{
		if (g_explosionTank[i].state == 1)
		{
			DrawRotaGraph((int)g_explosionTank[i].pos.x, (int)g_explosionTank[i].pos.y, g_explosionTank[i].image.extRate, g_explosionTank[i].image.angle, g_explosionTank[i].image.handle[g_explosionTank[i].anime.frameNum], TRUE, FALSE);
		}
	}

	//�e
	for (i = 0; i < MAX_EFFECT; i++)
	{
		if (g_explosionBullet[i].state == 1)
		{
			DrawRotaGraph((int)g_explosionBullet[i].pos.x, (int)g_explosionBullet[i].pos.y, g_explosionBullet[i].image.extRate, g_explosionBullet[i].image.angle, g_explosionBullet[i].image.handle[g_explosionBullet[i].anime.frameNum], TRUE, FALSE);
		}
	}

	//�Ə��̕`��
	DrawRotaGraph(g_mousePosX, g_mousePosY, IMAGE_EXTRATE, PI / 2, g_image[0], TRUE);

	//�f�o�b�O
#if DEBUG
	DrawFormatString(0, 0, COLOR_WHITE, "�J�[�\��X=%d", g_mousePosX);
	DrawFormatString(0, 20, COLOR_WHITE, "�J�[�\��Y=%d", g_mousePosY);
	DrawFormatString(0, 40, COLOR_WHITE, "playerPosX=%.1f", g_playerBody.pos.x);
	DrawFormatString(0, 60, COLOR_WHITE, "playerPosY=%.1f", g_playerBody.pos.y);

#endif

}



//----------------------------------------------------------------------
//! @brief �Q�[���̏I������
//!
//! @param[in] �Ȃ�
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void FinalizePlaying(void)
{
	//���[�J���ϐ��̐錾------------------------------------------------



	//���[�J���ϐ��̏�����----------------------------------------------



	//�����̓��e--------------------------------------------------------
	//�摜�f�[�^�̍폜
	InitGraph();

}



//*---------------------------------------------------------------------
//* �ǂݍ��񂾃}�b�v�f�[�^�̕`��
//*
//* @param [mapWidth] �}�b�v�̕�
//* @param [mapHeight] �}�b�v�̍���
//*
//* @return �Ȃ�
//*---------------------------------------------------------------------
void DrawMap(int mapWidth, int mapHeight)
{
	//���[�J���ϐ��̐錾---------------------------------
	int i;	//�z��Ɏg�p
	int j;	//�z��Ɏg�p

	//���[�J���ϐ��̏�����-------------------------------



	//�����̓��e-----------------------------------------
	for (i = 0; i < mapHeight; i++)	//����
	{
		for (j = 0; j < mapWidth; j++)	//��
		{
			DrawExtendGraph(0 + MAPCHIP_SIZE * j, 0 + MAPCHIP_SIZE * i, MAPCHIP_SIZE + MAPCHIP_SIZE * j, MAPCHIP_SIZE + MAPCHIP_SIZE * i, g_mapchip[g_mapData[i][j]], TRUE);
#if DEBUG
			//�f�o�b�O
			DrawBoxAA((float)0 + MAPCHIP_SIZE * j, (float)0 + MAPCHIP_SIZE * i, (float)MAPCHIP_SIZE + MAPCHIP_SIZE * j, (float)MAPCHIP_SIZE + MAPCHIP_SIZE * i, COLOR_BLACK, FALSE);
#endif
		}
	}

}



//----------------------------------------------------------------------
//! @brief �}�E�X��p�����Ə�����
//!
//! @param[in] �Ȃ�
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void MouseRanging(void)
{
	//���[�J���ϐ��̐錾------------------------------------------------



	//���[�J���ϐ��̏�����----------------------------------------------



	//�����̓��e--------------------------------------------------------
	//�}�E�X�̃}�E�X�J�[�\���̈ʒu���擾����
	GetMousePoint(&g_mousePosX, &g_mousePosY);

}



//----------------------------------------------------------------------
//! @brief ��Ԃ̕`�揈��
//!
//! @param[body] �`�悵������Ԃ̎ԑ�
//! @param[bttery] �`�悵������Ԃ̖C��
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void DrawTank(GameObject body, GameObject battery)
{
	//���[�J���ϐ��̐錾------------------------------------------------



	//���[�J���ϐ��̏�����----------------------------------------------



	//�����̓��e--------------------------------------------------------
	//�ԑ̂̕`��
	DrawRotaGraph((int)body.pos.x, (int)body.pos.y, body.image.extRate, body.image.angle, body.image.handle[0], TRUE, FALSE);

	//�C��̕`��
	DrawRotaGraph((int)battery.pos.x, (int)battery.pos.y, battery.image.extRate, battery.image.angle, battery.image.handle[0], TRUE, FALSE);

}



//----------------------------------------------------------------------
//! @brief �Ə��ɍ��킹���摜�̉�]����
//!
//! @param[in] �Ȃ�
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void RotaGraph(void)
{
	//���[�J���ϐ��̐錾------------------------------------------------
	double dx;
	double dy;


	//���[�J���ϐ��̏�����----------------------------------------------
	dx = g_playerBattery.pos.x - g_mousePosX;
	dy = g_playerBattery.pos.y - g_mousePosY;


	//�����̓��e--------------------------------------------------------
	g_playerBattery.image.angle = atan2(dy, dx);

}



//----------------------------------------------------------------------
//! @brief �v���C���[�̈ړ�����
//!
//! @param[in] �Ȃ�
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void MovePlayer(void)
{
	//���[�J���ϐ��̐錾------------------------------------------------
	//���̓f�o�C�X�̏��
	int keyState;

	double dx;
	double dy;


	//���[�J���ϐ��̏�����----------------------------------------------
	keyState = GetJoypadInputState(DX_INPUT_KEY_PAD1);


	//�����̓��e--------------------------------------------------------
	// W�L�[����
	if (keyState & PAD_INPUT_8)
	{
		g_playerBody.vel.y = -PLAYER_SPEED;

	}
	// S�L�[����
	if (keyState & PAD_INPUT_5)
	{
		g_playerBody.vel.y = PLAYER_SPEED;

	}
	// D�L�[����
	if (keyState & PAD_INPUT_6)
	{
		g_playerBody.vel.x = PLAYER_SPEED;

	}
	// A�L�[����
	if (keyState & PAD_INPUT_4)
	{
		g_playerBody.vel.x = -PLAYER_SPEED;
	}

	//����
	g_playerBody.vel.x = g_playerBody.vel.x * FRICTION;
	g_playerBody.vel.y = g_playerBody.vel.y * FRICTION;

	//������
	if ((g_playerBody.vel.x > 1 || g_playerBody.vel.x < -1) && (g_playerBody.vel.y > 1 || g_playerBody.vel.y < -1))
	{
		g_playerBody.vel.x /= ROOT2;
		g_playerBody.vel.y /= ROOT2;
	}

	//�ԑ̂̉�]
	dx = g_playerBody.vel.x;
	dy = g_playerBody.vel.y;
	g_playerBody.image.angle = atan2(dy, dx);
	

	//���W�̍X�V
	g_playerBody.pos.x += g_playerBody.vel.x;
	g_playerBody.pos.y += g_playerBody.vel.y;

	//�C��̈ʒu���ԑ̂̈ʒu�ɍ��킹��
	g_playerBattery.pos = g_playerBody.pos;

}


//----------------------------------------------------------------------
//! @brief �}�b�v��̈ړ������̏���
//!
//! @param[name]���������������I�u�W�F�N�g
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
int LimitMapArea(GameObject *name)
{
	//���[�J���ϐ��̐錾------------------------------------------------
	int mapT;
	int mapB;
	int mapL;
	int mapR;

	//���[�J���ϐ��̏�����----------------------------------------------
	mapT = (int)((name->pos.y - 16) / MAPCHIP_SIZE);
	mapB = (int)((name->pos.y + 16) / MAPCHIP_SIZE);
	mapL = (int)((name->pos.x - 16) / MAPCHIP_SIZE);
	mapR = (int)((name->pos.x + 16) / MAPCHIP_SIZE);

	//�����̓��e--------------------------------------------------------
	//����
	if (g_mapData[mapT][mapL] != 0)
	{
		//���W��߂�
		name->pos.x -= name->vel.x;
		name->pos.y -= name->vel.y;
		return 1;
	}
	//����
	if (g_mapData[mapB][mapL] != 0)
	{
		//���W��߂�
		name->pos.x -= name->vel.x;
		name->pos.y -= name->vel.y;
		return 1;
	}
	//�E��
	if (g_mapData[mapT][mapR] != 0)
	{
		//���W��߂�
		name->pos.x -= name->vel.x;
		name->pos.y -= name->vel.y;
		return 1;
	}
	//�E��
	if (g_mapData[mapB][mapR] != 0)
	{
		//���W��߂�
		name->pos.x -= name->vel.x;
		name->pos.y -= name->vel.y;
		return 1;
	}

	return 0;
}



//----------------------------------------------------------------------
//! @brief �v���C���[�̒e�̔��ˏ���
//!
//! @param[in] �Ȃ�
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void PlayerShootBullet(void)
{
	//���[�J���ϐ��̐錾------------------------------------------------
	int i;	//�z��Ɏg�p


	//���[�J���ϐ��̏�����----------------------------------------------


	//�����̓��e--------------------------------------------------------
	g_lastInputMouseState = ~g_currentInputMouseState;	//�O��̃}�E�X�̓��͏�Ԃ̋L��
	g_currentInputMouseState = GetMouseInput();	//�}�E�X�̓��͏�Ԃ̎擾

	//�}�E�X�̍��L�[���������
	if (((g_lastInputMouseState&g_currentInputMouseState) & MOUSE_INPUT_LEFT) != 0)
	{
		//�e�𒲂ׂ�
		for (i = 0; i < MAX_PLAYER_BULLETS; i++)
		{
			if (g_playerBullets[i].state == 0)
			{
				g_playerBullets[i].pos = g_playerBattery.pos;	//�C��̍��W�ɍ��킹��
				g_playerBullets[i].vel.x = -PLAYER_BULLET_SPEED*(float)cos(g_playerBattery.image.angle);	//���x��^����
				g_playerBullets[i].vel.y = -PLAYER_BULLET_SPEED*(float)sin(g_playerBattery.image.angle);
				g_playerBullets[i].image.angle = g_playerBattery.image.angle;	//�摜�̊p�x�����킹��
				g_playerBullets[i].state = 1;	//�e���g�p���ɂ���
				g_playerBullets[i].hp = 2;	//�e��hp���񕜂�����

				//���ʉ��̍Đ�
				PlaySoundMem(g_effectSound[2], DX_PLAYTYPE_BACK, TRUE);
				break;
			}
		}
	}


}



//----------------------------------------------------------------------
//! @brief ���˂��ꂽ�e�̈ړ�����
//!
//! @param[bullet] �e
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void MoveBullet(GameObject *bullet)
{
	//���[�J���ϐ��̐錾------------------------------------------------



	//���[�J���ϐ��̏�����----------------------------------------------



	//�����̓��e--------------------------------------------------------
	//���W�̍X�V
	bullet->pos.x += bullet->vel.x;
	bullet->pos.y += bullet->vel.y;

}



//----------------------------------------------------------------------
//! @brief �e�̈ړ������̏���
//!
//! @param[bullet] �e
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void LimitBulletAria(GameObject *bullet)
{
	//���[�J���ϐ��̐錾------------------------------------------------
	int mapX;
	int mapY;

	int result;
	int resultT;
	int resultB;
	int resultL;
	int resultR;

	float afterPosX;
	float afterPosY;

	//���[�J���ϐ��̏�����----------------------------------------------




	//�����̓��e--------------------------------------------------------
	//���̈ړ���̍��W�����߂�
	afterPosX = bullet->pos.x + bullet->vel.x;
	afterPosY = bullet->pos.y + bullet->vel.y;

	//���W���}�b�v�ɕϊ�
	mapX = (int)(afterPosX / MAPCHIP_SIZE);	
	mapY = (int)(afterPosY / MAPCHIP_SIZE);

	//�����蔻��
	result= DetectCollisionMapChip(bullet, mapX, mapY);
	resultT = DetectCollisionMapChip(bullet, mapX, mapY - 1);
	resultB = DetectCollisionMapChip(bullet, mapX, mapY + 1);
	resultL = DetectCollisionMapChip(bullet, mapX - 1, mapY);
	resultR = DetectCollisionMapChip(bullet, mapX + 1, mapY);

	if (resultT == 1)
	{
		//�e��hp�����炷
		bullet->hp--;
	}
	else if (resultB == 1)
	{
		//�e��hp�����炷
		bullet->hp--;
	}
	else if (resultL == 1)
	{
		//�e��hp�����炷
		bullet->hp--;
	}
	else if (resultR == 1)
	{
		//�e��hp�����炷
		bullet->hp--;
	}
	else if (result == 1)
	{
		//�e��hp�����炷
		bullet->hp--;
	}

	//�摜�̊p�x��ς���
	bullet->image.angle = atan2(bullet->vel.y, bullet->vel.x);

	//�e�̏���
	if (bullet->hp <= 0)
	{
		//���g�p�ɂ���
		bullet->state = 0;
	}

}



//----------------------------------------------------------------------
//! @brief �����̌�������̏���
//!
//! @param[p1x] ����a�̎n�_��x���W
//! @param[p1y] ����a�̎n�_��y���W
//! @param[p2x] ����a�̏I�_��x���W
//! @param[p2y] ����a�̏I�_��y���W
//! @param[p3x] ����b�̎n�_��x���W
//! @param[p3y] ����b�̎n�_��y���W
//! @param[p4x] ����b�̏I�_��x���W
//! @param[p4y] ����b�̏I�_��y���W
//!
//! @return 1 �������Ă���
//! @return 0 �������Ă��Ȃ�
//----------------------------------------------------------------------
int Intersection(float p1x, float p1y, float p2x, float p2y, float p3x, float p3y, float p4x, float p4y)
{
	//���[�J���ϐ��̐錾------------------------------------------------
	float a;
	float b;
	float c;
	float d;

	//���[�J���ϐ��̏�����----------------------------------------------
	a = (p1x - p2x)*(p3y - p1y) + (p1y - p2y)*(p1x - p3x);
	b = (p1x - p2x)*(p4y - p1y) + (p1y - p2y)*(p1x - p4x);
	c = (p3x - p4x)*(p1y - p3y) + (p3y - p4y)*(p3x - p1x);
	d = (p3x - p4x)*(p2y - p3y) + (p3y - p4y)*(p3x - p2x);

	//�����̓��e--------------------------------------------------------
	if (a*b < 0)
	{
		if (c*d < 0)
		{
			return 1;	//�������Ă���
		}
	}

	return 0;	//�������Ă��Ȃ�

}



//----------------------------------------------------------------------
//! @brief �}�b�v�`�b�v�Ƃ̓����蔻��̏���
//!
//! @param[bullet] �e
//! @param[mapX] �}�b�v�`�b�v��X
//! @param[mapY] �}�b�v�`�b�v��Y
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
int DetectCollisionMapChip(GameObject *bullet, int mapX, int mapY)
{
	//���[�J���ϐ��̐錾------------------------------------------------
	int resultT;
	int resultB;
	int resultL;
	int resultR;

	float afterPosX;
	float afterPosY;

	float left;
	float right;
	float top;
	float bottom;

	//���[�J���ϐ��̏�����----------------------------------------------
	afterPosX = bullet->pos.x + bullet->vel.x;
	afterPosY = bullet->pos.y + bullet->vel.y;



	//�����̓��e--------------------------------------------------------

	//�ړ��\�����ׂ�
	if (g_mapData[mapY][mapX] == 1)
	{
		//�}�b�v�`�b�v�̏㉺���E�̍��W�����߂�
		left = (float)(mapX*MAPCHIP_SIZE);
		right = (float)(mapX*MAPCHIP_SIZE + MAPCHIP_SIZE);
		top = (float)(mapY*MAPCHIP_SIZE);
		bottom = (float)(mapY*MAPCHIP_SIZE + MAPCHIP_SIZE);

		//�����蔻�菈��
		//���
		resultT = Intersection(bullet->pos.x, bullet->pos.y, afterPosX, afterPosY, left, top, right, top);
		//����
		resultB = Intersection(bullet->pos.x, bullet->pos.y, afterPosX, afterPosY, left, bottom, right, bottom);
		//����
		resultL = Intersection(bullet->pos.x, bullet->pos.y, afterPosX, afterPosY, left, top, left, bottom);
		//�E��
		resultR = Intersection(bullet->pos.x, bullet->pos.y, afterPosX, afterPosY, right, top, right, bottom);

		//��ӂ̔�����s��
		if ((resultT == 1) && (bullet->pos.y < top))
		{
			//y�����̌�����ς���
			bullet->vel.y *= -1;

			//���ʉ��̍Đ�
			PlaySoundMem(g_effectSound[3], DX_PLAYTYPE_BACK, TRUE);

			//�������I����
			return 1;
		}

		//���ӂ̔�����s��
		if ((resultB == 1) && (bullet->pos.y > bottom))
		{
			//y�����̌�����ς���
			bullet->vel.y *= -1;

			//���ʉ��̍Đ�
			PlaySoundMem(g_effectSound[3], DX_PLAYTYPE_BACK, TRUE);

			//�������I����
			return 1;
		}

		//���̔�����s��
		if ((resultL == 1) && (bullet->pos.x < left))
		{
			//x�����̌�����ς���
			bullet->vel.x *= -1;

			//���ʉ��̍Đ�
			PlaySoundMem(g_effectSound[3], DX_PLAYTYPE_BACK, TRUE);

			//�������I����
			return 1;
		}

		//�E�̔�����s��
		if ((resultR == 1) && (bullet->pos.x > right))
		{
			//x�����̌�����ς���
			bullet->vel.x *= -1;

			//���ʉ��̍Đ�
			PlaySoundMem(g_effectSound[3], DX_PLAYTYPE_BACK, TRUE);

			//�������I����
			return 1;
		}
	}

	//�ǂ��ɂ��������Ă��Ȃ�
	return 0;

}



//----------------------------------------------------------------------
//! @brief �G�̈ړ�����
//!
//! @param[in] �Ȃ�
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void MoveEnemy(void)
{
	//���[�J���ϐ��̐錾------------------------------------------------
	int i;	//�z��Ɏg�p
	int j;	//�z��Ɏg�p

	double dx;
	double dy;

	//�Փ˔���̌���
	int resultM;	//�}�b�v
	int resultP;	//�v���C���[
	int resultE;	//�G

	//���[�J���ϐ��̏�����----------------------------------------------
	resultM = 0;
	resultP = 0;
	resultE = 0;


	//�����̓��e--------------------------------------------------------
	//�G�𒲂ׂ�
	for (i = 0; i < MAX_ENEMIES; i++)
	{
		if (g_enemyBody[i].state == 1)
		{
			//���x�̏�����
			g_enemyBody[i].vel.x = 0.0f;
			g_enemyBody[i].vel.y = 0.0f;

			switch (g_enemyBody[i].moveType)
			{
			case 1:
				//��
				MoveUp(&g_enemyBody[i]);
				break;
			case 2:
				//��
				MoveDown(&g_enemyBody[i]);
				break;
			case 3:
				//��
				MoveLeft(&g_enemyBody[i]);
				break;
			case 4:
				//�E
				MoveRight(&g_enemyBody[i]);
				break;
			case 5:
				//����
				MoveUp(&g_enemyBody[i]);
				MoveLeft(&g_enemyBody[i]);
				break;
			case 6:
				//�E��
				MoveUp(&g_enemyBody[i]);
				MoveRight(&g_enemyBody[i]);
				break;
			case 7:
				//����
				MoveDown(&g_enemyBody[i]);
				MoveLeft(&g_enemyBody[i]);
				break;
			case 8:
				//�E��
				MoveDown(&g_enemyBody[i]);
				MoveRight(&g_enemyBody[i]);
				break;
			default:
				break;
			}

			//����
			g_enemyBody[i].vel.x = g_enemyBody[i].vel.x * FRICTION;
			g_enemyBody[i].vel.y = g_enemyBody[i].vel.y * FRICTION;

			//������
			if ((g_enemyBody[i].vel.x > 1 || g_enemyBody[i].vel.x < -1) && (g_enemyBody[i].vel.y > 1 || g_enemyBody[i].vel.y < -1))
			{
				g_enemyBody[i].vel.x /= ROOT2;
				g_enemyBody[i].vel.y /= ROOT2;
			}

			//�ԑ̂̉�]
			dx = g_enemyBody[i].vel.x;
			dy = g_enemyBody[i].vel.y;
			g_enemyBody[i].image.angle = atan2(dy, dx);


			//���W�̍X�V
			g_enemyBody[i].pos.x += g_enemyBody[i].vel.x;
			g_enemyBody[i].pos.y += g_enemyBody[i].vel.y;

			//�C��̈ʒu���ԑ̂̈ʒu�ɍ��킹��
			g_enemyBattery[i].pos = g_enemyBody[i].pos;

			//�G�̈ړ�����
			resultM = LimitMapArea(&g_enemyBody[i]);
			resultP = DetectCollisionBullet(&g_enemyBody[i], &g_playerBody);
			for (j = 0; j < MAX_ENEMIES; j++)
			{
				if (g_enemyBody[j].state == 1)
				{
					resultE = DetectCollisionBullet(&g_enemyBody[i], &g_enemyBody[j]);
				}
				if ((resultE == 1) && (j != i))
				{
					break;
				}
			}
			 
			//�ړ����Ԃ̃J�E���g
			g_enemyBody[i].moveCounter--;

			if ((g_enemyBody[i].moveCounter <= 0) || (resultM == 1) || (resultP == 1) || (resultE == 1))
			{
				g_enemyBody[i].moveType = GetRand(8 - 1) + 1;
				g_enemyBody[i].moveCounter = GetRand(180 - 1) + 1;
			}
		}
	}
}



//----------------------------------------------------------------------
//! @brief ��ړ��̏���
//!
//! @param[name] �G
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void MoveUp(GameObject *name)
{
	//���x�̑��
	switch (name->type)
	{
	case 1:
		name->vel.y = -ENEMY_SPEED_ZERO;
		break;
	case 2:
		name->vel.y = -ENEMY_SPEED_LATE;
		break;
	case 3:
		name->vel.y = -ENEMY_SPEED_NORMAL;
		break;
	case 4:
		name->vel.y = -ENEMY_SPEED_LATE;
		break;
	case 5:
		name->vel.y = -ENEMY_SPEED_ZERO;
		break;
	case 6:
		name->vel.y = -ENEMY_SPEED_FAST;
		break;
	case 7:
		name->vel.y = -ENEMY_SPEED_FAST;
		break;
	default:

		break;
	}
	
}



//----------------------------------------------------------------------
//! @brief ���ړ��̏���
//!
//! @param[name] �G
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void MoveDown(GameObject *name)
{
	//���x�̑��
	switch (name->type)
	{
	case 1:
		name->vel.y = ENEMY_SPEED_ZERO;
		break;
	case 2:
		name->vel.y = ENEMY_SPEED_LATE;
		break;
	case 3:
		name->vel.y = ENEMY_SPEED_NORMAL;
		break;
	case 4:
		name->vel.y = ENEMY_SPEED_LATE;
		break;
	case 5:
		name->vel.y = ENEMY_SPEED_ZERO;
		break;
	case 6:
		name->vel.y = ENEMY_SPEED_FAST;
		break;
	case 7:
		name->vel.y = ENEMY_SPEED_FAST;
		break;
	default:

		break;
	}

}



//----------------------------------------------------------------------
//! @brief ���ړ��̏���
//!
//! @param[name] �G
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void MoveLeft(GameObject *name)
{
	//���x�̑��
	switch (name->type)
	{
	case 1:
		name->vel.x = -ENEMY_SPEED_ZERO;
		break;
	case 2:
		name->vel.x = -ENEMY_SPEED_LATE;
		break;
	case 3:
		name->vel.x = -ENEMY_SPEED_NORMAL;
		break;
	case 4:
		name->vel.x = -ENEMY_SPEED_LATE;
		break;
	case 5:
		name->vel.x = -ENEMY_SPEED_ZERO;
		break;
	case 6:
		name->vel.x = -ENEMY_SPEED_FAST;
		break;
	case 7:
		name->vel.x = -ENEMY_SPEED_FAST;
		break;
	default:

		break;
	}

}



//----------------------------------------------------------------------
//! @brief �E�ړ��̏���
//!
//! @param[name] �G
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void MoveRight(GameObject *name)
{
	//���x�̑��
	switch (name->type)
	{
	case 1:
		name->vel.x = ENEMY_SPEED_ZERO;
		break;
	case 2:
		name->vel.x = ENEMY_SPEED_LATE;
		break;
	case 3:
		name->vel.x = ENEMY_SPEED_NORMAL;
		break;
	case 4:
		name->vel.x = ENEMY_SPEED_LATE;
		break;
	case 5:
		name->vel.x = ENEMY_SPEED_ZERO;
		break;
	case 6:
		name->vel.x = ENEMY_SPEED_FAST;
		break;
	case 7:
		name->vel.x = ENEMY_SPEED_FAST;
		break;
	default:

		break;
	}

}



//----------------------------------------------------------------------
//! @brief �G�̒e�̔��ˏ���
//!
//! @param[in] �Ȃ�
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void EnemyShootBullet(void)
{
	//���[�J���ϐ��̐錾------------------------------------------------
	int i;	//�z��Ɏg�p
	int j;	//�z��Ɏg�p
	int k;	//�z��Ɏg�p
	int l;	//�z��Ɏg�p

	int result;	//�\�����ʂɎg�p
	int resultCount;

	float dx;
	float dy;

	
	//���[�J���ϐ��̏�����----------------------------------------------
	result = 1;
	resultCount = 0;

	//�����̓��e--------------------------------------------------------

	for (i = 0; i < MAX_ENEMIES; i++)
	{
		if (g_enemyBattery[i].state == 1)
		{
			//�C��̌��������킹��
			dx = g_enemyBattery[i].pos.x - g_playerBody.pos.x;
			dy = g_enemyBattery[i].pos.y - g_playerBody.pos.y;
			
			g_enemyBattery[i].image.angle = atan2(dy, dx);

			if (g_enemyBattery[i].bulletInterval <= 0)
			{
				for (j = 0; j < MAP_HEIGHT; j++)
				{
					for (k = 0; k < MAP_WIDTH; k++)
					{
						//�ː���ɏ�Q�������邩���ׂ�
						result = ForecastTrajectory(&g_enemyBattery[i], &g_playerBody, k, j);
						if (result == 0)
						{
							resultCount++;
						}
					}
				}
				if (resultCount == 300)
				{
					for (l = 0; l < MAX_ENEMY_BULLETS; l++)
					{
						if (g_enemyBullets[l].state == 0)
						{
							ShootWayBullet(&g_enemyBattery[i], &g_enemyBullets[l], &g_playerBody);
							g_enemyBullets[l].state = 1;

							switch (g_enemyBattery[i].type)
							{
							case 1:
								g_enemyBullets[l].hp = 2;
								g_enemyBattery[i].bulletInterval = 180;
								break;
							case 2:
								g_enemyBullets[l].hp = 2;
								g_enemyBattery[i].bulletInterval = 180;
								break;
							case 3:
								g_enemyBullets[l].hp = 1;
								g_enemyBattery[i].bulletInterval = 120;
								break;
							case 4:
								g_enemyBullets[l].hp = 2;
								g_enemyBattery[i].bulletInterval = 100;
								break;
							case 5:
								g_enemyBullets[l].hp = 3;
								g_enemyBattery[i].bulletInterval = 120;
								break;
							case 6:
								g_enemyBullets[l].hp = 2;
								g_enemyBattery[i].bulletInterval = 60;
								break;
							case 7:
								g_enemyBullets[l].hp = 3;
								g_enemyBattery[i].bulletInterval = 100;
								break;
							default:

								break;
							}
							
							//���ʉ��̍Đ�
							PlaySoundMem(g_effectSound[2], DX_PLAYTYPE_BACK, TRUE);

							break;
						}
					}
				}
			}

			//���e���˂܂ł̃J�E���g
			g_enemyBattery[i].bulletInterval--;
		}
	}



}



//----------------------------------------------------------------------
//! @brief �����e�̔��ˏ���
//!
//! @param[shooter] �ˎ�
//! @param[bullet] �������e
//! @param[target] �ڕW
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void ShootWayBullet(GameObject *shooter, GameObject *bullet, GameObject *target)
{
	//���[�J���ϐ��̐錾////////////////////////////////////////////////
	double dx;
	double dy;
	double angle;

	//���[�J���ϐ��̏�����//////////////////////////////////////////////
	dx = (double)(target->pos.x - shooter->pos.x);	//�ڕW��x���W - �ˎ��x���W
	dy = (double)(target->pos.y - shooter->pos.y);	//�ڕW��y���W - �ˎ��y���W

	angle = atan2(dy, dx);	//�ڕW�Ǝˎ�̊p�x�̌v�Z

	//�����̓��e////////////////////////////////////////////////////////
	//���ˈʒu�̏C��
	bullet->pos.x = shooter->pos.x;
	bullet->pos.y = shooter->pos.y;
	switch (shooter->type)
	{
	case 1:
		bullet->vel.x = (float)(ENEMY_BULLET_SPEED_NORMAL*cos(angle));
		bullet->vel.y = (float)(ENEMY_BULLET_SPEED_NORMAL*sin(angle));
		break;
	case 2:
		bullet->vel.x = (float)(ENEMY_BULLET_SPEED_NORMAL*cos(angle));
		bullet->vel.y = (float)(ENEMY_BULLET_SPEED_NORMAL*sin(angle));
		break;
	case 3:
		bullet->vel.x = (float)(ENEMY_BULLET_SPEED_FAST*cos(angle));
		bullet->vel.y = (float)(ENEMY_BULLET_SPEED_FAST*sin(angle));
		break;
	case 4:
		bullet->vel.x = (float)(ENEMY_BULLET_SPEED_NORMAL*cos(angle));
		bullet->vel.y = (float)(ENEMY_BULLET_SPEED_NORMAL*sin(angle));
		break;
	case 5:
		bullet->vel.x = (float)(ENEMY_BULLET_SPEED_FAST*cos(angle));
		bullet->vel.y = (float)(ENEMY_BULLET_SPEED_FAST*sin(angle));
		break;
	case 6:
		bullet->vel.x = (float)(ENEMY_BULLET_SPEED_NORMAL*cos(angle));
		bullet->vel.y = (float)(ENEMY_BULLET_SPEED_NORMAL*sin(angle));
		break;
	case 7:
		bullet->vel.x = (float)(ENEMY_BULLET_SPEED_FAST*cos(angle));
		bullet->vel.y = (float)(ENEMY_BULLET_SPEED_FAST*sin(angle));
		break;
	default:

		break;
	}
	
}



//----------------------------------------------------------------------
//! @brief �e�Ƃ̓����蔻�菈��
//!
//! @param[main] �����蔻����s���I�u�W�F�N�g
//! @param[partner] �����蔻����s���I�u�W�F�N�g
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
int DetectCollisionBullet(GameObject *main, GameObject *partner)
{
	//���[�J���ϐ��̐錾------------------------------------------------
	int resultT;
	int resultB;
	int resultL;
	int resultR;

	float afterPosX;
	float afterPosY;

	float left;
	float right;
	float top;
	float bottom;

	//���[�J���ϐ��̏�����----------------------------------------------
	afterPosX = partner->pos.x + partner->vel.x;
	afterPosY = partner->pos.y + partner->vel.y;



	//�����̓��e--------------------------------------------------------

	//�}�b�v�`�b�v�̏㉺���E�̍��W�����߂�
	left = (float)(main->pos.x - 16);
	right = (float)(main->pos.x + 16);
	top = (float)(main->pos.y - 16);
	bottom = (float)(main->pos.y + 16);

	//�����蔻�菈��
	//���
	resultT = Intersection(partner->pos.x,partner->pos.y, afterPosX, afterPosY, left, top, right, top);
	//����
	resultB = Intersection(partner->pos.x, partner->pos.y, afterPosX, afterPosY, left, bottom, right, bottom);
	//����
	resultL = Intersection(partner->pos.x,partner->pos.y, afterPosX, afterPosY, left, top, left, bottom);
	//�E��
	resultR = Intersection(partner->pos.x,partner->pos.y, afterPosX, afterPosY, right, top, right, bottom);
	
	//��ӂ̔�����s��
	if ((resultT == 1) && (partner->pos.y < top))
	{
		//�������I����
		return 1;
	}
	
	//���ӂ̔�����s��
	if ((resultB == 1) && (partner->pos.y > bottom))
	{
		//�������I����
		return 1;
	}

	//���̔�����s��
	if ((resultL == 1) && (partner->pos.x < left))
	{
		//�������I����
		return 1;
	}

	//�E�̔�����s��
	if ((resultR == 1) && (partner->pos.x > right))
	{
		//�������I����
		return 1;
	}
	
	//�ǂ��ɂ��������Ă��Ȃ�
	return 0;

}



//----------------------------------------------------------------------
//! @brief �e���\���̏���
//!
//! @param[shooter] �ˎ�
//! @param[target] �^�[�Q�b�g
//! @param[mapX] �}�b�v�`�b�v��X
//! @param[mapY] �}�b�v�`�b�v��Y
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
int ForecastTrajectory(GameObject *shooter, GameObject *target, int mapX, int mapY)
{
	//���[�J���ϐ��̐錾------------------------------------------------
	int resultT;
	int resultB;
	int resultL;
	int resultR;

	float left;
	float right;
	float top;
	float bottom;

	//���[�J���ϐ��̏�����----------------------------------------------

	


	//�����̓��e--------------------------------------------------------

	if (g_mapData[mapY][mapX] == 1)
	{
		//�}�b�v�`�b�v�̏㉺���E�̍��W�����߂�
		left = (float)(mapX*MAPCHIP_SIZE);
		right = (float)(mapX*MAPCHIP_SIZE + MAPCHIP_SIZE);
		top = (float)(mapY*MAPCHIP_SIZE);
		bottom = (float)(mapY*MAPCHIP_SIZE + MAPCHIP_SIZE);

		//�����蔻�菈��
		//���
		resultT = Intersection(shooter->pos.x, shooter->pos.y, target->pos.x, target->pos.y, left, top, right, top);
		//����
		resultB = Intersection(shooter->pos.x, shooter->pos.y, target->pos.x, target->pos.y, left, bottom, right, bottom);
		//����
		resultL = Intersection(shooter->pos.x, shooter->pos.y, target->pos.x, target->pos.y, left, top, left, bottom);
		//�E��
		resultR = Intersection(shooter->pos.x, shooter->pos.y, target->pos.x, target->pos.y, right, top, right, bottom);

		//��ӂ̔�����s��
		if (resultT == 1)
		{
			//�������I����
			return 1;
		}

		//���ӂ̔�����s��
		if (resultB == 1)
		{
			//�������I����
			return 1;
		}

		//���̔�����s��
		if (resultL == 1)
		{
			//�������I����
			return 1;
		}

		//�E�̔�����s��
		if (resultR == 1)
		{
			//�������I����
			return 1;
		}
	}

	//�Ԃɉ����Ȃ�
	return 0;

}



//----------------------------------------------------------------------
//! @brief �����̃A�j���[�V��������
//!
//! @param[name] �I�u�W�F�N�g��
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void EffectExplosion(Effect *name)
{
	//���[�J���ϐ��̐錾//----------------------------------------------
	


	//���[�J���ϐ��̏�����//--------------------------------------------



	//�����̓��e//------------------------------------------------------
	//�`�掞�Ԃ��͂���
	name->anime.elapsedTime++;
	//���݂̕`�掞�Ԃ��I�����
	if (name->anime.elapsedTime > name->anime.frameDuration)
	{
		//���̃t���[���Ɉړ�
		name->anime.frameNum++;
		//�`�掞�Ԃ̃��Z�b�g
		name->anime.elapsedTime = 0;

		//�S�Ẳ摜��`�悷��
		if (name->anime.frameNum == name->anime.numFrame)
		{
			//���g�p�ɂ���
			name->state = 0;
		}
	}
	
}