//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   ScenePlaying.cpp
//!
//! @brief  ScenePlayingのソースファイル
//!
//! @date   
//!
//! @author 深草直斗
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// ヘッダファイルの読み込み ================================================
#include <math.h>
#include "ScenePlaying.h"
#include "GameMain.h"


//データ型の定義============================================================
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


//定数の定義================================================================
//デバッグ
#define DEBUG 0

//円周率
#define PI    3.1415926535897932384626433832795f

//√2
#define ROOT2 (1.41421356f)

//画像の倍率
#define IMAGE_EXTRATE (0.07f)

//プレイヤーの移動速度
#define PLAYER_SPEED (3.0f)

//減速
#define FRICTION (0.5f)

//プレイヤーの弾の速度
#define PLAYER_BULLET_SPEED (4.0f)

//プレイヤーの弾の最大数
#define MAX_PLAYER_BULLETS (5)

//敵の最大数
#define MAX_ENEMIES (10)

//敵の移動速度
//type1,type5
#define ENEMY_SPEED_ZERO (0.0f)
//type2,type4
#define ENEMY_SPEED_LATE (2.0f)
//type3
#define ENEMY_SPEED_NORMAL (3.0f)
//type6,type7
#define ENEMY_SPEED_FAST (4.0f)

//敵の弾の速度
//type1,type2,type4,type6
#define ENEMY_BULLET_SPEED_NORMAL (4.0f)
//type3,type5,type7
#define ENEMY_BULLET_SPEED_FAST (5.0f)

//敵の弾の最大数
#define MAX_ENEMY_BULLETS (100)

//マップの最大数
#define MAX_MAP (100)

//マップの幅
#define MAP_WIDTH (20)

//マップの高さ
#define MAP_HEIGHT (15)

//マップチップサイズ
#define MAPCHIP_SIZE (32)

//エフェクトの最大数
#define MAX_EFFECT (100)


//関数のプロトタイプ宣言====================================================

//読み込んだマップデータの描画
void DrawMap(int mapWidth, int mapHeight);

//マウスを用いた照準
void MouseRanging(void);

//戦車の描画
void DrawTank(GameObject body, GameObject battery);

//照準に合わせた画像の回転
void  RotaGraph(void);

//プレイヤーの移動
void MovePlayer(void);

//マップ上の移動制限
int LimitMapArea(GameObject *name);

//プレイヤーの弾の発射
void PlayerShootBullet(void);

//発射された弾の移動
void MoveBullet(GameObject *bullet);

//弾の移動制限
void LimitBulletAria(GameObject *bullet);

//線分の交差判定
int Intersection(float p1x, float p1y, float p2x, float p2y, float p3x, float p3y, float p4x, float p4y);

//マップチップとの当たり判定
int DetectCollisionMapChip(GameObject *bullet, int mapX, int mapY);

//敵の移動
void MoveEnemy(void);

//移動方法
//上
void MoveUp(GameObject *name);
//下
void MoveDown(GameObject *name);
//左
void MoveLeft(GameObject *name);
//右
void MoveRight(GameObject *name);

//敵の弾の発射
void EnemyShootBullet(void);

//弾道予測
int ForecastTrajectory(GameObject *shooter, GameObject *target, int mapX, int mapY);

//弾の発射方法
//方向弾
void ShootWayBullet(GameObject *shooter, GameObject *bullet, GameObject *target);

//弾との当たり判定
int DetectCollisionBullet(GameObject *main, GameObject *partner);

//爆発アニメーション
void EffectExplosion(Effect *name);

// グローバル変数の定義 ====================================================
HGRP g_image[100];

HGRP g_tankImage[16];

//入力デバイスの状態
int g_currentInputMouseState;	//現在のマウスの入力状態
int g_lastInputMouseState;	//前回のマウスの入力状態

int g_mousePosX;
int g_mousePosY;

//プレイヤー
//砲台
GameObject g_playerBattery;
//車体
GameObject g_playerBody;
//弾
GameObject g_playerBullets[MAX_PLAYER_BULLETS];

//エネミー
//砲台
GameObject g_enemyBattery[MAX_ENEMIES];
//車体
GameObject g_enemyBody[MAX_ENEMIES];
//弾
GameObject g_enemyBullets[MAX_ENEMY_BULLETS];

//敵の数
int g_enemyNumber;

//スコア
extern Score g_score;

//マップデータ
extern int g_mapData[MAP_HEIGHT][MAP_WIDTH];
//位置データ
extern int g_posData[MAP_HEIGHT][MAP_WIDTH];
//マップの画像
int g_mapchip[10];
//ステージ
extern int g_stageNumber;

//効果音
int g_effectSound[10];

//エフェクトアニメーション
//戦車の爆発
Effect g_explosionTank[MAX_EFFECT];
//弾の爆発
Effect g_explosionBullet[MAX_EFFECT];


