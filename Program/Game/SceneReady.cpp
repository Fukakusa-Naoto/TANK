//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   SceneReady.cpp
//!
//! @brief  プレイ前準備シーンのソースファイル
//!
//! @date   日付
//!
//! @author 制作者名
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// ヘッダファイルの読み込み ================================================
#include "GameMain.h"
#include "SceneReady.h"


//データ型の定義============================================================





//定数の定義================================================================

//マップの最大数
#define MAX_MAP (100)

//マップの幅
#define MAP_WIDTH (20)

//マップの高さ
#define MAP_HEIGHT (15)


// グローバル変数の定義 ====================================================

//マップデータ
int g_mapData[MAP_HEIGHT][MAP_WIDTH];
//位置データ
int g_posData[MAP_HEIGHT][MAP_WIDTH];
//ステージ
extern int g_stageNumber;

//画像
HGRP g_readyImage[10];

//数字
HGRP g_readyNumber[16];


//関数の宣言================================================================

//マップデータの読み込み
void LoadMap(char *mapFile, int mapWidth, int mapHeight);

//位置データの読み込み
void LoadPosition(char *posFile, int mapWidth, int mapHeight);


// 関数の定義 ==============================================================
//----------------------------------------------------------------------
//! @brief プレイ前準備の初期化処理
//!
//! @param[in] なし
//!
//! @return なし
//----------------------------------------------------------------------
void InitializeReady(void)
{
	//ローカル変数の宣言//----------------------------------------------

	//読み込むマップファイル
	char *mapFile[MAX_MAP];

	//読み込む位置ファイル
	char *posFile[MAX_MAP];

	//ローカル変数の初期化//--------------------------------------------

	//マップファイルの読み込み
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

	//位置ファイルの読み込み
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

	//処理の内容//------------------------------------------------------

	//マップ
	LoadMap(mapFile[g_stageNumber], MAP_WIDTH, MAP_HEIGHT);
	//位置
	LoadPosition(posFile[g_stageNumber], MAP_WIDTH, MAP_HEIGHT);

	//画像の読み込み
	g_readyImage[0] = LoadGraph("Resources/Images/Ready.png");
	g_readyImage[1] = LoadGraph("Resources/Images/ステージ.png");

	//数字画像の読み込み
	LoadDivGraph("Resources/Images/stageNumber.png", 16, 4, 4, 83, 83, g_readyNumber);

}



//----------------------------------------------------------------------
//! @brief プレイ前準備の更新処理
//!
//! @param[in] なし
//!
//! @return なし
//----------------------------------------------------------------------
void UpdateReady(void)
{
	//ローカル変数の宣言//----------------------------------------------
	static int timer = 0;


	//ローカル変数の初期化//--------------------------------------------



	//処理の内容//------------------------------------------------------
	timer++;

	if (timer == 180)
	{
		timer = 0;
		//シーンの切り替え
		ChangeScene(SCENE_PLAYING);	//プレイシーンへ
	}
}



//----------------------------------------------------------------------
//! @brief プレイ前準備の描画処理
//!
//! @param[in] なし
//!
//! @return なし
//----------------------------------------------------------------------
void RenderReady(void)
{
	//ローカル変数の宣言//----------------------------------------------
	


	//ローカル変数の初期化//--------------------------------------------



	//処理の内容//------------------------------------------------------

	//背景の描画
	DrawGraph(0, 0, g_readyImage[0], TRUE);

	//文字の描画
	DrawGraph(150, 100, g_readyImage[1], TRUE);

	//数字の描画
	//十の桁
	DrawGraph(360, 105, g_readyNumber[g_stageNumber / 10], TRUE);
	//一の桁
	DrawGraph(400, 107, g_readyNumber[g_stageNumber % 10], TRUE);
}



//----------------------------------------------------------------------
//! @brief プレイ前準備の終了処理
//!
//! @param[in] なし
//!
//! @return なし
//----------------------------------------------------------------------
void FinalizeReady(void)
{
	//ローカル変数の宣言//----------------------------------------------
	


	//ローカル変数の初期化//--------------------------------------------



	//処理の内容//------------------------------------------------------
	//画像データの削除
	InitGraph();


}



//*---------------------------------------------------------------------
//* マップデータの読み込み
//*
//* @param [*mapFile] マップファイル名
//* @param [mapWidth] マップの幅
//* @param [mapHeight] マップの高さ
//*
//* @return なし
//*---------------------------------------------------------------------
void LoadMap(char *mapFile, int mapWidth, int mapHeight)
{
	//ローカル変数の宣言--------------------------------
	// ファイル構造体
	FILE *fp;
	char buf[512];

	int i;	//配列に使用
	int j;	//配列に使用

			//ローカル変数の初期化------------------------------
	fp = fopen(mapFile, "r");

	i = 0;
	j = 0;
	//処理の内容----------------------------------------
	if (fp == NULL)
	{
		fprintf(stderr, "ファイルがオープンできません\n");
		// 強制終了
		exit(1);
	}

	while (fgets(buf, 512, fp) != NULL)
	{
		for (i = 0; i < mapWidth; i++)
		{
			if ((buf[i] >= '0') && (buf[i] <= '9'))	//1～9の文字である
			{
				g_mapData[j][i] = buf[i] - 0x30;	//文字を数字に変換
			}
		}
		j++;
	}

	fclose(fp);

}



//*---------------------------------------------------------------------
//* 位置データの読み込み
//*
//* @param [*mapFile] 位置ファイル名
//* @param [mapWidth] マップの幅
//* @param [mapHeight] マップの高さ
//*
//* @return なし
//*---------------------------------------------------------------------
void LoadPosition(char *posFile, int mapWidth, int mapHeight)
{
	//ローカル変数の宣言--------------------------------
	// ファイル構造体
	FILE *fp;
	char buf[512];

	int i;	//配列に使用
	int j;	//配列に使用

			//ローカル変数の初期化------------------------------
	fp = fopen(posFile, "r");

	i = 0;
	j = 0;
	//処理の内容----------------------------------------
	if (fp == NULL)
	{
		fprintf(stderr, "ファイルがオープンできません\n");
		// 強制終了
		exit(1);
	}

	while (fgets(buf, 512, fp) != NULL)
	{
		for (i = 0; i < mapWidth; i++)
		{
			if ((buf[i] >= '0') && (buf[i] <= '9'))	//1～9の文字である
			{
				g_posData[j][i] = buf[i] - 0x30;	//文字を数字に変換
			}
		}
		j++;
	}

	fclose(fp);

}
