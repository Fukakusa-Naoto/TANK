//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   SceneTitle.cpp
//!
//! @brief  タイトルシーン関連のソースファイル
//!
//! @date   
//!
//! @author 深草直斗
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// ヘッダファイルの読み込み ================================================
#include "SceneTitle.h"
#include "GameMain.h"


//定数の定義================================================================





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

//関数のプロトタイプ宣言====================================================




// グローバル変数の定義 ====================================================

//画像
Title g_titleImage[10];

//ステージ
int g_stageNumber;

//スコア
Score g_score;


// 関数の定義 ==============================================================
//----------------------------------------------------------------------
//! @brief ゲームの初期化処理
//!
//! @param[in] なし
//!
//! @return なし
//----------------------------------------------------------------------
void InitializeTitle(void)
{
	//ローカル変数の宣言------------------------------------------------



	//ローカル変数の初期化----------------------------------------------



	//処理の内容--------------------------------------------------------

	g_titleImage[0].pos.x = SCREEN_CENTER_X;
	g_titleImage[0].pos.y = SCREEN_CENTER_Y;
	g_titleImage[0].image.angle = 0.0;
	g_titleImage[0].image.extRate = 1.0;
	g_titleImage[0].image.handle = LoadGraph("Resources/Images/はじめる.png");

	g_titleImage[1].pos.x = SCREEN_CENTER_X;
	g_titleImage[1].pos.y = SCREEN_CENTER_Y;
	g_titleImage[1].image.angle = 0.0;
	g_titleImage[1].image.extRate = 1.0;
	g_titleImage[1].image.handle = LoadGraph("Resources/Images/おわる.png");

	g_titleImage[2].pos.x = 0.0f;
	g_titleImage[2].pos.y = 0.0f;
	g_titleImage[2].image.angle = 0.0;
	g_titleImage[2].image.extRate = 1.0;
	g_titleImage[2].image.handle = LoadGraph("Resources/Images/title.png");

}



//----------------------------------------------------------------------
//! @brief ゲームの更新処理
//!
//! @param[in] なし
//!
//! @return なし
//----------------------------------------------------------------------
void UpdateTitle(void)
{
	//ローカル変数の宣言------------------------------------------------
	static int select = 0;


	//ローカル変数の初期化----------------------------------------------



	//処理の内容--------------------------------------------------------

	//キー操作
	if (((g_lastInputState&g_currentInputState)&PAD_INPUT_8) != 0)	//Wキーが押される
	{
		select = 0;	//'はじめる'
	}
	if (((g_lastInputState&g_currentInputState)&PAD_INPUT_5) != 0)	//Sキーが押される
	{
		select = 1;	//'おわる'
	}

	//'はじめる'が選択されているとき
	if (select == 0)
	{
		g_titleImage[0].image.extRate = 1.0;
		g_titleImage[1].image.extRate = 0.8;

		if (((g_lastInputState&g_currentInputState)&PAD_INPUT_10) != 0)	//スペースキーが押される
		{
			//ステージを初期化
			g_stageNumber = 1;

			//スコアの初期化
			g_score.type1 = 0;
			g_score.type2 = 0; 
			g_score.type3 = 0;
			g_score.type4 = 0;
			g_score.type5 = 0;
			g_score.type6 = 0;
			g_score.type7 = 0;

			//次のシーンへ
			ChangeScene(SCENE_READY);	//プレイ前準備へ
		}
	}

	//'おわる'が選択されているとき
	if (select == 1)
	{
		g_titleImage[0].image.extRate = 0.8;
		g_titleImage[1].image.extRate = 1.0;

		if (((g_lastInputState&g_currentInputState)&PAD_INPUT_10) != 0)	//スペースキーが押される
		{
			ExitGame();	//ゲームを終了する
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
void RenderTitle(void)
{
	//ローカル変数の宣言------------------------------------------------



	//ローカル変数の初期化----------------------------------------------



	//処理の内容--------------------------------------------------------

	//背景の描画
	DrawGraph((int)g_titleImage[2].pos.x, (int)g_titleImage[2].pos.y, g_titleImage[2].image.handle, TRUE);

	//'はじめる'の描画
	DrawRotaGraph((int)g_titleImage[0].pos.x, (int)g_titleImage[0].pos.y+80, g_titleImage[0].image.extRate, g_titleImage[0].image.angle, g_titleImage[0].image.handle, TRUE, FALSE);

	//'おわる'の描画
	DrawRotaGraph((int)g_titleImage[1].pos.x, (int)g_titleImage[1].pos.y+130, g_titleImage[1].image.extRate, g_titleImage[1].image.angle, g_titleImage[1].image.handle, TRUE, FALSE);

}



//----------------------------------------------------------------------
//! @brief ゲームの終了処理
//!
//! @param[in] なし
//!
//! @return なし
//----------------------------------------------------------------------
void FinalizeTitle(void)
{
	//ローカル変数の宣言------------------------------------------------



	//ローカル変数の初期化----------------------------------------------



	//処理の内容--------------------------------------------------------
	//画像データの削除
	InitGraph();


}