// 関数の定義 ==============================================================
//----------------------------------------------------------------------
//! @brief ゲームの初期化処理
//!
//! @param[in] なし
//!
//! @return なし
//----------------------------------------------------------------------
void InitializePlaying(void)
{
	//ローカル変数の宣言------------------------------------------------
	int i;	//配列に使用
	int j;	//配列に使用
	int k;	//配列に使用
	

	//ローカル変数の初期化----------------------------------------------




	//処理の内容--------------------------------------------------------
	g_image[0] = LoadGraph("Resources/Images/照準.png");
	g_image[5] = LoadGraph("Resources/Images/弾.png");

	LoadDivGraph("Resources/Images/tank.png", 16, 4, 4, 654, 654, g_tankImage);

	SetMouseDispFlag(FALSE);
	g_mousePosX = 0;
	g_mousePosY = 0;

	//マップ画像の読み込み
	g_mapchip[0] = { LoadGraph("Resources/Images/マップチップ2.png") };
	g_mapchip[1] = { LoadGraph("Resources/Images/マップチップ1.png") };
	g_mapchip[2] = { LoadGraph("Resources/Images/マップチップ3.png") };

	//効果音
	//弾の爆発音
	g_effectSound[0] = LoadSoundMem("Resources/Sounds/se_maoudamashii_battle18.mp3");
	//戦車の爆発音
	g_effectSound[1] = LoadSoundMem("Resources/Sounds/se_maoudamashii_explosion03.mp3");
	//弾の発射音
	g_effectSound[2] = LoadSoundMem("Resources/Sounds/se_maoudamashii_instruments_drum2_tom2.mp3");
	//弾が壁に反射する音
	g_effectSound[3] = LoadSoundMem("Resources/Sounds/se_maoudamashii_se_finger01.mp3");
	

	//敵のstateの初期化
	for (i = 0; i < MAX_ENEMIES; i++)
	{
		g_enemyBattery[i].state = 0;
		g_enemyBody[i].state = 0;
	}

	//敵の数の初期化
	g_enemyNumber = 0;

	//各戦車を初期位置に配置
	for (i = 0; i < MAP_HEIGHT; i++)
	{
		for (j = 0; j < MAP_WIDTH; j++)
		{
			switch (g_posData[i][j])
			{
			case 1:
				//プレイヤー
				//砲台
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

				//車体
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

			//敵戦車
			case 2:
				//type1
				for (k = 0; k < MAX_ENEMIES; k++)
				{
					if ((g_enemyBattery[k].state == 0) && (g_enemyBody[k].state == 0))
					{
						//砲台
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

						//車体
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

						//出現した敵の数を数える
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
						//砲台
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

						//車体
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

						//出現した敵の数を数える
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
						//砲台
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

						//車体
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

						//出現した敵の数を数える
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
						//砲台
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

						//車体
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

						//出現した敵の数を数える
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
						//砲台
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

						//車体
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

						//出現した敵の数を数える
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
						//砲台
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

						//車体
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

						//出現した敵の数を数える
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
						//砲台
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

						//車体
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

						//出現した敵の数を数える
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

	//プレイヤーの弾
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

	//エネミーの弾
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

	//エフェクト
	//戦車の爆発
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
	
	//弾の爆発
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
//! @brief ゲームの更新処理
//!
//! @param[in] なし
//!
//! @return なし
//----------------------------------------------------------------------
void UpdatePlaying(void)
{
	//ローカル変数の宣言------------------------------------------------
	int i;	//配列に使用
	int j;	//配列に使用
	int k;	//配列に使用

	int result;	//当たり判定に使用

	//ローカル変数の初期化----------------------------------------------



	//処理の内容--------------------------------------------------------

	//マウスを用いた照準
	MouseRanging();


	//照準に合わせた画像の回転
	RotaGraph();

	//プレイヤーの移動
	MovePlayer();

	//マップ上の移動制限
	LimitMapArea(&g_playerBody);

	//プレイヤーの弾の発射
	PlayerShootBullet();

	//発射された弾の移動
	for (i = 0; i < MAX_PLAYER_BULLETS; i++)
	{
		if (g_playerBullets[i].state == 1)
		{
			MoveBullet(&g_playerBullets[i]);
			//弾の移動制限
			LimitBulletAria(&g_playerBullets[i]);
		}
	}

	//敵の移動
	MoveEnemy();

	//敵の弾の発射
	EnemyShootBullet();

	//弾の移動
	for (i = 0; i < MAX_ENEMY_BULLETS; i++)
	{
		if (g_enemyBullets[i].state == 1)
		{
			MoveBullet(&g_enemyBullets[i]);
			//弾の移動制限
			LimitBulletAria(&g_enemyBullets[i]);
		}
	}

	//当たり判定
	//プレイヤーとプレイヤーの弾
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

					//爆発エフェクト
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

					//効果音の再生
					PlaySoundMem(g_effectSound[1], DX_PLAYTYPE_BACK, TRUE);

				}
			}
		}
	}

	//プレイヤーの弾と敵
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

						//爆発エフェクト
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

						//スコアのカウント
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

						//出現している敵の数を減らす
						g_enemyNumber--;

						//効果音の再生
						PlaySoundMem(g_effectSound[1], DX_PLAYTYPE_BACK, TRUE);
					}
				}
			}
		}
	}

	//敵の弾と敵
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

						//爆発エフェクト
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

						//出現している敵の数を減らす
						g_enemyNumber--;

						//効果音の再生
						PlaySoundMem(g_effectSound[1], DX_PLAYTYPE_BACK, TRUE);
					}
				}
			}
		}
	}

	//プレイヤーと敵の弾
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

					//爆発エフェクト
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

	//プレイヤーの弾と弾
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

						//爆発エフェクト
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

						//効果音の再生
						PlaySoundMem(g_effectSound[0], DX_PLAYTYPE_BACK, TRUE);
					}
				}
			}
		}
	}


	//プレイヤーの弾と敵の弾
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

						//爆発エフェクト
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

						//効果音の再生
						PlaySoundMem(g_effectSound[0], DX_PLAYTYPE_BACK, TRUE);
					}
				}
			}
		}
	}

	//敵の弾と弾
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

						//爆発エフェクト
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

						//効果音の再生
						PlaySoundMem(g_effectSound[0], DX_PLAYTYPE_BACK, TRUE);
					}
				}
			}
		}
	}



	//爆発エフェクトの処理
	//戦車
	for (i = 0; i < MAX_EFFECT; i++)
	{
		if (g_explosionTank[i].state == 1)
		{
			EffectExplosion(&g_explosionTank[i]);
		}
	}

	//弾
	for (i = 0; i < MAX_EFFECT; i++)
	{
		if (g_explosionBullet[i].state == 1)
		{
			EffectExplosion(&g_explosionBullet[i]);
		}
	}



	//プレイの終了
	//プレイヤーがやられた場合
	if (g_playerBody.state == 0)
	{
		//シーンの切り替え
		ChangeScene(SCENE_RESULT);	//リザルトシーンへ
	}

	//敵を全滅させた場合
	if (g_enemyNumber == 0)
	{
		//最終ステージだった場合
		if (g_stageNumber == 10)
		{
			//シーンの切り替え
			ChangeScene(SCENE_RESULT);	//リザルトシーンへ
		}
		else
		{
			//次のステージへ
			g_stageNumber++;
			ChangeScene(SCENE_READY);	//プレイ前準備シーンへ
		}
	}

}



//----------------------------------------------------------------------
//! @brief ゲームの描画処理
//!
//! @param[in] なし
//!
//! @return なし
//----------------------------------------------------------------------
void RenderPlaying(void)
{
	//ローカル変数の宣言------------------------------------------------
	int i;	//配列に使用

	//ローカル変数の初期化----------------------------------------------



	//処理の内容--------------------------------------------------------
	//背景
	//マップの描画
	DrawMap(MAP_WIDTH, MAP_HEIGHT);

	//プレイヤーの描画//
	if ((g_playerBattery.state == 1) && (g_playerBody.state == 1))
	{
		DrawTank(g_playerBody, g_playerBattery);
	}

	//弾の描画
	for (i = 0; i < MAX_PLAYER_BULLETS; i++)
	{
		if (g_playerBullets[i].state == 1)
		{
			DrawRotaGraph((int)g_playerBullets[i].pos.x,(int)g_playerBullets[i].pos.y, g_playerBullets[i].image.extRate, g_playerBullets[i].image.angle, g_playerBullets[i].image.handle[0], TRUE, FALSE);
		}
	}


	//敵の描画//
	for (i = 0; i < MAX_ENEMIES; i++)
	{
		if ((g_enemyBattery[i].state == 1) && (g_enemyBody[i].state == 1))
		{
			DrawTank(g_enemyBody[i], g_enemyBattery[i]);
		}
	}
	//弾の描画
	for (i = 0; i < MAX_ENEMY_BULLETS; i++)
	{
		if (g_enemyBullets[i].state == 1)
		{
			DrawRotaGraph((int)g_enemyBullets[i].pos.x, (int)g_enemyBullets[i].pos.y, g_enemyBullets[i].image.extRate, g_enemyBullets[i].image.angle, g_enemyBullets[i].image.handle[0], TRUE, FALSE);
		}
	}

	//爆発の描画
	//戦車
	for (i = 0; i < MAX_EFFECT; i++)
	{
		if (g_explosionTank[i].state == 1)
		{
			DrawRotaGraph((int)g_explosionTank[i].pos.x, (int)g_explosionTank[i].pos.y, g_explosionTank[i].image.extRate, g_explosionTank[i].image.angle, g_explosionTank[i].image.handle[0], TRUE, FALSE);
		}
	}

	//弾
	for (i = 0; i < MAX_EFFECT; i++)
	{
		if (g_explosionBullet[i].state == 1)
		{
			DrawRotaGraph((int)g_explosionBullet[i].pos.x, (int)g_explosionBullet[i].pos.y, g_explosionBullet[i].image.extRate, g_explosionBullet[i].image.angle, g_explosionBullet[i].image.handle[0], TRUE, FALSE);
		}
	}

	//爆発エフェクト
	//戦車
	for (i = 0; i < MAX_EFFECT; i++)
	{
		if (g_explosionTank[i].state == 1)
		{
			DrawRotaGraph((int)g_explosionTank[i].pos.x, (int)g_explosionTank[i].pos.y, g_explosionTank[i].image.extRate, g_explosionTank[i].image.angle, g_explosionTank[i].image.handle[g_explosionTank[i].anime.frameNum], TRUE, FALSE);
		}
	}

	//弾
	for (i = 0; i < MAX_EFFECT; i++)
	{
		if (g_explosionBullet[i].state == 1)
		{
			DrawRotaGraph((int)g_explosionBullet[i].pos.x, (int)g_explosionBullet[i].pos.y, g_explosionBullet[i].image.extRate, g_explosionBullet[i].image.angle, g_explosionBullet[i].image.handle[g_explosionBullet[i].anime.frameNum], TRUE, FALSE);
		}
	}

	//照準の描画
	DrawRotaGraph(g_mousePosX, g_mousePosY, IMAGE_EXTRATE, PI / 2, g_image[0], TRUE);

	//デバッグ
#if DEBUG
	DrawFormatString(0, 0, COLOR_WHITE, "カーソルX=%d", g_mousePosX);
	DrawFormatString(0, 20, COLOR_WHITE, "カーソルY=%d", g_mousePosY);
	DrawFormatString(0, 40, COLOR_WHITE, "playerPosX=%.1f", g_playerBody.pos.x);
	DrawFormatString(0, 60, COLOR_WHITE, "playerPosY=%.1f", g_playerBody.pos.y);

#endif

}



//----------------------------------------------------------------------
//! @brief ゲームの終了処理
//!
//! @param[in] なし
//!
//! @return なし
//----------------------------------------------------------------------
void FinalizePlaying(void)
{
	//ローカル変数の宣言------------------------------------------------



	//ローカル変数の初期化----------------------------------------------



	//処理の内容--------------------------------------------------------
	//画像データの削除
	InitGraph();

}



//*---------------------------------------------------------------------
//* 読み込んだマップデータの描画
//*
//* @param [mapWidth] マップの幅
//* @param [mapHeight] マップの高さ
//*
//* @return なし
//*---------------------------------------------------------------------
void DrawMap(int mapWidth, int mapHeight)
{
	//ローカル変数の宣言---------------------------------
	int i;	//配列に使用
	int j;	//配列に使用

	//ローカル変数の初期化-------------------------------



	//処理の内容-----------------------------------------
	for (i = 0; i < mapHeight; i++)	//高さ
	{
		for (j = 0; j < mapWidth; j++)	//幅
		{
			DrawExtendGraph(0 + MAPCHIP_SIZE * j, 0 + MAPCHIP_SIZE * i, MAPCHIP_SIZE + MAPCHIP_SIZE * j, MAPCHIP_SIZE + MAPCHIP_SIZE * i, g_mapchip[g_mapData[i][j]], TRUE);
#if DEBUG
			//デバッグ
			DrawBoxAA((float)0 + MAPCHIP_SIZE * j, (float)0 + MAPCHIP_SIZE * i, (float)MAPCHIP_SIZE + MAPCHIP_SIZE * j, (float)MAPCHIP_SIZE + MAPCHIP_SIZE * i, COLOR_BLACK, FALSE);
#endif
		}
	}

}



//----------------------------------------------------------------------
//! @brief マウスを用いた照準処理
//!
//! @param[in] なし
//!
//! @return なし
//----------------------------------------------------------------------
void MouseRanging(void)
{
	//ローカル変数の宣言------------------------------------------------



	//ローカル変数の初期化----------------------------------------------



	//処理の内容--------------------------------------------------------
	//マウスのマウスカーソルの位置を取得する
	GetMousePoint(&g_mousePosX, &g_mousePosY);

}



//----------------------------------------------------------------------
//! @brief 戦車の描画処理
//!
//! @param[body] 描画したい戦車の車体
//! @param[bttery] 描画したい戦車の砲台
//!
//! @return なし
//----------------------------------------------------------------------
void DrawTank(GameObject body, GameObject battery)
{
	//ローカル変数の宣言------------------------------------------------



	//ローカル変数の初期化----------------------------------------------



	//処理の内容--------------------------------------------------------
	//車体の描画
	DrawRotaGraph((int)body.pos.x, (int)body.pos.y, body.image.extRate, body.image.angle, body.image.handle[0], TRUE, FALSE);

	//砲台の描画
	DrawRotaGraph((int)battery.pos.x, (int)battery.pos.y, battery.image.extRate, battery.image.angle, battery.image.handle[0], TRUE, FALSE);

}



//----------------------------------------------------------------------
//! @brief 照準に合わせた画像の回転処理
//!
//! @param[in] なし
//!
//! @return なし
//----------------------------------------------------------------------
void RotaGraph(void)
{
	//ローカル変数の宣言------------------------------------------------
	double dx;
	double dy;


	//ローカル変数の初期化----------------------------------------------
	dx = g_playerBattery.pos.x - g_mousePosX;
	dy = g_playerBattery.pos.y - g_mousePosY;


	//処理の内容--------------------------------------------------------
	g_playerBattery.image.angle = atan2(dy, dx);

}



//----------------------------------------------------------------------
//! @brief プレイヤーの移動処理
//!
//! @param[in] なし
//!
//! @return なし
//----------------------------------------------------------------------
void MovePlayer(void)
{
	//ローカル変数の宣言------------------------------------------------
	//入力デバイスの状態
	int keyState;

	double dx;
	double dy;


	//ローカル変数の初期化----------------------------------------------
	keyState = GetJoypadInputState(DX_INPUT_KEY_PAD1);


	//処理の内容--------------------------------------------------------
	// Wキー入力
	if (keyState & PAD_INPUT_8)
	{
		g_playerBody.vel.y = -PLAYER_SPEED;

	}
	// Sキー入力
	if (keyState & PAD_INPUT_5)
	{
		g_playerBody.vel.y = PLAYER_SPEED;

	}
	// Dキー入力
	if (keyState & PAD_INPUT_6)
	{
		g_playerBody.vel.x = PLAYER_SPEED;

	}
	// Aキー入力
	if (keyState & PAD_INPUT_4)
	{
		g_playerBody.vel.x = -PLAYER_SPEED;
	}

	//減速
	g_playerBody.vel.x = g_playerBody.vel.x * FRICTION;
	g_playerBody.vel.y = g_playerBody.vel.y * FRICTION;

	//等速化
	if ((g_playerBody.vel.x > 1 || g_playerBody.vel.x < -1) && (g_playerBody.vel.y > 1 || g_playerBody.vel.y < -1))
	{
		g_playerBody.vel.x /= ROOT2;
		g_playerBody.vel.y /= ROOT2;
	}

	//車体の回転
	dx = g_playerBody.vel.x;
	dy = g_playerBody.vel.y;
	g_playerBody.image.angle = atan2(dy, dx);
	

	//座標の更新
	g_playerBody.pos.x += g_playerBody.vel.x;
	g_playerBody.pos.y += g_playerBody.vel.y;

	//砲台の位置を車体の位置に合わせる
	g_playerBattery.pos = g_playerBody.pos;

}


//----------------------------------------------------------------------
//! @brief マップ上の移動制限の処理
//!
//! @param[name]制限をかけたいオブジェクト
//!
//! @return なし
//----------------------------------------------------------------------
int LimitMapArea(GameObject *name)
{
	//ローカル変数の宣言------------------------------------------------
	int mapT;
	int mapB;
	int mapL;
	int mapR;

	//ローカル変数の初期化----------------------------------------------
	mapT = (int)((name->pos.y - 16) / MAPCHIP_SIZE);
	mapB = (int)((name->pos.y + 16) / MAPCHIP_SIZE);
	mapL = (int)((name->pos.x - 16) / MAPCHIP_SIZE);
	mapR = (int)((name->pos.x + 16) / MAPCHIP_SIZE);

	//処理の内容--------------------------------------------------------
	//左上
	if (g_mapData[mapT][mapL] != 0)
	{
		//座標を戻す
		name->pos.x -= name->vel.x;
		name->pos.y -= name->vel.y;
		return 1;
	}
	//左下
	if (g_mapData[mapB][mapL] != 0)
	{
		//座標を戻す
		name->pos.x -= name->vel.x;
		name->pos.y -= name->vel.y;
		return 1;
	}
	//右上
	if (g_mapData[mapT][mapR] != 0)
	{
		//座標を戻す
		name->pos.x -= name->vel.x;
		name->pos.y -= name->vel.y;
		return 1;
	}
	//右下
	if (g_mapData[mapB][mapR] != 0)
	{
		//座標を戻す
		name->pos.x -= name->vel.x;
		name->pos.y -= name->vel.y;
		return 1;
	}

	return 0;
}



//----------------------------------------------------------------------
//! @brief プレイヤーの弾の発射処理
//!
//! @param[in] なし
//!
//! @return なし
//----------------------------------------------------------------------
void PlayerShootBullet(void)
{
	//ローカル変数の宣言------------------------------------------------
	int i;	//配列に使用


	//ローカル変数の初期化----------------------------------------------


	//処理の内容--------------------------------------------------------
	g_lastInputMouseState = ~g_currentInputMouseState;	//前回のマウスの入力状態の記憶
	g_currentInputMouseState = GetMouseInput();	//マウスの入力状態の取得

	//マウスの左キーが押される
	if (((g_lastInputMouseState&g_currentInputMouseState) & MOUSE_INPUT_LEFT) != 0)
	{
		//弾を調べる
		for (i = 0; i < MAX_PLAYER_BULLETS; i++)
		{
			if (g_playerBullets[i].state == 0)
			{
				g_playerBullets[i].pos = g_playerBattery.pos;	//砲台の座標に合わせる
				g_playerBullets[i].vel.x = -PLAYER_BULLET_SPEED*(float)cos(g_playerBattery.image.angle);	//速度を与える
				g_playerBullets[i].vel.y = -PLAYER_BULLET_SPEED*(float)sin(g_playerBattery.image.angle);
				g_playerBullets[i].image.angle = g_playerBattery.image.angle;	//画像の角度を合わせる
				g_playerBullets[i].state = 1;	//弾を使用中にする
				g_playerBullets[i].hp = 2;	//弾のhpを回復させる

				//効果音の再生
				PlaySoundMem(g_effectSound[2], DX_PLAYTYPE_BACK, TRUE);
				break;
			}
		}
	}


}



//----------------------------------------------------------------------
//! @brief 発射された弾の移動処理
//!
//! @param[bullet] 弾
//!
//! @return なし
//----------------------------------------------------------------------
void MoveBullet(GameObject *bullet)
{
	//ローカル変数の宣言------------------------------------------------



	//ローカル変数の初期化----------------------------------------------



	//処理の内容--------------------------------------------------------
	//座標の更新
	bullet->pos.x += bullet->vel.x;
	bullet->pos.y += bullet->vel.y;

}



//----------------------------------------------------------------------
//! @brief 弾の移動制限の処理
//!
//! @param[bullet] 弾
//!
//! @return なし
//----------------------------------------------------------------------
void LimitBulletAria(GameObject *bullet)
{
	//ローカル変数の宣言------------------------------------------------
	int mapX;
	int mapY;

	int result;
	int resultT;
	int resultB;
	int resultL;
	int resultR;

	float afterPosX;
	float afterPosY;

	//ローカル変数の初期化----------------------------------------------




	//処理の内容--------------------------------------------------------
	//次の移動先の座標を求める
	afterPosX = bullet->pos.x + bullet->vel.x;
	afterPosY = bullet->pos.y + bullet->vel.y;

	//座標をマップに変換
	mapX = (int)(afterPosX / MAPCHIP_SIZE);	
	mapY = (int)(afterPosY / MAPCHIP_SIZE);

	//当たり判定
	result= DetectCollisionMapChip(bullet, mapX, mapY);
	resultT = DetectCollisionMapChip(bullet, mapX, mapY - 1);
	resultB = DetectCollisionMapChip(bullet, mapX, mapY + 1);
	resultL = DetectCollisionMapChip(bullet, mapX - 1, mapY);
	resultR = DetectCollisionMapChip(bullet, mapX + 1, mapY);

	if (resultT == 1)
	{
		//弾のhpを減らす
		bullet->hp--;
	}
	else if (resultB == 1)
	{
		//弾のhpを減らす
		bullet->hp--;
	}
	else if (resultL == 1)
	{
		//弾のhpを減らす
		bullet->hp--;
	}
	else if (resultR == 1)
	{
		//弾のhpを減らす
		bullet->hp--;
	}
	else if (result == 1)
	{
		//弾のhpを減らす
		bullet->hp--;
	}

	//画像の角度を変える
	bullet->image.angle = atan2(bullet->vel.y, bullet->vel.x);

	//弾の消滅
	if (bullet->hp <= 0)
	{
		//未使用にする
		bullet->state = 0;
	}

}



//----------------------------------------------------------------------
//! @brief 線分の交差判定の処理
//!
//! @param[p1x] 線分aの始点のx座標
//! @param[p1y] 線分aの始点のy座標
//! @param[p2x] 線分aの終点のx座標
//! @param[p2y] 線分aの終点のy座標
//! @param[p3x] 線分bの始点のx座標
//! @param[p3y] 線分bの始点のy座標
//! @param[p4x] 線分bの終点のx座標
//! @param[p4y] 線分bの終点のy座標
//!
//! @return 1 交差している
//! @return 0 交差していない
//----------------------------------------------------------------------
int Intersection(float p1x, float p1y, float p2x, float p2y, float p3x, float p3y, float p4x, float p4y)
{
	//ローカル変数の宣言------------------------------------------------
	float a;
	float b;
	float c;
	float d;

	//ローカル変数の初期化----------------------------------------------
	a = (p1x - p2x)*(p3y - p1y) + (p1y - p2y)*(p1x - p3x);
	b = (p1x - p2x)*(p4y - p1y) + (p1y - p2y)*(p1x - p4x);
	c = (p3x - p4x)*(p1y - p3y) + (p3y - p4y)*(p3x - p1x);
	d = (p3x - p4x)*(p2y - p3y) + (p3y - p4y)*(p3x - p2x);

	//処理の内容--------------------------------------------------------
	if (a*b < 0)
	{
		if (c*d < 0)
		{
			return 1;	//交差している
		}
	}

	return 0;	//交差していない

}



//----------------------------------------------------------------------
//! @brief マップチップとの当たり判定の処理
//!
//! @param[bullet] 弾
//! @param[mapX] マップチップのX
//! @param[mapY] マップチップのY
//!
//! @return なし
//----------------------------------------------------------------------
int DetectCollisionMapChip(GameObject *bullet, int mapX, int mapY)
{
	//ローカル変数の宣言------------------------------------------------
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

	//ローカル変数の初期化----------------------------------------------
	afterPosX = bullet->pos.x + bullet->vel.x;
	afterPosY = bullet->pos.y + bullet->vel.y;



	//処理の内容--------------------------------------------------------

	//移動可能か調べる
	if (g_mapData[mapY][mapX] == 1)
	{
		//マップチップの上下左右の座標を求める
		left = (float)(mapX*MAPCHIP_SIZE);
		right = (float)(mapX*MAPCHIP_SIZE + MAPCHIP_SIZE);
		top = (float)(mapY*MAPCHIP_SIZE);
		bottom = (float)(mapY*MAPCHIP_SIZE + MAPCHIP_SIZE);

		//当たり判定処理
		//上辺
		resultT = Intersection(bullet->pos.x, bullet->pos.y, afterPosX, afterPosY, left, top, right, top);
		//下辺
		resultB = Intersection(bullet->pos.x, bullet->pos.y, afterPosX, afterPosY, left, bottom, right, bottom);
		//左辺
		resultL = Intersection(bullet->pos.x, bullet->pos.y, afterPosX, afterPosY, left, top, left, bottom);
		//右辺
		resultR = Intersection(bullet->pos.x, bullet->pos.y, afterPosX, afterPosY, right, top, right, bottom);

		//上辺の判定を行う
		if ((resultT == 1) && (bullet->pos.y < top))
		{
			//y成分の向きを変える
			bullet->vel.y *= -1;

			//効果音の再生
			PlaySoundMem(g_effectSound[3], DX_PLAYTYPE_BACK, TRUE);

			//処理を終える
			return 1;
		}

		//下辺の判定を行う
		if ((resultB == 1) && (bullet->pos.y > bottom))
		{
			//y成分の向きを変える
			bullet->vel.y *= -1;

			//効果音の再生
			PlaySoundMem(g_effectSound[3], DX_PLAYTYPE_BACK, TRUE);

			//処理を終える
			return 1;
		}

		//左の判定を行う
		if ((resultL == 1) && (bullet->pos.x < left))
		{
			//x成分の向きを変える
			bullet->vel.x *= -1;

			//効果音の再生
			PlaySoundMem(g_effectSound[3], DX_PLAYTYPE_BACK, TRUE);

			//処理を終える
			return 1;
		}

		//右の判定を行う
		if ((resultR == 1) && (bullet->pos.x > right))
		{
			//x成分の向きを変える
			bullet->vel.x *= -1;

			//効果音の再生
			PlaySoundMem(g_effectSound[3], DX_PLAYTYPE_BACK, TRUE);

			//処理を終える
			return 1;
		}
	}

	//どこにも当たっていない
	return 0;

}



//----------------------------------------------------------------------
//! @brief 敵の移動処理
//!
//! @param[in] なし
//!
//! @return なし
//----------------------------------------------------------------------
void MoveEnemy(void)
{
	//ローカル変数の宣言------------------------------------------------
	int i;	//配列に使用
	int j;	//配列に使用

	double dx;
	double dy;

	//衝突判定の結果
	int resultM;	//マップ
	int resultP;	//プレイヤー
	int resultE;	//敵

	//ローカル変数の初期化----------------------------------------------
	resultM = 0;
	resultP = 0;
	resultE = 0;


	//処理の内容--------------------------------------------------------
	//敵を調べる
	for (i = 0; i < MAX_ENEMIES; i++)
	{
		if (g_enemyBody[i].state == 1)
		{
			//速度の初期化
			g_enemyBody[i].vel.x = 0.0f;
			g_enemyBody[i].vel.y = 0.0f;

			switch (g_enemyBody[i].moveType)
			{
			case 1:
				//上
				MoveUp(&g_enemyBody[i]);
				break;
			case 2:
				//下
				MoveDown(&g_enemyBody[i]);
				break;
			case 3:
				//左
				MoveLeft(&g_enemyBody[i]);
				break;
			case 4:
				//右
				MoveRight(&g_enemyBody[i]);
				break;
			case 5:
				//左上
				MoveUp(&g_enemyBody[i]);
				MoveLeft(&g_enemyBody[i]);
				break;
			case 6:
				//右上
				MoveUp(&g_enemyBody[i]);
				MoveRight(&g_enemyBody[i]);
				break;
			case 7:
				//左下
				MoveDown(&g_enemyBody[i]);
				MoveLeft(&g_enemyBody[i]);
				break;
			case 8:
				//右下
				MoveDown(&g_enemyBody[i]);
				MoveRight(&g_enemyBody[i]);
				break;
			default:
				break;
			}

			//減速
			g_enemyBody[i].vel.x = g_enemyBody[i].vel.x * FRICTION;
			g_enemyBody[i].vel.y = g_enemyBody[i].vel.y * FRICTION;

			//等速化
			if ((g_enemyBody[i].vel.x > 1 || g_enemyBody[i].vel.x < -1) && (g_enemyBody[i].vel.y > 1 || g_enemyBody[i].vel.y < -1))
			{
				g_enemyBody[i].vel.x /= ROOT2;
				g_enemyBody[i].vel.y /= ROOT2;
			}

			//車体の回転
			dx = g_enemyBody[i].vel.x;
			dy = g_enemyBody[i].vel.y;
			g_enemyBody[i].image.angle = atan2(dy, dx);


			//座標の更新
			g_enemyBody[i].pos.x += g_enemyBody[i].vel.x;
			g_enemyBody[i].pos.y += g_enemyBody[i].vel.y;

			//砲台の位置を車体の位置に合わせる
			g_enemyBattery[i].pos = g_enemyBody[i].pos;

			//敵の移動制限
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
			 
			//移動時間のカウント
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
//! @brief 上移動の処理
//!
//! @param[name] 敵
//!
//! @return なし
//----------------------------------------------------------------------
void MoveUp(GameObject *name)
{
	//速度の代入
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
//! @brief 下移動の処理
//!
//! @param[name] 敵
//!
//! @return なし
//----------------------------------------------------------------------
void MoveDown(GameObject *name)
{
	//速度の代入
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
//! @brief 左移動の処理
//!
//! @param[name] 敵
//!
//! @return なし
//----------------------------------------------------------------------
void MoveLeft(GameObject *name)
{
	//速度の代入
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
//! @brief 右移動の処理
//!
//! @param[name] 敵
//!
//! @return なし
//----------------------------------------------------------------------
void MoveRight(GameObject *name)
{
	//速度の代入
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
//! @brief 敵の弾の発射処理
//!
//! @param[in] なし
//!
//! @return なし
//----------------------------------------------------------------------
void EnemyShootBullet(void)
{
	//ローカル変数の宣言------------------------------------------------
	int i;	//配列に使用
	int j;	//配列に使用
	int k;	//配列に使用
	int l;	//配列に使用

	int result;	//予測結果に使用
	int resultCount;

	float dx;
	float dy;

	
	//ローカル変数の初期化----------------------------------------------
	result = 1;
	resultCount = 0;

	//処理の内容--------------------------------------------------------

	for (i = 0; i < MAX_ENEMIES; i++)
	{
		if (g_enemyBattery[i].state == 1)
		{
			//砲台の向きを合わせる
			dx = g_enemyBattery[i].pos.x - g_playerBody.pos.x;
			dy = g_enemyBattery[i].pos.y - g_playerBody.pos.y;
			
			g_enemyBattery[i].image.angle = atan2(dy, dx);

			if (g_enemyBattery[i].bulletInterval <= 0)
			{
				for (j = 0; j < MAP_HEIGHT; j++)
				{
					for (k = 0; k < MAP_WIDTH; k++)
					{
						//射線上に障害物があるか調べる
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
							
							//効果音の再生
							PlaySoundMem(g_effectSound[2], DX_PLAYTYPE_BACK, TRUE);

							break;
						}
					}
				}
			}

			//次弾発射までのカウント
			g_enemyBattery[i].bulletInterval--;
		}
	}



}



//----------------------------------------------------------------------
//! @brief 方向弾の発射処理
//!
//! @param[shooter] 射手
//! @param[bullet] 撃たれる弾
//! @param[target] 目標
//!
//! @return なし
//----------------------------------------------------------------------
void ShootWayBullet(GameObject *shooter, GameObject *bullet, GameObject *target)
{
	//ローカル変数の宣言////////////////////////////////////////////////
	double dx;
	double dy;
	double angle;

	//ローカル変数の初期化//////////////////////////////////////////////
	dx = (double)(target->pos.x - shooter->pos.x);	//目標のx座標 - 射手のx座標
	dy = (double)(target->pos.y - shooter->pos.y);	//目標のy座標 - 射手のy座標

	angle = atan2(dy, dx);	//目標と射手の角度の計算

	//処理の内容////////////////////////////////////////////////////////
	//発射位置の修正
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
//! @brief 弾との当たり判定処理
//!
//! @param[main] 当たり判定を行うオブジェクト
//! @param[partner] 当たり判定を行うオブジェクト
//!
//! @return なし
//----------------------------------------------------------------------
int DetectCollisionBullet(GameObject *main, GameObject *partner)
{
	//ローカル変数の宣言------------------------------------------------
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

	//ローカル変数の初期化----------------------------------------------
	afterPosX = partner->pos.x + partner->vel.x;
	afterPosY = partner->pos.y + partner->vel.y;



	//処理の内容--------------------------------------------------------

	//マップチップの上下左右の座標を求める
	left = (float)(main->pos.x - 16);
	right = (float)(main->pos.x + 16);
	top = (float)(main->pos.y - 16);
	bottom = (float)(main->pos.y + 16);

	//当たり判定処理
	//上辺
	resultT = Intersection(partner->pos.x,partner->pos.y, afterPosX, afterPosY, left, top, right, top);
	//下辺
	resultB = Intersection(partner->pos.x, partner->pos.y, afterPosX, afterPosY, left, bottom, right, bottom);
	//左辺
	resultL = Intersection(partner->pos.x,partner->pos.y, afterPosX, afterPosY, left, top, left, bottom);
	//右辺
	resultR = Intersection(partner->pos.x,partner->pos.y, afterPosX, afterPosY, right, top, right, bottom);
	
	//上辺の判定を行う
	if ((resultT == 1) && (partner->pos.y < top))
	{
		//処理を終える
		return 1;
	}
	
	//下辺の判定を行う
	if ((resultB == 1) && (partner->pos.y > bottom))
	{
		//処理を終える
		return 1;
	}

	//左の判定を行う
	if ((resultL == 1) && (partner->pos.x < left))
	{
		//処理を終える
		return 1;
	}

	//右の判定を行う
	if ((resultR == 1) && (partner->pos.x > right))
	{
		//処理を終える
		return 1;
	}
	
	//どこにも当たっていない
	return 0;

}



//----------------------------------------------------------------------
//! @brief 弾道予測の処理
//!
//! @param[shooter] 射手
//! @param[target] ターゲット
//! @param[mapX] マップチップのX
//! @param[mapY] マップチップのY
//!
//! @return なし
//----------------------------------------------------------------------
int ForecastTrajectory(GameObject *shooter, GameObject *target, int mapX, int mapY)
{
	//ローカル変数の宣言------------------------------------------------
	int resultT;
	int resultB;
	int resultL;
	int resultR;

	float left;
	float right;
	float top;
	float bottom;

	//ローカル変数の初期化----------------------------------------------

	


	//処理の内容--------------------------------------------------------

	if (g_mapData[mapY][mapX] == 1)
	{
		//マップチップの上下左右の座標を求める
		left = (float)(mapX*MAPCHIP_SIZE);
		right = (float)(mapX*MAPCHIP_SIZE + MAPCHIP_SIZE);
		top = (float)(mapY*MAPCHIP_SIZE);
		bottom = (float)(mapY*MAPCHIP_SIZE + MAPCHIP_SIZE);

		//当たり判定処理
		//上辺
		resultT = Intersection(shooter->pos.x, shooter->pos.y, target->pos.x, target->pos.y, left, top, right, top);
		//下辺
		resultB = Intersection(shooter->pos.x, shooter->pos.y, target->pos.x, target->pos.y, left, bottom, right, bottom);
		//左辺
		resultL = Intersection(shooter->pos.x, shooter->pos.y, target->pos.x, target->pos.y, left, top, left, bottom);
		//右辺
		resultR = Intersection(shooter->pos.x, shooter->pos.y, target->pos.x, target->pos.y, right, top, right, bottom);

		//上辺の判定を行う
		if (resultT == 1)
		{
			//処理を終える
			return 1;
		}

		//下辺の判定を行う
		if (resultB == 1)
		{
			//処理を終える
			return 1;
		}

		//左の判定を行う
		if (resultL == 1)
		{
			//処理を終える
			return 1;
		}

		//右の判定を行う
		if (resultR == 1)
		{
			//処理を終える
			return 1;
		}
	}

	//間に何もない
	return 0;

}



//----------------------------------------------------------------------
//! @brief 爆発のアニメーション処理
//!
//! @param[name] オブジェクト名
//!
//! @return なし
//----------------------------------------------------------------------
void EffectExplosion(Effect *name)
{
	//ローカル変数の宣言//----------------------------------------------
	


	//ローカル変数の初期化//--------------------------------------------



	//処理の内容//------------------------------------------------------
	//描画時間をはかる
	name->anime.elapsedTime++;
	//現在の描画時間が終わった
	if (name->anime.elapsedTime > name->anime.frameDuration)
	{
		//次のフレームに移動
		name->anime.frameNum++;
		//描画時間のリセット
		name->anime.elapsedTime = 0;

		//全ての画像を描画する
		if (name->anime.frameNum == name->anime.numFrame)
		{
			//未使用にする
			name->state = 0;
		}
	}
	
}